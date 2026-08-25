/**
* @file pubsub.h
* @brief OV-Watch应用功能中间层实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#ifndef PUBSUB_H
#define PUBSUB_H

#include <stdint.h>

// Message structure
typedef struct {
    int id;
    char data[256];
} PubSub_Message_t;

// Subscriber function pointer type
typedef void (*Subscriber)(PubSub_Message_t);

// SubscriberNode structure
typedef struct SubscriberNode {
    Subscriber subscriber;
    struct SubscriberNode *next;
} SubscriberNode;

// Publisher structure
typedef struct {
    SubscriberNode *subscribers;
} PubSub_Publisher_t;

// Function declarations
void Publisher_init(PubSub_Publisher_t *publisher);
void Publisher_subscribe(PubSub_Publisher_t *publisher, Subscriber subscriber);
void Publisher_unsubscribe(PubSub_Publisher_t *publisher, Subscriber subscriber);
void Publisher_publish(PubSub_Publisher_t *publisher, PubSub_Message_t message);

extern PubSub_Publisher_t SDL_KeyBoard_Publisher;

#endif // PUBSUB_H
