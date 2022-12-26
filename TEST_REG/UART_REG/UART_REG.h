/*
 * UART_REG.h
 *
 *  Created on: Oct 1, 2022
 *      Author: Minh Phuong
 */

#ifndef UART_REG_H_
#define UART_REG_H_

#include <stdint.h>

typedef struct
{
	volatile uint32_t SR2;
	volatile uint32_t DR2;
	volatile uint32_t BRR2;
	volatile uint32_t CR12;
	volatile uint32_t CR22;
	volatile uint32_t CR32;
	volatile uint32_t GTPR2;
} UART_Types;

  /*Base Add*/
#define UART2_BaseADDr2                 (0x40004400UL)
#define UART2                 ((UART_Types*)UART2_BaseADDr2)

/*UART_DMA*/
typedef struct
{
	volatile uint32_t S7CR2;
	volatile uint32_t S7NDTR2;
	volatile uint32_t S7PAR2;
	volatile uint32_t S7M0AR2;
	volatile uint32_t S7M1AR2;
	volatile uint32_t S7FCR2;
} UART_DMA1_C7_Types;


/*Base Add*/
#define DMA_1C7_BaseADDr                (0x400260b8ul)
#define DMA_1C7                 ((UART_DMA1_C7_Types*)DMA_1C7_BaseADDr)



void UART_Init(void);
void UART_Send_Data(uint8_t* pTXBuffer, uint32_t len);
void UART_Receive_Data(uint8_t* pRXBuffer, uint32_t len);

/*Interrupt UART*/
void UART_RX_Interrupt_Init(void);

/*DMA_UART*/
//void UART_DMA_Init(void);




#endif /* UART_REG_H_ */
