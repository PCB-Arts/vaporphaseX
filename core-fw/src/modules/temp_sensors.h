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


#ifndef MODULES_TEMP_SENSORS_H_
#define MODULES_TEMP_SENSORS_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/max31856/MAX31856.h"
#include "modules/temp_sensor.h"

#include <stddef.h>

//=================================
// class

extern struct MAX31856_t max31856_temperature_sensor_heater_1;
extern struct MAX31856_t max31856_temperature_sensor_heater_2;
extern struct MAX31856_t max31856_temperature_sensor_pcb;
extern struct MAX31856_t max31856_temperature_sensor_galden;

extern TemperatureSensor temperature_sensor_heater_1;
extern TemperatureSensor temperature_sensor_heater_2;
extern TemperatureSensor temperature_sensor_pcb;
extern TemperatureSensor temperature_sensor_galden;
extern TemperatureSensor temperature_sensor_water;

extern struct MAX31856_t* max31856_temp_sensors[];
extern const size_t max31856_temp_sensors_size;

extern TemperatureSensor* temp_sensors[];
extern const size_t temp_sensors_size;

extern int max31856_temp_sensor_cycle_complete;

extern void max31856_temp_sensors_init();
extern void max31856_temp_sensors_worker();
extern void max31856_temp_sensors_millis_sync();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_TEMP_SENSORS_H_ */
