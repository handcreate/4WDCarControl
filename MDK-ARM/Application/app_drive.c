#include "app_drive.h"

#include "com_debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_data_receive.h"

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

// 遥控数据
extern RemoteData remote_data;
// 遥控状态
extern RemoteState remote_state;

void app_drive_init(void)
{
    per_motor_init(&front_left_motor);
    per_motor_init(&front_right_motor);
    per_motor_init(&back_left_motor);
    per_motor_init(&back_right_motor);
}

void app_drive_run(void)
{
    // 如果遥控器没有连接，则停止
    if(remote_state != CONNECTED)
    {
        app_drive_stop();
        return;
    }

    // 计算4个电机的直行速度，使用遥控器的 pitch 数据，
    // 0~450为后退，450~550为停止，550~1000为前进
    int16_t thr = remote_data.pit; // 这里赋值给变量是为了保证设置过程中不会被其他线程修改
    int16_t speed = 0;
    if(thr < 450)
    {
        speed = 1000 * (thr - 450) / 450;
    }
    else if(thr > 550)
    {
        speed = 1000 * (thr - 550) / 450;
    }

    // 使用差速转向，设置4个电机的速度，使用遥控器的 yaw 数据，
    // 0~450为左转，450~550为直行，550~1000为右转
    int16_t speed_left = speed;
    int16_t speed_right = speed;
    int16_t dir = remote_data.yaw;
    if(dir < 450)
    {
        speed_left = speed * (dir - 450) / 450;
    }
    else if(dir > 550)
    {
        speed_right = speed * (550 - dir) / 450;
    }
    
    per_motor_set_speed(&front_left_motor, speed_left);
    per_motor_set_speed(&front_right_motor, speed_right);
    per_motor_set_speed(&back_left_motor, speed_left);
    per_motor_set_speed(&back_right_motor, speed_right);
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
