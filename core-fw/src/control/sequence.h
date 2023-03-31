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


#ifndef MODULES_SEQUENCE_H_
#define MODULES_SEQUENCE_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include <stdint.h>
#include <stdbool.h>
#include "com_stack/profile_com.h"

//=================================
// declarations

typedef struct{
	int currentState;

	int x;
	int y;
}currentState;

struct sequence_t {
	TemperatureProfile* profile;

	//desired galden temperature
	int galden_temp;

	//prevent condensation by keeping the PCB "warm"
	int anti_condensation_temp;

	//waypoint/step index
	int index;

	//remaining time of step
	int step_time;

	int sequence_time;

} extern sequence;

#define	SEQ_FLAG_LID_OPEN	0b0000000000000001
#define SEQ_FLAG_COOLANT_TEMP	0b0000000000000010

extern void seq_init();
extern void seq_worker();
extern void seq_seconds_sync();

extern void seq_start(TemperatureProfile* profile);
extern void seq_abort();
extern int seq_progress();
extern const char* seq_get_state_str();
extern uint16_t seq_get_flags();

extern void seq_get_limits(uint16_t* min_temp, uint16_t* max_temp,
		uint16_t* total_dur);

extern bool seq_running();
bool seq_can_abort();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_SEQUENCE_H_ */
