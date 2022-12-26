/*
 * GPIO_C.h
 *
 *  Created on: Sep 29, 2022
 *      Author: Minh Phuong
 */

#ifndef GPIO_REG_H_
#define GPIO_REG_H_



#include <stdint.h>

typedef struct
{
	volatile uint32_t MODER2;
	volatile uint32_t OTYPER2;
	volatile uint32_t OSPEEDR2;
	volatile uint32_t PUPDR2;
	volatile uint32_t IDR2;
	volatile uint32_t ODR2;
	volatile uint32_t BSRR2;
	volatile uint32_t LCKR2;
	volatile uint32_t AFRL2;
	volatile uint32_t AFRH2;
} GPIO_Types;

#define GPIOD_BaseADDr2               (0x40020c00UL)
#define GPIOD2                       ((GPIO_Types*)GPIOD_BaseADDr2)


#define GPIOA_BaseADDr2              (0x40020000UL)
#define GPIOA2                        ((GPIO_Types*)GPIOA_BaseADDr2)

#define GPIOE_BaseADDr2              (0x40021000UL)
#define GPIOE2                        ((GPIO_Types*)GPIOE_BaseADDr2)

#define GPIOB_BaseADDr2              (0x40005400UL)
#define GPIOB2                        ((GPIO_Types*)GPIOB_BaseADDr2)


void GPIO_UART2_Setup(void);

#endif /* GPIO_REG_H_ */
