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


#ifndef MODULES_ENCODER_H_
#define MODULES_ENCODER_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include <stdint.h>
#include "global.h"

//=================================
// declarations

struct encoder_t {
	//encoder position absolute
	__IO int32_t position;

	// the encoder is simulated,
	// meaning the timer is not controlled by an external encoder
	bool simulated_encoder;

	// timer in encoder mode
	TIM_HandleTypeDef* htim;

	// private value for difference calculation
	int16_t counter_value;

	int16_t factor;
};

extern void encoder_init(struct encoder_t* enc);

/*
 * has to be called faster than the counter can count 0x7F ticks
 */
extern void encoder_worker(struct encoder_t* enc);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_ENCODER_H_ */
