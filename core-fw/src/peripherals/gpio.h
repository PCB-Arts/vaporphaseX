/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2021 - PCB Arts GmbH
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

#ifndef GPIO_H_
#define GPIO_H_

#include "global.h"

typedef struct __IO_pin_t {
	GPIO_TypeDef* port;
	uint16_t pin;
} IO_pin_t;

#define IO_PIN_TPL(port, pin) { port, pin }

void iopin_write(IO_pin_t io_pin, GPIO_PinState state);
GPIO_PinState iopin_read(IO_pin_t io_pin);

#endif /* GPIO_H_ */
