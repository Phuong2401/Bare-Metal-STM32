/*
 * SPI_REG.c
 *
 *  Created on: Oct 10, 2022
 *      Author: Minh Phuong
 */
#include "SPI_REG.h"
#include "GPIO_REG.h"

#define SS_ACTIVED GPIOE2 -> ODR2 &= ~( 1 << 3)
#define SS_INACTIVED GPIOE2 -> ODR2 |= ( 1 << 3);

void SPI1_INIT()
{

 GPIOA2 -> MODER2 &= ~(0b111111 << 10);
 GPIOA2 -> MODER2 |= (0b10 << 10) | (0b10 << 12) |(0b10 << 14);
 GPIOA2 -> AFRL2 &= ~(0xfff << 20);
 GPIOA2 -> AFRL2 |= (5 << 20) | (5 << 24) | (5 << 28);

 GPIOE2 -> MODER2 &= ~(0b11 << 6);
 GPIOE2 -> MODER2 |= (0b01 << 6);
 GPIOE2 -> ODR2 |= ( 0b01 << 2);

 GPIOE2 -> ODR2 |= ( 1 << 3);
 SPI_1 -> CR1_2 |= (1 << 8) | (1 << 9) | (1 << 2) | (1 << 6) | (0b100 << 3);
}

void SPI_Write(uint8_t cmd, uint8_t data)
{
	SS_ACTIVED;

	while(((SPI_1 -> SR_2 >> 1) & 1) != 1);
	SPI_1 -> DR_2 = cmd | (0 << 7);  //Write mode
	while(((SPI_1 -> SR_2 >> 1) & 1) == 1);
	while(((SPI_1 -> SR_2 >> 0) & 1) != 1);
	while(((SPI_1 -> SR_2 >> 7) & 1) == 1);

	uint32_t temp = SPI_1 -> DR_2;

	while(((SPI_1 -> SR_2 >> 1) & 1) != 1);
	SPI_1 -> DR_2 = data;
	while(((SPI_1 -> SR_2 >> 1) & 1) == 1);
	while(((SPI_1 -> SR_2 >> 0) & 1) != 1);
	while(((SPI_1 -> SR_2 >> 7) & 1) == 1);

	temp = SPI_1 -> DR_2;

	SS_INACTIVED;
}

uint8_t SPI_Read(uint8_t cmd)
{
	SS_ACTIVED;

	while(((SPI_1 -> SR_2 >> 1) & 1) != 1);
	SPI_1 -> DR_2 = cmd | (1 << 7);
	while(((SPI_1 -> SR_2 >> 1) & 1) == 1);
	while(((SPI_1 -> SR_2 >> 0) & 1) != 1);
	while(((SPI_1 -> SR_2 >> 7) & 1) == 1);

	uint32_t temp = SPI_1 -> DR_2;

	while(((SPI_1 -> SR_2 >> 1) & 1) != 1);
	SPI_1 -> DR_2 |= 0x00;
	while(((SPI_1 -> SR_2 >> 1) & 1) == 1);
	while(((SPI_1 -> SR_2 >> 0) & 1) != 1);
	while(((SPI_1 -> SR_2 >> 7) & 1) == 1);

	temp = SPI_1 -> DR_2;

	SS_INACTIVED;
	return temp;
}
