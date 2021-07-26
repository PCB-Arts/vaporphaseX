[#ftl]
/**
  ******************************************************************************
  * File Name          : ${name.split("/")?last}
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */
#n
/* USER CODE BEGIN ${name?split("/")?last} */

#include <TouchGFXDMA.hpp>

using namespace touchgfx;

TouchGFXDMA::TouchGFXDMA()
    : DMA_Interface(dma_queue), dma_queue(queue_storage, sizeof(queue_storage) / sizeof(queue_storage[0]))
{

}

TouchGFXDMA::~TouchGFXDMA()
{

}

void TouchGFXDMA::initialize()
{

}

BlitOperations TouchGFXDMA::getBlitCaps()
{
    // Defalt all blitOps are disabled
    // Enable those that are supported by uncommenting the blitOp below
    return static_cast<BlitOperations>(
                                        0
                                    /*  | BLIT_OP_FILL */
                                    /*  | BLIT_OP_FILL_WITH_ALPHA */
                                    /*  | BLIT_OP_COPY */
                                    /*  | BLIT_OP_COPY_WITH_ALPHA */
                                    /*  | BLIT_OP_COPY_ARGB8888 */
                                    /*  | BLIT_OP_COPY_ARGB8888_WITH_ALPHA */
                                    /*  | BLIT_OP_COPY_A4 */
                                    /*  | BLIT_OP_COPY_A8 */
                                      );
}


/*
 * Configures the DMA for copying data to the frame buffer.
 * Is called for the following BlitOperations:
 * BLIT_OP_COPYBLIT_OP_COPY_WITH_ALPHA, BLIT_OP_COPY_ARGB8888, BLIT_OP_COPY_ARGB8888_WITH_ALPHA, BLIT_OP_COPY_A4, and BLIT_OP_COPY_A8
 */
void TouchGFXDMA::setupDataCopy(const BlitOp& blitOp)
{
    /* After transfer completion call signalDMAInterrupt() */
}

/*
 * Configures the DMA for "filling" the frame-buffer with a single color.
 * Is called for the following BlitOperations:
 * BLIT_OP_FILL and BLIT_OP_FILL_WITH_ALPHA
 */
void TouchGFXDMA::setupDataFill(const BlitOp& blitOp)
{
    /* After transfer completion call signalDMAInterrupt() */
}

void TouchGFXDMA::start()
{
    // If the framebuffer is placed in cached memory (e.g. SRAM) then we need
    // to flush the Dcache prior to letting DMA2D accessing it. That's done
    // using SCB_CleanDCache() or SCB_CleanDCache_by_Addr().
    DMA_Interface::start();
}

/* USER CODE END ${name?split("/")?last} */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/