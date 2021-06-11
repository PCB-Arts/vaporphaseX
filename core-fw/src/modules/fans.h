/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2021 - PCB Arts GmbH
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


#ifndef MODULES_FANS_H_
#define MODULES_FANS_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/fan.h"

//=================================
// declarations

extern pid_param_t hydro_cooler_pid;

extern struct fan_t hydro_cooler;
extern struct fan_t quick_cooler;
extern struct fan_t lid_fan;

extern void fans_init();
void fans_emergency_stop();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_FANS_H_ */
