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

#ifndef __FREQ_H__
#define __FREQ_H__


#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/encoder.h"
#include "utilities/direction.h"
#include "stm32f3xx_hal_tim.h"

#include <stdint.h>


// This module provides functionality to generate pulses using a TIM pwm generatation
// It also counts the pulses and writes them into the position field of the emulated_encoder var.
//=================================
// class

void freq_init(TIM_HandleTypeDef* htim, uint16_t channel, TIM_HandleTypeDef* pulseCounterHtim);
void freq_set(int interval_us, enum direction_t direction);
void freq_encoder_worker();

extern struct encoder_t freq_encoder;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __FREQ_H__ */
