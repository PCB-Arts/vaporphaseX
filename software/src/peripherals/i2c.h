/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 - PCB Arts GmbH - Kevin Schick
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


#ifndef PERIPHERALS_I2C_H_
#define PERIPHERALS_I2C_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include <stdint.h>

//=================================
// class

#define I2C_READ 0x01
#define I2C_WRITE 0x00

struct I2C_device_t {
	uint8_t (*process)(void);
	void (*isr)(void);
	void (*reset)(void);
};

struct I2C_set_t {
	const uint8_t count;
	const struct I2C_device_t devices[];
};

extern volatile uint8_t I2C_watchdog;

extern void I2C_init(volatile struct I2C_set_t* set);
extern void I2C_reconfigure();

extern void I2C_worker();
extern void I2C_reset();
extern void I2C_millis_event();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PERIPHERALS_I2C_H_ */
