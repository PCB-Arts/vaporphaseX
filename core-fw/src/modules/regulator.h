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


#ifndef MODULES_REGULATOR_H_
#define MODULES_REGULATOR_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include <stdbool.h>

#include "modules/pid.h"
#include "modules/temp_sensor.h"
#include "modules/axis.h"

//=================================
// class

pid_param_t regulator_pid;

void regulator_init(TemperatureSensor* s, struct axis_t* a);
void regulator_compute();
void regulator_enable();
void regulator_disable();
bool regulator_enabled();
bool regulator_temperature_reached();

void regulator_set_temp(float temperature);
float regulator_get_target_temperature();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_REGULATOR_H_ */
