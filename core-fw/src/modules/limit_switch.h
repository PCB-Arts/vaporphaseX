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

#ifndef MODULES_LIMIT_SWITCH_H_
#define MODULES_LIMIT_SWITCH_H_

#include "peripherals/gpio.h"

typedef struct __limit_switch_t {
	IO_pin_t pin;

    // if the pin is triggered is set to true and timestamp to current time
    // will be reset after reading of unset pin
	volatile bool triggered;
    volatile uint32_t triggered_timestamp_ms;

    // time in ms when the  after the switch is triggered
    uint32_t trigger_propagation_delay_ms;

} limit_switch_t;

void limit_switch_init(limit_switch_t* limit_switch);

// returns true when the limit switch recognizes a contact
// this might not be the current reading of the pin
bool limit_switch_active(limit_switch_t* limit_switch);

void limit_switch_triggered_isr(limit_switch_t* limit_switch);

#endif
