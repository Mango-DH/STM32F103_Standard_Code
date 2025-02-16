#include <cmsis_compiler.h>
#include "utility_error.h"

/***
* @filename:utility_error.h
* @brief:为HAL层功能提供通用的错误处理接口
***/
void MY_Err_Check(My_Err_Type err)
{
	while(1)
	{
		__NOP();
	}
}
