#ifndef __APP_DRIVE_H__
#define __APP_DRIVE_H__

#include "per_motor.h"

void app_drive_init(void);
void app_drive_run(void);
void app_drive_stop(void);
// 测试单个马达，执行顺序为：前进0~1000，后退0~1000
void app_drive_test_single(MotorStruct *motor);
// 依次顺序测试各个驱动
void app_drive_test_seq(void);

#endif // __APP_DRIVE_H__
