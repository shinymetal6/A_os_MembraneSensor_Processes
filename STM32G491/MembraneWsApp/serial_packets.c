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
 * serial_packets.c
 *
 *  Created on: Sep 12, 2024
 *      Author: fil
 */
#include "main.h"

#include "../MembraneWsApp/A_os_includes.h"


#ifdef	MEMBRANE_WS_2412171_00
#include "../MembraneWsApp/membrane_includes.h"
extern	MembraneInfo_TypeDef	MembraneInfo;
extern	uint8_t	reprog_data_area[FLASHRAM_SIZE];

void set_check_bits_error(uint32_t flash_pktcntr)
{
uint32_t sensor_check_bitsindex;
	sensor_check_bitsindex = flash_pktcntr / 32;
	MembraneSystem.sensor_check_bits[sensor_check_bitsindex] |= 1 << (flash_pktcntr & 0x1f );
	MembraneSystem.sensor_check_bits_num ++;
}

void clear_check_bits_error(uint32_t flash_pktcntr)
{
uint32_t sensor_check_bitsindex;
	sensor_check_bitsindex = flash_pktcntr / 32;
	MembraneSystem.sensor_check_bits[sensor_check_bitsindex] &= ~(1 << (flash_pktcntr & 0x1f ));
	if ( MembraneSystem.sensor_check_bits_num )
		MembraneSystem.sensor_check_bits_num --;
}

uint8_t packet_assemble(void)
{
uint8_t ret_val = PKT_NOT_COMPLETE;

	if ( MembraneSystem.sensor_rxstate == SENSORS_WAIT_INITIATOR_CHAR)
	{
		if(MembraneSystem.sensor_rxchar == SENSORS_INITIATOR_CHAR)
		{
			bzero(MembraneSystem.sensor_rxbuf,SENSORS_RX_LEN266);
			MembraneSystem.sensor_rxbuf[0] = SENSORS_INITIATOR_CHAR;
			MembraneSystem.sensor_rxstate = SENSORS_DATA_PHASE;
			MembraneSystem.sensor_rxindex = 1;
		}
	}
	else
	{
		MembraneSystem.sensor_rxbuf[MembraneSystem.sensor_rxindex] = MembraneSystem.sensor_rxchar;
		if ( MembraneSystem.sensor_rxindex == 1)
		{
			switch(MembraneSystem.sensor_rxbuf[1])
			{
			case DOWNLOAD_COMMAND 				:
			case DOWNLOAD_PARAMS_COMMAND 		:
			case SINGLE_PACKET_DOWNLOAD_COMMAND :
			case DOWNLOAD_PREPARE_COMMAND 		:
				MembraneSystem.sensor_total_rxcount = SENSORS_RX_LEN266;
				break;
			case DOWNLOAD_CHECK_COMMAND :
			case WRITE_FLASH_COMMAND :
			case SENSORS_DISCOVERY :
			case SENSORS_GET_DATA :
				MembraneSystem.sensor_total_rxcount = SENSORS_RX_CMDLEN4;
				break;
			case SENSORS_SPECIAL_CMDS :
				MembraneSystem.sensor_total_rxcount = SENSORS_RX_CMDLEN4;
				break;
			case SENSORS_INFOREQ_CMDS :
				MembraneSystem.sensor_total_rxcount = SENSORS_RX_CMDLEN4;
				break;
			default:
				MembraneSystem.sensor_rxstate = SENSORS_WAIT_INITIATOR_CHAR;
			}
			MembraneSystem.sensor_rxindex = 2;
		}
		else
		{
			MembraneSystem.sensor_rxbuf[MembraneSystem.sensor_rxindex] = MembraneSystem.sensor_rxchar;
			MembraneSystem.sensor_rxindex ++;
			if (MembraneSystem.sensor_rxbuf[1] == SENSORS_SPECIAL_CMDS)
			{
				if ( MembraneSystem.sensor_rxchar == SENSORS_TERMINATOR_CHAR)
					ret_val = CMD_SPECIAL_CMDS;
			}
			else if (MembraneSystem.sensor_rxbuf[1] == SENSORS_INFOREQ_CMDS)
			{
				if ( MembraneSystem.sensor_rxchar == SENSORS_TERMINATOR_CHAR)
					ret_val = CMD_INFOREQUEST_CMDS;
			}
			else
			{
				if ( MembraneSystem.sensor_rxindex == (MembraneSystem.sensor_total_rxcount))
				{
					switch(MembraneSystem.sensor_rxbuf[1])
					{
					case DOWNLOAD_COMMAND :
						ret_val = UPD_PKT_COMPLETE;
						break;
					case DOWNLOAD_PARAMS_COMMAND :
						ret_val = UPD_PARAMS_PKT_COMPLETE;
						break;
					case SENSORS_INFOREQ_CMDS :
						ret_val = CMD_INFOREQUEST_CMDS;
						break;
					case SINGLE_PACKET_DOWNLOAD_COMMAND :
						ret_val = UPD_SINGLE_PKT_COMPLETE;
						break;
					case DOWNLOAD_PREPARE_COMMAND :
						ret_val = UPD_INFOPKT_COMPLETE;
						break;
					case DOWNLOAD_CHECK_COMMAND :
						ret_val = UPD_PKT_STATREQUEST;
						break;
					case WRITE_FLASH_COMMAND :
						ret_val = UPD_START_FLASH;
						break;
					case SENSORS_DISCOVERY :
					case SENSORS_GET_DATA :
						ret_val = CMD_PKT_COMPLETE;
						break;
					default:
						ret_val = PKT_NOT_COMPLETE;
					}
				}
			}
		}
	}
	if ( ret_val != PKT_NOT_COMPLETE)
	{
		MembraneSystem.sensor_rxstate = SENSORS_WAIT_INITIATOR_CHAR;
		if ((MembraneSystem.sensor_rxbuf[SENSORS_ADDRESS] != SENSORS_BROADCAST_ADDR) && ( MembraneSystem.sensor_rxbuf[SENSORS_ADDRESS] != MembraneInfo.board_address))
			ret_val = PKT_NOT_COMPLETE;
		else
			MembraneSystem.sensor_addressed_sensor = MembraneSystem.sensor_rxbuf[SENSORS_ADDRESS];

	}
	return ret_val;
}

