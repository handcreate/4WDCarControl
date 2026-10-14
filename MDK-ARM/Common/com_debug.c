#include "com_debug.h"

/**********************************************************************
 * @brief    重定向printf函数
 * @param[in] ch    要发送的字符
 * @param[in] f     文件指针（未使用）
 * @return int      返回发送的字符
 * 
 * @note     <特别提醒>
**********************************************************************/
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&DEBUG_USART, (uint8_t *)&ch, 1, 1000);
    return ch;
}
