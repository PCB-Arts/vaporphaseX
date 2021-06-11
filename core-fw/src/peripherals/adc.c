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

#include "conf.h"
#include "adc.h"
#include "assert.h"

#if !MOCK_CORE

#define ADC_MAX_NUMBER_OF_CONVERSIONS 16

static uint32_t adc_values_buffer[ADC_MAX_NUMBER_OF_CONVERSIONS];

// 12 bit resolution
static uint32_t adc_resolution_max_value = 0xfff;

void adc_init(ADC_HandleTypeDef* hadc) {
	assert(hadc->Init.NbrOfConversion <= ADC_MAX_NUMBER_OF_CONVERSIONS);
	assert(hadc->Init.DMAContinuousRequests == ENABLE);
	assert(hadc->Init.ScanConvMode == ENABLE);
	assert(hadc->Init.ContinuousConvMode == ENABLE);

	// TODO: support other resolutions?
	assert(hadc->Init.Resolution == ADC_RESOLUTION_12B);

	assert(hadc->DMA_Handle->Init.Mode == DMA_CIRCULAR);
	assert(hadc->DMA_Handle->Init.MemDataAlignment == DMA_MDATAALIGN_WORD);

	HAL_ADC_Start_DMA(hadc, adc_values_buffer, hadc->Init.NbrOfConversion);

	// allow first read to fill buffer
	HAL_Delay(1);
}

uint32_t adc_get_value(const AdcSignalHandle* handle) {
	return adc_values_buffer[handle->sample_index];
}

uint32_t adc_resolution_get_max_value(const AdcSignalHandle* handle) {
	return adc_resolution_max_value;
}

#endif
