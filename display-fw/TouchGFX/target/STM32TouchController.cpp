/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * This file was created by TouchGFX Generator 4.23.1. This file is only
  * generated once! Delete this file from your project and re-generate code
  * using STM32CubeMX or change this file manually to update it.
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

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include "i2c.h"


volatile uint8_t TouchINT_irq = 0;
int32_t X_touch;
int32_t Y_touch;
void STM32TouchController::init()
{
    /**
     * Initialize touch controller and driver
     *
     */


	    HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_SET);



	    HAL_NVIC_SetPriority(EXTI6_IRQn, 7, 0);
	    HAL_NVIC_EnableIRQ(EXTI6_IRQn);
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    /**
     * By default sampleTouch returns false,
     * return true if a touch has been detected, otherwise false.
     *
     * Coordinates are passed to the caller by reference by x and y.
     *
     * This function is called by the TouchGFX framework.
     * By default sampleTouch is called every tick, this can be adjusted by HAL::setTouchSampleRate(int8_t);
     *
     */
    uint8_t rx_buf[16] = {0};

    if (TouchINT_irq)
   // if (! HAL_GPIO_ReadPin( CTP_INT_GPIO_Port, CTP_INT_Pin ) )
    {

        TouchINT_irq = 0;

        if (HAL_OK !=  HAL_I2C_Master_Receive(&hi2c1, (0x41 << 1), rx_buf, 16, 100))
        {
        	return false;
        }
        if (rx_buf[0] == 0x48)
        {
            X_touch = 0;
            X_touch = rx_buf[3] & 0x0F;
            X_touch <<= 8;
            X_touch |= rx_buf[2];
            Y_touch = 0;
            Y_touch = rx_buf[5] & 0x0F;
            Y_touch <<= 8;
            Y_touch |= rx_buf[4];
            *(int32_t*)&x = X_touch;
            *(int32_t*)&y = Y_touch;
        return true;

        }
        else
        {
            return false;
        }




    }


    return false;
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{

    if (GPIO_Pin == CTP_INT_Pin)
    {
    	TouchINT_irq = 1;

    }
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
