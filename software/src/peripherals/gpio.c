/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 - PCB Arts GmbH - Kevin Schick
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
#include "peripherals/gpio.h"
#include "hardware_declarations.h"
#include "light_hal/io.h"

#include <stm32f3xx.h>

//=================================
// defines

void gpio_init() {

	IO_TypeDef pin_config;
	pin_config.Type = IO_TYPE_PUSH_PULL;
	pin_config.Pull = IO_PULL_NONE;
	pin_config.Speed = IO_SPEED_LOW;
	pin_config.Alternate = 0b0;


	/////////////////////////////////////////////////////////////
	// outputs

	pin_config.Mode = IO_MODE_OUTPUT;

	//LED 2
	pin_config.Pin = LED_GREEN_PIN;
	io_config(LED_GREEN_PORT, &pin_config);

	//optical signal
	pin_config.Pin = OPTICAL_SIGNAL_PIN;
	io_config(OPTICAL_SIGNAL_PORT, &pin_config);

	//acoustic signal
	pin_config.Pin = ACOUSTC_SIGNAL_PIN;
	io_config(ACOUSTC_SIGNAL_PORT, &pin_config);

	//temperature sensor CS 0
	pin_config.Pin = TEMP_SENSOR_0_CS_PIN;
	io_config(TEMP_SENSOR_0_CS_PORT, &pin_config);

	//temperature sensor CS 1
	pin_config.Pin = TEMP_SENSOR_1_CS_PIN;
	io_config(TEMP_SENSOR_1_CS_PORT, &pin_config);

	//temperature sensor CS 2
	pin_config.Pin = TEMP_SENSOR_2_CS_PIN;
	io_config(TEMP_SENSOR_2_CS_PORT, &pin_config);

	//temperature sensor CS 3
	pin_config.Pin = TEMP_SENSOR_3_CS_PIN;
	io_config(TEMP_SENSOR_3_CS_PORT, &pin_config);

	//temperature sensor CS 4
	pin_config.Pin = TEMP_SENSOR_4_CS_PIN;
	io_config(TEMP_SENSOR_4_CS_PORT, &pin_config);

	//heater enable
	pin_config.Pin = HEATER_ENABLE_PIN;
	io_config(HEATER_ENABLE_PORT, &pin_config);

	//pump enable
	pin_config.Pin = PUMP_ENABLE_PIN;
	io_config(PUMP_ENABLE_PORT, &pin_config);

	//compartment lighting enable
	pin_config.Pin = LIGHTING_ENABLE_PIN;
	io_config(LIGHTING_ENABLE_PORT, &pin_config);

	//lift direction
	pin_config.Pin = LIFT_DIRECTION_PIN;
	io_config(LIFT_DIRECTION_PORT, &pin_config);

	//lid direction
	pin_config.Pin = LID_DIRECTION_PIN;
	io_config(LID_DIRECTION_PORT, &pin_config);

	//LCD chip select
	pin_config.Pin = LCD_CS_PIN;
	io_config(LCD_CS_PORT, &pin_config);

	//LCD reset
	pin_config.Pin = LCD_RESET_PIN;
	io_config(LCD_RESET_PORT, &pin_config);

	//LCD DCX
	pin_config.Pin = LCD_DCX_PIN;
	io_config(LCD_DCX_PORT, &pin_config);


	/////////////////////////////////////////////////////////////
	// inputs

	pin_config.Mode = IO_MODE_INPUT;
	pin_config.Pull = IO_PULL_DOWN;

	//UI encoder pin 1
	pin_config.Pin = UI_ENCODER1_PIN;
	io_config(UI_ENCODER1_PORT, &pin_config);

	//UI encoder pin 2
	pin_config.Pin = UI_ENCODER2_PIN;
	io_config(UI_ENCODER2_PORT, &pin_config);

	//UI select button
	pin_config.Pin = UI_SELECT_BUTTON_PIN;
	io_config(UI_SELECT_BUTTON_PORT, &pin_config);

	//heater over temperature protection status
	pin_config.Pin = HEATER_OTP_STATUS_PIN;
	io_config(HEATER_OTP_STATUS_PORT, &pin_config);

	//lift end switch top
	pin_config.Pin = LIFT_END_TOP_PIN;
	io_config(LIFT_END_TOP_PORT, &pin_config);

	//lift end switch bottom
	pin_config.Pin = LIFT_END_BOTTOM_PIN;
	io_config(LIFT_END_BOTTOM_PORT, &pin_config);

	//lid end switch top
	pin_config.Pin = LID_END_TOP_PIN;
	io_config(LID_END_TOP_PORT, &pin_config);

	//lid end switch bottom
	pin_config.Pin = LID_END_BOTTOM_PIN;
	io_config(LID_END_BOTTOM_PORT, &pin_config);


	//pull-up´s enabled
	pin_config.Pull = IO_PULL_UP;

	//lift encoder pin 1
	pin_config.Pin = LIFT_ENCODER1_PIN;
	io_config(LIFT_ENCODER1_PORT, &pin_config);

	//lift encoder pin 2
	pin_config.Pin = LIFT_ENCODER2_PIN;
	io_config(LIFT_ENCODER2_PORT, &pin_config);

	//lid encoder pin 1
	pin_config.Pin = LID_ENCODER1_PIN;
	io_config(LID_ENCODER1_PORT, &pin_config);

	//lid encoder pin 2
	pin_config.Pin = LID_ENCODER2_PIN;
	io_config(LID_ENCODER2_PORT, &pin_config);

	/////////////////////////////////////////////////////////////
	// analog

	pin_config.Mode = IO_MODE_ANALOG;
	pin_config.Pull = IO_PULL_NONE;


	/////////////////////////////////////////////////////////////
	// alternate functions or special cases

	pin_config.Mode = IO_MODE_FUNCTION;

	//fan PWM
	pin_config.Alternate = FAN_PWM_AF;
	pin_config.Pin = FAN_PWM_PIN;
	io_config(FAN_PWM_PORT, &pin_config);

	//QC fan PWM
	pin_config.Alternate = QC_FAN_PWM_AF;
	pin_config.Pin = QC_FAN_PWM_PIN;
	io_config(QC_FAN_PWM_PORT, &pin_config);

	//lift enable PWM
	pin_config.Alternate = LIFT_ENABLE_AF;
	pin_config.Pin = LIFT_ENABLE_PIN;
	io_config(LIFT_ENABLE_PORT, &pin_config);

	//lid enable PWM
	pin_config.Alternate = LID_ENABLE_AF;
	pin_config.Pin = LID_ENABLE_PIN;
	io_config(LID_ENABLE_PORT, &pin_config);


	pin_config.Speed = IO_SPEED_HIGH;

	//SPI2
	pin_config.Alternate = SPI2_AF;
	pin_config.Pin = SPI2_SCK_PIN;
	io_config(SPI2_SCK_PORT, &pin_config);

	pin_config.Pin = SPI2_MISO_PIN;
	io_config(SPI2_MISO_PORT, &pin_config);

	pin_config.Pin = SPI2_MOSI_PIN;
	io_config(SPI2_MOSI_PORT, &pin_config);

	//SPI2
	pin_config.Alternate = SPI3_AF;
	pin_config.Pin = SPI3_SCK_PIN;
	io_config(SPI3_SCK_PORT, &pin_config);

	pin_config.Pin = SPI3_MISO_PIN;
	io_config(SPI3_MISO_PORT, &pin_config);

	pin_config.Pin = SPI3_MOSI_PIN;
	io_config(SPI3_MOSI_PORT, &pin_config);

	//USART2
	pin_config.Alternate = USART2_AF;
	pin_config.Pin = USART2_TX_PIN;
	io_config(USART2_TX_PORT, &pin_config);
	pin_config.Pin = USART2_RX_PIN;
	io_config(USART2_RX_PORT, &pin_config);
}
