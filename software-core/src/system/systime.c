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
#include "interrupt_priorities.h"

#include <stm32f3xx.h>

//=================================
// definitions

//systick frequency = 100 kHz
#define SYSTICK_FREQ 100000

struct systime_sync_flag_t {
	uint8_t millis;
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t days;
} static volatile sync_flag = {0};

static volatile struct timepoint_t systime_tp = {0};
static void systime_slicer();

void systime_init() {
	//configure frequency
	SysTick_Config(SystemCoreClock / SYSTICK_FREQ);

	//set interrupt priority
	NVIC_SetPriority(SysTick_IRQn, NVIC_PRIO_SYSTIME);
}

void SysTick_Handler() {
	systime_slicer();
}

void systime_timepoint_reset(volatile struct timepoint_t* tp) {
	*tp = (const struct timepoint_t){0};
}

struct timepoint_t systime_get_timepoint() {
	//return a copy of systime
	return systime_tp;
}

void systime_slicer() {
	//relative time slicer
	//
	//events are fired after all propagation overflows were performed:
	// 999ms & 59s & 59min & 23h & 0day + 1ms = 0ms & 0s & 0min & 0h & 1day
	//
	//events are fired in order: day, hour, minute, second, millis, micros10

	if (++systime_tp.micros10 >= 100){
		systime_tp.micros10 = 0;
		sync_flag.millis = 1;

		if (++systime_tp.millis >= 1000) {
			systime_tp.millis = 0;
			sync_flag.seconds = 1;

			if (++systime_tp.seconds >= 60) {
				systime_tp.seconds = 0;
				sync_flag.minutes = 1;

				if (++systime_tp.minutes >= 60) {
					systime_tp.minutes = 0;
					sync_flag.hours = 1;

					if (++systime_tp.hours >= 24) {
						systime_tp.hours = 0;
						sync_flag.days = 1;

						//prevent overflow
						if (systime_tp.days < 0xFFFF) {
							++systime_tp.days;
						}

						systime_async_day();
					}
					systime_async_hour();
				}
				systime_async_minute();
			}
			systime_async_second();
		}
		systime_async_millis();
	}
	systime_async_micros10();
}

//translate asynchronous systime events into synchronous events
//asynchronous events are executed form within the interrupt context
//synchronous events are executed from within the main execution context
void systime_worker() {
	if (sync_flag.millis){
		if (sync_flag.seconds){
			if (sync_flag.minutes){
				if (sync_flag.hours){
					if (sync_flag.days){
						systime_sync_day();
					}
					systime_sync_hour();
				}
				systime_sync_minute();
			}
			systime_sync_second();
		}
		systime_sync_millis();

		//reset all sync flags
		sync_flag = (const struct systime_sync_flag_t){0};
	}
}
