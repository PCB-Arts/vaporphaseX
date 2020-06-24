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
#include "ili9341_text.h"
#include "ili9341_spi.h"
#include "ili9341_draw.h"
#include "fonts/glcdfont.h"

//=================================
// definitions

static void LCD_drawChar(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		unsigned char c) {

	uint16_t scaledWidth = (uint16_t) (lcd->textSize * 6), doubleScaledWidth = scaledWidth * lcd->textSize;

	uint16_t x1 = (uint16_t) (x0 + scaledWidth - 1), y1 = (uint16_t) (y0 + 8 * lcd->textSize - 1);

	uint16_t doubleSize = lcd->textSize * lcd->textSize;
	uint16_t count = (uint16_t) (48 * doubleSize);

	uint16_t charPixels[count];

	uint16_t mx, my;
	int8_t i, j, sx, sy;
	uint8_t line;
	uint16_t pixelColor;

	if (x0 >= lcd_getWidth(lcd) || y0 >= lcd_getHeight(lcd) || x1 < 0 || y1 < 0)
		return;

	if (!lcd->_cp437 && (c >= 176))
		c++; // Handle 'classic' charset behavior

	uint16_t characterNumber = (uint16_t) (c * 5);

	if (lcd->textBgColor == TRANSPARENT_COLOR) {
		lcd_readPixels(lcd, x0, y0, x1, y1, charPixels);
	}

	LCD_setAddressWindowToWrite(lcd, x0, y0, x1, y1);

	for (i = 0; i < 6; i++) {
		line = (uint8_t) (i < 5 ? pgm_read_byte(font + characterNumber + i) : 0x0);
		my = (uint16_t) (i * lcd->textSize);

		for (j = 0; j < 8; j++, line >>= 1)
		{
			mx = (uint16_t) (j * doubleScaledWidth);

			pixelColor = line & 0x1 ? lcd->textColor : lcd->textBgColor;

			if (pixelColor == TRANSPARENT_COLOR)
				continue;

			for (sx = 0; sx < lcd->textSize; ++sx) {
				for (sy = 0; sy < lcd->textSize; ++sy) {
					charPixels[mx + my + sy * scaledWidth + sx] = pixelColor;
				}
			}
		}
	}

	spi_set16(&lcd->spid);
	ili9341_sendData16(lcd, charPixels, count);
	spi_set8(&lcd->spid);
}

void lcd_write(struct ili9341_t* lcd, unsigned char c)  {
	if (c == '\n') {
		lcd->cursorY += lcd->textSize * 8;
		lcd->cursorX = 0;
	} else if (c == '\r') {
		lcd->cursorX = 0;
	} else {
		if (lcd->wrap && ((lcd->cursorX + lcd->textSize * 6) >= lcd_getWidth(lcd))) { // Heading off edge?
			lcd->cursorX = 0;            // Reset x to zero
			lcd->cursorY += lcd->textSize * 8; // Advance y one line
		}
		LCD_drawChar(lcd, lcd->cursorX, lcd->cursorY, c);
		lcd->cursorX += lcd->textSize * 6;
	}
}

void lcd_writeString(struct ili9341_t* lcd, unsigned char *s) {
	while (*(s))
		lcd_write(lcd, *s++);
}

void lcd_setCursor(struct ili9341_t* lcd, uint16_t x, uint16_t y) {
	lcd->cursorX = x;
	lcd->cursorY = y;
}

void lcd_setTextSize(struct ili9341_t* lcd, uint8_t size) {
	lcd->textSize = size;
}

void lcd_setTextColor(struct ili9341_t* lcd, uint16_t color) {
	lcd->textColor = color;
}

void lcd_setTextBgColor(struct ili9341_t* lcd, uint16_t color) {
	lcd->textBgColor = color;
}

uint16_t lcd_getCursorX(struct ili9341_t* lcd) {
	return lcd->cursorX;
}

uint16_t lcd_getCursorY(struct ili9341_t* lcd) {
	return lcd->cursorY;
}
