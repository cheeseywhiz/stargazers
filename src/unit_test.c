/*
 * unit_test.c
 *
 *  Created on: Apr 11, 2022
 *      Author: broml
 */
#include "unit_test.h"
#include "stdio.h"
#include "vector.h"
#include "pan_tilt.h"
#include "math.h"

int eq(float a, float b){
	return ((a>b) ? (a-b) : (b-a)) < .001;
}

void test_1(void){

	// setup
	printf("Running unit test 1...\t");
	int passed = 1;

	// test

	Vector a = { 1, 0, 0};
	Vector b = { 0, 1, 0};
	Vector c;

	cross_product(&a, &b, &c);

	if(!(c.x == 0 && c.y == 0 && c.z == 1)){
		passed = 0;
	}

	cross_product(&b, &a, &c);

	if(!(c.x == 0 && c.y == 0 && c.z == -1)){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
	}
}

void test_2(void){

	// setup
	printf("Running unit test 2...\t");
	int passed = 1;

	// test
	Vector X = {1, 0, 0};
	Vector Y = {0, 1, 0};
	Vector Z = {0, 0, 1};

	float alpha, beta, gamma;

	calculate_euler_angles(&X, &Y, &Z, &alpha, &beta, &gamma);
	if(!(eq(alpha, 0) && eq(beta, 0) && eq(gamma, 0))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%d, %d, %d\n", (int)(alpha*180/PI), (int)(beta*180/PI), (int)(gamma*180/PI));
	}
}

void test_3(void){

	// setup
	printf("Running unit test 3...\t");
	int passed = 1;

	// test
	Vector X = {1, 0, 0};
	Vector Y = {0, 1, 1};
	Vector Z = {0, -1, 1};
	normalize(&Y);
	normalize(&Z);

	float alpha, beta, gamma;

	calculate_euler_angles(&X, &Y, &Z, &alpha, &beta, &gamma);
	if(!(eq(alpha, 0) && eq(beta, PI/4) && eq(gamma, 0))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%d, %d, %d\n", (int)(alpha*180/PI), (int)(beta*180/PI), (int)(gamma*180/PI));
	}
}

void test_4(void){

	// setup
	printf("Running unit test 4...\t");
	int passed = 1;

	// test
	Vector X = {0, 1, 0};
	Vector Y = {-1, 0, 0};
	Vector Z = {0, 0, 1};
	normalize(&Y);
	normalize(&Z);

	float alpha, beta, gamma;

	calculate_euler_angles(&X, &Y, &Z, &alpha, &beta, &gamma);
	if(!(eq(alpha, 0) && eq(beta, 0) && eq(gamma, PI/2))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%d, %d, %d\n", (int)(alpha*180/PI), (int)(beta*180/PI), (int)(gamma*180/PI));
	}
}

void test_5(void){

	// setup
	printf("Running unit test 5...\t");
	int passed = 1;

	// test
	Vector X = {-1, 0, 0};
	Vector Y = {0, 1, 0};
	Vector Z = {0, 0, -1};
	normalize(&Y);
	normalize(&Z);

	float alpha, beta, gamma;

	calculate_euler_angles(&X, &Y, &Z, &alpha, &beta, &gamma);
	if(!(eq(alpha, 0) && eq(beta, PI) && eq(gamma, PI))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%d, %d, %d\n", (int)(alpha*180/PI), (int)(beta*180/PI), (int)(gamma*180/PI));
	}
}

void test_6(void){
	// setup
	printf("Running unit test 6...\t");
	int passed = 1;

	// test
	Vector X = {1, 1, sqrt(2)};
	Vector Y = {1, 1, -sqrt(2)};
	Vector Z;

	cross_product(&X, &Y, &Z);

	normalize(&X);
	normalize(&Y);
	normalize(&Z);

	float alpha, beta, gamma;

	calculate_euler_angles(&X, &Y, &Z, &alpha, &beta, &gamma);

	if(!(eq(alpha, -3*PI/4) && eq(beta, PI/2) && eq(gamma, 3*PI/4))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%.2f, %.2f, %.2f\n", X.x, X.y, X.z);
		printf("\t%.2f, %.2f, %.2f\n", Y.x, Y.y, Y.z);
		printf("\t%.2f, %.2f, %.2f\n", Z.x, Z.y, Z.z);
		printf("\t%d, %d, %d\n", (int)(alpha*180/PI), (int)(beta*180/PI), (int)(gamma*180/PI));
	}
}

void test_7(void){
	// setup
	printf("Running unit test 7...\t");
	int passed = 1;

	// test
	Vector X = {1, 1, sqrt(2)};
	Vector Y = {1, 1, -sqrt(2)};
	Vector Z;

	cross_product(&X, &Y, &Z);

	normalize(&X);
	normalize(&Y);
	normalize(&Z);

	float alpha, beta, gamma;

	calculate_euler_angles(&X, &Y, &Z, &alpha, &beta, &gamma);

	// get desired vector
	Vector star = {0, 0, 1};

	// rotation
	intrinsic_rotation(&star, alpha, beta, gamma);

	if(!(eq(star.x, Z.x) && eq(star.y, Z.y) && eq(star.z, Z.z))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%d, %d, %d\n", (int)(alpha*180/PI), (int)(beta*180/PI), (int)(gamma*180/PI));
		printf("\t%.2f, %.2f, %.2f\n", star.x, star.y, star.z);
	}
}

