/*
 * stepper.h
 *
 *  Created on: Mar 27, 2022
 *      Author: broml
 *
 *  Stepper control for A4988 stepper drivers.
 *  No need for speed considerations, only angle.
 *
 *  deg. per step = 1.8
 *  min. step width = 1 us (use 1 ms)
 *  max. speed = 5 degrees per second
 *
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "stdlib.h"
#include "stm32l4xx_hal.h"
#define STEPPER_STEP_DELAY 10 // ms
#define STEPPER_RES 1.8 // deg per stepper
#define STEPPER_CW 0
#define STEPPER_CCW 1

typedef struct Stepper Stepper;

struct Stepper{
	float angle;
	GPIO_TypeDef * step_port;
	GPIO_TypeDef * dir_port;
	uint16_t step_pin;
	uint16_t dir_pin;
};

void stepper_init(Stepper * stepper_inst,
					GPIO_TypeDef * step_port,
					uint16_t step_pin,
					GPIO_TypeDef * dir_port,
					uint16_t dir_pin);

void write_stepper(Stepper * stepper_inst, uint16_t angle);

void make_step(Stepper * stepper_inst, int direction);


#endif /* INC_STEPPER_H_ */
