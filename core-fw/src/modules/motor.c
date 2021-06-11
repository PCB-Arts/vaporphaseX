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
#include "modules/motor.h"
#include "utilities/irq.h"

static void stepper_set_speed(struct motor_t *motor, int speed);

//=================================
// definitions


void motor_run(struct motor_t* motor, enum direction_t direction) {
	//save new motor direction
	motor->direction = direction;

	//enable motor driver
	if (motor->is_stepper) {
		//set motor direction
		iopin_write(motor->direction_io, direction == FWD ? 0 : 1);
		stepper_set_speed(motor, motor->speed);
		iopin_write(motor->enable_io, 1);

	} else {
		if(motor->direction == FWD) {
			iopin_write(motor->halfbridge_a_io, 1);
			iopin_write(motor->halfbridge_b_io, 0);
		} else {
			iopin_write(motor->halfbridge_a_io, 0);
			iopin_write(motor->halfbridge_b_io, 1);
		}
	}
}

void motor_stop(struct motor_t* motor) {
	//disable motor driver
	if (motor->is_stepper) {
		stepper_set_speed(motor, 0);
		iopin_write(motor->enable_io, 0);

	} else {
		iopin_write(motor->halfbridge_a_io, 1);
		iopin_write(motor->halfbridge_b_io, 1);
	}

	//reset direction
	motor->direction = DIRECTION_NONE;
}

void motor_set_speed(struct motor_t* motor, int speed) {
	//save new speed
	motor->speed = speed;

	disable_irq();

	if (motor->is_stepper) {
		stepper_set_speed(motor, speed);

	} else {
		// TODO: create possibility to set speed of the dc motor
		// we would need a connected timer for the motor pins
	}

	enable_irq();
}

static void stepper_set_speed(struct motor_t *motor, int speed) {
	if (speed == 0) {
		motor->set_step_ivl(0, motor->direction);

	} else {
		// multiply by 4 (from 2mm/step to 8mm/step)
		const int base = 1E6 / 3600 * 4;
		// TODO: define and DOCUMENT function to use here
		// 1 -> 100
		// 100 -> 1E6 / 3600
		motor->set_step_ivl(base * 100 / speed, motor->direction);
	}
}

