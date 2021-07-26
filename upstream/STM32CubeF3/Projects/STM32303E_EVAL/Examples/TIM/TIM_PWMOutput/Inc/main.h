/**
  ******************************************************************************
  * @file    TIM/TIM_PWMOutput/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32303e_eval.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated
   resources */
/* Definition for TIMx clock resources */
#define TIMx                           TIM3
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM3_CLK_ENABLE()

/* Definition for TIMx Channel Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define TIMx_GPIO_PORT_CHANNEL1        GPIOC
#define TIMx_GPIO_PORT_CHANNEL2        GPIOC
#define TIMx_GPIO_PORT_CHANNEL3        GPIOC
#define TIMx_GPIO_PORT_CHANNEL4        GPIOC
#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_6
#define TIMx_GPIO_PIN_CHANNEL2         GPIO_PIN_7
#define TIMx_GPIO_PIN_CHANNEL3         GPIO_PIN_8
#define TIMx_GPIO_PIN_CHANNEL4         GPIO_PIN_9
#define TIMx_GPIO_AF_CHANNEL1          GPIO_AF2_TIM3
#define TIMx_GPIO_AF_CHANNEL2          GPIO_AF2_TIM3
#define TIMx_GPIO_AF_CHANNEL3          GPIO_AF2_TIM3
#define TIMx_GPIO_AF_CHANNEL4          GPIO_AF2_TIM3
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
