#include "test_util.h"
#include "temp_sensors.h"

#include "systime.h"
#include "main/loop.h"
#include "peripheral_mock.h"
#include "regulator.h"

void test_temperature_sensor_update() {
	for (int i = 0; i < max31856_temp_sensors_size; ++i) {
		max31856_temp_sensors[i]->tasks.reg.LTC |= MAX31856_TASK_READ;
	}
    for(int i = 0; i < 30; i++) {
        max31856_temp_sensors_worker();
    }
}

void run_for_millis(int millis) {
    for (int ms = 0; ms < 1000; ms += 5) {
        main_loop();
        for (int i = 0; i < 5; i++) {
            systime_slicer();
            HAL_IncTick();
        }
    }
}

void run_for_seconds(int seconds) {
    for (int s = 0; s < seconds; s++) {
        run_for_millis(seconds * 1000);
    }
}

void load_example_profile(TemperatureProfile* profile) {
    profile->profileId = 1;
    profile->datapoints[0].time = 30;
    profile->datapoints[0].temperature = 50;
    profile->datapoints[1].time = 30;
    profile->datapoints[1].temperature = 100;
    profile->datapoints[2].time = 30;
    profile->datapoints[2].temperature = 150;
    profile->datapoints[3].time = 40;
    profile->datapoints[3].temperature = 200;
    profile->datapoints[4].time = 60;
    profile->datapoints[4].temperature = 230;
    profile->datapoints[5].time = 30;
    profile->datapoints[5].temperature = 120;
    profile->datapoints[6].time = 30;
    profile->datapoints[6].temperature = 80;

    profile->totalDatapoints = 7;
}

void print_mock_state() {
    
    DEBUG_TEST_OUTPUT("State: galden-temp: %.2f, water: %.2f, pcb: %.2f heater: %d, pump: %d, lid fan: %d, qc: %d, lift pos: %d, radiator fan pwm: %d lid pos: %d target temp: %.2f",
        vpo_mock_model.galden_temperature,
        vpo_mock_model.water_temperature,
        vpo_mock_model.pcb_temperature,
        vpo_mock_model.heater_pwm_started,
        vpo_mock_model.pump_enable,
        vpo_mock_model.lid_fan_enable,
        vpo_mock_model.quick_cool_fan_enable,
        vpo_mock_model.lift_position,
        vpo_mock_model.radior_fan_pwm,
        vpo_mock_model.lid_position,
        regulator_get_target_temperature()
    );

    DEBUG_TEST_OUTPUT("regulator temp reached: %d", regulator_temperature_reached());
}
