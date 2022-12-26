/*
 * FLASH_REG.h
 *
 *  Created on: Oct 17, 2022
 *      Author: Minh Phuong
 */

#ifndef FLASH_REG_H_
#define FLASH_REG_H_

#include <stdint.h>

typedef struct
{
	volatile uint32_t ACR_;
	volatile uint32_t KEYR_;
	volatile uint32_t OPTKEYR_;
	volatile uint32_t SR_;
	volatile uint32_t CR_;
	volatile uint32_t OPTCR_;
} Flash_Types;

#define Flash_BaseADDr             (0x40023c00UL)
#define  Flash_                       ((Flash_Types*)Flash_BaseADDr)

void Flash_Section_Erase(int Sector);
void Flash_Program( uint8_t* flash, char* data, int size);

#endif /* FLASH_REG_H_ */
