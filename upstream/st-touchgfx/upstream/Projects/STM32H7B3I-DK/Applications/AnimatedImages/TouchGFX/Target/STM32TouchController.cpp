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
#include <stm32h7b3i_discovery_ts.h>
#include <TouchGFXHAL.hpp>
#include <cmsis_os.h>

void STM32TouchController::init()
{
    TS_Init_t hTS;

    hTS.Orientation = TS_SWAP_XY;
    hTS.Accuracy = 0;
    hTS.Width = touchgfx::HAL::FRAME_BUFFER_WIDTH;
    hTS.Height = touchgfx::HAL::FRAME_BUFFER_HEIGHT;
    BSP_TS_Init(0, &hTS);
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    TS_State_t TS_State = { 0 };

    /* This should never fail !! */
    if (BSP_TS_GetState(0, &TS_State) != BSP_ERROR_NONE)
    {
        configASSERT(0);
    }

    if (TS_State.TouchDetected)
    {
        x = TS_State.TouchX;
        y = TS_State.TouchY;

        return true;
    }

    return false;
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
