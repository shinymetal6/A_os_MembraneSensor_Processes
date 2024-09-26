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
 * membrane_includes.h
 *
 *  Created on: Feb 13, 2024
 *      Author: fil
 */

#ifndef STM32U575_MEMBRANE_INCLUDES_H_
#define STM32U575_MEMBRANE_INCLUDES_H_

#include "serial_packets.h"
#include "flash_ops.h"
#include "algo.h"

extern	CRC_HandleTypeDef hcrc;
#define	FLASH_CRC	hcrc

#define		APP_NAME				"G491 IsNowFLASHED"
#define		MAX_SENSORS				16
#define		MAX_LINES				4
#define		MAX_BOARDS				4
#define		USB_BUF_LEN				64
#define 	SINGLE_PKT_SIZE			6
#define		SENSORS_NUM				MAX_SENSORS
#define		SENSORS_LINE			MAX_LINES

#define		WATER_SENSOR			1
#define		TEMPERATURE_SENSOR		2
#define		SENSORS_BOARD_TYPE		WATER_SENSOR

#define		MAILBOX_ID				0
#define		MAILBOX_LEN				32
#define		SENSORS_TX_LEN			32
#define		SAMPLES_LEN				32
#define		SENSORS_RX_LEN266		266
#define		SENSORS_RX_CMDLEN		64
#define		SENSORS_RX_CMDLEN4		4
#define		SENSORS_RX_TIMEOUT		10
#define		SENSORS_RX_DISABLE		4
#define		SENSORS_BROADCAST_ADDR	0xff

#define		PRC1_TIMER_INTERVAL			100

#define		SENSORS_UPDATE_LEN			SENSORS_RX_LEN266	// <DAC[256 bytes data][4 bytes CRC32]D> , crc is on bytes data only
#define		SENSORS_UPDATE_PAYLOAD		256
#define		SENSORS_INITIATOR			0
#define		SENSORS_CMD					1
#define		SENSORS_ADDRESS				2
/* update packet definitions */
#define		SENSORS_UPDATE_PKTCNT		3
#define		SENSORS_UPDATE_DATA			4
#define		SENSORS_UPDATE_CRC_HH		(SENSORS_UPDATE_LEN-6)
#define		SENSORS_UPDATE_CRC_HL		(SENSORS_UPDATE_LEN-5)
#define		SENSORS_UPDATE_CRC_LH		(SENSORS_UPDATE_LEN-4)
#define		SENSORS_UPDATE_CRC_LL		(SENSORS_UPDATE_LEN-3)
#define		SENSORS_UPDATE_CRC_HH_SHIFT	24
#define		SENSORS_UPDATE_CRC_HL_SHIFT	16
#define		SENSORS_UPDATE_CRC_LH_SHIFT	8
#define		SENSORS_UPDATE_CRC_LL_SHIFT	0
#define		SENSORS_UPDATE_CLOSING_FLAG	(SENSORS_UPDATE_LEN-2)
#define		SENSORS_UPDATE_TERMINATOR	(SENSORS_UPDATE_LEN-1)
/* standard packet definitions */
#define		SENSORS_STD_CLOSING_FLAG	3

#define		SENSORS_ADDRESS				2

#define		SENSORS_BROADCAST_ADDR	0xff

#define		CHECK_BIT_SIZE			8

#define		FLASHRAM_SIZE			(SENSORS_UPDATE_PAYLOAD*256)
#define		FLASH_PARAMETER_SIZE	(2*FLASH_PAGE_SIZE)

#define		MEMBRANE_DAC_WAVETABLE_SIZE	256
/*
 * Commands
 */
#define	DOWNLOAD_PREPARE_COMMAND			'U'
#define	DOWNLOAD_COMMAND					'D'
#define	SINGLE_PACKET_DOWNLOAD_COMMAND		'S'
#define	DOWNLOAD_CHECK_COMMAND				'C'
#define	DOWNLOAD_FLASH_COMMAND				'F'
#define	WRITE_FLASH_COMMAND					'W'
#define	SENSORS_GET_DATA					'A'
#define	SENSORS_DISCOVERY					'Z'
#define	SENSORS_INITIATOR_CHAR				'<'
#define	SENSORS_TERMINATOR_CHAR				'>'

