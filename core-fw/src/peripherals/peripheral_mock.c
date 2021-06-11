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

#include "peripheral_mock.h"

#include "adc.h"
#include "freq.h"
#include "gpio.h"
#include "pwm.h"
#include "spi.h"
#include "tim.h"

#include "main.h"

#include "stdlib.h"
#include "math.h"
#include "modules/ntc.h"

#include "modules/temp_sensors.h"
#include "modules/max31856/MAX31856_def.h"

#include "modules/lid.h"
#include "modules/lift.h"
#include "modules/axis_isr.h"

#include "assert.h"

#if MOCK_CORE

VpoModel vpo_mock_model;

void peripheral_mock_init() {
	vpo_mock_model = (VpoModel) {
		.water_temperature = 20,
		.heater_temperature = 21,
		.pcb_temperature = 22,
		.galden_temperature = 23,

		.lift_enable = GPIO_PIN_RESET,
		.pump_enable = GPIO_PIN_RESET,
		.lift_direction = GPIO_PIN_RESET,
		.quick_cool_fan_enable = GPIO_PIN_RESET,
		.lid_fan_enable = GPIO_PIN_RESET,
		.lid_motor_halfbridge_a = GPIO_PIN_RESET,
		.lid_motor_halfbridge_b = GPIO_PIN_RESET,
		.spi_cs_0 = GPIO_PIN_SET,
		.spi_cs_1 = GPIO_PIN_SET,
		.spi_cs_2 = GPIO_PIN_SET,
		.spi_cs_3 = GPIO_PIN_SET,

		.third_party = GPIO_PIN_SET,
		.coolant_level_switch = GPIO_PIN_SET,
		.bootloader_check = GPIO_PIN_SET,
		.coolant_otp_status = GPIO_PIN_RESET,
		.lid_overcurrent = GPIO_PIN_RESET,
		.lid_lock_monitor = GPIO_PIN_RESET,

		.lid_end_top = GPIO_PIN_RESET,
		// the lid lid is often closed on reset?
		.lid_end_bottom = GPIO_PIN_SET,

		// some arbitrary values, position is over limit
		.lid_position = -4500,
		.lid_bottom_position = -4000,
		.lid_top_position = 25E3,
		.lid_speed_pos_per_ms = 5,

		.lift_end_top = GPIO_PIN_RESET,
		.lift_end_bottom = GPIO_PIN_RESET,

		// some arbitrary values, position not over limit
		.lift_position = 90E3,
		.lift_top_position = 108E3,
		.lift_bottom_position = 0,

		.heater_otp_status = GPIO_PIN_RESET,

		.heater_pwm_started = false,
		.radior_fan_pwm = 0,
		.lid_encoder_value = 0,
		.lift_tim_started = false,
		.lift_freq_interval_us = 0,
		.lift_encoder_value = 0,

		.temperature_simulation_enabled = true
	};
}

uint32_t mock_ms_last_called = 0;

uint32_t mock_delta_t() {
	uint32_t current_ms = HAL_GetTick();
	uint32_t delta_t = current_ms - mock_ms_last_called;
	mock_ms_last_called = current_ms;

	if(delta_t > 1000) {
		delta_t = 1000;
	}

	return delta_t;
}

void mock_simulate_lid(uint32_t delta_t) {
	// move lid
	// up
	if (vpo_mock_model.lid_motor_halfbridge_a && !vpo_mock_model.lid_motor_halfbridge_b) {
		vpo_mock_model.lid_position += vpo_mock_model.lid_speed_pos_per_ms * delta_t;

	// down
	} else if (!vpo_mock_model.lid_motor_halfbridge_a && vpo_mock_model.lid_motor_halfbridge_b) {
		vpo_mock_model.lid_position -= vpo_mock_model.lid_speed_pos_per_ms * delta_t;
	}

	// lid encoder has inverted sign
	vpo_mock_model.lid_encoder_value = -vpo_mock_model.lid_position;

	if(vpo_mock_model.lid_position <= vpo_mock_model.lid_bottom_position) {
		if(!vpo_mock_model.lid_end_bottom) {
			vpo_mock_model.lid_end_bottom = 1;
			axis_limsw_bwd_isr(&lid_axis);
		}
	} else {
		vpo_mock_model.lid_end_bottom = 0;
	}

	if(vpo_mock_model.lid_position >= vpo_mock_model.lid_top_position) {
		if(!vpo_mock_model.lid_end_top) {
			vpo_mock_model.lid_end_top = 1;
			axis_limsw_fwd_isr(&lid_axis);
		}
	} else {
		vpo_mock_model.lid_end_top = 0;
	}
}

