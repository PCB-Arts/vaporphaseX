/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2026 - PCB Arts GmbH
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

#ifndef __OSPI_IF_H
#define __OSPI_IF_H

#include <stddef.h>
#include <stdint.h>

/* Programming interface for the external MX25LM51245G OSPI NOR flash.
 * Addresses are offsets into the flash (0 = 0x90000000 in the application's
 * memory mapped view). The memory is driven in 1-line SPI STR mode. */

/* Resets the memory into SPI mode. Call after MX_OCTOSPI1_Init(). */
int OSPI_If_Init(void);
/* Erases all 64 KiB sectors touching [Address, Address + Length). */
int OSPI_If_Erase(uint32_t Address, uint32_t Length);
int OSPI_If_Write(uint32_t Address, const uint8_t *Data, size_t Length);

#endif /* __OSPI_IF_H */
