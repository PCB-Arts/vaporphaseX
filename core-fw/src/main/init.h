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

#ifndef MAIN_INIT_H_
#define MAIN_INIT_H_

#include "com_stack/profile_com.h"
#include "control/init.h"
#include "ui/cli.h"
#include "ui/cli_commands.h"
#include "system/core_clock.h"
#include "system/systime.h"
#include "interrupt_priorities.h"
#include "peripherals/adc.h"
#include "peripherals/freq.h"
#include "peripherals/spi.h"
#include "peripherals/comm_usart.h"
#include "peripherals/pwm.h"
#include "peripherals/wwdg.h"
#include "peripherals/peripheral_mock.h"
#include "modules/axis.h"
#include "modules/lid.h"
#include "modules/lift.h"
#include "modules/regulator.h"
#include "modules/temp_sensors.h"
#include "modules/heater.h"
#include "modules/fan.h"
#include "modules/fans.h"
#include "modules/pump.h"
#include "modules/max31856/MAX31856.h"
#include "modules/temp_sensor.h"
#include "packet_handler.h"

#include "log.h"
#include "version.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern CRC_HandleTypeDef hcrc;

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim15;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern AdcSignalHandle ntc_adc_signal;

void init_modules();

#endif
