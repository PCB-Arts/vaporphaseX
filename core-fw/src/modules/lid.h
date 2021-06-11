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


#ifndef MODULES_LID_H_
#define MODULES_LID_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/axis.h"
#include "vpo_types.h"

//=================================
// declarations

struct axis_t lid_axis;

void lid_init();
void lid_worker();
const char *lid_status();
void lid_emergency_stop();

// has to called frequently e.g. every ms
void lid_encoder_worker();

void lid_calibrate();
bool lid_overcurrent_was_detected();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_LID_H_ */
