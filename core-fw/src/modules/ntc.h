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

#ifndef MODULES_NTC_H_
#define MODULES_NTC_H_

#include "peripherals/adc.h"

typedef struct __NtcTemperatureSensor {
	AdcSignalHandle* adc_signal;
} NtcTemperatureSensor;

extern NtcTemperatureSensor ntc_temperature_sensor_water;

/* returns the temperature in °C */
float ntc_get_temperature(NtcTemperatureSensor* sensor);

extern const double celsius_to_kelvin;
extern const double ntc_resistance_25_ohm;
extern const double ntc_beta_kelvin;
extern const double ntc_temperature_25_kelvin;
extern const double ntc_voltage_divider_other_resistor_ohm;

#endif /* MODULES_NTC_H_ */
