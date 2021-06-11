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
#include "peripherals/gpio.h"
#include "modules/pump.h"
#include "modules/fans.h"
#include "modules/temp_sensors.h"
#include "main.h"

//=================================
// definitions

static const IO_pin_t pump_enable_pin = IO_PIN_TPL(PUMP_ENABLE_GPIO_Port, PUMP_ENABLE_Pin);
static bool pump_auto_enabled = false;
static bool enabled = false;

#define PUMP_ENABLE_WATER_TEMPERATURE_THRESHOLD 30
#define PUMP_ENABLE_WATER_TEMPERATURE_THRESHOLD_HYSTERESE 5
#define PUMP_ENABLE_GALDEN_TEMPERATURE_THRESHOLD 70
#define PUMP_ENABLE_GALDEN_TEMPERATURE_THRESHOLD_HYSTERESE 5

void pump_enable()
{
	enabled = true;
	iopin_write(pump_enable_pin, 1);
	fan_set_auto_enabled(&hydro_cooler, true);
}

void pump_disable()
{
	enabled = false;
	iopin_write(pump_enable_pin, 0);
	fan_set_auto_enabled(&hydro_cooler, false);
}

void pump_worker()
{
	if (!pump_auto_enabled)
	{
		return;
	}
	
	if(enabled) {
		if((temperature_sensor_get_temperature(&temperature_sensor_water) <
				PUMP_ENABLE_WATER_TEMPERATURE_THRESHOLD -
				PUMP_ENABLE_WATER_TEMPERATURE_THRESHOLD_HYSTERESE)
					&&
		(temperature_sensor_get_temperature(&temperature_sensor_galden) <
				PUMP_ENABLE_GALDEN_TEMPERATURE_THRESHOLD -
				PUMP_ENABLE_GALDEN_TEMPERATURE_THRESHOLD_HYSTERESE)) {
			pump_disable();
		}
	} else {
		if (
				temperature_sensor_get_temperature(&temperature_sensor_water) > PUMP_ENABLE_WATER_TEMPERATURE_THRESHOLD
				||
				temperature_sensor_get_temperature(&temperature_sensor_galden) > PUMP_ENABLE_GALDEN_TEMPERATURE_THRESHOLD) {
			pump_enable();
		}
	}
}

void pump_set_auto_enabled(bool auto_enabled)
{
	pump_auto_enabled = auto_enabled;
}

bool pump_is_auto_enabled()
{
	return pump_auto_enabled;
}

bool pump_enabled(){
	return enabled;
}
