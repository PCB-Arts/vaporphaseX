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

#ifndef PACKET_HANDLER_H_
#define PACKET_HANDLER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <global.h>


/* Flash clear operations can take a long time. Allow the device plenty of time to respond. */
#ifndef DEFAULT_TIMEOUT_MS
#define DEFAULT_TIMEOUT_MS 2000
#endif /* DEFAULT_TIMEOUT_MS */

/* Base protocol definitions */

enum packet_type {
	/* Base protocol packet types */
    PTYPE_NOP = 0,
	PTYPE_RESPONSE = 1,
	PTYPE_IDENTIFY = 2,
	PTYPE_REBOOT = 3,
	PTYPE_BL_FLASH_CLEAR = 4,
	PTYPE_BL_FLASH_WRITE = 5,
	PTYPE_PROP_SET = 6,
	PTYPE_PROP_GET = 7,
	PTYPE_LOG_MSG = 8,
	PTYPE_BOOTED = 9,
	PTYPE_PROP_WATCH = 10,

	/*  minimum packet type that can be allocated by application protocols */
	PTYPE_APP_START = 64,

	PTYPE_PROFILE_PACKAGE = 65,

	/* Error response. The device may respond to any request with an error packet. */
	PTYPE_ERROR = 255,
};

enum packet_error_codes {
	/* Base protocol error codes */
	PRES_OK = 0,
	PRES_GENERIC_ERROR = 1,
	/* The request was invalid (e.g. wrong payload size, invalid values etc.) */
	PRES_INVALID_REQUEST = 2,
	PRES_SYSTEM_ERROR = 3,
	/* Communication/USART error, e.g. invalid framing, buffer overrun or broken CRC */
	PRES_COMMS_ERROR = 4,
	PRES_PROPERTY_NOT_FOUND = 5,
	PRES_COMMAND_NOT_FOUND = 6,
	PRES_TIMEOUT = 7,
	/* Hardware reported an error condition */
	PRES_HW_ERROR = 8,
	PRES_EOF = 9,

	/* minimum error code that can be allocated by application protocols */
	PRES_APP_START = 1024,

	PRES_REMOTE_ERROR = 0x40000000,
};

/* Log protocol log message priorities */
enum log_msg_prio {
	P_CRITICAL,
	P_ERROR,
	P_WARNING,
	P_INFO,
	P_DEBUG,
	P_DEBUG_FINE,
	P_TRACE,
	P_USER
};

struct __attribute__((__packed__)) pkt_evt_log_msg {
	uint8_t prio;
	uint8_t msg[];
};

typedef struct __attribute__((__packed__)) __ProfileTransmissionPacket {
	int16_t times[60];
	int16_t temperatures[60];
	int8_t entriesOffset;
	int8_t numberEntries;
	int8_t totalEntries;
	int8_t profileId;
} ProfileTransmissionPacket;

/* Status packet firmware/hardware ID registry */

enum fw_ids {
	FW_ID_VPO_CORE_BOOTLOADER = 0x10,
	FW_ID_VPO_CORE = 0x20,
	FW_ID_VPO_DISPLAY_BOOTLOADER = 0x30,
	FW_ID_VPO_DISPLAY_MAIN_FW = 0x40,
};

enum hw_ids {
	HW_ID_VPO_CORE = 1,
	HW_ID_VPO_DISPLAY = 2
};

struct __attribute__((__packed__)) pkt_res_identify {
	uint32_t sys_time_ms;
	uint8_t device_id[12];
	uint16_t flash_size_kbytes;
	uint16_t chip_rev;
	uint16_t chip_id;
	uint16_t fw_id;
	uint16_t fw_ver_major;
	uint16_t fw_ver_minor;
	uint16_t hw_id;
	uint16_t fw_rev_major;
	uint16_t fw_rev_minor;
};

struct __attribute__((__packed__)) pkt_req_bl_flash_clear {
	uint32_t magic;
};

struct __attribute__((__packed__)) pkt_req_bl_flash_write {
	uint32_t magic;
	uint32_t address;
	uint8_t data[128];
};

struct __attribute__((__packed__)) pkt_res_error {
	int32_t rc;
	uint8_t msg[];
};

struct __attribute__((__packed__)) pkt_req_prop_get {
	uint16_t prop_id;
};

union prop_value {
	uint8_t u8;
	int8_t i8;
	uint16_t u16;
	int16_t i16;
	uint32_t u32;
	int32_t i32;
	float f32;
	bool bl;
	uint64_t u64;
	int64_t i64;
	uint8_t data[8];
};

struct __attribute__((__packed__)) pkt_req_prop_set {
	uint16_t prop_id;
	union prop_value val;
};

struct __attribute__((__packed__)) pkt_res_prop_get {
	union prop_value val;
};

union ll_pkt_payload {
	struct pkt_req_bl_flash_clear req_bl_flash_clear;
	struct pkt_req_bl_flash_write req_bl_flash_write;
   	struct pkt_req_prop_get req_prop_get;
   	struct pkt_req_prop_set req_prop_set;

   	struct pkt_res_identify res_identify;
   	struct pkt_res_error res_error;
   	struct pkt_res_prop_get res_prop_get;
};

struct __attribute__((__packed__)) ll_pkt_header {
	uint32_t crc32;
	/* CRC computed over entire packet starting here */
	uint8_t packet_id;
	uint8_t packet_type;
	uint16_t reserved; // align header to multiple of 4 bytes
};

struct __attribute__((__packed__)) ll_pkt {
	struct ll_pkt_header header;
    union ll_pkt_payload payload;
};

/* volatile pointer to volatile memory: memory written by USART ISR, pointer itself by packet_received */
extern volatile struct ll_pkt * volatile packet_res_buf;
extern volatile size_t packet_res_payload_len;
extern volatile const char * const remote_errmsg;


extern int app_prop_set(int prop_id, const union prop_value *val, size_t val_len);
extern int app_prop_get(int prop_id, union prop_value *val);
extern int app_packet_handler(struct ll_pkt *pkt, size_t payload_len);
extern int app_profile_frame_received(ProfileTransmissionPacket *packet);
extern void app_log_handler(struct ll_pkt *pkt, size_t payload_len);
extern const char *packet_errmsg;

/* Callback to pass received packet from USART driver to protocol logic */
int packet_received(struct ll_pkt *pkt, size_t payload_len);

int usart_respond(struct ll_pkt *req, size_t payload_len);
int usart_respond_error(struct ll_pkt *req, enum packet_error_codes err, const char *msg);
int usart_send_log_msg(uint8_t prio, const char *msg);

/* Send a sequence of NOP packets to synchronize receiver protocol handling */
int usart_sync_call_identify(struct pkt_res_identify *out);
int usart_sync_await_response(void *out, size_t out_len, uint8_t packet_id);
int usart_sync_prop_watch(int prop_id, union prop_value *out_buf, void (*callback)(int));
int usart_sync_prop_unwatch(int prop_id, union prop_value *out_buf);
int usart_sync_prop_changed(int prop_id, union prop_value *new_value);
int usart_async_prop_set(int prop_id, const union prop_value *in_buf);
int usart_sync_prop_get(int prop_id, union prop_value *out_buf);

void Packet_Init(UART_HandleTypeDef* h);
int Packet_SendNonBlocking(struct ll_pkt *pkt, size_t payload_len);
void Packet_Worker();

// redefine in user application to use other delay method than HAL_Delay
void Packet_Delay(uint32_t ms);

#ifdef __cplusplus
}
#endif
#endif /* PACKET_HANDLER_H_ */
