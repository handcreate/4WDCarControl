#include "app_freertos_task.h"
#include "com_debug.h"
#include "app_data_receive.h"

// 测试任务
void test_task( void *args );
#define TEST_TASK_STACK_SIZE 128
#define TEST_TASK_PRIORITY 1
#define TEST_TASK_NAME "test_task"
#define TEST_TASK_PERIOD 1000
TaskHandle_t test_task_handle;

// 通讯任务
void comm_task( void *args );
#define COMM_TASK_STACK_SIZE 128
#define COMM_TASK_PRIORITY 2
#define COMM_TASK_NAME "comm_task"
#define COMM_TASK_PERIOD 6
TaskHandle_t comm_task_handle;

void app_freertos_start(void)
{
    // 创建测试任务
    // xTaskCreate(test_task, 
    //     TEST_TASK_NAME, 
    //     TEST_TASK_STACK_SIZE, 
    //     NULL, 
    //     TEST_TASK_PRIORITY, 
    //     &test_task_handle);

    // 创建通讯任务
    xTaskCreate(comm_task, 
        COMM_TASK_NAME, 
        COMM_TASK_STACK_SIZE, 
        NULL, 
        COMM_TASK_PRIORITY, 
        &comm_task_handle);

    vTaskStartScheduler();
}

void test_task( void *args )
{
    while (1)
    {
        DEBUG_PRINTF("test_task running\n");
        vTaskDelay(TEST_TASK_PERIOD);
    }
}

void comm_task(void *args)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        // 接收数据
        uint8_t res = app_receive_data();

        // 处理连接状态
        app_process_connect_state(res);

        // 收到关机指令，通知电源任务关机
        // if(remote_data.shutdown == 1)
        // {
        //     xTaskNotifyGive(power_task_handle);
        // }

        // 处理飞行状态
        app_process_flight_state();

        vTaskDelayUntil(&xLastWakeTime, COMM_TASK_PERIOD);
    }
}
