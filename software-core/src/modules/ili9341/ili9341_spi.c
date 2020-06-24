/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2020 PCB Arts GmbH - Kevin Schick
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
#include "ili9341_spi.h"
#include "light_hal/io.h"

//=================================
// definitions

void ili9341_sendCmd(struct ili9341_t* lcd, uint8_t cmd) {
	spi_start(&lcd->spid);
	ili9341_sendCmdCont(lcd, cmd);
	spi_stop(&lcd->spid);
}

void ili9341_sendCmdCont(struct ili9341_t* lcd, uint8_t cmd) {
	IOPIN_WRITE(lcd->DCX_pin, ILI9341_DCX_COMMAND);
	spi_send8(&lcd->spid, &cmd, 1);
	spi_wait(&lcd->spid);
}

void ili9341_sendData8(struct ili9341_t* lcd, uint8_t* data, size_t len) {
	spi_start(&lcd->spid);
	ili9341_sendData8(lcd, data, len);
	spi_stop(&lcd->spid);
}

void ili9341_sendDataCont8(struct ili9341_t* lcd, uint8_t* data, size_t len) {
	IOPIN_WRITE(lcd->DCX_pin, ILI9341_DCX_DATA);
	spi_send8(&lcd->spid, data, len);
	spi_wait(&lcd->spid);
}

void ili9341_sendData16(struct ili9341_t* lcd, uint16_t* data, size_t len) {
	spi_start(&lcd->spid);
	ili9341_sendDataCont16(lcd, data, len);
	spi_stop(&lcd->spid);
}

void ili9341_sendDataCont16(struct ili9341_t* lcd, uint16_t* data, size_t len) {
	IOPIN_WRITE(lcd->DCX_pin, ILI9341_DCX_DATA);
	spi_send16(&lcd->spid, data, len);
	spi_wait(&lcd->spid);
}

void ili9341_sendDataCircular16(struct ili9341_t* lcd, uint16_t* data, size_t len) {
	IOPIN_WRITE(lcd->DCX_pin, ILI9341_DCX_DATA);
	spi_sendCircular16(&lcd->spid, data, len);
	spi_wait(&lcd->spid);
}

void ili9341_recvDataCont8(struct ili9341_t* lcd, uint8_t* data) {
	uint8_t dummy = 0xFF;
	IOPIN_WRITE(lcd->DCX_pin, ILI9341_DCX_DATA);
	spi_xfer8(&lcd->spid, &dummy, data, 1);
	spi_wait(&lcd->spid);
}

void ili9341_fill16(struct ili9341_t* lcd, uint16_t color, size_t len) {
	spi_start(&lcd->spid);
	ili9341_sendCmdCont(lcd, LCD_GRAM);
	spi_set16(&lcd->spid);
	while (len != 0)
	{
		uint16_t ts = (uint16_t) (len > UINT16_MAX ? UINT16_MAX : len);
		ili9341_sendDataCircular16(lcd, &color, ts);
		len -= ts;
	}
	spi_set8(&lcd->spid);
	spi_stop(&lcd->spid);
}
