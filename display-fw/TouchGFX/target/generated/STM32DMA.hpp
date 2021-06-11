/**
  ******************************************************************************
  * File Name          : STM32DMA.hpp
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

#ifndef STM32F4DMA_HPP
#define STM32F4DMA_HPP

#include <touchgfx/hal/DMA.hpp>

/**
 * @class STM32F4DMA
 *
 * @brief This class specializes DMA_Interface for the STM32F4 processors.
 *
 * @sa touchgfx::DMA_Interface
 */
class STM32F4DMA : public touchgfx::DMA_Interface
{
    /**
     * @typedef touchgfx::DMA_Interface Base
     *
     * @brief Defines an alias representing the base.
     *
     Defines an alias representing the base.
     */
    typedef touchgfx::DMA_Interface Base;
public:
    /**
     * @fn STM32F4DMA::STM32F4DMA();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    STM32F4DMA();

    /**
     * @fn STM32F4DMA::~STM32F4DMA();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~STM32F4DMA();

    /**
     * @fn touchgfx::BlitOperations STM32F4DMA::getBlitCaps();
     *
     * @brief Gets the blit capabilities.
     *
     *        Gets the blit capabilities.
     *
     *        This DMA supports a range of blit caps: BLIT_OP_COPY, BLIT_OP_COPY_ARGB8888,
     *        BLIT_OP_COPY_ARGB8888_WITH_ALPHA, BLIT_OP_COPY_A4, BLIT_OP_COPY_A8.
     *
     *
     * @return Currently supported blitcaps.
     */
    virtual touchgfx::BlitOperations getBlitCaps();

    /**
     * @fn void STM32F4DMA::initialize();
     *
     * @brief Perform hardware specific initialization.
     *
     *        Perform hardware specific initialization.
     */
    virtual void initialize();

    /**
     * @fn void STM32F4DMA::signalDMAInterrupt()
     *
     * @brief Raises a DMA interrupt signal.
     *
     *        Raises a DMA interrupt signal.
     */
    virtual void signalDMAInterrupt()
    {
        executeCompleted();
    }

protected:
    /**
     * @fn virtual void STM32F4DMA::setupDataCopy(const touchgfx::BlitOp& blitOp);
     *
     * @brief Configures the DMA for copying data to the frame buffer.
     *
     *        Configures the DMA for copying data to the frame buffer.
     *
     * @param blitOp Details on the copy to perform.
     */
    virtual void setupDataCopy(const touchgfx::BlitOp& blitOp);

    /**
     * @fn virtual void STM32F4DMA::setupDataFill(const touchgfx::BlitOp& blitOp);
     *
     * @brief Configures the DMA for "filling" the frame-buffer with a single color.
     *
     *        Configures the DMA for "filling" the frame-buffer with a single color.
     *
     * @param blitOp Details on the "fill" to perform.
     */
    virtual void setupDataFill(const touchgfx::BlitOp& blitOp);

private:
    touchgfx::LockFreeDMA_Queue dma_queue;
    touchgfx::BlitOp            queue_storage[96];
};

#endif // STM32F4DMA_HPP

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