uint32_t		updsingle_ptr;

void update_packet_process(uint8_t assemble_result)
{
int 		pnum;
uint32_t	i;

	if ( assemble_result == UPD_INFOPKT_COMPLETE )
	{
		pnum = sscanf((char *)&MembraneSystem.sensor_rxbuf[SENSORS_UPDATE_DATA],"datalen %d 0x%08x U>",(int *)&MembraneSystem.flash_datalen,(int *)&MembraneSystem.flash_pkt_crc);
		if ( pnum == 2 )
		{
			MembraneSystem.flash_flags |= FLASH_UPDATECMD_RXED;
			MembraneSystem.flash_address =  (uint8_t *)&reprog_data_area;
			MembraneSystem.flash_counter = 0;
			MembraneSystem.flash_pktcntr = 0;
			MembraneSystem.flash_line_num = 0;
			for(i=0;i<CHECK_BIT_SIZE;i++)
				MembraneSystem.sensor_check_bits[i] = 0;
			MembraneSystem.sensor_check_bits_num = 0;
			MembraneSystem.sensor_rxpacket_counter = 0;
			clear_flash_area();
		}
	}
	else if ( assemble_result == UPD_PKT_COMPLETE )
	{
		if ((MembraneSystem.flash_flags & FLASH_UPDATECMD_RXED ) == FLASH_UPDATECMD_RXED )
		{
			if (MembraneSystem.sensor_rxbuf[SENSORS_UPDATE_TERMINATOR] == '>' )
			{
				MembraneSystem.flash_flags |= FLASH_DATA_PHASE;
				MembraneSystem.flash_pkt_crc = do_crc((uint32_t *)&MembraneSystem.sensor_rxbuf[SENSORS_UPDATE_DATA],SENSORS_UPDATE_PAYLOAD+4);
				if (( MembraneSystem.flash_pkt_crc )|| (MembraneSystem.flash_pktcntr != MembraneSystem.sensor_rxbuf[SENSORS_UPDATE_PKTCNT]))
				{
					set_check_bits_error(MembraneSystem.sensor_rxpacket_counter);
					MembraneSystem.flash_download_crc_error++;
				}
				else
				{
					for(i=0;i<SENSORS_UPDATE_PAYLOAD;i++,MembraneSystem.flash_address++)
						*MembraneSystem.flash_address = MembraneSystem.sensor_rxbuf[i+SENSORS_UPDATE_DATA];
				}
				MembraneSystem.flash_counter+=SENSORS_UPDATE_PAYLOAD;
				if ( MembraneSystem.flash_counter == MembraneSystem.flash_datalen)
				{
					MembraneSystem.flash_flags &= ~FLASH_DATA_PHASE;
					MembraneSystem.flash_flags |= FLASH_DATA_PHASE_END;
					if ( MembraneSystem.flash_download_crc_error == 0 )
						MembraneSystem.flash_flags = FLASH_READY2FLASH;
					else
						MembraneSystem.flash_flags = FLASH_SOME_ERRORS;
				}
				MembraneSystem.flash_pktcntr++;
				MembraneSystem.sensor_rxpacket_counter++;
			}
			MembraneSystem.sensors_status &= ~FLASH_UPDATECMD_RXED;
		}
	}
	else if ( assemble_result == UPD_PKT_STATREQUEST )
	{
		/* no broadcast allowed on UPD_PKT_STATREQUEST */
		if ( MembraneSystem.sensor_rxbuf[SENSORS_ADDRESS] == MembraneInfo.board_address)
		{
			MembraneSystem.work_sensor_txbuf[0] = SENSORS_INITIATOR_CHAR;
			MembraneSystem.work_sensor_txbuf[1] = DOWNLOAD_CHECK_COMMAND;
			MembraneSystem.work_sensor_txbuf[2] = MembraneInfo.board_address;
			MembraneSystem.work_sensor_txbuf[3] = MembraneSystem.flash_download_crc_error >> 24;
			MembraneSystem.work_sensor_txbuf[4] = MembraneSystem.flash_download_crc_error >> 16;
			MembraneSystem.work_sensor_txbuf[5] = MembraneSystem.flash_download_crc_error >> 8;
			MembraneSystem.work_sensor_txbuf[6] = MembraneSystem.flash_download_crc_error >> 0;
			MembraneSystem.work_sensor_txbuf[7] = SENSORS_TERMINATOR_CHAR;
			MembraneSystem.work_sensor_txbuflen = 8;
		}
	}
	else if ( assemble_result == UPD_START_FLASH )
	{
		if (( MembraneSystem.flash_flags & FLASH_READY2FLASH) == FLASH_READY2FLASH)
		{
			if (( MembraneSystem.sensor_addressed_sensor != SENSORS_BROADCAST_ADDR) && (  MembraneSystem.sensor_rxbuf[SENSORS_ADDRESS] == MembraneInfo.board_address ))
			{
				MembraneSystem.work_sensor_txbuf[0] = SENSORS_INITIATOR_CHAR;
				MembraneSystem.work_sensor_txbuf[1] = WRITE_FLASH_COMMAND;
				MembraneSystem.work_sensor_txbuf[2] = MembraneInfo.board_address;
				MembraneSystem.work_sensor_txbuf[3] = SENSORS_TERMINATOR_CHAR;
				MembraneSystem.work_sensor_txbuflen = 4;
				MembraneSystem.flash_flags |= FLASH_PROG_FW;
			}
		}
	}
	else if ( assemble_result == UPD_SINGLE_PKT_COMPLETE )
	{
		/* no broadcast allowed on UPD_SINGLE_PKT_COMPLETE */
		/* SENSORS_UPDATE_PKTCNT contains the packet to be corrected */
		if ( MembraneSystem.sensor_rxbuf[SENSORS_ADDRESS] == MembraneInfo.board_address)
		{
			MembraneSystem.flash_pkt_crc = do_crc((uint32_t *)&MembraneSystem.sensor_rxbuf[SENSORS_UPDATE_DATA],SENSORS_UPDATE_PAYLOAD+4);
			if ( MembraneSystem.flash_pkt_crc == 0 )
			{
				updsingle_ptr = MembraneSystem.sensor_rxbuf[SENSORS_UPDATE_PKTCNT] * SENSORS_UPDATE_PAYLOAD;
				MembraneSystem.flash_address = &reprog_data_area[updsingle_ptr];
				for(i=0;i<SENSORS_UPDATE_PAYLOAD;i++,MembraneSystem.flash_address++)
					*MembraneSystem.flash_address = MembraneSystem.sensor_rxbuf[i+SENSORS_UPDATE_DATA];
				clear_check_bits_error(MembraneSystem.sensor_rxbuf[SENSORS_UPDATE_PKTCNT]);
				if ( MembraneSystem.sensor_check_bits_num == 0 )
					MembraneSystem.flash_flags = FLASH_READY2FLASH;
				else
					MembraneSystem.flash_flags = FLASH_SOME_ERRORS;
			}
		}
	}
	else if ( assemble_result == UPD_PARAMS_PKT_COMPLETE )
	{
		/* only broadcast allowed on UPD_PARAMS_PKT_COMPLETE */
		if ( MembraneSystem.sensor_rxbuf[SENSORS_ADDRESS] == SENSORS_BROADCAST_ADDR)
			MembraneSystem.flash_flags = FLASH_PROG_PARAMS | FLASH_READY2FLASH;
	}
}

