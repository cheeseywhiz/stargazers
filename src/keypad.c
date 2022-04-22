/*
 * keypad.c
 *
 *  Created on: Apr 12, 2022
 *      Author: tomasfe
 */


#include "keypad.h"

// Return number
// A = 10, B = 11, C = 12, D = 13, * = 14, # = 15
int readKeypad(){
    GPIO_PinState state = GPIO_PIN_RESET;
    // C1 is turned on
    HAL_GPIO_WritePin(C1_PORT, C1_PIN, GPIO_PIN_SET);
    state = HAL_GPIO_ReadPin(R1_PORT, R1_PIN);
    if(state == GPIO_PIN_SET){
        return 1;
    }
    state = HAL_GPIO_ReadPin(R2_PORT, R2_PIN);
    if(state == GPIO_PIN_SET){
        return 4;
    	//return 2;
    }
    state = HAL_GPIO_ReadPin(R3_PORT, R3_PIN);
    if(state == GPIO_PIN_SET){
        return 7;
        //return 3;
    }
    /*state = HAL_GPIO_ReadPin(R4_PORT, R4_PIN);
    if(state == GPIO_PIN_SET){
        return 14; // *
    	//return 10; // A
    }*/
    HAL_GPIO_WritePin(C1_PORT, C1_PIN, GPIO_PIN_RESET);

    // C2 is turned on
    HAL_GPIO_WritePin(C2_PORT, C2_PIN, GPIO_PIN_SET);
    state = HAL_GPIO_ReadPin(R1_PORT, R1_PIN);
    if(state == GPIO_PIN_SET){
        return 2;
    	//return 4;
    }
    state = HAL_GPIO_ReadPin(R2_PORT, R2_PIN);
    if(state == GPIO_PIN_SET){
        return 5;
    	//return 5;
    }
    state = HAL_GPIO_ReadPin(R3_PORT, R3_PIN);
    if(state == GPIO_PIN_SET){
        return 8;
    	//return 6;
    }
    /*state = HAL_GPIO_ReadPin(R4_PORT, R4_PIN);
    if(state == GPIO_PIN_SET){
        return 0;
    	//return 11; // B
    }*/
    HAL_GPIO_WritePin(C2_PORT, C2_PIN, GPIO_PIN_RESET);

    // C3 is turned on
    HAL_GPIO_WritePin(C3_PORT, C3_PIN, GPIO_PIN_SET);
    state = HAL_GPIO_ReadPin(R1_PORT, R1_PIN);
    if(state == GPIO_PIN_SET){
        return 3;
    	//return 7;
    }
    state = HAL_GPIO_ReadPin(R2_PORT, R2_PIN);
    if(state == GPIO_PIN_SET){
        return 6;
        //return 8;
    }
    state = HAL_GPIO_ReadPin(R3_PORT, R3_PIN);
    if(state == GPIO_PIN_SET){
        return 9;
    }
    /*state = HAL_GPIO_ReadPin(R4_PORT, R4_PIN);
    if(state == GPIO_PIN_SET){
        return 15; // #
    	//return 12; // C
    }*/
    HAL_GPIO_WritePin(C3_PORT, C3_PIN, GPIO_PIN_RESET);

    // C4 is turned on
    HAL_GPIO_WritePin(C4_PORT, C4_PIN, GPIO_PIN_SET);
    state = HAL_GPIO_ReadPin(R1_PORT, R1_PIN);
    if(state == GPIO_PIN_SET){
        return 10; // A
    	//return 14; // *
    }
    state = HAL_GPIO_ReadPin(R2_PORT, R2_PIN);
    if(state == GPIO_PIN_SET){
        return 11; // B
    	//return 0;
    }
    state = HAL_GPIO_ReadPin(R3_PORT, R3_PIN);
    if(state == GPIO_PIN_SET){
        return 12; // C
    	//return 15; // #
    }
    /*state = HAL_GPIO_ReadPin(R4_PORT, R4_PIN);
    if(state == GPIO_PIN_SET){
        return 13; // D
    }*/
    HAL_GPIO_WritePin(C4_PORT, C4_PIN, GPIO_PIN_RESET);

    return -1;
}

void loadKeypadInterrupt(){
	HAL_GPIO_WritePin(C1_PORT, C1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(C2_PORT, C2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(C3_PORT, C3_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(C4_PORT, C4_PIN, GPIO_PIN_SET);
}

void clearKeypadInterrupt(){
	HAL_GPIO_WritePin(C1_PORT, C1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(C2_PORT, C2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(C3_PORT, C3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(C4_PORT, C4_PIN, GPIO_PIN_RESET);
}
