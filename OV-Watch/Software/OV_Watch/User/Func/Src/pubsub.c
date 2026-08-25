/**
* @file pubsub.c
* @brief OV-Watch应用功能中间层实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#include "../Inc/pubsub.h"
#include <stdlib.h>
#include <stdio.h>

PubSub_Publisher_t SDL_KeyBoard_Publisher;


// Initialize the publisher
void Publisher_init(PubSub_Publisher_t *publisher) {
    publisher->subscribers = NULL;
}

// Subscribe a new subscriber
void Publisher_subscribe(PubSub_Publisher_t *publisher, Subscriber subscriber) {
    SubscriberNode *node = (SubscriberNode *)malloc(sizeof(SubscriberNode));
    if (node == NULL) {
        // Handle memory allocation failure
        return;
    }
    node->subscriber = subscriber;
    node->next = publisher->subscribers;
    publisher->subscribers = node;
}

// Unsubscribe an existing subscriber
void Publisher_unsubscribe(PubSub_Publisher_t *publisher, Subscriber subscriber) {
    SubscriberNode **current = &publisher->subscribers;
    while (*current != NULL) {
        if ((*current)->subscriber == subscriber) {
            SubscriberNode *to_delete = *current;
            *current = (*current)->next;
            free(to_delete);
            return;
        }
        current = &((*current)->next);
    }
}

// Publish a message to all subscribers
void Publisher_publish(PubSub_Publisher_t *publisher, PubSub_Message_t message) {
    SubscriberNode *node = publisher->subscribers;
    while (node != NULL) {
        node->subscriber(message);
        node = node->next;
    }
}
