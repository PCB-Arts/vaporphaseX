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

#include "cobs.h"

int cobs_encode(char *dst, size_t dstlen, const char *src, size_t srclen) {
    if (srclen > 254)
        return -1;

    // if srclen <= 254 we have exactly 2 bytes overhead
	uint32_t cobs_length = srclen + 2;

	if (dstlen < cobs_length) {
		return -1;
	}

    size_t p = 0;

    for (p = 0; p <= srclen; p++) {
    	char val;

    	if (p != 0 && src[p-1] != 0) {
            val = src[p-1];
        } else {
            size_t q = p;
            while (q < srclen && src[q] != 0)
                q++;
            val = (char)q-p+1;
        }

        dst[p] = val;
    }

    // delimiter
    dst[p] = 0;

    return p + 1;
}

/*@ requires \valid(dst + (0..dstlen-1));
  @ requires \valid_read(src + (0..srclen-1));
  @ requires \separated(dst + (0..dstlen-1), src + (0..srclen-1));
  @
  @ behavior maybe_valid_frame:
  @     assumes 1 <= srclen <= dstlen <= 65535;
  @     assumes \exists integer j; j > 0 && \forall integer i; 0 <= i < j ==> src[i] != 0;
  @     assumes \exists integer i; 0 <= i < srclen && src[i] == 0;
  @     assigns dst[0..dstlen-1];
  @     ensures \result >= 0 || \result == -3;
  @     ensures \result >= 0 ==> src[\result+1] == 0;
  @     ensures \result >= 0 ==> (\forall integer i; 0 <= i < \result ==> src[i] != 0);
  @
  @ behavior invalid_frame:
  @     assumes 1 <= srclen <= dstlen <= 65535;
  @     assumes src[0] == 0 || \forall integer i; 0 <= i < srclen ==> src[i] != 0;
  @     assigns dst[0..dstlen-1];
  @     ensures \result == -2;
  @
  @ behavior invalid_buffers:
  @     assumes    dstlen < 0 || dstlen > 65535
  @             || srclen < 1 || srclen > 65535
  @             || dstlen < srclen;
  @     assigns \nothing;
  @     ensures \result == -1;
  @
  @ complete behaviors;
  @ disjoint behaviors;
  @*/
ssize_t cobs_decode(char *dst, size_t dstlen, const char *src, size_t srclen) {
    if (dstlen > 65535 || srclen > 65535)
        return -1;

    if (srclen < 1)
        return -1;

    if (dstlen < srclen)
        return -1;

    size_t p = 1;
    size_t c = (unsigned char)src[0];
    //@ assert 0 <= c < 256;
    //@ assert 0 <= c;
    //@ assert c < 256;
    if (c == 0)
        return -2; /* invalid framing. An empty frame would be [...] 00 01 00, not [...] 00 00 */
    //@ assert c >= 0;
    //@ assert c != 0;
    //@ assert c <= 257;
    //@ assert c > 0;
    //@ assert c >= 0 && c != 0 ==> c > 0;

    /*@ //loop invariant \forall integer i; 0 <= i <= p ==> (i == srclen || src[i] != 0);
      @ loop invariant \forall integer i; 1 <= i < p ==> src[i] != 0;
      @ loop invariant c > 0;
      @ loop invariant 1 <= p <= srclen <= dstlen <= 65535;
      @ loop invariant \separated(dst + (0..dstlen-1), src + (0..srclen-1));
      @ loop invariant \valid_read(src + (0..srclen-1));
      @ loop invariant \forall integer i; 1 <= i <= srclen ==> \valid(dst + i - 1);
      @ loop assigns dst[0..dstlen-1], p, c;
      @ loop variant srclen-p;
      @*/
    while (p < srclen && src[p]) {
        char val;
        c--;

        //@ assert src[p] != 0;
        if (c == 0) {
            c = (unsigned char)src[p];
            val = 0;
        } else {
            val = src[p];
        }

        //@ assert 0 <= p-1 <= dstlen-1;
        dst[p-1] = val;
        p++;
    }

    if (p == srclen)
        return -2; /* Invalid framing. The terminating null byte should always be present in the input buffer. */

    if (c != 1)
        return -3; /* Invalid framing. The skip counter does not hit the end of the frame. */

    //@ assert 0 < p <= srclen <= 65535;
    //@ assert src[p] == 0;
    //@ assert \forall integer i; 1 <= i < p ==> src[i] != 0;
    return p-1;
}

void cobs_decode_incremental_initialize(struct cobs_decode_state *state) {
    state->p = 0;
    state->c = 0;
}

int cobs_decode_incremental(struct cobs_decode_state *state, char *dst, size_t dstlen, char src) {
    if (state->p == 0) {
        if (src == 0)
            goto empty_errout; /* invalid framing. An empty frame would be [...] 00 01 00, not [...] 00 00 */
        state->c = (unsigned char)src;
        state->p++;
        return 0;
    }

    if (!src) {
        if (state->c != 1)
            goto errout; /* Invalid framing. The skip counter does not hit the end of the frame. */
        int rv = state->p-1;
        cobs_decode_incremental_initialize(state);
        return rv;
    }

    char val;
    state->c--;

    if (state->c == 0) {
        state->c = (unsigned char)src;
        val = 0;
    } else {
        val = src;
    }

    size_t pos = state->p-1;
    if (pos >= dstlen)
        return -2; /* output buffer too small */
    dst[pos] = val;
    state->p++;
    return 0;

errout:
    cobs_decode_incremental_initialize(state);
    return -1;

empty_errout:
    cobs_decode_incremental_initialize(state);
    return -3;
}

#ifdef VALIDATION
/*@
  @ requires 0 <= d < 256;
  @ assigns \nothing;
  @*/
size_t test(char foo, unsigned int d) {
    unsigned int c = (unsigned char)foo;
    if (c != 0) {
        //@ assert c < 256;
        //@ assert c >= 0;
        //@ assert c != 0;
        //@ assert c > 0;
    }
    if (d != 0) {
        //@ assert d >= 0;
        //@ assert d != 0;
        //@ assert d > 0;
    }
    return c + d;
}

#include <__fc_builtin.h>

void main(void) {
    char inbuf[254];
    char cobsbuf[256];
    char outbuf[256];

    size_t range = Frama_C_interval(0, sizeof(inbuf));
    Frama_C_make_unknown((char *)inbuf, range);

    cobs_encode(cobsbuf, sizeof(cobsbuf), inbuf, sizeof(inbuf));
    cobs_decode(outbuf, sizeof(outbuf), cobsbuf, sizeof(cobsbuf));

    //@ assert \forall integer i; 0 <= i < sizeof(inbuf) ==> outbuf[i] == inbuf[i];
}
#endif//VALIDATION
