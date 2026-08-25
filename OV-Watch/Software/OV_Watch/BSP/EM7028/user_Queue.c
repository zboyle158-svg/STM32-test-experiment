/**
* @file user_Queue.c
* @brief OV-Watch板级外设驱动实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#include "user_Queue.h"


// 初始化队列
void initQueue(Queue *queue)
{
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

// 判断队列是否为空
bool isQueueEmpty(Queue *queue)
{
    return queue->size == 0;
}

// 判断队列是否已满
bool isQueueFull(Queue *queue)
{
    return queue->size == QUEUE_SIZE;
}

// 入队
void enqueue(Queue *queue, unsigned long item)
{
    if (isQueueFull(queue))
    {
        printf("队列已满，无法入队！\n");
        return;
    }
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    queue->data[queue->rear] = item;
    queue->size++;
}

// 出队
uint32_t dequeue(Queue *queue)
{
    if (isQueueEmpty(queue))
    {
        printf("队列为空，无法出队！\n");
        return 0;
    }
    unsigned long item = queue->data[queue->front];
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    queue->size--;
    return item;
}

