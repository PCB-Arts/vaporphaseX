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

/*
 * Based on STMicroelectronics example code from the STM32Cube
 * USB_Host/FWupgrade_Standalone example (flash_if.c):
 *
 * Copyright (c) 2017 STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "flash_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FLASH_EraseInitTypeDef FLASH_EraseInitStruct;
static uint32_t PageError = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Unlocks the Flash to enable the flash control register access.
 * @param  None
 * @retval None
 */
void FLASH_If_FlashUnlock(void)
{
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
}

void FLASH_IF_FlashLock(void) { HAL_FLASH_Lock(); }

/**
 * @brief  Erases the user Flash area from Address up to the end of the flash.
 *         The rest of bank 1 is erased page by page, bank 2 is mass erased.
 * @param  Address: Start address for erasing data, must be page aligned
 * @retval 0: Erase sectors done with success
 *         1: Erase error
 */
uint32_t FLASH_If_EraseSectors(uint32_t Address)
{
  const uint32_t bank2_start = FLASH_BASE + FLASH_BANK_SIZE;

  if (Address < APPLICATION_ADDRESS || Address >= bank2_start ||
      (Address - FLASH_BASE) % FLASH_PAGE_SIZE != 0)
  {
    return (1);
  }

  /* Erase the rest of the first bank */
  FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  FLASH_EraseInitStruct.Banks = FLASH_BANK_1;
  FLASH_EraseInitStruct.Page = (Address - FLASH_BASE) / FLASH_PAGE_SIZE;
  FLASH_EraseInitStruct.NbPages = FLASH_PAGE_NB - FLASH_EraseInitStruct.Page;
  if (HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &PageError) != HAL_OK)
    return (1);

  /* Erase the second bank */
  FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_MASSERASE;
  FLASH_EraseInitStruct.Banks = FLASH_BANK_2;
  if (HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &PageError) != HAL_OK)
    return (1);

  return (0);
}

/**
 * @brief  Writes a data buffer in flash, one quad-word at a time.
 * @note   Address must be quad-word aligned. A trailing partial quad-word is
 *         padded with 0xFF, so it can only be the last write to that
 *         quad-word (the flash ECC does not allow programming it twice).
 * @param  Address: Start address for writing data buffer
 * @param  Data: Pointer on data buffer
 * @param  Length: Number of bytes to write
 * @retval 0: Data successfully written to Flash memory
 *         1: Error occurred while writing data in Flash memory
 */
uint32_t FLASH_If_Write(uint32_t Address, const uint8_t *Data, size_t Length)
{
  uint32_t quad_word[FLASH_IF_PROGRAM_SIZE / sizeof(uint32_t)];

  if (Address < APPLICATION_ADDRESS || Address % FLASH_IF_PROGRAM_SIZE != 0 ||
      Length > APPLICATION_ADDRESS + USER_FLASH_SIZE - Address)
  {
    return (1);
  }

  for (size_t i = 0; i < Length; i += FLASH_IF_PROGRAM_SIZE)
  {
    size_t chunk =
        Length - i < FLASH_IF_PROGRAM_SIZE ? Length - i : FLASH_IF_PROGRAM_SIZE;

    memset(quad_word, 0xFF, sizeof(quad_word));
    memcpy(quad_word, &Data[i], chunk);

    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, Address + i,
                          (uint32_t)quad_word) != HAL_OK)
      return (1);
  }

  return (0);
}
