/**
  ******************************************************************************
  * @file    USB_Host/FWupgrade_Standalone/Src/flash_if.c
  * @author  MCD Application Team
  * @brief   This file provides all the flash layer functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
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
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "flash_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t FirstSector = 0;
uint32_t NbOfSectors = 0;
uint32_t SectorError = 0;

/* Private function prototypes -----------------------------------------------*/
static uint32_t FLASH_If_GetSectorNumber(uint32_t Address);
static FLASH_EraseInitTypeDef FLASH_EraseInitStruct;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks the Flash to enable the flash control register access.
  * @param  None
  * @retval None
  */
void FLASH_If_FlashUnlock(void)
{
  HAL_FLASH_Unlock();
}

void FLASH_IF_FlashLock(void)
{
	HAL_FLASH_Lock();
}

/**
  * @brief  Gets Flash readout protection status.
  * @param  None
  * @retval ReadOut protection status
  */
FlagStatus FLASH_If_IsReadOutProtectionEnabled(void)
{
	FLASH_OBProgramInitTypeDef FLASH_OBProgramInitStruct = {0};
	HAL_FLASHEx_OBGetConfig(&FLASH_OBProgramInitStruct);

	return FLASH_OBProgramInitStruct.RDPLevel > 0;}

/**
  * @brief  Erases the required FLASH Sectors.
  * @param  Address: Start address for erasing data
  * @retval 0: Erase sectors done with success
  *         1: Erase error
  */
uint32_t FLASH_If_EraseSectors(uint32_t Address)
{
  /* Erase the user Flash area
    (area defined by APPLICATION_ADDRESS and USER_FLASH_LAST_PAGE_ADDRESS) ****/

  if(Address <= (uint32_t) USER_FLASH_LAST_PAGE_ADDRESS)
  {
    /* Get the 1st sector to erase */
    FirstSector = FLASH_If_GetSectorNumber(Address);

    /* Get the number of sector to erase from 1st sector */
    NbOfSectors = FLASH_If_GetSectorNumber(USER_FLASH_LAST_PAGE_ADDRESS) - FirstSector + 1;

    /* Erase the first bank */
    FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    FLASH_EraseInitStruct.Sector = FirstSector;
    FLASH_EraseInitStruct.NbSectors = NbOfSectors;
    FLASH_EraseInitStruct.Banks = FLASH_BANK_1;
    FLASH_EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    if(HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &SectorError) != HAL_OK)
      return (1);

    /* Erase the second bank */
    FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_MASSERASE;
    FLASH_EraseInitStruct.Banks = FLASH_BANK_2;
    FLASH_EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    if(HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &SectorError) != HAL_OK)
      return (1);
  }
  else
  {
    return (1);
  }

  return (0);
}

/**
  * @brief  Writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  Address: Start address for writing data buffer
  * @param  Data: Pointer on data buffer
  * @retval 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  */
uint32_t FLASH_If_Write(uint32_t Address, uint32_t Data)
{
  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and APPLICATION_ADDRESS) ***********/

  if(Address <= (uint32_t) USER_FLASH_LAST_PAGE_ADDRESS)
  {
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Data)!= HAL_OK)
      return (1);
  }
  else
  {
    return (1);
  }

  return (0);
}

/**
  * @brief  Returns the Flash sector Number of the address
  * @param  None
  * @retval The Flash sector Number of the address
  */
static uint32_t FLASH_If_GetSectorNumber(uint32_t Address)
{
  uint32_t sector = 0;

  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;
  }
  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_SECTOR_11;
  }
  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_SECTOR_12;
  }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_SECTOR_13;
  }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_SECTOR_14;
  }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_SECTOR_15;
  }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_SECTOR_16;
  }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_SECTOR_17;
  }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_SECTOR_18;
  }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_SECTOR_19;
  }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_SECTOR_20;
  }
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_SECTOR_21;
  }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_SECTOR_22;
  }
  else
  {
    sector = FLASH_SECTOR_23;
  }
  return sector;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
