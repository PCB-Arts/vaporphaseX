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
#include "modules/lid.h"
#include "modules/lid_isr.h"
#include "modules/lift.h"
#include "modules/encoder.h"
#include "modules/event.h"
#include "modules/motor.h"
#include "peripherals/pwm.h"
#include "log.h"
#include "main.h"

//=================================
// definitions

#define LID_MAX INT32_MAX
#define LID_MIN -50

static struct encoder_t lid_encoder;
static bool lid_overcurrent = false;

// use an unused pin to get this compiled
static struct motor_t lid_motor = MOTOR_TPL(
		IO_PIN_TPL(LID_MOTOR_HALFBRIDGE_A_GPIO_Port, LID_MOTOR_HALFBRIDGE_A_Pin),
		IO_PIN_TPL(LID_MOTOR_HALFBRIDGE_B_GPIO_Port, LID_MOTOR_HALFBRIDGE_B_Pin));

struct axis_t lid_axis = AXIS_TPL(LID_MIN, LID_MAX,
		&lid_motor, &lid_encoder,
		IO_PIN_TPL(LID_END_TOP_GPIO_Port, LID_END_TOP_Pin),
		IO_PIN_TPL(LID_END_BOTTOM_GPIO_Port, LID_END_BOTTOM_Pin));

static const IO_pin_t overcurrent_pin = IO_PIN_TPL(LID_OVERCURRENT_GPIO_Port, LID_OVERCURRENT_Pin);


void lid_init() {
	lid_axis.lim_sw_bwd.trigger_propagation_delay_ms = 50;
	lid_axis.lim_sw_fwd.trigger_propagation_delay_ms = 50;
}


void lid_worker() {
	if (iopin_read(overcurrent_pin) == GPIO_PIN_SET) {
		lid_overcurrent_isr();
	}
	axis_worker(&lid_axis);
}

const char *lid_status() {
	enum axis_mode_t status = axis_status(&lid_axis);
	enum direction_t dir = lid_motor.direction;

	if (status == AXIS_ERROR)
		return "error stop";

	if (status == AXIS_NORMAL) {
		if (dir == FWD)
			return "ok fwd";
		if (dir== BWD)
			return "ok bwd";
		return "ok stop";

	} else {
		if (dir == FWD)
			return "cal fwd";
		if (dir == BWD)
			return "cal bwd";
		return "cal stop";
	}

	return "unknown";
}

void lid_encoder_worker() {
	encoder_worker(&lid_encoder);
}

void lid_emergency_stop(void) {
	axis_emergency_stop(&lid_axis);
}

void lid_overcurrent_isr() {
	// According to hardware development no software action needed
	if (!lid_overcurrent){
		vpo_log("Lid overcurrent detected");
		lid_overcurrent = true;
	}
}

void lid_calibrate() {
	axis_cal(&lid_axis, BWD);
}

bool lid_overcurrent_was_detected() {
	return lid_overcurrent;
}
