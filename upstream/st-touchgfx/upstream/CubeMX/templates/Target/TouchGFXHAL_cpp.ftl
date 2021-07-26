[#ftl]
/**
  ******************************************************************************
  * File Name          : ${name.split("/")?last}
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */
[#list SWIPdatas as data]
#include <TouchGFXHAL.hpp>

/* USER CODE BEGIN ${name?split("/")?last} */

#include "${FamilyName?lower_case}xx.h"

using namespace touchgfx;

[#if data.parameters.tgfx_buffering_strategy == "Partial"]
/* ******************************************************
 * Functions required by Partial Frame Buffer Strategy
 * ******************************************************
 *
 *  int touchgfxDisplayDriverTransmitActive() must return whether or not data is currently being transmitted, over e.g. SPI.
 *  void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h) will be called
 *  when the framework wants to send a block. The user must then transfer the data represented by the arguments.
 *
 *  A user must call touchgfx::startNewTransfer(); once touchgfxDisplayDriverTransmitBlock() has successfully sent a block.
 *  E.g. if using DMA to transfer the block, this could be called in the "Transfer Completed" interrupt handler.
 *
 */
[#if data.parameters.tgfx_partialfb_transmit_strategy == "Disabled"]
#error "A user must call touchgfx::startNewTransfer(); once touchgfxDisplayDriverTransmitBlock() has succesfully sent a block."
[/#if]
#error "A user must implement C-methods touchgfxDisplayDriverTransmitActive() and touchgfxDisplayDriverTransmitBlock() used by the Partial Framebuffer Strategy."
[/#if]

void TouchGFXHAL::initialize()
{
    // Calling parent implementation of initialize().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.
    // Please note, HAL::initialize() must be called to initialize the framework.

    TouchGFXGeneratedHAL::initialize();
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

    return TouchGFXGeneratedHAL::getTFTFrameBuffer();
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

    TouchGFXGeneratedHAL::setTFTFrameBuffer(address);
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

    TouchGFXGeneratedHAL::flushFrameBuffer(rect);
[#if data.parameters.tgfx_mcu_family?? && data.parameters.tgfx_mcu_family == "m7"]

    // If the framebuffer is placed in Write Through cached memory (e.g. SRAM) then we need
    // to flush the Dcache to make sure framebuffer is correct in RAM. That's done
    // using SCB_CleanInvalidateDCache().

    // SCB_CleanInvalidateDCache();
[/#if]
}

bool TouchGFXHAL::blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes)
{
    return TouchGFXGeneratedHAL::blockCopy(dest, src, numBytes);
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

    TouchGFXGeneratedHAL::configureInterrupts();
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

    TouchGFXGeneratedHAL::enableInterrupts();
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

    TouchGFXGeneratedHAL::disableInterrupts();
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

    TouchGFXGeneratedHAL::enableLCDControllerInterrupt();
}

/* USER CODE END ${name?split("/")?last} */
[/#list]

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
