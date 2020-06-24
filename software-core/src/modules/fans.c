/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2020 PCB Arts GmbH - Kevin Schick
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


//=================================
// included dependencies
#include "modules/fans.h"
#include "modules/temp_sensors.h"
#include "peripherals/pwm.h"

//=================================
// definitions

#define HYDRO_COLLER_TARGET_TEMP_C 60
#define HYDRO_COLLER_TARGET_LEVEL 30

struct pid_param_t hydro_cooler_pid;

struct fan_t hydro_cooler = FAN_TPL(
		&pwm_set_TIM2CH4, &TSNS_3, &hydro_cooler_pid,
		HYDRO_COLLER_TARGET_TEMP_C, HYDRO_COLLER_TARGET_LEVEL,
		FAN_MODE_PID);


#define QUICK_COLLER_TARGET_TEMP_C 60
#define QUICK_COLLER_TARGET_LEVEL 30

struct fan_t quick_cooler = FAN_TPL(
		&pwm_set_TIM2CH3, &TSNS_0, 0x0,
		QUICK_COLLER_TARGET_TEMP_C, QUICK_COLLER_TARGET_LEVEL,
		FAN_MODE_2POS);


extern void fans_init() {
	fan_set_level(&hydro_cooler, hydro_cooler.target_level);
	fan_set_level(&quick_cooler, quick_cooler.target_level);

	//link fan and temperature parameters to pid controller structure
	fan_pid_init(&hydro_cooler);
}
