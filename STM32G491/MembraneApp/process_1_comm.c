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
#include "A_os_includes.h"


#ifdef	MEMBRANE_2412171_00
#include "membrane_includes.h"

MembraneSystem_TypeDef			MembraneSystem;
uint8_t 						uart_rx_buffer[SENSORS_RX_LEN+10];

extern	MembraneInfo_TypeDef	MembraneInfo;
uint8_t							mailbox_out[PRC1_MAILBOX_LEN];

void process_1_comm(uint32_t process_id)
{
uint32_t	wakeup,flags;

	allocate_hw(HW_UART1,0);
	hw_receive_uart(HW_UART1,&MembraneSystem.sensor_rxchar,1,1000);
	MembraneSystem.sensor_rxindex = 0;
	mailbox_out[0] = 0xde;
	mailbox_out[1] = 0xad;
	mailbox_out[2] = 0xbe;
	mailbox_out[3] = 0xef;

	clear_flash_area();
	create_timer(TIMER_ID_0,500,TIMERFLAGS_FOREVER | TIMERFLAGS_ENABLED);
	MembraneSystem.sensor_rxstate = SENSORS_WAIT_INITIATOR_CHAR;

	while(1)
	{
		wait_event(EVENT_TIMER | EVENT_UART1_IRQ | EVENT_ADC1_IRQ | EVENT_DAC_IRQ);
		get_wakeup_flags(&wakeup,&flags);
		MembraneSystem.work_sensor_txbuflen = 0;

		if (( wakeup & WAKEUP_FROM_TIMER) == WAKEUP_FROM_TIMER)
		{
		}
		if (( wakeup & WAKEUP_FROM_UART1_IRQ) == WAKEUP_FROM_UART1_IRQ)
		{
			if (( flags & WAKEUP_FLAGS_UART_RX) == WAKEUP_FLAGS_UART_RX)
			{
				if ( packet_process_commands() == 0 )
				{
					mbx_send(ACQUISITION_PROCESS_ID,PRC1_MAILBOX_ID,mailbox_out,PRC1_MAILBOX_MSGLEN);
				}
			}
		}
		send_work_uart_packet();
	}
}

#endif // #ifdef	MEMBRANE_2412171_00