uint8_t data_packet_process(void)
{
uint8_t ret_val = 1;
	MembraneSystem.work_sensor_txbuflen = 0;
	if (  MembraneSystem.sensor_rxbuf[SENSORS_ADDRESS] == MembraneInfo.board_address )
	{
		if (( MembraneSystem.sensor_rxbuf[SENSORS_INITIATOR] == SENSORS_INITIATOR_CHAR) && ( MembraneSystem.sensor_rxbuf[SENSORS_STD_CLOSING_FLAG] == SENSORS_TERMINATOR_CHAR))
		{
			switch ( MembraneSystem.sensor_rxbuf[SENSORS_CMD] )
			{
			case SENSORS_GET_DATA :
				MembraneSystem.work_sensor_txbuf[0] = SENSORS_INITIATOR_CHAR;
				MembraneSystem.work_sensor_txbuf[1] = SENSORS_GET_DATA_COMMAND_REPLY;
				MembraneSystem.work_sensor_txbuf[2] = MembraneInfo.board_address;
				MembraneSystem.work_sensor_txbuf[3] = SENSORS_BOARD_TYPE;
				MembraneSystem.work_sensor_txbuf[4] = 'W';
				MembraneSystem.work_sensor_txbuf[5] = AcqSystem.conductivity_value >> 8;
				MembraneSystem.work_sensor_txbuf[6] = AcqSystem.conductivity_value & 0xff;
				MembraneSystem.work_sensor_txbuf[7] = 'K';
				MembraneSystem.work_sensor_txbuf[8] = 0;
				MembraneSystem.work_sensor_txbuf[9] = (1 << AcqSystem.internal_scale_factor) & 0xff;
				MembraneSystem.work_sensor_txbuf[10] = 'S';
				MembraneSystem.work_sensor_txbuf[11] = AcqSystem.dac_out_value >> 8;
				MembraneSystem.work_sensor_txbuf[12] = AcqSystem.dac_out_value & 0xff;
				MembraneSystem.work_sensor_txbuf[13] = 'C';
				MembraneSystem.work_sensor_txbuf[14] = AcqSystem.calibration_value >> 8;
				MembraneSystem.work_sensor_txbuf[15] = AcqSystem.calibration_value & 0xff;
				MembraneSystem.work_sensor_txbuf[16] = 'T';
				MembraneSystem.work_sensor_txbuf[17] = AcqSystem.temperature_data >> 8;
				MembraneSystem.work_sensor_txbuf[18] = AcqSystem.temperature_data & 0xff;
				MembraneSystem.work_sensor_txbuf[19] = SENSORS_TERMINATOR_CHAR;
				MembraneSystem.work_sensor_txbuflen = 20;
				ret_val = 0;
				break;
			case SENSORS_DISCOVERY :
				MembraneSystem.work_sensor_txbuf[0] = SENSORS_INITIATOR_CHAR;
				MembraneSystem.work_sensor_txbuf[1] = SENSORS_DISCOVERY_REPLY;
				MembraneSystem.work_sensor_txbuf[2] = MembraneInfo.board_address;
				MembraneSystem.work_sensor_txbuf[3] = SENSORS_BOARD_TYPE;
				MembraneSystem.work_sensor_txbuf[4] = SENSORS_TERMINATOR_CHAR;
				MembraneSystem.work_sensor_txbuflen = 5;
				ret_val = 0;
				break;
			default :
				ret_val = 1;
				break;
			}
		}
	}
	return ret_val;
}

