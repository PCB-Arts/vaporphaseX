/**
 ******************************************************************************
 * @file    Src/main.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    4-April-2016
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *notice, this list of conditions and the following disclaimer in the
 *documentation and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/** @addtogroup STM32L4xx_IAP_Main
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "command.h"
#include "flash_if.h"
#include "memory_card.h"
#include "mx_init.h"
#include "third_party.h"
#include "bootloader_definitions.h"

#include <packet_handler.h>

#include <tinyprintf.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAX_MENU_INDEX ((uint32_t)3)
#define MENU_ACTIVE ((uint32_t)1)
#define MENU_NOT_ACTIVE ((uint32_t)0)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction JumpToApplication;
uint32_t JumpAddress;

uint32_t apb1_frequency;

#define LAYER0_ADDRESS (LCD_FB_START_ADDRESS)
#define LCD_BG_LAYER_ADDRESS 0xC1300000

__IO uint32_t valid_buffer = 0;

#define VSYNC 1
#define VBP 1
#define VFP 1
#define VACT 480
#define HSYNC 1
#define HBP 1
#define HFP 1
#define HACT 800

#define BGCOLOR 0xff000000

uint8_t pColLeft[] = {0x00, 0x00, 0x03, 0x1F}; /*   0 -> 399 */
uint8_t pPage[] = {0x00, 0x00, 0x01, 0xDF};    /*   0 -> 479 */
uint8_t pSyncLeft[] = {0x02, 0x15};            /* Scan @ 533 */

extern LTDC_HandleTypeDef hltdc_eval;
extern DSI_HandleTypeDef hdsi_eval;

FIL DownloadFile;
const char *filename_disp_fw = "VPO_DISP.BIN";
const char *filename_disp_fw_external = "VPO_DISP_ASSETS.BIN";
const char *filename_core_fw = "VPO_CORE.BIN";

static void SystemClock_Config(void);
static void Main_Menu(void);
void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address);
static void await_user_button(void);

static int Main_lcd_line = 1;
static char Main_line[64] = {' '};

static int retry_counter = 3;

void Main_LCD_printf(const char *format, ...) {
  va_list format_arguments;

  va_start(format_arguments, format);

  // keep single space at the beginning
  tfp_vsnprintf(&Main_line[1], sizeof(Main_line) - 1, format, format_arguments);

  va_end(format_arguments);

  BSP_LCD_DisplayStringAtLine(Main_lcd_line++, (uint8_t *)Main_line);
}

void Main_LCD_Clear() {
  BSP_LCD_Clear(BGCOLOR);
  Main_lcd_line = 1;
}

#define BOOTLOADER_CHECK_Pin GPIO_PIN_13
#define BOOTLOADER_CHECK_GPIO_Port GPIOG

void Main_BootloaderSwitchInit() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOG_CLK_ENABLE();

  GPIO_InitStruct.Pin = BOOTLOADER_CHECK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BOOTLOADER_CHECK_GPIO_Port, &GPIO_InitStruct);
}


