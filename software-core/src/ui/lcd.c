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
#include "lcd.h"
#include "peripherals/spi.h"
#include "modules/ili9341/ili9341_core.h"
#include "modules/ili9341/ili9341_spi.h"
#include "modules/ili9341/ili9341_draw.h"
#include "modules/ili9341/ili9341_text.h"
#include "modules/sequence.h"
#include "hardware_declarations.h"
#include "light_hal/io.h"

#include <stdlib.h>
#include <stdio.h>

//=================================
// definitions

#define VERSION "version: 0.0.0 - Git:a0b6a51790bd"
#define REPO_URL "https://github.com/PCB-Arts/vaporphaseone"
#define BOOT_MSG_MS 3000
#define MAX_TEMP_ADD 20


struct ili9341_t lcd_top = ILI9341_TPL(\
		/*spi device*/
		SPI_DEV_TPL(
		IO_PIN_TPL(LCD_CS_PORT, LCD_CS_PIN),
		&spi2),\

		/*DCX pin*/
		IO_PIN_TPL(LCD_DCX_PORT, LCD_DCX_PIN),\

		/*reset pin*/
		IO_PIN_TPL(LCD_RESET_PORT, LCD_RESET_PIN),\

		/*LCD size*/
		LCD_PIXEL_WIDTH, LCD_PIXEL_HEIGHT);

enum {
	LCD_INIT,
	LCD_BOOT_MSG_DRAW,
	LCD_BOOT_MSG_WAIT,
	LCD_IDLE
} lcd_state = LCD_INIT;

uint32_t screen_delay = 0;

struct grid_t {
	uint16_t color;
	uint8_t x, y;
};

struct conv_t {
	float g;
	int16_t o;
};

struct chart_t {
	uint16_t x, y, h, w;
	uint16_t color;
	struct grid_t grid;
	struct conv_t cx;
	struct conv_t cy;
};

struct chart_t chart;

static void init_top();
static void about_screen();
static void draw_fullscreen_chart();
static void clear_chart(struct chart_t* c);
static void draw_chart_frame(struct chart_t* c);
static void draw_grid(struct chart_t* c);
static void draw_deg_c(uint16_t x, uint16_t y);
static void draw_time_s(uint16_t x, uint16_t y);
static void draw_seq(struct chart_t* c);

static void calculate_conv_factors(struct chart_t* c);
static uint16_t conv_to_pixel(struct conv_t* conv, uint16_t val);
static uint16_t conv_to_value(struct conv_t* conv, uint16_t pixel);

static void init_top() {
	lcd_init(&lcd_top);
	lcd_setOrientation(&lcd_top, ORIENTATION_LANDSCAPE_MIRROR);
	lcd_fillScreen(&lcd_top, BLACK);
	lcd_setTextBgColor(&lcd_top, BLACK);
	lcd_setTextSize(&lcd_top, 2);
	lcd_setTextColor(&lcd_top, WHITE);
}

void ui_lcd_millis_sync() {
	if (screen_delay) screen_delay--;
}

void ui_lcd_worker() {
	switch (lcd_state) {
	case LCD_INIT:
		if (spi_lock(&lcd_top.spid)) {
			spi_mode(&lcd_top.spid, 0, 0);
			init_top();
			spi_unlock(&lcd_top.spid);
			lcd_state = LCD_BOOT_MSG_DRAW;
		}
		break;

	case LCD_BOOT_MSG_DRAW:
		if (spi_lock(&lcd_top.spid)) {
			spi_mode(&lcd_top.spid, 0, 0);
			about_screen();
			spi_unlock(&lcd_top.spid);
			screen_delay = BOOT_MSG_MS;
			lcd_state = LCD_BOOT_MSG_WAIT;
		}
		break;

	case LCD_BOOT_MSG_WAIT:
		if (!screen_delay) {
			draw_fullscreen_chart();
			lcd_state = LCD_IDLE;
		}
		break;

	case LCD_IDLE:

		break;
	}
}

static void about_screen() {
	char tmpstr[100];

	lcd_setTextSize(&lcd_top, 2);

	lcd_setCursor(&lcd_top, 75, 80);
	sprintf(tmpstr, "Vapor Phase One");
	lcd_writeString(&lcd_top, (uint8_t*)tmpstr);

	lcd_setCursor(&lcd_top, 100, 105);
	sprintf(tmpstr, "by PCB Arts");
	lcd_writeString(&lcd_top, (uint8_t*)tmpstr);

	lcd_setTextSize(&lcd_top, 1);

	lcd_setCursor(&lcd_top, 20, 210);
	sprintf(tmpstr, VERSION);
	lcd_writeString(&lcd_top, (uint8_t*)tmpstr);

	lcd_setCursor(&lcd_top, 20, 220);
	sprintf(tmpstr, REPO_URL);
	lcd_writeString(&lcd_top, (uint8_t*)tmpstr);
}

static void draw_fullscreen_chart() {
	chart.x = 30;
	chart.y = 20;
	chart.h = 200;
	chart.w = 280;
	chart.color = NAVY;
	draw_chart_frame(&chart);
	clear_chart(&chart);

	calculate_conv_factors(&chart);

	//draw_deg_c(5, 210);
	//draw_time_s(15, 230);

	chart.grid.x = 7;
	chart.grid.y = 5;
	chart.grid.color = DGRAY;
	draw_grid(&chart);

	draw_seq(&chart);
}

