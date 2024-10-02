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
 * algo.h
 *
 *  Created on: Sep 13, 2024
 *      Author: fil
 */


#ifndef STM32G491_MEMBRANEWSAPP_ALGO_H_
#define STM32G491_MEMBRANEWSAPP_ALGO_H_



extern	void algo_init(void);
extern	void algo_set_dac_complete(void);
extern	void algo_periodic_worker(void);
extern	void algo_calibration_worker(void);
extern	void algo_acquisition_worker(void);


#endif /* STM32G491_MEMBRANEWSAPP_ALGO_H_ */
