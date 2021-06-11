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


#ifndef MODULES_MOTOR_H_
#define MODULES_MOTOR_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "utilities/direction.h"
#include "peripherals/gpio.h"

#include <stdbool.h>

//=================================
// declarations

struct motor_t {
	//motor direction selection pin
	IO_pin_t halfbridge_a_io;
	IO_pin_t halfbridge_b_io;

	IO_pin_t direction_io;
	IO_pin_t enable_io;

	// motor is either stepper or DC motor with a halfbridge
	bool is_stepper;

	void (*set_step_ivl)(int, enum direction_t direction);

	//motor speed (0-100%)
	int speed;
	int min_ivl;

	//rotational direction
	enum direction_t direction;
};

#define MOTOR_TPL(halfbridge_a_io, halfbridge_b_io)\
	{halfbridge_a_io, halfbridge_b_io, {}, {}, false, NULL, 100, 0, DIRECTION_NONE}
#define STEPPER_TPL(direction_io, enable_io, step_ivl)\
	{{}, {}, direction_io, enable_io, true, step_ivl, 100, 20, DIRECTION_NONE}

extern void motor_run(struct motor_t* motor, enum direction_t direction);
extern void motor_stop(struct motor_t* motor);
extern void motor_set_speed(struct motor_t* motor, int speed);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_MOTOR_H_ */
