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

#ifndef __MX_INIT_H
#define __MX_INIT_H

#include "global.h"
#include "conf.h"

void MX_DMA_Init(void);

#if UART_INTERFACE == 3

void MX_USART3_UART_Init(void);

extern UART_HandleTypeDef huart3;

extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;


#elif UART_INTERFACE == 6

void MX_USART6_UART_Init(void);

extern UART_HandleTypeDef huart6;

extern DMA_HandleTypeDef hdma_usart6_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;

#endif


#endif
