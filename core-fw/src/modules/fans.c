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
#include "modules/fans.h"
#include "modules/temp_sensors.h"
#include "peripherals/pwm.h"
#include "main.h"

//=================================
// definitions

#define HYDRO_COOLER_TARGET_TEMP_C 20
#define HYDRO_COOLER_PRORTIONAL_FACTOR -10
#define HYDRO_COOLER_TARGET_LEVEL 0

pid_param_t hydro_cooler_pid = { .Kp = HYDRO_COOLER_PRORTIONAL_FACTOR };

struct fan_t hydro_cooler = FAN_TPL(
		&pwm_set_FAN, &temperature_sensor_water, &hydro_cooler_pid,
		HYDRO_COOLER_TARGET_TEMP_C, HYDRO_COOLER_TARGET_LEVEL,
		FAN_MODE_PID);

#define QUICK_COOLER_TARGET_TEMP_C 70
#define QUICK_COOLER_TARGET_LEVEL 0


IO_pin_t qc_enable_pin = IO_PIN_TPL(QC_FAN_ENABLE_GPIO_Port, QC_FAN_ENABLE_Pin);
void qc_fan_enable(bool enable) {
	iopin_write(qc_enable_pin, enable);
}

// TODO: improve abstraction of gpio enabled fan
struct fan_t quick_cooler = ENABLE_FAN_TPL(
		&qc_fan_enable, &temperature_sensor_galden, 0x0,
		QUICK_COOLER_TARGET_TEMP_C, QUICK_COOLER_TARGET_LEVEL,
		FAN_MODE_2POS);

#define LID_FAN_TARGET_TEMP_C 100
#define LID_FAN_TARGET_LEVEL 0

IO_pin_t lid_fan_enable_pin = IO_PIN_TPL(LID_FAN_ENABLE_GPIO_Port, LID_FAN_ENABLE_Pin);
void lid_fan_enable(bool enable) {
	iopin_write(lid_fan_enable_pin, enable);
}

struct fan_t lid_fan = ENABLE_FAN_TPL(
		&lid_fan_enable, &temperature_sensor_galden, 0x0,
		LID_FAN_TARGET_TEMP_C, LID_FAN_TARGET_LEVEL,
		FAN_MODE_2POS);

void fans_init() {
	fan_set_level(&hydro_cooler, hydro_cooler.target_level);
	fan_set_level(&quick_cooler, quick_cooler.target_level);

	//link fan and temperature parameters to pid controller structure
	fan_pid_init(&hydro_cooler);

	hydro_cooler.pid->Kp = HYDRO_COOLER_PRORTIONAL_FACTOR;
}

void fans_emergency_stop() {
	fan_emergency_stop(&hydro_cooler);
	fan_emergency_stop(&quick_cooler);
}