static void clear_chart(struct chart_t* c) {
	lcd_fillRect(&lcd_top, c->x, c->y, c->w, c->h, c->color);
}

static void draw_chart_frame(struct chart_t* c) {
	uint16_t horzw;
	horzw = lcd_getWidth(&lcd_top);

	//top border
	uint16_t tbx, tby, tbh;
	tbx = 0;
	tby = 0;
	tbh = c->y;
	lcd_fillRect(&lcd_top, tbx, tby, horzw, tbh, BLACK);

	//bottom border
	uint16_t bbx, bby, bbh;
	bbx = 0;
	bby = c->y + c->h;
	bbh = lcd_getHeight(&lcd_top) - (c->y + c->h);
	lcd_fillRect(&lcd_top, bbx, bby, horzw, bbh, BLACK);

	uint16_t verth;
	verth = lcd_getHeight(&lcd_top) - (tbh + bbh);

	//left border
	uint16_t lbx, lby, lbw;
	lbx = 0;
	lby = tbh;
	lbw = c->x;
	lcd_fillRect(&lcd_top, lbx, lby, lbw, verth, BLACK);

	//right border
	uint16_t rbx, rby, rbw;
	rbx = c->x + c->w;
	rby = tbh;
	rbw = lcd_getWidth(&lcd_top) - (c->x + c->w);
	lcd_fillRect(&lcd_top, rbx, rby, rbw, verth, BLACK);
}

static void draw_grid(struct chart_t* c) {
	uint16_t dx = c->w / c->grid.x;
	uint16_t dy = c->h / c->grid.y;
	uint16_t height = lcd_getHeight(&lcd_top);
	char tmpstr[5];

	for (int i = 0; i < c->grid.x + 1; ++i) {
		uint16_t x = i * dx + c->x;

		lcd_drawFastVLine(&lcd_top, x, c->y, dy * c->grid.y,
				c->grid.color);

		if (i < c->grid.x) {
			lcd_setCursor(&lcd_top, x, c->y + c->h + 5);
			sprintf(tmpstr, "%u", conv_to_value(&c->cx, x));
			lcd_writeString(&lcd_top, (uint8_t*)tmpstr);
		}
	}

	for (int i = 0; i < c->grid.y + 1; ++i) {
		uint16_t y = i * dy + c->y;

		lcd_drawFastHLine(&lcd_top, c->x, y, dx * c->grid.x,
				c->grid.color);

		lcd_setCursor(&lcd_top, c->x - 20, y - 4);
		sprintf(tmpstr, "%u", conv_to_value(&c->cy, y) + 1);
		lcd_writeString(&lcd_top, (uint8_t*)tmpstr);
	}
}

static void draw_deg_c(uint16_t x, uint16_t y) {
	char tmpstr[2];
	lcd_setTextSize(&lcd_top, 1);

	lcd_setCursor(&lcd_top, x, y);
	sprintf(tmpstr, "o");
	lcd_writeString(&lcd_top, (uint8_t*)tmpstr);

	lcd_setCursor(&lcd_top, x + 7, y + 2);
	sprintf(tmpstr, "C");
	lcd_writeString(&lcd_top, (uint8_t*)tmpstr);
}

static void draw_time_s(uint16_t x, uint16_t y) {
	char tmpstr[5] = "t(s)";
	lcd_setTextSize(&lcd_top, 1);
	lcd_setCursor(&lcd_top, x, y);
	lcd_writeString(&lcd_top, (uint8_t*)tmpstr);
}

static void draw_seq(struct chart_t* c) {
	uint16_t dur = 0;

	for (int i_seq = 0; i_seq < SEQ_STEPS - 1; ++i_seq) {

		int32_t dx = sequence.steps[i_seq][SEQ_DUR];
		int32_t y0 = sequence.steps[i_seq][SEQ_TEMP];
		int32_t y1 = sequence.steps[i_seq + 1][SEQ_TEMP];

		//exit loop if duration is zero
		if (!dx)
			break;

		//calculate temperature slope between the current and the next step
		const float g = ((float)(y1 - y0)) / dx;

		for (int time = 0; time < dx; ++time) {
			//calculate linear interpolated temperature
			uint16_t temp0 = (time * g + y0);
			uint16_t temp1 = ((time + 1) * g + y0);

			uint16_t px0 = conv_to_pixel(&c->cx, dur + time);
			uint16_t px1 = conv_to_pixel(&c->cx, dur + time + 1);
			uint16_t py0 = conv_to_pixel(&c->cy, temp0);
			uint16_t py1 = conv_to_pixel(&c->cy, temp1);

			lcd_drawLine(&lcd_top, px0, py0, px1, py1, YELLOW, 2);
		}

		dur += dx;
	}
}

static void calculate_conv_factors(struct chart_t* c) {
	uint16_t tmax, tmin, tdur;
	seq_get_limits(&tmin, &tmax, &tdur);
	tmax += MAX_TEMP_ADD;

	c->cx.g = (float)c->w / tdur;
	c->cx.o = c->x;

	c->cy.g = ((float)c->h / (tmax - tmin)) * (-1);
	c->cy.o = c->h - ((c->y - c->cy.g * tmin) * (-1));
}

static uint16_t conv_to_pixel(struct conv_t* conv, uint16_t val) {
	return val * conv->g + conv->o;
}

static uint16_t conv_to_value(struct conv_t* conv, uint16_t pixel) {
	return (pixel - conv->o) / conv->g;
}
