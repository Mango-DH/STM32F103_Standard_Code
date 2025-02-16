#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <cmsis_compiler.h>


#define MAX_QUEUES 1  // 最大支持的队列数量

typedef struct {
    uint8_t *base;        // 队列的基地址
    size_t element_size;  // 每个元素的大小
    size_t capacity;      // 队列的容量（元素数量）
    size_t front;         // 队列头索引
    size_t rear;          // 队列尾索引
    size_t count;         // 队列中已使用的元素数量
} Queue;


void Queue_Init(Queue *queue, void *buffer, size_t element_size, size_t capacity);
bool Queue_Enqueue(Queue *queue, void *element);
bool Queue_Dequeue(Queue *queue, void *element) ;
bool Queue_Is_Full(const Queue *queue) ;
bool Queue_Is_Empty(const Queue *queue) ;
void Queue_Test(Queue *queue);

#endif
