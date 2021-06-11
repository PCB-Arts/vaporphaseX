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
#include "lock.h"

#include <stm32f3xx.h>

//=================================
// definitions

int lock(mutex* mtx) {
	uint32_t val;

	//read mutex
	val = __LDREXW(mtx);

	//set mutex if not already set
	if (!val--) {
		//data memory barrier instruction
		__DMB();

		//write mutex
		if (0 == __STREXW(val, mtx)) {
			//data memory barrier instruction
			__DMB();

			//mutex was locked successfully
			return 1;
		}
	}

	//unable to lock mutex
	return 0;
}

void unlock(mutex* mtx) {
	//data memory barrier instruction
	__DMB();

	*mtx = 0;
}
