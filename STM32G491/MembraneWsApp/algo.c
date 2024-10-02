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
 * algo.c
 *
 *  Created on: Sep 13, 2024
 *      Author: fil
 */

#include "main.h"

#include "../Common/A_os_includes.h"


#ifdef	MEMBRANE_WS_2412171_00
#include "../MembraneWsApp/membrane_includes.h"

__attribute__ ((aligned (32)))AcqSystem_TypeDef		AcqSystem;

__attribute__ ((aligned (32))) uint16_t	analog_buffer[NUM_ANALOG_CHANNELS];
__attribute__ ((aligned (32))) uint16_t	calibration_buffer[NUM_CALIBRATION_CHANNELS];

__attribute__ ((aligned (32))) uint16_t steady_dac_tab[MEMBRANE_DAC_WAVETABLE_SIZE];
__attribute__ ((aligned (32))) uint16_t closing_dac_tab[MEMBRANE_DAC_WAVETABLE_SIZE];
__attribute__ ((aligned (32))) uint16_t calibration_dac_tab[MEMBRANE_DAC_WAVETABLE_SIZE];

__attribute__ ((aligned (32))) uint16_t	calibration_results[ADC_NUM_CALIBRATION_CYCLES];
__attribute__ ((aligned (32))) uint16_t	acquisition_results[ADC_NUM_ACQUISITION_CYCLES];


__attribute__ ((aligned (32))) const uint16_t initial_dac_sine_tab[MEMBRANE_DAC_WAVETABLE_SIZE] =
{
		0x800, 0x832, 0x864, 0x896, 0x8c8, 0x8fa, 0x92c, 0x95e,
		0x98f, 0x9c0, 0x9f1, 0xa22, 0xa52, 0xa82, 0xab1, 0xae0,
		0xb0f, 0xb3d, 0xb6b, 0xb98, 0xbc5, 0xbf1, 0xc1c, 0xc47,
		0xc71, 0xc9a, 0xcc3, 0xceb, 0xd12, 0xd39, 0xd5f, 0xd83,
		0xda7, 0xdca, 0xded, 0xe0e, 0xe2e, 0xe4e, 0xe6c, 0xe8a,
		0xea6, 0xec1, 0xedc, 0xef5, 0xf0d, 0xf24, 0xf3a, 0xf4f,
		0xf63, 0xf76, 0xf87, 0xf98, 0xfa7, 0xfb5, 0xfc2, 0xfcd,
		0xfd8, 0xfe1, 0xfe9, 0xff0, 0xff5, 0xff9, 0xffd, 0xffe,
		0xfff, 0xffe, 0xffd, 0xff9, 0xff5, 0xff0, 0xfe9, 0xfe1,
		0xfd8, 0xfcd, 0xfc2, 0xfb5, 0xfa7, 0xf98, 0xf87, 0xf76,
		0xf63, 0xf4f, 0xf3a, 0xf24, 0xf0d, 0xef5, 0xedc, 0xec1,
		0xea6, 0xe8a, 0xe6c, 0xe4e, 0xe2e, 0xe0e, 0xded, 0xdca,
		0xda7, 0xd83, 0xd5f, 0xd39, 0xd12, 0xceb, 0xcc3, 0xc9a,
		0xc71, 0xc47, 0xc1c, 0xbf1, 0xbc5, 0xb98, 0xb6b, 0xb3d,
		0xb0f, 0xae0, 0xab1, 0xa82, 0xa52, 0xa22, 0x9f1, 0x9c0,
		0x98f, 0x95e, 0x92c, 0x8fa, 0x8c8, 0x896, 0x864, 0x832,
		0x800, 0x7cd, 0x79b, 0x769, 0x737, 0x705, 0x6d3, 0x6a1,
		0x670, 0x63f, 0x60e, 0x5dd, 0x5ad, 0x57d, 0x54e, 0x51f,
		0x4f0, 0x4c2, 0x494, 0x467, 0x43a, 0x40e, 0x3e3, 0x3b8,
		0x38e, 0x365, 0x33c, 0x314, 0x2ed, 0x2c6, 0x2a0, 0x27c,
		0x258, 0x235, 0x212, 0x1f1, 0x1d1, 0x1b1, 0x193, 0x175,
		0x159, 0x13e, 0x123, 0x10a, 0xf2, 0xdb, 0xc5, 0xb0,
		0x9c, 0x89, 0x78, 0x67, 0x58, 0x4a, 0x3d, 0x32,
		0x27, 0x1e, 0x16, 0xf, 0xa, 0x6, 0x2, 0x1,
		0x0, 0x1, 0x2, 0x6, 0xa, 0xf, 0x16, 0x1e,
		0x27, 0x32, 0x3d, 0x4a, 0x58, 0x67, 0x78, 0x89,
		0x9c, 0xb0, 0xc5, 0xdb, 0xf2, 0x10a, 0x123, 0x13e,
		0x159, 0x175, 0x193, 0x1b1, 0x1d1, 0x1f1, 0x212, 0x235,
		0x258, 0x27c, 0x2a0, 0x2c6, 0x2ed, 0x314, 0x33c, 0x365,
		0x38e, 0x3b8, 0x3e3, 0x40e, 0x43a, 0x467, 0x494, 0x4c2,
		0x4f0, 0x51f, 0x54e, 0x57d, 0x5ad, 0x5dd, 0x60e, 0x63f,
		0x670, 0x6a1, 0x6d3, 0x705, 0x737, 0x769, 0x79b, 0x7cd,
};


