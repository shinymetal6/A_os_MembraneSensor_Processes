/* 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Project : A_os
*/
/*
 * flash_ops.c
 *
 *  Created on: Sep 13, 2024
 *      Author: fil
 */
#include "main.h"

#include "../MembraneWsApp/A_os_includes.h"


#ifdef	MEMBRANE_COMMON
#include "../MembraneWsApp/membrane_includes.h"

extern	uint8_t					*_FlashDataRam_start;
extern	uint8_t					*_board_info_start;
FLASHDATARAM_AREA uint8_t		reprog_data_area[FLASHRAM_SIZE];
extern	MembraneInfo_TypeDef	MembraneInfo;

uint32_t	do_crc(uint32_t *data_ptr,uint32_t flash_data_len)
{
	FLASH_CRC.Instance->INIT = 0xffffffff;
	return HAL_CRC_Calculate(&FLASH_CRC, data_ptr, flash_data_len);
}

void clear_flash_area(void)
{
uint32_t	i;
	MembraneSystem.flash_address =  (uint8_t *)&reprog_data_area;
	for(i=0;i<FLASHRAM_SIZE;i++)
		MembraneSystem.flash_address[i] = 0xff;
}

void do_flash_update(uint8_t *flash_data,uint32_t size)
{
	flash_update(0,flash_data,size);
}

uint32_t	pflash_address,psize;
uint8_t		*pflash_data;
void local_flash_update(uint32_t flash_address,uint8_t *flash_data,uint32_t size)
{
	pflash_address = flash_address;
	pflash_data = flash_data;
	psize = size;
}

void do_params_update(void)
{
	pflash_address = (uint32_t )&_board_info_start-FLASH_BASE;
	pflash_data = (uint8_t *)&MembraneInfo;
	flash_update((uint32_t )&_board_info_start-FLASH_BASE,(uint8_t *)&MembraneInfo,FLASH_PAGE_SIZE);
}
#endif // #ifdef	MEMBRANE_2412171_00
