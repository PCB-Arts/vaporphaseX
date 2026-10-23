/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : TouchGFXGPIO.cpp
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

#include <touchgfx/hal/GPIO.hpp>

/**
 * GPIO_ID Enum
 * The signals represented by this enum are used by TouchGFX framework to signal internal events.
 *
 * VSYNC_FREQ,  /// Pin is toggled at each VSYNC
 * RENDER_TIME, /// Pin is high when frame rendering begins, low when finished
 * FRAME_RATE,  /// Pin is toggled when the frame buffers are swapped.
 * MCU_ACTIVE   /// Pin is high when framework is utilizing the MCU.
 *
 * Configure GPIO's with the same name as the GPIO_IDs above, as output, in CubeMX to export
 * the signals for performance measuring. See support.touchgfx.com for further details.
 *
 */

/* USER CODE BEGIN TouchGFXGPIO.cpp */
#include "main.h"

using namespace touchgfx;

/*
 * Perform configuration of IO pins.
 */
void GPIO::init()
{

}

/*
 * Sets a pin high.
 */
void GPIO::set(GPIO_ID id)
{
    switch (id)
    {
    case GPIO::VSYNC_FREQ:
#if defined(VSYNC_FREQ_GPIO_Port) && defined(VSYNC_FREQ_Pin)
        HAL_GPIO_WritePin(VSYNC_FREQ_GPIO_Port, VSYNC_FREQ_Pin, GPIO_PIN_SET);
#endif
        break;
    case GPIO::RENDER_TIME:
#if defined(RENDER_TIME_GPIO_Port) && defined(RENDER_TIME_Pin)
        HAL_GPIO_WritePin(RENDER_TIME_GPIO_Port, RENDER_TIME_Pin, GPIO_PIN_SET);
#endif
        break;
    case GPIO::FRAME_RATE:
#if defined(FRAME_RATE_GPIO_Port) && defined(FRAME_RATE_Pin)
        HAL_GPIO_WritePin(FRAME_RATE_GPIO_Port, FRAME_RATE_Pin, GPIO_PIN_SET);
#endif
        break;
    case GPIO::MCU_ACTIVE:
#if defined(MCU_ACTIVE_GPIO_Port) && defined(MCU_ACTIVE_Pin)
        HAL_GPIO_WritePin(MCU_ACTIVE_GPIO_Port, MCU_ACTIVE_Pin, GPIO_PIN_SET);
#endif
        break;
    }
}

/*
 * Sets a pin low.
 */
void GPIO::clear(GPIO_ID id)
{
    switch (id)
    {
    case GPIO::VSYNC_FREQ:
#if defined(VSYNC_FREQ_GPIO_Port) && defined(VSYNC_FREQ_Pin)
        HAL_GPIO_WritePin(VSYNC_FREQ_GPIO_Port, VSYNC_FREQ_Pin, GPIO_PIN_RESET);
#endif
        break;
    case GPIO::RENDER_TIME:
#if defined(RENDER_TIME_GPIO_Port) && defined(RENDER_TIME_Pin)
        HAL_GPIO_WritePin(RENDER_TIME_GPIO_Port, RENDER_TIME_Pin, GPIO_PIN_RESET);
#endif
        break;
    case GPIO::FRAME_RATE:
#if defined(FRAME_RATE_GPIO_Port) && defined(FRAME_RATE_Pin)
        HAL_GPIO_WritePin(FRAME_RATE_GPIO_Port, FRAME_RATE_Pin, GPIO_PIN_RESET);
#endif
        break;
    case GPIO::MCU_ACTIVE:
#if defined(MCU_ACTIVE_GPIO_Port) && defined(MCU_ACTIVE_Pin)
        HAL_GPIO_WritePin(MCU_ACTIVE_GPIO_Port, MCU_ACTIVE_Pin, GPIO_PIN_RESET);
#endif
        break;
    }
}

/*
 * Toggles a pin.
 */
void GPIO::toggle(GPIO_ID id)
{
    switch (id)
    {
    case GPIO::VSYNC_FREQ:
#if defined(VSYNC_FREQ_GPIO_Port) && defined(VSYNC_FREQ_Pin)
        HAL_GPIO_TogglePin(VSYNC_FREQ_GPIO_Port, VSYNC_FREQ_Pin);
#endif
        break;
    case GPIO::RENDER_TIME:
#if defined(RENDER_TIME_GPIO_Port) && defined(RENDER_TIME_Pin)
        HAL_GPIO_TogglePin(RENDER_TIME_GPIO_Port, RENDER_TIME_Pin);
#endif
        break;
    case GPIO::FRAME_RATE:
#if defined(FRAME_RATE_GPIO_Port) && defined(FRAME_RATE_Pin)
        HAL_GPIO_TogglePin(FRAME_RATE_GPIO_Port, FRAME_RATE_Pin);
#endif
        break;
    case GPIO::MCU_ACTIVE:
#if defined(MCU_ACTIVE_GPIO_Port) && defined(MCU_ACTIVE_Pin)
        HAL_GPIO_TogglePin(MCU_ACTIVE_GPIO_Port, MCU_ACTIVE_Pin);
#endif
        break;
    }
}

/*
 * Gets the state of a pin.
 */
bool GPIO::get(GPIO_ID id)
{
    GPIO_PinState bitstatus = GPIO_PIN_RESET;
    switch (id)
    {
    case GPIO::VSYNC_FREQ:
#if defined(VSYNC_FREQ_GPIO_Port) && defined(VSYNC_FREQ_Pin)
        bitstatus = HAL_GPIO_ReadPin(VSYNC_FREQ_GPIO_Port, VSYNC_FREQ_Pin);
#endif
        break;
    case GPIO::RENDER_TIME:
#if defined(RENDER_TIME_GPIO_Port) && defined(RENDER_TIME_Pin)
        bitstatus = HAL_GPIO_ReadPin(RENDER_TIME_GPIO_Port, RENDER_TIME_Pin);
#endif
        break;
    case GPIO::FRAME_RATE:
#if defined(FRAME_RATE_GPIO_Port) && defined(FRAME_RATE_Pin)
        bitstatus = HAL_GPIO_ReadPin(FRAME_RATE_GPIO_Port, FRAME_RATE_Pin);
#endif
        break;
    case GPIO::MCU_ACTIVE:
#if defined(MCU_ACTIVE_GPIO_Port) && defined(MCU_ACTIVE_Pin)
        bitstatus = HAL_GPIO_ReadPin(MCU_ACTIVE_GPIO_Port, MCU_ACTIVE_Pin);
#endif
        break;
    }
    return (bitstatus == GPIO_PIN_SET);
}

/* USER CODE END TouchGFXGPIO.cpp */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
