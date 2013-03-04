//@date: 2011-03-16
#include "typedef.h"
#include "lock.h"
#include <string.h>
#include <stdbool.h>

#ifndef QUEUE_H
#define QUEUE_H

#define MSGBUF_SIZE 2048
#define DEFAULT_QUEUE_SIZE 1500

DECLS_BEGIN

struct _Queue;
typedef struct _Queue Queue;

Queue* queue_create(size_t max_size);

Ret    queue_push(Queue* thiz, size_t length, char* data, unsigned long clientID);
Ret    queue_pop(Queue* thiz, size_t* length, char* data, unsigned long* clientID);
bool   queue_isFull(Queue* thiz);
bool   queue_isEmpty(Queue* thiz);
size_t queue_size(Queue* thiz);

void   queue_destroy(Queue* thiz);

DECLS_END

#endif
