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

#ifndef PERIPHERALS_ADC_H_
#define PERIPHERALS_ADC_H_

#include "global.h"

/* module for reading and buffering the adc values from fixed ADC */

typedef struct __AdcSignalHandle {
	uint32_t sample_index;
} AdcSignalHandle;

/*
 * The ADC is expected to have dma continous request enabled
 * and the dma to be configured in circular mode.
 * The adc must be configured to have the same number of samples
 * as the size of the buffer.
 * The ADC interrupt should not be active, as this would choke the CPU.
 */
void adc_init(ADC_HandleTypeDef* hadc);
uint32_t adc_resolution_get_max_value(const AdcSignalHandle* handle);
uint32_t adc_get_value(const AdcSignalHandle* handle);

#endif /* PERIPHERALS_ADC_H_ */
