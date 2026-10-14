#ifndef COM_DEBUG_H
#define COM_DEBUG_H

#include "usart.h"
#include "stdio.h"
#include "string.h"

// 声明调试串口宏
#define DEBUG_USART huart1

// 申明调试开关
#define DEBUG_ON 1

// 定义printf宏定义，显示所在文件和行号
#if DEBUG_ON
// 使用宏定义只打印文件名，不打印路径
#define __FILENM__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define __FILENAME__ (strrchr(__FILENM__, '\\') ? strrchr(__FILENM__, '\\') + 1 : __FILENM__)
#define DEBUG_PRINTF(format, ...) printf("[%s:%d] " format, __FILENAME__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINTF(format, ...)
#endif

#endif // COM_DEBUG_H
