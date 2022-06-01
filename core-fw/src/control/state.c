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

#include <stdbool.h>
#include <math.h>

#include "system/systime.h"
#include "state.h"
#include "conf.h"
#include "log.h"

#include "control/sequence.h"
#include "control/lid_control.h"
#include "control/selftest.h"

#include "main.h"

#include "modules/pump.h"
#include "modules/heater.h"
#include "modules/fan.h"
#include "modules/fans.h"
#include "modules/lift.h"
#include "modules/temp_sensors.h"
#include "modules/lid.h"
#include "modules/lift.h"
#include "modules/monitor_pins.h"



#define START_REFLOW_MAX_WATER_TEMPERATURE 60
#define START_SELFTEST_MAX_GALDEN_TEMPERATURE 50

static bool State_Booted;

static bool error_heater_otp;
static bool error_coolant_otp;
static bool error_lid_ocp;

extern calibpos calib;						 // Änderung
extern currentState cS;


void Mock_Worker();

void State_Init() {
	State_Booted = false;

	error_heater_otp = false;
	error_coolant_otp = false;
	error_lid_ocp = false;
}

bool State_ReflowActive() {
	return seq_running();
}

bool State_SelftestActive() {
	return selftest_active();
}

static bool State_StartWaterTemperatureCondition() {
	return temperature_sensor_get_temperature(&temperature_sensor_water) < START_REFLOW_MAX_WATER_TEMPERATURE;
}

static bool State_StartSelftestGaldenTemperatureCondition() {
	return temperature_sensor_get_temperature(&temperature_sensor_galden) < START_SELFTEST_MAX_GALDEN_TEMPERATURE;
}

bool State_BaseStartCondition() {
	return State_Booted
		&& !State_ReflowActive()
		&& !State_SelftestActive();
}

bool State_CanStartReflow() {
	return State_BaseStartCondition()
		&& lid_control_lid_closed()
		&& State_StartWaterTemperatureCondition();
}

bool State_CanStartSelftest() {
	return State_BaseStartCondition()
			&& State_StartSelftestGaldenTemperatureCondition();
}

bool State_CanOpenLid() {
	return State_BaseStartCondition()
		&& lid_control_lid_open_requirements_met();
}

void State_SetReflow(bool active) {
	Vpo_CoreStateTypeDef state = State_getState();

	if(state.error) {
	    vpo_log("SetReflow on Error state");
		return;
	}

	if(active && !state.can_start_reflow) {
	    vpo_log("SetReflow start conditions not met");
		return;
	}

	if(State_ReflowActive() == active) {
	    vpo_log("SetReflow already running");
		return;
	}

	if(active) {
		if(globalProfile.profileId < 0) {
			vpo_log("error starting profile, not loaded!");
		} else {
			seq_start(&globalProfile);
			vpo_log("start reflow");
		}
	} else {
		seq_abort();
		vpo_log("abort reflow");
	}
}


void State_SetLidOpen(bool open) {
	Vpo_CoreStateTypeDef state = State_getState();

	if (state.error) {
	    vpo_log("SetLidOpen on Error state");
		return;
	}

	if(open) {
		if(!state.can_open_lid) {
			return;
		}

		lid_control_lid_open();
	} else {
		lid_control_lid_close();
	}

	vpo_log(open ? "open lid" : "close lid");
}

/* 
 * Returns true if the requirements to leave boot state are fullfiled
 */ 


bool State_BootRequirementsFullfiled() {
	return lift_boot_ready() && lid_control_lid_boot_ready();
}

void State_SetLidOpenMode(LidOpenMode mode) {
	lid_control_set_lid_open_mode(mode);
}

void State_Worker() {
	if(heater_otp_active()) {
		error_heater_otp = true;
	}

	if(coolant_otp_active()) {
		error_coolant_otp = true;
	}

	if(lid_ocp_active()) {
		error_lid_ocp = true;
	}

	if(State_Booted) {
		return;
	}

	// first calibrate lid axis and after lid is closed and calibrated do the lift
	
	if (lid_axis.cal_done == 1 && lift_axis.cal_done == 0 && lift_axis.mode == AXIS_NORMAL) {
		cS.currentState = 4;
		lift_calibrate();
	} else if(lid_axis.cal_done == 0 && lid_axis.mode == AXIS_NORMAL) {
		lid_calibrate();
	}

	// Lift move to open lid position
	if (calib.liftflag == 1 && lid_axis.cal_done == 1 && lift_axis.cal_done == 1){			//Änderung
			calib.liftflag = 0;
			axis_move_to(&lift_axis, LIFT_LID_OPEN_POSITION);
			}
	if(lift_lid_open_state_reached() == true){
		cS.currentState = 0;
	}

	if (!State_BootRequirementsFullfiled()) {
		return;
	}
	State_Booted = true;
	
}

Vpo_CoreStateTypeDef State_getState() {
	Vpo_CoreStateTypeDef state;

	state.error_lid_ocp = error_lid_ocp;
	state.error_heater_otp = error_heater_otp;
	state.error_coolant_otp = error_coolant_otp;

	state.error = error_lid_ocp | error_heater_otp | error_coolant_otp;

	state.reflow_active = State_ReflowActive();

	state.can_start_reflow = State_CanStartReflow();

	state.can_open_lid = State_CanOpenLid();

	state.can_update_profile = !State_ReflowActive();

	state.lid_state = lid_control_lid_state();

	state.lid_open_mode = lid_control_get_lid_open_mode();

	state.can_stop_reflow = seq_can_abort();

	state.can_start_selftest = State_CanStartSelftest();

	state.selftest_active = selftest_active();

	state.current_state = State_CurrentState();

	return state;
}

Vpo_CoreMonitorState1 State_getMonitor1State(){
	Vpo_CoreMonitorState1 state;

	state.heater1_temp = temperature_sensor_get_temperature(&temperature_sensor_heater_1);
	state.heater2_temp = temperature_sensor_get_temperature(&temperature_sensor_heater_2);
	state.galden_temp = temperature_sensor_get_temperature(&temperature_sensor_galden);
	state.pcb_temp = temperature_sensor_get_temperature(&temperature_sensor_pcb);
	return state;
}


Vpo_CoreMonitorState2 State_getMonitor2State(){
	Vpo_CoreMonitorState2 state;

	state.coolant_temp = temperature_sensor_get_temperature(&temperature_sensor_water);

	state.lid_cooling_fan_on = lid_fan.level > 0;
	state.coolant_pump_on = pump_enabled();
	state.quick_cool_fan_on = quick_cooler.level > 0;
	state.heater_on = heater_enabled();
	state.heater_otp_active = iopin_read(heater_otp_status_pin);
	state.coolant_otp_active = iopin_read(coolant_otp_status_pin);
	state.lid_lock_active = iopin_read(lid_lock_monitor_pin);

	state.radiator_fan_speed = fan_get_speed(&hydro_cooler);

	return state;
}

Vpo_CoreMonitorState3 State_getMonitor3State(){
	Vpo_CoreMonitorState3 state;

	state.lid_position = lid_axis.encoder->position;
	state.lift_position = lift_axis.encoder->position;

	return state;
}

void State_StartSelftest() {
	if(!State_CanStartSelftest()) {
		return;
	}

	selftest_start();
}

CurrentState State_CurrentState(){
	if(cS.currentState == 1){
		return CS_Preheat;
	}
	else if(cS.currentState == 2){
		return CS_Soldering;
		}
	else if(cS.currentState == 3){
		return CS_AntiCondensation;
		}
	else if(cS.currentState == 4){
		return CS_Calibration;
	}
	else{
		return CS_Idle;
	}
}