void update_steady_dac_tab( uint16_t value )
{
uint32_t	i;

	for(i=0;i<MEMBRANE_DAC_WAVETABLE_SIZE;i++)
		steady_dac_tab[i] = value;
}

void algo_init(void)
{
uint32_t	i;

	AcqSystem.dac_out_value = STEADY_VALUE;
	AcqSystem.internal_scale_factor = 0;
	update_steady_dac_tab(AcqSystem.dac_out_value);

	for(i=0;i<MEMBRANE_DAC_WAVETABLE_SIZE;i++)
		calibration_dac_tab[i] = CALIBRATION_VALUE;
	for(i=0;i<MEMBRANE_DAC_WAVETABLE_SIZE;i++)
		closing_dac_tab[i] = 0;

	AcqSystem.dac_state_machine = DAC_STATE_IDLE;
}

void algo_run_acquisition(void)
{
	AcqSystem.acquisition_status |= ACQ_DAC_RUN;
}

void compile_table_val( uint16_t *table )
{
	IntDac_UpdateCurrentTab(table,MEMBRANE_DAC_WAVETABLE_SIZE);
}


void algo_set_dac_complete(void)
{
	AcqSystem.acquisition_status |= ACQ_DAC_CYCLE_COMPLETE;
}

void algo_periodic_worker(void)
{
	if (( AcqSystem.acquisition_status & ACQ_DAC_RUN) == ACQ_DAC_RUN)
	{
		AcqSystem.acquisition_status &= ~ACQ_COMPLETE;
		switch( AcqSystem.dac_state_machine )
		{
		case	DAC_STATE_IDLE :
			IntDac_Stop();
			compile_table_val(calibration_dac_tab);
			IntDac_Start(DAC_NUM_CALIBRATION,DAC_STOP_AT_END | DAC_WAKEUP_AT_CYCLE);

			AcqSystem.acquisition_status &= ~(ACQ_ADC_RUN | ACQ_DAC_CYCLE_COMPLETE);
			AcqSystem.operation_counter = DAC_NUM_CALIBRATION;

			IntAdc_Start(HW_ADC2);
			AcqSystem.cycle_operation_counter = 0;
			AcqSystem.dac_state_machine = DAC_STATE_CALIBRATION;
			break;
		case	DAC_STATE_CALIBRATION :
			if (( AcqSystem.acquisition_status & ACQ_DAC_CYCLE_COMPLETE ) == ACQ_DAC_CYCLE_COMPLETE)
			{
				IntAdc_Stop(HW_ADC2);
				IntDac_Stop();
				IntAdc_Start(HW_ADC1);

				compile_table_val((uint16_t  *)initial_dac_sine_tab);
				AcqSystem.operation_counter = DAC_NUM_SINES;
				AcqSystem.cycle_operation_counter = 0;
				IntDac_Start(DAC_NUM_SINES,DAC_STOP_AT_END | DAC_WAKEUP_AT_CYCLE);

				AcqSystem.acquisition_status &= ~ACQ_DAC_CYCLE_COMPLETE;
				AcqSystem.dac_state_machine = DAC_STATE_SINEGEN;
			}
			break;
		case	DAC_STATE_SINEGEN :
			if (( AcqSystem.acquisition_status & ACQ_DAC_CYCLE_COMPLETE ) == ACQ_DAC_CYCLE_COMPLETE)
			{
				AcqSystem.acquisition_status &= ~ACQ_DAC_CYCLE_COMPLETE;
				AcqSystem.operation_counter = DAC_NUM_ACQUISITION;
				AcqSystem.cycle_operation_counter = 0;
				IntDac_Stop();
				compile_table_val(steady_dac_tab);
				IntDac_Start(DAC_NUM_ACQUISITION,DAC_STOP_AT_END | DAC_WAKEUP_AT_CYCLE);

				AcqSystem.acquisition_status |= ACQ_ADC_RUN;
				AcqSystem.dac_state_machine = DAC_STATE_ACQUISITION;
			}
			break;
		case	DAC_STATE_ACQUISITION :
			if (( AcqSystem.acquisition_status & ACQ_DAC_CYCLE_COMPLETE ) == ACQ_DAC_CYCLE_COMPLETE)
			{
				IntAdc_Stop(HW_ADC1);
				IntDac_Stop();

				compile_table_val(closing_dac_tab);
				IntDac_Start(DAC_NUM_CLOSING,DAC_STOP_AT_END | DAC_WAKEUP_AT_CYCLE | DAC_3ST_AT_END);

				AcqSystem.acquisition_status &= ~ACQ_ADC_RUN;
				AcqSystem.dac_state_machine = DAC_STATE_CLOSING_CYCLE;
			}
			break;
		case	DAC_STATE_CLOSING_CYCLE :
			if (( AcqSystem.acquisition_status & ACQ_DAC_CYCLE_COMPLETE ) == ACQ_DAC_CYCLE_COMPLETE)
			{
				IntDac_Stop();
				AcqSystem.dac_state_machine = DAC_STATE_CYCLE_END;
			}
			break;
		case	DAC_STATE_CYCLE_END :
			AcqSystem.dac_state_machine = DAC_STATE_IDLE;
			AcqSystem.acquisition_status = ACQ_COMPLETE;
			break;
		}
	}
}

