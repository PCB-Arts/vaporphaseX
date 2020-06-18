/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 PCB Arts GmbH - Kevin Schick
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
#include "modules/heater.h"
#include "utilities/irq.h"
#include "hardware_declarations.h"

//=================================
// definitions

#define HEATER_WD_S 5
#define HEATER_OTP_C 350.0f
#define HEATER_OTP_HYST 5.0f

#define HEATER_MASK_ERR_EN 0b00000000

static struct MAX31856_t* sensor;
static uint8_t flags;
static uint16_t watchdog_s = HEATER_WD_S;

static float set_temp, set_hyst;
static int regulation, enabled;


static int sensor_valid();
static void check_max_temp();

static int has_faults();
static void set_flag(uint8_t flag);
static int operational_condition();


void heater_init(struct MAX31856_t* s) {
	sensor = s;
}

static int sensor_valid() {
	if (MAX31856_has_fault(sensor)) {
		//set sensor fault
		set_flag(HEATER_ERR_SENSOR);
		return 0;
	} else {
		return 1;
	}
}

void heater_worker() {
	if (!operational_condition())
		return;

	//reset heater watchdog timeout
	watchdog_s = HEATER_WD_S;

	//check if regulation is enabled
	if (regulation) {
		const float temp = sensor->temperature;

		if (temp >= set_temp) {
			//disable heater if set temperature is reached
			heater_disable();

		} else if (temp < set_temp - set_hyst) {
			//enable heater if set temperature is not reached
			heater_enable();
		}
	}
}

static void check_max_temp() {
	//check max. allowed temperature limit
	if (sensor->temperature >= HEATER_OTP_C) {
		//set over temperature flag
		set_flag(HEATER_ERR_MAX_TEMP);
	}
}

void heater_seconds_sync() {
	if (watchdog_s) {
		//decrement watchdog timer
		--watchdog_s;
	} else {
		//set watchdog fault
		set_flag(HEATER_ERR_WATCHDOG);
	}
}

static void set_flag(uint8_t flag) {
	//set flag
	SET_BIT(flags, flag);

	//disable heater if has faults
	if (has_faults())
		heater_disable();
}

uint8_t heater_get_flags() {
	return flags;
}

void heater_set_temperature(float temperature) {
	set_temp = temperature;
}

void heater_set_hysteresis(float hysteresis) {
	set_hyst = hysteresis;
}

void heater_regulation_enable() {
	regulation = 1;
}

void heater_regulation_disable() {
	regulation = 0;
}

void heater_enable() {
	disable_irq();

	enabled = !has_faults();

	//enabled only if no heater faults are active
	io_write_pin(HEATER_ENABLE_PORT, HEATER_ENABLE_PIN, enabled);

	enable_irq();
}

void heater_disable() {
	enabled = 0;
	io_write_pin(HEATER_ENABLE_PORT, HEATER_ENABLE_PIN, enabled);
}

static int has_faults() {
	//mask predefined flags for faults
	return (flags & ~(HEATER_MASK_ERR_EN)) != 0;
}

static int operational_condition() {
	if (!sensor_valid())
		return 0;

	check_max_temp();

	if (IO_READ_PIN(HEATER_OTP_STATUS_PORT, HEATER_OTP_STATUS_PIN))
		heater_overtemp();

	return !has_faults();
}

int heater_temperature_reached() {
	if (!sensor_valid())
		return 0;

	const float temp = sensor->temperature;

	return (temp >= set_temp - set_hyst);
}

void heater_overtemp() {
	//set OTP tripped flag
	set_flag(HEATER_ERR_OTP_TRIPPED);
}

void heater_reset_flags() {
	CLEAR_REG(flags);
	operational_condition();
}
