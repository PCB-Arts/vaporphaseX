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

#include "temp_sensor.h"

float temperature_sensor_get_temperature(TemperatureSensor* sensor) {
	switch(sensor->type) {
	case TemperatureSensorTypeMax31856:
	{
		return sensor->max31856->temperature;
	}

	case TemperatureSensorNTC:
	{
		return ntc_get_temperature(sensor->ntc);
	}

	default:
		break;
	}

	return 0;
}

int32_t temperture_sensor_get_temperature_int(TemperatureSensor* sensor) {
	return (int32_t)temperature_sensor_get_temperature(sensor);
}

bool temperature_sensor_has_fault(TemperatureSensor* sensor) {
	switch(sensor->type) {
	case TemperatureSensorTypeMax31856:
	{
		return MAX31856_has_fault(sensor->max31856);
	}
	case TemperatureSensorNTC:
	{
		return false;
	}

	default:
		break;
	}

	return false;
}

