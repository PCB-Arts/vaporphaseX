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

#ifndef __COBS_H__
#define __COBS_H__

#include <stdint.h>
#include <unistd.h>
#include <string.h>


struct cobs_decode_state {
    size_t p;
    size_t c;
};


/**
 * encodes the src buffer into a cobs encoded frame
 * @param dst pointer to destionation buffer
 * @param dstlen size of destination buffer
 * @param src pointer to source buffer
 * @param srclen size of source data
 * @return error code iff < 0 else size of encoded cobs reusult
 */
int cobs_encode(char *dst, size_t dstlen, const char *src, size_t srclen);

ssize_t cobs_decode(char *dst, size_t dstlen, const char *src, size_t srclen);


void cobs_decode_incremental_initialize(struct cobs_decode_state *state);
int cobs_decode_incremental(struct cobs_decode_state *state, char *dst, size_t dstlen, char src);

#endif//__COBS_H__
