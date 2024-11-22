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
 * Project : A_os_MembraneSensor_Processes
*/
/*
 * processes_table.c
 *
 *  Created on: May 20, 2024
 *      Author: fil
 */
#include "main.h"
#include "../../CurrentVersion.h"
#include "../Common/A_os_includes.h"
#ifdef	MEMBRANE_WS_2412171_00
#include "membrane_includes.h"

extern	void process_1_comm(uint32_t process_id);		//This is process1
extern	void process_2_acquisition(uint32_t process_id);		//This is process2
extern	void process_3(uint32_t process_id);		//This is process3
extern	void process_4(uint32_t process_id);			//This is process4

VERSIONING	uint8_t	app_name[32] 		= APP_NAME;
VERSIONING	uint8_t	app_version[32] 	= APP_VERSION;

BOARDINFO_DATA_AREA	const MembraneInfo_TypeDef				MembraneFlashInfo =
{
		.header_string = "MembraneInfoStart",
		.board_address = 8,
		.board_type = SENSORS_BOARD_TYPE,
		.name_string = APP_NAME,
		.version_string = APP_VERSION,
		.Aos_version_string = A_OS_VERSION,
		.DSC_serial_string = "DSC 04",
		.DSC_date = "14/11/2024",
		.tail_string = "MembraneInfoEnd"
};

BOARDPARAMETERS_AREA	const MembraneParameters_TypeDef	MembraneFlashParameters =
{
		.header_string  = "MembraneParametersStart",
		.threshold_low  = PARAM_THRESHOLD_MIN,
		.threshold_high = PARAM_THRESHOLD_MAX,
		.hysteresis_K   = PARAM_HYSTERESIS,
		.hard_limit_low = PARAM_HARDLIMIT_LOW,
		.hard_limit_high= PARAM_HARDLIMIT_HIGH,
		.sine_number	= PARAM_SINE_NUMBER,
		.tail_string   = "MembraneParametersEnd"
};

__attribute__ ((aligned (32)))	const MembraneAppInfo_TypeDef	MembraneAppInfo =
{
		.name_string = APP_NAME,
		.version_string = APP_VERSION,
};

__attribute__ ((aligned (32)))	MembraneInfo_TypeDef		MembraneInfo;
__attribute__ ((aligned (32)))	MembraneParameters_TypeDef	MembraneParameters;

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