void test_8(void){

	// setup
	printf("Running unit test 8...\t");
	int passed = 1;

	// test

	Vector vec;

	// s.t. 1
	vector_from_angles(&vec, 0, PI/2);
	if(!(eq(vec.x, 0) && eq(vec.y, 0) && eq(vec.z, 1))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%.2f, %.2f, %.2f\n", vec.x, vec.y, vec.z);
	}
}

void test_9(void){

	// setup
	printf("Running unit test 9...\t");
	int passed = 1;

	// test

	Vector vec;

	// s.t. 2
	vector_from_angles(&vec, PI/2, PI/4);
	if(!(eq(vec.x,sqrt(2)/2) && eq(vec.y, sqrt(2)/2) && eq(vec.z, 0))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%.2f, %.2f, %.2f\n", vec.x, vec.y, vec.z);
	}
}

void test_10(void){

	// setup
	printf("Running unit test 10...\t");
	int passed = 1;

	// test

	if(!eq(_toggle_altitude_phi(0),PI/2)){
		passed = 0;
	}

	if(!eq(_toggle_altitude_phi(PI/2),0)){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
	}
}

void test_11(void){

	// setup
	printf("Running unit test 11...\t");
	int passed = 1;

	// test
	Vector vec = {1, 0, 0};
	Vector axis = {1, 0, 0};
	rotate(&vec, &axis, PI/2);

	if(!(eq(vec.x, 1) && eq(vec.y, 0) && eq(vec.z, 0))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%.2f, %.2f, %.2f\n", vec.x, vec.y, vec.z);
	}
}

void test_12(void){

	// setup
	printf("Running unit test 12...\t");
	int passed = 1;

	// test
	Vector vec = {1, 0, 0};
	Vector axis = {0, 0, 1};
	rotate(&vec, &axis, PI/2);

	if(!(eq(vec.x, 0) && eq(vec.y, 1) && eq(vec.z, 0))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%.2f, %.2f, %.2f\n", vec.x, vec.y, vec.z);
	}
}

void test_13(void){
	// setup
	printf("Running unit test 13...\t");
	int passed = 1;

	// test
	Vector X = {1, 1, sqrt(2)};
	Vector Y = {1, 1, -sqrt(2)};
	Vector Z;

	cross_product(&X, &Y, &Z);

	normalize(&X);
	normalize(&Y);
	normalize(&Z);

	float alpha, beta, gamma;

	calculate_euler_angles(&X, &Y, &Z, &alpha, &beta, &gamma);

	// get desired vector
	Vector star = {0, 1, 0};

	// rotation
	intrinsic_rotation(&star, alpha, beta, gamma);

	if(!(eq(star.x, Y.x) && eq(star.y, Y.y) && eq(star.z, Y.z))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%d, %d, %d\n", (int)(alpha*180/PI), (int)(beta*180/PI), (int)(gamma*180/PI));
		printf("\t%.2f, %.2f, %.2f\n", star.x, star.y, star.z);
	}
}

void test_14(void){
	// setup
	printf("Running unit test 14...\t");
	int passed = 1;

	// test
	Vector X = {1, 1, sqrt(2)};
	Vector Y = {1, 1, -sqrt(2)};
	Vector Z;

	cross_product(&X, &Y, &Z);

	normalize(&X);
	normalize(&Y);
	normalize(&Z);

	float alpha, beta, gamma;

	calculate_euler_angles(&X, &Y, &Z, &alpha, &beta, &gamma);

	// get desired vector
	Vector star = {1,0,0};

	// rotation
	intrinsic_rotation(&star, alpha, beta, gamma);

	if(!(eq(star.x, X.x) && eq(star.y, X.y) && eq(star.z, X.z))){
		passed = 0;
	}

	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
		printf("\t%d, %d, %d\n", (int)(alpha*180/PI), (int)(beta*180/PI), (int)(gamma*180/PI));
		printf("\t%.2f, %.2f, %.2f\n", star.x, star.y, star.z);
	}
}

void test_(void){

	// setup
	printf("Running unit test _...\t");
	int passed = 1;

	// test



	// clean up
	if(passed){
		printf("PASSED\n");
	}
	else {
		printf("FAILED\n");
	}
}

void run_tests(void){

	printf("\n\n\nRUNNING UNIT TESTS...\n");

	test_1();
	test_2();
	test_3();
	test_4();
	test_5();
	test_6();
	test_7();
	test_8();
	test_9();
	test_10();
	test_11();
	test_12();
	test_13();
	test_14();
}


