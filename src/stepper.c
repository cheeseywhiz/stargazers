/*
 * stepper.c
 *
 *  Created on: Mar 27, 2022
 *      Author: broml
 */

#include "stepper.h"
#include "math.h"

void stepper_init(Stepper * stepper_inst,
					GPIO_TypeDef * step_port,
					uint16_t step_pin,
					GPIO_TypeDef * dir_port,
					uint16_t dir_pin){
	stepper_inst->angle = 0;
	stepper_inst->state_angle = 0;
	stepper_inst->step_port = step_port;
	stepper_inst->step_pin = step_pin;
	stepper_inst->dir_port = dir_port;
	stepper_inst->dir_pin = dir_pin;

}

float stepper_mod(float angle){
	// convert to positive
	angle = (angle < 0) ? fmod(angle,360) + 360 : angle;

	// map to [0,360)
	angle = fmod(angle, 360);

	// default zero
	angle = (angle > 359) ? 0 : angle;

	return angle;
}

void write_stepper(Stepper * stepper_inst, float angle){

	float angle_diff = angle - stepper_inst->angle;

	// convert to positive
	float state_angle = stepper_mod(stepper_inst->state_angle + angle_diff);

	// find number of steps
	int num_steps = abs((state_angle - stepper_inst->state_angle)) / STEPPER_RES;
	int dir = (state_angle > stepper_inst->state_angle) ? STEPPER_CW : STEPPER_CCW;
	float step_size = (dir == STEPPER_CW) ? STEPPER_RES : -STEPPER_RES;

	// carry out steps.

	for(uint16_t step = 0; step < num_steps; ++step){
		make_step(stepper_inst, dir);
		stepper_inst->state_angle += step_size;
		stepper_inst->angle += step_size;
	}

	stepper_inst->state_angle = stepper_mod(stepper_inst->state_angle);
	stepper_inst->angle = stepper_mod(stepper_inst->angle);
}

void make_step(Stepper * stepper_inst, int dir){

	// Set direction
	HAL_GPIO_WritePin(stepper_inst->dir_port, stepper_inst->dir_pin, dir);
	HAL_Delay(1);

	// Make step
	HAL_GPIO_WritePin(stepper_inst->step_port, stepper_inst->step_pin, 1);
	HAL_Delay(STEPPER_STEP_DELAY);

	// Finish step
	HAL_GPIO_WritePin(stepper_inst->step_port, stepper_inst->step_pin, 0);

}

