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

#ifndef __BOOTLOADER_DEFINITIONS_H
#define __BOOTLOADER_DEFINITIONS_H

#define BL_FLASH_CLEAR_PKT_MAGIC 0xDEAD5732
#define BL_FLASH_WRITE_PKT_MAGIC 0x00131200

extern uint32_t _User_Flash_Start;
extern uint32_t _User_Flash_Length;
extern uint32_t _RAM_Start;
extern uint32_t _RAM_Length;

#define USER_FLASH_START ((uint32_t) &_User_Flash_Start)
#define USER_FLASH_LENGTH ((uint32_t) &_User_Flash_Length)
#define RAM_START ((uint32_t) &_RAM_Start)
#define RAM_LENGTH ((uint32_t) &_RAM_Length)

#define BOOTLOADER_IMAGE_HEADER_SIZE 0x28

#define BOOTLOADER_SKIP_VERIFICATION_INDICATOR "VPO"

void jump_to_user_fw();

#endif
