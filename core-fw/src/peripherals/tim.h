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

#ifndef PERIPHERALS_TIM_H_
#define PERIPHERALS_TIM_H_

#include "global.h"

uint32_t tim_get_counter(TIM_HandleTypeDef* htim);
uint32_t tim_get_autoreload(TIM_HandleTypeDef* htim);
void tim_base_start(TIM_HandleTypeDef* htim);
void tim_encoder_start(TIM_HandleTypeDef* htim);
void tim_pwm_start(TIM_HandleTypeDef* htim, uint16_t channel);
void tim_pwm_stop(TIM_HandleTypeDef* htim, uint16_t channel);
void tim_set_autoreload(TIM_HandleTypeDef* htim, uint32_t arr);
void tim_set_compare(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t compare);

#endif /* PERIPHERALS_TIM_H_ */
