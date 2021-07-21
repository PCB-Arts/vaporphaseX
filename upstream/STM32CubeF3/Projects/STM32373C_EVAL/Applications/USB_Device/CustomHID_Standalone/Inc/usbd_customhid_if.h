/**
  ******************************************************************************
  * @file    USB_Device/CustomHID_Standalone/Inc/usbd_customhid_if.h
  * @author  MCD Application Team
  * @brief   Header for usbd_customhid_if.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CUSTOMHID_IF_H
#define __USBD_CUSTOMHID_IF_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_customhid.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LED1_REPORT_ID           0x01
#define LED1_REPORT_COUNT        0x01

#define LED2_REPORT_ID           0x02
#define LED2_REPORT_COUNT        0x01

#define LED3_REPORT_ID           0x03
#define LED3_REPORT_COUNT        0x01

#define LED4_REPORT_ID           0x04
#define LED4_REPORT_COUNT        0x01

#define KEY_REPORT_ID            0x05
#define TAMPER_REPORT_ID         0x06
#define ADC_REPORT_ID            0x07

/* User can use this section to tailor ADCx instance used and associated 
   resources */
/* Definition for ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
     
#define ADCx_FORCE_RESET()              __HAL_RCC_ADC1_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC1_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_1
#define ADCx_CHANNEL_GPIO_PORT          GPIOB 

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_9

/* Definition for ADCx's DMA */
#define ADCx_DMA_CHANNEL                DMA1_Channel1
        
/* Definition for ADCx's NVIC */
#define ADCx_DMA_IRQn                   DMA1_Channel1_IRQn

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops;

#endif /* __USBD_CUSTOMHID_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
