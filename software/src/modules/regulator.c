/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 PCB Arts GmbH - Kevin Schick
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  _____    ____    ____
 *       \  /       |    \        /\            |
 *   ____/  |       |____/       /  \     __  __|__   ___
 *  |       |       |    \      /____\   /      |    |___
 *  |       \____   |____/     /      \  |      |     ___|
 *
 *         _    __   __   __     __         _    __   __
 *  \  /  /_\  |__| |  | |__|   |__| |__|  /_\  |__  |__
 *   \/  /   \ |    |__| | \    |    |  | /   \  __| |__
 *
 *
 *  PCB Arts GmbH
 *  Kurgartenstrasse 59
 *  90762 Fuerth
 *  GERMANY
 *
 *  www.pcb-arts.com
 *  mail(at)pcb-arts.com
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


//=================================
// included dependencies
#include "modules/regulator.h"
#include "modules/max31856/MAX31856_def.h"
#include "modules/heater.h"

#include <stdlib.h>

//=================================
// defines

#define TEMP_REACHED_HYST 2.0f
#define MOTOR_SPEED_MIN 30
#define MOTOR_SPEED_MAX 100
#define TEMP_DELTA_MAX 250

static int enabled;

struct {
	float value;
	int32_t counts;
} static temperature_set;

static int32_t lift_pos_set;
static struct MAX31856_t* sensor;
static volatile struct axis_t* axis;
struct pid_param_t regulator_pid;

static int sensor_valid();
static void adjust_motor_speed();


void regulator_init(struct MAX31856_t* s, volatile struct axis_t* a) {
	sensor = s;
	axis = a;

	//check for null ptr
	if (!axis || !sensor)
		return;

	//assign values to the temperature regulation PID controller
	regulator_pid.w = (int32_t*)&temperature_set.counts;
	regulator_pid.y = (int32_t*)&lift_pos_set;
	regulator_pid.x = &sensor->LTC;
	regulator_pid.y_max = axis->pos_max;
	regulator_pid.y_min = axis->pos_min;
	regulator_pid.e_sum_min = -1000;
	regulator_pid.e_sum_max = 1000;
	regulator_pid.polarity = 1;
}

void regulator_compute() {
	if (enabled && axis && sensor_valid()) {

		//compute new lift position
		pid_compute(&regulator_pid);

		//speed up or slow down the motor
		adjust_motor_speed();

		//move lift to new position
		axis_move_to(axis, lift_pos_set);
	}
}

void regulator_enable() {
	enabled = 1;
}

void regulator_disable() {
	enabled = 0;
}

void regulator_set_temp(float temperature) {
	temperature_set.value = temperature;
	temperature_set.counts = temperature / MAX31856_RESOLUTION;
}

static void adjust_motor_speed() {
	int delta = abs(regulator_pid.e_prev);

	//scale temperature error to PWM motor speed
	//e.g. 0C - 250C => 30% - 100%
	const int speed_range = (MOTOR_SPEED_MAX - MOTOR_SPEED_MIN);
	int speed = ((delta * speed_range) / TEMP_DELTA_MAX) + MOTOR_SPEED_MIN;

	//limit the max speed
	speed = (speed > MOTOR_SPEED_MAX ? MOTOR_SPEED_MAX : speed);

	motor_set_speed(axis->motor, speed);
}

static int sensor_valid() {
	if (MAX31856_has_fault(sensor)) {
		//move lift to upper position if regulator is enabled
		if (enabled && axis)
			axis_move_to(axis, 0);

		return 0;
	} else {
		return 1;
	}
}

int regulator_temperature_reached() {
	if (!sensor_valid())
		return 0;

	const float temp = sensor->temperature;

	return (temp >= temperature_set.value - TEMP_REACHED_HYST);
}
