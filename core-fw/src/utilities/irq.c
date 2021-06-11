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


//=================================
// included dependencies
#include "utilities/irq.h"

#include <stdint.h>
#include <stm32f3xx.h>

//=================================
// definitions

static volatile uint32_t isr_prim;

// TODO: fix this!!!

void disable_irq() {
	//get interrupt enabled state
	isr_prim = __get_PRIMASK();

	//disable interrupts
	__disable_irq();
}

void enable_irq() {
	//enable interrupts only if they were enabled
	//before they were disabled them
	if (!isr_prim) {
		__enable_irq();
	}
}
