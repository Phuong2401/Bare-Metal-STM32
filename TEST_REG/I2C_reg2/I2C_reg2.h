/*
 * I2C_reg2.h
 *
 *  Created on: Nov 16, 2022
 *      Author: Minh Phuong
 */

#ifndef I2C_REG2_H_
#define I2C_REG2_H_

#include <stdint.h>

void I2C_Config1 (void);

void I2C_Start1 (void);

void I2C_Write1 (uint8_t data);

void I2C_Address1 (uint8_t Address);

void I2C_Stop1 (void);

void I2C_Read1 (uint8_t Address, uint8_t *buffer, uint8_t size);

#endif /* I2C_REG2_H_ */
