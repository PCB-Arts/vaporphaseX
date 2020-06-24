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
#include "ili9341_draw.h"
#include "ili9341_spi.h"

#include <stdlib.h>

//=================================
// definitions

static void LCD_putPixelCont(struct ili9341_t* lcd, uint16_t x, uint16_t y, uint16_t color);
static void LCD_fillCircleHelper(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t r, uint8_t cornername, int16_t delta, uint16_t color);


void lcd_readPixels(struct ili9341_t* lcd, uint16_t x1, uint16_t y1,
		uint16_t x2, uint16_t y2, uint16_t *buf) {

	uint8_t red, green, blue;
	uint32_t count = (uint32_t) ((x2 - x1 + 1) * (y2 - y1 + 1));

	LCD_setAddressWindowToRead(lcd, x1, y1, x2, y2);

	IOPIN_WRITE(lcd->DCX_pin, ILI9341_DCX_DATA);

	spi_start(&lcd->spid);
	ili9341_recvDataCont8(lcd, &red);

	for (uint32_t i = 0; i < count; ++i)
	{
		ili9341_recvDataCont8(lcd, &red);
		ili9341_recvDataCont8(lcd, &green);
		ili9341_recvDataCont8(lcd, &blue);

		buf[i] = (uint16_t) ILI9341_COLOR(red, green, blue);
	}
	spi_stop(&lcd->spid);
}

void lcd_fillRect(struct ili9341_t* lcd, uint16_t x1, uint16_t y1,
		uint16_t w, uint16_t h, uint16_t color) {

	uint32_t count = w * h;
	LCD_setAddressWindowToWrite(lcd, x1, y1, (uint16_t) (x1 + w - 1),
			(uint16_t) (y1 + h - 1));
	ili9341_fill16(lcd, color, count);
}

void lcd_fillScreen(struct ili9341_t* lcd, uint16_t color) {
	lcd_fillRect(lcd, 0, 0, lcd_getWidth(lcd), lcd_getHeight(lcd), color);
}

void lcd_drawRect(struct ili9341_t* lcd, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, uint16_t color) {

	if (w == 0 || h == 0)
		return;

	if (w == 1) {
		lcd_drawFastVLine(lcd, x, y, h, color);
		return;
	}

	if (h == 1) {
		lcd_drawFastHLine(lcd, x, y, w, color);
		return;
	}

	lcd_drawFastHLine(lcd, x, y, w, color);
	lcd_drawFastHLine(lcd, x, (uint16_t) (y + h - 1), w, color);
	lcd_drawFastVLine(lcd, x, y, h, color);
	lcd_drawFastVLine(lcd, (uint16_t) (x + w - 1), y, h, color);
}

void lcd_putPixel(struct ili9341_t* lcd, uint16_t x, uint16_t y,
		uint16_t color) {

	LCD_setAddressWindowToWrite(lcd, x, y, x, y);
	ili9341_fill16(lcd, color, 1);
}

static void LCD_putPixelCont(struct ili9341_t* lcd, uint16_t x, uint16_t y, uint16_t color)
{
	LCD_setAddressWindowToWrite(lcd, x, y, x, y);
	ili9341_fill16(lcd, color, 1);
}

void lcd_drawFastHLine(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t w, uint16_t color) {

	if (w == 1) {
		lcd_putPixel(lcd, x0, y0, color);
		return;
	}
	lcd_fillRect(lcd, x0, y0, w, 1, color);
}

void lcd_drawFastVLine(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t h, uint16_t color) {

	if (h == 1) {
		lcd_putPixel(lcd, x0, y0, color);
		return;
	}
	lcd_fillRect(lcd, x0, y0, 1, h, color);
}

