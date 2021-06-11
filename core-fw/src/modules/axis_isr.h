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


#ifndef MODULES_AXIS_ISR_H_
#define MODULES_AXIS_ISR_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "modules/axis.h"

//=================================
// declarations

extern void axis_limsw_fwd_isr(struct axis_t* axis);
extern void axis_limsw_bwd_isr(struct axis_t* axis);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_AXIS_ISR_H_ */
