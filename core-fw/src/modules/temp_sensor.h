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

#ifndef MODULES_TEMP_SENSOR_H_
#define MODULES_TEMP_SENSOR_H_

#include "max31856/MAX31856.h"
#include "ntc.h"

enum TemperatureSensorType {
	TemperatureSensorTypeMax31856 = 1,
	TemperatureSensorNTC = 2
};

typedef struct __TemperatureSensor {
	enum TemperatureSensorType type;
	struct MAX31856_t* max31856;
	NtcTemperatureSensor* ntc;
} TemperatureSensor;

/* returns the temperature in °C */
float temperature_sensor_get_temperature(TemperatureSensor* sensor);
int32_t temperture_sensor_get_temperature_int(TemperatureSensor* sensor);
bool temperature_sensor_has_fault(TemperatureSensor* sensor);

#endif /* MODULES_TEMP_SENSOR_H_ */
