/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "conf.h"

#include "peripherals/adc.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
void main_millis_sync();
extern AdcSignalHandle ntc_adc_signal;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LID_OVERCURRENT_Pin GPIO_PIN_14
#define LID_OVERCURRENT_GPIO_Port GPIOC
#define LID_MOTOR_HALFBRIDGE_B_Pin GPIO_PIN_15
#define LID_MOTOR_HALFBRIDGE_B_GPIO_Port GPIOC
#define NTC_TEMP_Pin GPIO_PIN_0
#define NTC_TEMP_GPIO_Port GPIOC
#define LIFT_DIRECTION_Pin GPIO_PIN_1
#define LIFT_DIRECTION_GPIO_Port GPIOC
#define QC_FAN_ENABLE_Pin GPIO_PIN_2
#define QC_FAN_ENABLE_GPIO_Port GPIOC
#define BOOTLOADER_CHECK_Pin GPIO_PIN_3
#define BOOTLOADER_CHECK_GPIO_Port GPIOC
#define RADIATOR_MONITOR_Pin GPIO_PIN_0
#define RADIATOR_MONITOR_GPIO_Port GPIOA
#define LID_MONITOR_Pin GPIO_PIN_1
#define LID_MONITOR_GPIO_Port GPIOA
#define PUMP_MONITOR_Pin GPIO_PIN_2
#define PUMP_MONITOR_GPIO_Port GPIOA
#define QC_FAN_MONITOR_Pin GPIO_PIN_3
#define QC_FAN_MONITOR_GPIO_Port GPIOA
#define LIFT_MONITOR_Pin GPIO_PIN_4
#define LIFT_MONITOR_GPIO_Port GPIOF
#define LID_ENCODER1_Pin GPIO_PIN_6
#define LID_ENCODER1_GPIO_Port GPIOA
#define LID_ENCODER2_Pin GPIO_PIN_7
#define LID_ENCODER2_GPIO_Port GPIOA
#define LID_END_TOP_Pin GPIO_PIN_10
#define LID_END_TOP_GPIO_Port GPIOB
#define LID_END_TOP_EXTI_IRQn EXTI15_10_IRQn
#define LID_END_BOTTOM_Pin GPIO_PIN_11
#define LID_END_BOTTOM_GPIO_Port GPIOB
#define LID_END_BOTTOM_EXTI_IRQn EXTI15_10_IRQn
#define LIFT_END_TOP_Pin GPIO_PIN_12
#define LIFT_END_TOP_GPIO_Port GPIOB
#define LIFT_END_TOP_EXTI_IRQn EXTI15_10_IRQn
#define LIFT_END_BOTTOM_Pin GPIO_PIN_13
#define LIFT_END_BOTTOM_GPIO_Port GPIOB
#define LIFT_END_BOTTOM_EXTI_IRQn EXTI15_10_IRQn
#define TEMP_SENSOR_2_CS_Pin GPIO_PIN_14
#define TEMP_SENSOR_2_CS_GPIO_Port GPIOB
#define LIFT_STEP_Pin GPIO_PIN_15
#define LIFT_STEP_GPIO_Port GPIOB
#define HEATER_OTP_STATUS_Pin GPIO_PIN_6
#define HEATER_OTP_STATUS_GPIO_Port GPIOC
#define HEATER_OTP_STATUS_EXTI_IRQn EXTI9_5_IRQn
#define COOLANT_OTP_STATUS_Pin GPIO_PIN_7
#define COOLANT_OTP_STATUS_GPIO_Port GPIOC
#define LID_FAN_ENABLE_Pin GPIO_PIN_9
#define LID_FAN_ENABLE_GPIO_Port GPIOC
#define OCP_LATCH_RESET_Pin GPIO_PIN_8
#define OCP_LATCH_RESET_GPIO_Port GPIOA
#define HEATER_ENABLE_Pin GPIO_PIN_9
#define HEATER_ENABLE_GPIO_Port GPIOA
#define THIRD_PARTY_Pin GPIO_PIN_10
#define THIRD_PARTY_GPIO_Port GPIOA
#define COOLANT_LEVEL_SWITCH_Pin GPIO_PIN_11
#define COOLANT_LEVEL_SWITCH_GPIO_Port GPIOA
#define FAN_PWM_Pin GPIO_PIN_12
#define FAN_PWM_GPIO_Port GPIOA
#define LIFT_ENABLE_Pin GPIO_PIN_15
#define LIFT_ENABLE_GPIO_Port GPIOA
#define LID_MOTOR_HALFBRIDGE_A_Pin GPIO_PIN_12
#define LID_MOTOR_HALFBRIDGE_A_GPIO_Port GPIOC
#define LID_LOCK_MONITOR_Pin GPIO_PIN_2
#define LID_LOCK_MONITOR_GPIO_Port GPIOD
#define TEMP_SENSOR_0_CS_Pin GPIO_PIN_6
#define TEMP_SENSOR_0_CS_GPIO_Port GPIOB
#define TEMP_SENSOR_1_CS_Pin GPIO_PIN_7
#define TEMP_SENSOR_1_CS_GPIO_Port GPIOB
#define PUMP_ENABLE_Pin GPIO_PIN_8
#define PUMP_ENABLE_GPIO_Port GPIOB
#define TEMP_SENSOR_3_CS_Pin GPIO_PIN_9
#define TEMP_SENSOR_3_CS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
