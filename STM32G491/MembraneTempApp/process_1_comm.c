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
 * process_1_comm.c
 *
 *  Created on: May 20, 2024
 *      Author: fil
 */


/*
 * 	sprintf((char *)MembraneSystem.update_line,"<U0 datalen %d U>",(int )len );
 *
 */
#include "main.h"

#include "../Common/A_os_includes.h"

#ifdef	MEMBRANE_TEMP_2412171_00
#include "membrane_includes.h"

MembraneSystem_TypeDef				MembraneSystem;
uint8_t 							uart_rx_buffer[SENSORS_RX_LEN266+10];

extern	MembraneInfo_TypeDef		MembraneFlashInfo;
extern	MembraneInfo_TypeDef		MembraneInfo;
extern	MembraneParameters_TypeDef	MembraneParameters;


extern	uint8_t						reprog_data_area[FLASHRAM_SIZE];
uint32_t	sizeMembraneFlashInfo;

void process_1_comm(uint32_t process_id)
{
uint32_t	wakeup,flags;

	sizeMembraneFlashInfo = sizeof(MembraneFlashInfo);
	bcopy(&MembraneFlashInfo,&MembraneInfo,sizeof(MembraneFlashInfo));
	allocate_hw(HW_UART1,0);
	hw_receive_uart(HW_UART1,&MembraneSystem.sensor_rxchar,1,1000);
	MembraneSystem.sensor_rxindex = 0;
	clear_flash_area();

	create_timer(TIMER_ID_0,100,TIMERFLAGS_FOREVER | TIMERFLAGS_ENABLED);
	MembraneSystem.sensor_rxstate = SENSORS_WAIT_INITIATOR_CHAR;

	while(1)
	{
		wait_event(EVENT_TIMER | EVENT_UART1_IRQ );
		get_wakeup_flags(&wakeup,&flags);
		MembraneSystem.work_sensor_txbuflen = 0;

		if (( wakeup & WAKEUP_FROM_TIMER) == WAKEUP_FROM_TIMER)
		{
			if (( MembraneSystem.flash_flags & (FLASH_PROG_FW | FLASH_READY2FLASH) ) == (FLASH_PROG_FW | FLASH_READY2FLASH))
				do_flash_update((uint8_t *)&reprog_data_area,FLASHRAM_SIZE-FLASH_PARAMETER_SIZE);

			if (( MembraneSystem.flash_flags & (FLASH_PROG_PARAMS | FLASH_READY2FLASH) ) == (FLASH_PROG_PARAMS | FLASH_READY2FLASH))
				do_params_update();
		}
		if (( wakeup & WAKEUP_FROM_UART1_IRQ) == WAKEUP_FROM_UART1_IRQ)
		{
			if (( flags & WAKEUP_FLAGS_UART_RX) == WAKEUP_FLAGS_UART_RX)
				packet_process_commands();
		}
		send_work_uart_packet();
	}
}

#endif // #ifdef	MEMBRANE_2412171_00
