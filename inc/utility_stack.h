#ifndef __STACK_H
#define __STACK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <cmsis_compiler.h>

#define MAX_STACKS 10     // 栈的大小


typedef struct {
    uint8_t *base;        // 栈的基地址
    uint8_t *top;         // 栈的栈顶指针
    size_t element_size;  // 每个元素的大小
    size_t capacity;      // 栈的容量
    size_t count;         // 栈中已使用的元素数量
    bool is_full;         // 栈是否满
} Stack;


void Stack_Init(Stack *stack, void *buffer, size_t element_size, size_t capacity);
bool Stack_Push(Stack *stack, void *element);
bool Stack_Pop(Stack *stack, void *element);
bool Stack_Is_Full(const Stack *stack);
bool Stack_Is_Empty(const Stack *stack);
void Stack_Test(void);


#endif
