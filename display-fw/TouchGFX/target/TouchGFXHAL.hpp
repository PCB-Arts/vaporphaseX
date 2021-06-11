/**
  ******************************************************************************
  * File Name          : TouchGFXHAL.hpp
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
#ifndef TouchGFXHAL_HPP
#define TouchGFXHAL_HPP

/* USER CODE BEGIN TouchGFXHAL.hpp */

#include <TouchGFXGeneratedHAL.hpp>

/* USER CODE BEGIN user includes */

/* USER CODE END user includes */

/**
 * @class TouchGFXHAL
 *
 * @brief HAL implementation for TouchGFX.
 *
 * @sa HAL
 */
class TouchGFXHAL : public TouchGFXGeneratedHAL
{
public:
    /**
     * @fn TouchGFXHAL::TouchGFXHAL(touchgfx::DMA_Interface& dma, touchgfx::LCD& display, touchgfx::TouchController& tc, uint16_t width, uint16_t height) : TouchGFXGeneratedHAL(dma, display, tc, width, height)
     *
     * @brief Constructor.
     *
     *        Constructor. Initializes members.
     *
     * @param [in,out] dma     Reference to DMA interface.
     * @param [in,out] display Reference to LCD interface.
     * @param [in,out] tc      Reference to Touch Controller driver.
     * @param width            Width of the display.
     * @param height           Height of the display.
     */
    TouchGFXHAL(touchgfx::DMA_Interface& dma, touchgfx::LCD& display, touchgfx::TouchController& tc, uint16_t width, uint16_t height);

    /**
     * @fn void TouchGFXHAL::initialize();
     *
     * @brief This function is responsible for initializing the entire framework.
     *
     *        This function is responsible for initializing the entire framework.
     */
    void initialize();

    /**
     * @fn virtual void TouchGFXHAL::disableInterrupts();
     *
     * @brief Disables the DMA and LCD interrupts.
     *
     *        Disables the DMA and LCD interrupts.
     */
    virtual void disableInterrupts();

    /**
     * @fn virtual void TouchGFXHAL::enableInterrupts();
     *
     * @brief Enables the DMA and LCD interrupts.
     *
     *        Enables the DMA and LCD interrupts.
     */
    virtual void enableInterrupts();

    /**
     * @fn virtual void TouchGFXHAL::configureInterrupts();
     *
     * @brief Sets the DMA and LCD interrupt priorities.
     *
     *        Sets the DMA and LCD interrupt priorities.
     */
    virtual void configureInterrupts();

    /**
     * @fn virtual void TouchGFXHAL::enableLCDControllerInterrupt();
     *
     * @brief Configure the LCD controller to fire interrupts at VSYNC.
     *
     *        Configure the LCD controller to fire interrupts at VSYNC. Called automatically
     *        once TouchGFX initialization has completed.
     */
    virtual void enableLCDControllerInterrupt();

    /**
     * @fn virtual void TouchGFXHAL::flushFrameBuffer();
     *
     * @brief This function is called whenever the framework has performed a complete draw.
     *
     * This specialization is only in place to keep compilers happy. Base impl. will call the
     * Rect version.
     * @see HAL::flushFrameBuffer
     */
    virtual void flushFrameBuffer()
    {
        TouchGFXGeneratedHAL::flushFrameBuffer();
    }

    /**
     * @fn virtual void TouchGFXHAL::flushFrameBuffer(const Rect& rect);
     *
     * @brief This function is called whenever the framework has performed a partial draw.
     *
     *        This function is called whenever the framework has performed a partial draw.
     *        On the STM32F7, make sure to clean and invalidate the data cache. This is to
     *        ensure that LTDC sees correct data when transferring to the display.
     *
     * @param rect The area of the screen that has been drawn, expressed in absolute coordinates.
     *
     * @see flushFrameBuffer().
     */
    virtual void flushFrameBuffer(const touchgfx::Rect& rect);

    /**
     * @fn virtual void TouchGFXHAL::taskEntry();
     *
     * @brief Main event loop.
     *
     *        Main event loop. Will wait for VSYNC signal, and then process next frame. Call
     *        this function from your GUI task.
     *
     * @note This function never returns!
     */
    virtual void taskEntry();

/* USER CODE BEGIN virtual overloaded public methods */

/* USER CODE END virtual overloaded public methods */

protected:
    /**
     * @fn virtual uint16_t* TouchGFXHAL::getTFTFrameBuffer() const;
     *
     * @brief Gets the frame buffer address used by the TFT controller.
     *
     *        Gets the frame buffer address used by the TFT controller.
     *
     * @return The address of the frame buffer currently being displayed on the TFT.
     */
    virtual uint16_t* getTFTFrameBuffer() const;

    /**
     * @fn virtual void TouchGFXHAL::setTFTFrameBuffer(uint16_t* adr);
     *
     * @brief Sets the frame buffer address used by the TFT controller.
     *
     *        Sets the frame buffer address used by the TFT controller.
     *
     * @param [in,out] adr New frame buffer address.
     */
    virtual void setTFTFrameBuffer(uint16_t* adr);

/* USER CODE BEGIN virtual overloaded protected methods */
    /**
     * @fn virtual void TouchGFXHAL::setFrameBufferStartAddresses(void* frameBuffer, void* doubleBuffer, void* animationStorage)
     *
     * @brief Sets frame buffer start addresses.
     *
     *        Sets individual frame buffer start addresses.
     *
     * @param [in] frameBuffer      Buffer for frame buffer data, must be non-null.
     * @param [in] doubleBuffer     If non-null, buffer for double buffer data. If null double buffering is disabled.
     * @param [in] animationStorage If non-null, the animation storage. If null animation storage is disabled.
     */
    virtual void setFrameBufferStartAddresses(void* frameBuffer, void* doubleBuffer, void* animationStorage);

    /**
     * This function is overridden to detect whether there are any frame buffer changes in this frame.
     * @return The value of the base implementation.
     */
    virtual bool beginFrame();

    /**
     * This function is overridden to detect whether there are any frame buffer changes in this frame.
     * @note Will also call the base implementation.
     */
    virtual void endFrame();
/* USER CODE END virtual overloaded protected methods */
};

/* USER CODE END TouchGFXHAL.hpp */

#endif // TouchGFXHAL_HPP

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
