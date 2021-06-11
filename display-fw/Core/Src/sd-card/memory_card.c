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

#include "main.h"
#include "sd-card/memory_card.h"

#include <string.h>

FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL g_logfile;     /* File object */
FIL g_selftestLogfile;
char SDPath[4]; /* SD card logical drive path */

static int sd_open_logfile(void);
static int sd_close_logfile(void);

int sd_mount(void)
{
	/* Link the micro SD disk I/O driver */
	if (FATFS_LinkDriver(&SD_Driver, SDPath) != 0)
	{
		return -1;
	}

	/* Register the file system object to the FatFs module */
	if (f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
	{
		/* FatFs Initialization Error */
		return -1;
	}

	return sd_open_logfile();
}

int sd_unmount(void)
{
	sd_close_logfile();
	/* Unlink the RAM disk I/O driver */
	FATFS_UnLinkDriver(SDPath);
	return 0;
}

int sd_open_logfile() {
	FRESULT result = f_open(&g_logfile, LOGFILE_FILENAME, FA_WRITE | FA_OPEN_APPEND);
	if (result != FR_OK)
		return -1;
	return 0;
}

int sd_close_logfile() {
	if (f_close(&g_logfile) != FR_OK)
		return -1;
	return 0;
}

int sd_open_selftest_logfile() {
	FRESULT result = f_open(&g_selftestLogfile, SELFTEST_LOGFILE_FILENAME, FA_WRITE | FA_CREATE_ALWAYS);
	if (result != FR_OK)
		return -1;
	return 0;
}

int sd_close_selftest_logfile() {
	if (f_close(&g_selftestLogfile) != FR_OK)
		return -1;
	return 0;
}

/* Write zero-terminated string to logfile and append newline. */
int sd_log_write_line(size_t n, const char *log_line) {
	unsigned int bytes_written;
	if (f_write(&g_logfile, log_line, strnlen(log_line, n), &bytes_written) != FR_OK) {
		return -1;
	}
	if (f_write(&g_logfile, "\n", 1, &bytes_written) != FR_OK) {
		return -1;
	}
	if (f_sync(&g_logfile) != FR_OK) {
		return -1;
	}
	return 0;
}
