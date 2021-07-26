/**
  ******************************************************************************
  * @file    ADC/ADC_DualMode_18MSamplesPerSec/Inc/main.h
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
/* User can use this section to tailor ADCx instance under use and associated
   resources */

/* ## Definition of ADC related resources ################################### */
/* Definition of ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC12_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC12_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC12_RELEASE_RESET()

/* Definition of ADCx channels */
#define ADCx_CHANNELa                   ADC_CHANNEL_7

/* Definition of ADCx channels pins */
#define ADCx_CHANNELa_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define ADCx_CHANNELa_GPIO_PORT         GPIOC
#define ADCx_CHANNELa_PIN               GPIO_PIN_1


/* Definition of ADCx DMA resources */
#define ADCx_DMA_CLK_ENABLE()            __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx_DMA_STREAM                  DMA1_Channel1

#define ADCx_DMA_IRQn                    DMA1_Channel1_IRQn
#define ADCx_DMA_IRQHandler              DMA1_Channel1_IRQHandler

/* Definition of ADCx NVIC resources */
#define ADCx_IRQn                        ADC1_2_IRQn
#define ADCx_IRQHandler                  ADC1_2_IRQHandler

/* Definition of ADCy clock resources */
#define ADCy                             ADC2
#define ADCy_CLK_ENABLE()                __HAL_RCC_ADC12_CLK_ENABLE()
#define ADCy_CHANNEL_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()

#define ADCy_FORCE_RESET()               __HAL_RCC_ADC12_FORCE_RESET()
#define ADCy_RELEASE_RESET()             __HAL_RCC_ADC12_RELEASE_RESET()




/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
