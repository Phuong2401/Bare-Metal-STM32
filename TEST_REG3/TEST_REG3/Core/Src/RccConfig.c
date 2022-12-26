

/**
  ******************************************************************************

  CLOCK Setup For STM32F446RE
  Author:   ControllersTech
  Updated:  2nd Aug 2020

  ******************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ******************************************************************************
*/

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 100
  *            PLL_P                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */

#include "RccConfig.h"

void SysClockConfig (void)
{
		/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Select the Clock Source and wait for it to be set
	
	********************************************************/
	
	#define PLL_M   8
	#define PLL_N   100
	#define PLL_P   2

	/*1. ENABLE HSE and wait for the HSE to become Ready*/
	RCC ->CR |= ( 1 << 16); // HSE clock enable.
	while(!(RCC ->CR & (1 << 17))); //Wait HSE clock ready flag.

	/*2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR*/
	RCC ->AHB1ENR |= (1 << 28);
	PWR ->CR |= (0b11 << 14);

	/*3. Configure the FLASH PREFETCH and the LATENCY Related Settings*/
	FLASH ->ACR |= (1 << 8)|(1 << 9)| (1 << 10)| ( 3<< 0);

	/*4. Configure the PRESCALARS HCLK, PCLK1, PCLK2*/
	RCC ->CFGR &= ~(1 << 4); //AHB PRESCALARS (DIV1)
	RCC ->AHB1ENR |= (0b100 << 10); //  APB Low speed prescaler (APB1) - DIV2
	RCC ->AHB1ENR &= (1 << 13); // APB high-speed prescaler (APB2) - DIV1

	/*5. Configure the MAIN PLL*/
	RCC ->PLLCFGR |= (PLL_M << 0) | (PLL_N << 6) | (PLL_P << 16) | (1 << 22);

	/*6. Enable the PLL and wait for it to become ready*/
	RCC ->CR |= (1 << 24);
	while(!(RCC ->CR & ( 1 << 25)));

	/*7. Select the Clock Source and wait for it to be set*/
	RCC ->CFGR |= (0b10 << 0);
	while(!(RCC ->CFGR & ( 0b10 << 2)));
}
