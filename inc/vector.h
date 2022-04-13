/*
 * vector.h
 *
 *  Created on: Apr 11, 2022
 *      Author: broml
 */

#ifndef INC_VECTOR_H_
#define INC_VECTOR_H_

#define PI 3.141592

typedef struct Vector Vector;

struct Vector{
	float x;
	float y;
	float z;
};

float deg2rad(float deg);

float rad2deg(float rad);

void vector_from_angles(Vector * vec, float phi, float theta);

void angles_from_vector(Vector * vec, float * phi, float * theta);

void copy_vector(Vector * original, Vector * copy);

float magnitude(Vector * vec);

void scale(Vector * vec, float mag);

void normalize(Vector * vec);

void cross_product(Vector * a, Vector * b, Vector * c);

float dot_product(Vector * a, Vector * b);

void add(Vector * a, Vector * b, Vector * c);

void rotate(Vector * vec, Vector * axis, float theta);

void calculate_euler_angles(Vector * X, Vector * Y, Vector * Z, float * alpha, float * beta, float * gamma);

int euler_angle_success(float alpha, float beta, float gamma);

void intrinsic_rotation(Vector * vec, float alpha, float beta, float gamma);

#endif /* INC_VECTOR_H_ */
