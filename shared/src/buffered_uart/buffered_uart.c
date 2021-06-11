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

UART_HandleTypeDef* buff_huart;

void BufferedUart_Tx_Init();
void BufferedUart_Rx_Init();

void BufferedUart_Rx_ErrorCallback(UART_HandleTypeDef *huart);
void BufferedUart_Tx_ErrorCallback(UART_HandleTypeDef *huart);

void BufferedUart_ErrorCallback(UART_HandleTypeDef *huart) {
	BufferedUart_Rx_ErrorCallback(huart);
	BufferedUart_Tx_ErrorCallback(huart);
}

void BufferedUart_Init(UART_HandleTypeDef* h) {
	buff_huart = h;

	h->ErrorCallback = BufferedUart_ErrorCallback;

	BufferedUart_Tx_Init();
	BufferedUart_Rx_Init();
}
