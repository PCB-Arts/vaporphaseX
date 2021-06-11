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

#include <stdarg.h>
#include <stdio.h>

#include "packet_handler.h"
#include "log.h"

static char log_buffer_1[128];

void vpo_log(const char* format, ...) {
  va_list format_arguments;

  va_start(format_arguments, format);

  vsnprintf(log_buffer_1, sizeof(log_buffer_1), format, format_arguments);

  va_end(format_arguments);

  usart_send_log_msg(P_INFO, log_buffer_1);
}

