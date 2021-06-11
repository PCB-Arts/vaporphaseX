#include "main.h"
#include <unity.h>

#include "init.h"
#include "loop.h"
#include "pump.h"
#include "lift.h"

#include "state.h"
#include "sequence.h"
#include "lift.h"

#include "test_util.h"

#define TAL_PHASE_THRESHOLD_TEMPERATURE 140

void setUp(void) {
    init_modules();
}

void tearDown(void) {
    
}

void test_untouched_system() {
    TEST_ASSERT_FLOAT_WITHIN(2, 21, vpo_mock_model.heater_temperature);
    TEST_ASSERT_FLOAT_WITHIN(2, 21, vpo_mock_model.pcb_temperature);
    TEST_ASSERT_FLOAT_WITHIN(2, 21, vpo_mock_model.water_temperature);
    TEST_ASSERT_FLOAT_WITHIN(2, 21, vpo_mock_model.galden_temperature);

    run_for_seconds(10);

    TEST_ASSERT_FLOAT_WITHIN(2, 21, vpo_mock_model.heater_temperature);
    TEST_ASSERT_FLOAT_WITHIN(2, 21, vpo_mock_model.pcb_temperature);
    TEST_ASSERT_FLOAT_WITHIN(2, 21, vpo_mock_model.water_temperature);
    TEST_ASSERT_FLOAT_WITHIN(2, 21, vpo_mock_model.galden_temperature);
}

void test_sequence() {
    int time_guard;

    time_guard = 15; // max 15s
    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
        if(--time_guard < 0) {
            TEST_FAIL();
        }
    }

    load_example_profile(&globalProfile);

    State_SetReflow(true);

    run_for_millis(5);

    // check that regulator is started initially
    TEST_ASSERT_TRUE(regulator_enabled());
    TEST_ASSERT_EQUAL(globalProfile.datapoints[0].temperature, regulator_get_target_temperature());

    time_guard = 4000; // max 20s
    int start_time = 0;
    while(sequence.sequence_time == -1) {
        run_for_millis(5);
        start_time += 5;
        TEST_ASSERT_EQUAL(globalProfile.datapoints[0].temperature, regulator_get_target_temperature());
        if(--time_guard < 0) {
            TEST_FAIL();
        }
    }

    for (int i = 0; i < globalProfile.totalDatapoints; i++) {
        //print_mock_state();
        //DEBUG_TEST_OUTPUT("Step: %d", i);
        TEST_ASSERT_FLOAT_WITHIN(15, globalProfile.datapoints[i].temperature, vpo_mock_model.pcb_temperature);
        
        // Test TAL Heater regulator disable
        if(globalProfile.datapoints[i].temperature > TAL_PHASE_THRESHOLD_TEMPERATURE) {
            TEST_ASSERT_FALSE(heater_regulation_enabled());
            TEST_ASSERT_TRUE(vpo_mock_model.heater_pwm_started);
        } else {
            TEST_ASSERT_TRUE(heater_regulation_enabled());
        }

        run_for_seconds(globalProfile.datapoints[i].time);
    }

    TEST_ASSERT_EQUAL(0, vpo_mock_model.heater_pwm_started);

    while(State_getState().reflow_active) {
        run_for_seconds(1);
    }

    TEST_ASSERT_EQUAL(DIRECTION_NONE, lift_axis.motor->direction);
    TEST_ASSERT_GREATER_OR_EQUAL(0, vpo_mock_model.lift_position);
    TEST_ASSERT_LESS_OR_EQUAL(100, vpo_mock_model.heater_temperature);
}

void test_regulator() {
    heater_enable();
    pump_enable();

    run_for_seconds(20);

    regulator_set_temp(230);
    regulator_enable();

    run_for_seconds(40);
    
    TEST_ASSERT_GREATER_OR_EQUAL(220, vpo_mock_model.pcb_temperature);

    regulator_set_temp(50);

    run_for_seconds(80);

    TEST_ASSERT_LESS_OR_EQUAL(55, vpo_mock_model.pcb_temperature);
}

int main() {
    HAL_Init();
    HAL_Delay(2000);    // service delay
    UNITY_BEGIN();

    RUN_TEST(test_untouched_system);
    RUN_TEST(test_sequence);
    RUN_TEST(test_regulator);

    UNITY_END();

    while(1){}
}


void SysTick_Handler(void) {
    HAL_IncTick();
}