void mock_simulate_lift(uint32_t delta_t) {
	// move lid
	int position_before = vpo_mock_model.lift_position;
	if(vpo_mock_model.lift_enable && vpo_mock_model.lift_tim_started) {
		vpo_mock_model.lift_position += 1E3 * delta_t / vpo_mock_model.lift_freq_interval_us
				* (vpo_mock_model.lift_direction == 0 /* up */ ? 1 : -1);
	}

	vpo_mock_model.lift_encoder_value += abs(vpo_mock_model.lift_position - position_before);

	if(vpo_mock_model.lift_position <= vpo_mock_model.lift_bottom_position) {
		if(!vpo_mock_model.lift_end_bottom) {
			vpo_mock_model.lift_end_bottom = 1;
			axis_limsw_bwd_isr(&lift_axis);
		}
	} else {
		vpo_mock_model.lift_end_bottom = 0;
	}

	if(vpo_mock_model.lift_position >= vpo_mock_model.lift_top_position) {
		if(!vpo_mock_model.lift_end_top) {
			vpo_mock_model.lift_end_top = 1;
			axis_limsw_fwd_isr(&lift_axis);
		}
	} else {
		vpo_mock_model.lift_end_top = 0;
	}
}


// These are some random mock calculations
// they are not required to be exact, they should be stable

const static float heater_factor = 25;
const static float water_factor = 0.5;
const static float quick_cool_factor = 0.12;
const static float lid_fan_factor = 1;
const static float heater_galden_factor = 1;
const static float galden_heater_factor = 0.5;
const static float water_galden_factor = 0.05;
const static float galden_water_factor = 0.05;

const static float lift_position_temperature_factor = 3;

void mock_simulate_temperatures(uint32_t delta_t) {
	if(!vpo_mock_model.temperature_simulation_enabled) {
		return;
	}

	if(vpo_mock_model.heater_pwm_started) {
		vpo_mock_model.heater_temperature += delta_t * heater_factor / 1000;
	}

	if (vpo_mock_model.quick_cool_fan_enable) {
		vpo_mock_model.galden_temperature -= (vpo_mock_model.galden_temperature - 20) * delta_t * quick_cool_factor / 1000;
	}

	if(vpo_mock_model.lid_fan_enable && vpo_mock_model.lift_position > (vpo_mock_model.lift_top_position - vpo_mock_model.lift_bottom_position) * 0.7f) {
		vpo_mock_model.pcb_temperature -= (vpo_mock_model.pcb_temperature - 20) * delta_t * lid_fan_factor / 1000;
	}

	float heater_galden_diff = (vpo_mock_model.galden_temperature - vpo_mock_model.heater_temperature);
	vpo_mock_model.galden_temperature -= heater_galden_diff * delta_t * heater_galden_factor / 1000;
	vpo_mock_model.heater_temperature += heater_galden_diff * delta_t * galden_heater_factor  / 1000;

	if (vpo_mock_model.pump_enable) {
		vpo_mock_model.water_temperature -= (vpo_mock_model.water_temperature - 20) * (100 - vpo_mock_model.radior_fan_pwm) * delta_t * water_factor / 100000;
	}
	float water_galden_diff = (vpo_mock_model.galden_temperature - vpo_mock_model.water_temperature);
	vpo_mock_model.galden_temperature -= water_galden_diff * delta_t * water_galden_factor / 1000;
	vpo_mock_model.water_temperature += water_galden_diff * delta_t * galden_water_factor / 1000;

	if (vpo_mock_model.galden_temperature > 240) {
		vpo_mock_model.galden_temperature = 240;
	}

	float galden_water_ratio = (float)(vpo_mock_model.lift_position) / (vpo_mock_model.lift_top_position - vpo_mock_model.lift_bottom_position);
	if(galden_water_ratio < 0)
		galden_water_ratio = 0;
	else if(galden_water_ratio > 1)
		galden_water_ratio = 1;

	float lift_position_temperature = (1 - galden_water_ratio) * vpo_mock_model.galden_temperature + galden_water_ratio * vpo_mock_model.water_temperature;

	vpo_mock_model.pcb_temperature -= (vpo_mock_model.pcb_temperature - lift_position_temperature) * delta_t * lift_position_temperature_factor / 1000;
}

