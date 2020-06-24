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


#ifndef MODULES_ILI9341_SPI_H_
#define MODULES_ILI9341_SPI_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "ili9341_core.h"

#include <stdint.h>
#include <stddef.h>

//=================================
// declarations

extern void ili9341_sendCmd(struct ili9341_t* lcd, uint8_t cmd);
extern void ili9341_sendCmdCont(struct ili9341_t* lcd, uint8_t cmd);
extern void ili9341_sendData8(struct ili9341_t* lcd, uint8_t* data, size_t len);
extern void ili9341_sendDataCont8(struct ili9341_t* lcd, uint8_t* data, size_t len);
extern void ili9341_sendData16(struct ili9341_t* lcd, uint16_t* data, size_t len);
extern void ili9341_sendDataCont16(struct ili9341_t* lcd, uint16_t* data, size_t len);
extern void ili9341_sendDataCircular16(struct ili9341_t* lcd, uint16_t* data, size_t len);

extern void ili9341_recvDataCont8(struct ili9341_t* lcd, uint8_t* data);

extern void ili9341_fill16(struct ili9341_t* lcd, uint16_t color, size_t len);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_ILI9341_SPI_H_ */
