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
#include "modules/motor.h"
#include "utilities/irq.h"

//=================================
// definitions

void motor_run(struct motor_t* motor, enum direction_t direction) {
	//save new motor direction
	motor->direction = direction;

	//set motor direction
	IOPIN_WRITE(motor->direction_pin, (direction == FWD ? 1 : 0));

	//enable motor driver
	motor->set_enable_pwm(motor->speed);
}

void motor_stop(struct motor_t* motor) {
	//disable motor driver
	motor->set_enable_pwm(0);

	//reset direction
	motor->direction = DIRECTION_NONE;
}

void motor_set_speed(struct motor_t* motor, int speed) {
	//save new speed
	motor->speed = speed;

	disable_irq();

	//update pwm if motor is running
	if (motor->direction != DIRECTION_NONE) {
		//update speed
		motor->set_enable_pwm(motor->speed);
	}

	enable_irq();
}
