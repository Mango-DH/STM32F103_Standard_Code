#include "utility_queue.h"
/**
* @name:  Queue_Init 
* @brief: 为每个队列分配内存，并初始化队列结构体
* @param: queue     			指向要初始化的队列结构体
* @param: buffer    			分配给这个队列的内存空间的起始地址
* @param: element_size    每个队列元素的大小（以字节为单位）
* @param: capacity        队列的容量，即队列中最多可以容纳多少个元素
* @reval: None
*/
void Queue_Init(Queue *queue, void *buffer, size_t element_size, size_t capacity) 
{
    queue->base = (uint8_t *)buffer;
    queue->element_size = element_size;
    queue->capacity = capacity;
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
}

/**
* @name:  Queue_Enqueue 
* @brief: 将元素添加到队列尾部，入队操作
* @param: queue     			指向要操作的队列结构体
* @param: element         要追加的元素
* @reval: 操作是否成功,成功返回true, 否则返回false
*/
bool Queue_Enqueue(Queue *queue, void *element)
{
    if (queue->count >= queue->capacity) {
        return false;  // 队列满
    }

    // 计算尾部元素的位置
    uint8_t *destination = queue->base + (queue->rear * queue->element_size);
    // 将元素复制到队列尾部
    memcpy(destination, element, queue->element_size);
    queue->rear = (queue->rear + 1) % queue->capacity;  // 循环递增尾部索引
    queue->count++;

    return true;
}


/**
* @name:  Queue_Dequeue 
* @brief: 从队列头部移除数据，出队操作
* @param: queue     			指向要操作的队列结构体
* @param: element         要出去的元素
* @reval: 操作是否成功,成功返回true, 否则返回false
*/
bool Queue_Dequeue(Queue *queue, void *element) 
{
    if (queue->count == 0) {
        return false;  // 队列空
    }

    // 计算头部元素的位置
    uint8_t *source = queue->base + (queue->front * queue->element_size);
    // 将元素从队列头部复制到目标位置
    memcpy(element, source, queue->element_size);
    queue->front = (queue->front + 1) % queue->capacity;  // 循环递增头部索引
    queue->count--;

    return true;
}


/**
* @name:  Queue_Is_Full 
* @brief: 增加检查队列是否已满的函数
* @param: queue     			指向要操作的队列结构体
* @reval: 操作是否成功,成功返回true, 否则返回false
*/
bool Queue_Is_Full(const Queue *queue) 
{
    return queue->count >= queue->capacity;
}


/**
* @name:  Queue_Is_Empty 
* @brief: 增加检查队列是否为空的函数
* @param: queue     			指向要操作的队列结构体
* @reval: 操作是否成功,成功返回true, 否则返回false
*/
bool Queue_Is_Empty(const Queue *queue) 
{
    return queue->count == 0;
}


/**
* @name:  Queue_Test 
* @brief: queue.h文件函数功能检查
* @reval: None
*/
void Queue_Test(Queue *queue)
{
  Queue queues[MAX_QUEUES];
    uint8_t queue_buffers[MAX_QUEUES][10]; // 每个队列的容量为100个元素
    
    // 初始化队列
    for (int i = 0; i < MAX_QUEUES; i++) {
        Queue_Init(&queues[i], queue_buffers[i], sizeof(int), 100);
    }

    // 测试入队操作
    //printf("Testing Queue Enqueue...\n");
    for (int i = 0; i < 5; i++) {
        int value = i;  // 使用不同的值来测试入队操作
        if (!Queue_Enqueue(&queues[0], &value)) {
            //printf("Failed to enqueue value: %d\n", value);
            break;
        }
    }

    // 检查队列是否满
    bool queue_is_full = Queue_Is_Full(&queues[0]);
    //printf("Queue is full: %s\n", queue_is_full ? "true" : "false");

    // 测试出队操作
    //printf("Testing Queue Dequeue...\n");
    int dequeued_value;
    while (Queue_Dequeue(&queues[0], &dequeued_value)) {
        //printf("Dequeued value: %d\n", dequeued_value);
    }

    // 检查队列是否为空
    bool queue_is_empty = Queue_Is_Empty(&queues[0]);
    //printf("Queue is empty: %s\n", queue_is_empty ? "true" : "false");
}


