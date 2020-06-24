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


//=================================
// included dependencies
#include "modules/fan.h"

//=================================
// definitions
#define FAN_MAX 100
#define FAN_MIN 0


void fan_pid_init(struct fan_t* fan) {
	if (!fan || !fan->sensor || !fan->pid)
		return ;

	fan->pid->y = &fan->level;
	fan->pid->x = &fan->sensor->itemperature;
	fan->pid->w = &fan->target_temp;
	fan->pid->e_sum_max = 200000;
	fan->pid->e_sum_min = 0;
	fan->pid->y_max = 100;
	fan->pid->y_min = fan->target_level;
}

void fan_regulate(struct fan_t* fan) {
	if (!fan || !fan->sensor || !fan->set_pwm)
		return ;

	if (fan->mode == FAN_MODE_PID && fan->pid) {
		if (pid_compute(fan->pid))
			fan->set_pwm(fan->level);

	} else if (fan->mode == FAN_MODE_2POS) {

		if (fan->sensor->temperature > fan->target_temp) {
			fan_set_level(fan, 100);
		} else if (fan->sensor->temperature < fan->target_temp - 3) {
			fan_set_level(fan, fan->target_level);
		}

	}
}

void fan_target_temp(struct fan_t* fan, int32_t temperature) {
	if (!fan)
		return ;

	fan->target_temp = temperature;
}

void fan_target_level(struct fan_t* fan, int32_t level) {
	if (!fan)
		return ;

	//check limits
	if (level < FAN_MIN)
		level = FAN_MIN;
	else if (level > FAN_MAX)
		level = FAN_MAX;

	fan->target_level = level;
}

void fan_set_level(struct fan_t* fan, int32_t level) {
	if (!fan || !fan->set_pwm)
		return ;

	//check limits
	if (level < FAN_MIN)
		level = FAN_MIN;
	else if (level > FAN_MAX)
		level = FAN_MAX;

	fan->level = level;

	//set PWM to the inverted level (100% PWM = 0% FAN)
	fan->set_pwm(FAN_MAX - level);
}

void fan_mode(struct fan_t* fan, enum FAN_MODE_t mode) {
	if (!fan)
		return ;

	fan->mode = mode;
}
