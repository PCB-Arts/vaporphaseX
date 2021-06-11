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

#include "buffered_uart/tx_buffer.h"

#include <cmsis_compiler.h>

#include <string.h>


int TxBuffer_CheckHandle(TxBuffer_Handle* handle) {
	if (handle == NULL) {
		return -TX_BUFFER_INCORRECT_ARGUMENT;
	}

	if (handle->buffer == NULL) {
		return -TX_BUFFER_INCORRECT_ARGUMENT;
	}

	if (handle->buffer_size == 0) {
		return -TX_BUFFER_INCORRECT_ARGUMENT;
	}

	return 0;
}

int TxBuffer_Init(TxBuffer_Handle* handle) {
	int rc = TxBuffer_CheckHandle(handle);
	if (rc != 0) {
		return rc;
	}

	handle->sent_pointer = 0;
	handle->ready_pointer = 0;

	return 0;
}

// returns end pointer
int TxBuffer_Copy(TxBuffer_Handle* handle, uint32_t dest_pointer, const uint8_t* data, uint32_t size)
{
	uint8_t wrap_around = handle->buffer_size - dest_pointer < size;

	if(wrap_around) {
		uint32_t part1_size = handle->buffer_size - dest_pointer;
		uint32_t part2_size = size - part1_size;
		memcpy(handle->buffer + handle->ready_pointer, data, part1_size);
		memcpy(handle->buffer, data + part1_size, part2_size);

		return part2_size;
	}

	memcpy(handle->buffer + handle->ready_pointer, data, size);
	return dest_pointer + size != handle->buffer_size ? dest_pointer + size : 0;
}


int TxBuffer_Append_unsafe(TxBuffer_Handle* handle, const uint8_t* data, uint32_t size) {
	uint32_t space_left = handle->sent_pointer <= handle->ready_pointer ?
			handle->buffer_size - (handle->ready_pointer - handle->sent_pointer) :
			handle->sent_pointer - handle->ready_pointer;

	if (space_left < size) {
		return -TX_BUFFER_NO_SPACE_LEFT;
	}

	handle->ready_pointer = TxBuffer_Copy(handle, handle->ready_pointer, data, size);

	return 0;
}

// See post: https://stm32f4-discovery.net/2015/06/how-to-properly-enabledisable-interrupts-in-arm-cortex-m/ on how the interrupt is disabled.

int TxBuffer_Append(TxBuffer_Handle* handle, const uint8_t* data, uint32_t size) {
	int rc = TxBuffer_CheckHandle(handle);
	if (rc != 0) {
		return rc;
	}

	uint32_t primask = __get_PRIMASK();
	__disable_irq();

	rc = TxBuffer_Append_unsafe(handle, data, size);

	/* Enable interrupts back only if they were enabled before we disable it here in this function */
	if (!primask) {
		__enable_irq();
	}

	return rc;
}

TxBuffer_DataReference TxBuffer_NextData_unsafe(TxBuffer_Handle* handle) {
	TxBuffer_DataReference reference;

	reference.buffer = &handle->buffer[handle->sent_pointer];

	if(handle->ready_pointer >= handle->sent_pointer) {
		reference.size = handle->ready_pointer - handle->sent_pointer;
	} else {
		// we return the buffer until the end and then return the wrapped around part on the next call
		reference.size = handle->buffer_size - handle->sent_pointer;
	}

	return reference;
}

TxBuffer_DataReference TxBuffer_NextData(TxBuffer_Handle* handle) {
	uint32_t primask = __get_PRIMASK();
	__disable_irq();

	TxBuffer_DataReference reference = TxBuffer_NextData_unsafe(handle);

	/* Enable interrupts back only if they were enabled before we disable it here in this function */
	if (!primask) {
		__enable_irq();
	}

	return reference;
}

int TxBuffer_ClearData_unsafe(TxBuffer_Handle *handle, TxBuffer_DataReference data_reference) {
	handle->sent_pointer = handle->sent_pointer + data_reference.size;

	if (handle->sent_pointer >= handle->buffer_size) {
		handle->sent_pointer = 0;
	}

	return 0;
}

int TxBuffer_ClearData(TxBuffer_Handle *handle, TxBuffer_DataReference data_reference) {
	int rc = TxBuffer_CheckHandle(handle);
	if (rc != 0) {
		return rc;
	}

	uint32_t primask = __get_PRIMASK();
	__disable_irq();

	rc = TxBuffer_ClearData_unsafe(handle, data_reference);

	/* Enable interrupts back only if they were enabled before we disable it here in this function */
	if (!primask) {
		__enable_irq();
	}

	return rc;
}

int TxBuffer_ClearAll(TxBuffer_Handle *handle) {
	int rc = TxBuffer_CheckHandle(handle);
	if (rc != 0) {
		return rc;
	}

	uint32_t primask = __get_PRIMASK();
	__disable_irq();

	handle->sent_pointer = 0;
	handle->ready_pointer = 0;

	/* Enable interrupts back only if they were enabled before we disable it here in this function */
	if (!primask) {
		__enable_irq();
	}

	return rc;
}

