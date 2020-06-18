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


//=================================
// included dependencies
#include "modules/sequence.h"
#include "modules/regulator.h"
#include "modules/temp_sensors.h"
#include "modules/heater.h"
#include "modules/fan.h"
#include "modules/fans.h"
#include "modules/axis.h"
#include "modules/lift.h"
#include "modules/lid.h"

//=================================
// definitions

enum {
	SEQ_IDLE, //not running
	SEQ_SAFETY_CHECK, //lid closed?
	SEQ_GENERATE_VAPOUR, //bring the galden up to temperature
	SEQ_PRE_HEAT, //bring the PCB up to temperature
	SEQ_RUN, //run temperature profile
	SEQ_ABORT, //abort sequence, shut-off heater and start coolers
	SEQ_COOL_DOWN //wait until the galden has cooled down
} static seq_state = SEQ_IDLE;

static uint16_t flags;

struct sequence_t sequence;
static int running;
static void seq_fsm();
static int safety_checks();
static void start_cool_down();
static int next_step();
static void update_regulator();
static void try_interpolation();
static int calc_interpolaion();
static void condensate_prevention();
static void fans_run_mode();
static void fans_cool_mode();
static void raise_lift();
static void setup_heater();


void seq_worker() {
	if (running) {
		seq_fsm();
	}
}

void seq_seconds_sync() {
	if (running) {
		if (sequence.step_time) {
			sequence.step_time--;
			try_interpolation();
		}

		//perform checks => abort on fail
		if (seq_state != SEQ_COOL_DOWN && !safety_checks()) {
			seq_state = SEQ_ABORT;
		}
	}
}

static void seq_fsm() {
	switch (seq_state) {
	default:
	case SEQ_IDLE:
		break;

	case SEQ_SAFETY_CHECK:
		if (safety_checks()) {
			setup_heater();
			raise_lift();
			fans_run_mode();
			seq_state = SEQ_GENERATE_VAPOUR;
		} else {
			seq_state = SEQ_IDLE;
			running = 0;
		}
		break;

	case SEQ_GENERATE_VAPOUR:
		//wait until the heater was able to generate vapour
		if (heater_temperature_reached()) {
			//get temperature and duration of the first step
			update_regulator();
			regulator_enable();
			seq_state = SEQ_PRE_HEAT;
		}
		break;

	case SEQ_PRE_HEAT:
		//wait until the regulator was able to reach the
		//target temperature of the first step
		if (regulator_temperature_reached()) {
			update_regulator();
			seq_state = SEQ_RUN;
		}
		break;

	case SEQ_RUN:
		if (!sequence.step_time) {
			if (next_step()) {
				sequence.index++;
				update_regulator();
			} else {
				start_cool_down();
			}

		}
		break;

	case SEQ_ABORT:
		start_cool_down();
		break;

	case SEQ_COOL_DOWN:
		//wait until the chamber has cooled down and the vapour condensated
		if (TSNS_0.temperature < 80) {

			raise_lift();
			fans_run_mode();
			seq_state = SEQ_IDLE;
			running = 0;
		}
		break;
	}
}

static int safety_checks() {
	int rc = 1;

	//check axis calibration was performed
	if (!lid_axis.cal_done || !lift_axis.cal_done)
		rc = 0;

	//check if lid is closed
	if (lid_axis.encoder->position > 200)
		rc = 0;

	//check cooling circuit temperature
	if (TSNS_4.temperature > 60)
		rc = 0;

	return rc;
}

static int next_step() {
	const int next = sequence.index + 1;

	//end sequence at max or at first zero duration
	return (next < SEQ_STEPS && sequence.steps[next][SEQ_DUR] ? 1 : 0);
}

static void try_interpolation() {
	if (next_step()) {
		int temp = calc_interpolaion();
		if (temp)
			regulator_set_temp(temp);
	}
}

