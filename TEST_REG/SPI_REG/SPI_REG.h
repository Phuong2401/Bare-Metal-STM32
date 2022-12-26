/*
 * SPI_REG.h
 *
 *  Created on: Oct 10, 2022
 *      Author: Minh Phuong
 */

#ifndef SPI_REG_H_
#define SPI_REG_H_

#include <stdint.h>

typedef struct
{
	volatile uint32_t CR1_2;
	volatile uint32_t CR2_2;
	volatile uint32_t SR_2;
	volatile uint32_t DR_2;
	volatile uint32_t CRCPR_2;
	volatile uint32_t TXCRCR_2;
	volatile uint32_t I2SCFGR_2;
	volatile uint32_t I2SPR_2;
} SPI_Types;

#define SPI1_BaseADDr             (0x40013000UL)
#define SPI_1                      ((SPI_Types*)SPI1_BaseADDr)


void SPI1_INIT(void);
uint8_t SPI_Read(uint8_t cmd);
void SPI_Write(uint8_t cmd, uint8_t data);

#endif /* SPI_REG_H_ */
