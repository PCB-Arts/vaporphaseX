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

#ifndef __MEMORYCARD_H
#define __MEMORYCARD_H


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"

#define LOGFILE_FILENAME "VaporPhaseOne.log"
#define SELFTEST_LOGFILE_FILENAME "Selftest.log"
extern FIL g_logfile;
extern FIL g_selftestLogfile;

int sd_mount(void);
int sd_unmount(void);

int sd_open_selftest_logfile();
int sd_close_selftest_logfile();

/* Write zero-terminated string to logfile and append newline.
 * n - buffer size of log_line, passed to strnlen. */
int sd_log_write_line(size_t n, const char *log_line);

#ifdef __cplusplus
}
#endif

#endif  /* __MEMORYCARD_H */