static int calc_interpolaion() {
	if (sequence.index + 1 >= SEQ_STEPS)
		return 0;

	const int dx = sequence.steps[sequence.index][SEQ_DUR];
	const int xp = sequence.step_time;
	const int y0 = sequence.steps[sequence.index][SEQ_TEMP];
	const int y1 = sequence.steps[sequence.index + 1][SEQ_TEMP];

	//calculate temperature slope between the current and the next step
	const float g = ((float)(y1 - y0)) / dx;

	//calculate linear interpolated temperature
	return (((dx - xp) * g) + y0);
}
static void condensate_prevention() {
	//prevent condensation by keeping the PCB "warm"
	regulator_set_temp(sequence.anti_condensation_temp);
	regulator_enable();
}

static void start_cool_down() {
	seq_state = SEQ_COOL_DOWN;
	heater_regulation_disable();
	heater_disable();
	condensate_prevention();
	fans_cool_mode();
}

static void fans_run_mode() {
	//set cooling fans 60%
	hydro_cooler.mode = FAN_MODE_MANUAL;
	fan_set_level(&hydro_cooler, 60);

	//disable quick cool fans
	quick_cooler.mode = FAN_MODE_MANUAL;
	fan_set_level(&quick_cooler, 0);
}

static void fans_cool_mode() {
	//set cooling fans 100%
	hydro_cooler.mode = FAN_MODE_MANUAL;
	fan_set_level(&hydro_cooler, 100);

	//enable quick cool fans
	quick_cooler.mode = FAN_MODE_MANUAL;
	fan_set_level(&quick_cooler, 100);
}

static void raise_lift() {
	//disable lift height/temperature regulation
	regulator_disable();

	//raise lift to top
	axis_move_to(&lift_axis, 0);
}

static void setup_heater() {
	heater_set_temperature(sequence.galden_temp);
	heater_regulation_enable();
}

static void update_regulator() {
	//check sequence limits, max steps
	if (sequence.index >= SEQ_STEPS)
		return;

	//reload step countdown timer
	sequence.step_time = sequence.steps[sequence.index][SEQ_DUR];

	//update regulator temperature
	regulator_set_temp(sequence.steps[sequence.index][SEQ_TEMP]);
}

void seq_start() {
	running = 1;
	sequence.index = 0;
	seq_state = SEQ_SAFETY_CHECK;
}

void seq_stop() {
	running = 0;
	heater_regulation_disable();
	heater_disable();
	raise_lift();
}

void seq_continue() {
	if (seq_state == SEQ_RUN && safety_checks()) {
		running = 1;
		setup_heater();
		fans_run_mode();
		update_regulator();
		regulator_enable();
	}
}

void seq_abort() {
	seq_state = SEQ_ABORT;
}

int seq_progress() {
	int total = 0, done = 0;

	for (int i = 0; i < SEQ_STEPS; ++i) {
		int dur = sequence.steps[i][SEQ_DUR];

		if (!dur)
			break;

		if (i <= sequence.index)
			done += dur;

		total += dur;
	}

	done -= sequence.step_time;

	//calculate percent completed eg. 65%
	return (seq_state == SEQ_RUN ? (int)(((float)done / total) * 100) : 0);
}

const char* seq_get_state_str() {
	switch (seq_state) {
	default:
		return " - - ";

	case SEQ_IDLE:
	case SEQ_SAFETY_CHECK:
		return "idle";

	case SEQ_GENERATE_VAPOUR:
		return "warm up";

	case SEQ_PRE_HEAT:
		return "pre heat";

	case SEQ_RUN:
		return "run";

	case SEQ_ABORT:
	case SEQ_COOL_DOWN:
		return "cooling";
	}
}

uint16_t seq_get_flags() {
	return flags;
}

extern void seq_get_limits(uint16_t* min_temp, uint16_t* max_temp,
		uint16_t* total_dur) {

	uint16_t tmax = 0, tmin = UINT16_MAX, total = 0;

	for (int i = 0; i < SEQ_STEPS; ++i) {
		int dur = sequence.steps[i][SEQ_DUR];

		if (!dur)
			break;

		total += dur;

		if (sequence.steps[i][SEQ_TEMP] > tmax)
			tmax = sequence.steps[i][SEQ_TEMP];

		if (sequence.steps[i][SEQ_TEMP] < tmin)
			tmin = sequence.steps[i][SEQ_TEMP];
	}

	if (max_temp)
		*max_temp = tmax;

	if (min_temp)
		*min_temp = tmin;

	if (total_dur)
		*total_dur = total;
}
