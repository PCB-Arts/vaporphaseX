#include "main.h"
#include <unity.h>
#include "test_util.h"

#include "peripherals/peripheral_mock.h"
#include "main/init.h"
#include "ui/cli_commands.h"

#include "modules/axis_isr.h"
#include "modules/lift.h"

extern volatile char cli_send_buffer[];

void assert_motor_up() {
    TEST_ASSERT_EQUAL(1, vpo_mock_model.lift_enable);
    TEST_ASSERT_EQUAL(0, vpo_mock_model.lift_direction);
    TEST_ASSERT_LESS_OR_EQUAL_INT(1000, vpo_mock_model.lift_freq_interval_us);
    TEST_ASSERT_NOT_EQUAL(0, vpo_mock_model.lift_freq_interval_us);
}

void assert_motor_down() {
    TEST_ASSERT_EQUAL(1, vpo_mock_model.lift_enable);
    TEST_ASSERT_EQUAL(1, vpo_mock_model.lift_direction);
    TEST_ASSERT_LESS_OR_EQUAL_INT(1000, vpo_mock_model.lift_freq_interval_us);
    TEST_ASSERT_NOT_EQUAL(0, vpo_mock_model.lift_freq_interval_us);
}

void assert_motor_stop() {
    TEST_ASSERT_EQUAL(0, vpo_mock_model.lift_enable);
    TEST_ASSERT_EQUAL(0, vpo_mock_model.lift_tim_started);
}

void setUp(void) {
    // init inputs
    vpo_mock_model.lift_end_bottom = 0;
    vpo_mock_model.lift_end_top = 0;
    lift_axis.lim_sw_fwd.trigger_propagation_delay_ms = 0;
    lift_axis.lim_sw_bwd.trigger_propagation_delay_ms = 0;

    init_modules();
    lift_calibrate();

    lift_worker();
    assert_motor_up();

    vpo_mock_model.lift_end_top = 1;
    axis_limsw_fwd_isr(&lift_axis);
    lift_worker();
    assert_motor_stop();

    // easy to for test setup so that not each test has to clear pin
    vpo_mock_model.lift_end_top = 0;
}

void tearDown(void) {
    
}

void up_test() {
    cli_commands_execute("lift up");
    lift_worker();
    assert_motor_up();

    cli_commands_execute("lift stop");
    assert_motor_stop();
}

void down_test() {
    cli_commands_execute("lift down");
    
    lift_worker();
    assert_motor_down();

    cli_commands_execute("lift stop");
    assert_motor_stop();
}

void calibration_test() {
    cli_commands_execute("lift cal");

    lift_worker();
    TEST_ASSERT_EQUAL(AXIS_CAL_FIND, axis_status(&lift_axis));
    assert_motor_up();

    vpo_mock_model.lift_end_top = 1;
    axis_limsw_fwd_isr(&lift_axis);
    lift_worker();

    assert_motor_stop();
}

void calibration_test_active_limit_switch() {
    vpo_mock_model.lift_end_top = 1;

    cli_commands_execute("lift cal");

    lift_worker();
    TEST_ASSERT_EQUAL(AXIS_CAL_FREE, axis_status(&lift_axis));
    assert_motor_down();

    vpo_mock_model.lift_end_top = 0;
    lift_worker();
    TEST_ASSERT_EQUAL(AXIS_CAL_FREE, axis_status(&lift_axis));
    assert_motor_stop();

    lift_axis.cal_free_stopped_timestamp = HAL_GetTick() - 500;
    lift_worker();

    TEST_ASSERT_EQUAL(AXIS_CAL_FIND, axis_status(&lift_axis));
    assert_motor_up();

    vpo_mock_model.lift_end_top = 1;
    axis_limsw_fwd_isr(&lift_axis);
    lift_worker();

    assert_motor_stop();
}

void position_test() {
    // test initially calibrated position
    cli_commands_execute("lift pos");
    TEST_ASSERT_EQUAL_STRING("lift pos: 0\r\n", cli_send_buffer);

    cli_commands_execute("lift down");
    vpo_mock_model.lift_encoder_value = 150;
    freq_encoder_worker();

    cli_commands_execute("lift pos");
    TEST_ASSERT_EQUAL_STRING("lift pos: -150\r\n", cli_send_buffer);

    cli_commands_execute("lift up");
    vpo_mock_model.lift_encoder_value = 200;
    freq_encoder_worker();

    cli_commands_execute("lift pos");
    TEST_ASSERT_EQUAL_STRING("lift pos: -100\r\n", cli_send_buffer);
}

int main() {
    HAL_Init();
    HAL_Delay(2000);    // service delay
    UNITY_BEGIN();

    RUN_TEST(up_test);
    RUN_TEST(down_test);
    RUN_TEST(calibration_test);
    RUN_TEST(calibration_test_active_limit_switch);
    RUN_TEST(position_test);

    UNITY_END();

    while(1) {}
}

void SysTick_Handler(void) {
    HAL_IncTick();
}
