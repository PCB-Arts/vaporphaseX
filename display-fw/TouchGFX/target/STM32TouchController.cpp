/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include "stm32469i_discovery_ts.h"

static bool bsp_ts_initialized = false;

void STM32TouchController::init()
{
    /**
     * Initialize touch controller and driver
     *
     */
    if(BSP_TS_Init(800, 480) == TS_OK)
    {
        bsp_ts_initialized = true;
    }

    assert(bsp_ts_initialized);
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
    if(bsp_ts_initialized)
    {
        TS_StateTypeDef state;
        BSP_TS_GetState(&state);
        if (state.touchDetected)
        {
            x = state.touchX[0];
            y = state.touchY[0];
            return true;
        }
    }
    return false;
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
