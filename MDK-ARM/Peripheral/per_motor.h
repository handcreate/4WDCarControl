#ifndef __PER_MOTOR_H
#define __PER_MOTOR_H

#include "gpio.h"
#include "tim.h"

typedef struct {
    TIM_HandleTypeDef *htim;
    int16_t channel;
    GPIO_TypeDef *dir_port1; // 控制方向引脚1的端口
    uint16_t dir_pin1; // 控制方向引脚1
    GPIO_TypeDef *dir_port2; // 控制方向引脚2的端口
    uint16_t dir_pin2; // 控制方向引脚2
    int16_t speed; // 当前速度
    int8_t dir; // 当前方向，1表示正转，-1表示反转
} MotorStruct;

void per_motor_init(MotorStruct *motor);

/**********************************************************************
 * @brief    设置电机速度，取值范围[-1000, 1000]，>0表示正转，<0表示反转
 * @param[in/out] motor <参数注释>
 * @param[in/out] speed <参数注释>
 * 
 * @note     <特别提醒>
**********************************************************************/
void per_motor_set_speed(MotorStruct *motor, int16_t speed);

#endif
