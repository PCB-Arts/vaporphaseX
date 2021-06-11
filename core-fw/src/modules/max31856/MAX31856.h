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


#ifndef MODULES_MAX31856_H_
#define MODULES_MAX31856_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "MAX31856_def.h"
#include "peripherals/spi.h"

#include <stdint.h>

//=================================
// declarations

#define MAX31856_TASK_READ	0b00000001
#define MAX31856_TASK_WRITE	0b00000010

union MAX31856_tasks_t {
	struct {
		//configuration registers
		uint8_t CR;

		//fault mask & threshold registers
		uint8_t FTH;

		//cold junction temperature registers
		uint8_t CJT;

		//Linearized TC Temperature
		uint8_t LTC;
	} reg;

	uint32_t code;
};

#define MAX31856_TASK_TPL() {{\
		MAX31856_TASK_WRITE,\
		MAX31856_TASK_WRITE,\
		0,\
		MAX31856_TASK_READ}}


struct MAX31856_t {
	//temperature as float (full resolution)
	float temperature;

	//temperature as integer (reduced resolution)
	int32_t itemperature;

	//Linearized TC Temperature
	int32_t LTC;

	//Linearized Temperature High Fault Threshold
	int16_t LTHFT;

	//Linearized Temperature Low Fault Threshold
	int16_t LTLFT;

	//Cold-Junction Temperature Register
	int16_t CJT;

	//Configuration 0 Register
	uint8_t CR0;

	//Configuration 1 Register
	uint8_t CR1;

	//Cold-Junction High Fault Threshold
	int8_t CJHF;

	//Cold-Junction Low Fault Threshold
	int8_t CJLF;

	//Cold-Junction Temperature Offset Register
	int8_t CJTO;

	//Fault Mask Register
	uint8_t MASK;

	//Fault Status Register
	uint8_t SR;

	//number of strikes (faults)
	uint8_t strikes;

	//pending tasks
	union MAX31856_tasks_t tasks;

	//spi device
	struct spi_dev_t spid;
};

#define MAX31856_TPL(spi_dev)\
	{0 ,0, 0,\
	(MAX31856_LTHFTH_DEFAULT << 8 | MAX31856_LTHFTL_DEFAULT),\
	(MAX31856_LTLFTH_DEFAULT << 8 | MAX31856_LTLFTL_DEFAULT),\
	(MAX31856_CJTH_DEFAULT << 8 | MAX31856_CJTL_DEFAULT),\
	MAX31856_CR0_DEFAULT,\
	MAX31856_CR1_DEFAULT,\
	MAX31856_CJHF_DEFAULT,\
	MAX31856_CJLF_DEFAULT,\
	MAX31856_CJTO_DEFAULT,\
	MAX31856_MASK_DEFAULT,\
	0,\
	0,\
	MAX31856_TASK_TPL(),\
	spi_dev}


extern int MAX31856_process(struct MAX31856_t* dev);
extern int MAX31856_has_fault(struct MAX31856_t* dev);
extern void MAX31856_reset();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_MAX31856_H_ */
