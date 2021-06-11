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
#include "modules/axis.h"
#include "utilities/direction.h"
#include "peripherals/gpio.h"

#include <stdlib.h>

//=================================
// definitions

static int axis_check_target(struct axis_t* axis);
static void axis_limsw_routine(struct axis_t* axis);
static void axis_limitswitch_worker(struct axis_t* axis);

void axis_init(struct axis_t* axis) {
	motor_stop(axis->motor);
	
	//reset encoder
	encoder_init(axis->encoder);

	axis->cal_done = 0;
	axis->mode = AXIS_NORMAL;

	limit_switch_init(&axis->lim_sw_fwd);
	limit_switch_init(&axis->lim_sw_bwd);
}


// WORKAROUND TO wait during calibration turn around
#define CAL_FREE_STOP_TIME_MS 500

void axis_worker(struct axis_t* axis) {

	axis_limitswitch_worker(axis);

	switch (axis->mode) {

	default:
	case AXIS_ERROR:
		break;

	case AXIS_CAL_FIND:
		break;

	case AXIS_NORMAL:
		axis_check_target(axis);
		break;

	case AXIS_CAL_FWD:
		if (axis_forward_limit_switch_active(axis)) {
			axis->mode = AXIS_CAL_FREE;
			axis->cal_free_waiting = false;
			motor_run(axis->motor, BWD);
		} else {
			axis->mode = AXIS_CAL_FIND;
			motor_run(axis->motor, FWD);
		}
		break;

	case AXIS_CAL_BWD:
		if (axis_backward_limit_switch_active(axis)) {
			axis->mode = AXIS_CAL_FREE;
			axis->cal_free_waiting = false;
			motor_run(axis->motor, FWD);
		} else {
			axis->mode = AXIS_CAL_FIND;
			motor_run(axis->motor, BWD);
		}
		break;

	case AXIS_CAL_FREE:
		// check if stopped and waiting for reverse
		if (axis->cal_free_waiting) {
			if(HAL_GetTick() - axis->cal_free_stopped_timestamp >= CAL_FREE_STOP_TIME_MS) {

				if (axis->cal_free_direction == DIRECTION_NONE) {
					//error motor not moving during calibration
				}

				axis->mode = AXIS_CAL_FIND;

				//reverse motor
				motor_run(axis->motor, (axis->cal_free_direction == FWD ? BWD : FWD));
			}

		} else if (!axis_forward_limit_switch_active(axis) && //check if both limit switches are free
				!axis_backward_limit_switch_active(axis)) {

			struct motor_t* m = axis->motor;

			axis->cal_free_waiting = true;
			axis->cal_free_stopped_timestamp = HAL_GetTick();
			axis->cal_free_direction = m->direction;

			motor_stop(m);
		}

		break;
	}
}

enum axis_mode_t axis_status(struct axis_t* axis) {
	return axis->mode;
}

int axis_has_error(struct axis_t* axis) {
	return axis->mode == AXIS_ERROR;
}

static int axis_check_target(struct axis_t* axis) {
	const int32_t pos = axis->encoder->position;

	if (axis->motor->direction == FWD &&
			(pos >= axis->pos_target || pos >= axis->pos_max)) {

		//target reached
		motor_stop(axis->motor);
		return 1;

	} else if (axis->motor->direction == BWD &&
			(pos <= axis->pos_target || pos <= axis->pos_min)) {

		//target reached
		motor_stop(axis->motor);
		return 1;
	}

	//target NOT reached yet
	return 0;
}

void axis_move_to(struct axis_t* axis, int32_t target) {
	if (axis_has_error(axis))
		return;

	//check limits
	if (target < axis->pos_min && target > axis->pos_max)
		return;

	int32_t pos = axis->encoder->position;

	//limit switch check
	if (target > pos && axis_forward_limit_switch_active(axis)) {
		return;
	} else if (target < pos && axis_backward_limit_switch_active(axis)) {
		return;
	}

	//save new target
	axis->pos_target = target;

	//calculate delta
	int64_t d = abs(target - pos);

	//check minimum delta
	if (d > AXIS_MIN_POS_OFFSET)
		motor_run(axis->motor, (target > pos ? FWD : BWD));
}

void axis_move(struct axis_t* axis, enum direction_t direction) {
	if (axis_has_error(axis))
		return;

	switch (direction) {
	case DIRECTION_NONE:
		motor_stop(axis->motor);
		break;

	case FWD:
		if(axis_forward_limit_switch_active(axis)) {
			return;
		}
		axis->pos_target = INT32_MAX;
		motor_run(axis->motor, FWD);
		break;

	case BWD:
		if(axis_backward_limit_switch_active(axis)) {
			return;
		}
		axis->pos_target = INT32_MIN;
		motor_run(axis->motor, BWD);
		break;
	}
}

/* Clears error condition */
void axis_cal(struct axis_t* axis, enum direction_t direction) {
	axis->cal_done = 0;
	axis->mode = (direction == FWD ? AXIS_CAL_FWD : AXIS_CAL_BWD);
}

void axis_limsw_fwd_isr(struct axis_t* axis) {
	//hardware limit switch reached
	limit_switch_triggered_isr(&axis->lim_sw_fwd);
}

void axis_limsw_bwd_isr(struct axis_t* axis) {
	//hardware limit switch reached
	limit_switch_triggered_isr(&axis->lim_sw_bwd);
}

void axis_emergency_stop(struct axis_t* axis) {
	motor_stop(axis->motor);

	axis->mode = AXIS_ERROR;
}

static void axis_limsw_routine(struct axis_t* axis) {
	motor_stop(axis->motor);

	if (axis->mode == AXIS_CAL_FIND) {
		//clear position
		axis->encoder->position = 0;

		//set calibration done flag
		axis->cal_done = 1;

		//exit calibration mode
		axis->mode = AXIS_NORMAL;
	}
}

static void axis_limitswitch_worker(struct axis_t* axis) {
	if (axis_forward_limit_switch_active(axis) && axis->motor->direction == FWD) {
		axis_limsw_routine(axis);
	}
	if (axis_backward_limit_switch_active(axis) && axis->motor->direction == BWD) {
		axis_limsw_routine(axis);
	}
}

bool axis_forward_limit_switch_active(struct axis_t* axis) {
	return limit_switch_active(&axis->lim_sw_fwd);
}

bool axis_backward_limit_switch_active(struct axis_t* axis) {
	return limit_switch_active(&axis->lim_sw_bwd);
}

bool axis_calibrated(struct axis_t* axis) {
	return axis->cal_done != 0;
}

bool axis_stopped(struct axis_t* axis) {
	return axis->motor->direction == DIRECTION_NONE;
}

int axis_position(struct axis_t* axis) {
	return axis->encoder->position;
}
