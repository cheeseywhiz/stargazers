/*
 * imu.c
 *
 *  Created on: Apr 4, 2022
 *      Author: broml
 */

#include "imu.h"
#include "stdio.h"
#include "math.h"

HAL_StatusTypeDef ret;

void imu_init(IMU * imu_inst, I2C_HandleTypeDef * hi2c){

	imu_inst->hi2c = hi2c;

	// enable accelerometer
	_write_reg_i2c_lsm303(hi2c, ACCEL_DEVICE_ADDR, CTRL_REG1_A, CTRL_REG1_A_EN);
	_write_reg_i2c_lsm303(hi2c, ACCEL_DEVICE_ADDR, CTRL_REG4_A, CTRL_REG4_A_EN);

	// enable magnetometer
	_write_reg_i2c_lsm303(hi2c, MAG_DEVICE_ADDR, CRB_REG_M, CRB_REG_M_EN);
	_write_reg_i2c_lsm303(hi2c, MAG_DEVICE_ADDR, MR_REG_M, MR_REG_M_EN);

}

void _write_reg_i2c_lsm303(I2C_HandleTypeDef * hi2c, uint8_t dev_addr, uint8_t sub_addr, uint8_t write_val){

	uint8_t write_buf[] = {sub_addr, write_val};

	ret = HAL_I2C_Master_Transmit(hi2c, dev_addr, write_buf, 2, 1000);

	if ( ret != HAL_OK ) {
	  ret = 1;
	  //printf("log (ijbd): failed I2C.\n");
	}
}

void _read_i2c_lsm303(I2C_HandleTypeDef * hi2c, uint8_t dev_addr, uint8_t sub_addr, uint8_t * buffer, uint8_t read_size){

	// send address
	ret = HAL_I2C_Master_Transmit(hi2c, dev_addr, &sub_addr, 1, 1000);

	if ( ret != HAL_OK ) {
	  ret = 1;
	  //printf("log (ijbd): failed I2C.\n");
	}

	// read from address
	ret = HAL_I2C_Master_Receive(hi2c, dev_addr, buffer, read_size, 1000);

	if ( ret != HAL_OK ) {
	  ret = 1;
	  //printf("log (ijbd): failed I2C.\n");
	}
}

void read_imu(IMU * imu_inst){
	uint8_t buffer[6];

	// read accelerometer
	_read_i2c_lsm303(imu_inst->hi2c, ACCEL_DEVICE_ADDR, ACCEL_REG_OUT, buffer, 6);

	imu_inst->accel.x = (int16_t)((buffer[1] << 8) | buffer[0]);
	imu_inst->accel.y = (int16_t)((buffer[3] << 8) | buffer[2]);
	imu_inst->accel.z = (int16_t)((buffer[5] << 8) | buffer[4]);

	normalize(&imu_inst->accel);

	// read magnetometer
	_read_i2c_lsm303(imu_inst->hi2c, MAG_DEVICE_ADDR, MAG_REG_OUT, buffer, 6);

	imu_inst->magnet.x = (int16_t)((buffer[0] << 8) | buffer[1]);
	imu_inst->magnet.y = (int16_t)((buffer[4] << 8) | buffer[5]);
	imu_inst->magnet.z = (int16_t)((buffer[2] << 8) | buffer[3]);

	normalize(&imu_inst->magnet);

}

void imu_time_average(IMU * imu_inst, Vector * average_accel, Vector * average_magnet, uint16_t num_samples){

	scale(average_accel, 0);
	scale(average_magnet, 0);
	for(int n = 0; n < num_samples; ++n){
		read_imu(imu_inst);
		add(average_accel, &imu_inst->accel, average_accel);
		add(average_magnet, &imu_inst->magnet, average_magnet);
		HAL_Delay(50);
	}

	normalize(average_accel);
	normalize(average_magnet);
}


