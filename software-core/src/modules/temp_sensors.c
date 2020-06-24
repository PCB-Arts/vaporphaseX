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
#include "modules/temp_sensors.h"
#include "modules/max31856/MAX31856_def.h"
#include "peripherals/spi.h"
#include "hardware_declarations.h"
#include "utilities/arrays.h"

#include <stdint.h>

//=================================
// defines

#define TEMP_SENSOR_CYCLE_MS 250


struct MAX31856_t TSNS_0 = MAX31856_TPL( SPI_DEV_TPL( IO_PIN_TPL(
		TEMP_SENSOR_0_CS_PORT, TEMP_SENSOR_0_CS_PIN), &spi3));

struct MAX31856_t TSNS_1 = MAX31856_TPL( SPI_DEV_TPL( IO_PIN_TPL(
		TEMP_SENSOR_1_CS_PORT, TEMP_SENSOR_1_CS_PIN), &spi3));

struct MAX31856_t TSNS_2 = MAX31856_TPL( SPI_DEV_TPL( IO_PIN_TPL(
		TEMP_SENSOR_2_CS_PORT, TEMP_SENSOR_2_CS_PIN), &spi3));

struct MAX31856_t TSNS_3 = MAX31856_TPL( SPI_DEV_TPL( IO_PIN_TPL(
		TEMP_SENSOR_3_CS_PORT, TEMP_SENSOR_3_CS_PIN), &spi3));

struct MAX31856_t TSNS_4 = MAX31856_TPL( SPI_DEV_TPL( IO_PIN_TPL(
		TEMP_SENSOR_4_CS_PORT, TEMP_SENSOR_4_CS_PIN), &spi3));


struct MAX31856_t* temp_sensors[] = {
		&TSNS_0,
		&TSNS_1,
		&TSNS_2,
		&TSNS_3,
		&TSNS_4
};

const size_t temp_sensors_size = SIZEOFA(temp_sensors);
int temp_sensor_cycle_complete;


static void schedule_LTC_read();


void temp_sensors_init() {
	//build CR0 value
	const uint8_t CR0 = MAX31856_CR0_CMODE | MAX31856_CR0_5060HZ;

	//build CR1 value
	uint8_t CR1 = MAX31856_CR1_DEFAULT;
	MODIFY_REG(CR1, MAX31856_CR1_AVGSEL, (0b000 << 4));

	//apply to all temperature sensors
	for (int i = 0; i < SIZEOFA(temp_sensors); ++i) {
		temp_sensors[i]->CR0 = CR0;
		temp_sensors[i]->CR1 = CR1;

		//set CS idle
		spi_stop(&temp_sensors[i]->spid);
	}
}

extern void temp_sensors_worker() {
	static unsigned int i;

	if (i >= temp_sensors_size) {
		i = 0;
		temp_sensor_cycle_complete = 1;
	}

	if (!MAX31856_process(temp_sensors[i]))
		i++;
}

void temp_sensors_millis_sync() {
	static unsigned int delay;

	if (delay) {
		delay--;
	} else {
		delay = TEMP_SENSOR_CYCLE_MS;
		schedule_LTC_read();
	}
}

static void schedule_LTC_read() {
	for (int i = 0; i < temp_sensors_size; ++i) {
		temp_sensors[i]->tasks.reg.LTC |= MAX31856_TASK_READ;
	}
}
