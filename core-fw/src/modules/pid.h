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


#ifndef MODULES_REGULATORS_PID_H_
#define MODULES_REGULATORS_PID_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include <stdint.h>

//=================================
// class

typedef struct __pid_param_t {
	float time_step_size; // time step size in s 

	//regulation factors
	float Kp;
	float Ki;
	float Kd;

	float error_integral;
	float error_prev;

	float y_min;
	float y_max;
} pid_param_t;

extern float pid_compute(pid_param_t* pid, float reference, float process_variable);
extern void pid_reset(pid_param_t* pid);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_REGULATORS_PID_H_ */
