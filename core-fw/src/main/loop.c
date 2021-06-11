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

#include "control/loop.h"
#include "ui/cli.h"
#include "ui/cli_commands.h"
#include "system/core_clock.h"
#include "system/systime.h"
#include "interrupt_priorities.h"
#include "peripherals/adc.h"
#include "peripherals/freq.h"
#include "peripherals/spi.h"
#include "peripherals/comm_usart.h"
#include "peripherals/pwm.h"
#include "peripherals/wwdg.h"
#include "peripherals/peripheral_mock.h"
#include "modules/axis.h"
#include "modules/lid.h"
#include "modules/lift.h"
#include "modules/regulator.h"
#include "modules/temp_sensors.h"
#include "modules/heater.h"
#include "modules/fan.h"
#include "modules/fans.h"
#include "modules/pump.h"
#include "modules/max31856/MAX31856.h"
#include "modules/temp_sensor.h"
#include "packet_handler.h"
#include "log.h"
#include "version.h"

#include "conf.h"

#define MAX_STARTUP_WATER_TEMPERATURE_C 80
bool first_temperature_reading = true;

void main_loop() {
	wwdg_reset();
	systime_worker();

#if MOCK_CORE
	peripheral_mock_worker();
#endif

	cli_worker();

	max31856_temp_sensors_worker();
	lid_worker();
	lift_worker();

	//check if all temperature sensors were updated
	if (max31856_temp_sensor_cycle_complete) {
		max31856_temp_sensor_cycle_complete = 0;


		// TODO check if this logic is still valid
		if (first_temperature_reading) {
			first_temperature_reading = false;

			if (temperature_sensor_get_temperature(&temperature_sensor_water) > MAX_STARTUP_WATER_TEMPERATURE_C) {
				vpo_log("Water temperature exceeded the maximal allowed water temperature");
			}
		}

		heater_worker();
	}

	pump_worker();

	control_modules_loop();

	Packet_Worker();
}

#define BROADCASTER_DELAY_MS 250
static uint16_t broadcaster_delay;

void main_millis_sync() {
	//check if broadcasting delay has passed
	if (broadcaster_delay) {
		--broadcaster_delay;
	} else {
		broadcaster_delay = BROADCASTER_DELAY_MS;

		//trigger next broadcasting cycle
		cli_broadcast_trigger();
	}

	static int ms;
	if (++ms > 100) {
		regulator_compute();
		fan_regulate(&hydro_cooler);
		fan_regulate(&quick_cooler);
		fan_regulate(&lid_fan);
		ms = 0;
	}
}
