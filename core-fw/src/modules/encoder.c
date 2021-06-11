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
#include "modules/encoder.h"
#include "utilities/direction.h"
#include "peripherals/tim.h"
#include "limits.h"
#include "assert.h"

//=================================
// definitions

void encoder_init(struct encoder_t* enc) {
	// ensure that timer config covers the complete int16 range
	assert(tim_get_autoreload(enc->htim) == (uint16_t)-1);

	enc->counter_value = tim_get_counter(enc->htim);
	enc->position = 0;

	if(enc->simulated_encoder) {
		tim_base_start(enc->htim);
		return;
	}

    tim_encoder_start(enc->htim);

    if(enc->factor == 0) {
    	enc->factor = 1;
    }
}

void encoder_worker(struct encoder_t* enc) {
	int16_t old_value = enc->counter_value;
	int16_t new_value = tim_get_counter(enc->htim);

    int16_t diff = new_value - old_value;

    enc->position += diff * enc->factor;
    enc->counter_value = new_value;
}
