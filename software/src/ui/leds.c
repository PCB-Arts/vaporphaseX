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
#include "ui/leds.h"
#include "hardware_declarations.h"
#include "light_hal/io.h"

//=================================
// definitions

#define LED_BLINK_SLOW_MS 1000
#define LED_BLINK_FAST_MS 100

#define LED_FLASH_ON_MS 100
#define LED_FLASH_OFF_MS 100
#define LED_FLASH_DELAY_MS 800

//led initialization template
#define LED_TPL(port, pin) {pin, 0, LED_MODE_EXTERNAL, 0, port}

struct LED_t LED_green = LED_TPL(LED_GREEN_PORT, LED_GREEN_PIN);

static void LED_update(struct LED_t* led);

void ui_led_millis_sync() {
	LED_update(&LED_green);
}

void LED_update(struct LED_t* led) {
	if (led->delay) {
		led->delay--;
		return;
	}

	switch (led->mode) {
	default:
	case LED_MODE_EXTERNAL:
	break;

	case LED_MODE_BLINK_SLOW:
		led->delay = LED_BLINK_SLOW_MS;
		io_toggle_pins(led->GPIOx, IO_MASK(led->pin_mask));
		break;

	case LED_MODE_BLINK_FAST:
		led->delay = LED_BLINK_FAST_MS;
		io_toggle_pins(led->GPIOx, IO_MASK(led->pin_mask));
		break;

	case LED_MODE_FLASH:
		switch  (led->flash_state) {
		case LED_MODE_FLASH_ON_1:
		case LED_MODE_FLASH_ON_2:
			led->delay = LED_FLASH_ON_MS;
			io_write_pin(led->GPIOx, led->pin_mask, 1);
			++led->flash_state;
			break;

		case LED_MODE_FLASH_OFF_1:
			led->delay = LED_FLASH_OFF_MS;
			io_write_pin(led->GPIOx, led->pin_mask, 0);
			led->flash_state = LED_MODE_FLASH_ON_2;
			break;

		case LED_MODE_FLASH_OFF_2:
			led->delay = LED_FLASH_DELAY_MS;
			io_write_pin(led->GPIOx, led->pin_mask, 0);
			led->flash_state = LED_MODE_FLASH_ON_1;
			break;
		}
	}
}
