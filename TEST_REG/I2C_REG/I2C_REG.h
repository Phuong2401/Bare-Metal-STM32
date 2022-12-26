/*
 * I2C_REG.h
 *
 *  Created on: Oct 16, 2022
 *      Author: Minh Phuong
 */

#ifndef I2C_REG_H_
#define I2C_REG_H_

#include <stdint.h>

typedef struct
{
	volatile uint32_t CR1_;
	volatile uint32_t CR2_;
	volatile uint32_t OAR1_;
	volatile uint32_t OAR2_;
	volatile uint32_t DR_;
	volatile uint32_t SR1_;
	volatile uint32_t SR2_;
	volatile uint32_t CCR_;
	volatile uint32_t TRISE_;
} I2C_Types;

#define I2C1_BaseADDr             (0x40005400UL)
#define  I2C1_                       ((I2C_Types*)I2C1_BaseADDr)

void I2C1_init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Wite( uint8_t data);
void I2C_Address( uint8_t Address);
void I2C_Read_OneByte( uint8_t address, uint8_t* buffer);


#endif /* I2C_REG_H_ */
