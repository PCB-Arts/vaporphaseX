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


#ifndef UI_CLI_H_
#define UI_CLI_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies

//=================================
// declarations

extern int cli_line_pos;
extern int cli_append_CRLF;

extern void cli_init();
extern void cli_worker();
extern void cli_broadcast_trigger();
extern void cli_send_single(const char* buf, int len);
extern int cli_starts_with(const char *str, const char *sub, int* len_sub);
extern int cli_add_CRLF(volatile char* buf, int len);
extern void cli_set_execute_cb(void (*cb)(const char* buf));
extern void cli_set_broadcaster_cb(int (*cb)(volatile char* buf));


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UI_CLI_H_ */
