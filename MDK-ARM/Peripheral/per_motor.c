#include "per_motor.h"

void per_motor_init(MotorStruct *motor) 
{
    __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, 0);
    HAL_TIM_PWM_Start(motor->htim, motor->channel);
}

void per_motor_set_speed(MotorStruct *motor, int16_t speed) 
{
    if(speed < 0) 
    {
        motor->dir = -1;
        motor->speed = -speed;
        if(motor->speed > 1000) 
        {
            motor->speed = 1000;
        }
    }
    else if(speed > 0)
    {
        motor->dir = 1;
        motor->speed = speed;
        if(motor->speed > 1000) 
        {
            motor->speed = 1000;
        }
    }
    else
    {
        // 注意这里不要修改dir的值
        motor->speed = 0;
    }

    // 设置两个引脚的电平来控制电机的方向
    if(motor->dir == -1)
    {
        HAL_GPIO_WritePin(motor->dir_port1, motor->dir_pin1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->dir_port2, motor->dir_pin2, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(motor->dir_port1, motor->dir_pin1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(motor->dir_port2, motor->dir_pin2, GPIO_PIN_RESET);
    }
    // 设置PWM的占空比来控制电机的速度
    __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, motor->speed);
}

