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

#include "peripherals/freq.h"
#include "interrupt_priorities.h"

#include "tim.h"
#include "assert.h"

#include "conf.h"

enum direction_t freq_encoder_direction = DIRECTION_NONE;
struct encoder_t freq_encoder = { .simulated_encoder = true };

void freq_encoder_worker() {
	encoder_worker(&freq_encoder);
}

static TIM_HandleTypeDef* freq_htim;
static uint16_t freq_channel;

// expect base frequency 2kHz --> smallest interval length 1ms
#define EMULATED_ENCODER_EXPECTED_PRESCALER 31

void freq_init(TIM_HandleTypeDef* htim, uint16_t channel, TIM_HandleTypeDef* pulseCounterHtim) {
	freq_htim = htim;
	freq_channel = channel;
	freq_encoder.htim = pulseCounterHtim;

	assert(htim->Instance->PSC == EMULATED_ENCODER_EXPECTED_PRESCALER);

	encoder_init(&freq_encoder);

	// start counter and pwm signal output, but set interval to 0
	// this means the counter is running, but no pulses are sent
	freq_set(0, DIRECTION_NONE);
}



/* Set pulse interval to interval_ms milliseconds. To disable, pass interval_ms=0. */
void freq_set(int interval_us, enum direction_t direction) {
	freq_encoder_direction = direction;
	tim_pwm_stop(freq_htim, freq_channel);

	if (direction == DIRECTION_NONE || interval_us == 0) {
		return;
	}

	tim_set_autoreload(freq_htim, interval_us * 2 - 1);
	tim_set_compare(freq_htim, freq_channel, interval_us);

	if (direction == FWD) {
		freq_encoder.factor = 1;
	} else {
		freq_encoder.factor = -1;
	}

	tim_pwm_start(freq_htim, freq_channel);
}

