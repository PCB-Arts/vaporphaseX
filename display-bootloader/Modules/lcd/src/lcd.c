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

#include "lcd.h"

#include "ltdc.h"
#include "main.h"
#include "tim.h"

#define LCD_BYTES_PER_PIXEL 3U

/* Placed in FB_RAM at 0x20000000 by the linker script, the same place as the
 * application's TouchGFX framebuffer (and the LTDC address from the .ioc). */
static uint8_t lcd_framebuffer[LCD_WIDTH * LCD_HEIGHT * LCD_BYTES_PER_PIXEL]
    __attribute__((section("FramebufferSection"), aligned(32)));

static const sFONT *lcd_font = &Font20;
static uint32_t lcd_text_color = LCD_COLOR_WHITE;
static uint32_t lcd_back_color = LCD_COLOR_BLACK;
static bool lcd_initialized = false;

static inline void LCD_SetPixel(uint32_t x, uint32_t y, uint32_t color)
{
  /* RGB888 is stored blue first */
  uint8_t *p = &lcd_framebuffer[(y * LCD_WIDTH + x) * LCD_BYTES_PER_PIXEL];
  p[0] = color & 0xFF;
  p[1] = (color >> 8) & 0xFF;
  p[2] = (color >> 16) & 0xFF;
}

static void LCD_FillRect(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h,
                         uint32_t color)
{
  for (uint32_t y = y0; y < y0 + h && y < LCD_HEIGHT; y++)
  {
    for (uint32_t x = x0; x < x0 + w && x < LCD_WIDTH; x++)
    {
      LCD_SetPixel(x, y, color);
    }
  }
}

/* Same glyph layout as the ST Utilities/Fonts tables: one row is
 * (Width + 7) / 8 bytes, MSB first, glyphs start at ' '. */
static void LCD_DrawChar(uint32_t x0, uint32_t y0, char c)
{
  const uint32_t bytes_per_row = (lcd_font->Width + 7U) / 8U;

  if (c < ' ' || c > '~')
  {
    c = '?';
  }

  const uint8_t *glyph =
      &lcd_font->table[(uint32_t)(c - ' ') * lcd_font->Height * bytes_per_row];

  for (uint32_t row = 0; row < lcd_font->Height; row++)
  {
    uint32_t bits = 0;
    for (uint32_t b = 0; b < bytes_per_row; b++)
    {
      bits = (bits << 8) | glyph[row * bytes_per_row + b];
    }

    for (uint32_t col = 0; col < lcd_font->Width; col++)
    {
      bool set = bits & (1U << (bytes_per_row * 8U - 1U - col));
      LCD_SetPixel(x0 + col, y0 + row, set ? lcd_text_color : lcd_back_color);
    }
  }
}

void LCD_Init(void)
{
  LCD_Clear(lcd_back_color);

  HAL_LTDC_SetAddress(&hltdc, (uint32_t)lcd_framebuffer, LTDC_LAYER_1);

  HAL_GPIO_WritePin(LCD_DISP_RESET_GPIO_Port, LCD_DISP_RESET_Pin, GPIO_PIN_SET);

  /* Backlight */
  if (HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  lcd_initialized = true;
}

bool LCD_IsInitialized(void) { return lcd_initialized; }

void LCD_SetFont(const sFONT *font) { lcd_font = font; }

void LCD_SetTextColor(uint32_t color) { lcd_text_color = color; }

void LCD_SetBackColor(uint32_t color) { lcd_back_color = color; }

void LCD_Clear(uint32_t color)
{
  LCD_FillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

void LCD_DisplayStringAtLine(uint16_t line, const char *str)
{
  const uint32_t y = (uint32_t)line * lcd_font->Height;
  uint32_t x = 0;

  if (y + lcd_font->Height > LCD_HEIGHT)
  {
    return;
  }

  for (; *str && x + lcd_font->Width <= LCD_WIDTH; str++, x += lcd_font->Width)
  {
    LCD_DrawChar(x, y, *str);
  }

  LCD_FillRect(x, y, LCD_WIDTH - x, lcd_font->Height, lcd_back_color);
}
