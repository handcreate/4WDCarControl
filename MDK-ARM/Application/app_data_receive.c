#include "app_data_receive.h"
#include "per_SI24R1.h"
#include "string.h"
#include "stdio.h"

// 遥控状态
RemoteState remote_state = DISCONNECTED;
// 飞行状态
FlightState flight_state = IDLE;
// 遥控数据
RemoteData remote_data = {
    .thr = 500,
    .yaw = 500,
    .pit = 500,
    .rol = 500,
    .shutdown = 0,
    .fix_height = 0
}; 

uint8_t comm_buffer[TX_PLOAD_WIDTH] = {0};
// 失联次数
uint8_t lost_count = 0;

ThrottleState thr_state = FREE; // 油门状态
uint32_t max_enter_time = 0; // 最大进入时间
uint32_t min_enter_time = 0; // 最小进入时间

uint8_t app_receive_data(void)
{
    memset(comm_buffer, 0, sizeof(comm_buffer));
    uint8_t res = per_SI24R1_RxPacket(comm_buffer);
    if(res != 0)
    {
        return 1;
    }

    if(strlen((char *)comm_buffer) == 0)
    {
        return 1;
    }

    // 帧头校验
    if(comm_buffer[0] != FRAME_HEAD_CODE_1 
        || comm_buffer[1] != FRAME_HEAD_CODE_2 
        || comm_buffer[2] != FRAME_HEAD_CODE_3) 
    {
        return 1;
    }

    // 数据校验和检查
    uint32_t sum = 0;
    for(uint8_t i = 0; i < 13; i++)
    {
        sum += comm_buffer[i];
    }
    uint32_t sum_rx = (comm_buffer[13] << 24) | (comm_buffer[14] << 16) | (comm_buffer[15] << 8) | comm_buffer[16];
    if(sum != sum_rx)
    {
        return 1;
    }

    // 数据解析
    remote_data.thr = (comm_buffer[3] << 8) | comm_buffer[4];
    remote_data.yaw = (comm_buffer[5] << 8) | comm_buffer[6];
    remote_data.pit = (comm_buffer[7] << 8) | comm_buffer[8];
    remote_data.rol = (comm_buffer[9] << 8) | comm_buffer[10];
    remote_data.shutdown = comm_buffer[11];
    remote_data.fix_height = comm_buffer[12];

    // printf("remote:%d,%d,%d,%d,%d,%d\n", 
    //     remote_data.thr, remote_data.yaw, remote_data.pit, 
    //     remote_data.rol, remote_data.shutdown, remote_data.fix_height);
    
    return 0;
}

void app_process_connect_state(uint8_t res)
{
    if(res == 0)
    {
        // 连接成功
        remote_state = CONNECTED;
        lost_count = 0;
    }
    else
    {
        lost_count++;
        if(lost_count >= MAX_RETRY_TIMES)
        {
            remote_state = DISCONNECTED;
            lost_count = 0;
        }
    }
}

/**********************************************************************
 * @brief    处理解锁
 * @return uint8_t       0: 解锁成功 1: 解锁失败
 * 
 * @note     <特别提醒>
**********************************************************************/
static uint8_t app_process_unlock(void)
{
    switch (thr_state)
    {
    case FREE:
        // 当油门小于 850 时，进入 MAX 状态，并记录进入时间
        if(remote_data.thr > 850)
        {
            thr_state = MAX;
            max_enter_time = HAL_GetTick();
        }
        break;
    case MAX:
        if(remote_data.thr < 850)
        {
            // 判断油门是否在 MAX 状态下持续达到 1 秒
            if(HAL_GetTick() - max_enter_time >= 1000)
            {
                thr_state = LEAVE_MAX;
            }
            else
            {
                thr_state = FREE;
            }
        }
        break;
    case LEAVE_MAX:
        // 当油门小于 100 时，进入 MIN 状态，并记录进入时间
        if(remote_data.thr < 100)
        {
            thr_state = MIN;
            min_enter_time = HAL_GetTick();
        }
        break;
    case MIN:
        // 判断油门是否在 MIN 状态下持续达到 1 秒
        if(HAL_GetTick() - min_enter_time < 1000)
        {
            if(remote_data.thr > 100)
            {
                thr_state = FREE;
            }
        }
        else
        {
            thr_state = UNLOCK;
        }
        break;
    case UNLOCK:
        /* code */
        break;
    
    default:
        break;
    }

    if(thr_state == UNLOCK)
    {
        return 0;
    }
    return 1;
} 

void app_process_flight_state(void)
{
    switch (flight_state)
    {
    case IDLE:
        if(app_process_unlock() == 0)
        {
            flight_state = NORMAL;
            thr_state = FREE;
        }
        break;
    case NORMAL:
        if(remote_data.fix_height == 1)
        {
            flight_state = FIX_HEIGHT;
            remote_data.fix_height = 0;
        }

        if(remote_state == DISCONNECTED)
        {
            flight_state = FAIL;
        }
        break;
    case FIX_HEIGHT:
        if(remote_data.fix_height == 1)
        {
            flight_state = NORMAL;
            remote_data.fix_height = 0;
        }

        if(remote_state == DISCONNECTED)
        {
            flight_state = FAIL;
        }
        break;
    case FAIL:
        // vTaskDelay(1);
        HAL_Delay(1);
        flight_state = IDLE;
        break;
    
    default:
        break;
    }
}
