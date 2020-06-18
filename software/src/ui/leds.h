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


#ifndef UI_LEDS_H_
#define UI_LEDS_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include <stm32f3xx.h>
#include <stdint.h>

//=================================
// declarations

enum LED_mode_t {
	LED_MODE_EXTERNAL,
	LED_MODE_BLINK_SLOW,
	LED_MODE_BLINK_FAST,
	LED_MODE_FLASH
};

enum LED_flash_state_t {
	LED_MODE_FLASH_ON_1,
	LED_MODE_FLASH_OFF_1,
	LED_MODE_FLASH_ON_2,
	LED_MODE_FLASH_OFF_2
};

struct LED_t {
	const uint32_t pin_mask;
	uint16_t delay;
	enum LED_mode_t mode;
	enum LED_flash_state_t flash_state;
	GPIO_TypeDef* const GPIOx;
};

extern struct LED_t LED_green;

extern void ui_led_millis_sync();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UI_LEDS_H_ */
