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

#include <cmsis_os.h>
#include <tinyprintf.h>

#include "log.h"
#include "sd-card/sd.hpp"

static char log_buffer_1[128];
static char log_buffer_2[128];

void vpo_component_log(const char * message, const char * component) {
	tfp_snprintf(log_buffer_2, sizeof(log_buffer_2), "%d:%s: %s", (int)osKernelGetTickCount(), component, message);

	SD_Log(log_buffer_2);
}

void vpo_log(const char* format, ...) {
  va_list format_arguments;

  va_start(format_arguments, format);

  // keep single space at the beginning
  tfp_vsnprintf(log_buffer_1, sizeof(log_buffer_1), format, format_arguments);

  va_end(format_arguments);

  vpo_component_log(log_buffer_1, "DISP");
}

