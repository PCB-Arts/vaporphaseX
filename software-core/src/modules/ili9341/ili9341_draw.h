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


#ifndef MODULES_ILI9341_DRAW_H_
#define MODULES_ILI9341_DRAW_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "ili9341_core.h"

//=================================
// declarations

#ifndef _int16_swap
#define _int16_swap(a, b) { int16_t t = a; a = b; b = t; }
#endif

#define ILI9341_COLOR(r, g, b)\
	((((uint16_t)b) >> 3) |\
	((((uint16_t)g) << 3) & 0x07E0) |\
	((((uint16_t)r) << 8) & 0xf800))


extern void lcd_readPixels(struct ili9341_t* lcd, uint16_t x1, uint16_t y1,
		uint16_t x2, uint16_t y2, uint16_t *buf);

extern void lcd_fillRect(struct ili9341_t* lcd, uint16_t x1, uint16_t y1,
		uint16_t w, uint16_t h, uint16_t color);

extern void lcd_fillScreen(struct ili9341_t* lcd, uint16_t color);

extern void lcd_drawRect(struct ili9341_t* lcd, uint16_t x, uint16_t y,
		uint16_t w, uint16_t h, uint16_t color);

extern void lcd_putPixel(struct ili9341_t* lcd, uint16_t x, uint16_t y,
		uint16_t color);

extern void lcd_drawFastHLine(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t w, uint16_t color);

extern void lcd_drawFastVLine(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t h, uint16_t color);

extern void lcd_drawLine(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t x1, uint16_t y1, uint16_t color, uint8_t thickness);

extern void lcd_drawCircle(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t r, uint16_t color);

extern void lcd_fillCircle(struct ili9341_t* lcd, uint16_t x0, uint16_t y0,
		uint16_t r, uint16_t color);

extern void lcd_setVerticalScrolling(struct ili9341_t* lcd,
		uint16_t startY, uint16_t endY);

extern void lcd_scroll(struct ili9341_t* lcd, uint16_t v);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_ILI9341_DRAW_H_ */
