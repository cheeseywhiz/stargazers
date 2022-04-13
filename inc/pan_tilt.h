/*
 * pan_and_tilt.h
 *
 *  Created on: Apr 9, 2022
 *      Author: broml
 */

#ifndef INC_PAN_TILT_H_
#define INC_PAN_TILT_H_

#include "servo.h"
#include "stepper.h"
#include "imu.h"

#define NUM_SAMPLES_FOR_AVERAGE 200

typedef struct PanTilt PanTilt;

struct PanTilt{
	Servo * tilt_servo;
	Stepper * pan_stepper;
	IMU * imu;
	float alpha;
	float beta;
	float gamma;
};

void pan_tilt_init(PanTilt * pan_tilt, Stepper * pan_stepper, Servo * tilt_servo, IMU * imu);

void calibrate_pan_tilt(PanTilt * pan_tilt);

void write_pan_tilt(PanTilt * pan_tilt, int16_t altitude, int16_t azimuth, int16_t declination);

float _toggle_altitude_phi(float phase);

float _toggle_azimuth_theta(float phase);

#endif /* INC_PAN_TILT_H_ */