int main(void) {
  /* STM32L4xx HAL library initialization:
 - Configure the Flash prefetch
 - Systick timer is configured by default as source of time base, but user
   can eventually implement his proper time base source (a general purpose
   timer for example or other time source), keeping in mind that Time base
   duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
   handled in milliseconds basis.
 - Low Level Initialization
   */
  HAL_Init();

  Main_BootloaderSwitchInit();

  /* Jump into bootloader mode if Key push-button on board is pressed, or if the
   * application interrupt table looks invalid. */
  if (HAL_GPIO_ReadPin(BOOTLOADER_CHECK_GPIO_Port, BOOTLOADER_CHECK_Pin) == GPIO_PIN_SET) {
	  jump_to_user_fw();
  }

	/* Configure the system clock */
	SystemClock_Config();

	MX_DMA_Init();

	#if UART_INTERFACE == 3
	MX_USART3_UART_Init();
	Packet_Init(&huart3);
	#elif UART_INTERFACE == 6
	MX_USART6_UART_Init();
	Packet_Init(&huart6);
	#endif

	/* Initialize the SDRAM */
	BSP_SDRAM_Init();

	/* Init the user interface */
	BSP_LCD_InitEx(LCD_ORIENTATION_LANDSCAPE);
	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND,
							 LCD_FB_START_ADDRESS);


	/* Clear the LCD */
	BSP_LCD_Clear(BGCOLOR);
	/* Set the LCD Back Color */
	BSP_LCD_SetBackColor(BGCOLOR);
	/* Set the LCD Text Color */
	BSP_LCD_SetTextColor(0xffffffff);
	/* Set the font size */
	BSP_LCD_SetFont(&Font20);
	Main_LCD_printf("VaporPhaseOne Bootloader");

	valid_buffer = 1;
	HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2,
					  OTM8009A_CMD_WRTESCN, pSyncLeft);

	/* Initialize CARD */
	if (Card_Init() == FR_OK) {
	  /* Run main menu */
	  Main_Menu();
	} else {
	  Card_Unlink();
	  /*  Error message */
	  Main_LCD_printf("Cannot open micro SD card");
	  Main_LCD_printf("### FATAL ERROR ###");
	  Main_LCD_printf("Please power-cycle or reset device.");
	}

  /* Infinite loop */
  while (1) {
  }
}

void Main_print_usart_error(int rc) {
  if (rc < -PRES_REMOTE_ERROR) {
    Main_LCD_printf("Remote error: %d", (-rc) & ~PRES_REMOTE_ERROR);
  } else {
	  Main_LCD_printf("Communication error: %d", -rc);
  }
}

int usart_sync_cmd_flash_clear() {
  struct ll_pkt pkt = {
      .header = {.packet_id = 0, .packet_type = PTYPE_BL_FLASH_CLEAR},
      .payload = {.req_bl_flash_clear = {.magic = 0xDEAD5732}}};

  int rc = Packet_SendNonBlocking(&pkt, sizeof(struct pkt_req_bl_flash_clear));
  if (rc)
    return rc;

  return usart_sync_await_response(NULL, 0, 0);
}

int usart_sync_cmd_reboot() {
  struct ll_pkt pkt = {.header = {.packet_id = 0, .packet_type = PTYPE_REBOOT}};

  return Packet_SendNonBlocking(&pkt, 0);
}

/**
 * sends one paket of data to flash to the core controlller.
 * @param buffer pointer to data to read from
 * @param available_data number of bytes available for reading from buffer
 * @param addr of core controller to write to
 * @returns amount of bytes send in written command (<= available_data) or error
 * code
 */
int usart_sync_cmd_flash_write(uint8_t *buffer, size_t available_data,
                               size_t addr) {
  struct ll_pkt pkt = {
      .header = {.packet_id = 0, .packet_type = PTYPE_BL_FLASH_WRITE},
      .payload = {.req_bl_flash_write = {
                      .magic = 0x00131200,
                      .address = addr,
                  }}};

  size_t max_paket_data_size = sizeof(pkt.payload.req_bl_flash_write.data);

  size_t bytes_to_send = available_data < max_paket_data_size
                             ? available_data
                             : max_paket_data_size;

  memcpy(pkt.payload.req_bl_flash_write.data, buffer, bytes_to_send);

  if (bytes_to_send == 0)
    return -PRES_EOF;

  int rc = Packet_SendNonBlocking(
      &pkt, offsetof(struct pkt_req_bl_flash_write, data) + bytes_to_send);
  if (rc)
    return rc;

  rc = usart_sync_await_response(NULL, 0, 0);
  if (rc < 0)
    return rc;

  return bytes_to_send;
}

static void await_user_button() {
  /* Wait for button to be released */
  while (HAL_GPIO_ReadPin(BOOTLOADER_CHECK_GPIO_Port, BOOTLOADER_CHECK_Pin) == GPIO_PIN_RESET) {
	  HAL_Delay(1);
  }

  /* Wait for button to be pressed */
  while (HAL_GPIO_ReadPin(BOOTLOADER_CHECK_GPIO_Port, BOOTLOADER_CHECK_Pin) == GPIO_PIN_SET) {
    HAL_Delay(1);
  }

}

