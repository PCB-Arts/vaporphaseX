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

#ifndef PERIPHERALS_PERIPHERAL_MOCK_H_
#define PERIPHERALS_PERIPHERAL_MOCK_H_

#include "peripherals/gpio.h"

typedef struct __VpoModel {
// temperatures
	float water_temperature;
	float heater_temperature;
	float pcb_temperature;
	float galden_temperature;

// gpio outputs
	GPIO_PinState lift_enable;
	GPIO_PinState pump_enable;
	GPIO_PinState lift_direction;
	GPIO_PinState quick_cool_fan_enable;
	GPIO_PinState lid_fan_enable;
	GPIO_PinState lid_motor_halfbridge_a;
	GPIO_PinState lid_motor_halfbridge_b;
	GPIO_PinState spi_cs_0;
	GPIO_PinState spi_cs_1;
	GPIO_PinState spi_cs_2;
	GPIO_PinState spi_cs_3;

// gpio inputs
	GPIO_PinState third_party;
	GPIO_PinState coolant_level_switch;
	GPIO_PinState bootloader_check;
	GPIO_PinState coolant_otp_status;
	GPIO_PinState lid_overcurrent;
	GPIO_PinState lid_lock_monitor;

// gpio inputs with external interrupt
	GPIO_PinState lid_end_top;
	GPIO_PinState lid_end_bottom;
	GPIO_PinState lift_end_top;
	GPIO_PinState lift_end_bottom;
	GPIO_PinState heater_otp_status;

// TIM values
	bool heater_pwm_started;
	int radior_fan_pwm;
	int lid_encoder_value;

	int lift_freq_interval_us;
	int lift_encoder_value;

// additional Model values
	int lid_position;
	int lid_bottom_position;
	int lid_top_position;
	int lid_speed_pos_per_ms;

	int lift_position;
	int lift_bottom_position;
	int lift_top_position;
	bool lift_tim_started;

	bool temperature_simulation_enabled;
} VpoModel;

extern VpoModel vpo_mock_model;

void peripheral_mock_init();
void peripheral_mock_worker();

#endif /* PERIPHERALS_PERIPHERAL_MOCK_H_ */
