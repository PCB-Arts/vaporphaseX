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


#ifndef MODULES_MOTOR_H_
#define MODULES_MOTOR_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "light_hal/io.h"
#include "utilities/direction.h"

//=================================
// declarations

struct motor_t {
	//motor direction selection pin
	IO_pin_t direction_pin;

	//motor enable pwm
	void (*set_enable_pwm)(int);

	//motor speed (0-100%)
	int speed;

	//rotational direction
	enum direction_t direction;
};

#define MOTOR_TPL(direction_io, enable_pwm)\
	{direction_io, enable_pwm, 100, DIRECTION_NONE}

extern void motor_run(struct motor_t* motor, enum direction_t direction);
extern void motor_stop(struct motor_t* motor);
extern void motor_set_speed(struct motor_t* motor, int speed);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_MOTOR_H_ */
