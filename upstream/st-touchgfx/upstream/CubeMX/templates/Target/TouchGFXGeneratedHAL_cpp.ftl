[#ftl]
/**
  ******************************************************************************
  * File Name          : ${name.split("/")?last}
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */
[#list SWIPdatas as data]

[#-- This calculation is only for Partial FB so always assume its custom --]
[#assign pixelformat = data.parameters.tgfx_pixel_format.split(":")?first]
[#assign BitsPerPixel = data.parameters.tgfx_pixel_format.split(":")?last]
[#assign Address1 = data.parameters.tgfx_address1]
[#assign Address2 = data.parameters.tgfx_address2]

[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
[#assign Width = data.parameters.tgfx_ltdc_width]
[#assign Height = data.parameters.tgfx_ltdc_height]
[#else]
[#assign Width = data.parameters.tgfx_custom_width]
[#assign Height = data.parameters.tgfx_custom_height]
[/#if]

[#assign BytesPerPixel = 1]

[#assign FramebufferExpression = ""]
[#assign AdvanceHorizontally = "0"]
[#if BitsPerPixel == "1"]
[#assign AdvanceHorizontally = "rect.x / 8"]
[#assign FramebufferExpression = "(((${Width} + 7) / 8) * ${Height} + 3) / 4"]
[/#if]
[#if BitsPerPixel == "2"]
[#assign AdvanceHorizontally = "rect.x / 4"]
[#assign FramebufferExpression = "(((${Width} + 3) / 4) * ${Height} + 3) / 4"]
[/#if]
[#if BitsPerPixel == "4"]
[#assign AdvanceHorizontally = "rect.x / 2"]
[#assign FramebufferExpression = "(((${Width} + 1) / 2) * ${Height} + 3) / 4"]
[/#if]
[#if BitsPerPixel == "8"]
[#assign AdvanceHorizontally = "rect.x"]
[#assign FramebufferExpression = "(${Width} * ${Height} + 3) / 4"]
[/#if]
[#if BitsPerPixel == "16"]
[#assign BytesPerPixel = 2]
[#assign AdvanceHorizontally = "rect.x * 2"]
[#assign FramebufferExpression = "(${Width} * ${Height} * 2 + 3) / 4"]
[/#if]
[#if BitsPerPixel == "24"]
[#assign BytesPerPixel = 3]
[#assign AdvanceHorizontally = "rect.x * 3"]
[#assign FramebufferExpression = "(${Width} * ${Height} * 3 + 3) / 4"]
[/#if]
[#if BitsPerPixel == "32"]
[#assign BytesPerPixel = 4]
[#assign AdvanceHorizontally = "rect.x * 4"]
[#assign FramebufferExpression = "${Width} * ${Height}"]
[/#if]

#include <TouchGFXGeneratedHAL.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
#include <gui/common/FrontendHeap.hpp>
[#if data.parameters.tgfx_vsync == "vsync_ltdc"]
#include <touchgfx/hal/GPIO.hpp>
[/#if]
[#if data.parameters.tgfx_partialfb_transmit_strategy == "Enabled"]
#include <touchgfx/hal/PartialFrameBufferManager.hpp>
[/#if]
[#if data.parameters.tgfx_datareader == "Enabled"]
#include <TouchGFXDataReader.hpp>
[/#if]

#include "${FamilyName?lower_case}xx.h"
[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
#include "${FamilyName?lower_case}xx_hal_ltdc.h"
[/#if]

using namespace touchgfx;

[#if data.parameters.tgfx_location == "Allocate" || data.parameters.tgfx_display_interface == "disp_ltdc"]
namespace {
[#if data.parameters.tgfx_location == "Allocate"]
    // Use the section "TouchGFX_Framebuffer" in the linker to specify the placement of the buffer
    LOCATION_PRAGMA("TouchGFX_Framebuffer")
    uint32_t frameBuf[${FramebufferExpression}[#if data.parameters.tgfx_buffering_strategy == "Double"] * 2[/#if]] LOCATION_ATTRIBUTE("TouchGFX_Framebuffer");
[/#if]
[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
    static uint16_t lcd_int_active_line;
    static uint16_t lcd_int_porch_line;
[/#if]
}
[/#if]

[#if data.parameters.tgfx_buffering_strategy == "Partial"]
/* ******************************************************
 * Functions required by Partial Frame Buffer Strategy
 * ******************************************************
 *
 *  int touchgfxDisplayDriverTransmitActive() must return whether or not data is currently being transmitted, over e.g. SPI.
 *  void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h) will be called
 *  when the framework wants to send a block. The user must then transfer the data represented by the arguments.
[#if data.parameters.tgfx_partialfb_transmit_strategy == "Disabled"]
 *
 *  A user must call touchgfx::startNewTransfer(); once touchgfxDisplayDriverTransmitBlock() has successfully sent a block.
 *  E.g. if using DMA to transfer the block, this could be called in the "Transfer Completed" interrupt handler.
 *
[/#if]
 */

extern "C" int touchgfxDisplayDriverTransmitActive();
[#if data.parameters.tgfx_partialfb_transmit_strategy == "Enabled"]
extern "C" int touchgfxDisplayDriverShouldTransferBlock(uint16_t bottom);
[/#if]
extern "C" void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
extern "C" void touchgfxSignalVSync(void);

// Block Allocator for Partial Framebuffer strategy
static ManyBlockAllocator<${data.parameters.tgfx_block_size}, /* block size */
                   ${data.parameters.tgfx_number_of_blocks}, /* number of blocks */
                   ${BytesPerPixel}  /* bytes per pixel */
                   > blockAllocator;
[/#if]

void TouchGFXGeneratedHAL::initialize()
{
    HAL::initialize();

    registerEventListener(*(Application::getInstance()));
[#if data.parameters.tgfx_buffering_strategy == "Single" && data.parameters.tgfx_display_interface == "disp_ltdc"]
    registerTaskDelayFunction(&OSWrappers::taskDelay);
    setFrameRefreshStrategy(HAL::REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL);
[/#if]
[#if data.parameters.tgfx_oswrapper == "NoOS"]
    enableLCDControllerInterrupt();
    enableInterrupts();
[/#if]
[#if data.parameters.tgfx_buffering_strategy == "Partial"]
    // Partial framebuffer strategy
    setFrameBufferAllocator(&blockAllocator);
    setFrameRefreshStrategy(HAL::REFRESH_STRATEGY_PARTIAL_FRAMEBUFFER);
[/#if]
[#if data.parameters.tgfx_location == "Allocate"]
[#if data.parameters.tgfx_buffering_strategy == "Single"]
    setFrameBufferStartAddresses((void*)frameBuf, (void*)0, (void*)0);
[/#if]
[#if data.parameters.tgfx_buffering_strategy == "Double"]
    setFrameBufferStartAddresses((void*)frameBuf, (void*)(frameBuf + sizeof(frameBuf)/(sizeof(uint32_t)*2)), (void*)0);
[/#if]
[/#if]
[#if data.parameters.tgfx_location == "By Address"]
[#if data.parameters.tgfx_buffering_strategy == "Single"]
    setFrameBufferStartAddresses((void*)${Address1}, (void*)0, (void*)0);
[/#if]
[#if data.parameters.tgfx_buffering_strategy == "Double"]
    setFrameBufferStartAddresses((void*)${Address1}, (void*)${Address2}, (void*)0);
[/#if]
[/#if]
[#if data.parameters.tgfx_buffering_strategy == "Single"]
    /*
     * Set whether the DMA transfers are locked to the TFT update cycle. If
     * locked, DMA transfer will not begin until the TFT controller has finished
     * updating the display. If not locked, DMA transfers will begin as soon as
     * possible. Default is true (DMA is locked with TFT).
     */
    lockDMAToFrontPorch(true);
[/#if]
}

void TouchGFXGeneratedHAL::configureInterrupts()
{
[#if data.parameters.tgfx_hardware_accelerator == "dma_2d"]
    NVIC_SetPriority(DMA2D_IRQn, 9);
[/#if]
[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
    NVIC_SetPriority(LTDC_IRQn, 9);
[/#if]
}

void TouchGFXGeneratedHAL::enableInterrupts()
{
[#if data.parameters.tgfx_hardware_accelerator == "dma_2d"]
    NVIC_EnableIRQ(DMA2D_IRQn);
[/#if]
[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
    NVIC_EnableIRQ(LTDC_IRQn);
[/#if]
}

void TouchGFXGeneratedHAL::disableInterrupts()
{
[#if data.parameters.tgfx_hardware_accelerator == "dma_2d"]
    NVIC_DisableIRQ(DMA2D_IRQn);
[/#if]
[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
    NVIC_DisableIRQ(LTDC_IRQn);
[/#if]
}

void TouchGFXGeneratedHAL::enableLCDControllerInterrupt()
{
[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
    lcd_int_active_line = (LTDC->BPCR & 0x7FF) - 1;
    lcd_int_porch_line = (LTDC->AWCR & 0x7FF) - 1;

    /* Sets the Line Interrupt position */
    LTDC->LIPCR = lcd_int_active_line;
    /* Line Interrupt Enable            */
    LTDC->IER |= LTDC_IER_LIE;
[/#if]
}

[#-- Only Generate these methods if using partial framebuffer --]

[#if data.parameters.tgfx_buffering_strategy == "Partial"]
bool TouchGFXGeneratedHAL::beginFrame()
{
    return HAL::beginFrame();
}

void TouchGFXGeneratedHAL::endFrame()
{
[#--  If strategy is partial and display interface is custom then we must transmit the data manually --]
[#--  And therefor we also need to guard the next frame until all blocks are transferred --]
  // We must guard the next frame until we're done transferring all blocks over our display interface
  // through either a semaphore if user is running an OS or a simple variable if not
[#if data.parameters.tgfx_partialfb_transmit_strategy == "Enabled"]
  PartialFrameBufferManager::transmitRemainingBlocks();
[#else]
  // Wait until all blocks have been transmitted
  while(touchgfxDisplayDriverTransmitActive()){}
[/#if]

  HAL::endFrame();
  touchgfx::OSWrappers::signalRenderingDone();
}
[#elseif data.parameters.tgfx_display_interface == "disp_custom"]
inline uint8_t* TouchGFXGeneratedHAL::advanceFrameBufferToRect(uint8_t* fbPtr, const touchgfx::Rect& rect) const
{
    //       Advance vertically                   Advance horizontally
    fbPtr += rect.y * lcd().framebufferStride() + ${AdvanceHorizontally};
    return fbPtr;
}
[/#if]

uint16_t* TouchGFXGeneratedHAL::getTFTFrameBuffer() const
{
[#-- Improve the logic for this section to consider both display interface and framebuffer location. --]
[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
    return (uint16_t*)LTDC_Layer1->CFBAR;
[#elseif data.parameters.tgfx_location == "Allocate"]
[#-- TODO: Add support for Double FrameBuffer + Custom Display Interface (Also Generally). --]
    return (uint16_t*)frameBuf;
[#else]
    //getTFTFrameBuffer() not used for selected Frame Buffer Strategy
    return 0;
[/#if]
}

void TouchGFXGeneratedHAL::setTFTFrameBuffer(uint16_t* adr)
{
[#-- TODO: Add support for Double FrameBuffer / Custom Interface. --]
[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
    LTDC_Layer1->CFBAR = (uint32_t)adr;

    /* Reload immediate */
    LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
[#else]
    //setTFTFrameBuffer() not used for selected display interface
[/#if]
}

void TouchGFXGeneratedHAL::flushFrameBuffer(const touchgfx::Rect& rect)
{
  HAL::flushFrameBuffer(rect);
[#if data.parameters.tgfx_buffering_strategy == "Partial"]
[#if data.parameters.tgfx_partialfb_transmit_strategy == "Enabled"]
  // Try transmitting a block
  PartialFrameBufferManager::tryTransmitBlock();
[#else]
  // Once flushFrameBuffer() is called by the framework a block is already for transfer
  // Mark it ready for transfer and transmit it if user defined method isTransmittingData() does not return false
  // If data is not being transmitted, transfer the data with user defined method transmitFrameBufferBlock().
  frameBufferAllocator->markBlockReadyForTransfer();
  if (!touchgfxDisplayDriverTransmitActive())
  {
    touchgfx::Rect r;
    // Get pointer to block buffer and coordinates of the rect
    const uint8_t* pixels = frameBufferAllocator->getBlockForTransfer(r);
    // Start transmission of the block
    DisplayDriverTransmitBlock((uint8_t*)pixels, r.x, r.y, r.width, r.height);
  }
[/#if]
[/#if]
}

bool TouchGFXGeneratedHAL::blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes)
{
[#if data.parameters.tgfx_datareader == "Enabled"]
  // If requested address is addressable use TouchGFXDataReader to retrive the data
  if(reader != NULL && ( static_cast<TouchGFXDataReader*>(reader)->addressIsAddressable(src) == false ) )
  {
    static_cast<TouchGFXDataReader*>(reader)->copyData(src, dest, numBytes);
    return true;
  }
[/#if]
  return HAL::blockCopy(dest, src, numBytes);
}

[#if data.parameters.tgfx_display_interface == "disp_ltdc" && data.parameters.tgfx_buffering_strategy == "Single"]
uint16_t TouchGFXGeneratedHAL::getTFTCurrentLine()
{
    // This function only requires an implementation if single buffering
    // on LTDC display is being used (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).

    // The CPSR register (bits 15:0) specify current line of TFT controller.
    uint16_t curr = (uint16_t)(LTDC->CPSR & 0xffff);
    uint16_t backPorchY = (uint16_t)(LTDC->BPCR & 0x7FF) + 1;

    // The semantics of the getTFTCurrentLine() function is to return a value
    // in the range of 0-totalheight. If we are still in back porch area, return 0.
    return (curr < backPorchY) ? 0 : (curr - backPorchY);
}
[/#if]

[#if data.parameters.tgfx_buffering_strategy == "Partial"]
/*
 * ******************************************************
 * Required by Partial Frame Buffer Strategy
 * ******************************************************
 */
namespace touchgfx
{
/**
 * This function is called by FrameBufferAllocator if no block is
 * available.
 */
__weak void FrameBufferAllocatorWaitOnTransfer()
{
  /* NOTE: This function should not be modified, when the fuction is needed,
   *       FrameBufferAllocatorWaitOnTransfer should be implemented in the user file
   */
}

/**
 * Called by FrameBufferAllocator when a block is drawn and
 * therefore ready for transfer. The LCD driver should use this
 * method to start a transfer.
 */
__weak void FrameBufferAllocatorSignalBlockDrawn()
{
  /* NOTE: This function should not be modified, when the fuction is needed,
   *       FrameBufferAllocatorSignalBlockDrawn should be implemented in the user file
   */
}

[#if data.parameters.tgfx_partialfb_transmit_strategy == "Enabled"]
/**
 * Check if a Frame Buffer Block is beeing transmitted.
 */
__weak int transmitActive()
{
  return touchgfxDisplayDriverTransmitActive();
}

/**
 * Check if a Frame Buffer Block ending at bottom should be transferred.
 */
__weak int shouldTransferBlock(uint16_t bottom)
{
  return touchgfxDisplayDriverShouldTransferBlock(bottom);
}

/**
 * Transmit a Frame Buffer Block.
 */
__weak void transmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  touchgfxDisplayDriverTransmitBlock(pixels, x, y, w, h);
}
[#else]
// A user must call touchgfx::startNewTransfer(); once transmitFrameBufferBlock() has successfully sent a block.
void startNewTransfer()
{
  FrameBufferAllocator* fba = HAL::getInstance()->getFrameBufferAllocator();

  // Free the previous transmitted block, marking it ready for rendering
  fba->freeBlockAfterTransfer();
  if (fba->hasBlockReadyForTransfer())
  {
    touchgfx::Rect r;
    // Get pointer to block buffer and coordinates of the rect
    const uint8_t* pixels = fba->getBlockForTransfer(r);
    // Start transmission of the block
    DisplayDriverTransmitBlock((uint8_t*)pixels, r.x, r.y, r.width, r.height);
  }
}
[/#if]

extern "C"
void DisplayDriver_TransferCompleteCallback()
{
  // After completed transmission start new transfer if blocks are ready.
[#if data.parameters.tgfx_partialfb_transmit_strategy == "Enabled"]
  PartialFrameBufferManager::tryTransmitBlockFromIRQ();
[#else]
  touchgfx::startNewTransfer();
[/#if]
}
}

extern "C"
void touchgfxSignalVSync(void)
{
  /* VSync has occurred, increment TouchGFX engine vsync counter */
  touchgfx::HAL::getInstance()->vSync();

  /* VSync has occurred, signal TouchGFX engine */
  touchgfx::OSWrappers::signalVSync();
}
[/#if]

[#if data.parameters.tgfx_vsync == "vsync_ltdc"]
extern "C"
{
    void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
    {
        if (LTDC->LIPCR == lcd_int_active_line)
        {
            //entering active area
            HAL_LTDC_ProgramLineEvent(hltdc, lcd_int_porch_line);
            HAL::getInstance()->vSync();
            OSWrappers::signalVSync();
            // Swap frame buffers immediately instead of waiting for the task to be scheduled in.
            // Note: task will also swap when it wakes up, but that operation is guarded and will not have
            // any effect if already swapped.
            HAL::getInstance()->swapFrameBuffers();
            GPIO::set(GPIO::VSYNC_FREQ);
        }
        else
        {
            //exiting active area
            HAL_LTDC_ProgramLineEvent(hltdc, lcd_int_active_line);
            GPIO::clear(GPIO::VSYNC_FREQ);
            HAL::getInstance()->frontPorchEntered();
        }
    }
}
[/#if]
[/#list]
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
