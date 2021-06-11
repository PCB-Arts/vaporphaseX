#include "main.h"
#include <unity.h>

#include "init.h"

#include "pid.h"

pid_param_t pid;

void setUp(void) {
    pid_reset(&pid);

    pid.y_min = -1000;
    pid.y_max = 1000;
    pid.time_step_size = 0.1;
    pid.Kp = 0;
    pid.Ki = 0;
    pid.Kd = 0;
}

void tearDown(void) {
    
}

void test_pid_proportional() {
    pid.Kp = 10;

    // error of 2
    TEST_ASSERT_EQUAL_FLOAT(20, pid_compute(&pid, 10, 8));

    // proportional term is time independent
    TEST_ASSERT_EQUAL_FLOAT(20, pid_compute(&pid, 10, 8));
}

void test_pid_integral() {
    pid.Ki = 20;

    // 2 for 100ms
    TEST_ASSERT_EQUAL_FLOAT(4, pid_compute(&pid, 10, 8));

    // 2 for 100ms and then 1 for 100ms
    TEST_ASSERT_EQUAL_FLOAT(6, pid_compute(&pid, 10, 9));

    // 2 for 100ms, 1 for 100ms and then 0 for 100ms
    TEST_ASSERT_EQUAL_FLOAT(6, pid_compute(&pid, 10, 10));
}

void test_pid_differential() {
    pid.Kd = 3;

    // set prev error
    pid_compute(&pid, 5, 4);

    // from 1 to 1 in 100ms
    TEST_ASSERT_EQUAL_FLOAT(0, pid_compute(&pid, 5, 4));
    
    // from 1 to 2 in 100ms
    TEST_ASSERT_EQUAL_FLOAT(30, pid_compute(&pid, 7, 5));
    
    // from 2 to -1 in 100ms
    TEST_ASSERT_EQUAL_FLOAT(-90, pid_compute(&pid, 2, 3));
}

int main() {
    HAL_Init();
    HAL_Delay(2000);    // service delay
    UNITY_BEGIN();

    RUN_TEST(test_pid_proportional);
    RUN_TEST(test_pid_integral);
    RUN_TEST(test_pid_differential);

    UNITY_END();

    while(1){}
}

void SysTick_Handler(void) {
    HAL_IncTick();
}