/*
 * I2C_reg2.c
 *
 *  Created on: Nov 16, 2022
 *      Author: Minh Phuong
 */



#include "I2C_reg2.h"
#include "stm32f411xe.h"

void I2C_Config1()
{
	/**** STEPS FOLLOWED  ************
	1. Enable the I2C CLOCK and GPIO CLOCK
	2. Configure the I2C PINs for ALternate Functions
		a) Select Alternate Function in MODER Register
		b) Select Open Drain Output
		c) Select High SPEED for the PINs
		d) Select Pull-up for both the Pins
		e) Configure the Alternate Function in AFR Register
	3. Reset the I2C
	4. Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
	5. Configure the clock control registers
	6. Configure the rise time register
	7. Program the I2C_CR1 register to enable the peripheral
	*/

	/*1. Enable the I2C CLOCK and GPIO CLOCK*/
	RCC ->APB1ENR |= (1 << 21); // enable I2C1 CLOCK
	RCC ->AHB1ENR |= (1 << 1); // Enable GPIOB CLOCK

	/*2. Configure the I2C PINs for ALternate Functions*/
	GPIOB ->MODER |= (0b10 << 12) | (0b10 << 18); // Bits (13:12)= 1:0 --> Alternate Function for Pin PB6; Bits (19:18)= 1:0 --> Alternate Function for Pin PB9
	GPIOB ->OTYPER |=(0b01 << 6) | (0b01 << 9); //  Bit6=1, Bit9=1  output open drain
    GPIOB ->OSPEEDR |= (0b11 << 12) | (0b11 << 18); // Bits (13:12)= 1:1 --> High Speed for PIN PB6; Bits (19:18)= 1:1 --> High Speed for PIN PB9
    GPIOB->AFR[0] |= (0b0100 << 24);
    GPIOB->AFR[0] |= (0b0100 << 4);

    /*3. Reset the I2C 	*/
    I2C1->CR1 |= (1<<15);
    I2C1->CR1 &= ~(1<<15);

    // Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
    I2C1->CR2 |= (50<<0);  // PCLK1 FREQUENCY in 50 MHz

    // Configure the clock control registers
    I2C1->CCR = 250<<0;

    // Configure the rise time register
    I2C1->TRISE = 51;

    // Program the I2C_CR1 register to enable the peripheral
    I2C1->CR1 |= (1<<0);  // Enable I2C
}

void I2C_Start1 ()
{
	/**** STEPS FOLLOWED  ************
	1. Send the START condition
	2. Wait for the SB ( Bit 0 in SR1) to set. This indicates that the start condition is generated
	*/

	I2C1->CR1 |= (1<<10);  // Enable the ACK
	I2C1->CR1 |= (1<<8);  // Generate START
	while (!(I2C1->SR1 & (1<<0)));  // Wait fror SB bit to set
}

void I2C_Write1 (uint8_t data)
{
	/**** STEPS FOLLOWED  ************
	1. Wait for the TXE (bit 7 in SR1) to set. This indicates that the DR is empty
	2. Send the DATA to the DR Register
	3. Wait for the BTF (bit 2 in SR1) to set. This indicates the end of LAST DATA transmission
	*/

	while (!(I2C1->SR1 & (1<<7)));  // wait for TXE bit to set
	I2C1->DR = data;
	while (!(I2C1->SR1 & (1<<2)));  // wait for BTF bit to set
}

void I2C_Address1 (uint8_t Address)
{
	/**** STEPS FOLLOWED  ************
	1. Send the Slave Address to the DR Register
	2. Wait for the ADDR (bit 1 in SR1) to set. This indicates the end of address transmission
	3. clear the ADDR by reading the SR1 and SR2
	*/

	I2C1->DR = Address;  //  send the address
	while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
	uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit

}

void I2C_Stop1 ()
{
	I2C1->CR1 |= (1<<9);  // Stop I2C
}

void I2C_Read1 (uint8_t Address, uint8_t *buffer, uint8_t size)
{
/**** STEPS FOLLOWED  ************
1. If only 1 BYTE needs to be Read
	a) Write the slave Address, and wait for the ADDR bit (bit 1 in SR1) to be set
	b) the Acknowledge disable is made during EV6 (before ADDR flag is cleared) and the STOP condition generation is made after EV6
	c) Wait for the RXNE (Receive Buffer not Empty) bit to set
	d) Read the data from the DR

2. If Multiple BYTES needs to be read
  a) Write the slave Address, and wait for the ADDR bit (bit 1 in SR1) to be set
	b) Clear the ADDR bit by reading the SR1 and SR2 Registers
	c) Wait for the RXNE (Receive buffer not empty) bit to set
	d) Read the data from the DR
	e) Generate the Acknowlegment by settint the ACK (bit 10 in SR1)
	f) To generate the nonacknowledge pulse after the last received data byte, the ACK bit must be cleared just after reading the
		 second last data byte (after second last RxNE event)
	g) In order to generate the Stop/Restart condition, software must set the STOP/START bit
	   after reading the second last data byte (after the second last RxNE event)
*/

	int remaining = size;

	if (size == 1)
		{
			/**** STEP 1-a ****/
			I2C1->DR = Address;  //  send the address
			while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set

			/**** STEP 1-b ****/
			I2C1->CR1 &= ~(1<<10);  // clear the ACK bit
			uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit.... EV6 condition
			I2C1->CR1 |= (1<<9);  // Stop I2C

			/**** STEP 1-c ****/
			while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set

			/**** STEP 1-d ****/
			buffer[size-remaining] = I2C1->DR;  // Read the data from the DATA REGISTER

		}

}
