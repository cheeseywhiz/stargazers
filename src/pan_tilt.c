/*
 * pan_and_tilt.c
 *
 *  Created on: Apr 9, 2022
 *      Author: broml
 */

#include <pan_tilt.h>
#include "math.h"
#include "vector.h"
#include "stdio.h"

void pan_tilt_init(PanTilt * pan_tilt, Stepper * pan_stepper, Servo * tilt_servo, IMU * imu){
	pan_tilt->pan_stepper = pan_stepper;
	pan_tilt->tilt_servo = tilt_servo;
	pan_tilt->imu = imu;
	pan_tilt->alpha = NAN;
	pan_tilt->beta = NAN;
	pan_tilt->gamma = NAN;
}

void calculate_orientation(PanTilt * pan_tilt){

	// reset euler angles
	pan_tilt->alpha = NAN;
	pan_tilt->beta = NAN;
	pan_tilt->gamma = NAN;

	// calculate euler angles
	while(!euler_angle_success(pan_tilt->alpha, pan_tilt->beta, pan_tilt->gamma)) {

		// print log
		printf("$log (ijbd): calculating euler angles.\n");

		// time average positions
		Vector accel = {0,0,0};
		Vector magnet = {0,0,0};

		imu_time_average(pan_tilt->imu, &accel, &magnet, (uint16_t)NUM_SAMPLES_FOR_AVERAGE);

		// get orientation vectors
		Vector up, north, west;
			// up
		copy_vector(&accel, &up);
		normalize(&up);
			// west
		cross_product(&up, &magnet, &west);
		normalize(&west);
			// north
		cross_product(&west, &up, &north);
		normalize(&north);

		// calculate euler angles
		calculate_euler_angles(&north, &west, &up, &pan_tilt->alpha, &pan_tilt->beta, &pan_tilt->gamma);

	}

	// reset stepper angle
	pan_tilt->pan_stepper->angle = 0;
}

void calibrate_pan_tilt(PanTilt * pan_tilt, uint8_t num_iterations){

	for(uint8_t iter=0; iter < num_iterations; ++iter){
		// print log
		printf("$log (ijbd): calibrating (round %d of %d).\n", iter+1, num_iterations);

		// get orientation
		calculate_orientation(pan_tilt);

		// move north
		write_pan_tilt(pan_tilt, 0, 0, 0);
	}
}

void write_pan_tilt(PanTilt * pan_tilt, int16_t altitude, int16_t azimuth, int16_t declination){
	// get desired vector
	Vector star;
	vector_from_angles(&star, _toggle_altitude_phi(deg2rad(altitude)), _toggle_azimuth_theta(deg2rad(azimuth-declination)));

	// change coordinate systems
	intrinsic_rotation(&star, pan_tilt->alpha, pan_tilt->beta, pan_tilt->gamma);

	// convert to polar
	float phi, theta;
	angles_from_vector(&star, &phi, &theta);

	// convert to motor angles
	float tilt_angle = rad2deg(_toggle_altitude_phi(phi));
	float pan_angle = rad2deg(_toggle_azimuth_theta(theta));

	// write motors in order
	write_stepper(pan_tilt->pan_stepper, pan_angle);
	write_servo(pan_tilt->tilt_servo, tilt_angle);

}

float _toggle_altitude_phi(float phase){
	return PI/2 - phase;
}

float _toggle_azimuth_theta(float phase){
	return 2*PI - phase;
}


