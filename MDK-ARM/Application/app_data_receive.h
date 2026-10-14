#ifndef __APP_DATA_RECEIVE_H__
#define __APP_DATA_RECEIVE_H__

#include "main.h"

// 定义三位帧头校验码
#define FRAME_HEAD_CODE_1 'X'
#define FRAME_HEAD_CODE_2 'I'
#define FRAME_HEAD_CODE_3 'S'

// 最大重试次数
#define MAX_RETRY_TIMES 10


// 遥控状态定义
typedef enum
{
    CONNECTED,
    DISCONNECTED
} RemoteState;

// 飞行状态定义
typedef enum
{
    IDLE = 0,
    NORMAL,
    FIX_HEIGHT,
    FAIL
} FlightState;

// 油门状态定义
typedef enum
{
    FREE = 0,
    MAX,
    LEAVE_MAX,
    MIN,
    UNLOCK,
} ThrottleState;

// 遥控数据结构体
typedef struct
{
    int16_t thr;
    int16_t yaw;
    int16_t pit;
    int16_t rol;
    uint8_t shutdown; // 关机
    uint8_t fix_height; // 定高
} RemoteData;


/**********************************************************************
 * @brief    接收数据处理函数
 * @return uint8_t       0:接收成功 1:接收失败 或 没有接收到数据
 * 
 * @note     <特别提醒>
**********************************************************************/
uint8_t app_receive_data(void);

/**********************************************************************
 * @brief    根据接收数据处理函数的返回值，处理连接状态
 * @param[in/out] res   <参数注释>
 * 
 * @note     <特别提醒>
**********************************************************************/
void app_process_connect_state(uint8_t res);

/**********************************************************************
 * @brief    使用状态机处理飞行状态
 * 
 * @note     <特别提醒>
**********************************************************************/
void app_process_flight_state(void);

#endif /* __APP_DATA_RECEIVE_H__ */