uint8_t special_packet_process(void)
{
int	pnum;

	if (( MembraneSystem.sensor_rxbuf[SENSORS_CMD]  == SENSORS_SPECIAL_CMDS ) && ( MembraneSystem.sensor_rxbuf[SENSORS_ADDRESS]  == SENSORS_BROADCAST_ADDR ))
	{
		switch ( MembraneSystem.sensor_rxbuf[SENSORS_SPECIAL_PARAM1] )
		{
		case SENSORS_SPECIAL_PARAM1_GO_NORMAL :
			MembraneSystem.sensors_status &= ~SENSORS_SPECIALMODE;
			break;
		case SENSORS_SPECIAL_PARAM1_GO_LISTENONLY :
			MembraneSystem.sensors_status |= SENSORS_SPECIALMODE;
			break;
		case SENSORS_SPECIAL_PARAM1_GET_PARAMS :
			if (( MembraneSystem.sensors_status & SENSORS_SPECIALMODE) == SENSORS_SPECIALMODE )
			{
				bzero(MembraneInfo.DSC_serial_string,MEMBRANEINFO_STD_LEN);
				bzero(MembraneInfo.DSC_date,MEMBRANEINFO_STD_LEN);
				pnum = sscanf((char *)&MembraneSystem.sensor_rxbuf[5],"%s %s",MembraneInfo.DSC_serial_string,MembraneInfo.DSC_date);
				if ( pnum == 2)
				{
					MembraneInfo.board_address = MembraneSystem.sensor_rxbuf[4];
					MembraneSystem.flash_flags = FLASH_PROG_PARAMS | FLASH_READY2FLASH;
				}
			}
			break;
		}
	}
	return 0;
}

