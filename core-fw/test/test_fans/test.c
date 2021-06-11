#include "main.h"
#include <unity.h>

#include "peripherals/peripheral_mock.h"
#include "main/init.h"
#include "ui/cli_commands.h"
#include "modules/fans.h"
#include "test_util.h"

void setUp(void) {
    init_modules();
}

void tearDown(void) {
    
}

void radiator_fan_cli_manual_test() {
    vpo_mock_model.radior_fan_pwm = 0;

    cli_commands_execute("hydro cooler man 0");

    //inverted pwm
    TEST_ASSERT_EQUAL(100, vpo_mock_model.radior_fan_pwm);

    cli_commands_execute("hydro cooler man 100");
    TEST_ASSERT_EQUAL(0, vpo_mock_model.radior_fan_pwm);
}

void radiator_fan_cli_auto_test() {
    vpo_mock_model.radior_fan_pwm = -1;
    // somewhat high temperature that should trigger fan
    vpo_mock_model.water_temperature = 90;

    cli_commands_execute("hydro cooler man 0");
    fan_regulate(&hydro_cooler);

    //inverted pwm
    TEST_ASSERT_EQUAL(100, vpo_mock_model.radior_fan_pwm);

    cli_commands_execute("hydro cooler auto");
    fan_regulate(&hydro_cooler);

    //inverted pwm
    TEST_ASSERT_LESS_OR_EQUAL_INT(50, vpo_mock_model.radior_fan_pwm);
}

void radiator_fan_proportional_regulator() {
    vpo_mock_model.radior_fan_pwm = -1;
    cli_commands_execute("hydro cooler auto");

    // min 0%
    vpo_mock_model.water_temperature = 20;
    fan_regulate(&hydro_cooler);
    TEST_ASSERT_EQUAL(100, vpo_mock_model.radior_fan_pwm);

    // max 100%
    vpo_mock_model.water_temperature = 30;
    fan_regulate(&hydro_cooler);
    TEST_ASSERT_EQUAL(0, vpo_mock_model.radior_fan_pwm);

    // 50°C ~ 50%
    vpo_mock_model.water_temperature = 25;
    fan_regulate(&hydro_cooler);
    TEST_ASSERT_EQUAL(50, vpo_mock_model.radior_fan_pwm);
}

void quick_cool_fan_cli_manual_test() {
    vpo_mock_model.quick_cool_fan_enable = 0;

    cli_commands_execute("quick cooler man 1");

    TEST_ASSERT_EQUAL(1, vpo_mock_model.quick_cool_fan_enable);

    cli_commands_execute("quick cooler man 0");
    TEST_ASSERT_EQUAL(0, vpo_mock_model.quick_cool_fan_enable);
}

void quick_cool_fan_cli_auto_test() {
    vpo_mock_model.quick_cool_fan_enable = 0;

    fan_set_auto_enabled(&quick_cooler, true);
    cli_commands_execute("quick cooler auto");

    vpo_mock_model.galden_temperature = 71;
    test_temperature_sensor_update();
    fan_regulate(&quick_cooler);

    TEST_ASSERT_EQUAL(1, vpo_mock_model.quick_cool_fan_enable);

    vpo_mock_model.galden_temperature = 60;
    test_temperature_sensor_update();
    fan_regulate(&quick_cooler);

    TEST_ASSERT_EQUAL(0, vpo_mock_model.quick_cool_fan_enable);
}

void lid_fan_default_auto_test() {
    vpo_mock_model.lid_fan_enable = 0;
    vpo_mock_model.galden_temperature = 101;
    test_temperature_sensor_update();
    fan_set_auto_enabled(&lid_fan, true);

    fan_regulate(&lid_fan);

    TEST_ASSERT_EQUAL(1, vpo_mock_model.lid_fan_enable);

    vpo_mock_model.galden_temperature = 90;
    test_temperature_sensor_update();

    fan_regulate(&lid_fan);

    TEST_ASSERT_EQUAL(0, vpo_mock_model.lid_fan_enable);
}

void lid_fan_cli_manual_test() {
    vpo_mock_model.lid_fan_enable = 0;

    cli_commands_execute("lid cooler man 1");

    TEST_ASSERT_EQUAL(1, vpo_mock_model.lid_fan_enable);

    cli_commands_execute("lid cooler man 0");
    TEST_ASSERT_EQUAL(0, vpo_mock_model.lid_fan_enable);
}

void lid_cooler_fan_cli_auto_test() {
    vpo_mock_model.lid_fan_enable = 0;
    vpo_mock_model.galden_temperature = 101;
    test_temperature_sensor_update();
    fan_set_auto_enabled(&lid_fan, true);

    cli_commands_execute("lid cooler auto");

    fan_regulate(&lid_fan);

    TEST_ASSERT_EQUAL(1, vpo_mock_model.lid_fan_enable);

    vpo_mock_model.galden_temperature = 90;
    test_temperature_sensor_update();

    fan_regulate(&lid_fan);

    TEST_ASSERT_EQUAL(0, vpo_mock_model.lid_fan_enable);
}

int main() {
    HAL_Init();
    HAL_Delay(2000);    // service delay
    UNITY_BEGIN();

    RUN_TEST(radiator_fan_cli_manual_test);
    RUN_TEST(radiator_fan_cli_auto_test);
    RUN_TEST(radiator_fan_proportional_regulator);
    RUN_TEST(quick_cool_fan_cli_manual_test);
    RUN_TEST(quick_cool_fan_cli_auto_test);
    RUN_TEST(lid_fan_default_auto_test);
    RUN_TEST(lid_fan_cli_manual_test);
    RUN_TEST(lid_cooler_fan_cli_auto_test);

    UNITY_END();

    while(1){}
}

void SysTick_Handler(void) {
    HAL_IncTick();
}