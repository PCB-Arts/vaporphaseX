/**
  ******************************************************************************
  * File Name          : TouchGFXHAL.cpp
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
#include <TouchGFXHAL.hpp>

/* USER CODE BEGIN TouchGFXHAL.cpp */

#include "stm32f4xx_hal.h"
#include <touchgfx/hal/OSWrappers.hpp>
#include "main.h"

/* USER CODE BEGIN Includes */
#include <touchgfx/hal/GPIO.hpp>
#include "../Components/otm8009a/otm8009a.h"
#include <CortexMMCUInstrumentation.hpp>
#include <KeySampler.hpp>
#include "FreeRTOS.h"
#include "task.h"
/* USER CODE END Includes */

/* USER CODE BEGIN private defines */
/**
  * @brief  LCD Display OTM8009A ID
  */
#define LCD_OTM8009A_ID  ((uint32_t) 0)
/* USER CODE END private defines */

/* USER CODE BEGIN private variables */
volatile bool displayRefreshing = false;
volatile bool refreshRequested = true;
static int updateRegion = 0;
static uint16_t* currFbBase = 0;
/* USER CODE END private variables */

/* USER CODE BEGIN private functions */

/* USER CODE END private functions */

/* USER CODE BEGIN extern C prototypes */
extern "C" {
    extern DSI_HandleTypeDef hdsi;
    extern LTDC_HandleTypeDef hltdc;

    uint8_t pCols[4][4] =
    {
        {0x00, 0x00, 0x00, 0xC7}, /*   0 -> 199 */
        {0x00, 0xC8, 0x01, 0x8F}, /* 200 -> 399 */
        {0x01, 0x90, 0x02, 0x57}, /* 400 -> 599 */
        {0x02, 0x58, 0x03, 0x1F}, /* 600 -> 799 */
    };

    uint8_t pColLeft[] = { 0x00, 0x00, 0x01, 0xbf }; /*   0 -> 447 */
    uint8_t pColRight[] = { 0x01, 0xc0, 0x03, 0x1F }; /* 448 -> 799 */

    uint8_t pPage[] = { 0x00, 0x00, 0x01, 0xDF }; /*   0 -> 479 */
    uint8_t pScanCol[] = { 0x02, 0x15 };             /* Scan @ 533 */

    /* Request tear interrupt at specific scanline. Implemented in BoardConfiguration.cpp */
    void LCD_ReqTear();

    /* Configures display to update left half of the screen. Implemented in BoardConfiguration.cpp  - 24bpp mode*/
    void LCD_SetUpdateRegionLeft();

    /* Configures display to update right half of the screen. Implemented in BoardConfiguration.cpp - 24bpp mode*/
    void LCD_SetUpdateRegionRight();
}
/* USER CODE END extern C prototypes */

using namespace touchgfx;

/* USER CODE BEGIN private class objects */
static CortexMMCUInstrumentation mcuInstr;
static KeySampler btnctrl;
/* USER CODE END private class objects */

TouchGFXHAL::TouchGFXHAL(touchgfx::DMA_Interface& dma, touchgfx::LCD& display, touchgfx::TouchController& tc, uint16_t width, uint16_t height)
/* USER CODE BEGIN TouchGFXHAL Constructor */
    : TouchGFXGeneratedHAL(dma,
                           display,
                           tc,
                           width + 32, /* Align to match 832 pixel for optimal DSI transfer */
                           height)
/* USER CODE END TouchGFXHAL Constructor */
{
/* USER CODE BEGIN TouchGFXHAL Constructor Code */

/* USER CODE END TouchGFXHAL Constructor Code */
}

void TouchGFXHAL::initialize()
{
/* USER CODE BEGIN initialize step 1 */
    GPIO::init();
/* USER CODE END initialize step 1 */

    // Calling parent implementation of initialize().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.
    // Please note, HAL::initialize() must be called to initialize the framework.

    TouchGFXGeneratedHAL::initialize();

/* USER CODE BEGIN initialize step 2 */
    lockDMAToFrontPorch(false);

    mcuInstr.init();
    setMCUInstrumentation(&mcuInstr);
    enableMCULoadCalculation(true);

    setButtonController(&btnctrl);
/* USER CODE END initialize step 2 */
}

void TouchGFXHAL::taskEntry()
{
/* USER CODE BEGIN taskEntry step 1 */

/* USER CODE END taskEntry step 1 */

    enableLCDControllerInterrupt();
    enableInterrupts();

/* USER CODE BEGIN taskEntry step 2 */

/* USER CODE END taskEntry step 2 */

    OSWrappers::waitForVSync();
    backPorchExited();

/* USER CODE BEGIN taskEntry step 3 */
    /* Enable the LCD, Send Display on DCS command to display */
    HAL_DSI_ShortWrite(&hdsi, LCD_OTM8009A_ID, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPON, 0x00);
/* USER CODE END taskEntry step 3 */

    for (;;)
    {
        OSWrappers::waitForVSync();
        backPorchExited();
    }
}

