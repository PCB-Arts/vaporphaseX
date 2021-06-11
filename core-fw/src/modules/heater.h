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


#ifndef MODULES_HEATER_H_
#define MODULES_HEATER_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/temp_sensor.h"

#include <stdint.h>
#include <stdbool.h>

//=================================
// declarations

#define HEATER_ERR_SENSOR	0b00000001
#define HEATER_ERR_MAX_TEMP	0b00000010
#define HEATER_ERR_WATCHDOG	0b00000100
#define HEATER_ERR_OTP_TRIPPED	0b00001000
#define HEATER_ERR_EMERGENCY_STOP	0b00010000

void heater_init(TemperatureSensor* s1, TemperatureSensor* s2, TemperatureSensor* _galdenSensor, TIM_HandleTypeDef* htim, uint16_t channel);
void heater_worker();
void heater_seconds_sync();
uint8_t heater_get_flags();
void heater_reset_flags();
int heater_temperature_reached();

void heater_set_temperature(float temperature);
void heater_set_hysteresis(float hysteresis);

void heater_regulation_enable();
void heater_regulation_disable();
bool heater_regulation_enabled();
void heater_enable();
void heater_disable();

bool heater_enabled();

void heater_overtemp();

void heater_emergency_stop();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_HEATER_H_ */
