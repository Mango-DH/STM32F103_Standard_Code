#ifndef __MY_ERR_H
#define __MY_ERR_H

typedef enum MY_ERR
{
	MY_OK                = 0,
	MY_ERR_INVALID_PARAM = 1,   //无效参数
	MY_ERR_CORE_FAIL     = 2,
	
}My_Err_Type;

void MY_Err_Check(My_Err_Type err);

#endif
