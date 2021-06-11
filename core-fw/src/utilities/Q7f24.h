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


#ifndef UTILITIES_Q7F24_H_
#define UTILITIES_Q7F24_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include <stdint.h>

//=================================
// declarations

#define Q7f24_FRACTION 24

//32 bit fixed point type with 7 bit integer and 24 bit fractional
typedef int32_t Q7f24;

#define FLTTOQ7f24(d) ((Q7f24)( (d)*(float)(1<<(Q7f24_FRACTION)) ))
#define Q7f24TOFLT(a) ( (float)(a) / (float)(1<<(Q7f24_FRACTION)) )


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UTILITIES_Q7F24_H_ */
