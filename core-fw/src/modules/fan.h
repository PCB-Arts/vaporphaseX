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


#ifndef MODULES_FAN_H_
#define MODULES_FAN_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/max31856/MAX31856.h"
#include "modules/pid.h"
#include "temp_sensor.h"

//=================================
// declarations

enum FAN_MODE_t {
	FAN_MODE_PID,
	FAN_MODE_2POS
};

struct fan_t {
	void (*set_pwm)(int);
	TemperatureSensor* sensor;
	pid_param_t* pid;
	int32_t target_temp;
	int32_t target_level;
	int32_t level;
	enum FAN_MODE_t mode;
	void (*set_enable)(bool);
	bool auto_enabled;
};

#define FAN_TPL(pwm, sensor, pid, temp, level, mode) \
		{pwm,\
		sensor,\
		pid,\
		temp,\
		level,\
		100,\
		mode,\
		NULL,\
		false}

#define ENABLE_FAN_TPL(enable, sensor, pid, temp, level, mode) \
		{NULL,\
		sensor,\
		pid,\
		temp,\
		level,\
		100,\
		mode,\
		enable,\
		false}


extern void fan_pid_init(struct fan_t* fan);
extern void fan_regulate(struct fan_t* fan);
extern void fan_target_temp(struct fan_t* fan, int32_t temperature);
extern void fan_target_level(struct fan_t* fan, int32_t level);
extern void fan_set_level(struct fan_t* fan, int32_t level);
extern void fan_mode(struct fan_t* fan, enum FAN_MODE_t mode);
int8_t fan_get_speed(struct fan_t* fan);
void fan_emergency_stop(struct fan_t *fan);
void fan_set_auto_enabled(struct fan_t *fan, bool enabled);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_FAN_H_ */
