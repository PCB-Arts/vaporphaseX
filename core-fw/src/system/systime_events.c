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
#include "system/systime.h"
#include "ui/cli.h"
#include "main.h"
#include "modules/heater.h"
#include "control/sequence.h"
#include "modules/temp_sensors.h"
#include "modules/lid.h"
#include "peripherals/freq.h"

//=================================
// definitions

static volatile uint32_t delay;


void systime_sync_millis(void) {
	max31856_temp_sensors_millis_sync();
	main_millis_sync();
}

void systime_sync_second(void) {
	heater_seconds_sync();
	seq_seconds_sync();
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
	lid_encoder_worker();
	freq_encoder_worker();
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
