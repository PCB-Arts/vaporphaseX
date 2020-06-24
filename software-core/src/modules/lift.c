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
#include "modules/lift.h"
#include "modules/lift_isr.h"
#include "modules/encoder.h"
#include "light_hal/io.h"
#include "hardware_declarations.h"
#include "modules/motor.h"
#include "peripherals/pwm.h"

//=================================
// definitions

#define LIFT_MAX 50
#define LIFT_MIN -111111

static volatile struct encoder_t lift_encoder;

static struct motor_t lift_motor = MOTOR_TPL(
		IO_PIN_TPL(LIFT_DIRECTION_PORT, LIFT_DIRECTION_PIN),
		&pwm_set_TIM1CH3);

volatile struct axis_t lift_axis = AXIS_TPL(LIFT_MIN, LIFT_MAX,
		&lift_motor, &lift_encoder,
		IO_PIN_TPL(LIFT_END_TOP_PORT, LIFT_END_TOP_PIN),
		IO_PIN_TPL(LIFT_END_BOTTOM_PORT, LIFT_END_BOTTOM_PIN));


void lift_worker() {
	axis_worker(&lift_axis);
}

void lift_encoder_isr() {
	unsigned int t1;

	//build new t1 value from the two encoder pins
	t1 = IO_READ_PIN(LIFT_ENCODER2_PORT, LIFT_ENCODER2_PIN) << 1;
	t1 |= IO_READ_PIN(LIFT_ENCODER1_PORT, LIFT_ENCODER1_PIN) & 0b01;

	//update encoder
	encoder_step(&lift_encoder, t1);
}
