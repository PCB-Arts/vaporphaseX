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
#include "MAX31856.h"
#include "peripherals/spi.h"

#include <stm32f3xx.h>

//=================================
// definitions

//buffer size (address + payload bytes)
#define MAX31856_MGR_BUFFER_SIZE 9

//number of strikes before reporting a sensor fault
#define MAX31856_STRIKES_TO_FAULT 3

#define MAX31856_RC_IDLE 0
#define MAX31856_RC_SPI_LOCKED 1
#define MAX31856_RC_SPI_BYSY 2
#define MAX31856_RC_EXECTUE 3


enum {
	MAX31856_IDLE,
	MAX31856_XFER
} static state = MAX31856_IDLE;

static int execute_task(struct MAX31856_t* dev);
static void finalize_task();

static void resync_device();
static int strike();

static int pack_buffer();
static void pack_LTC();
static void pack_CR();
static void pack_FTH();
static void pack_CJT();

static void unpack_buffer();
static void unpack_LTC();
static void unpack_CR();
static void unpack_FTH();
static void unpack_CJT();


static __IO uint8_t buffer[MAX31856_MGR_BUFFER_SIZE];
static struct MAX31856_t* device;


int MAX31856_process(struct MAX31856_t* dev) {
	if(!dev)
		return 0;

	int rc = MAX31856_RC_IDLE;

	if (spi_ready(&dev->spid)) {
		if (state == MAX31856_IDLE) {
			rc = execute_task(dev);
		} else {
			finalize_task();
		}
	} else {
		rc = MAX31856_RC_SPI_BYSY;
	}

	return rc;
}

int MAX31856_has_fault(struct MAX31856_t* dev) {
	//check ptr and strike count
	if (!dev || dev->strikes >= MAX31856_STRIKES_TO_FAULT) {
		//fault
		return 1;
	} else {
		//no fault
		return 0;
	}
}

void MAX31856_reset() {
	state = MAX31856_IDLE;
}

static int execute_task(struct MAX31856_t* dev) {
	int rc;

	if (dev->tasks.code != 0 && spi_lock(&dev->spid)) {
		device = dev;

		//fill buffer
		unsigned int len = pack_buffer();

		spi_set8(&dev->spid);
		spi_start(&dev->spid);
		spi_xfer8(&dev->spid, buffer, buffer, len);

		state = MAX31856_XFER;

		rc = MAX31856_RC_EXECTUE;
	} else {
		//1 = SPI locked
		//0 = no tasks pending
		rc = (dev->tasks.code != 0);
	}

	return rc;
}

static void finalize_task() {
	spi_stop(&device->spid);
	spi_unlock(&device->spid);

	unpack_buffer();

	//reset strikes if there are no further pending tasks
	if (device->tasks.code == 0)
		device->strikes = 0;

	state = MAX31856_IDLE;
}

static int strike() {
	spi_stop(&device->spid);

	//sync registers, write config and get temperature reading
	resync_device();

	if (MAX31856_has_fault(device)){
		//fault
		return 1;
	} else {
		//increment strikes but no fault... yet
		device->strikes++;
		return 0;
	}
}

static void resync_device() {
	//sync registers, write config and get temperature reading
	device->tasks.reg.CR |= MAX31856_TASK_WRITE;
	device->tasks.reg.FTH |= MAX31856_TASK_WRITE;
	device->tasks.reg.LTC |= MAX31856_TASK_READ;
}

int pack_buffer() {
	//buffer length
	int len = 0;

	if (device->tasks.reg.CR != 0) {
		pack_CR();
		len = 3;

	} else if (device->tasks.reg.FTH != 0) {
		pack_FTH();
		len = 9;

	} else if (device->tasks.reg.CJT != 0) {
		pack_CJT();
		len = 3;

	} else if (device->tasks.reg.LTC != 0) {
		pack_LTC();
		len = 4;
	}

	return len;
}

static void pack_LTC() {
	//LTC is read only
	buffer[0] = MAX31856_ADR_LTC | MAX31856_REG_READ;
}

static void pack_CR() {
	if (device->tasks.reg.CR & MAX31856_TASK_WRITE) {
		buffer[0] = MAX31856_ADR_CR0 | MAX31856_REG_WRITE;

		//write data to buffer
		buffer[1] = device->CR0;
		buffer[2] = device->CR1;
	} else {
		buffer[0] = MAX31856_ADR_CR0 | MAX31856_REG_READ;
	}
}

