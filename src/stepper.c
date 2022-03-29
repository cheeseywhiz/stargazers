/*
 * stepper.c
 *
 *  Created on: Mar 27, 2022
 *      Author: broml
 */

#include "stepper.h"

void stepper_init(Stepper * stepper_inst,
					GPIO_TypeDef * step_port,
					uint16_t step_pin,
					GPIO_TypeDef * dir_port,
					uint16_t dir_pin){
	stepper_inst->angle = 0;
	stepper_inst->step_port = step_port;
	stepper_inst->step_pin = step_pin;
	stepper_inst->dir_port = dir_port;
	stepper_inst->dir_pin = dir_pin;

}

void write_stepper(Stepper * stepper_inst, uint16_t angle){
	int num_steps = abs(angle - stepper_inst->angle) / STEPPER_RES;
	int dir = (angle > stepper_inst->angle) ? STEPPER_CW : STEPPER_CCW;

	for(int step = 0; step < num_steps; ++step){
		make_step(stepper_inst, dir);
		stepper_inst->angle += (dir == STEPPER_CW) ? STEPPER_RES : -STEPPER_RES;
	}
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

