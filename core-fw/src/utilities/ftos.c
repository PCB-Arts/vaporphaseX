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
#include "utilities/ftos.h"
#include "utilities/powi.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

//=================================
// definitions

static unsigned char str[20];

unsigned char *ftos(float f, int precision) {
	memset(str,0,sizeof(str));

	int a, b, c, k, l=0, m, i=0;

	// check for negetive float
	if(f < 0.0f) {
		str[i++] = '-';
		f *= -1;
	}

	a = f;    // extracting whole number
	f -= a;   // extracting decimal part

	//default whole number length
	k = 2;

	// number of digits in whole number
	while(k > 0) {
		l = powi(10, k);
		m = a/l;

		if (m < 10) {
			if(m > 0)
				break;
			k--;
		} else {
			k++;
		}
	}

	// number of digits in whole number are k+1

	/*
	extracting most significant digit i.e. right most digit , and concatenating to string
	obtained as quotient by dividing number by 10^k where k = (number of digit -1)
	*/
	for(l=k+1; l>0; l--) {
		b = powi(10, l-1);
		c = a/b;
		str[i++] = c+48;
		a %= b;
	}

	str[i++] = '.';

	/* extracting decimal digits till precision */
	for(l=0; l<precision; l++) {
		f *= 10.0f;
		b = f;
		str[i++] = b+48;
		f -= b;
	}

	str[i]='\0';
	return str;
}
