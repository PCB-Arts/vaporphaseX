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


//=================================
// included dependencies
#include "modules/pid.h"

//=================================
// defines

float pid_compute(pid_param_t* pid, float reference, float process_variable) {
	const float error = reference - process_variable;

	//calculate the rate of change
	const float error_delta = (error - pid->error_prev) / pid->time_step_size;

	//save error for next cycle
	pid->error_prev = error;

	//add to error integral
	pid->error_integral += error * pid->time_step_size;

	//proportional factor
	const float fKp = pid->Kp * error;

	//integral factor
	const float fKi = pid->Ki * pid->error_integral;

	//derivative factor
	const float fKd = pid->Kd * error_delta;

	float y = fKp + fKi + fKd;

	//limit y values
	if (y > pid->y_max) {
		y = pid->y_max;
	} else if (y < pid->y_min) {
		y = pid->y_min;
	}
	
	return y;
}

void pid_reset(pid_param_t* pid) {
	pid->error_prev = 0;

	pid->error_integral = 0;
}
