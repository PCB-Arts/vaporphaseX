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

#include "ntc.h"

#include "math.h"

const double celsius_to_kelvin = 273.15;

// parameters at 25°C from
// https://www.mouser.de/datasheet/2/362/semitec_usa_corporation_smtcd00017-7-1742901.pdf
// for 104JT-050
const double ntc_resistance_25_ohm = 100E3; // 100kOhm
const double ntc_beta_kelvin = 4390;

const double ntc_temperature_25_kelvin = 298.15;

const double ntc_voltage_divider_other_resistor_ohm = 10E3; //10kOhm

// adc channel must be set externally
NtcTemperatureSensor ntc_temperature_sensor_water;

float ntc_get_temperature(NtcTemperatureSensor* sensor) {
	uint32_t adc_resulution_max = adc_resolution_get_max_value(sensor->adc_signal);
	uint32_t adc_value = adc_get_value(sensor->adc_signal);

	// resistance of ntc is calculated using the ratio of voltages
	double resistance_ohm = ntc_voltage_divider_other_resistor_ohm
			* (adc_value) /
			(adc_resulution_max - adc_value + 1); // add 1 to prevent division by 0

	// equation from https://www.giangrandi.org/electronics/ntc/ntc.shtml
	double temp_kelvin = 1 /
			(
					(log(resistance_ohm / ntc_resistance_25_ohm) / ntc_beta_kelvin)
					+ (1 / ntc_temperature_25_kelvin)
			);

	double temp_celsius = (temp_kelvin - celsius_to_kelvin);

	return (float)temp_celsius;
}

