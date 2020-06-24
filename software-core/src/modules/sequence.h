/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2020 PCB Arts GmbH - Kevin Schick
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  _____    ____    ____
 *       \  /       |    \        /\            |
 *   ____/  |       |____/       /  \     __  __|__   ___
 *  |       |       |    \      /____\   /      |    |___
 *  |       \____   |____/     /      \  |      |     ___|
 *
 *         _    __   __   __     __         _    __   __
 *  \  /  /_\  |__| |  | |__|   |__| |__|  /_\  |__  |__
 *   \/  /   \ |    |__| | \    |    |  | /   \  __| |__
 *
 *
 *  PCB Arts GmbH
 *  Kurgartenstrasse 59
 *  90762 Fuerth
 *  GERMANY
 *
 *  www.pcb-arts.com
 *  mail(at)pcb-arts.com
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


#ifndef MODULES_SEQUENCE_H_
#define MODULES_SEQUENCE_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include <stdint.h>

//=================================
// declarations

#define SEQ_STEPS 10
#define SEQ_TEMP 0
#define SEQ_DUR 1

struct sequence_t {
	//waypoints/steps in the soldering process
	//step[n][0] = temperature in C [0 .. 65535]
	//step[n][1] = duration in seconds [0 .. 65535]
	unsigned int steps[SEQ_STEPS][2];

	//desired galden temperature
	float galden_temp;

	//prevent condensation by keeping the PCB "warm"
	float anti_condensation_temp;

	//waypoint/step index
	int index;

	//remaining time of step
	int step_time;
} extern sequence;

#define	SEQ_FLAG_LID_OPEN	0b0000000000000001
#define SEQ_FLAG_COOLANT_TEMP	0b0000000000000010

extern void seq_worker();
extern void seq_seconds_sync();

extern void seq_start();
extern void seq_stop();
extern void seq_continue();
extern void seq_abort();
extern int seq_progress();
extern const char* seq_get_state_str();
extern uint16_t seq_get_flags();

extern void seq_get_limits(uint16_t* min_temp, uint16_t* max_temp,
		uint16_t* total_dur);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MODULES_SEQUENCE_H_ */
