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
#include "modules/lid.h"
#include "modules/lid_isr.h"
#include "modules/encoder.h"
#include "light_hal/io.h"
#include "hardware_declarations.h"
#include "modules/motor.h"
#include "peripherals/pwm.h"

//=================================
// definitions

#define LID_MAX 30000
#define LID_MIN -50

static volatile struct encoder_t lid_encoder;

static struct motor_t lid_motor = MOTOR_TPL(
		IO_PIN_TPL(LID_DIRECTION_PORT, LID_DIRECTION_PIN),
		&pwm_set_TIM1CH2);

volatile struct axis_t lid_axis = AXIS_TPL(LID_MIN, LID_MAX,
		&lid_motor, &lid_encoder,
		IO_PIN_TPL(LID_END_TOP_PORT, LID_END_TOP_PIN),
		IO_PIN_TPL(LID_END_BOTTOM_PORT, LID_END_BOTTOM_PIN));


void lid_worker() {
	axis_worker(&lid_axis);
}

void lid_encoder_isr() {
	unsigned int t1;

	//build new t1 value from the two encoder pins
	t1 = IO_READ_PIN(LID_ENCODER2_PORT, LID_ENCODER2_PIN) << 1;
	t1 |= IO_READ_PIN(LID_ENCODER1_PORT, LID_ENCODER1_PIN) & 0b01;

	//update encoder
	encoder_step(&lid_encoder, t1);
}