void lcd_drawLine(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t x1, uint16_t y1, uint16_t color, uint8_t thickness) {

	int16_t Dx = (int16_t) abs(x1 - x0);
	int16_t Dy = (int16_t) abs(y1 - y0);

	if (Dx == 0 && Dy == 0) {
		lcd_putPixel(lcd, x0, y0, color);
		return;
	}

	int16_t steep = Dy > Dx;
	int16_t dx, dy, err, yStep;

	if (steep) {
		_int16_swap(x0, y0);
		_int16_swap(x1, y1);
	}

	if (x0 > x1) {
		_int16_swap(x0, x1);
		_int16_swap(y0, y1);
	}

	dx = x1 - x0;
	dy = (int16_t) abs(y1 - y0);

	err = (int16_t) (dx / 2);

	if (y0 < y1) {
		yStep = 1;
	} else {
		yStep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			for (int thick = 0; thick < thickness; ++thick) {
				lcd_putPixel(lcd, y0 + thick, x0, color);
			}
		} else {
			for (int thick = 0; thick < thickness; ++thick) {
				lcd_putPixel(lcd, x0, y0 + thick, color);
			}
		}

		err -= dy;
		if (err < 0) {
			y0 += yStep;
			err += dx;
		}
	}
}

void lcd_drawCircle(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t r, uint16_t color) {

	if (r == 0) {
		lcd_putPixel(lcd, x0, y0, color);
		return;
	}

	int16_t f = (int16_t) (1 - r);
	int16_t dx = 1;
	int16_t dy = (int16_t) (-2 * r);
	int16_t x = 0;

	uint16_t y = r;

	LCD_putPixelCont(lcd, x0, y0 + r, color);
	LCD_putPixelCont(lcd, x0, y0 - r, color);
	LCD_putPixelCont(lcd, x0 + r, y0, color);
	LCD_putPixelCont(lcd, x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			dy += 2;
			f += dy;
		}
		x++;
		dx += 2;
		f += dx;

		LCD_putPixelCont(lcd, x0 + x, y0 + y, color);
		LCD_putPixelCont(lcd, x0 - x, y0 + y, color);
		LCD_putPixelCont(lcd, x0 + x, y0 - y, color);
		LCD_putPixelCont(lcd, x0 - x, y0 - y, color);
		LCD_putPixelCont(lcd, x0 + y, y0 + x, color);
		LCD_putPixelCont(lcd, x0 - y, y0 + x, color);
		LCD_putPixelCont(lcd, x0 + y, y0 - x, color);
		LCD_putPixelCont(lcd, x0 - y, y0 - x, color);
	}
}

// Used to do circles and roundrects
static void LCD_fillCircleHelper(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t r, uint8_t cornername, int16_t delta, uint16_t color) {

	if (r == 0)
		return;
	if (r == 1) {
		lcd_putPixel(lcd, x0, y0, color);
		return;
	}

	int16_t f = (int16_t) (1 - r);
	int16_t dx = 1;
	int16_t dy = (int16_t) (-2 * r);
	int16_t x = 0;

	uint16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			dy += 2;
			f += dy;
		}

		x++;
		dx += 2;
		f += dx;

		if (cornername & 0x1) {
			lcd_drawFastVLine(lcd, x0 + x, y0 - y, (uint16_t) (2 * y + 1 + delta), color);
			lcd_drawFastVLine(lcd, x0 + y, y0 - x, (uint16_t) (2 * x + 1 + delta), color);
		}
		if (cornername & 0x2) {
			lcd_drawFastVLine(lcd, x0 - x, y0 - y, (uint16_t) (2 * y + 1 + delta), color);
			lcd_drawFastVLine(lcd, x0 - y, y0 - x, (uint16_t) (2 * x + 1 + delta), color);
		}
	}
}

void lcd_fillCircle(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t r, uint16_t color) {

	lcd_drawFastVLine(lcd, x0, y0 - r, (uint16_t) (2 * r + 1), color);
	LCD_fillCircleHelper(lcd, x0, y0, r, 3, 0, color);
}

void lcd_setVerticalScrolling(struct ili9341_t* lcd,
		uint16_t startY, uint16_t endY) {


	ili9341_sendCmd(lcd, LCD_VSCRDEF);
	uint16_t d[] =
	{ startY, (uint16_t) (LCD_PIXEL_HEIGHT - startY - endY), endY };

	spi_set16(&lcd->spid);
	ili9341_sendData16(lcd, d, 3);
	spi_set8(&lcd->spid);
}

void lcd_scroll(struct ili9341_t* lcd, uint16_t v) {
	ili9341_sendCmd(lcd, LCD_VSCRSADD);
	spi_set16(&lcd->spid);
	ili9341_sendData16(lcd, &v, 1);
	spi_set8(&lcd->spid);
}
