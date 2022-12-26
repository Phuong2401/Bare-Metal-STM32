#include "stm32f411xe.h"

void SysClockConfig(void)
	{
		#define PLL_M   4 
		#define PLL_N   100
		#define PLL_P   0
		
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

	void GPIO_config(void)
		{
			/*
			1. Enable the GPIO CLOCK
			2. Set the Pin as the Output
			3. Configure the Output Mode
			*/
			
		/*1. Enable the GPIO CLOCK*/
		RCC ->AHB1ENR |= (1 << 3); // Enable the GPIOD CLOCK
		/*2. Set the Pin as the Output*/
		GPIOD ->MODER |= (0b01 << 26);
    GPIOD ->MODER |= (0b01 << 24);			
    /*3. Configure the Output Mode*/
    GPIOD ->OTYPER = 0;
    GPIOD ->OSPEEDR = 0;				
		}
		
 void delay(uint32_t time)
 {
 while(time --);
 }
int main(void)
	{
		
		SysClockConfig();
		GPIO_config();
	while(1)
		{
		GPIOD ->BSRR |= (1 << 13);
			GPIOD ->BSRR |= (1 << 12);
		delay(1000000);
		GPIOD ->BSRR |= (1 << 29);
				GPIOD ->BSRR |= (1 << 28);
		delay(1000000);
		}
	}