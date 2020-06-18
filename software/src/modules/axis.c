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
#include "modules/axis.h"
#include "utilities/direction.h"
#include "hardware_declarations.h"

#include <stdlib.h>

//=================================
// definitions

#define MIN_POS_OFFSET 1500

static int axis_check_target(volatile struct axis_t* axis);
static void axis_limsw(volatile struct axis_t* axis);


void axis_init(volatile struct axis_t* axis) {
	//reset encoder
	encoder_init(axis->encoder);
}

void axis_worker(volatile struct axis_t* axis) {
	switch (axis->mode) {

	default:
	case AXIS_CAL_FIND:
		break;

	case AXIS_NORMAL:
		axis_check_target(axis);
		break;

	case AXIS_CAL_FWD:
		if (IOPIN_READ(axis->lim_sw_fwd)) {
			axis->mode = AXIS_CAL_FREE;
			motor_run(axis->motor, BWD);
		} else {
			axis->mode = AXIS_CAL_FIND;
			motor_run(axis->motor, FWD);
		}
		break;

	case AXIS_CAL_BWD:
		if (IOPIN_READ(axis->lim_sw_bwd)) {
			axis->mode = AXIS_CAL_FREE;
			motor_run(axis->motor, FWD);
		} else {
			axis->mode = AXIS_CAL_FIND;
			motor_run(axis->motor, BWD);
		}
		break;

	case AXIS_CAL_FREE:
		//check if both limit switches are free
		if (!IOPIN_READ(axis->lim_sw_fwd) &&
				!IOPIN_READ(axis->lim_sw_bwd)) {

			struct motor_t* m = axis->motor;

			if (m->direction == DIRECTION_NONE) {
				//error motor not moving during calibration
			}

			axis->mode = AXIS_CAL_FIND;

			//reverse motor
			motor_run(m, (m->direction == FWD ? BWD : FWD));
		}
		break;
	}
}

int axis_status(volatile struct axis_t* axis) {

}

static int axis_check_target(volatile struct axis_t* axis) {
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

void axis_move_to(volatile struct axis_t* axis, int32_t target) {
	//check limits
	if (target < axis->pos_min && target > axis->pos_max)
		return;

	//save new target
	axis->pos_target = target;

	int32_t pos = axis->encoder->position;

	//calculate delta
	int64_t d = abs(target - pos);

	//check minimum delta
	if (d > MIN_POS_OFFSET)
		motor_run(axis->motor, (target > pos ? FWD : BWD));
}

void axis_move(volatile struct axis_t* axis, enum direction_t direction) {
	switch (direction) {
	case DIRECTION_NONE:
		motor_stop(axis->motor);
		break;

	case FWD:
		axis->pos_target = INT32_MAX;
		motor_run(axis->motor, FWD);
		break;

	case BWD:
		axis->pos_target = INT32_MIN;
		motor_run(axis->motor, BWD);
		break;
	}
}

void axis_cal(volatile struct axis_t* axis, enum direction_t direction) {
	axis->mode = (direction == FWD ? AXIS_CAL_FWD : AXIS_CAL_BWD);
}

void axis_limsw_fwd_isr(volatile struct axis_t* axis) {
	//hardware limit switch reached
	if (axis->motor->direction == FWD) {
		axis_limsw(axis);
	}
}

void axis_limsw_bwd_isr(volatile struct axis_t* axis) {
	//hardware limit switch reached
	if (axis->motor->direction == BWD) {
		axis_limsw(axis);
	}
}

static void axis_limsw(volatile struct axis_t* axis) {
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
