#include "utility_stack.h"
#include <string.h>
#include <stdlib.h>
/**
* @name:  Stack_Init 
* @brief: 初始化栈
* @param: stack   			  指向要操作的栈结构体
* @param: buffer    		  指向栈中元素，分配给这个栈的内存空间的起始地址
* @param: element_size		表示每个栈元素的大小（以字节为单位）决定栈中每个元素所占的内存空间
* @param: capacity    		栈中最多容纳的元素数量
* @reval: None
*/
void Stack_Init(Stack *stack, void *buffer, size_t element_size, size_t capacity) 
{
    stack->base = (uint8_t *)buffer;
    stack->top = stack->base;
    stack->element_size = element_size;
    stack->capacity = capacity;
    stack->count = 0;
    stack->is_full = false;
}

/**
* @name:  Stack_Push 
* @brief: 支持任意大小的压栈操作
* @param: stack        		指向要操作的栈结构体
* @param: element      		该参数指向要压入栈的元素。stack_push 函数将把这个元素复制到栈的栈顶位置
* @reval: 操作是否成功,成功返回true, 否则返回false
*/
bool Stack_Push(Stack *stack, void *element) 
{
    if (stack->count >= stack->capacity) 
		{
        stack->is_full = true;
        return false;  // 栈满
    }

    // 将元素复制到栈顶位置
    memcpy(stack->top, element, stack->element_size);
    stack->top += stack->element_size;
    stack->count++;

    if (stack->count >= stack->capacity) {
        stack->is_full = true;
    }
    return true;
}

/**
* @name:  Stack_Pop 
* @brief: 从栈中弹出数据
* @param: stack       		指向要操作的栈结构体
* @param: element      		该参数用于存储从栈中弹出的元素，Stack_Pop 函数将把栈顶的元素复制到 element 所指向的内存区域
* @reval: 操作是否成功,成功返回true, 否则返回false
*/
bool Stack_Pop(Stack *stack, void *element)   //弹栈，弹出数据
{
    if (stack->count == 0) 
		{
        return false;  // 栈空
    }

    stack->top -= stack->element_size;
    memcpy(element, stack->top, stack->element_size);
    stack->count--;
    stack->is_full = false;
    return true;
}

/**
* @name:  Stack_Pop 
* @brief: 检查栈是否满了
* @param: stack       		指向要操作的栈结构体
* @reval: 栈满返回true, 否则返回false
*/
bool Stack_Is_Full(const Stack *stack) 
{
    return stack->is_full;
}

/**
* @name:  Stack_Pop 
* @brief: 检查栈是否为空
* @param: stack       		指向要操作的栈结构体
* @reval: 操作是否成功,成功返回true, 否则返回false
*/
bool Stack_Is_Empty(const Stack *stack) 
{
    return stack->count == 0;
}


/**
* @name:  Stack_Test 
* @brief: stack.h文件函数功能检查
* @reval: None
*/
void Stack_Test(void)
{
		volatile bool stack_is_full;
	  volatile bool stack_is_empty;
	
	/*	
	volatile 关键字在 C 语言中用于告诉编译器，一个变量的值可能会在任何时间发生变化，
	即使在代码中没有明显的操作。这在嵌入式系统中非常常见，特别是当变量的值可能被硬件、外部设备或中断服务程序（ISR）修改时
	通常情况下，编译器会优化代码，比如它可能会将某个变量的值保存在寄存器中，
	如果编译器认为该变量的值不会改变，就可能不再从内存中读取这个变量的值
	然而，当一个变量被声明为 volatile 时，编译器会停止这样的优化，
	并确保每次都从内存中读取变量的最新值
	*/
	
	
    Stack stacks[MAX_STACKS];
	
    // 为每个栈分配固定大小的内存空间
    uint8_t stack_buffers[MAX_STACKS][10]; // 每个栈的容量为10个元素

 
    for (int i = 0; i < MAX_STACKS; i++) // 初始化栈
		{
        Stack_Init(&stacks[i], stack_buffers[i], sizeof(int), 10);
    }

       
    for (int i = 0; i < 10; i++) // 测试压栈操作
		{ 
			int value = i;
        if (!Stack_Push(&stacks[0], &value)) 
				{
           stack_is_full = Stack_Is_Full(&stacks[0]);
           break;
        }
    }
		__NOP();   //检查 stack_is_full的值

		stack_is_full = Stack_Is_Full(&stacks[0]);
		__NOP();
		
    // 测试弹栈操作
    int popped_value;
    while (Stack_Pop(&stacks[0], &popped_value)) {
        __NOP();    //检查 popped_value的值
    }
		stack_is_empty = Stack_Is_Empty(&stacks[0]);   //清除栈
		__NOP();
}

