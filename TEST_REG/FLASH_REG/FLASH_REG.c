/*
 * FLASH_REG.c
 *
 *  Created on: Oct 17, 2022
 *      Author: Minh Phuong
 */
#include "FLASH_REG.h"

void Flash_Section_Erase(int Sector)
{
	if(((Flash_ -> CR_ >> 31) & 1) == 1)
	{
		Flash_ -> KEYR_ = 0x45670123;
		Flash_ -> KEYR_ =  0xCDEF89AB;
	}
	while(((Flash_ -> SR_ >> 16) & 1) == 1);

	Flash_ -> CR_ |= 1 << 1; //Sector erase mode
	Flash_ -> CR_ |= Sector << 3; //Sector 7 from  0x0806 0000 - 0x0807 FFFF 128kb

	Flash_ -> CR_ |= 1 << 16; //Start

	while(((Flash_ -> SR_ >> 16) & 1) == 1);
	Flash_ -> CR_ &= ~(1 << 1); // Stop
}

void Flash_Program(uint8_t* flash, char* data, int size)
{
	if(((Flash_ -> CR_ >> 31) & 1) == 1)
	{
		Flash_ -> KEYR_ = 0x45670123;
		Flash_ -> KEYR_ =  0xCDEF89AB;
	}
	while(((Flash_ -> SR_ >> 16) & 1) == 1);
	Flash_ -> CR_ |= 1; //Sector erase mode

	//uint8_t* flash = (uint8_t*)0x08060000;
	for(int i = 0; i < size; i++)
	{
	  flash[i] = data[i];
	  while(((Flash_ -> SR_ >> 16) & 1) == 1);
	}
}

/*void Flash_Program(char* data, int size)
{
	if(((Flash_ -> CR_ >> 31) & 1) == 1)
	{
		Flash_ -> KEYR_ = 0x45670123;
		Flash_ -> KEYR_ =  0xCDEF89AB;
	}
	while(((Flash_ -> SR_ >> 16) & 1) == 1);
	Flash_ -> CR_ |= 1; //Sector erase mode

	uint8_t* flash = (uint8_t*)0x08060000;
	for(int i = 0; i < size; i++)
	{
	  flash[i] = data[i];
	  while(((Flash_ -> SR_ >> 16) & 1) == 1);
	}
}
*/
