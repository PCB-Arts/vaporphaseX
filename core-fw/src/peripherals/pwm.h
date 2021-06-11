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


#ifndef PERIPHERALS_PWM_H_
#define PERIPHERALS_PWM_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include <stdint.h>

//=================================
// class

extern void pwm_set_FAN(int value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PERIPHERALS_PWM_H_ */
