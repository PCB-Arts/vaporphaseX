/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2026 - PCB Arts GmbH
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

#ifndef __LCD_H
#define __LCD_H

#include <stdbool.h>
#include <stdint.h>

#include "fonts.h"

/* Minimal text console on the LTDC layer 0 (800x480 RGB888), replacing the
 * STM32469I-Discovery BSP_LCD_* functions used by the F469 bootloader. */

#define LCD_WIDTH 800U
#define LCD_HEIGHT 480U

/* Colors are 0xRRGGBB */
#define LCD_COLOR_BLACK 0x000000U
#define LCD_COLOR_WHITE 0xFFFFFFU

/* Releases the panel reset, points LTDC at the framebuffer and switches the
 * backlight on. Call after MX_LTDC_Init() and MX_TIM15_Init(). */
void LCD_Init(void);
bool LCD_IsInitialized(void);

void LCD_SetFont(const sFONT *font);
void LCD_SetTextColor(uint32_t color);
void LCD_SetBackColor(uint32_t color);
void LCD_Clear(uint32_t color);
/* Draws str on text line `line` (font height units); the rest of the line is
 * cleared to the back color and characters beyond the screen are dropped. */
void LCD_DisplayStringAtLine(uint16_t line, const char *str);

#endif /* __LCD_H */
