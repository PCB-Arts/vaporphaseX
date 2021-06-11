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

#include "tim.h"
#include "conf.h"

#if !MOCK_CORE

uint32_t tim_get_counter(TIM_HandleTypeDef* htim) {
	return __HAL_TIM_GET_COUNTER(htim);
}

uint32_t tim_get_autoreload(TIM_HandleTypeDef* htim) {
	return __HAL_TIM_GET_AUTORELOAD(htim);
}

void tim_base_start(TIM_HandleTypeDef* htim) {
	HAL_TIM_Base_Start(htim);
}

void tim_encoder_start(TIM_HandleTypeDef* htim) {
	HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
}

void tim_pwm_start(TIM_HandleTypeDef* htim, uint16_t channel) {
	HAL_TIM_PWM_Start(htim, channel);
}
void tim_pwm_stop(TIM_HandleTypeDef* htim, uint16_t channel) {
	HAL_TIM_PWM_Stop(htim, channel);
}

void tim_set_autoreload(TIM_HandleTypeDef* htim, uint32_t arr) {
	__HAL_TIM_SET_AUTORELOAD(htim, arr);
}

void tim_set_compare(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t compare) {
	__HAL_TIM_SET_COMPARE(htim, channel, compare);
}

#endif

