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
#include "system/systime.h"
#include "ui/leds.h"
#include "ui/lcd.h"
#include "ui/cli.h"
#include "main.h"
#include "modules/heater.h"
#include "modules/sequence.h"
#include "modules/temp_sensors.h"
#include "modules/ili9341/ili9341_core.h"

//=================================
// definitions

static volatile uint32_t delay;


void systime_sync_millis(void) {
	ui_lcd_millis_sync();
	ui_led_millis_sync();
	temp_sensors_millis_sync();
	main_millis_sync();
}

void systime_sync_second(void) {
	heater_seconds_sync();
	seq_seconds_sync();
	main_seconds_sync();
}

void systime_sync_minute(void) {

}

void systime_sync_hour(void) {

}

void systime_sync_day(void) {

}



void systime_async_micros10(void) {

}

void systime_async_millis(void) {
	if (delay) delay--;
}

void systime_async_second(void) {

}

void systime_async_minute(void) {

}

void systime_async_hour(void) {

}

void systime_async_day(void) {

}

void delay_ms(uint32_t d) {
	delay = d;
	while(delay);
}
