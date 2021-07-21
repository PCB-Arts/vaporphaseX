[#ftl]
/**
  ******************************************************************************
  * File Name          : ${name.split("/")?last}
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */
#ifndef TOUCHGFXDMA_HPP
#define TOUCHGFXDMA_HPP

/* USER CODE BEGIN ${name?split("/")?last} */

#include <touchgfx/hal/DMA.hpp>

/**
 * @class TouchGFXDMA
 *
 * @brief This class specializes DMA_Interface for the TouchGFX processors.
 *
 * @sa touchgfx::DMA_Interface
 */
class TouchGFXDMA : public touchgfx::DMA_Interface
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
     * @fn TouchGFXDMA::TouchGFXDMA();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    TouchGFXDMA();

    /**
     * @fn TouchGFXDMA::~TouchGFXDMA();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~TouchGFXDMA();

    /**
     * @fn touchgfx::BlitOperations TouchGFXDMA::getBlitCaps();
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
     * @fn void TouchGFXDMA::initialize();
     *
     * @brief Perform hardware specific initialization.
     *
     *        Perform hardware specific initialization.
     */
    virtual void initialize();

    /**
     * @fn void TouchGFXDMA::signalDMAInterrupt()
     *
     * @brief Raises a DMA interrupt signal.
     *
     *        Raises a DMA interrupt signal.
     */
    virtual void signalDMAInterrupt()
    {
        executeCompleted();
    }

    /**
     * @fn void TouchGFXDMA::start();
     *
     * @brief Signals that DMA transfers can start.
     *
     *        Signals that DMA transfers can start. If any elements are in the queue, start it.
     */
    virtual void start();

protected:
    /**
     * @fn virtual void TouchGFXDMA::setupDataCopy(const touchgfx::BlitOp& blitOp);
     *
     * @brief Configures the DMA for copying data to the frame buffer.
     *
     *        Configures the DMA for copying data to the frame buffer.
     *
     * @param blitOp Details on the copy to perform.
     */
    virtual void setupDataCopy(const touchgfx::BlitOp& blitOp);

    /**
     * @fn virtual void TouchGFXDMA::setupDataFill(const touchgfx::BlitOp& blitOp);
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
    touchgfx::BlitOp            queue_storage[96];  /* Amount of drawing operations which can be queued */
};

/* USER CODE END ${name?split("/")?last} */

#endif // TOUCHGFXDMA_HPP

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/