/*
 * RS485 Replies
 */
#define	DOWNLOAD_PREPARE_COMMAND_REPLY		DOWNLOAD_PREPARE_COMMAND
#define	DOWNLOAD_COMMAND_REPLY				DOWNLOAD_COMMAND
#define	DOWNLOAD_CHECK_COMMAND_REPLY_OK		'O'
#define	DOWNLOAD_CHECK_COMMAND_REPLY_NOK	'N'	/* 32 bits packet fail follows */
#define	DOWNLOAD_FLASH_COMMAND_REPLY		DOWNLOAD_FLASH_COMMAND
#define	SENSORS_GET_DATA_COMMAND_REPLY		SENSORS_GET_DATA
#define	SENSORS_DISCOVERY_REPLY				SENSORS_DISCOVERY

typedef struct
{
	uint8_t 		sensors_status;
	uint8_t 		sensor_rxbuf[SENSORS_RX_LEN266];
	uint8_t 		sensor_rxchar;
	uint32_t 		sensor_rxindex;
	uint32_t 		sensor_total_rxcount;
	uint8_t 		sensor_rxstate;
	uint8_t 		sensor_addressed_sensor;
	char			sensor_txbuf[SENSORS_TX_LEN];
	char			work_sensor_txbuf[SENSORS_TX_LEN];
	uint32_t 		work_sensor_txbuflen;
	uint32_t		sensor_rxpacket_counter;
	uint32_t		sensor_check_bits[CHECK_BIT_SIZE];
	uint32_t		sensor_check_bits_num;
	/* sensors control */
	/* update section */
	uint32_t		flash_datalen;
	uint32_t		flash_counter;
	uint8_t			*flash_address;
	uint32_t		flash_line_num;
	uint8_t 		flash_flags;
	uint8_t 		flash_pktcntr;
	uint32_t		flash_pkt_crc;
	uint32_t		flash_download_crc_error;
	/* others */
	uint8_t			name_version_string[USB_BUF_LEN];
	uint8_t			name_version_string_len;
}MembraneSystem_TypeDef;

typedef struct
{
	uint8_t			header_string[32];
	uint8_t			board_address;
	uint8_t			board_type;
	uint8_t			name_string[32];
	uint8_t			version_string[32];
	uint8_t			Aos_version_string[32];
	uint8_t			tail_string[32];
}MembraneInfo_TypeDef;

typedef struct
{
	uint8_t 		acquisition_status;
	uint8_t			dac_state_machine;
	uint32_t		operation_counter;
	uint32_t		cycle_operation_counter;
	//uint16_t		dac_data[4];
	uint16_t		dac_out_value;
	uint16_t		internal_scale_factor;
	uint32_t		temperature_data;
	uint32_t		vrefint_data;
	uint32_t		calibration_value;
	uint32_t		conductivity_value;
}AcqSystem_TypeDef;

/* sensors_status */
#define	ACQ_DAC_RUN				0x01
#define	ACQ_DAC_STARTED			0x02
#define	ACQ_DAC_CYCLE_COMPLETE	0x04
#define	ACQ_ADC_CALIBRATED		0x08
#define	ACQ_ADC_RUN				0x10
#define	ACQ_ADC_STARTED			0x20
#define	ACQ_ADC_CYCLE_COMPLETE	0x40
#define	ACQ_COMPLETE			0x80

#define	SENSORS_WAIT_INITIATOR_CHAR			0
#define	SENSORS_DATA_PHASE					1
#define	SENSORS_UPDATE_PACKET_PHASE			2
#define	SENSORS_UPDATEINFO_PACKET_PHASE		3
#define	SENSORS_UPDATE_SINGLE_PACKET_PHASE	4
#define	SENSORS_UPDATE_CHECK_PHASE			5
#define	SENSORS_ACQUISITION_PACKET_PHASE	6

/* sensor_rxstate */

