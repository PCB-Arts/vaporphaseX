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

#include "buffered_uart.h"

#include "buffered_uart/tx_buffer.h"

static uint8_t tx_buf[4096];
static TxBuffer_Handle tx_buffer_handle;
static volatile uint8_t sending = 0;
static volatile TxBuffer_DataReference sendingReference;

extern UART_HandleTypeDef* buff_huart;

// send some delimiters on init or error
static uint8_t tx_reset_data[5] = { 0, 0, 0, 0, 0 };

static void BufferedUart_Tx_SendNextAvailableData() {
	uint8_t start_sending = 0;
	TxBuffer_DataReference temp_reference;


	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	// interrupts disabled

	if(!sending) {
		temp_reference = TxBuffer_NextData(&tx_buffer_handle);
		if(temp_reference.size > 0) {
			sending = 1;
			start_sending = 1;
		}
	}

	// restore interrupts
	if (!primask) {
		__enable_irq();
	}


	if (start_sending) {
		sendingReference = temp_reference;
		HAL_UART_Transmit_DMA(buff_huart, sendingReference.buffer, sendingReference.size);
	}
}

static void BufferedUart_Tx_CompleteCallback(UART_HandleTypeDef *huart) {
	TxBuffer_ClearData(&tx_buffer_handle, sendingReference);

	// set sending to 0 to be able to start next transmission
	sending = 0;

	BufferedUart_Tx_SendNextAvailableData();
}

// clear all data from buffer and start again
void BufferedUart_Tx_ErrorCallback(UART_HandleTypeDef *huart) {
	TxBuffer_ClearAll(&tx_buffer_handle);

	sending = 0;

	BufferedUart_Tx_Append(tx_reset_data, sizeof(tx_reset_data));

	BufferedUart_Tx_SendNextAvailableData();
}

void BufferedUart_Tx_Init() {
	// TODO: add check if DMA is present

	tx_buffer_handle.buffer = tx_buf;
	tx_buffer_handle.buffer_size = sizeof(tx_buf);
	TxBuffer_Init(&tx_buffer_handle);

	buff_huart->TxCpltCallback = BufferedUart_Tx_CompleteCallback;

	BufferedUart_Tx_Append(tx_reset_data, sizeof(tx_reset_data));
}

int BufferedUart_Tx_Append(uint8_t* b, uint32_t size) {
	int rc = TxBuffer_Append(&tx_buffer_handle, b, size);
	if(rc != 0) {
		return rc;
	}

	BufferedUart_Tx_SendNextAvailableData();

	return 0;
}