/**
 * Gets the frame buffer address used by the TFT controller.
 *
 * @return The address of the frame buffer currently being displayed on the TFT.
 */
uint16_t* TouchGFXHAL::getTFTFrameBuffer() const
{
    // Calling parent implementation of getTFTFrameBuffer().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

/* USER CODE BEGIN getTFTFrameBuffer */
    return currFbBase;
/* USER CODE END getTFTFrameBuffer */
}

/**
 * Sets the frame buffer address used by the TFT controller.
 *
 * @param [in] address New frame buffer address.
 */
void TouchGFXHAL::setTFTFrameBuffer(uint16_t* address)
{
    // Calling parent implementation of setTFTFrameBuffer(uint16_t* address).
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

/* USER CODE BEGIN setTFTFrameBuffer */
    TouchGFXGeneratedHAL::setTFTFrameBuffer(address);
/* USER CODE END setTFTFrameBuffer */
}

/**
 * This function is called whenever the framework has performed a partial draw.
 *
 * @param rect The area of the screen that has been drawn, expressed in absolute coordinates.
 *
 * @see flushFrameBuffer().
 */
void TouchGFXHAL::flushFrameBuffer(const touchgfx::Rect& rect)
{
    // Calling parent implementation of flushFrameBuffer(const touchgfx::Rect& rect).
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.
    // Please note, HAL::flushFrameBuffer(const touchgfx::Rect& rect) must
    // be called to notify the touchgfx framework that flush has been performed.

/* USER CODE BEGIN flushFrameBuffer step 1 */
    TouchGFXGeneratedHAL::flushFrameBuffer(rect);
/* USER CODE END flushFrameBuffer step 1 */
}

/**
 * Configures the interrupts relevant for TouchGFX. This primarily entails setting
 * the interrupt priorities for the DMA and LCD interrupts.
 */
void TouchGFXHAL::configureInterrupts()
{
    // Calling parent implementation of configureInterrupts().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

/* USER CODE BEGIN configureInterrupts */
    TouchGFXGeneratedHAL::configureInterrupts();
/* USER CODE END configureInterrupts */
}

/**
 * Used for enabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::enableInterrupts()
{
    // Calling parent implementation of enableInterrupts().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

/* USER CODE BEGIN enableInterrupts */
    TouchGFXGeneratedHAL::enableInterrupts();
/* USER CODE END enableInterrupts */
}

/**
 * Used for disabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::disableInterrupts()
{
    // Calling parent implementation of disableInterrupts().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

/* USER CODE BEGIN disableInterrupts */
    TouchGFXGeneratedHAL::disableInterrupts();
/* USER CODE END disableInterrupts */
}

/**
 * Configure the LCD controller to fire interrupts at VSYNC. Called automatically
 * once TouchGFX initialization has completed.
 */
void TouchGFXHAL::enableLCDControllerInterrupt()
{
    // Calling parent implementation of enableLCDControllerInterrupt().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

/* USER CODE BEGIN enableLCDControllerInterrupt */
    LCD_ReqTear();
    TouchGFXGeneratedHAL::enableLCDControllerInterrupt();
/* USER CODE END enableLCDControllerInterrupt */
}

/* USER CODE BEGIN virtual overloaded methods */
void TouchGFXHAL::setFrameBufferStartAddresses(void* frameBuffer, void* doubleBuffer, void* animationStorage)
{
    currFbBase = (uint16_t*)frameBuffer;
    HAL::setFrameBufferStartAddresses(frameBuffer, doubleBuffer, animationStorage);
}

bool TouchGFXHAL::beginFrame()
{
    refreshRequested = false;
    return HAL::beginFrame();
}

void TouchGFXHAL::endFrame()
{
    HAL::endFrame();
    if (frameBufferUpdatedThisFrame)
    {
        refreshRequested = true;
    }
}
/* USER CODE END virtual overloaded methods */

