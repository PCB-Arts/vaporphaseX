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

#include "main.h"
#include "init.h"
#include "conf.h"

#if MOCK_CORE
#include "peripherals/peripheral_mock.h"
#endif

AdcSignalHandle ntc_adc_signal = {
	.sample_index = 5,
};

// definitions

static const IO_pin_t ocp_latch_reset = IO_PIN_TPL(OCP_LATCH_RESET_GPIO_Port, OCP_LATCH_RESET_Pin);

void init_modules() {
	///////////////////////////
	//init modules

#if MOCK_CORE
	peripheral_mock_init();
#endif

	profile_com_init();

	Packet_Init(&huart1);

	comm_usart_init();
	cli_init();
	cli_set_execute_cb(&cli_commands_execute);
	cli_set_broadcaster_cb(&cli_commands_broadcast);

	adc_init(&hadc1);
	ntc_temperature_sensor_water.adc_signal = &ntc_adc_signal;

	spi1.perif = &hspi1;
	max31856_temp_sensors_init();

	fans_init();

	lid_axis.encoder->htim = &htim3;
	lid_axis.encoder->factor = -1;

	// important set propagation delay before limit switch init
	lid_init();
	axis_init(&lid_axis);
	vpo_log("Lid calibrated");
	fan_set_auto_enabled(&lid_fan, true);

	freq_init(&htim15, TIM_CHANNEL_2, &htim1);
	axis_init(&lift_axis);
	vpo_log("Lift calibrated");
	regulator_init(&temperature_sensor_pcb, &lift_axis);

	heater_init(&temperature_sensor_heater_1, &temperature_sensor_heater_2, &temperature_sensor_galden,  &htim2, TIM_CHANNEL_3);
	heater_regulation_disable();

	pump_set_auto_enabled(true);

	fan_set_auto_enabled(&quick_cooler, true);

	control_modules_init();

	iopin_write(ocp_latch_reset, 0);
	vpo_log("OCP Latch Reset Low");
}
