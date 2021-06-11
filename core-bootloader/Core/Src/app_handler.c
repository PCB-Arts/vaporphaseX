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

#include "main.h"

#include "packet_handler.h"
#include "vpo_protocol.h"
#include "flash_if.h"

#include "bootloader_definitions.h"

int cmd_flash_erase()
{
	Flash_If_Init();

	for (size_t pg_addr= USER_FLASH_START;
			pg_addr < USER_FLASH_START + USER_FLASH_LENGTH;
			pg_addr += FLASH_PAGE_SIZE) {
		if (Flash_If_Erase(pg_addr))
			return PRES_SYSTEM_ERROR;
	}

	if (FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE) != HAL_OK)
		return PRES_SYSTEM_ERROR;

	return PRES_OK;
}

int cmd_flash_write(size_t target, uint8_t *data, size_t len)
{
	if (Flash_If_Write(data, (uint8_t *)target, len))
		return PRES_SYSTEM_ERROR;

	if (FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE) != HAL_OK)
		return PRES_SYSTEM_ERROR;

	return PRES_OK;
}

int app_packet_handler(struct ll_pkt *pkt, size_t payload_len) {
	switch (pkt->header.packet_type) {
	case PTYPE_BL_FLASH_CLEAR:
		if (payload_len != sizeof(pkt->payload.req_bl_flash_clear))
			return -PRES_INVALID_REQUEST;

		if (pkt->payload.req_bl_flash_clear.magic != BL_FLASH_CLEAR_PKT_MAGIC)
			return -PRES_INVALID_REQUEST;

		packet_errmsg = "Error clearing flash memory";
		return cmd_flash_erase();

	case PTYPE_BL_FLASH_WRITE:
		if (payload_len > sizeof(pkt->payload.req_bl_flash_write) || payload_len == 0 || (payload_len&3))
			return -PRES_INVALID_REQUEST;

		size_t target_addr = pkt->payload.req_bl_flash_write.address;
		size_t image_len = payload_len - offsetof(struct pkt_req_bl_flash_write, data);
		if (target_addr < 0 ||
				target_addr > USER_FLASH_LENGTH)
			return -PRES_INVALID_REQUEST;

		target_addr += USER_FLASH_START;

		if (target_addr + image_len > USER_FLASH_START + USER_FLASH_LENGTH)
			return -PRES_INVALID_REQUEST;

		if (pkt->payload.req_bl_flash_write.magic != BL_FLASH_WRITE_PKT_MAGIC)
			return -PRES_INVALID_REQUEST;

		packet_errmsg = "Error writing to flash memory";
		return cmd_flash_write(target_addr, pkt->payload.req_bl_flash_write.data, image_len);

	case PTYPE_REBOOT:
		if (payload_len != 0)
			return -PRES_INVALID_REQUEST;

		usart_respond(pkt, 0);
		HAL_Delay(500);
		NVIC_SystemReset();
		return -PRES_SYSTEM_ERROR;

	default:
		return PRES_COMMAND_NOT_FOUND;
	}
}

int app_prop_get (int prop_id, union prop_value *val)
{
	switch (prop_id) {
	case VPO_PROP_THIRD_PARTY_SOFTWARE:
		val->bl = HAL_GPIO_ReadPin(THIRD_PARTY_GPIO_Port, THIRD_PARTY_Pin) == GPIO_PIN_RESET;
		break;
	default:
		return -PRES_PROPERTY_NOT_FOUND;
	}

	return 0;
}