/* USER CODE BEGIN extern C functions */
extern "C" {

    /**************************** LINK OTM8009A (Display driver) ******************/
    /**
     * @brief  DCS or Generic short/long write command
     * @param  NbParams: Number of parameters. It indicates the write command mode:
     *                 If inferior to 2, a long write command is performed else short.
     * @param  pParams: Pointer to parameter values table.
     * @retval HAL status
     */
    void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t* pParams)
    {
        if (NbrParams <= 1)
        {
            HAL_DSI_ShortWrite(&hdsi, LCD_OTM8009A_ID, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]);
        }
        else
        {
            HAL_DSI_LongWrite(&hdsi,  LCD_OTM8009A_ID, DSI_DCS_LONG_PKT_WRITE, NbrParams, pParams[NbrParams], pParams);
        }
    }

    /**
     * Request TE at scanline.
     */
    void LCD_ReqTear(void)
    {
        static uint8_t ScanLineParams[2];

        uint16_t scanline = 533;
        ScanLineParams[0] = scanline >> 8;
        ScanLineParams[1] = scanline & 0x00FF;

        HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, 0x44, ScanLineParams);
        // set_tear_on
        HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x35, 0x00);
    }

    void LCD_SetUpdateRegionLeft()
    {
        HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
    }

    void LCD_SetUpdateRegionRight()
    {
        HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColRight);
    }

    void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef* hdsi)
    {
        GPIO::set(GPIO::VSYNC_FREQ);
        HAL::getInstance()->vSync();
        OSWrappers::signalVSync();

        // In single buffering, only require that the system waits for display update to be finished if we
        // actually intend to update the display in this frame.
        HAL::getInstance()->lockDMAToFrontPorch(refreshRequested);

        if (refreshRequested && !displayRefreshing)
        {
            // We have an update pending.
            if (HAL::getInstance())
            {
                // Swap frame buffers immediately instead of waiting for the task to be scheduled in.
                // Note: task will also swap when it wakes up, but that operation is guarded and will not have
                // any effect if already swapped.
                HAL::getInstance()->swapFrameBuffers();
            }

            // Update region 0 = first area of display first half for 24bpp
            updateRegion = 0;

            //Set update region
            LCD_SetUpdateRegionLeft();

            HAL_DSI_Refresh(hdsi);
            displayRefreshing = true;
        }
        else
        {
            GPIO::clear(GPIO::VSYNC_FREQ);
        }
    }

    void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef* hdsi)
    {
        if (updateRegion == 0)
        {
            // If we transferred the left half, also transfer right half.

            DSI->WCR &= ~(DSI_WCR_DSIEN);

            LTDC_Layer1->CFBAR = ((uint32_t)currFbBase) + 448 * 3;
            uint16_t ADJUSTED_WIDTH = 384; //64-byte aligned width
            uint16_t REAL_WIDTH = 384 - 32; //we only actually have this amount of pixels on display
            LTDC->AWCR = ((ADJUSTED_WIDTH + 2) << 16) | 0x1E2; //adj
            LTDC->TWCR = ((REAL_WIDTH + 2 + 1 - 1) << 16) | 0x1E3;
            LTDC_Layer1->WHPCR = ((REAL_WIDTH + 2) << 16) | 3;
            LTDC_Layer1->CFBLR = ((832 * 3) << 16) | ((REAL_WIDTH) * 3 + 3);
            LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
            LCD_SetUpdateRegionRight(); //Set display column to 448-799
            DSI->WCR |= DSI_WCR_DSIEN;
            updateRegion = 1;

            HAL_DSI_Refresh(hdsi);
        }
        else
        {
            // Otherwise we are done refreshing.

            DSI->WCR &= ~(DSI_WCR_DSIEN);
            LTDC_Layer1->CFBAR = (uint32_t)currFbBase;
            uint16_t WIDTH = 448;
            LTDC->AWCR = ((WIDTH + 2) << 16) | 0x1E2;
            LTDC->TWCR = ((WIDTH + 2 + 1) << 16) | 0x1E3;
            LTDC_Layer1->WHPCR = ((WIDTH + 2) << 16) | 3;
            LTDC_Layer1->CFBLR = (((832 * 3) << 16) | ((WIDTH * 3) + 3));
            LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
            LCD_SetUpdateRegionLeft(); //Set display column to 0-447
            DSI->WCR |= DSI_WCR_DSIEN;
            GPIO::clear(GPIO::VSYNC_FREQ);

            displayRefreshing = false;
            if (HAL::getInstance())
            {
                // Signal to the framework that display update has finished.
                HAL::getInstance()->frontPorchEntered();
            }
        }
    }

    portBASE_TYPE IdleTaskHook(void* p)
    {
        if ((int)p) //idle task sched out
        {
            touchgfx::HAL::getInstance()->setMCUActive(true);
        }
        else //idle task sched in
        {
            touchgfx::HAL::getInstance()->setMCUActive(false);
        }
        return pdTRUE;
    }
}
/* USER CODE END extern C functions */

/* USER CODE END TouchGFXHAL.cpp */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
