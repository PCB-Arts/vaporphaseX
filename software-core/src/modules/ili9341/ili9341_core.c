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
#include "ili9341_core.h"
#include "ili9341_spi.h"

//=================================
// definitions

static void lcd_reset(struct ili9341_t* lcd);
static void lcd_configure(struct ili9341_t* lcd);
static void lcd_exitStandby(struct ili9341_t* lcd);

extern void delay_ms(uint32_t d);


static const uint8_t init_commands[] =
{
		// power control A
		6, LCD_POWERA, 0x39, 0x2C, 0x00, 0x34, 0x02,

		// power control B
		4, LCD_POWERB, 0x00, 0xC1, 0x30,

		// driver timing control A
		4, LCD_DTCA, 0x85, 0x00, 0x78,

		// driver timing control B
		3, LCD_DTCB, 0x00, 0x00,

		// power on sequence control
		5, LCD_POWER_SEQ, 0x64, 0x03, 0x12, 0x81,

		// pump ratio control
		2, LCD_PRC, 0x20,

		// power control 1
		2, LCD_POWER1, 0x23,

		// power control 2
		2, LCD_POWER2, 0x10,

		// VCOM control 1
		3, LCD_VCOM1, 0x3E, 0x28,

		// VCOM cotnrol 2
		2, LCD_VCOM2, 0x86,

		// memory access control
		2, LCD_MAC, 0x48,

		// pixel format set
		2, LCD_PIXEL_FORMAT, 0x55,

		// frame rate control
		3, LCD_FRMCTR1, 0x00, 0x18,

		// display function control
		4, LCD_DFC, 0x08, 0x82, 0x27,

		// 3gamma function disable
		2, LCD_3GAMMA_EN, 0x00,

		// gamma curve selected
		2, LCD_GAMMA, 0x01,

		// set positive gamma
		16, LCD_PGAMMA, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
			0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,

		// set negative gamma
		16, LCD_NGAMMA, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
			0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,

		// EOF
		0 };


static void lcd_reset(struct ili9341_t* lcd) {
	IOPIN_WRITE(lcd->reset_pin, 0);
	delay_ms(10);
	IOPIN_WRITE(lcd->reset_pin, 1);
	delay_ms(50);
}

static void lcd_exitStandby(struct ili9341_t* lcd) {
	ili9341_sendCmd(lcd, LCD_SLEEP_OUT);
	delay_ms(150);
	ili9341_sendCmd(lcd, LCD_DISPLAY_ON);
}

static void lcd_configure(struct ili9341_t* lcd) {
	uint8_t count;
	uint8_t* address = (uint8_t*)init_commands;

	spi_start(&lcd->spid);
	while (1)
	{
		count = *(address++);
		if (count-- == 0)
			break;
		ili9341_sendCmdCont(lcd, *(address++));
		ili9341_sendDataCont8(lcd, address, count);
		address += count;
	}
	spi_stop(&lcd->spid);
	lcd_setOrientation(lcd, 0);
}

void lcd_init(struct ili9341_t* lcd) {
	lcd_reset(lcd);
	spi_set8(&lcd->spid);
	lcd_exitStandby(lcd);
	lcd_configure(lcd);
}

void lcd_setOrientation(struct ili9341_t* lcd, uint8_t o) {
	if (o == ORIENTATION_LANDSCAPE || o == ORIENTATION_LANDSCAPE_MIRROR)
	{
		//flip width and height
		lcd->screen_height = LCD_PIXEL_WIDTH;
		lcd->screen_width = LCD_PIXEL_HEIGHT;
	}
	else
	{
		lcd->screen_height = LCD_PIXEL_HEIGHT;
		lcd->screen_width = LCD_PIXEL_WIDTH;
	}

	spi_start(&lcd->spid);
	ili9341_sendCmdCont(lcd, LCD_MAC);
	ili9341_sendDataCont8(lcd, &o, 1);
	spi_stop(&lcd->spid);
}

void lcd_setAddressWindow(struct ili9341_t* lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	uint16_t pointData[2];

	spi_start(&lcd->spid);
	spi_set8(&lcd->spid);
	ili9341_sendCmdCont(lcd, LCD_COLUMN_ADDR);
	pointData[0] = x1;
	pointData[1] = x2;
	spi_set16(&lcd->spid);
	ili9341_sendDataCont16(lcd, pointData, 2);
	spi_set8(&lcd->spid);

	ili9341_sendCmdCont(lcd, LCD_PAGE_ADDR);
	pointData[0] = y1;
	pointData[1] = y2;
	spi_set16(&lcd->spid);
	ili9341_sendDataCont16(lcd, pointData, 2);
	spi_set8(&lcd->spid);
	spi_stop(&lcd->spid);
}

uint16_t lcd_getWidth(struct ili9341_t* lcd)
{
	return lcd->screen_width;
}

uint16_t lcd_getHeight(struct ili9341_t* lcd)
{
	return lcd->screen_height;
}
