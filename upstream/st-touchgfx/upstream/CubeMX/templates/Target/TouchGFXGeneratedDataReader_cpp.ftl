[#ftl]
[#list SWIPdatas as data]
/**
  ******************************************************************************
  * File Name          : ${name.split("/")?last}
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */
#include <TouchGFXGeneratedDataReader.hpp>
#include <${FamilyName?lower_case}xx.h>

/*
 * External Interface description for starting transmissions
 */
extern "C" void DataReader_WaitForReceiveDone();
extern "C" void DataReader_ReadData(uint32_t address24, uint8_t* buffer, uint32_t length);
extern "C" void DataReader_StartDMAReadData(uint32_t address24, uint8_t* buffer, uint32_t length);

/*
 * flashEndAddress is calculated using the FLASH_BASE address provided in ${FamilyName?lower_case}xx.h
 * and by reading the flash size of the MCU located at address FLASHSIZE_BASE.
 * FLASHSIZE_BASE is in kBytes and is shifted left to translate into an address space.
 */
TouchGFXGeneratedDataReader::TouchGFXGeneratedDataReader() :
  flashEndAddress(FLASH_BASE + ((*(uint16_t*)FLASHSIZE_BASE) << 10)) {}

bool TouchGFXGeneratedDataReader::addressIsAddressable(const void* address)
{
  /* Check if adrress is in internal flash */
  if (address >= (void*)FLASH_BASE && address <(void*)(flashEndAddress))
  {
    return true;
  }

  /* Check if adrress is in internal RAM */
  if (address >= (void*)SRAM_BASE && address <(void*)(SRAM_BASE + SRAM_SIZE_MAX))
  {
    return true;
  }

  // Address is neither internal RAM or internal flash
  return false;
}

void TouchGFXGeneratedDataReader::copyData(const void* src, void* dst, uint32_t bytes)
{
  /* Redirect copy data through driver interface */
  DataReader_StartDMAReadData((uint32_t)src, (uint8_t*)dst, bytes);

  /* Wait on data transmision complete */
  DataReader_WaitForReceiveDone();
}

void TouchGFXGeneratedDataReader::startFlashLineRead(const void* src, uint32_t bytes)
{
[#if data.parameters.tgfx_datareader_block_size != "0"]
  /* If requested number of bytes are less than ${data.parameters.tgfx_datareader_dma_transfersize}
   * CPU and polling will be used to read from flash.
   * If number of bytes requested is larger than ${data.parameters.tgfx_datareader_dma_transfersize}
   * a DMA transfer will be started.
   */
  if (bytes < ${data.parameters.tgfx_datareader_dma_transfersize})
  {
    /* Start transfer using the CPU */
    DataReader_ReadData((uint32_t)src, (readToBuffer1 ? buffer1 : buffer2), bytes);
  }
  else
  {
    /* Start transfer using DMA */
    DataReader_StartDMAReadData((uint32_t)src, (readToBuffer1 ? buffer1 : buffer2), bytes);
  }
[#else]
  assert(0 && "Text or ARGB8888 used, set Line Buffer Size to width*4 bytes of the largets ARGB8888 or glyph.");
[/#if]
}

const uint8_t* TouchGFXGeneratedDataReader::waitFlashReadComplete()
{
[#if data.parameters.tgfx_datareader_block_size != "0"]
  /*
   * Switch line buffer and then wait for transmission to complete before returning new buffer.
   */
  uint8_t* readBuffer = readToBuffer1 ? buffer1 : buffer2;
  DataReader_WaitForReceiveDone();
  readToBuffer1 = !readToBuffer1;
  return readBuffer;
[#else]
  assert(0 && "Text or ARGB8888 used, set Line Buffer Size to width*4 bytes of the largets ARGB8888 or glyph.");
  return 0;
[/#if]
}
[/#list]