static void pack_FTH() {
	if (device->tasks.reg.FTH & MAX31856_TASK_WRITE) {
		buffer[0] = MAX31856_ADR_MASK | MAX31856_REG_WRITE;

		//write data to buffer
		buffer[1] = device->MASK;
		buffer[2] = (uint8_t)device->CJHF;
		buffer[3] = (uint8_t)device->CJLF;
		buffer[4] = (uint8_t)(device->LTHFT >> 8);
		buffer[5] = (uint8_t)(device->LTHFT & 0xFF);
		buffer[6] = (uint8_t)(device->LTLFT >> 8);
		buffer[7] = (uint8_t)(device->LTLFT & 0xFF);
		buffer[8] = (uint8_t)device->CJTO;
	} else {
		buffer[0] = MAX31856_ADR_MASK | MAX31856_REG_READ;
	}
}

static void pack_CJT() {
	if (device->tasks.reg.CJT & MAX31856_TASK_WRITE) {
		buffer[0] = MAX31856_ADR_CJT | MAX31856_REG_WRITE;

		//write data to buffer
		buffer[1] = (uint8_t)(device->CJT >> 8);
		buffer[2] = (uint8_t)(device->CJT & 0xFF);
	} else {
		buffer[0] = MAX31856_ADR_CJT | MAX31856_REG_READ;
	}
}

void unpack_buffer() {
	if (device->tasks.reg.CR != 0) {
		unpack_CR();

	} else if (device->tasks.reg.FTH != 0) {
		unpack_FTH();

	} else if (device->tasks.reg.CJT != 0) {
		unpack_CJT();

	} else if (device->tasks.reg.LTC != 0) {
		unpack_LTC();
	}
}

static void unpack_LTC() {
	//extract data from RX array
	const int32_t raw = (int32_t)
		((uint32_t)buffer[1] << 16 |
		(uint32_t)buffer[2] << 8 |
		(uint32_t)buffer[3]);

	//shift by 5 (24 bit - 5 bit = 19 bit)
	device->LTC = raw >> 5;

	//calculate temperature
	device->temperature = device->LTC * MAX31856_RESOLUTION;
	device->itemperature = device->temperature;

	//reset pending flag
	device->tasks.reg.LTC = 0;
}

static void unpack_CR() {
	if (device->tasks.reg.CR & MAX31856_TASK_WRITE) {
		//reset pending flag
		CLEAR_BIT(device->tasks.reg.CR, MAX31856_TASK_WRITE);
	} else {
		//read data from buffer
		device->CR0 = buffer[1];
		device->CR1 = buffer[2];

		//reset pending flag
		CLEAR_BIT(device->tasks.reg.CR, MAX31856_TASK_READ);
	}
}

static void unpack_FTH() {
	if (device->tasks.reg.FTH & MAX31856_TASK_WRITE) {
		//reset pending flag
		CLEAR_BIT(device->tasks.reg.FTH, MAX31856_TASK_WRITE);
	} else {
		//read data from buffer
		device->MASK = buffer[1];
		device->CJHF = (int8_t)buffer[2];
		device->CJLF = (int8_t)buffer[3];

		device->LTHFT = (int16_t)
			((int16_t)buffer[4] << 8 |
			(int16_t)buffer[5]);

		device->LTLFT = (int16_t)
			((int16_t)buffer[6] << 8 |
			(int16_t)buffer[7]);

		device->CJTO = (int8_t)buffer[8];

		//reset pending flag
		CLEAR_BIT(device->tasks.reg.FTH, MAX31856_TASK_READ);
	}
}

static void unpack_CJT() {
	if (device->tasks.reg.CJT & MAX31856_TASK_WRITE) {
		//reset pending flag
		CLEAR_BIT(device->tasks.reg.CJT, MAX31856_TASK_WRITE);
	} else {
		int16_t raw = (int16_t)
				((int16_t)buffer[1] << 8 |
				(int16_t)buffer[2]);

		//write data to buffer
		device->CJT = raw >> 2;

		//reset pending flag
		CLEAR_BIT(device->tasks.reg.CJT, MAX31856_TASK_READ);
	}
}
