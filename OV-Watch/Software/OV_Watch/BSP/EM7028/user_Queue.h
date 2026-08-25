/**
* @file user_Queue.h
* @brief OV-Watch板级外设驱动实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "main.h"

// 定义队列结构
#define QUEUE_SIZE 7

typedef struct {
    int8_t front;
    int8_t rear;
    int8_t size;
    uint32_t data[QUEUE_SIZE];
} Queue;

void initQueue(Queue *queue);
bool isQueueEmpty(Queue *queue);
bool isQueueFull(Queue *queue);
void enqueue(Queue *queue, unsigned long item);
uint32_t dequeue(Queue *queue);
