
#include "UART_REG.h"
#include "GPIO_REG.h"

void UART_Init()
{
  GPIO_UART2_Setup();

  UART2 -> CR12 |= (1<<13)|(1<<2)|(1<<3);
  UART2 -> BRR2 = (104 << 4) | 3;
}
void UART_Send_Data(uint8_t* pTXBuffer, uint32_t len)
{
	uint32_t count;

	for(count = 0; count < len; count++)
		{
			 while((( UART2 -> SR2 >> 7) &1) != 1);
			 UART2 -> DR2 = (uint8_t)(*pTXBuffer & 0xff);
			 pTXBuffer++;
		}
}
void UART_Receive_Data(uint8_t* pRXBuffer, uint32_t len)
{
    uint32_t count;
    	for(count = 0; count < len; count++)
    	{
    		while(((UART2 -> SR2 >> 5) &1) != 1);
    		*pRXBuffer = (uint8_t)( UART2 -> DR2 & 0xff);
    		pRXBuffer++;
    	}
}

void UART_RX_Interrupt_Init()
{
	UART2 ->CR12 |= (1 << 5);

    uint32_t *ISER1 = (uint32_t*)0xe000e104;
    *ISER1 |= (1 <<(38-32));
}

