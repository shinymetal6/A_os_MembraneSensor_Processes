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
 * process_3.c
 *
 *  Created on: May 20, 2024
 *      Author: fil
 */

#include "main.h"

#include "../Common/A_os_includes.h"

#ifdef	MEMBRANE_WS_2412171_00

void process_3(uint32_t process_id)
{
	wait_event(HW_SLEEP_FOREVER);
}

#endif // #ifdef	MEMBRANE_2412171_00
