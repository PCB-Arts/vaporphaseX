#include "main.h"
#include <unity.h>

#include "init.h"
#include "loop.h"
#include "control/state.h"
#include "control/sequence.h"

#include "test_util.h"

void setUp(void) {
    init_modules();
}

void tearDown(void) {
    
}

void test_initial_state() {
    TEST_ASSERT_EQUAL(0, lift_axis.cal_done);
    TEST_ASSERT_EQUAL(0, lid_axis.cal_done);

    Vpo_CoreStateTypeDef state = State_getState();

    TEST_ASSERT_EQUAL(false, state.can_start_reflow);
    TEST_ASSERT_EQUAL(false, state.can_open_lid);
}

void test_boot_finished() {
    main_loop();

    // first start with calibrating the lid and then after lid is closed start with calibrating the lift
    TEST_ASSERT_NOT_EQUAL(AXIS_NORMAL, lid_axis.mode);
    TEST_ASSERT_EQUAL(AXIS_NORMAL, lift_axis.mode);

    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
    }

    TEST_ASSERT_EQUAL(1, lift_axis.cal_done);
    TEST_ASSERT_EQUAL(1, lid_axis.cal_done);
    TEST_ASSERT_EQUAL(true, axis_forward_limit_switch_active(&lift_axis));
    TEST_ASSERT_EQUAL(true, axis_backward_limit_switch_active(&lid_axis));

    Vpo_CoreStateTypeDef state = State_getState();

    TEST_ASSERT_EQUAL(true, state.can_start_reflow);
    TEST_ASSERT_EQUAL(true, state.can_open_lid);
}

