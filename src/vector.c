/*
 * vector.c
 *
 *  Created on: Apr 11, 2022
 *      Author: broml
 */

#include "vector.h"
#include "math.h"
#include "stdio.h"

float deg2rad(float deg){
	return deg*PI/180;
}

float rad2deg(float rad){
	return rad*180/PI;
}

void vector_from_angles(Vector * vec, float phi, float theta){
	/* Following connvetion:
	 *
	 * 'https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates'
	 */
	vec->x = sin(phi)*cos(theta);
	vec->y = sin(phi)*sin(theta);
	vec->z = cos(phi);
}

void angles_from_vector(Vector * vec, float * phi, float * theta){
	*theta = atan2(vec->y, vec->x);
	*phi = atan2(sqrt(vec->x*vec->x + vec->y*vec->y), vec->z);
}

void copy_vector(Vector * original, Vector * copy){
	copy->x = original->x;
	copy->y = original->y;
	copy->z = original->z;
}

float magnitude(Vector * vec){
	return sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

void scale(Vector * vec, float mag){
	vec->x *= mag;
	vec->y *= mag;
	vec->z *= mag;
}

void normalize(Vector * vec){
	scale(vec, 1/magnitude(vec));
}

void cross_product(Vector * a, Vector * b, Vector * c){
	c->x = a->y * b->z - a->z * b->y;
	c->y = a->z * b->x - a->x * b->z;
	c->z = a->x * b->y - a->y * b->x;
}

float dot_product(Vector * a, Vector * b){
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

void calculate_euler_angles(Vector * X, Vector * Y, Vector * Z, float * alpha, float * beta, float * gamma){
	/* Calculate proper euler angles for ZXZ rotation:
	 *
	 * https://en.wikipedia.org/wiki/Euler_angles#Proper_Euler_angles
	 */
	// special cases

	if(Z->z > .999 || Z->z < -.999){
		*alpha = 0;
		*beta = (Z->z > 0) ? 0 : PI;
		*gamma = atan2(X->y, X->x);
	}
	else{
		*alpha = atan2(Z->x, -Z->y);
		*beta = atan2(sqrt(Z->x * Z->x + Z->y * Z->y), Z->z);
		*gamma = atan2(X->z, Y->z);
	}

	if(!euler_angle_success(*alpha, *beta, *gamma)){
		printf("%1.2f, %1.2f, %1.2f\n", *alpha, *beta, *gamma);
	}
}

int euler_angle_success(float alpha, float beta, float gamma){
	return !isnan(alpha) && !isnan(beta) && !isnan(gamma);
}

void rotate(Vector * vec, Vector * axis, float theta){
	/* Rodrigues' rotation formula
	 *
	 * 'https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula'
	 *
	 * This function rotates the vector, vec,
	 * around the axis defined by the unit vector, 'axis',
	 * by radians, 'theta'
	 *
	 * v_rot = v*cos(theta) + (k cross v)*sin(theta) + k(k dot v)*(1-cos(theta)
	 *
	 */
	Vector v_cos;
	Vector k_cross_v;
	Vector k_k_v;

	// first term
	copy_vector(vec, &v_cos);
	scale(&v_cos, cos(theta));

	// second term
	cross_product(axis, vec, &k_cross_v);
	scale(&k_cross_v, sin(theta));

	// third term
	copy_vector(axis, &k_k_v);
	scale(&k_k_v, dot_product(axis, vec)*(1-cos(theta)));

	// combine
	add(&v_cos, &k_cross_v, vec);
	add(vec, &k_k_v, vec);

}

void add(Vector * a, Vector * b, Vector * c){
	c->x = a->x + b->x;
	c->y = a->y + b->y;
	c->z = a->z + b->z;
}

void intrinsic_rotation(Vector * vec, float alpha, float beta, float gamma){
	/*
	 * Intrinsic rotation by euler numbers: alpha, beta, and gamma.
	 *
	 * 'https://en.wikipedia.org/wiki/Euler_angles#Precession,_nutation_and_intrinsic_rotation'
	 *
	 */

	Vector x = {1, 0, 0};
	Vector z = {0, 0, 1};

	// first rotation around z axis
	rotate(vec, &z, alpha);
	rotate(&x, &z, alpha); // find new x

	// second rotation around new x axis;
	rotate(vec, &x, beta);
	rotate(&z, &x, beta);

	// third rotation around new z axis
	rotate(vec, &z, gamma);

}