int Main_FlashUserCoreFirmwareCallback(uint8_t *data, size_t offset,
                                       size_t length) {
  size_t bytes_written = 0;

  while (length - bytes_written > 0) {
    int result = usart_sync_cmd_flash_write(
        &data[bytes_written], length - bytes_written, offset + bytes_written);

    if (result < 0) {
      Main_print_usart_error(result);
      return DOWNLOAD_WRITE_FAIL;
    }

    bytes_written += result;
  }

  return DOWNLOAD_OK;
}

int Main_FlashUserCoreFirmware() {
  int rc;

  if (f_open(&DownloadFile, filename_core_fw, FA_READ) != FR_OK) {
    Main_LCD_printf("Cannot open core file: %s", filename_core_fw);
    return 1;
  }

  /* Do a dry-run first to make sure the signature is valid */
  if (COMMAND_ProgramFlashMemory(&DownloadFile, NULL) != DOWNLOAD_OK) {
    Main_LCD_printf("Core firmware image signature verification error");
    return 1;
  }

  if (f_lseek(&DownloadFile, 0) != FR_OK) {
    return 1;
  }

  Main_LCD_printf("Erasing core controller...");
  if ((rc = usart_sync_cmd_flash_clear())) {
    Main_print_usart_error(rc);
    return 1;
  }

  Main_LCD_printf("Erase complete. Programming core controller...");


  /* Now do the actual programming. Verify the signature again to protect
   * against TOCTOU. */
  if (COMMAND_ProgramFlashMemory(
          &DownloadFile, Main_FlashUserCoreFirmwareCallback) != DOWNLOAD_OK) {
    Main_LCD_printf("Programming error");
    return 1;
  }

  f_close(&DownloadFile);

  return 0;
}

int Main_FlashUserDisplayFirmwareCallback(uint8_t *data, size_t offset,
                                          size_t length) {
  /* Program flash memory */
  for (size_t i = 0; i < length; i += 4) {
    uint32_t word_to_flash = *(uint32_t *)(data + i);

    if (FLASH_If_Write((APPLICATION_ADDRESS + offset + i), word_to_flash) !=
        0x00) {
      return DOWNLOAD_WRITE_FAIL;
    }
  }

  return DOWNLOAD_OK;
}

int Main_FlashUserDisplayFirmwareInternal() {
  if (f_open(&DownloadFile, filename_disp_fw, FA_READ) != FR_OK) {
    Main_LCD_printf("Cannot open display file: %s", filename_disp_fw);
    return 1;
  }

  /* Do a dry-run first to make sure the signature is valid */
  if (COMMAND_ProgramFlashMemory(&DownloadFile, NULL) != DOWNLOAD_OK) {
    Main_LCD_printf("Display firmware image signature verification error");
    return 1;
  }

  if (f_lseek(&DownloadFile, 0) != FR_OK) {
    return 1;
  }

  Main_LCD_printf("Erasing display controller...");

  FLASH_If_FlashUnlock();
  if (FLASH_If_EraseSectors(APPLICATION_ADDRESS) != 0) {
    Main_LCD_printf("Flash erase error");
    return 1;
  }

  Main_LCD_printf("Erase complete. Programming display controller...");
  /* Now do the actual programming. Verify the signature again to protect
   * against TOCTOU. */
  if (COMMAND_ProgramFlashMemory(&DownloadFile,
                                 Main_FlashUserDisplayFirmwareCallback) !=
      DOWNLOAD_OK) {
    Main_LCD_printf("Programming error");
    return 1;
  }

  f_close(&DownloadFile);

  FLASH_IF_FlashLock();

  return 0;
}

int Main_FlashUserDisplayExternalCallback(uint8_t *data, size_t offset,
                                          size_t length) {
  return BSP_QSPI_Write(data, offset, length) == QSPI_OK ? DOWNLOAD_OK
                                                         : DOWNLOAD_WRITE_FAIL;
}

