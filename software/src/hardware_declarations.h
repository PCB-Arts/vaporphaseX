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


#ifndef HARDWARE_DECLARATIONS_H_
#define HARDWARE_DECLARATIONS_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "light_hal/io.h"

#include <stm32f3xx.h>

//=================================
// declarations


/////////////////////////////////////////////////////////////////////
// user interface

#define LED_GREEN_PORT			GPIOD
#define LED_GREEN_PIN			IO_PIN13

#define UI_ENCODER1_PORT		GPIOC
#define UI_ENCODER1_PIN			IO_PIN14

#define UI_ENCODER2_PORT		GPIOC
#define UI_ENCODER2_PIN			IO_PIN15

#define UI_SELECT_BUTTON_PORT		GPIOC
#define UI_SELECT_BUTTON_PIN		IO_PIN13

#define OPTICAL_SIGNAL_PORT		GPIOC
#define OPTICAL_SIGNAL_PIN		IO_PIN08

#define ACOUSTC_SIGNAL_PORT		GPIOC
#define ACOUSTC_SIGNAL_PIN		IO_PIN09

#define LCD_CS_PORT			GPIOB
#define LCD_CS_PIN			IO_PIN12

#define LCD_DCX_PORT			GPIOC
#define LCD_DCX_PIN			IO_PIN07

#define LCD_RESET_PORT			GPIOB
#define LCD_RESET_PIN			IO_PIN00

#define TOUCH_CS_PORT			GPIOA
#define TOUCH_CS_PIN			IO_PIN01


/////////////////////////////////////////////////////////////////////
// SPI 3

#define SPI3_AF				IO_AF06
#define SPI3_SCK_PORT			GPIOC
#define SPI3_SCK_PIN			IO_PIN10
#define SPI3_MOSI_PORT			GPIOC
#define SPI3_MOSI_PIN			IO_PIN12
#define SPI3_MISO_PORT			GPIOC
#define SPI3_MISO_PIN			IO_PIN11


/////////////////////////////////////////////////////////////////////
// SPI 2

#define SPI2_AF				IO_AF05
#define SPI2_SCK_PORT			GPIOB
#define SPI2_SCK_PIN			IO_PIN13
#define SPI2_MOSI_PORT			GPIOB
#define SPI2_MOSI_PIN			IO_PIN15
#define SPI2_MISO_PORT			GPIOB
#define SPI2_MISO_PIN			IO_PIN14


/////////////////////////////////////////////////////////////////////
// Temperature sensors

#define TEMP_SENSOR_0_CS_PORT		GPIOC
#define TEMP_SENSOR_0_CS_PIN		IO_PIN06

#define TEMP_SENSOR_1_CS_PORT		GPIOC
#define TEMP_SENSOR_1_CS_PIN		IO_PIN05

#define TEMP_SENSOR_2_CS_PORT		GPIOA
#define TEMP_SENSOR_2_CS_PIN		IO_PIN07

#define TEMP_SENSOR_3_CS_PORT		GPIOA
#define TEMP_SENSOR_3_CS_PIN		IO_PIN06

#define TEMP_SENSOR_4_CS_PORT		GPIOA
#define TEMP_SENSOR_4_CS_PIN		IO_PIN05


/////////////////////////////////////////////////////////////////////
// heater

#define HEATER_ENABLE_PORT		GPIOA
#define HEATER_ENABLE_PIN		IO_PIN01

#define HEATER_OTP_STATUS_PORT		GPIOA
#define HEATER_OTP_STATUS_PIN		IO_PIN04


/////////////////////////////////////////////////////////////////////
// cooling

#define PUMP_ENABLE_PORT		GPIOA
#define PUMP_ENABLE_PIN			IO_PIN00

#define FAN_PWM_AF			IO_AF01
#define FAN_PWM_PORT			GPIOB
#define FAN_PWM_PIN			IO_PIN11

#define QC_FAN_PWM_AF			IO_AF01
#define QC_FAN_PWM_PORT			GPIOB
#define QC_FAN_PWM_PIN			IO_PIN10


/////////////////////////////////////////////////////////////////////
// compartment

#define LIGHTING_ENABLE_PORT		GPIOA
#define LIGHTING_ENABLE_PIN		IO_PIN08


/////////////////////////////////////////////////////////////////////
// lift

#define LIFT_DIRECTION_PORT		GPIOB
#define LIFT_DIRECTION_PIN		IO_PIN05

#define LIFT_ENABLE_AF			IO_AF06
#define LIFT_ENABLE_PORT		GPIOA
#define LIFT_ENABLE_PIN			IO_PIN10

#define LIFT_ENCODER1_PORT		GPIOB
#define LIFT_ENCODER1_PIN		IO_PIN09

#define LIFT_ENCODER2_PORT		GPIOB
#define LIFT_ENCODER2_PIN		IO_PIN08

#define LIFT_END_TOP_PORT		GPIOC
#define LIFT_END_TOP_PIN		IO_PIN00

#define LIFT_END_BOTTOM_PORT		GPIOC
#define LIFT_END_BOTTOM_PIN		IO_PIN01


/////////////////////////////////////////////////////////////////////
// lid

#define LID_DIRECTION_PORT		GPIOB
#define LID_DIRECTION_PIN		IO_PIN04

#define LID_ENABLE_AF			IO_AF06
#define LID_ENABLE_PORT			GPIOA
#define LID_ENABLE_PIN			IO_PIN09

#define LID_ENCODER1_PORT		GPIOB
#define LID_ENCODER1_PIN		IO_PIN07

#define LID_ENCODER2_PORT		GPIOB
#define LID_ENCODER2_PIN		IO_PIN06

#define LID_END_TOP_PORT		GPIOC
#define LID_END_TOP_PIN			IO_PIN02

#define LID_END_BOTTOM_PORT		GPIOC
#define LID_END_BOTTOM_PIN		IO_PIN03


/////////////////////////////////////////////////////////////////////
// communication interface

#define USB_AF
#define USB_DM_PORT			GPIOA
#define USB_DM_PIN			IO_PIN11
#define USB_DP_PORT			GPIOA
#define USB_DP_PIN			IO_PIN12

#define USART2_AF			IO_AF07
#define USART2_TX_PORT			GPIOA
#define USART2_TX_PIN			IO_PIN02
#define USART2_RX_PORT			GPIOA
#define USART2_RX_PIN			IO_PIN03


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HARDWARE_DECLARATIONS_H_ */
