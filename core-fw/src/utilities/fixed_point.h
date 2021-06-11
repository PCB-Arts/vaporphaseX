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


#ifndef UTILITIES_FIXED_POINT_H_
#define UTILITIES_FIXED_POINT_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include <stdint.h>

//=================================
// declarations

// basic operations with two numbers a and b of fixed point q format
// returning the answer in q format
#define FIX_ADD(a,b) ((a)+(b))
#define FIX_SUB(a,b) ((a)-(b))
#define FIX_MUL(a,b,q) (((a)*(b))>>(q))
#define FIX_DIV(a,b,q) (((a)<<(q))/(b))

// basic operations where a is of fixed point q format and b is an integer
#define FIX_ADDI(a,b,q) ((a)+((b)<<(q)))
#define FIX_SUBI(a,b,q) ((a)-((b)<<(q)))
#define FIX_MULI(a,b) ((a)*(b))
#define FIX_DIVI(a,b) ((a)/(b))

// convert a from q1 format to q2 format
#define FIX_CONV(a, q1, q2) (((q2)>(q1)) ? (a)<<((q2)-(q1)) : (a)>>((q1)-(q2)))

// general operation between a in q1 format and b in q2 format
// returning the result in q3 format
#define FIX_ADDG(a,b,q1,q2,q3) (FIX_CONV(a,q1,q3)+FIX_CONV(b,q2,q3))
#define FIX_SUBG(a,b,q1,q2,q3) (FIX_CONV(a,q1,q3)-FIX_CONV(b,q2,q3))
#define FIX_MULG(a,b,q1,q2,q3) FIX_CONV((a)*(b), (q1)+(q2), q3)
#define FIX_DIVG(a,b,q1,q2,q3) (FIX_CONV(a, q1, (q2)+(q3))/(b))

// convert to and from single precision floating point
#define FLTTOFIX(d, q) ((int)( (d)*(float)(1<<(q)) ))
#define FIXTOFLT(a, q) ( (float)(a) / (float)(1<<(q)) )


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UTILITIES_FIXED_POINT_H_ */