void send_sensor_info(void)
{
	MembraneSystem.work_sensor_txbuflen = 0;
	if (  MembraneSystem.sensor_rxbuf[SENSORS_ADDRESS] == MembraneInfo.board_address )
	{
		sprintf((char *)MembraneSystem.work_sensor_txbuf,"<IAA %s %s >",(char *)MembraneInfo.DSC_serial_string,(char *)MembraneInfo.DSC_date);
		MembraneSystem.work_sensor_txbuflen = strlen((char *)MembraneSystem.work_sensor_txbuf);
		MembraneSystem.work_sensor_txbuf[2] = MembraneInfo.board_address;
		MembraneSystem.work_sensor_txbuf[3] = SENSORS_BOARD_TYPE;
	}
}

uint8_t packet_process_commands(void)
{
uint8_t		assemble_result;
uint8_t 	ret_val = 1;

	assemble_result = packet_assemble();
	if ( assemble_result != PKT_NOT_COMPLETE )
	{
		if ( assemble_result == CMD_PKT_COMPLETE )
			ret_val = data_packet_process();
		else if ( assemble_result == CMD_SPECIAL_CMDS )
			special_packet_process();
		else if ( assemble_result == CMD_INFOREQUEST_CMDS )
			send_sensor_info();
		else
			update_packet_process(assemble_result);
	}
	return ret_val;
}

void send_work_uart_packet(void)
{
uint32_t	i;

	if (( MembraneSystem.sensors_status &  SENSORS_SPECIALMODE) == 0 )
	{
		if ( MembraneSystem.work_sensor_txbuflen )
		{
			bzero(MembraneSystem.sensor_txbuf , MembraneSystem.work_sensor_txbuflen+2);
			for(i=0;i<MembraneSystem.work_sensor_txbuflen;i++)
				MembraneSystem.sensor_txbuf[i+1] = MembraneSystem.work_sensor_txbuf[i];
			MembraneSystem.sensor_txbuf[0] = 0;
			__disable_irq();
			DWT_Delay_us(100);
			__enable_irq();

			hw_send_uart_dma(HW_UART1,(uint8_t *)MembraneSystem.sensor_txbuf,MembraneSystem.work_sensor_txbuflen+2);
			MembraneSystem.work_sensor_txbuflen = 0;
		}
	}
}

#endif // #ifdef	MEMBRANE_2412171_00

