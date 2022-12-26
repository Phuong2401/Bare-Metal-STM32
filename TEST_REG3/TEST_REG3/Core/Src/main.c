#include "stm32f411xe.h"
#include "RccConfig.h"
#include "I2C_reg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
/********/

uint32_t RedCount, BlueCount;
typedef enum {FALSE, TRUE} bool;
bool is_suspended;

/**********/

#define RXSIZE 20
uint8_t rx_buf[32] = {0};
uint8_t main_buf[32] = {0};
uint8_t indx = 0;

/********/

void vApplicationMallocFailedHook();
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* PcTaskName);

void GPIO_config(void);
void delay(uint32_t time);

void UART_Init(void);
void DMA_Config (uint32_t srcAdd, uint32_t destAdd, uint16_t datasize);
void UART_Send_Data(uint8_t* pTXBuffer, uint32_t len);
void DMA1_Stream7_IRQ()
{
	__asm("NOP");
	//rx_buf[indx++] = USART2 ->DR;
}

void LedRedControllerTask( void* PVParemetter);
void LedOrangeControllerTask( void* PVParemetter);


int main(void)
	{


//	    memcpy (0x20000000, 0x00000000, 0x198);
//	    uint32_t* VTOR = (uint32_t*)0xe000ed08;
//	    *VTOR = 0x20000000;
//
//	    uint32_t* temp = (uint32_t*)0x200000d8;
//	    *temp = (int)DMA1_Stream7_IRQ | 1;


		//SysClockConfig();
		GPIO_config();

		UART_Init();
		DMA_Config((uint32_t)&USART2->DR, (uint32_t)rx_buf, sizeof(rx_buf));

		//xTaskCreate(LedOrangeControllerTask, "LedOrangeControllerTask", 128, NULL, 1, NULL);
	while(1)
		{
//		        GPIOD->BSRR |= (1<<12);  // Set the pin PA5
//		        GPIOD->BSRR |= (1<<13);  // Set the pin PA5
//				delay (1000000);
//				GPIOD->BSRR |= ((1<<12) <<16);  // Reset pin PA5
//				GPIOD->BSRR |= ((1<<13) <<16);  // Reset pin PA5
//				delay (1000000);
		}
	}


/******************************************************************************************************************/


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

void UART_Init()
{
	RCC ->APB1ENR |= 1 << 17;
	RCC ->AHB1LPENR |= 1 << 0;

	GPIOA -> MODER |= (0b10 << 4) | (0b10 << 6);
	GPIOA -> AFR[0] &= (0xff << 8);
	GPIOA -> AFR[0]  |= (7 << 8) | (7 << 12);

	USART2 -> CR1 |= (1<<13)|(1<<2)|(1<<3);
	USART2 -> BRR = (104 << 4) | 3;

}
void DMA_Config (uint32_t srcAdd, uint32_t destAdd, uint16_t datasize)
{
	RCC ->AHB1LPENR |= 1 << 21;

	USART2 ->CR3 |= (1 << 6);

	DMA1_Stream7 ->NDTR = datasize;
	DMA1_Stream7 ->PAR = USART2->DR;
	DMA1_Stream7 ->M0AR = destAdd;
	DMA1_Stream7 ->CR |= 1 << 0 | (0b110 << 25) | (1 << 10);
}

void UART_Send_Data(uint8_t* pTXBuffer, uint32_t len)
{
	uint32_t count;

	for(count = 0; count < len; count++)
		{
			 while((( USART2 -> SR >> 7) &1) != 1);
			 USART2 -> DR = (uint8_t)(*pTXBuffer & 0xff);
			 pTXBuffer++;
		}
}

void LedRedControllerTask( void* PVParemetter)
{
	while(1)
	{
	RedCount ++;
}
}
void LedOrangeControllerTask( void* PVParemetter)
{
	while(1)
	{

	BlueCount ++;
	}
}


void vApplicationMallocFailedHook()
{

}
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* PcTaskName)
{

}




















