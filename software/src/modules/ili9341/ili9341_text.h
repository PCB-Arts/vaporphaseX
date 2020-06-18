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


#ifndef MODULES_ILI9341_TEXT_H_
#define MODULES_ILI9341_TEXT_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "ili9341_core.h"

#include <stdint.h>

//=================================
// declarations

#define TRANSPARENT_COLOR CYAN

#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))

extern void lcd_write(struct ili9341_t* lcd, unsigned char c);
extern void lcd_writeString(struct ili9341_t* lcd, unsigned char *s);

extern void lcd_setCursor(struct ili9341_t* lcd, uint16_t x, uint16_t y);
extern void lcd_setTextSize(struct ili9341_t* lcd, uint8_t size);
extern void lcd_setTextColor(struct ili9341_t* lcd, uint16_t color);
extern void lcd_setTextBgColor(struct ili9341_t* lcd, uint16_t color);

extern uint16_t lcd_getCursorX(struct ili9341_t* lcd);
extern uint16_t lcd_getCursorY(struct ili9341_t* lcd);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_ILI9341_TEXT_H_ */
