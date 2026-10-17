#include "app_drive.h"

#include "com_debug.h"
#include "FreeRTOS.h"
#include "task.h"

// 电机结构体
MotorStruct front_left_motor = { 
    .htim = &htim2, 
    .channel = TIM_CHANNEL_3, 
    .dir_port1 = GPIOB,
    .dir_pin1 = CC_IN1_Pin, 
    .dir_port2 = GPIOB,
    .dir_pin2 = CC_IN2_Pin, 
    .dir = 1,
    .speed = 0 };
MotorStruct front_right_motor = { 
    .htim = &htim2, 
    .channel = TIM_CHANNEL_2, 
    .dir_port1 = GPIOB,
    .dir_pin1 = CB_IN1_Pin,
    .dir_port2 = GPIOB,
    .dir_pin2 = CB_IN2_Pin,
    .dir = 1,
    .speed = 0 };
MotorStruct back_left_motor = { 
    .htim = &htim2, 
    .channel = TIM_CHANNEL_4, 
    .dir_port2 = GPIOB,
    .dir_pin2 = CD_IN1_Pin,
    .dir_port1 = GPIOB,
    .dir_pin1 = CD_IN2_Pin,
    .dir = 1,
    .speed = 0 };
MotorStruct back_right_motor = { 
    .htim = &htim2, 
    .channel = TIM_CHANNEL_1, 
    .dir_port1 = GPIOA,
    .dir_pin1 = CA_IN1_Pin,
    .dir_port2 = GPIOA,
    .dir_pin2 = CA_IN2_Pin,
    .dir = 1,
    .speed = 0 };

void app_drive_init(void)
{
    per_motor_init(&front_left_motor);
    per_motor_init(&front_right_motor);
    per_motor_init(&back_left_motor);
    per_motor_init(&back_right_motor);
}

void app_drive_run(void)
{
    
}

void app_drive_stop(void)
{
    per_motor_set_speed(&front_left_motor, 0);
    per_motor_set_speed(&front_right_motor, 0);
    per_motor_set_speed(&back_left_motor, 0);
    per_motor_set_speed(&back_right_motor, 0);
}

void app_drive_test_single(MotorStruct *motor)
{
    motor->dir = 1;
    for(int16_t speed = 0; speed < 1000; speed += 100)
    {
        DEBUG_PRINTF("speed: %d, dir: %d\n", speed, motor->dir);
        per_motor_set_speed(motor, speed);
        vTaskDelay(1000);
    }
    motor->dir = -1;
    for(int16_t speed = 0; speed > -1000; speed -= 100)
    {
        DEBUG_PRINTF("speed: %d, dir: %d\n", speed, motor->dir);
        per_motor_set_speed(motor, speed);
        vTaskDelay(1000);
    }
    per_motor_set_speed(motor, 0);
}

void app_drive_test_seq(void)
{
    app_drive_test_single(&front_left_motor);
    app_drive_test_single(&front_right_motor);
    app_drive_test_single(&back_left_motor);
    app_drive_test_single(&back_right_motor);
}
