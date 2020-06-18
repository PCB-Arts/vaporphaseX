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


#ifndef MODULES_AXIS_H_
#define MODULES_AXIS_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/encoder.h"
#include "modules/motor.h"
#include "light_hal/io.h"
#include "utilities/direction.h"

//=================================
// declarations

enum axis_mode_t {
	AXIS_NORMAL,
	AXIS_CAL_FWD,
	AXIS_CAL_BWD,
	AXIS_CAL_FREE,
	AXIS_CAL_FIND
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
	IO_pin_t lim_sw_fwd;

	//limit switch in backward direction
	IO_pin_t lim_sw_bwd;

	//encoder pointer
	volatile struct encoder_t* encoder;

	//was a calibration performed since startup?
	char cal_done;
};

#define AXIS_TPL(min, max, motor, encoder, limsw_fwd, limsw_bwd)\
	{min, max, 0, AXIS_NORMAL, motor, limsw_fwd, limsw_bwd, encoder, 0}


extern void axis_init(volatile struct axis_t* axis);
extern void axis_worker(volatile struct axis_t* axis);
extern int axis_status(volatile struct axis_t* axis);
extern void axis_move_to(volatile struct axis_t* axis, int32_t target);
extern void axis_move(volatile struct axis_t* axis, enum direction_t direction);
extern void axis_cal(volatile struct axis_t* axis, enum direction_t direction);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_AXIS_H_ */
