/*
 * I2C_REG.c
 *
 *  Created on: Oct 16, 2022
 *      Author: Minh Phuong
 */
#include "I2C_REG.h"
#include "GPIO_REG.h"

void I2C1_init()
{
	GPIOB2 -> MODER2 |= (0b10 << 12) | (0b10 << 18); // PB6 PB9 as alternate function
	GPIOB2 -> OTYPER2 |= (1 << 6) | (1 << 9); // Open-rain
	GPIOB2 -> OSPEEDR2 |= (0b11 << 12) | (0b11 << 18); // High-speed
	GPIOB2 -> PUPDR2 |= (0b01 << 12) | (0b01 << 18); // High-speed

	GPIOB2 -> AFRL2 &= ~(0b1111 << 24); // PB6
	GPIOB2 -> AFRL2 |= (0b0100 << 24);

	GPIOB2 -> AFRH2 &= ~(0b1111 << 4); // PB9
	GPIOB2 -> AFRH2 |= (0b0100 << 4);



	I2C1_ ->CR1_ |= (1 << 15); //reset the I2c
	I2C1_ ->CR1_ &= ~(1 << 15);

	I2C1_ ->CR2_ |= (16 << 0); // config peripheral clock is 16MHZ
	I2C1_ ->CCR_ = 80 << 0;
	I2C1_ ->TRISE_ = 46;

	I2C1_ ->CR1_ |= 1; // Enable I2c
}

void I2C_Start()
{
	I2C1_ ->CR1_|= (1 << 10); // Enable ACK
	I2C1_ ->CR1_|= (1 << 8); // Generation the Start bit
	while(!(I2C1_ -> SR1_ & (1 << 0))); // Wait for Sb bit to set
}

void I2C_Stop()
{
	I2C1_ ->CR1_|= (1 << 9); // Generation the Start bit
}

void I2C_Wite( uint8_t data)
{
	while(!(I2C1_ ->SR1_ & (1 << 7))); // Wait for TXE bit to set
	I2C1_ -> DR_ = data;
	while(!(I2C1_ -> SR1_  & (1 << 2))); // Wait for BTF bit to set
}

void I2C_Address( uint8_t Address)
{
	I2C1_ ->DR_ = Address; // Send address to DR register
	while(!(I2C1_ ->SR1_ & (1 << 1))); // wait for ADDR bit to set
	uint8_t temp = I2C1_ -> SR1_ | I2C1_ -> SR2_; // Read SR1 and SR2 to clean the ADDR bit
}

void I2C_Read_OneByte( uint8_t address, uint8_t* buffer)
{
	I2C1_ ->DR_ = address;
	while(!(I2C1_ ->SR1_ & (1 << 1))); // wait for ADDR bit to set

	I2C1_ -> CR1_ &= ~(1 << 10); //clear the ACK bit
	uint8_t temp = I2C1_ -> SR1_ | I2C1_ -> SR2_; // Read SR1 and SR2 to clean the ADDR bit

	I2C1_ ->CR1_|= (1 << 9); // Generation the Start bit

	while(!(I2C1_ ->SR1_ & (1 << 6))); // wait for RxNE bit to set

	*buffer = I2C1_ ->DR_;
}








/*void I2C_Wite( uint8_t address, uint8_t Reg, uint8_t data)
{
	//while(((I2C1_2 -> SR2_2 >> 1) & 1) == 1); // wait busy flat is clean

	I2C1_ -> CR1_ |= (1 << 8); // Generation Start
    while(((I2C1_ -> SR1_) & 1) == 1); // wait start bit is generated

    I2C1_ ->DR_ = (address << 1) | 0; // Write Slave Add
    while(((I2C1_ -> SR1_ >> 1) & 1) != 1); // wait Address is sent
    while(((I2C1_ -> SR1_ >> 10) & 1) == 1); // wait ACK is sent by slave

    I2C1_ ->DR_ = Reg;
    while(((I2C1_ -> SR1_ >> 2) & 1) != 1); // wait ACK is sent by slave
    while(((I2C1_ -> SR1_ >> 10) & 1) == 1); // wait ACK is sent by slave

	I2C1_ ->DR_ = data;
	while(((I2C1_ -> SR1_ >> 2) & 1) != 1); // wait ACK is sent by slave
	while(((I2C1_ -> SR1_ >> 10) & 1) == 1); // wait ACK is sent by slave

	I2C1_ -> CR1_ |= (1 << 9); // Generation Stop bit
}

void I2C_Read( const uint8_t address, uint8_t Reg, uint8_t buffer)
{
	//while(((I2C1_2 -> SR2_2 >> 1) & 1) == 1); // wait busy flat is clean

	I2C1_ -> CR1_ |= (1 << 8); // Generation Start
    while(((I2C1_ -> SR1_) & 1) != 1); // wait start bit is generated

    I2C1_ ->DR_ = (address << 1) | 0; // Write Slave Add  - write
    while(((I2C1_ -> SR1_ >> 1) & 1) != 1); // wait Address is sent
    uint32_t temp = I2C1_ -> SR1_ | I2C1_ -> SR2_; // read SR1 and SR2 to clear the ADDR bit
    while(((I2C1_ -> SR1_ >> 10) & 1) == 1); // wait ACK is sent by slave

    I2C1_ ->DR_ = Reg; // Write Reg Add
	while(((I2C1_ -> SR1_ >> 1) & 1) != 1); // wait Address is sent
	while(((I2C1_ -> SR1_ >> 10) & 1) == 1); // wait ACK is sent by slave


	I2C1_ -> CR1_ |= (1 << 8); // Generation re-Start
    while(((I2C1_ -> SR1_) & 1) != 1); // wait re-start bit is generated

    I2C1_ ->DR_ = (address << 1) | 1; // Write Slave Add - read
	while(((I2C1_ -> SR1_ >> 1) & 1) != 1); // wait Address is sent
    temp = I2C1_ -> SR1_ | I2C1_ -> SR2_; // read SR1 and SR2 to clear the ADDR bit

    buffer = I2C1_ ->DR_;

    I2C1_ -> CR1_ |= (1 << 9); // Generation Stop bit
}
*/
