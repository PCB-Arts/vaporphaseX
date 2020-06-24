/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 PCB Arts GmbH - Kevin Schick
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  _____    ____    ____
 *       \  /       |    \        /\            |
 *   ____/  |       |____/       /  \     __  __|__   ___
 *  |       |       |    \      /____\   /      |    |___
 *  |       \____   |____/     /      \  |      |     ___|
 *
 *         _    __   __   __     __         _    __   __
 *  \  /  /_\  |__| |  | |__|   |__| |__|  /_\  |__  |__
 *   \/  /   \ |    |__| | \    |    |  | /   \  __| |__
 *
 *
 *  PCB Arts GmbH
 *  Kurgartenstrasse 59
 *  90762 Fuerth
 *  GERMANY
 *
 *  www.pcb-arts.com
 *  mail(at)pcb-arts.com
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


//=================================
// included dependencies
#include "ui/cli.h"
#include "peripherals/comm_usart.h"

#include <string.h>

//=================================
// definitions

#define CLI_BUFFER_SIZE 35
#define CLI_SEND_BUFFER_SIZE 20

static volatile char vbuffer[CLI_BUFFER_SIZE];

volatile char cli_send_buffer[CLI_SEND_BUFFER_SIZE];
static volatile int send_pos, send_len, ready = 0;

int cli_line_pos = 0;
int cli_append_CRLF;

static void (*execute_cb)(const char* buf);
static int (*broadcaster_cb)(volatile char* buf);

static void receive(const char character);
static void send_next();


void cli_init() {
	//set USART receiver callback
	comm_usart_set_receiver_cb(&receive);

	//set USART send callback
	comm_usart_set_send_cb(&send_next);
}

void cli_worker() {
	if (ready) {
		char buffer[CLI_BUFFER_SIZE];

		//create non volatile copy of vbuffer
		memcpy(buffer, (char*)vbuffer, CLI_BUFFER_SIZE);

		//execute command
		if (execute_cb)
			execute_cb(buffer);

		//reset command ready flag
		ready = 0;
	}

	//if broadcast consists of multiple lines re-trigger until done
	if (cli_line_pos)
		cli_broadcast_trigger();
}

static void receive(const char character) {
	static volatile int buffer_pos;

	if (!ready){
		switch (character) {
		default:
			//check buffer limit
			if (buffer_pos < CLI_BUFFER_SIZE){
				//append character
				vbuffer[buffer_pos++] = character;
			}
			break;

		//return
		case '\r':
			//check buffer limit
			if (buffer_pos < CLI_BUFFER_SIZE){
				//append null termination
				vbuffer[buffer_pos] = '\0';
			}

			//set command ready flag
			ready = 1;

			buffer_pos = 0;
			break;

		//backspace
		case '\177':
			//check if buffer is empty
			if (buffer_pos > 0) {
				//delete last character
				vbuffer[--buffer_pos] = '\0';
			}
			break;
		}
	}
}

static void send_next() {
	//send next pending character
	if (send_pos < send_len)
		comm_usart_send(cli_send_buffer[send_pos++]);

	//disable send request
	if (send_pos == send_len)
		comm_usart_filalize_xfer();
}

void cli_broadcast_trigger() {
	//check if transmitter is ready
	if (send_pos != send_len || !broadcaster_cb)
		return;

	//reset CRLF flag
	cli_append_CRLF = 1;

	//fill buffer with broadcast values
	const int len = broadcaster_cb(cli_send_buffer);

	//check size
	if (len > 0 && len <= CLI_SEND_BUFFER_SIZE) {
		//add carriage return and new line
		if (cli_append_CRLF)
			send_len = cli_add_CRLF(cli_send_buffer, len);
		else
			send_len = len;

		//reset send pos
		send_pos = 0;

		//start transmission
		comm_usart_enable_transmitter();
	}
}

int cli_add_CRLF(volatile char* buf, int len) {
	//check if buffer space left
	if (len <= CLI_SEND_BUFFER_SIZE - 2) {
		buf[len] = '\r';
		buf[len + 1] = '\n';
		return len + 2;
	} else {
		return len;
	}
}

void cli_send_single(const char* buf, int len) {
	//check if transmitter is ready
	if (send_pos != send_len)
		return;

	//check size
	if (len > 0 && len <= CLI_SEND_BUFFER_SIZE){
		//copy buf to send buffer
		memcpy((char*)cli_send_buffer, buf, len);

		//reset send pos
		send_pos = 0;

		//send send length
		send_len = len;

		//start transmission
		comm_usart_enable_transmitter();
	}
}

int cli_starts_with(const char *str, const char *sub, int* len_sub) {
	//determine length of sub (using '\0')
	*len_sub = strlen(sub);

	//check if str starts with sub
	if(strncmp(str, sub, *len_sub) == 0)
		return 1;

	return 0;
}

void cli_set_execute_cb(void (*cb)(const char* buf)) {
	execute_cb = cb;
}

void cli_set_broadcaster_cb(int (*cb)(volatile char* buf)) {
	broadcaster_cb = cb;
}
