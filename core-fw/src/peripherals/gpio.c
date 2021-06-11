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

#include "gpio.h"
#include "conf.h"

#if !MOCK_CORE

void iopin_write(IO_pin_t io_pin, GPIO_PinState state) {
	HAL_GPIO_WritePin(io_pin.port, io_pin.pin, state);
}

GPIO_PinState iopin_read(IO_pin_t io_pin) {
	return HAL_GPIO_ReadPin(io_pin.port, io_pin.pin);
}

#endif
