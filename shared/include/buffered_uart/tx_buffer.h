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

#ifndef __TX_BUFFER_H
#define __TX_BUFFER_H

#include <stdint.h>

// interrupt safe sliding windows transmission buffer
// limitations:
// - single consumer
// - on wrap around returns the data on two parts

typedef struct {
  // must be set by user
  uint8_t *buffer;

  uint32_t buffer_size;

  // are set by init
  // pointers are relative to buffer
  volatile uint32_t sent_pointer;
  volatile uint32_t ready_pointer;
} TxBuffer_Handle;

typedef struct {
  uint8_t *buffer;

  int32_t size;
} TxBuffer_DataReference;

enum TxBuffer_ErrorCodes {
  TX_BUFFER_NO_SPACE_LEFT = 1,
  TX_BUFFER_INCORRECT_ARGUMENT = 2,
};

int TxBuffer_Init(TxBuffer_Handle *handle);
int TxBuffer_Append(TxBuffer_Handle *handle, const uint8_t *data, uint32_t size);
TxBuffer_DataReference TxBuffer_NextData(TxBuffer_Handle *handle);
int TxBuffer_ClearData(TxBuffer_Handle *bufferUart_handle,
                       TxBuffer_DataReference data_reference);
int TxBuffer_ClearAll(TxBuffer_Handle *handle);

#endif
