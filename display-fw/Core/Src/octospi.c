/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    octospi.c
  * @brief   This file provides code for the configuration
  *          of the OCTOSPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "octospi.h"

/* USER CODE BEGIN 0 */
#include "mx25lm51245g.h"


static uint8_t ospi_memory_reset            (OSPI_HandleTypeDef *hospi);
static int32_t OSPI_NOR_EnterSOPIMode		(OSPI_HandleTypeDef *hospi);
int32_t OSPI_DLYB_Enable				(OSPI_HandleTypeDef *hospi);
int32_t OSPI_NOR_EnableMemoryMappedMode(OSPI_HandleTypeDef *hospi);

/* USER CODE END 0 */

OSPI_HandleTypeDef hospi1;

/* OCTOSPI1 init function */
void MX_OCTOSPI1_Init(void)
{

  /* USER CODE BEGIN OCTOSPI1_Init 0 */
	MX25LM51245G_Info_t pInfo;
  /* USER CODE END OCTOSPI1_Init 0 */

  OSPIM_CfgTypeDef sOspiManagerCfg = {0};
  HAL_OSPI_DLYB_CfgTypeDef HAL_OSPI_DLYB_Cfg_Struct = {0};

  /* USER CODE BEGIN OCTOSPI1_Init 1 */
  /* Get Flash informations of one memory */
  (void)MX25LM51245G_GetFlashInfo(&pInfo);
  /* USER CODE END OCTOSPI1_Init 1 */
  hospi1.Instance = OCTOSPI1;
  hospi1.Init.FifoThreshold = 4;
  hospi1.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;
  hospi1.Init.MemoryType = HAL_OSPI_MEMTYPE_MACRONIX;
  hospi1.Init.DeviceSize = 32;
  hospi1.Init.ChipSelectHighTime = 2;
  hospi1.Init.FreeRunningClock = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi1.Init.ClockMode = HAL_OSPI_CLOCK_MODE_0;
  hospi1.Init.WrapSize = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi1.Init.ClockPrescaler = 2;
  hospi1.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  hospi1.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  hospi1.Init.ChipSelectBoundary = 0;
  hospi1.Init.DelayBlockBypass = HAL_OSPI_DELAY_BLOCK_BYPASSED;
  hospi1.Init.MaxTran = 0;
  hospi1.Init.Refresh = 0;
  if (HAL_OSPI_Init(&hospi1) != HAL_OK)
  {
    Error_Handler();
  }
  sOspiManagerCfg.ClkPort = 1;
  sOspiManagerCfg.DQSPort = 1;
  sOspiManagerCfg.NCSPort = 1;
  sOspiManagerCfg.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;
  sOspiManagerCfg.IOHighPort = HAL_OSPIM_IOPORT_1_HIGH;
  if (HAL_OSPIM_Config(&hospi1, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_OSPI_DLYB_Cfg_Struct.Units = 0;
  HAL_OSPI_DLYB_Cfg_Struct.PhaseSel = 0;
  if (HAL_OSPI_DLYB_SetConfig(&hospi1, &HAL_OSPI_DLYB_Cfg_Struct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OCTOSPI1_Init 2 */
  /* OSPI memory reset */
  if (ospi_memory_reset(&hospi1) != 0)
  {
	  Error_Handler();
  }

  if (MX25LM51245G_AutoPollingMemReady(&hospi1,MX25LM51245G_SPI_MODE, MX25LM51245G_STR_TRANSFER) != MX25LM51245G_OK)
  {
	  Error_Handler();
  }

  /* Enable octal mode */
  if (OSPI_NOR_EnterSOPIMode(&hospi1) != 0)
  {
	  Error_Handler();
  }


  if (OSPI_NOR_EnableMemoryMappedMode(&hospi1) != 0)
  {
	  Error_Handler();
  }
  /* USER CODE END OCTOSPI1_Init 2 */

}

void HAL_OSPI_MspInit(OSPI_HandleTypeDef* ospiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(ospiHandle->Instance==OCTOSPI1)
  {
  /* USER CODE BEGIN OCTOSPI1_MspInit 0 */

  /* USER CODE END OCTOSPI1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_OSPI;
    PeriphClkInit.OspiClockSelection = RCC_OSPICLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* OCTOSPI1 clock enable */
    __HAL_RCC_OSPIM_CLK_ENABLE();
    __HAL_RCC_OSPI1_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**OCTOSPI1 GPIO Configuration
    PF10     ------> OCTOSPIM_P1_CLK
    PF7     ------> OCTOSPIM_P1_IO2
    PF9     ------> OCTOSPIM_P1_IO1
    PC1     ------> OCTOSPIM_P1_IO4
    PF6     ------> OCTOSPIM_P1_IO3
    PF8     ------> OCTOSPIM_P1_IO0
    PA2     ------> OCTOSPIM_P1_NCS
    PC3     ------> OCTOSPIM_P1_IO6
    PC2     ------> OCTOSPIM_P1_IO5
    PC0     ------> OCTOSPIM_P1_IO7
    PA1     ------> OCTOSPIM_P1_DQS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF3_OCTOSPI1;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPI1;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPI1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF3_OCTOSPI1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN OCTOSPI1_MspInit 1 */

  /* USER CODE END OCTOSPI1_MspInit 1 */
  }
}

void HAL_OSPI_MspDeInit(OSPI_HandleTypeDef* ospiHandle)
{

  if(ospiHandle->Instance==OCTOSPI1)
  {
  /* USER CODE BEGIN OCTOSPI1_MspDeInit 0 */

  /* USER CODE END OCTOSPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_OSPIM_CLK_DISABLE();
    __HAL_RCC_OSPI1_CLK_DISABLE();

    /**OCTOSPI1 GPIO Configuration
    PF10     ------> OCTOSPIM_P1_CLK
    PF7     ------> OCTOSPIM_P1_IO2
    PF9     ------> OCTOSPIM_P1_IO1
    PC1     ------> OCTOSPIM_P1_IO4
    PF6     ------> OCTOSPIM_P1_IO3
    PF8     ------> OCTOSPIM_P1_IO0
    PA2     ------> OCTOSPIM_P1_NCS
    PC3     ------> OCTOSPIM_P1_IO6
    PC2     ------> OCTOSPIM_P1_IO5
    PC0     ------> OCTOSPIM_P1_IO7
    PA1     ------> OCTOSPIM_P1_DQS
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_6
                          |GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_0);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_1);

  /* USER CODE BEGIN OCTOSPI1_MspDeInit 1 */

  /* USER CODE END OCTOSPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

int32_t OSPI_DLYB_Enable(OSPI_HandleTypeDef *hospi)
{
  LL_DLYB_CfgTypeDef dlyb_cfg, dlyb_cfg_test;
  int32_t ret = 0;
  uint32_t div_value = 4;

  /* Delay block configuration ------------------------------------------------ */
  if (HAL_OSPI_DLYB_GetClockPeriod(hospi, &dlyb_cfg) != HAL_OK)
  {
    ret = 1;
  }

  /* PhaseSel is divided by 4 (emperic value)*/
  dlyb_cfg.PhaseSel /= div_value;

  /* save the present configuration for check*/
  dlyb_cfg_test = dlyb_cfg;

  /*set delay block configuration*/
  if (HAL_OSPI_DLYB_SetConfig(hospi, &dlyb_cfg) != HAL_OK)
  {
    ret = 1;
  }

  /*check the set value*/
  if (HAL_OSPI_DLYB_GetConfig(hospi, &dlyb_cfg) != HAL_OK)
  {
    ret = 1;
  }

  if ((dlyb_cfg.PhaseSel != dlyb_cfg_test.PhaseSel) || (dlyb_cfg.Units != dlyb_cfg_test.Units))
  {
    ret = 1;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Reset the OSPI memory.
  * @param  hospi: OSPI handle pointer
  * @retval O on success 1 on Failure.
  */
static uint8_t ospi_memory_reset(OSPI_HandleTypeDef *hospi)
{
	 int32_t ret = 0;

	  /* Enable write operations */
	  if(MX25LM51245G_ResetEnable(hospi, MX25LM51245G_SPI_MODE, MX25LM51245G_STR_TRANSFER) != MX25LM51245G_OK)
	  {
	    ret = 1;
	  }
	  else if(MX25LM51245G_ResetMemory(hospi, MX25LM51245G_SPI_MODE, MX25LM51245G_STR_TRANSFER) != MX25LM51245G_OK)
	  {
	    ret = 1;
	  }
	  else if(MX25LM51245G_ResetEnable(hospi, MX25LM51245G_OPI_MODE, MX25LM51245G_STR_TRANSFER) != MX25LM51245G_OK)
	  {
	    ret = 1;
	  }
	  else if(MX25LM51245G_ResetMemory(hospi, MX25LM51245G_OPI_MODE, MX25LM51245G_STR_TRANSFER) != MX25LM51245G_OK)
	  {
	    ret = 1;
	  }
	  else if(MX25LM51245G_ResetEnable(hospi, MX25LM51245G_OPI_MODE, MX25LM51245G_DTR_TRANSFER) != MX25LM51245G_OK)
	  {
	    ret = 1;
	  }
	  else if(MX25LM51245G_ResetMemory(hospi, MX25LM51245G_OPI_MODE, MX25LM51245G_DTR_TRANSFER) != MX25LM51245G_OK)
	  {
	    ret = 1;
	  }
	  else
	  {


	    /* After SWreset CMD, wait in case SWReset occurred during erase operation */
	    HAL_Delay(MX25LM51245G_RESET_MAX_TIME);
	  }

	  /* Return BSP status */
	  return ret;
}





static int32_t OSPI_NOR_EnterSOPIMode(OSPI_HandleTypeDef *hospi)
{
  int32_t ret = 0;
  uint8_t reg[2];

  /* Enable write operations */
  if (MX25LM51245G_WriteEnable(hospi, MX25LM51245G_SPI_MODE, MX25LM51245G_STR_TRANSFER) != MX25LM51245G_OK)
  {
    ret = 1;
  }
  /* Write Configuration register 2 (with new dummy cycles) */
  else if (MX25LM51245G_WriteCfg2Register(hospi, MX25LM51245G_SPI_MODE, MX25LM51245G_STR_TRANSFER, MX25LM51245G_CR2_REG3_ADDR, MX25LM51245G_CR2_DC_6_CYCLES) != MX25LM51245G_OK)
  {
    ret = 1;
  }
  /* Enable write operations */
  else if (MX25LM51245G_WriteEnable(hospi, MX25LM51245G_SPI_MODE, MX25LM51245G_STR_TRANSFER) != MX25LM51245G_OK)
  {
    ret = 1;
  }
  /* Write Configuration register 2 (with Octal I/O SPI protocol) */
  else if (MX25LM51245G_WriteCfg2Register(hospi, MX25LM51245G_SPI_MODE, MX25LM51245G_STR_TRANSFER, MX25LM51245G_CR2_REG1_ADDR, MX25LM51245G_CR2_SOPI) != MX25LM51245G_OK)
  {
    ret = 1;
  }
  else
  {
    /* Wait that the configuration is effective and check that memory is ready */
    HAL_Delay(MX25LM51245G_WRITE_REG_MAX_TIME);

    /* Reconfigure the memory type of the peripheral */
  //  hospi->Init.MemoryType            = HAL_OSPI_MEMTYPE_MACRONIX;
  //  hospi->Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  //  if (HAL_OSPI_Init(hospi) != HAL_OK)
  //  {
  //    ret = 1;
  //  }
    /* Check Flash busy ? */
     if (MX25LM51245G_AutoPollingMemReady(hospi, MX25LM51245G_OPI_MODE, MX25LM51245G_STR_TRANSFER) != MX25LM51245G_OK)
    {
      ret = 1;
    }
    /* Check the configuration has been correctly done */
    else if (MX25LM51245G_ReadCfg2Register(hospi, MX25LM51245G_OPI_MODE, MX25LM51245G_STR_TRANSFER, MX25LM51245G_CR2_REG1_ADDR, reg) != MX25LM51245G_OK)
    {
      ret = 1;
    }
    else
    {
      if (reg[0] != MX25LM51245G_CR2_SOPI)
      {
        ret = 1;
      }
    }
  }

  /* Return BSP status */
  return ret;
}


/**
  * @brief  Configure the OSPI in memory-mapped mode
  * @param  Instance  OSPI instance
  * @retval BSP status
  */
int32_t OSPI_NOR_EnableMemoryMappedMode(OSPI_HandleTypeDef *hospi)
{
  int32_t ret = 0;


    if(MX25LM51245G_EnableMemoryMappedModeSTR(hospi, MX25LM51245G_OPI_MODE, MX25LM51245G_4BYTES_SIZE) != MX25LM51245G_OK)
    {
      ret = 1;
    }




  /* Return BSP status */
  return ret;
}
/* USER CODE END 1 */
