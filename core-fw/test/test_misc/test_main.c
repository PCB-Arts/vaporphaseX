#include "main.h"
#include <unity.h>

#include "init.h"

#include "peripheral_mock.h"
#include "temp_sensors.h"
#include "test_util.h"

extern volatile char cli_send_buffer[];

void setUp(void) {

}

void tearDown(void) {
    
}

void test_spi_mock() {
    vpo_mock_model.pcb_temperature = 20;

    for(int i = 0; i < 30; i++) {
        max31856_temp_sensors_worker();
    }

    TEST_ASSERT_EQUAL_FLOAT(20, temperature_sensor_get_temperature(&temperature_sensor_pcb));

    vpo_mock_model.pcb_temperature = 30;
    max31856_temperature_sensor_pcb.tasks.reg.LTC |= MAX31856_TASK_READ;
    for(int i = 0; i < 30; i++) {
        max31856_temp_sensors_worker();
    }

    TEST_ASSERT_EQUAL_FLOAT(30, temperature_sensor_get_temperature(&temperature_sensor_pcb));
}

void test_cli_otp_command() {
    vpo_mock_model.pcb_temperature = 20;

    cli_commands_execute("otp stat");
    TEST_ASSERT_EQUAL_STRING("heater otp: 0\r\ncoolant otp: 0\r\nlid lock: 0\r\n", cli_send_buffer);

    vpo_mock_model.heater_otp_status = 1;

    cli_commands_execute("otp stat");
    TEST_ASSERT_EQUAL_STRING("heater otp: 1\r\ncoolant otp: 0\r\nlid lock: 0\r\n", cli_send_buffer);

    vpo_mock_model.coolant_otp_status = 1;

    cli_commands_execute("otp stat");
    TEST_ASSERT_EQUAL_STRING("heater otp: 1\r\ncoolant otp: 1\r\nlid lock: 0\r\n", cli_send_buffer);

    vpo_mock_model.lid_lock_monitor = 1;

    cli_commands_execute("otp stat");
    TEST_ASSERT_EQUAL_STRING("heater otp: 1\r\ncoolant otp: 1\r\nlid lock: 1\r\n", cli_send_buffer);
    
}

void test_limit_switch() {
    IO_pin_t lid_end_top_pin = IO_PIN_TPL(LID_END_TOP_GPIO_Port, LID_END_TOP_Pin);
    limit_switch_t limit_switch = { .pin = lid_end_top_pin, .trigger_propagation_delay_ms = 1000 };

    vpo_mock_model.lid_end_top = GPIO_PIN_SET;

    limit_switch_init(&limit_switch);

    TEST_ASSERT_TRUE(limit_switch_active(&limit_switch));
}


int main() {
    HAL_Init();
    HAL_Delay(2000);    // service delay
    UNITY_BEGIN();

    RUN_TEST(test_spi_mock);
    RUN_TEST(test_cli_otp_command);
    RUN_TEST(test_limit_switch);

    UNITY_END();

    while(1){}
}

void SysTick_Handler(void) {
    HAL_IncTick();
}