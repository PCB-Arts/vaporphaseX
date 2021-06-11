#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include "profile_com.h"

void test_temperature_sensor_update();

void run_for_millis(int millis);

void run_for_seconds(int seconds);

void load_example_profile(TemperatureProfile* profile);

void DEBUG_TEST_OUTPUT(const char* format, ...);

void print_mock_state();

#endif