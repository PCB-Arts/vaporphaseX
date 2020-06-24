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


#ifndef MODULES_FAN_H_
#define MODULES_FAN_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/max31856/MAX31856.h"
#include "modules/pid.h"

//=================================
// declarations

enum FAN_MODE_t {
	FAN_MODE_PID,
	FAN_MODE_2POS,
	FAN_MODE_MANUAL
};

struct fan_t {
	void (*set_pwm)(int);
	struct MAX31856_t* sensor;
	struct pid_param_t* pid;
	int32_t target_temp;
	int32_t target_level;
	int32_t level;
	enum FAN_MODE_t mode;
};

#define FAN_TPL(pwm, sensor, pid, temp, level, mode) \
		{pwm,\
		sensor,\
		pid,\
		temp,\
		level,\
		100,\
		mode}


extern void fan_pid_init(struct fan_t* fan);
extern void fan_regulate(struct fan_t* fan);
extern void fan_target_temp(struct fan_t* fan, int32_t temperature);
extern void fan_target_level(struct fan_t* fan, int32_t level);
extern void fan_set_level(struct fan_t* fan, int32_t level);
extern void fan_mode(struct fan_t* fan, enum FAN_MODE_t mode);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_FAN_H_ */
