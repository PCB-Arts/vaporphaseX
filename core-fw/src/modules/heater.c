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


//=================================
// included dependencies
#include "modules/heater.h"
#include "utilities/irq.h"
#include "modules/temp_sensor.h"
#include "peripherals/tim.h"
#include "main.h"
#include "log.h"

//=================================
// definitions

#define HEATER_WD_S 5
#define HEATER_OTP_C(x) (x)+40
#define HEATER_OTP_HYST 2.0f

// galden temperature on °C that should be reached before heater is operational (profile starts)
//#define HEATER_OPERATION_TEMPERATURE_THRESHOLD 222
#define HEATER_OPERATION_TEMPERATURE_THRESHOLD(x) (x)-8



#define HEATER_MASK_ERR_EN 0b00000000

static TemperatureSensor* sensor1;
static TemperatureSensor* sensor2;
static TemperatureSensor* galdenSensor;
static TIM_HandleTypeDef* sensor_enable_htim;
static uint16_t sensor_enable_channel;
static uint8_t flags;
static uint16_t watchdog_s = HEATER_WD_S;

static float set_temp, set_hyst;
static bool regulation, enabled;


static int sensor_valid();
static void check_max_temp();

static int has_faults();
static void set_flag(uint8_t flag);
static int operational_condition();

static const IO_pin_t otp_status_pin = IO_PIN_TPL(HEATER_OTP_STATUS_GPIO_Port, HEATER_OTP_STATUS_Pin);

void heater_init(TemperatureSensor* s1, TemperatureSensor* s2, TemperatureSensor* _galdenSensor, TIM_HandleTypeDef* htim, uint16_t channel) {
	sensor1 = s1;
	sensor2 = s2;
	galdenSensor = _galdenSensor;
	sensor_enable_htim = htim;
	sensor_enable_channel = channel;
	vpo_log("heater_init");

	heater_disable();

	// enable signal generation, signal form is defined wheter enabled or disabled
	tim_pwm_start(sensor_enable_htim, sensor_enable_channel);
}

static int sensor_valid() {
	if (temperature_sensor_has_fault(sensor1) || temperature_sensor_has_fault(sensor2) || temperature_sensor_has_fault(galdenSensor)) {
		//set sensor fault
		set_flag(HEATER_ERR_SENSOR);
		return 0;
	} else {
		return 1;
	}
}

void heater_worker() {
	if (!operational_condition())
		return;

	//reset heater watchdog timeout
	watchdog_s = HEATER_WD_S;

	//check if regulation is enabled
	if (regulation) {
		float temp1 = temperature_sensor_get_temperature(sensor1);
		float temp2 = temperature_sensor_get_temperature(sensor2);

		//greater temperature is respected
		if (temp2 > temp1){
			temp1 = temp2;
		}

		if (temp1 >= set_temp) {
			//disable heater if set temperature is reached
			heater_disable();
			vpo_log("heater_temp1_disable");

		} else if (temp1 < set_temp - set_hyst) {
			//enable heater if set temperature is not reached
			heater_enable();
		}
	}
}

static void check_max_temp() {
	//check max. allowed temperature limit
	if (temperature_sensor_get_temperature(sensor1) >= HEATER_OTP_C(globalGalden.galdenTemp)) {
		//set over temperature flag
		set_flag(HEATER_ERR_MAX_TEMP);
	}

	if (temperature_sensor_get_temperature(sensor2) >= HEATER_OTP_C(globalGalden.galdenTemp)) {
		//set over temperature flag
		set_flag(HEATER_ERR_MAX_TEMP);
	}
}

void heater_seconds_sync() {
	if (watchdog_s) {
		//decrement watchdog timer
		--watchdog_s;
	} else {
		//set watchdog fault
		set_flag(HEATER_ERR_WATCHDOG);
	}
}

static void set_flag(uint8_t flag) {
	//set flag
	SET_BIT(flags, flag);

	//disable heater if has faults
	if (has_faults()){
		heater_disable();
		vpo_log("heater_set_flag_disable");
	}
}

uint8_t heater_get_flags() {
	return flags;
}

void heater_set_temperature(float temperature) {
	set_temp = temperature;
}

void heater_set_hysteresis(float hysteresis) {
	set_hyst = hysteresis;
}

void heater_regulation_enable() {
	regulation = true;
}

void heater_regulation_disable() {
	regulation = false;
}

bool heater_regulation_enabled() {
	return regulation;
}

static void heater_set_enabled_signal() {
	// enable or disable rectangular signal
	if(enabled) {
		// generate signal by setting compare to 50%
		int half_duty_cycle = (tim_get_autoreload(sensor_enable_htim) + 1) / 2;
		tim_set_compare(sensor_enable_htim, sensor_enable_channel, half_duty_cycle);
	} else {
		// disable rectangular signal by setting compare to 0
		tim_set_compare(sensor_enable_htim, sensor_enable_channel, 0);
	}
}

void heater_enable() {
	disable_irq();

	//enabled only if no heater faults are active
	enabled = !has_faults();

	heater_set_enabled_signal();

	enable_irq();
}

void heater_disable() {
	enabled = false;
	vpo_log("heater_disable");
	heater_set_enabled_signal();
}

static int has_faults() {
	//mask predefined flags for faults
	return (flags & ~(HEATER_MASK_ERR_EN)) != 0;
}

static int operational_condition() {
	if (!sensor_valid())
		return 0;

	check_max_temp();

	if (iopin_read(otp_status_pin))
		heater_overtemp();

	return !has_faults();
}

int heater_temperature_reached() {
	if (!sensor_valid())
		return 0;

	const float temp = temperature_sensor_get_temperature(galdenSensor);
	return (temp >= HEATER_OPERATION_TEMPERATURE_THRESHOLD(globalGalden.galdenTemp));

}

void heater_overtemp() {
	//set OTP tripped flag
	set_flag(HEATER_ERR_OTP_TRIPPED);
}

void heater_reset_flags() {
	CLEAR_REG(flags);
	operational_condition();
}

void heater_emergency_stop() {
	set_flag(HEATER_ERR_EMERGENCY_STOP);
}

bool heater_enabled(){
	return enabled;
}
