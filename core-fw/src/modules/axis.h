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


#ifndef MODULES_AXIS_H_
#define MODULES_AXIS_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/encoder.h"
#include "modules/limit_switch.h"
#include "modules/motor.h"
#include "utilities/direction.h"

//=================================
// declarations

enum axis_mode_t {
	AXIS_NORMAL,
	AXIS_CAL_FWD,
	AXIS_CAL_BWD,
	AXIS_CAL_FREE,
	AXIS_CAL_FIND,
    AXIS_ERROR
};

struct axis_t{
	//working range minimum position limit
	int32_t pos_min;

	//working range maximum position limit
	int32_t pos_max;

	//target position
	int32_t pos_target;

	//mode of the axis
	enum axis_mode_t mode;

	//motor pointer
	struct motor_t* motor;

	//limit switch in forward direction
	limit_switch_t lim_sw_fwd;

	//limit switch in backward direction
	limit_switch_t lim_sw_bwd;

	//encoder pointer
	struct encoder_t* encoder;

	//was a calibration performed since startup?
	char cal_done;


	bool cal_free_waiting;
	uint32_t cal_free_stopped_timestamp;
	enum direction_t cal_free_direction;
};

#define AXIS_TPL(min, max, motor, encoder, limsw_fwd_pin, limsw_bwd_pin)\
	{min, max, 0, AXIS_NORMAL, motor, {limsw_fwd_pin}, {limsw_bwd_pin}, encoder, 0}

// pos changes that are smaller than AXIS_MIN_POS_OFFSET are discarded
#define AXIS_MIN_POS_OFFSET 1500

void axis_init(struct axis_t* axis);
void axis_worker(struct axis_t* axis);
enum axis_mode_t axis_status(struct axis_t* axis);
int axis_has_error(struct axis_t* axis);
void axis_move_to(struct axis_t* axis, int32_t target);
void axis_move(struct axis_t* axis, enum direction_t direction);
void axis_cal(struct axis_t* axis, enum direction_t direction);
void axis_emergency_stop(struct axis_t* axis);
bool axis_forward_limit_switch_active(struct axis_t* axis);
bool axis_backward_limit_switch_active(struct axis_t* axis);
bool axis_calibrated(struct axis_t* axis);
bool axis_stopped(struct axis_t* axis);
int axis_position(struct axis_t* axis);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_AXIS_H_ */
