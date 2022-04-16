/*
 * keypad.h
 *
 *  Created on: Apr 12, 2022
 *      Author: tomasfe
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>

//  pins (outputs) (BLACK)
#define C1_PORT GPIOA
#define C1_PIN GPIO_PIN_2
#define C2_PORT GPIOA
#define C2_PIN GPIO_PIN_0
#define C3_PORT GPIOB
#define C3_PIN GPIO_PIN_0
#define C4_PORT GPIOE
#define C4_PIN GPIO_PIN_0
//  pins (inputs) (WHITE) (with resistor)
#define R1_PORT GPIOE
#define R1_PIN GPIO_PIN_12
#define R2_PORT GPIOE
#define R2_PIN GPIO_PIN_14
#define R3_PORT GPIOE
#define R3_PIN GPIO_PIN_15
#define R4_PORT GPIOB
#define R4_PIN GPIO_PIN_10

#endif /* INC_KEYPAD_H_ */
