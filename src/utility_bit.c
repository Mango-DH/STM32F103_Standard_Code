#include "utility_bit.h"
#include <stdio.h>
/**
* @name:  Set_Bit 
* @brief: 指定uint32_t二进制数据中某一位的值
* @param: num            存放要更改的32位数据
* @param: bit_position   要更改的位置，从右向左依次为0, 1, 2... ,31
* @param: value          要更改的值，传入的参数为0或者1
* @reval: 操作是否成功，成功返回1，否则返回-1
*/
int Set_Bit(uint32_t *num, uint8_t bit_position, uint8_t value) 
{
    if (num == NULL || bit_position >= 32 || (value != 0 && value != 1)) 
		{
        return -1;
    }
    if (value == 1) 
		{   
        *num |= ((uint32_t)1 << bit_position);    // <<左移运算右边的位置补0
    } 
		else 
		{
        *num &= ~((uint32_t)1 << bit_position);
    }
    return 1;
}

/**
* @name:  Toggle_Bit 
* @brief: 反转 uint32_t  二进制数据中某一位的值
* @param: num            存放要更改的32位数据
* @param: bit_position   要翻转的位置，从右向左依次为0, 1, 2... ,31
* @reval: 操作是否成功，成功返回1，否则返回-1
*//**
* @name:  Toggle_Bit 
* @brief: 反转 uint32_t  二进制数据中某一位的值
* @param: num            存放要更改的32位数据
* @param: bit_position   要翻转的位置，从右向左依次为0, 1, 2... ,31
* @reval: 操作是否成功，成功返回1，否则返回-1
*/int Toggle_Bit(uint32_t *num, uint8_t bit_position) 
{
    if (num == NULL || bit_position >= 32) 
		{
        return -1;
    }
    *num ^= ((uint32_t)1 << bit_position);  
		//按位异或，运算时若两个数字不同则为1，否则为0
    //0与1异或得1，0与0异或得0，1与1异或得0，1与0异或得1

    return 1;
}


/**
* @name:  Combine_Bytes 
* @brief: 输入四个uint8_t,按高低八位拼接成uint32_t
* @param: byte1，byte2, byte3, byte4     存放要凭借的四个数据
* @param: result                         存放拼接好的数据
* @reval: 操作是否成功,成功返回1，否则返回-1
*/int Combine_Bytes(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint32_t *result) 
{
    if (result == NULL)
		{
        return -1;
    }
    *result = ((uint32_t)byte1 << 24) | ((uint32_t)byte2 << 16) | ((uint32_t)byte3 << 8) | byte4;
    return 1;
}

/**
* @name:  Split_uint32 
* @brief: 输入一个uint32_t, 分别获取拆出来的uint8_t
* @param: byte1，byte2, byte3, byte4     存放要凭借的四个数据
* @reval: 操作是否成功,成功返回1,否则返回-1 
*/int Split_uint32(uint32_t num, uint8_t *byte1, uint8_t *byte2, uint8_t *byte3, uint8_t *byte4) 
{
    if (byte1 == NULL || byte2 == NULL || byte3 == NULL || byte4 == NULL) 
		{
        return -1;
    } 
    *byte1 = (num >> 24) & 0xFF;
    *byte2 = (num >> 16) & 0xFF;
    *byte3 = (num >> 8) & 0xFF;
    *byte4 = num & 0xFF;  //0xFF  1111 1111		
    return 0;
}

/**
* @name:  Loop_Test 
* @brief: 循环测试函数 Set_Bit的功能
* @reval: None
*/
void Loop_Test(void)
{
	uint32_t m_bit32;
	for (uint32_t i = 0; i <= 0xFFFF; i++)
	{
    for (unsigned int bit = 0; bit < 32; bit++) 
		{  
			//测试所有可能的bit位
       //测试将bit位设置为1
       m_bit32 = i;
					
       Set_Bit(&m_bit32, bit, 1);
			__NOP();
				
       //测试将bit位设置为0
       Set_Bit(&m_bit32, bit, 0);
			__NOP();
    }				
  }
}

/**
* @name:  Bit_Test 
* @brief: 测试当前文件中函数的功能
* @reval: None
*/
void Bit_Test(void)
{
	uint32_t m_bit32 = 0x12345678;
	uint8_t byte1, byte2, byte3, byte4;
	uint32_t result;
  int test01val = Set_Bit(&m_bit32, 34, 1);	
	__NOP();
	int test02val = Toggle_Bit(&m_bit32, 33);
	__NOP();

	  byte1 = 0x12;
    byte2 = 0x34;
    byte3 = 0x56;
    byte4 = 0x78;
	int test03val = Combine_Bytes(byte1, byte2, byte3, byte4, &result) ;	
	__NOP();
	result = 0x78945612;
	int test04val = Split_uint32(result, &byte1, &byte2, &byte3, &byte4);
	__NOP();
}
// ~a = -(a+1)
