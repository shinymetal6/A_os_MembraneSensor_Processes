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

#include "../Common/A_os_includes.h"

#ifdef	MEMBRANE_TEMP_2412171_00
#include "membrane_includes.h"

__attribute__ ((aligned (32)))AcqSystem_TypeDef		AcqSystem;
__attribute__ ((aligned (32))) uint16_t	analog_buffer[NUM_ANALOG_CHANNELS];
__attribute__ ((aligned (32))) uint16_t steady_dac_tab[MEMBRANE_DAC_WAVETABLE_SIZE];

void init_dac_tab( uint16_t value )
{
uint32_t	i;

	for(i=0;i<MEMBRANE_DAC_WAVETABLE_SIZE;i++)
		steady_dac_tab[i] = 0xfff;
}

void process_2_acquisition(uint32_t process_id)
{
uint32_t	wakeup,flags;

	allocate_hw(HW_ADC1,0);
	IntAdc_Init(HW_ADC1,(uint32_t *)analog_buffer,NUM_ANALOG_CHANNELS);
	allocate_hw(HW_DAC,0);
	IntDac_Init((uint16_t *)steady_dac_tab,MEMBRANE_DAC_WAVETABLE_SIZE);

	create_timer(TIMER_ID_0,1000,TIMERFLAGS_FOREVER | TIMERFLAGS_ENABLED);
	IntDac_Start(0,DAC_WAKEUP_AT_CYCLE);
	IntAdc_Start(HW_ADC1);

	while(1)
	{
		wait_event(EVENT_TIMER | EVENT_DAC_IRQ);
		get_wakeup_flags(&wakeup,&flags);

		if (( wakeup & WAKEUP_FROM_TIMER) == WAKEUP_FROM_TIMER)
		{
		}

		if (( wakeup & WAKEUP_FROM_DAC_IRQ) == WAKEUP_FROM_DAC_IRQ)
		{
			AcqSystem.pt1000_data		= analog_buffer[DAC_ANALOG_INDEX];
			AcqSystem.vrefint_data		= __LL_ADC_CALC_VREFANALOG_VOLTAGE(analog_buffer[DAC_VREFINT_INDEX], LL_ADC_RESOLUTION_12B);
			AcqSystem.temperature_data	= __LL_ADC_CALC_TEMPERATURE(AcqSystem.vrefint_data, analog_buffer[DAC_TEMPERATURE_INDEX], LL_ADC_RESOLUTION_12B);
			AcqSystem.temperature_data -= 5;
		}
	}
}

#endif // #ifdef	MEMBRANE_2412171_00
