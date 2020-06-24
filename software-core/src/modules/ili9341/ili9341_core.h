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


#ifndef MODULES_ILI9341_CORE_H_
#define MODULES_ILI9341_CORE_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "ili9341_commands.h"
#include "peripherals/spi.h"
#include "light_hal/io.h"
#include <stdint.h>

//=================================
// declarations

#define LCD_setAddressWindowToWrite(lcd, x1,y1,x2,y2) \
    lcd_setAddressWindow(lcd, x1, y1, x2, y2); \
    ili9341_sendCmd(lcd, LCD_GRAM)

#define LCD_setAddressWindowToRead(lcd, x1,y1,x2,y2) \
    lcd_setAddressWindow(lcd, x1, y1, x2, y2); \
    ili9341_sendCmd(lcd, LCD_RAMRD)

#define ILI9341_DCX_COMMAND 0
#define ILI9341_DCX_DATA 1

struct ili9341_t {
	struct spi_dev_t spid;
	IO_pin_t DCX_pin;
	IO_pin_t reset_pin;

	uint16_t screen_width;
	uint16_t screen_height;

	uint16_t cursorX, cursorY;
	uint16_t textColor, textBgColor;
	uint8_t textSize;
	uint8_t wrap;
	uint8_t _cp437;
};

#define ILI9341_TPL(spid, dcx, reset, width, height)\
	{spid, dcx, reset, width, height, 0, 0, WHITE, BLACK, 4, 0, 0}

extern uint16_t lcd_getWidth(struct ili9341_t* lcd);
extern uint16_t lcd_getHeight(struct ili9341_t* lcd);

extern void lcd_init(struct ili9341_t* lcd);

extern void lcd_setOrientation(struct ili9341_t* lcd, uint8_t o);
extern void lcd_setAddressWindow(struct ili9341_t* lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_ILI9341_CORE_H_ */
