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

#include "lid_control.h"

#include "modules/lid.h"
#include "modules/lift.h"
#include "modules/temp_sensors.h"

#define LID_OPEN_TEMPERATURE_ECO_CELSIUS 70
#define LID_OPEN_TEMPERATURE_NORMAL_CELSIUS 80
#define LID_OPEN_TEMPERATURE_FAST_CELSIUS 90

#define LID_OPEN_TEMPERATURE_HYSTERESE_CELCIUS 2

static bool lid_prepare_opening = false;
static LidOpenMode lid_control_lid_open_mode = VPO_STANDARD;
static bool lid_temperature_requirements_met = false;

static inline void update_lid_temperature_requirements() {
	 if(!lid_temperature_requirements_met) {
		lid_temperature_requirements_met =
				temperature_sensor_get_temperature(&temperature_sensor_galden) <
				lid_control_lid_open_temperature()
				- LID_OPEN_TEMPERATURE_HYSTERESE_CELCIUS;
	} else {
		lid_temperature_requirements_met =
				temperature_sensor_get_temperature(&temperature_sensor_galden) <
				lid_control_lid_open_temperature();
	}
}

void lid_control_init() {
    lid_prepare_opening = false;
}
void lid_control_worker() {
    if(lid_prepare_opening) {
		if(lift_lid_open_state_reached()) {
			lid_prepare_opening = false;
			axis_move(&lid_axis, FWD);
		}
	}

   update_lid_temperature_requirements();
}

/*
 * requirements:
 * - lid is closed
 * - lid is not in motion
 * - lid is not calibrating
 * - no errors
 */
bool lid_control_lid_closed() {
	return lid_control_lid_state() == LID_STATE_CLOSED;
};

bool lid_control_lid_boot_ready() {
	return lid_control_lid_closed();
}

void lid_control_lid_open() {
	if(lid_control_lid_state() != LID_STATE_CLOSED) {
		return;
	}

	lid_prepare_opening = true;
	lift_lid_open_state();
}

void lid_control_lid_close() {
	axis_move(&lid_axis, BWD);
}

LidState lid_control_lid_state() {
	enum axis_mode_t status = axis_status(&lid_axis);
	enum direction_t dir = lid_axis.motor->direction;

	if (status == AXIS_ERROR)
		return LID_STATE_ERROR;

	if(lid_prepare_opening)
		return LID_STATE_OPENING;

	if (status != AXIS_NORMAL)
		return LID_STATE_INITIALISING;

	if (dir == FWD)
		return LID_STATE_OPENING;
	if (dir == BWD)
		return LID_STATE_CLOSING;
	
	if (axis_backward_limit_switch_active(&lid_axis))
		return LID_STATE_CLOSED;

	return LID_STATE_OPEN;
}

void lid_control_set_lid_open_mode(LidOpenMode lidOpenMode) {
	lid_control_lid_open_mode = lidOpenMode;
}

LidOpenMode lid_control_get_lid_open_mode() {
	return lid_control_lid_open_mode;
}

float lid_control_lid_open_temperature() {
	switch (lid_control_lid_open_mode)
	{
	case VPO_ECO:
		return LID_OPEN_TEMPERATURE_ECO_CELSIUS;
	case VPO_FAST:
		return LID_OPEN_TEMPERATURE_FAST_CELSIUS;
	default:
		return LID_OPEN_TEMPERATURE_NORMAL_CELSIUS;
	}
}

bool lid_control_lid_open_requirements_met() {
	return lid_control_lid_closed() &&
			lid_temperature_requirements_met;
}
