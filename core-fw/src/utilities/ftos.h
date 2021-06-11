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


#ifndef UTILITIES_FTOS_H_
#define UTILITIES_FTOS_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies

//=================================
// declarations

extern unsigned char *ftos(float f, int precision);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UTILITIES_FTOS_H_ */
