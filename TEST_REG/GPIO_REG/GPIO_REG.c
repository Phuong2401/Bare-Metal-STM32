

#include "GPIO_REG.h"

void GPIO_UART2_Setup(void)
{
	GPIOA2 -> MODER2 |= (0b10 << 4) | (0b10 << 6);
	GPIOA2 -> AFRL2 &= (0xff << 8);
	GPIOA2 -> AFRL2 |= (7 << 8) | (7 << 12);
}
