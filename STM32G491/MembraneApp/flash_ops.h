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
 * flash_ops.h
 *
 *  Created on: Sep 13, 2024
 *      Author: fil
 */


#ifndef STM32G491_MEMBRANEAPP_FLASH_OPS_H_
#define STM32G491_MEMBRANEAPP_FLASH_OPS_H_

extern	void clear_flash_area(void);
extern	void flash_check(uint8_t *flash_data_ptr,uint32_t size);
extern	uint32_t do_crc(uint32_t 	*data_ptr,uint32_t flash_data_len);
extern	void do_flash_update(uint8_t *flash_data,uint32_t size);
extern	void do_params_update(void);


#endif /* STM32G491_MEMBRANEAPP_FLASH_OPS_H_ */
