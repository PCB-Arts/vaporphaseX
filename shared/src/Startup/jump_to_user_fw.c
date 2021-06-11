/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2021 - PCB Arts GmbH
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#ifdef BOOTLOADER

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "global.h"
#include "bootloader_definitions.h"
#include "conf.h"

#include "mbedtls/sha256.h"

/* Caution! These variables are "placed" by the linker at their respective locations. Only use their addresses through the macros below! */
extern uint32_t _User_Flash_Start;
extern uint32_t _User_Flash_Length;
extern uint32_t _RAM_Start;
extern uint32_t _RAM_Length;

#define USER_FLASH_START ((uint32_t) &_User_Flash_Start)
#define USER_FLASH_LENGTH ((uint32_t) &_User_Flash_Length)
#define RAM_START ((uint32_t) &_RAM_Start)
#define RAM_LENGTH ((uint32_t) &_RAM_Length)


typedef struct __attribute__((packed)) __user_code_header {
  char skip_indicator[4];
  int32_t size;
  uint8_t sha256[32];
} user_code_header ;


const uint8_t* user_data = (uint8_t*) (USER_FLASH_START + BOOTLOADER_IMAGE_HEADER_SIZE);

uint32_t *user_interrupt_vector = (uint32_t*)  (USER_FLASH_START + BOOTLOADER_ISR_OFFSET);

const user_code_header *image_header = (user_code_header*) USER_FLASH_START;

static bool validate_user_firmware() {
	int user_code_size = image_header->size;

	if(user_code_size <= 0 || user_code_size > USER_FLASH_LENGTH) {
		return false;
	}

	uint8_t calculated_sha256[32];

	mbedtls_sha256(user_data, user_code_size, calculated_sha256, 0);

	return memcmp(calculated_sha256, image_header->sha256, sizeof(calculated_sha256)) == 0;
}

static bool should_validate_firmware() {
	// header that starts with VPO indicates no validation
	return strncmp(BOOTLOADER_SKIP_VERIFICATION_INDICATOR, image_header->skip_indicator, sizeof(image_header->skip_indicator)) != 0;
}

/* Returns on error */
void jump_to_user_fw()
{
	if (should_validate_firmware()) {
		if (!validate_user_firmware()) {
			return;
		}
	}

	/* Check for valid interrupt table by checking if the top of stack (vector entry 0) is within RAM bounds */
	if (user_interrupt_vector[0] >= RAM_START && user_interrupt_vector[0] <= RAM_START + RAM_LENGTH)
	{
		void (*user_app_entry)(void) = (void(*)(void)) user_interrupt_vector[1];

		// cleanup
		HAL_RCC_DeInit();
		HAL_DeInit();

		SysTick->CTRL = 0;
		SysTick->LOAD = 0;
		SysTick->VAL = 0;

		/* Initialize user application's Stack Pointer */
		__set_MSP(user_interrupt_vector[0]);

		// run firmware
		user_app_entry();
	}
}

#endif
