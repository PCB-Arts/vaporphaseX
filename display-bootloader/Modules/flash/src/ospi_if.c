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

#include "ospi_if.h"

#include "mx25lm51245g.h"
#include "octospi.h"

#define OSPI_IF_MODE MX25LM51245G_SPI_MODE
#define OSPI_IF_RATE MX25LM51245G_STR_TRANSFER
#define OSPI_IF_ADDRESS_SIZE MX25LM51245G_4BYTES_SIZE

/* The memory may still be in (D)OPI mode, e.g. after a reset while the
 * application was running, so the reset is sent in every mode. Same sequence
 * as the application's ospi_memory_reset(). */
static int OSPI_If_ResetMemory(void)
{
  static const struct
  {
    MX25LM51245G_Interface_t mode;
    MX25LM51245G_Transfer_t rate;
  } modes[] = {
      {MX25LM51245G_SPI_MODE, MX25LM51245G_STR_TRANSFER},
      {MX25LM51245G_OPI_MODE, MX25LM51245G_STR_TRANSFER},
      {MX25LM51245G_OPI_MODE, MX25LM51245G_DTR_TRANSFER},
  };

  for (size_t i = 0; i < sizeof(modes) / sizeof(modes[0]); i++)
  {
    if (MX25LM51245G_ResetEnable(&hospi1, modes[i].mode, modes[i].rate) !=
            MX25LM51245G_OK ||
        MX25LM51245G_ResetMemory(&hospi1, modes[i].mode, modes[i].rate) !=
            MX25LM51245G_OK)
    {
      return 1;
    }
  }

  /* Wait in case the reset occurred during an erase operation */
  HAL_Delay(MX25LM51245G_RESET_MAX_TIME);

  return 0;
}

int OSPI_If_Init(void)
{
  if (OSPI_If_ResetMemory() != 0)
  {
    return 1;
  }

  return MX25LM51245G_AutoPollingMemReady(&hospi1, OSPI_IF_MODE,
                                          OSPI_IF_RATE) != MX25LM51245G_OK;
}

int OSPI_If_Erase(uint32_t Address, uint32_t Length)
{
  uint32_t end = Address + Length;

  if (end > MX25LM51245G_FLASH_SIZE || end < Address)
  {
    return 1;
  }

  for (uint32_t sector = Address & ~(MX25LM51245G_SECTOR_64K - 1); sector < end;
       sector += MX25LM51245G_SECTOR_64K)
  {
    if (MX25LM51245G_WriteEnable(&hospi1, OSPI_IF_MODE, OSPI_IF_RATE) !=
            MX25LM51245G_OK ||
        MX25LM51245G_BlockErase(&hospi1, OSPI_IF_MODE, OSPI_IF_RATE,
                                OSPI_IF_ADDRESS_SIZE, sector,
                                MX25LM51245G_ERASE_64K) != MX25LM51245G_OK ||
        MX25LM51245G_AutoPollingMemReady(&hospi1, OSPI_IF_MODE, OSPI_IF_RATE) !=
            MX25LM51245G_OK)
    {
      return 1;
    }
  }

  return 0;
}

int OSPI_If_Write(uint32_t Address, const uint8_t *Data, size_t Length)
{
  if (Address + Length > MX25LM51245G_FLASH_SIZE || Address + Length < Address)
  {
    return 1;
  }

  while (Length > 0)
  {
    /* A page program must not cross a 256 byte page boundary */
    size_t chunk = MX25LM51245G_PAGE_SIZE - (Address % MX25LM51245G_PAGE_SIZE);
    if (chunk > Length)
    {
      chunk = Length;
    }

    if (MX25LM51245G_WriteEnable(&hospi1, OSPI_IF_MODE, OSPI_IF_RATE) !=
            MX25LM51245G_OK ||
        MX25LM51245G_PageProgram(&hospi1, OSPI_IF_MODE, OSPI_IF_ADDRESS_SIZE,
                                 (uint8_t *)Data, Address,
                                 chunk) != MX25LM51245G_OK ||
        MX25LM51245G_AutoPollingMemReady(&hospi1, OSPI_IF_MODE, OSPI_IF_RATE) !=
            MX25LM51245G_OK)
    {
      return 1;
    }

    Address += chunk;
    Data += chunk;
    Length -= chunk;
  }

  return 0;
}
