/*
 * I2C_reg.h
 *
 *  Created on: Nov 13, 2022
 *      Author: Minh Phuong
 */

#ifndef I2C_REG_H_
#define I2C_REG_H_

#include <stdint.h>

void I2C_Config (void);

void I2C_Start (void);

void I2C_Write (uint8_t data);

void I2C_Address (uint8_t Address);

void I2C_Stop (void);

void I2C_Read (uint8_t Address, uint8_t *buffer, uint8_t size);

#endif /* I2C_REG_H_ */