int Main_FlashUserDisplayFirmwareExternal() {
  Main_LCD_printf("External display flash: ");

  if (BSP_QSPI_Init() != QSPI_OK) {
    Main_LCD_printf("Error initializing QSPI.");
    return 1;
  }

  if (f_open(&DownloadFile, filename_disp_fw_external, FA_READ) != FR_OK) {
    Main_LCD_printf("Cannot open file: %s", filename_disp_fw);
    return 1;
  }

  Main_LCD_printf("Opened Downloadfile successfully");

  /* Do a dry-run first to make sure the signature is valid */
  if (COMMAND_ProgramFlashMemory(&DownloadFile, NULL) != DOWNLOAD_OK) {
    Main_LCD_printf("Firmware image signature verification error");
    return 1;
  }

  Main_LCD_printf("Signature checked successfully");

  if (f_lseek(&DownloadFile, 0) != FR_OK) {
	Main_LCD_printf("f_lseek problem");
    return 1;
  }

  Main_LCD_printf("Erasing external display flash...");

  if (BSP_QSPI_Erase_Chip() != QSPI_OK) {
    Main_LCD_printf("Flash erase error");
    return 1;
  }
  Main_LCD_printf("Erase complete. Programming external display flash...");

  /* Now do the actual programming. Verify the signature again to protect
   * against TOCTOU. */
  if (COMMAND_ProgramFlashMemory(&DownloadFile,
                                 Main_FlashUserDisplayExternalCallback) !=
      DOWNLOAD_OK) {
    Main_LCD_printf("Programming error");
    return 1;
  }

  f_close(&DownloadFile);

  BSP_QSPI_DeInit();

  return 0;
}

/**
 * @brief  Display the Main Menu on Display
 * @param  None
 * @retval None
 */
