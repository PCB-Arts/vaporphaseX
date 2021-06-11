#include "main.h"
#include <unity.h>
#include "test_util.h"

#include "peripherals/peripheral_mock.h"
#include "main/init.h"
#include "ui/cli_commands.h"

void setUp(void) {
    init_modules();
}

void tearDown(void) {
    
}

void enable_test() {
    vpo_mock_model.heater_pwm_started = 0;

    heater_enable();

    TEST_ASSERT_EQUAL(1, vpo_mock_model.heater_pwm_started);

    heater_disable();
    TEST_ASSERT_EQUAL(0, vpo_mock_model.heater_pwm_started);
}

void cli_test() {
    vpo_mock_model.heater_pwm_started = 0;

    cli_commands_execute("heater en 1");

    TEST_ASSERT_EQUAL(1, vpo_mock_model.heater_pwm_started);

    cli_commands_execute("heater en 0");
    TEST_ASSERT_EQUAL(0, vpo_mock_model.heater_pwm_started);
}

int main() {
    HAL_Init();
    HAL_Delay(2000);    // service delay
    UNITY_BEGIN();

    RUN_TEST(enable_test);
    RUN_TEST(cli_test);

    UNITY_END();

    while(1){}
}

void SysTick_Handler(void) {
    HAL_IncTick();
}