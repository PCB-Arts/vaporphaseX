/**
  ******************************************************************************
  * File Name          : TouchGFXGPIO.cpp
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

#include <touchgfx/hal/GPIO.hpp>

/**
 * GPIO_ID Enum, these are used bt TouchGFX framework to signal events.
 *
 * VSYNC_FREQ,  /// Pin is toggled at each VSYNC
 * RENDER_TIME, /// Pin is high when frame rendering begins, low when finished
 * FRAME_RATE,  /// Pin is toggled when the frame buffers are swapped.
 * MCU_ACTIVE   /// Pin is high when framework is utilizing the MCU.
 *
 */

/* USER CODE BEGIN TouchGFXGPIO.cpp */

#include <main.h>

using namespace touchgfx;

/*
 * Perform configuration of IO pins.
 */
void GPIO::init()
{
    // Done by CubeMX in main.c
}

/*
 * Sets a pin high.
 */
void GPIO::set(GPIO_ID id)
{
    switch (id)
    {
    case GPIO::VSYNC_FREQ:
        HAL_GPIO_WritePin(GPIOG, LED3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOG, VSYNC_FREQ_Pin, GPIO_PIN_SET);
        break;
    case GPIO::RENDER_TIME:
        HAL_GPIO_WritePin(GPIOG, LED2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOG, RENDER_TIME_Pin, GPIO_PIN_SET);
        break;
    case GPIO::FRAME_RATE:
        HAL_GPIO_WritePin(GPIOG, FRAME_RATE_Pin, GPIO_PIN_SET);
        break;
    case GPIO::MCU_ACTIVE:
        HAL_GPIO_WritePin(GPIOG, MCU_ACTIVE_Pin, GPIO_PIN_SET);
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
        HAL_GPIO_WritePin(GPIOG, LED3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOG, VSYNC_FREQ_Pin, GPIO_PIN_RESET);
        break;
    case GPIO::RENDER_TIME:
        HAL_GPIO_WritePin(GPIOG, LED2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOG, RENDER_TIME_Pin, GPIO_PIN_RESET);
        break;
    case GPIO::FRAME_RATE:
        HAL_GPIO_WritePin(GPIOG, FRAME_RATE_Pin, GPIO_PIN_RESET);
        break;
    case GPIO::MCU_ACTIVE:
        HAL_GPIO_WritePin(GPIOG, MCU_ACTIVE_Pin, GPIO_PIN_RESET);
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
        HAL_GPIO_TogglePin(GPIOG, LED3_Pin);
        HAL_GPIO_TogglePin(GPIOG, VSYNC_FREQ_Pin);
        break;
    case GPIO::RENDER_TIME:
        HAL_GPIO_TogglePin(GPIOG, LED2_Pin);
        HAL_GPIO_TogglePin(GPIOG, RENDER_TIME_Pin);
        break;
    case GPIO::FRAME_RATE:
        HAL_GPIO_TogglePin(GPIOG, FRAME_RATE_Pin);
        break;
    case GPIO::MCU_ACTIVE:
        HAL_GPIO_TogglePin(GPIOG, MCU_ACTIVE_Pin);
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
        bitstatus = HAL_GPIO_ReadPin(GPIOG, LED3_Pin);
        break;
    case GPIO::RENDER_TIME:
        bitstatus = HAL_GPIO_ReadPin(GPIOG, LED2_Pin);
        break;
    case GPIO::FRAME_RATE:
        bitstatus = HAL_GPIO_ReadPin(GPIOG, FRAME_RATE_Pin);
        break;
    case GPIO::MCU_ACTIVE:
        bitstatus = HAL_GPIO_ReadPin(GPIOG, MCU_ACTIVE_Pin);
        break;
    }
    return (bitstatus == GPIO_PIN_SET);
}

/* USER CODE END TouchGFXGPIO.cpp */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