void Main_Menu(void) {
  struct pkt_res_identify core_id_response;
  bool flash_only_display = false;

  int rc = usart_sync_call_identify(&core_id_response);
  if (rc < 0) {
    Main_LCD_printf("Cannot reach core controller");
    Main_print_usart_error(rc);
    Main_LCD_printf("Press USER button to ignore");
    await_user_button();
    Main_LCD_printf("continuing");
    flash_only_display = true;
  } else {
    Main_LCD_printf("Core connected: fw %d/hw %d", core_id_response.fw_id,
                    core_id_response.hw_id);
    if (core_id_response.fw_id != FW_ID_VPO_CORE_BOOTLOADER) {
    	Main_LCD_printf("Error: Core is not running the bootloader!");
    	Main_LCD_printf("Please set the core into bootloader state and try again.");
    	return;
    }

    rc = ThirdParty_Init();

    if (rc != 0) {
    	Main_LCD_printf("Error reading third party allowed state.");
    	return;
    }

    if (ThirdParty_Allowed()) {
    	Main_LCD_printf("Signature verification is disabled!");
    }
  }

  fileinfo_t finfno = {0};
  if (f_stat(filename_disp_fw, &finfno) != FR_OK) {
    Main_LCD_printf("Display firmware image \"%s\" not found", filename_disp_fw);
    return;
  }

  if (finfno.fsize > (FLASH_SIZE - IAP_SIZE)) {
    Main_LCD_printf("Display firmware image too large");
    return;
  }

  if (f_stat(filename_disp_fw_external, &finfno) != FR_OK) {
    Main_LCD_printf("Display firmware asset image \"%s\"", filename_disp_fw_external);
    Main_LCD_printf("not found");
    return;
  }

  if (!flash_only_display) {
    fileinfo_t finfno_core = {0};
    if (f_stat(filename_core_fw, &finfno_core) != FR_OK) {
      Main_LCD_printf("Core firmware image \"%s\" not found", filename_core_fw);
      return;
    }
  }

  Main_LCD_printf("Firmware update files found");
  Main_LCD_printf("Press user button to update firmware");
  if (flash_only_display) {
    Main_LCD_printf("### CAUTION! Communication failure! ###");
    Main_LCD_printf("### This will ONLY FLASH THE DISPLAY CONTROLLER ###");
  }
  await_user_button();

  Main_LCD_Clear();
  Main_LCD_printf("VaporPhaseOne Bootloader");

  if (!flash_only_display) {
    rc = Main_FlashUserCoreFirmware();
    // When flashing didn't work
    if (rc != 0) {
    	Main_LCD_Clear();
    	for (int i = 0; i <= retry_counter; i++){
    		Main_LCD_printf("Problem during core flashing. Retry %s", i);
    		rc = Main_FlashUserCoreFirmware();
    		if (rc == 0){
    			//leave the retry
    			break;
    		}
    		if (retry_counter == i){
    			// return after retring
    			return;
    		}
    	}
    }
  }

  rc = Main_FlashUserDisplayFirmwareInternal();
  // When flashing didn't work
  if (rc != 0) {
	Main_LCD_Clear();
	for (int i = 0; i <= retry_counter; i++){
		Main_LCD_printf("Problem during display flashing. Retry %s", i);
		rc = Main_FlashUserDisplayFirmwareInternal();
		if (rc == 0){
			//leave the retry
			break;
		}
		if (retry_counter == i){
			// return after retring
			return;
		}
	}
  }


  rc = Main_FlashUserDisplayFirmwareExternal();
  // When flashing didn't work
  if (rc != 0) {
  Main_LCD_Clear();
	for (int i = 0; i <= retry_counter; i++){
		Main_LCD_printf("Problem during ext_display flashing. Retry %s", i);
		rc = Main_FlashUserDisplayFirmwareExternal();
		if (rc == 0){
			//leave the retry
			break;
		}
		if (retry_counter == i){
			// return after retring
			return;
		}
	}
  }

  Main_LCD_printf("Programming complete");
  Main_LCD_printf("Please power-cycle device to reboot into new firmware");
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            PLL_R                          = 6
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 *         The SD clock configuration from PLLSAI:
 *            PLLSAIN                        = 384
 *            PLLSAIP                        = 8
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device
is clocked below the maximum system frequency, to update the voltage scaling
value regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if defined(USE_STM32469I_DISCO_REVA)
  RCC_OscInitStruct.PLL.PLLM = 25;
#else
  RCC_OscInitStruct.PLL.PLLM = 8;
#endif /* USE_STM32469I_DISCO_REVA */
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);

  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }

  /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }

  /* generate 48Mhz for SD card clock */
  RCC_PeriphClkInitStruct.PeriphClockSelection =
      RCC_PERIPHCLK_SDIO | RCC_PERIPHCLK_CK48;
  RCC_PeriphClkInitStruct.SdioClockSelection = RCC_SDIOCLKSOURCE_CK48;
  RCC_PeriphClkInitStruct.Clk48ClockSelection = RCC_CK48CLKSOURCE_PLLSAIP;
  RCC_PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  RCC_PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }

  SystemCoreClockUpdate();
  apb1_frequency = SystemCoreClock / 4;
}

/**
 * @brief  Initializes the LCD layers.
 * @param  LayerIndex: Layer foreground or background
 * @param  FB_Address: Layer frame buffer
 * @retval None
 */
void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address) {
  LCD_LayerCfgTypeDef Layercfg;

  /* Layer Init */
  Layercfg.WindowX0 = 0;
  Layercfg.WindowX1 = BSP_LCD_GetXSize() / 2;
  Layercfg.WindowY0 = 0;
  Layercfg.WindowY1 = BSP_LCD_GetYSize();
  Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  Layercfg.FBStartAdress = Address;
  Layercfg.Alpha = 255;
  Layercfg.Alpha0 = 0;
  Layercfg.Backcolor.Blue = 0;
  Layercfg.Backcolor.Green = 0;
  Layercfg.Backcolor.Red = 0;
  Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  Layercfg.ImageWidth = BSP_LCD_GetXSize() / 2;
  Layercfg.ImageHeight = BSP_LCD_GetYSize();

  HAL_LTDC_ConfigLayer(&hltdc_eval, &Layercfg, LayerIndex);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void) {
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1) {
  }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* User can add his own implementation to report the file name and line
number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1) {
  }
}

#endif

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
