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
#include "peripherals/pwm.h"
#include "main.h"
#include "stm32f3xx_hal_tim.h"

#include <stm32f3xx.h>

#include "conf.h"

extern TIM_HandleTypeDef htim4;

TIM_HandleTypeDef* fan_pwm_htim = &htim4;
uint32_t fan_pwm_channel = TIM_CHANNEL_2;

#if !MOCK_CORE

//=================================
// defines

void pwm_set_FAN(int value) {
	float p = value / 100.0f;
	uint32_t pulse = (__HAL_TIM_GET_AUTORELOAD(fan_pwm_htim) + 1) * p;

	__HAL_TIM_SET_COMPARE(fan_pwm_htim, fan_pwm_channel, pulse);

	HAL_TIM_PWM_Start(fan_pwm_htim, fan_pwm_channel);
}

#endif
