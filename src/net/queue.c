//@date: 2011-03-16
#include "queue.h"

typedef struct _QueueNode
{
	unsigned clientID;
	size_t   length;
	char     data[MSGBUF_SIZE];
}QueueNode;

struct _Queue
{
	size_t front;
	size_t rear;
	QueueNode* elements;
	size_t max_size;
	size_t size;

	Lock*  lock;
};

Queue* queue_create(size_t max_size)
{
	Queue* thiz = NULL;

	if(max_size == 0)
	{
		max_size = DEFAULT_QUEUE_SIZE; 
	}

	if( (thiz = (Queue*)malloc(sizeof(Queue))) == NULL)
	{
		return NULL;
	}

	if( (thiz->elements = (QueueNode*)malloc(max_size * sizeof(QueueNode))) == NULL)
	{
		free(thiz);
		return NULL;
	}

	thiz->size  = 0;
	thiz->front = 0;
	thiz->rear  = 0;
	thiz->max_size = max_size;

	thiz->lock  = lock_create();

	return thiz;
}

Ret queue_push(Queue* thiz, size_t length, char* data, unsigned long clientID)
{
	return_val_if_fail(thiz != NULL && data != NULL, RET_INVALID_PARAMS);

	if(queue_isFull(thiz))
	{
		return RET_FAIL;
	}

	lock_write(thiz->lock);

	if( (++thiz->rear) == thiz->max_size)
	{
		thiz->rear = 0;
	}

	thiz->elements[thiz->rear].clientID = clientID;
	thiz->elements[thiz->rear].length   = length;
	memcpy(thiz->elements[thiz->rear].data, data, length);
	thiz->size++;

	lock_unwrite(thiz->lock);

	return RET_OK;
}

Ret queue_pop(Queue* thiz, size_t* length, char* data, unsigned long* clientID)
{
	return_val_if_fail(thiz != NULL && data != NULL, RET_INVALID_PARAMS);

	if(queue_isEmpty(thiz))
	{
		return RET_FAIL;
	}

	lock_write(thiz->lock);

	if( (++thiz->front) == thiz->max_size)
	{
		thiz->front = 0;
	}

	*clientID = thiz->elements[thiz->front].clientID;
	*length   = thiz->elements[thiz->front].length;
	memcpy(data, thiz->elements[thiz->front].data, *length);
	thiz->size--;

	lock_unwrite(thiz->lock);

	return RET_OK;
}

bool queue_isFull(Queue* thiz)
{
	bool bRet = 0;

	return_val_if_fail(thiz != NULL, true);

	lock_read(thiz->lock);
	
	if(thiz->size == thiz->max_size)
	{
		bRet = true;
	}
	else
	{
		bRet = false;
	}

	lock_unread(thiz->lock);

	return bRet;
}

bool queue_isEmpty(Queue* thiz)
{
	bool bRet = 0;

	return_val_if_fail(thiz != NULL, true);

	lock_read(thiz->lock);

	if(thiz->size == 0)
	{
		bRet = true;
	}
	else
	{
		bRet = false;
	}

	lock_unread(thiz->lock);
	
	return bRet;
}

size_t queue_size(Queue* thiz)
{
	size_t length = 0;

	return_val_if_fail(thiz != NULL, 0);

	lock_read(thiz->lock);

	length = thiz->size;

	lock_unread(thiz->lock);
	
	return length;
}

void queue_destroy(Queue* thiz)
{
	if(thiz != NULL)
	{
		lock_write(thiz->lock);

		if(thiz->elements != NULL)
		{
			free(thiz->elements);
		}
		
		lock_unwrite(thiz->lock);

		lock_destroy(thiz->lock);

		free(thiz);
	}

	return;
}

#ifdef QUEUE_TEST

int main(int argc, char** argv)
{
	int i = 0;
	int n = 1000;

	char          data[8]  = {'I', ' ', 'a', 'm', ' ', 'O', 'K', '.'};
	size_t        length   = strlen(data);
	unsigned long clientID = 10000;

	Queue* queue = queue_create(1000);


	for(i = 0; i < n; i++)
	{
		assert(queue_push(queue, length, data, clientID) == RET_OK);
		assert(queue_size(queue) == i+1);	
	}

	assert(queue_isFull(queue) == true);

	for(i = 0; i < n; i++)
	{
		assert(queue_size(queue) == (n - i));
		assert(queue_pop(queue, &length, data, &clientID) == RET_OK);
	}

	assert(queue_isEmpty(queue) == true);

	queue_destroy(queue);

	return 0;
}

#endif
