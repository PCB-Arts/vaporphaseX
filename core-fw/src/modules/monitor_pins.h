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

#ifndef __MONITOR_PINS_H
#define __MONITOR_PINS_H

#include <stdbool.h>
#include "peripherals/gpio.h"

const IO_pin_t heater_otp_status_pin;
const IO_pin_t coolant_otp_status_pin;
const IO_pin_t lid_lock_monitor_pin;
const IO_pin_t lid_oc_status_pin;

bool heater_otp_active();
bool coolant_otp_active();
bool lid_ocp_active();

int current_monitor_lid_value();
int current_monitor_lift_value();
int current_monitor_pump_value();
int current_monitor_quick_cool_value();
int current_monitor_radiator_value();

#endif
