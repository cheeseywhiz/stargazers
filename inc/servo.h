/*
 * servo.h
 *
 *  Created on: Mar 27, 2022
 *      Author: broml
 *
 *  For DS3218 Servo motor:
 *
 *  Motion range = 270 degrees
 *  PWM period = 10 ms
 *  min pulse width = 500 us
 *  max pulse width = 2500 us
 *  neutral pulse width = 1500 us
 *
 * 	^ setup pan and tilt kit so level at 0
 *
 *  timer = 4
 *  TIM_CHANNEL = 3
 *  pin = PD14.
 *
 *	assuming one compare bit per degree...
 *
 *  pulse width resolution = (2500 us - 500 us) / 270 deg = 7.4 us
 *  pulse width freq = 1 / 7.4 us ~= 13.52 kHz
 *  psc = 4 MHz / 13.52 kHz - 1 ~= 29
 *  arr = 10 ms / 7.4 us - 1 ~= 1351
 *
 *  MIN_COMPARE_VAL = 500 us / 7.4 us ~= 68
 *
 *  angle limit is determined by the physical constraints of the tilt mechanism
 *  ANGLE_LIM = 90;
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "stm32l4xx_hal.h"
#include "stdlib.h"

#define SERVO_TIM_CHANNEL TIM_CHANNEL_3
#define SERVO_PRESCALER 29
#define SERVO_ARR 1351
#define SERVO_MIN_COMPARE_VAL 68
#define SERVO_ANGLE_LIM 90

typedef struct Servo Servo;

struct Servo{
	TIM_HandleTypeDef * timer;
};

void servo_init(Servo * servo_inst, TIM_HandleTypeDef * timer);

void write_servo(Servo * servo_inst, uint8_t angle);

void _set_compare(TIM_HandleTypeDef * timer, uint16_t compare_val);

void _attach_timer(Servo * servo_inst, TIM_HandleTypeDef * timer);

uint16_t _get_compare_val(uint8_t angle);


#endif /* INC_SERVO_H_ */
