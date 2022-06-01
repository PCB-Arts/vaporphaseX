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
#include <stdlib.h>
#include "modules/lift.h"
#include "modules/encoder.h"
#include "modules/motor.h"
#include "peripherals/freq.h"
#include "main.h"
#include "peripherals/gpio.h"
#include "lid.h"

//=================================
// definitions

#define LIFT_MAX 50
#define LIFT_MIN -1E6

static struct motor_t lift_motor = STEPPER_TPL(
		IO_PIN_TPL(LIFT_DIRECTION_GPIO_Port, LIFT_DIRECTION_Pin),
		IO_PIN_TPL(LIFT_ENABLE_GPIO_Port, LIFT_ENABLE_Pin),
		&freq_set);

struct axis_t lift_axis = AXIS_TPL(LIFT_MIN, LIFT_MAX,
		&lift_motor, &freq_encoder,
		IO_PIN_TPL(LIFT_END_TOP_GPIO_Port, LIFT_END_TOP_Pin),
		IO_PIN_TPL(LIFT_END_BOTTOM_GPIO_Port, LIFT_END_BOTTOM_Pin));

calibpos calib;

void lift_worker() {
	axis_worker(&lift_axis);
}

void lift_emergency_stop(void) {
	axis_emergency_stop(&lift_axis);
}

bool lift_boot_ready() {
	enum axis_mode_t status = axis_status(&lift_axis);
	enum direction_t dir = lift_motor.direction;

	return status == AXIS_NORMAL
		&& dir == DIRECTION_NONE
		&& axis_calibrated(&lift_axis);

}

#define LIFT_POSITION_MARGIN AXIS_MIN_POS_OFFSET

bool lift_lid_open_state_reached() {
	return lift_axis.motor->direction == DIRECTION_NONE &&
			abs(lift_axis.encoder->position - LIFT_LID_OPEN_POSITION) < LIFT_POSITION_MARGIN;
}

void lift_lid_open_state() {
	motor_set_speed(lift_axis.motor, LIFT_MOTOR_MAX_SPEED);
	axis_move_to(&lift_axis, LIFT_LID_OPEN_POSITION);
}

void lift_calibrate() {
	axis_cal(&lift_axis, FWD);
	calib.liftflag = 1;
}
