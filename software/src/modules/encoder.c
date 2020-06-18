/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 PCB Arts GmbH - Kevin Schick
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
#include "modules/encoder.h"
#include "utilities/direction.h"
#include "limits.h"

//=================================
// definitions

static enum direction_t detect_direction(volatile struct encoder_t* enc);


void encoder_init(volatile struct encoder_t* enc) {
	//initial encoder position is 0
	enc->position = 0;

	//set invalid t1 default value so first step after init is not counted
	enc->t1 = 0xFF;
}

void encoder_step(volatile struct encoder_t* enc, unsigned int t1) {
	//move last t1 (aka. "current") to t0 (aka. "last")
	enc->t0 = enc->t1;

	//save new t1 value
	enc->t1 = t1;

	//determine rotational direction
	const enum direction_t direction = detect_direction(enc);

	//update encoder position absolute
	if (direction == FWD && enc->position < INT32_MAX)
		enc->position++;
	else if (direction == BWD && enc->position > INT32_MIN)
		enc->position--;
}

static enum direction_t detect_direction(volatile struct encoder_t* enc) {

	// FWD direction
	//           ____      ____
	//          |    |    |
	// PIN 0  __|    |____|
	//             ____      __
	//            |    |    |
	// PIN 1  ____|    |____|
	//          ^ ^  ^ ^
	// STEP     0 1  2 3
	// DEC      2 3  1 0


	// BWD direction
	//             ____      __
	//            |    |    |
	// PIN 0  ____|    |____|
	//           ____      ____
	//          |    |    |
	// PIN 1  __|    |____|
	//          ^ ^  ^ ^
	// STEP     0 1  2 3
	// DEC      1 3  2 0

	enum direction_t direction = DIRECTION_NONE;

	//decode rotational direction
	switch (enc->t1) {
	case 0u:
		if (enc->t0 == 2u) {
			direction = FWD;
		} else if (enc->t0 == 1u){
			direction = BWD;
		}
		break;

	case 1u:
		if (enc->t0 == 0u) {
			direction = FWD;
		} else if (enc->t0 == 3u) {
			direction = BWD;
		}
		break;

	case 2u:
		if (enc->t0 == 3u) {
			direction = FWD;
		} else if (enc->t0 == 0u) {
			direction = BWD;
		}
		break;

	case 3u:
		if (enc->t0 == 1u) {
			direction = FWD;
		} else if (enc->t0 == 2u) {
			direction = BWD;
		}
		break;
	}

	return direction;
}
