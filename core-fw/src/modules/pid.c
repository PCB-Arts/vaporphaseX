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
#include "log.h"
#include "control/sequence.h"

extern currentState cS;


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

float pid_computeLIFT(pid_param_t* pid, float reference, float process_variable) {


	const float errorR = reference - process_variable;

	//calculate the rate of change
	const float error_deltaR = (errorR - pid->error_prevR) / pid->time_step_size;

	//save error for next cycle
	pid->error_prevR = errorR;

	//add to error integral
	if (cS.currentState >= 2){
	pid->error_integralR += errorR * pid->time_step_size;
	}
	//proportional factor
	const float fKpR = pid->KpR * errorR;
	//integral factor
	const float fKiR = pid->KiR * pid->error_integralR;
	//derivative factor
	const float fKdR = pid->KdR * error_deltaR;

	float y = fKpR + fKiR + fKdR;

	//limit y values
	if (y > pid->y_max) {
		y = pid->y_max;
	} else if (y < pid->y_min) {
		y = pid->y_min;
	}
	cS.x = y;
	return y;
}

void pid_reset(pid_param_t* pid) {
	pid->error_prev = 0;

	pid->error_integral = 0;
}
