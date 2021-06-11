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

#include "cobs.h"
#include "buffered_uart.h"
#include "packet_handler.h"

// CRC related code

#define PACKET_CRC_RESET CRC_CR_RESET

static uint32_t Packet_CalculateCrc(struct ll_pkt *pkt, size_t payload_len) {
	uint32_t crc32;

	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	// interrupts disabled


	CRC->CR = PACKET_CRC_RESET;

	for (size_t i=offsetof(struct ll_pkt_header, packet_id), length = sizeof(pkt->header) + payload_len; i<length; i++)
		CRC->DR = ((uint8_t *)pkt)[i];

	crc32 =  ~CRC->DR;

	// restore interrupts
	if (!primask) {
		__enable_irq();
	}

	return crc32;
}

// Rx related code

static uint32_t rx_framing_errors=0, rx_protocol_errors=0;

// TODO: should we define a shared max packet length?
static volatile uint8_t rx_decode_buf[1024];
static struct cobs_decode_state cobs_state;

void Packet_Rx_ProcessByte(char c) {
	int rc = cobs_decode_incremental(&cobs_state, (char *)rx_decode_buf, sizeof(rx_decode_buf), c);
	if (rc == 0) /* packet still incomplete */
		return;

	if (rc < 0) {
		rx_framing_errors++;
		return;
	}

	struct ll_pkt *pkt = (struct ll_pkt *)rx_decode_buf;

	/* A complete frame received */
	if (rc < sizeof(pkt->header)) {
		rx_protocol_errors++;
		return;
	}

	uint32_t crc = Packet_CalculateCrc(pkt, rc - sizeof(pkt->header));

	if (crc != pkt->header.crc32) {
		rx_protocol_errors++;
		return;
	}

	if (packet_received(pkt, rc - sizeof(pkt->header))) {
		rx_protocol_errors++;
	}
}

// poll regularly
void Packet_Worker() {
	BufferedUart_Rx_ProcessReceivedData((void (*)(uint8_t))Packet_Rx_ProcessByte);
}

// Tx related code

// TODO: single build buffer dependency

static uint8_t frame_build_buffer[256];

static void Packet_SetCrc(struct ll_pkt *pkt, size_t payload_len) {
    pkt->header.crc32 = Packet_CalculateCrc(pkt, payload_len);
}

/* len is the packet length excluding header */
int Packet_SendNonBlocking(struct ll_pkt *pkt, size_t payload_len) {
	Packet_SetCrc(pkt, payload_len);

    int32_t rc = cobs_encode(
    		(char *)frame_build_buffer, sizeof(frame_build_buffer),
    		(char *)pkt, sizeof(pkt->header) + payload_len);

    if (rc < 0) {
		return -PRES_COMMS_ERROR;
    }

    int32_t size = rc;

    return BufferedUart_Tx_Append(frame_build_buffer, size);
}

// init

void Packet_Init(UART_HandleTypeDef* h) {
	__HAL_RCC_CRC_CLK_ENABLE();
	
	BufferedUart_Init(h);

	cobs_decode_incremental_initialize(&cobs_state);

}

void __attribute__((weak)) Packet_Delay(uint32_t ms) {
	HAL_Delay(ms);
}
