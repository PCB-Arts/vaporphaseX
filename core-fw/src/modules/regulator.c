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
#include "modules/regulator.h"
#include "modules/heater.h"
#include "modules/temp_sensor.h"
#include "modules/pid.h"
#include <stdlib.h>
#include "control/sequence.h"
#include "modules/temp_sensors.h"

//=================================
// defines

#define TEMP_REACHED_HYST 2.0f
#define MOTOR_SPEED_MIN 30
#define MOTOR_SPEED_MAX 100
//#define TEMP_DELTA_MAX 250


static bool enabled;

float regulator_target_temperature;

static TemperatureSensor* sensor;
static struct axis_t* axis;
pid_param_t regulator_pid;

static int sensor_valid();

uint32_t max31856_get_ltc(struct MAX31856_t* sensor) {
	return sensor->LTC;
}

void regulator_init(TemperatureSensor* s, struct axis_t* a) {
	sensor = s;
	axis = a;

	//check for null ptr
	if (!axis || !sensor)
		return;

	// use an y between 0 and 100
	regulator_pid.y_max = MOTOR_SPEED_MAX;
	regulator_pid.y_min = -MOTOR_SPEED_MAX;

	regulator_pid.KpR = 20;
//	regulator_pid.KiR = 0.1;
//	regulator_pid.KdR = 125;

	regulator_pid.time_step_size = 0.01; // default time step of 100ms
}

void regulator_compute() {
	if (enabled && axis && sensor_valid()) {

		// compute lift speed

		float u = pid_computeLIFT(
			&regulator_pid,
			regulator_target_temperature,
			temperature_sensor_get_temperature(sensor)
		);

		motor_set_speed(axis->motor, abs(u));

		// move lift down for higher temperature


		axis_move_to(axis, u > 0 ? axis->pos_min : axis->pos_max);

	}
}


void regulator_enable() {
	enabled = true;
}

void regulator_disable() {
	motor_stop(axis->motor);
	motor_set_speed(axis->motor, MOTOR_SPEED_MAX);
	enabled = false;
}

bool regulator_enabled() {
	return enabled;
}

void regulator_set_temp(float temperature) {
	regulator_target_temperature = temperature;
}

static int sensor_valid() {
	if (temperature_sensor_has_fault(sensor)) {
		//move lift to upper position if regulator is enabled
		if (enabled && axis)
			axis_move_to(axis, 0);

		return 0;
	} else {
		return 1;
	}
}

bool regulator_temperature_reached() {
	if (!sensor_valid())
		return false;

	const float temp = temperature_sensor_get_temperature(sensor);

	return (temp >= regulator_target_temperature - TEMP_REACHED_HYST);
}

float regulator_get_target_temperature() {
	return regulator_target_temperature;
}
