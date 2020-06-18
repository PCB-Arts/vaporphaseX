/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 PCB Arts GmbH - Kevin Schick
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  _____    ____    ____
 *       \  /       |    \        /\            |
 *   ____/  |       |____/       /  \     __  __|__   ___
 *  |       |       |    \      /____\   /      |    |___
 *  |       \____   |____/     /      \  |      |     ___|
 *
 *         _    __   __   __     __         _    __   __
 *  \  /  /_\  |__| |  | |__|   |__| |__|  /_\  |__  |__
 *   \/  /   \ |    |__| | \    |    |  | /   \  __| |__
 *
 *
 *  PCB Arts GmbH
 *  Kurgartenstrasse 59
 *  90762 Fuerth
 *  GERMANY
 *
 *  www.pcb-arts.com
 *  mail(at)pcb-arts.com
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


#ifndef MODULES_HEATER_H_
#define MODULES_HEATER_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/max31856/MAX31856.h"

#include <stdint.h>

//=================================
// declarations

#define HEATER_ERR_SENSOR	0b00000001
#define HEATER_ERR_MAX_TEMP	0b00000010
#define HEATER_ERR_WATCHDOG	0b00000100
#define HEATER_ERR_OTP_TRIPPED	0b00001000

extern void heater_init(struct MAX31856_t* s);
extern void heater_worker();
extern void heater_seconds_sync();
extern uint8_t heater_get_flags();
extern void heater_reset_flags();
extern int heater_temperature_reached();

extern void heater_set_temperature(float temperature);
extern void heater_set_hysteresis(float hysteresis);

extern void heater_regulation_enable();
extern void heater_regulation_disable();
extern void heater_enable();
extern void heater_disable();

extern void heater_overtemp();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_HEATER_H_ */