void test_overall_state_lid_cooling_fan() {
    vpo_mock_model.temperature_simulation_enabled = false;

    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
    }

    TEST_ASSERT_EQUAL(false, vpo_mock_model.lid_fan_enable);

    // set model and sensor directly to skip time needed for sensor update
    vpo_mock_model.galden_temperature = 101;
    temperature_sensor_galden.max31856->temperature = 101;
    run_for_millis(5);

    TEST_ASSERT_GREATER_OR_EQUAL(101, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_LESS_OR_EQUAL(120, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(true, vpo_mock_model.lid_fan_enable);

    // set model and sensor directly to skip time needed for sensor update
    vpo_mock_model.galden_temperature = 95;
    temperature_sensor_galden.max31856->temperature = 95;
    run_for_millis(5);

    TEST_ASSERT_LESS_OR_EQUAL(95, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(false, vpo_mock_model.lid_fan_enable);
}

void test_overall_state_pump() {

    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
    }

    vpo_mock_model.temperature_simulation_enabled = false;

    TEST_ASSERT_EQUAL(false, vpo_mock_model.pump_enable);

    vpo_mock_model.water_temperature = 50;
    run_for_millis(100);

    TEST_ASSERT_GREATER_OR_EQUAL(40, temperature_sensor_get_temperature(&temperature_sensor_water));
    TEST_ASSERT_LESS_OR_EQUAL(70, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(true, vpo_mock_model.pump_enable);

    // set model and sensor directly to skip time needed for sensor update
    vpo_mock_model.galden_temperature = 150;
    temperature_sensor_galden.max31856->temperature = 75;
    vpo_mock_model.water_temperature = 20;
    run_for_millis(20);

    TEST_ASSERT_LESS_OR_EQUAL(40, temperature_sensor_get_temperature(&temperature_sensor_water));
    TEST_ASSERT_GREATER_OR_EQUAL(70, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(true, vpo_mock_model.pump_enable);
}

void test_profile_state_pump() {
    
    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
    }

    TEST_ASSERT_EQUAL(false, vpo_mock_model.pump_enable);

    load_example_profile(&globalProfile);
    seq_start(&globalProfile);

    run_for_seconds(1);

    TEST_ASSERT_LESS_OR_EQUAL(40, temperature_sensor_get_temperature(&temperature_sensor_water));
    TEST_ASSERT_LESS_OR_EQUAL(70, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(true, vpo_mock_model.pump_enable);

    seq_abort();
    run_for_seconds(1);

    TEST_ASSERT_LESS_OR_EQUAL(40, temperature_sensor_get_temperature(&temperature_sensor_water));
    TEST_ASSERT_LESS_OR_EQUAL(70, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(false, vpo_mock_model.pump_enable);
}


void test_overall_radiator_fans() {

    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
    }

    vpo_mock_model.water_temperature = 0;

    // inverted pwm value
    TEST_ASSERT_EQUAL(100 /* off */, vpo_mock_model.radior_fan_pwm);

    cli_commands_execute("pump man 1");
    run_for_seconds(1);

    TEST_ASSERT_EQUAL(true, vpo_mock_model.pump_enable);
    TEST_ASSERT_EQUAL(100 /* min value */, vpo_mock_model.radior_fan_pwm);

    vpo_mock_model.water_temperature = 100;
    run_for_millis(100);

    TEST_ASSERT_EQUAL(0 /* max value */, vpo_mock_model.radior_fan_pwm);

    cli_commands_execute("pump man 0");
    run_for_seconds(1);

    TEST_ASSERT_EQUAL(false, vpo_mock_model.pump_enable);
    TEST_ASSERT_EQUAL(100 /* off */, vpo_mock_model.radior_fan_pwm);

}

void test_overall_quick_cool_fans() {
    
    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
    }

    TEST_ASSERT_EQUAL(false, vpo_mock_model.quick_cool_fan_enable);

    // set model and sensor directly to skip time needed for sensor update
    vpo_mock_model.galden_temperature = 150;
    temperature_sensor_galden.max31856->temperature = 75;
    vpo_mock_model.water_temperature = 20;
    run_for_millis(20);

    TEST_ASSERT_GREATER_OR_EQUAL(70, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(true, vpo_mock_model.quick_cool_fan_enable);

    load_example_profile(&globalProfile);
    seq_start(&globalProfile);

    run_for_seconds(10);

    // quick cool fans should be off during heatup or run phase
    TEST_ASSERT_GREATER_OR_EQUAL(70, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(false, vpo_mock_model.quick_cool_fan_enable);

    seq_abort();

    run_for_seconds(1);
    
    // quick cool are on during cool down
    TEST_ASSERT_GREATER_OR_EQUAL(80, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(100, quick_cooler.level);
    TEST_ASSERT_EQUAL(true, vpo_mock_model.quick_cool_fan_enable);

    run_for_seconds(60);
 
    TEST_ASSERT_EQUAL(false, seq_running());
    TEST_ASSERT_EQUAL(false, vpo_mock_model.quick_cool_fan_enable);

    // test auto behaviour again

    // set model and sensor directly to skip time needed for sensor update
    vpo_mock_model.galden_temperature = 150;
    temperature_sensor_galden.max31856->temperature = 75;
    vpo_mock_model.water_temperature = 20;
    run_for_millis(20);

    TEST_ASSERT_GREATER_OR_EQUAL(70, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(true, vpo_mock_model.quick_cool_fan_enable);
}

void test_state_heater() {

    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
    }

    TEST_ASSERT_EQUAL(false, vpo_mock_model.heater_pwm_started);

    load_example_profile(&globalProfile);
    seq_start(&globalProfile);

    run_for_seconds(1);

    TEST_ASSERT_EQUAL(true, vpo_mock_model.heater_pwm_started);

    seq_abort();
    run_for_seconds(1);

    TEST_ASSERT_EQUAL(false, vpo_mock_model.heater_pwm_started);
}

void test_lid_open_close() {

    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
    }

    TEST_ASSERT_EQUAL(LID_STATE_CLOSED, State_getState().lid_state);
    TEST_ASSERT_EQUAL(true, State_getState().can_open_lid);

    State_SetLidOpen(true);
    
    TEST_ASSERT_EQUAL(false, State_getState().can_start_reflow);
    TEST_ASSERT_EQUAL(LID_STATE_OPENING, State_getState().lid_state);

    // to checkt that the lift is moved down we expect to start with only the lift moving
    TEST_ASSERT_EQUAL(DIRECTION_NONE, lid_axis.motor->direction);
    TEST_ASSERT_EQUAL(BWD, lift_axis.motor->direction);

    run_for_seconds(20);

    // check lift moved to 2cm (32000) under top position
    TEST_ASSERT_INT_WITHIN(AXIS_MIN_POS_OFFSET, -32000, lift_axis.encoder->position);
    TEST_ASSERT_EQUAL(LID_STATE_OPEN, State_getState().lid_state);
    TEST_ASSERT_EQUAL(false, State_getState().can_start_reflow);


    State_SetLidOpen(false);

    TEST_ASSERT_EQUAL(false, State_getState().can_start_reflow);
    TEST_ASSERT_EQUAL(LID_STATE_CLOSING, State_getState().lid_state);

    run_for_seconds(10);

    TEST_ASSERT_EQUAL(false, vpo_mock_model.lid_end_top);

    TEST_ASSERT_EQUAL(LID_STATE_CLOSED, State_getState().lid_state);
    TEST_ASSERT_EQUAL(true, State_getState().can_start_reflow);
}

void test_start_reflow_water_temperature_condition() {
    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
    }

    vpo_mock_model.temperature_simulation_enabled = false;

    vpo_mock_model.water_temperature = 59;

    run_for_seconds(1);

    TEST_ASSERT_EQUAL(true, State_getState().can_start_reflow);

    vpo_mock_model.water_temperature = 60.1;

    run_for_seconds(1);

    TEST_ASSERT_EQUAL(false, State_getState().can_start_reflow);

    vpo_mock_model.water_temperature = 59;

    run_for_seconds(1);

    TEST_ASSERT_EQUAL(true, State_getState().can_start_reflow);
}

#define LID_RELEASE_TEMPERATURE_HYSTERESE_CELSIUS 2

void test_lid_release_temperature() {

    vpo_mock_model.temperature_simulation_enabled = false;
    
    // release temperatures in °C
    const float normalReleaseTemperature = 80;
    const float ecoReleaseTemperature = 70;
    const float fastReleaseTemperature = 90;

    while(!State_getState().can_start_reflow) {
        run_for_seconds(1);
    }

    TEST_ASSERT_EQUAL(true, State_getState().can_open_lid);

    //default value is STANDARD
    TEST_ASSERT_EQUAL(VPO_STANDARD, State_getState().lid_open_mode);

    vpo_mock_model.galden_temperature = normalReleaseTemperature + 1;
    run_for_seconds(3);
    TEST_ASSERT_EQUAL(normalReleaseTemperature + 1, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(false, State_getState().can_open_lid);

    // test out hysterese

    vpo_mock_model.galden_temperature = normalReleaseTemperature - LID_RELEASE_TEMPERATURE_HYSTERESE_CELSIUS + 1;
    run_for_seconds(3);
    TEST_ASSERT_EQUAL(normalReleaseTemperature - LID_RELEASE_TEMPERATURE_HYSTERESE_CELSIUS + 1, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(false, State_getState().can_open_lid);

    vpo_mock_model.galden_temperature = normalReleaseTemperature - LID_RELEASE_TEMPERATURE_HYSTERESE_CELSIUS - 1;
    run_for_seconds(3);
    TEST_ASSERT_EQUAL(normalReleaseTemperature - LID_RELEASE_TEMPERATURE_HYSTERESE_CELSIUS - 1, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(true, State_getState().can_open_lid);



    State_SetLidOpenMode(VPO_ECO);
    TEST_ASSERT_EQUAL(VPO_ECO, State_getState().lid_open_mode);

    vpo_mock_model.galden_temperature = ecoReleaseTemperature + 1;
    run_for_seconds(3);
    TEST_ASSERT_EQUAL(ecoReleaseTemperature + 1, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(false, State_getState().can_open_lid);

    vpo_mock_model.galden_temperature = ecoReleaseTemperature - LID_RELEASE_TEMPERATURE_HYSTERESE_CELSIUS - 1;
    run_for_seconds(3);
    TEST_ASSERT_EQUAL(ecoReleaseTemperature - LID_RELEASE_TEMPERATURE_HYSTERESE_CELSIUS - 1, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(true, State_getState().can_open_lid);



    State_SetLidOpenMode(VPO_FAST);
    TEST_ASSERT_EQUAL(VPO_FAST, State_getState().lid_open_mode);

    vpo_mock_model.galden_temperature = fastReleaseTemperature + 1;
    run_for_seconds(3);
    TEST_ASSERT_EQUAL(fastReleaseTemperature + 1, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(false, State_getState().can_open_lid);

    vpo_mock_model.galden_temperature = fastReleaseTemperature - LID_RELEASE_TEMPERATURE_HYSTERESE_CELSIUS - 1;
    run_for_seconds(3);
    TEST_ASSERT_EQUAL(fastReleaseTemperature - LID_RELEASE_TEMPERATURE_HYSTERESE_CELSIUS - 1, temperature_sensor_get_temperature(&temperature_sensor_galden));
    TEST_ASSERT_EQUAL(true, State_getState().can_open_lid);
}

int main() {
    HAL_Init();
    HAL_Delay(2000);    // service delay
    UNITY_BEGIN();

    RUN_TEST(test_initial_state);
    RUN_TEST(test_boot_finished);
    RUN_TEST(test_overall_state_lid_cooling_fan);
    RUN_TEST(test_overall_state_pump);
    RUN_TEST(test_profile_state_pump);
    RUN_TEST(test_overall_radiator_fans);
    RUN_TEST(test_overall_quick_cool_fans);
    RUN_TEST(test_state_heater);
    RUN_TEST(test_lid_open_close);
    RUN_TEST(test_start_reflow_water_temperature_condition);
    RUN_TEST(test_lid_release_temperature);

    UNITY_END();

    while(1){}
}

void SysTick_Handler(void) {
    HAL_IncTick();
}