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

#include "monitor_pins.h"
#include "main.h"

#include "peripherals/adc.h"

const IO_pin_t heater_otp_status_pin = IO_PIN_TPL(HEATER_OTP_STATUS_GPIO_Port, HEATER_OTP_STATUS_Pin);
const IO_pin_t coolant_otp_status_pin = IO_PIN_TPL(COOLANT_OTP_STATUS_GPIO_Port, COOLANT_OTP_STATUS_Pin);
const IO_pin_t lid_lock_monitor_pin = IO_PIN_TPL(LID_LOCK_MONITOR_GPIO_Port, LID_LOCK_MONITOR_Pin);
const IO_pin_t lid_oc_status_pin = IO_PIN_TPL(LID_OVERCURRENT_GPIO_Port, LID_OVERCURRENT_Pin);

bool heater_otp_active() {
	return iopin_read(heater_otp_status_pin) == GPIO_PIN_SET;
}

bool coolant_otp_active() {
	return iopin_read(coolant_otp_status_pin) == GPIO_PIN_SET;
}

bool lid_ocp_active() {
	return iopin_read(lid_oc_status_pin) == GPIO_PIN_SET;
}

const AdcSignalHandle monitor_adc_handle_lid = { .sample_index = 1 };
const AdcSignalHandle monitor_adc_handle_lift = { .sample_index = 4 };
const AdcSignalHandle monitor_adc_handle_pump = { .sample_index = 2 };
const AdcSignalHandle monitor_adc_handle_quick_cool = { .sample_index = 3 };
const AdcSignalHandle monitor_adc_handle_radiator = { .sample_index = 0 };

int current_monitor_lid_value() {
	return adc_get_value(&monitor_adc_handle_lid);
}

int current_monitor_lift_value() {
	return adc_get_value(&monitor_adc_handle_lift);
}

int current_monitor_pump_value() {
	return adc_get_value(&monitor_adc_handle_pump);
}

int current_monitor_quick_cool_value() {
	return adc_get_value(&monitor_adc_handle_quick_cool);
}

int current_monitor_radiator_value() {
	return adc_get_value(&monitor_adc_handle_radiator);
}

