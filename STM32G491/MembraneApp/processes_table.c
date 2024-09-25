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
 * processes_table.c
 *
 *  Created on: May 20, 2024
 *      Author: fil
 */
#include "main.h"
#include "A_os_includes.h"
#include "../../CurrentVersion.h"
#ifdef	MEMBRANE_2412171_00
#include "membrane_includes.h"

extern	void process_1_comm(uint32_t process_id);		//This is process1
extern	void process_2_acquisition(uint32_t process_id);		//This is process2
extern	void process_3(uint32_t process_id);		//This is process3
extern	void process_4(uint32_t process_id);			//This is process4

VERSIONING	uint8_t	app_name[32] 		= APP_NAME;
VERSIONING	uint8_t	app_version[32] 	= APP_VERSION;

BOARDINFO_DATA_AREA	const MembraneInfo_TypeDef	MembraneInfo =
{
		.header_string = "MembraneInfo Header",
		.board_address = 5,
		.board_type = SENSORS_BOARD_TYPE,
		.name_string = APP_NAME,
		.version_string = APP_VERSION,
		.Aos_version_string = A_OS_VERSION,
		.tail_string = "MembraneInfo Tail"
};

USRprcs_t	UserProcesses[USR_PROCESS_NUMBER] =
{
		{
				.user_process = process_1_comm,
				.stack_size = 1024,
		},
		{
				.user_process = process_2_acquisition,
				.stack_size = 1024,
		},
		{
				.user_process = process_3,
				.stack_size = 256,
		},
		{
				.user_process = process_4,
				.stack_size = 256,
		}
};

#endif // #ifdef	MEMBRANE_2412171_00