// not used									0x10
// not used									0x20
// not used									0x40
// not used									0x80

/* sensors_status */
#define	SENSORS_RUN				0x01
#define	SENSORS_DAC_STARTED		0x02
#define	SENSORS_PKT_PENDING		0x40
#define	SENSORS_POWERED			0x80

/* usb_flags */
#define	USB_FLAGS_LOGOSENT		0x02
#define	USB_FLAGS_SENDINFO		0x04
#define	USB_FLAGS_SENDDATA		0x08
#define	USB_FLAGS_SENDREPLY		0x10
// not used						0x20
#define	USB_FLAGS_HEADEROK		0x40
#define	USB_FLAGS_PKTCOMPLETE	0x80

/* flash_flags */
#define	FLASH_UPDATECMD_RXED	0x01
#define	FLASH_DATA_PHASE		0x02
#define	FLASH_DATA_PHASE_END	0x04
#define	FLASH_CHECK_PHASE		0x08
#define	FLASH_PROG_START		0x10
// not used						0x20
#define	FLASH_SOME_ERRORS		0x40
#define	FLASH_READY2FLASH		0x80

#define	LINE_PROCESS_ID				1
#define	LINE_LEN					32

#define	NUM_ANALOG_CHANNELS			3
#define	DAC_ANALOG_INDEX			0
#define	DAC_VREFINT_INDEX			1
#define	DAC_TEMPERATURE_INDEX		2
#define	NUM_CALIBRATION_CHANNELS	2
#define	DAC_DIRECT_INDEX			0
#define	DAC_DAMPED_INDEX			1

#define	DAC_STATE_IDLE				0
#define	DAC_STATE_CALIBRATION		1
#define	DAC_STATE_SINEGEN			2
#define	DAC_STATE_ACQUISITION		3
#define	DAC_STATE_CLOSING_CYCLE		4
#define	DAC_STATE_CYCLE_END			5

#define	DAC_NUM_CALIBRATION			8
#define	ADC_NUM_CALIBRATION_CYCLES	32
#define	DAC_NUM_SINES				4
#define	DAC_NUM_ACQUISITION			32
#define	ADC_NUM_ACQUISITION_CYCLES	256
#define	DAC_ACQUIRE_TEMP			2
#define	DAC_ACQUIRE_DATA			1
#define	DAC_NUM_CLOSING				2

#define	DAC_MAX_VALUE				4095
#define	STEADY_VALUE				(DAC_MAX_VALUE / 2)
#define	CALIBRATION_VALUE			64

#define	MINIMUM_THRESHOLD			(DAC_MAX_VALUE / 4)
#define	MAXIMUM_THRESHOLD			(STEADY_VALUE+MINIMUM_THRESHOLD)

#define	PRC1_MAILBOX_ID				1
#define	PRC1_MAILBOX_LEN			8
#define	PRC1_MAILBOX_MSGLEN			4
#define	COMM_PROCESS_ID				1
#define	ACQUISITION_PROCESS_ID		2

extern	uint32_t	valid_samples,invalid_samples;
extern	uint8_t		sensors_samples[MAX_LINES][MAX_SENSORS][SENSORS_TX_LEN];
extern	uint8_t		sensors_map[MAX_LINES][MAX_SENSORS][SENSORS_TX_LEN];

extern	MembraneSystem_TypeDef	MembraneSystem;
extern	AcqSystem_TypeDef		AcqSystem;

extern	uint8_t Host_decode_USB_packet(uint8_t* Buf);
extern	uint8_t Host_pack_USB_packet(uint8_t *rx_buf,uint8_t len);
extern	void	System_Process_USB_Replies(void);
extern	void	Create_VersionString(void);

/* activators.c */
extern	uint8_t execute_command(uint8_t *cmd, uint8_t cmd_size);
extern	void gpio_init(void);
extern	void select_sensor_line(uint8_t line);
extern	void set_sensor_line_re(uint8_t re_line);

/* acquisition */
extern	void algo_run_acquisition(void);



#endif /* STM32U575_MEMBRANE_INCLUDES_H_ */