void peripheral_mock_worker() {
	uint32_t delta_t = mock_delta_t();

	if(delta_t == 0) {
		return;
	}

	mock_simulate_lid(delta_t);
	mock_simulate_lift(delta_t);

	mock_simulate_temperatures(delta_t);
}

void iopin_write(IO_pin_t io_pin, GPIO_PinState state) {

	if(io_pin.port == LIFT_ENABLE_GPIO_Port && io_pin.pin == LIFT_ENABLE_Pin) {
		vpo_mock_model.lift_enable = state;
	} else if(io_pin.port == PUMP_ENABLE_GPIO_Port && io_pin.pin == PUMP_ENABLE_Pin) {
		vpo_mock_model.pump_enable = state;
	} else if(io_pin.port == LIFT_DIRECTION_GPIO_Port && io_pin.pin == LIFT_DIRECTION_Pin) {
		vpo_mock_model.lift_direction = state;
	} else if(io_pin.port == QC_FAN_ENABLE_GPIO_Port && io_pin.pin == QC_FAN_ENABLE_Pin) {
		vpo_mock_model.quick_cool_fan_enable = state;
	} else if(io_pin.port == LID_FAN_ENABLE_GPIO_Port && io_pin.pin == LID_FAN_ENABLE_Pin) {
		vpo_mock_model.lid_fan_enable = state;
	} else if(io_pin.port == LID_MOTOR_HALFBRIDGE_A_GPIO_Port && io_pin.pin == LID_MOTOR_HALFBRIDGE_A_Pin) {
		vpo_mock_model.lid_motor_halfbridge_a = state;
	} else if(io_pin.port == LID_MOTOR_HALFBRIDGE_B_GPIO_Port && io_pin.pin == LID_MOTOR_HALFBRIDGE_B_Pin) {
		vpo_mock_model.lid_motor_halfbridge_b = state;
	} else if(io_pin.port == TEMP_SENSOR_0_CS_GPIO_Port && io_pin.pin == TEMP_SENSOR_0_CS_Pin) {
		vpo_mock_model.spi_cs_0 = state;
	} else if(io_pin.port == TEMP_SENSOR_1_CS_GPIO_Port && io_pin.pin == TEMP_SENSOR_1_CS_Pin) {
		vpo_mock_model.spi_cs_1 = state;
	} else if(io_pin.port == TEMP_SENSOR_2_CS_GPIO_Port && io_pin.pin == TEMP_SENSOR_2_CS_Pin) {
		vpo_mock_model.spi_cs_2 = state;
	} else if(io_pin.port == TEMP_SENSOR_3_CS_GPIO_Port && io_pin.pin == TEMP_SENSOR_3_CS_Pin) {
		vpo_mock_model.spi_cs_3 = state;
	}  else {
		// this pin should not be written to
		assert(false);
	}
}

GPIO_PinState iopin_read(IO_pin_t io_pin) {
	if(io_pin.port == THIRD_PARTY_GPIO_Port && io_pin.pin == THIRD_PARTY_Pin) {
		return vpo_mock_model.third_party;
	} else if(io_pin.port == COOLANT_LEVEL_SWITCH_GPIO_Port && io_pin.pin == COOLANT_LEVEL_SWITCH_Pin) {
		return vpo_mock_model.coolant_level_switch;
	} else if(io_pin.port == BOOTLOADER_CHECK_GPIO_Port && io_pin.pin == BOOTLOADER_CHECK_Pin) {
		return vpo_mock_model.bootloader_check;
	} else if(io_pin.port == COOLANT_OTP_STATUS_GPIO_Port && io_pin.pin == COOLANT_OTP_STATUS_Pin) {
		return vpo_mock_model.coolant_otp_status;
	} else if(io_pin.port == LID_OVERCURRENT_GPIO_Port && io_pin.pin == LID_OVERCURRENT_Pin) {
		return vpo_mock_model.lid_overcurrent;
	} else if(io_pin.port == LID_LOCK_MONITOR_GPIO_Port && io_pin.pin == LID_LOCK_MONITOR_Pin) {
		return vpo_mock_model.lid_lock_monitor;
	} else if(io_pin.port == LID_END_TOP_GPIO_Port && io_pin.pin == LID_END_TOP_Pin) {
		return vpo_mock_model.lid_end_top;
	} else if(io_pin.port == LID_END_BOTTOM_GPIO_Port && io_pin.pin == LID_END_BOTTOM_Pin) {
		return vpo_mock_model.lid_end_bottom;
	} else if(io_pin.port == LIFT_END_TOP_GPIO_Port && io_pin.pin == LIFT_END_TOP_Pin) {
		return vpo_mock_model.lift_end_top;
	} else if(io_pin.port == LIFT_END_BOTTOM_GPIO_Port && io_pin.pin == LIFT_END_BOTTOM_Pin) {
		return vpo_mock_model.lift_end_bottom;
	} else if(io_pin.port == HEATER_OTP_STATUS_GPIO_Port && io_pin.pin == HEATER_OTP_STATUS_Pin) {
		return vpo_mock_model.heater_otp_status;
	}

	assert(false);

	return GPIO_PIN_RESET;
}

