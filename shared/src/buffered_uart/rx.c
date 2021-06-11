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

static uint8_t rx_buf[4096];
static volatile uint32_t rx_buffer_offset = 0; // sum of all complete received buffer bytes, is incremented by buffer received complete callback
static volatile uint32_t rx_bytes_already_read = 0;

extern UART_HandleTypeDef* buff_huart;

static void BufferedUart_Rx_CompleteCallback(UART_HandleTypeDef *huart) {
	rx_buffer_offset += sizeof(rx_buf);
}

void BufferedUart_Rx_ErrorCallback(UART_HandleTypeDef *huart) {
	rx_buffer_offset = 0;
	rx_bytes_already_read = 0;

	HAL_UART_Receive_DMA(buff_huart, rx_buf, sizeof(rx_buf));
}

void BufferedUart_Rx_Init() {
	// TODO: add check if DMA is present

	buff_huart->RxCpltCallback = BufferedUart_Rx_CompleteCallback;

    HAL_UART_Receive_DMA(buff_huart, rx_buf, sizeof(rx_buf));
}

void BufferedUart_Rx_ProcessReceivedData(void (*process_callback)(uint8_t)) {
	// TODO: add overrun detection
	// TODO: improve consumption (through whole stack)

	uint32_t bytes_received = rx_buffer_offset + sizeof(rx_buf) - __HAL_DMA_GET_COUNTER(buff_huart->hdmarx);

	int32_t bytes_to_read = bytes_received - rx_bytes_already_read;

	uint32_t offset = rx_bytes_already_read % sizeof(rx_buf);

	if(bytes_to_read <= 0) {
		return;
	}

	if (offset + bytes_to_read < sizeof(rx_buf)) {
		uint32_t end = offset + bytes_to_read;
		for(int32_t i = offset; i < end; i++) {
			process_callback(rx_buf[i]);
		}
	} else {
		{
			uint32_t end = sizeof(rx_buf);
			for(int32_t i = offset; i < end; i++) {
				process_callback(rx_buf[i]);
			}
		}
		{
			uint32_t end = bytes_received % sizeof(rx_buf);
			for(int32_t i = 0; i < end; i++) {
				process_callback(rx_buf[i]);
			}
		}
	}

	rx_bytes_already_read += bytes_to_read;
}