void algo_calibration_worker(void)
{
uint32_t	i;
	if (( AcqSystem.acquisition_status & ACQ_ADC_CALIBRATED ) == 0 )
	{
		calibration_results[AcqSystem.cycle_operation_counter & 0xff] = calibration_buffer[DAC_DIRECT_INDEX];
		if (AcqSystem.cycle_operation_counter >= ADC_NUM_CALIBRATION_CYCLES)
		{
			AcqSystem.acquisition_status |= ACQ_ADC_CALIBRATED;
			AcqSystem.calibration_value = 0;
			for(i=0;i<ADC_NUM_CALIBRATION_CYCLES;i++)
				AcqSystem.calibration_value += calibration_results[i];
			AcqSystem.calibration_value /= ADC_NUM_CALIBRATION_CYCLES;
			if ( AcqSystem.calibration_value > CALIBRATION_VALUE)
				AcqSystem.calibration_value -= CALIBRATION_VALUE;
			else
				AcqSystem.calibration_value = CALIBRATION_VALUE - AcqSystem.calibration_value;
		}
		AcqSystem.cycle_operation_counter++;
	}
}

void algo_acquisition_worker(void)
{
uint32_t	i;

	if (( AcqSystem.acquisition_status & ACQ_ADC_CYCLE_COMPLETE ) == 0 )
	{
		acquisition_results[AcqSystem.cycle_operation_counter & 0xff] = analog_buffer[DAC_ANALOG_INDEX];
		if (AcqSystem.cycle_operation_counter >= ADC_NUM_ACQUISITION_CYCLES)
		{
			AcqSystem.acquisition_status |= ACQ_ADC_CYCLE_COMPLETE;
			AcqSystem.conductivity_value = 0;
			for(i=0;i<ADC_NUM_ACQUISITION_CYCLES;i++)
				AcqSystem.conductivity_value += acquisition_results[i];
			AcqSystem.conductivity_value /= ADC_NUM_ACQUISITION_CYCLES;
			if ( AcqSystem.conductivity_value < MINIMUM_THRESHOLD)
			{
				if ( AcqSystem.internal_scale_factor < 6)
					AcqSystem.internal_scale_factor ++;
			}
			if ( AcqSystem.conductivity_value > MAXIMUM_THRESHOLD)
			{
				if ( AcqSystem.internal_scale_factor )
					AcqSystem.internal_scale_factor --;
			}
			IntAdc_OpAmpGain(AcqSystem.internal_scale_factor);
			AcqSystem.conductivity_value = DAC_MAX_VALUE - AcqSystem.conductivity_value;
			AcqSystem.vrefint_data     = __LL_ADC_CALC_VREFANALOG_VOLTAGE(analog_buffer[DAC_VREFINT_INDEX], LL_ADC_RESOLUTION_12B);
			AcqSystem.temperature_data = __LL_ADC_CALC_TEMPERATURE(AcqSystem.vrefint_data, analog_buffer[DAC_TEMPERATURE_INDEX], LL_ADC_RESOLUTION_12B);
			AcqSystem.temperature_data -= 5;
		}
		AcqSystem.cycle_operation_counter++;
	}
}

#endif // #ifdef	MEMBRANE_2412171_00

