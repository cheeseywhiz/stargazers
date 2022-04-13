/*
 * imu.h
 *
 *  Created on: Apr 4, 2022
 *      Author: broml
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

#include "stm32l4xx_hal.h"
#include "stdlib.h"
#include "vector.h"

#define ACCEL_DEVICE_ADDR 0x32
#define MAG_DEVICE_ADDR 0x3C
#define CTRL_REG1_A 0x20
#define CTRL_REG1_A_EN 0x97
#define CTRL_REG4_A 0x23
#define CTRL_REG4_A_EN 0x00
#define CRB_REG_M 0x01
#define CRB_REG_M_EN 0x20
#define MR_REG_M 0x02
#define MR_REG_M_EN 0x00
#define G_PER_LSB .001
#define GAUSS_PER_LSB_XY .00091
#define GAUSS_PER_LSB_Z .001
#define ACCEL_REG_OUT 0xA8
#define MAG_REG_OUT 0x83

typedef struct IMU IMU;

struct IMU{
	Vector accel;
	Vector magnet;
	I2C_HandleTypeDef * hi2c;
};

void imu_init(IMU * imu_inst, I2C_HandleTypeDef * hi2c);

void read_imu(IMU * imu_inst);

void imu_time_average(IMU * imu_inst, Vector * average_accel, Vector * average_magnet, uint16_t num_samples);

void _write_reg_i2c_lsm303(I2C_HandleTypeDef * hi2c, uint8_t dev_addr, uint8_t sub_addr, uint8_t write_val);

void _read_i2c_lsm303(I2C_HandleTypeDef * hi2c, uint8_t dev_addr, uint8_t sub_addr, uint8_t * buffer, uint8_t read_size);

#endif /* INC_IMU_H_ */
