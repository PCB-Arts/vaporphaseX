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
#include "modules/pid.h"

//=================================
// defines

int32_t pid_compute(struct pid_param_t* pid) {
	const int32_t y_prev = *pid->y;
	const int32_t e = *pid->w - *pid->x;

	//calculate the rate of change
	pid->e_delta = (e - pid->e_prev);

	//save error for next cycle
	pid->e_prev = e;

	//add to error sum
	pid->e_sum += e;

	//limit error sum
	if (pid->e_sum > pid->e_sum_max)
		pid->e_sum = pid->e_sum_max;
	else if (pid->e_sum < pid->e_sum_min)
		pid->e_sum = pid->e_sum_min;


	//proportional factor
	const float fKp = pid->Kp * e;

	//integral factor
	const float fKi = pid->Ki * pid->e_sum;

	//derivative factor
	const float fKd = pid->Kd * pid->e_delta;


	//calculate total factor
	const float ftotal = fKp + fKi + fKd;

	//calculate new output value
	int32_t y = (pid->polarity ? y_prev - ftotal : y_prev + ftotal);

	//limit y values
	if (y > pid->y_max)
		y = pid->y_max;
	else if (y < pid->y_min)
		y = pid->y_min;

	*pid->y = y;
	return (y - y_prev);
}

void pid_reset(struct pid_param_t* pid) {
	//reset previous error
	pid->e_prev = 0;

	//reset error sum
	pid->e_sum = 0;
}
