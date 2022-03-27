/*
 * servo.c
 *
 *  Created on: Mar 27, 2022
 *      Author: broml
 */

#include "servo.h"

void servo_init(Servo * servo_inst, TIM_HandleTypeDef * timer){
	// attach timer
	_attach_timer(servo_inst, timer);

	// setup and start PWM
	__HAL_TIM_SET_PRESCALER(servo_inst->timer, SERVO_PRESCALER);
	__HAL_TIM_SET_AUTORELOAD(servo_inst->timer, SERVO_ARR);
	HAL_TIM_PWM_Start(servo_inst->timer, SERVO_TIM_CHANNEL);

}

void write_servo(Servo * servo_inst, uint8_t angle){
	// enforce angle constraint
	angle = (angle > SERVO_ANGLE_LIM) ? SERVO_ANGLE_LIM : angle;

	// update servo compare register value
	uint16_t compare_val = _get_compare_val(angle);
	_set_compare(servo_inst->timer, compare_val);
}

void _set_compare(TIM_HandleTypeDef * timer, uint16_t compare_val){
	__HAL_TIM_SET_COMPARE(timer, SERVO_TIM_CHANNEL, compare_val);
}

void _attach_timer(Servo * servo_inst, TIM_HandleTypeDef * timer){
	servo_inst->timer = timer;
}

uint16_t _get_compare_val(uint8_t angle){

	uint16_t compare_val = angle + (uint16_t)(SERVO_MIN_COMPARE_VAL);

	return compare_val;
}
