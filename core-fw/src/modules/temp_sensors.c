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
#include "modules/temp_sensors.h"
#include "modules/max31856/MAX31856_def.h"
#include "peripherals/spi.h"
#include "utilities/arrays.h"
#include "main.h"

#include <stdint.h>

//=================================
// defines
// CS0 = Heatertemp 1
// CS1 = Heatertemp 2
// CS2 = Galdentemp
// CS3 = PCBtemp

#define TEMP_SENSOR_CYCLE_MS 250


struct MAX31856_t max31856_temperature_sensor_heater_1 = MAX31856_TPL( SPI_DEV_TPL( IO_PIN_TPL(
		TEMP_SENSOR_0_CS_GPIO_Port, TEMP_SENSOR_0_CS_Pin), &spi1));

struct MAX31856_t max31856_temperature_sensor_heater_2 = MAX31856_TPL( SPI_DEV_TPL( IO_PIN_TPL(
		TEMP_SENSOR_1_CS_GPIO_Port, TEMP_SENSOR_1_CS_Pin), &spi1));

struct MAX31856_t max31856_temperature_sensor_pcb = MAX31856_TPL( SPI_DEV_TPL( IO_PIN_TPL(
		TEMP_SENSOR_3_CS_GPIO_Port, TEMP_SENSOR_3_CS_Pin), &spi1));

struct MAX31856_t max31856_temperature_sensor_galden = MAX31856_TPL( SPI_DEV_TPL( IO_PIN_TPL(
		TEMP_SENSOR_2_CS_GPIO_Port, TEMP_SENSOR_2_CS_Pin), &spi1));

struct MAX31856_t* max31856_temp_sensors[] = {
		&max31856_temperature_sensor_heater_1,
		&max31856_temperature_sensor_heater_2,
		&max31856_temperature_sensor_pcb,
		&max31856_temperature_sensor_galden,
};

TemperatureSensor temperature_sensor_heater_1 = {
		.type = TemperatureSensorTypeMax31856,
		.max31856 = &max31856_temperature_sensor_heater_1
};
TemperatureSensor temperature_sensor_heater_2 = {
		.type = TemperatureSensorTypeMax31856,
		.max31856 = &max31856_temperature_sensor_heater_2
};
TemperatureSensor temperature_sensor_pcb = {
		.type = TemperatureSensorTypeMax31856,
		.max31856 = &max31856_temperature_sensor_pcb
};
TemperatureSensor temperature_sensor_galden = {
		.type = TemperatureSensorTypeMax31856,
		.max31856 = &max31856_temperature_sensor_galden
};

TemperatureSensor temperature_sensor_water = {
		.type = TemperatureSensorNTC,
		.ntc = &ntc_temperature_sensor_water,
};

TemperatureSensor* temp_sensors[] = {
		&temperature_sensor_heater_1,
		&temperature_sensor_heater_2,
		&temperature_sensor_pcb,
		&temperature_sensor_galden,
		&temperature_sensor_water,
};
const size_t temp_sensors_size = SIZEOFA(temp_sensors);

const size_t max31856_temp_sensors_size = SIZEOFA(max31856_temp_sensors);
int max31856_temp_sensor_cycle_complete;


static void schedule_LTC_read();


void max31856_temp_sensors_init() {
	//build CR0 value
	const uint8_t CR0 = MAX31856_CR0_CMODE | MAX31856_CR0_5060HZ;

	//build CR1 value
	uint8_t CR1 = MAX31856_CR1_DEFAULT;
	MODIFY_REG(CR1, MAX31856_CR1_AVGSEL, (0b000 << 4));

	//apply to all temperature sensors
	for (int i = 0; i < max31856_temp_sensors_size; ++i) {
		max31856_temp_sensors[i]->CR0 = CR0;
		max31856_temp_sensors[i]->CR1 = CR1;

		//set CS idle
		spi_stop(&max31856_temp_sensors[i]->spid);

		max31856_temp_sensors[i]->temperature = 0;
	}
}

extern void max31856_temp_sensors_worker() {
	static unsigned int i;

	if (i >= max31856_temp_sensors_size) {
		i = 0;
		max31856_temp_sensor_cycle_complete = 1;
	}

	if (!MAX31856_process(max31856_temp_sensors[i]))
		i++;
}

void max31856_temp_sensors_millis_sync() {
	static unsigned int delay;

	if (delay) {
		delay--;
	} else {
		delay = TEMP_SENSOR_CYCLE_MS;
		schedule_LTC_read();
	}
}

static void schedule_LTC_read() {
	for (int i = 0; i < max31856_temp_sensors_size; ++i) {
		max31856_temp_sensors[i]->tasks.reg.LTC |= MAX31856_TASK_READ;
	}
}
