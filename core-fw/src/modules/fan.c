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
#include "modules/fan.h"
#include "temp_sensor.h"

//=================================
// definitions
#define FAN_MAX 100
#define FAN_MIN 0


void fan_pid_init(struct fan_t* fan) {
	if (!fan || !fan->sensor || !fan->pid || !fan->set_pwm)
		return ;

	fan->pid->time_step_size = 0.1;
	fan->pid->y_max = 100;
	fan->pid->y_min = fan->target_level;
}

void fan_regulate(struct fan_t* fan) {
	if (!fan || !fan->sensor || (!fan->set_pwm && !fan->set_enable))
		return;

	if(!fan->auto_enabled) {
		return;
	}

	if (fan->mode == FAN_MODE_PID && fan->pid) {
		int y = pid_compute(fan->pid, fan->target_temp, temperture_sensor_get_temperature_int(fan->sensor));

		fan->set_pwm(100 - y);

	} else if (fan->mode == FAN_MODE_2POS) {
		float temperature = temperature_sensor_get_temperature(fan->sensor);
		if (temperature > fan->target_temp) {
			fan_set_level(fan, 100);
		} else if (temperature < fan->target_temp - 3) {
			fan_set_level(fan, fan->target_level);
		}
	}
}

void fan_target_temp(struct fan_t* fan, int32_t temperature) {
	if (!fan)
		return ;

	fan->target_temp = temperature;
}

void fan_target_level(struct fan_t* fan, int32_t level) {
	if (!fan)
		return ;

	//check limits
	if (level < FAN_MIN)
		level = FAN_MIN;
	else if (level > FAN_MAX)
		level = FAN_MAX;

	fan->target_level = level;
}

void fan_set_level(struct fan_t* fan, int32_t level) {
	if (!fan || (!fan->set_pwm && !fan->set_enable))
		return ;

	//check limits
	if (level < FAN_MIN)
		level = FAN_MIN;
	else if (level > FAN_MAX)
		level = FAN_MAX;

	fan->level = level;

	if(fan->set_pwm) {
		//set PWM to the inverted level (100% PWM = 0% FAN)
		fan->set_pwm(FAN_MAX - level);
	} else {
		fan->set_enable(level > 0);
	}
}

void fan_mode(struct fan_t* fan, enum FAN_MODE_t mode) {
	if (!fan)
		return ;

	fan->mode = mode;
}

void fan_emergency_stop(struct fan_t *fan) {
	if(fan->set_pwm) {
		fan->set_pwm(0);
	} else {
		fan->set_enable(1);
	}
}

void fan_set_auto_enabled(struct fan_t *fan, bool enabled) {
	fan->auto_enabled = enabled;

	if(!enabled) {
		fan_set_level(fan, 0);
	}
}

int8_t fan_get_speed(struct fan_t* fan){
	return (int8_t) fan->level;
}
