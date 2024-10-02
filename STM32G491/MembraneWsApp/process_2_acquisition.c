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
 * process_2_acquisition.c
 *
 *  Created on: May 20, 2024
 *      Author: fil
 */

#include "main.h"

#include "../MembraneWsApp/A_os_includes.h"

#ifdef	MEMBRANE_2412171_00
#include "../MembraneWsApp/membrane_includes.h"


extern	AcqSystem_TypeDef		AcqSystem;

extern	 uint16_t	analog_buffer[NUM_ANALOG_CHANNELS];
extern	 uint16_t	calibration_buffer[NUM_CALIBRATION_CHANNELS];

extern	 uint16_t 	steady_dac_tab[MEMBRANE_DAC_WAVETABLE_SIZE];
extern	 uint16_t 	closing_dac_tab[MEMBRANE_DAC_WAVETABLE_SIZE];
extern	 uint16_t 	calibration_dac_tab[MEMBRANE_DAC_WAVETABLE_SIZE];

extern	 uint16_t	calibration_results[MEMBRANE_DAC_WAVETABLE_SIZE];
extern	 uint16_t	acquisition_results[MEMBRANE_DAC_WAVETABLE_SIZE];
extern	 uint16_t	initial_dac_sine_tab[MEMBRANE_DAC_WAVETABLE_SIZE];

uint8_t						from_prc1_mbx[PRC1_MAILBOX_LEN];

void process_2_acquisition(uint32_t process_id)
{
uint32_t	wakeup,flags;

	allocate_hw(HW_ADC1,0);
	allocate_hw(HW_ADC2,0);
	IntAdc_Init(HW_ADC1,(uint32_t *)analog_buffer,NUM_ANALOG_CHANNELS);
	IntAdc_Init(HW_ADC2,(uint32_t *)calibration_buffer,NUM_CALIBRATION_CHANNELS);
	allocate_hw(HW_DAC,0);
	IntDac_Init((uint16_t *)initial_dac_sine_tab,MEMBRANE_DAC_WAVETABLE_SIZE);

	algo_init();

	create_timer(TIMER_ID_0,10,TIMERFLAGS_FOREVER | TIMERFLAGS_ENABLED);

	while(1)
	{
		wait_event(EVENT_TIMER | EVENT_ADC1_IRQ | EVENT_DAC_IRQ | EVENT_MBX);
		get_wakeup_flags(&wakeup,&flags);

  		if (( wakeup & WAKEUP_FROM_MBX) == WAKEUP_FROM_MBX)
		{
			if ( mbx_receive(PRC1_MAILBOX_ID,from_prc1_mbx) == PRC1_MAILBOX_MSGLEN)
			{
				if (( from_prc1_mbx[0] == 0xde ) && ( from_prc1_mbx[1] == 0xad )&&( from_prc1_mbx[2] == 0xbe ) && ( from_prc1_mbx[3] == 0xef ))
					algo_run_acquisition();
			}
		}

		if (( wakeup & WAKEUP_FROM_TIMER) == WAKEUP_FROM_TIMER)
		{
			algo_periodic_worker();
		}

		if (( wakeup & WAKEUP_FROM_ADC1_IRQ) == WAKEUP_FROM_ADC1_IRQ)
		{
			algo_acquisition_worker();
		}

		if (( wakeup & WAKEUP_FROM_ADC2_IRQ) == WAKEUP_FROM_ADC2_IRQ)
		{
			algo_calibration_worker();
		}

		if (( wakeup & WAKEUP_FROM_DAC_IRQ) == WAKEUP_FROM_DAC_IRQ)
		{
			algo_set_dac_complete();
		}
	}
}

#endif // #ifdef	MEMBRANE_2412171_00
