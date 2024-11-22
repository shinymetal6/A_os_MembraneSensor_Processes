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
 * serial_packets.h
 *
 *  Created on: Sep 12, 2024
 *      Author: fil
 */


#ifndef STM32G491_MEMBRANEWSAPP_SERIAL_PACKETS_H_
#define STM32G491_MEMBRANEWSAPP_SERIAL_PACKETS_H_


#define	PKT_NOT_COMPLETE		0
#define	UPD_INFOPKT_COMPLETE	1
#define	UPD_PKT_COMPLETE		2
#define	UPD_PARAMS_PKT_COMPLETE	3
#define	UPD_SINGLE_PKT_COMPLETE	4
#define	UPD_PKT_STATREQUEST		5
#define	UPD_START_FLASH			6
#define	CMD_PKT_COMPLETE		7
#define	CMD_SPECIAL_CMDS		8
#define	CMD_INFOREQUEST_CMDS	9
#define	CMD_VERINFOREQUEST_CMDS	10

extern	uint8_t packet_process_commands(void);
extern	void send_work_uart_packet(void);


#endif /* STM32G491_MEMBRANEWSAPP_SERIAL_PACKETS_H_ */
