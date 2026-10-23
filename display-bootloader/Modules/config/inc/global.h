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

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "stm32u5xx_hal.h"

#define ARRAY_LEN(x)                                                           \
  ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

/* Offset of the user firmware vector table from the start of the user flash.
 * The Cortex-M33 needs VTOR aligned to the table size rounded up to a power of
 * two: the 155 vectors of the U5A9 are 620 bytes, so 1 KiB. */
#define BOOTLOADER_ISR_OFFSET 0x400

#endif /*__GLOBAL_H__*/
