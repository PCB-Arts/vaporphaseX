#include "main.h"
#include <unity.h>

#include "init.h"

#include "peripheral_mock.h"
#include "modules/pump.h"
#include "temp_sensors.h"
#include "test_util.h"

void setUp(void) {

}

void tearDown(void) {
    
}

void pump_test() {
    vpo_mock_model.pump_enable = 0;

    cli_commands_execute("pump man 1");

    TEST_ASSERT_EQUAL(vpo_mock_model.pump_enable, 1);
    TEST_ASSERT_EQUAL(false, pump_is_auto_enabled());

    cli_commands_execute("pump man 0");

    TEST_ASSERT_EQUAL(vpo_mock_model.pump_enable, 0);
    TEST_ASSERT_EQUAL(false, pump_is_auto_enabled());

    cli_commands_execute("pump auto");
    
    TEST_ASSERT_EQUAL(true, pump_is_auto_enabled());
}

int main() {
    HAL_Init();
    HAL_Delay(2000);    // service delay
    UNITY_BEGIN();

    RUN_TEST(pump_test);

    UNITY_END();

    while(1){}
}

void SysTick_Handler(void) {
    HAL_IncTick();
}