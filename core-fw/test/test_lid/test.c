#include "main.h"
#include <unity.h>
#include "test_util.h"

#include "peripherals/peripheral_mock.h"
#include "main/init.h"
#include "ui/cli_commands.h"
#include "control/state.h"

#include "modules/axis_isr.h"
#include "modules/lid.h"

#define CALIBRATION_WAITING_TIME_MS 500

extern volatile char cli_send_buffer[];

void assert_motor_up() {
    TEST_ASSERT_EQUAL(1, vpo_mock_model.lid_motor_halfbridge_a);
    TEST_ASSERT_EQUAL(0, vpo_mock_model.lid_motor_halfbridge_b);
}

void assert_motor_down() {
    TEST_ASSERT_EQUAL(0, vpo_mock_model.lid_motor_halfbridge_a);
    TEST_ASSERT_EQUAL(1, vpo_mock_model.lid_motor_halfbridge_b);
}

void assert_motor_stop() {
    TEST_ASSERT_EQUAL(1, vpo_mock_model.lid_motor_halfbridge_a);
    TEST_ASSERT_EQUAL(1, vpo_mock_model.lid_motor_halfbridge_b);
}

void setUp(void) {
    // init inputs
    init_modules();

    vpo_mock_model.lid_end_bottom = 0;
    vpo_mock_model.lid_end_top = 0;
    vpo_mock_model.lid_lock_monitor = 0;
    vpo_mock_model.lid_overcurrent = 0;
    lid_axis.lim_sw_fwd.trigger_propagation_delay_ms = 0;
    lid_axis.lim_sw_bwd.trigger_propagation_delay_ms = 0;

    TEST_ASSERT_EQUAL_STRING("ok stop", lid_status());
    assert_motor_stop();

    lid_calibrate();
    lid_worker();

    TEST_ASSERT_EQUAL_STRING("cal bwd", lid_status());
    assert_motor_down();

    vpo_mock_model.lid_end_bottom = 1;
    axis_limsw_bwd_isr(&lid_axis);
    lid_worker();

    TEST_ASSERT_EQUAL_STRING("ok stop", lid_status());
    assert_motor_stop();

    // easy to for test setup so that not each test has to clear pin
    vpo_mock_model.lid_end_bottom = 0;
}

void tearDown(void) {
    
}

void up_test() {
    cli_commands_execute("lid up");
    
    lid_worker();
    TEST_ASSERT_EQUAL_STRING("ok fwd", lid_status());
    assert_motor_up();

    cli_commands_execute("lid stop");
    TEST_ASSERT_EQUAL_STRING("ok stop", lid_status());
    assert_motor_stop();
}

void down_test() {
    cli_commands_execute("lid down");
    
    lid_worker();
    TEST_ASSERT_EQUAL_STRING("ok bwd", lid_status());
    assert_motor_down();

    cli_commands_execute("lid stop");
    TEST_ASSERT_EQUAL_STRING("ok stop", lid_status());
    assert_motor_stop();
}

void calibration_test() {
    cli_commands_execute("lid cal");

    TEST_ASSERT_EQUAL_STRING("cal stop", lid_status());
    lid_worker();
    TEST_ASSERT_EQUAL_STRING("cal bwd", lid_status());
    TEST_ASSERT_EQUAL(AXIS_CAL_FIND, axis_status(&lid_axis));
    assert_motor_down();

    vpo_mock_model.lid_end_bottom = 1;
    axis_limsw_bwd_isr(&lid_axis);
    lid_worker();

    TEST_ASSERT_EQUAL_STRING("ok stop", lid_status());
    assert_motor_stop();
}

void calibration_test_active_limit_switch() {
    vpo_mock_model.lid_end_bottom = 1;

    cli_commands_execute("lid cal");

    TEST_ASSERT_EQUAL_STRING("cal stop", lid_status());
    lid_worker();
    TEST_ASSERT_EQUAL_STRING("cal fwd", lid_status());
    TEST_ASSERT_EQUAL(AXIS_CAL_FREE, axis_status(&lid_axis));
    assert_motor_up();

    vpo_mock_model.lid_end_bottom = 0;
    lid_worker();

    TEST_ASSERT_EQUAL("cal stop", lid_status());
    lid_axis.cal_free_stopped_timestamp = HAL_GetTick() - CALIBRATION_WAITING_TIME_MS;
    lid_worker();

    TEST_ASSERT_EQUAL_STRING("cal bwd", lid_status());
    TEST_ASSERT_EQUAL(AXIS_CAL_FIND, axis_status(&lid_axis));
    assert_motor_down();

    vpo_mock_model.lid_end_bottom = 1;
    axis_limsw_bwd_isr(&lid_axis);
    lid_worker();

    TEST_ASSERT_EQUAL_STRING("ok stop", lid_status());
    assert_motor_stop();
}

void position_test() {
    // test initially calibrated position
    cli_commands_execute("lid pos");
    TEST_ASSERT_EQUAL_STRING("lid pos: 0\r\n", cli_send_buffer);

    // the sign of the signal is inverted so -20
    vpo_mock_model.lid_encoder_value = -20;
    lid_encoder_worker();

    cli_commands_execute("lid pos");
    TEST_ASSERT_EQUAL_STRING("lid pos: 20\r\n", cli_send_buffer);
}

// this test intends to check the general behaviour of the axis implementation
void axis_limit_switch_test() {
    // set position to center to prevent position boundary checks
    lid_axis.encoder->position = lid_axis.pos_min + (lid_axis.pos_max - lid_axis.pos_min) / 2;
    
    vpo_mock_model.lid_end_bottom = 1;
    axis_limsw_bwd_isr(&lid_axis);
    axis_move(&lid_axis, BWD);

    assert_motor_stop();

    // use big delta as small delta can get into min delta check
    axis_move_to(&lid_axis, lid_axis.pos_min);

    assert_motor_stop();

    vpo_mock_model.lid_end_bottom = 0;
    vpo_mock_model.lid_end_top = 1;
    axis_limsw_fwd_isr(&lid_axis);
    axis_move(&lid_axis, FWD);

    assert_motor_stop();

    axis_move_to(&lid_axis, lid_axis.pos_max);

    assert_motor_stop();
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
    RUN_TEST(axis_limit_switch_test);

    UNITY_END();

    while(1){}
}

void SysTick_Handler(void) {
    HAL_IncTick();
}
