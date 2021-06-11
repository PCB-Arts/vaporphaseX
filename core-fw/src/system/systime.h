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

#ifndef SYSTEM_SYSTIME_H_
#define SYSTEM_SYSTIME_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */

//=================================
// included dependencies
#include <stdint.h>

//=================================
// declarations

//timepoint runtime max ~179 years
struct timepoint_t {
	uint16_t micros10;
	uint16_t millis;
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint16_t days;
};

extern void systime_init();
extern void systime_worker();
void systime_slicer();

extern void systime_timepoint_reset(volatile struct timepoint_t* tp);
extern struct timepoint_t systime_get_timepoint();

//event callback´s must be implemented externally
//sync functions are executed form the main execution context

extern void systime_sync_millis(void);
extern void systime_sync_second(void);
extern void systime_sync_minute(void);
extern void systime_sync_hour(void);
extern void systime_sync_day(void);

//event callback´s must be implemented externally
//async functions are executed form the interrupt execution context

extern void systime_async_micros10(void);
extern void systime_async_millis(void);
extern void systime_async_second(void);
extern void systime_async_minute(void);
extern void systime_async_hour(void);
extern void systime_async_day(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SYSTEM_SYSTIME_H_ */
