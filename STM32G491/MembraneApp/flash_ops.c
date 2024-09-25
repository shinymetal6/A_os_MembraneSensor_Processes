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
#include "A_os_includes.h"


#ifdef	MEMBRANE_2412171_00
#include "membrane_includes.h"

extern	uint8_t				*_FlashDataRam_start;
FLASHDATARAM_AREA uint8_t	reprog_data_area[FLASHRAM_SIZE];

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
	flash_update(flash_data,size);
}
#endif // #ifdef	MEMBRANE_2412171_00
