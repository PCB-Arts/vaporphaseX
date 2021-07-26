[#ftl]
[#list configs as config]
[#assign HAL = config.family+"HAL"]
[/#list]
[#list SWIPdatas as data]
[#assign pixelformat = data.parameters.tgfx_pixel_format.split(":")?first]
[#assign BitsPerPixel = data.parameters.tgfx_pixel_format.split(":")?last]
[#compress]

[#--  Determine which width and height to use --]
[#assign Width = 0]
[#assign Height = 0]
[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
[#assign Width = data.parameters.tgfx_ltdc_width]
[#assign Height = data.parameters.tgfx_ltdc_height]
[#else]
[#assign Width = data.parameters.tgfx_custom_width]
[#assign Height = data.parameters.tgfx_custom_height]
[/#if]

[#-- Default --]
[#assign BytesPerPixel = 1]
[#if BitsPerPixel == "16"]
[#assign BytesPerPixel = 2]
[/#if]
[#if BitsPerPixel == "24"]
[#assign BytesPerPixel = 3]
[/#if]
[#if BitsPerPixel == "32"]
[#assign BytesPerPixel = 4]
[/#if]

/**
  ******************************************************************************
  * File Name          : ${name.split("/")?last}
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */

#include <texts/TypedTextDatabase.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <BitmapDatabase.hpp>
[#if data.parameters.tgfx_pixel_format.contains("ABGR2222") | data.parameters.tgfx_pixel_format.contains("ARGB2222") | data.parameters.tgfx_pixel_format.contains("BGRA2222") | data.parameters.tgfx_pixel_format.contains("RGBA2222")]
#include <platform/driver/lcd/LCD8bpp_${pixelformat}.hpp>
[#else]
[#if data.parameters.tgfx_datareader?? && data.parameters.tgfx_datareader == "Enabled"]
#include <TouchGFXDataReader.hpp>
#include <platform/driver/lcd/LCD${BitsPerPixel}bppSerialFlash.hpp>
[#else]
#include <platform/driver/lcd/LCD${BitsPerPixel}bpp.hpp>
[/#if]
[/#if]
[#if data.parameters.tgfx_oswrapper == "NoOS" | (data.parameters.tgfx_buffering_strategy == "Single" && data.parameters.tgfx_display_interface == "disp_ltdc")]
[#--  Hack to drive application when using NoOS --]
[#--  Include if using REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL --]
#include <touchgfx/hal/OSWrappers.hpp>
[/#if]

[#if data.parameters.tgfx_hardware_accelerator == "dma_2d"]
#include <STM32DMA.hpp>
[/#if]
[#if data.parameters.tgfx_hardware_accelerator == "dma_none"]
#include <touchgfx/hal/NoDMA.hpp>
[/#if]
[#if data.parameters.tgfx_hardware_accelerator == "dma_custom"]
#include <TouchGFXDMA.hpp>
[/#if]
#include <TouchGFXHAL.hpp>
#include <STM32TouchController.hpp>
#include <${FamilyName?lower_case}xx_hal.h>
[/#compress]
#n
extern "C" void touchgfx_init();
extern "C" void touchgfx_taskEntry();

[#compress]
static STM32TouchController tc;
[#if data.parameters.tgfx_hardware_accelerator == "dma_2d"]
static ${FamilyName}DMA dma;
[/#if]
[#if data.parameters.tgfx_hardware_accelerator == "dma_none"]
static NoDMA dma;
[/#if]
[#if data.parameters.tgfx_hardware_accelerator == "dma_custom"]
static TouchGFXDMA dma;
[/#if]
[#if data.parameters.tgfx_pixel_format.contains("ABGR2222") | data.parameters.tgfx_pixel_format.contains("ARGB2222") | data.parameters.tgfx_pixel_format.contains("BGRA2222") | data.parameters.tgfx_pixel_format.contains("RGBA2222")]
static LCD8bpp_${pixelformat} display;
[#else]
[#if data.parameters.tgfx_datareader?? && data.parameters.tgfx_datareader == "Enabled"]
static TouchGFXDataReader dataReader;
static LCD${BitsPerPixel}bppSerialFlash display(dataReader);
[#else]
static LCD${BitsPerPixel}bpp display;
[/#if]
[/#if]
[/#compress]

static ApplicationFontProvider fontProvider;
static Texts texts;
static TouchGFXHAL hal(dma, display, tc, ${Width}, ${Height});

void touchgfx_init()
{
  Bitmap::registerBitmapDatabase(BitmapDatabase::getInstance(), BitmapDatabase::getInstanceSize());
  TypedText::registerTexts(&texts);
  Texts::setLanguage(0);

[#if data.parameters.tgfx_datareader?? && data.parameters.tgfx_datareader == "Enabled"]
  /*
   * Parse TouchGFXDataReader instance pointer to TouchGFXHAL and ApplicationFontProvider
   * in order to enable external data access.
   */
  hal.setDataReader(&dataReader);
  fontProvider.setFlashReader(&dataReader);
[/#if]
  FontManager::setFontProvider(&fontProvider);

  FrontendHeap& heap = FrontendHeap::getInstance();
  /*
   * we need to obtain the reference above to initialize the frontend heap.
   */
  (void)heap;

  /*
   * Initialize TouchGFX
   */
  hal.initialize();
}

void touchgfx_taskEntry()
{
[#if data.parameters.tgfx_oswrapper == "NoOS"]
 /*
  * Main event loop will check for VSYNC signal, and then process next frame.
  *
  * Note This function returns immediately if there is no VSYNC signal.
  */
  if (OSWrappers::isVSyncAvailable())
  {
    hal.backPorchExited();
  }
[/#if]
[#if data.parameters.tgfx_oswrapper?contains("CMSIS")]
 /*
  * Main event loop. Will wait for VSYNC signal, and then process next frame. Call
  * this function from your GUI task.
  *
  * Note This function never returns
  */
  hal.taskEntry();
[/#if]
}
[/#list]

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
