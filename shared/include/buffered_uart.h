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

#ifndef __BUFFERED_UART_H__
#define __BUFFERED_UART_H__

// global.h should include the project specific HAL
#include "global.h"

void BufferedUart_Init(UART_HandleTypeDef* h);
int BufferedUart_Tx_Append(uint8_t* b, uint32_t size);
void BufferedUart_Rx_ProcessReceivedData(void (*process_callback)(uint8_t));

#endif