void adc_init(ADC_HandleTypeDef* hadc) {}

uint32_t adc_resolution_get_max_value(const AdcSignalHandle* handle) {
	return 0xFFF;
}

uint32_t adc_value_from_temperature_celsius(float temperature) {
	double temperature_kelvin = celsius_to_kelvin + temperature;
	double resistance = ntc_resistance_25_ohm * exp(ntc_beta_kelvin*(1/temperature_kelvin - 1/ntc_temperature_25_kelvin));

	double factor = 1 / (ntc_voltage_divider_other_resistor_ohm / resistance + 1);

	return factor * adc_resolution_get_max_value(NULL);
}

uint32_t adc_get_value(const AdcSignalHandle* handle) {
	if(handle->sample_index == ntc_adc_signal.sample_index) {
		return adc_value_from_temperature_celsius(vpo_mock_model.water_temperature);
	}

	return 1000 + handle->sample_index;
}

int spi_ready(struct spi_dev_t* spid) {
	return 1;
}

float get_temperature_for_spi(struct spi_dev_t* spid) {
	if(spid == &max31856_temperature_sensor_heater_1.spid || spid == &max31856_temperature_sensor_heater_2.spid) {
		return vpo_mock_model.heater_temperature;
	} else if (spid == &max31856_temperature_sensor_galden.spid) {
		return vpo_mock_model.galden_temperature;
	} else if(spid == &max31856_temperature_sensor_pcb.spid) {
		return vpo_mock_model.pcb_temperature;
	}

	return -1;
}

void spi_xfer8(struct spi_dev_t* spid, uint8_t* data_tx,
		uint8_t* data_rx, size_t len) {
	if((data_tx[0] & 0x80) == 0) {
		uint32_t raw = get_temperature_for_spi(spid) / MAX31856_RESOLUTION;
		raw = raw << 5;

		data_rx[1] = (raw >> 16) & 0xFF;
		data_rx[2] = (raw >> 8) & 0xFF;
		data_rx[3] = raw & 0xFF;
	}
}

void pwm_set_FAN(int value) {
	vpo_mock_model.radior_fan_pwm = value;
}

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim1;

uint32_t tim_get_counter(TIM_HandleTypeDef* htim) {
	if(htim == &htim3)
		return vpo_mock_model.lid_encoder_value;
	if(htim == &htim1)
		return vpo_mock_model.lift_encoder_value;
	return 0;
}

uint32_t tim_get_autoreload(TIM_HandleTypeDef* htim) {
	return (uint16_t)-1;
}

void tim_base_start(TIM_HandleTypeDef* htim) {

}

void tim_encoder_start(TIM_HandleTypeDef* htim) {

}

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim15;

void tim_pwm_start(TIM_HandleTypeDef* htim, uint16_t channel) {
	if(htim == &htim2 && channel == TIM_CHANNEL_3) {
		vpo_mock_model.heater_pwm_started = true;
	} else if(htim == &htim15 && channel == TIM_CHANNEL_2) {
		vpo_mock_model.lift_tim_started = true;
	}
}

void tim_pwm_stop(TIM_HandleTypeDef* htim, uint16_t channel) {
	if(htim == &htim2 && channel == TIM_CHANNEL_3) {
		vpo_mock_model.heater_pwm_started = false;
	} else if(htim == &htim15 && channel == TIM_CHANNEL_2) {
		vpo_mock_model.lift_tim_started = false;
	}
}

void tim_set_autoreload(TIM_HandleTypeDef* htim, uint32_t arr) {
	
}

void tim_set_compare(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t compare) {
	vpo_mock_model.lift_freq_interval_us = compare;
}

#endif
