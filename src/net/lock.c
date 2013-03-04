//date 2011-03-10
#include "lock.h"

struct _Lock
{
	pthread_mutex_t   r_mutex;
	pthread_mutex_t   w_mutex;
	
	unsigned long     r_counter;
	pthread_cond_t    r_cond_counter;
};

Lock* lock_create()
{
	Lock* thiz = (Lock*)malloc(sizeof(Lock));

	if( thiz != NULL)
	{
		pthread_mutex_init(&thiz->r_mutex, NULL);
		pthread_mutex_init(&thiz->w_mutex, NULL);
		
		pthread_cond_init(&thiz->r_cond_counter, NULL);
		thiz->r_counter = 0;
	}

	return thiz;
}

Ret lock_read(Lock* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	pthread_mutex_lock(&thiz->w_mutex);
	pthread_mutex_lock(&thiz->r_mutex);

	thiz->r_counter++;

	pthread_mutex_unlock(&thiz->w_mutex);
	pthread_mutex_unlock(&thiz->r_mutex);

	return RET_OK;
}

Ret lock_unread(Lock* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	pthread_mutex_lock(&thiz->r_mutex);

	thiz->r_counter--;

	if(thiz->r_counter == 0)
	{
		pthread_cond_signal(&thiz->r_cond_counter);
	}

	pthread_mutex_unlock(&thiz->r_mutex);

	return RET_OK;
}

Ret lock_write(Lock* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	pthread_mutex_lock(&(thiz->w_mutex));
	pthread_mutex_lock(&(thiz->r_mutex));

	if(thiz->r_counter == 0)
	{
		pthread_mutex_unlock(&thiz->r_mutex);
		return RET_OK;
	}
	pthread_cond_wait(&thiz->r_cond_counter, &thiz->r_mutex);
	pthread_mutex_unlock(&thiz->r_mutex);

	return RET_OK;
}

Ret lock_unwrite(Lock* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	pthread_mutex_unlock(&thiz->w_mutex);

	return RET_OK;
}

void lock_destroy(Lock* thiz)
{
	if(thiz != NULL)
	{
		pthread_mutex_destroy(&thiz->r_mutex);
		pthread_mutex_destroy(&thiz->w_mutex);
		pthread_cond_destroy(&thiz->r_cond_counter);
	}

	free(thiz);

	return;
}
/*
static Ret lock_get_rcounter(Lock* thiz, int* counter)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	*counter = thiz->r_counter;

	return RET_OK;
}
*/

#ifdef LOCK_TEST

#include <assert.h>
#include <pthread.h>

int data = 0;

static void test_lock(void)
{
	int i = 0;
	int n = 100;
	//int counter = 0;

	Lock* lock = lock_create();

	for(i = 0; i < n; i++)
	{
		assert(lock_read(lock) == RET_OK);
		//assert(lock_get_rcounter(lock, &counter) == RET_OK);
		//assert(counter == 1);
		assert(data == 0);
		assert(lock_unread(lock) == RET_OK);
		//assert(lock_get_rcounter(lock, &counter) == RET_OK);
		//assert(counter == 0);
		assert(lock_write(lock) == RET_OK);
		data = 1;
		assert(lock_unwrite(lock) == RET_OK);
		
		assert(lock_read(lock) == RET_OK);
		assert(data == 1);
		assert(lock_unread(lock) == RET_OK);

		assert(lock_write(lock) == RET_OK);
		data = 0;
		assert(lock_unwrite(lock) == RET_OK);
	}
	
	lock_destroy(lock);

	return;
}

static void test_invalid_params(void)
{
	printf("==========Warning is normal begin==========\n");
	assert(lock_read(NULL) == RET_INVALID_PARAMS);
	assert(lock_unread(NULL) == RET_INVALID_PARAMS);
	assert(lock_write(NULL) == RET_INVALID_PARAMS);
	assert(lock_unwrite(NULL) == RET_INVALID_PARAMS);
	printf("==========Warning is normal end============\n");

	return;
}

static void single_thread_test(void)
{
	test_lock();
	test_invalid_params();

	return;
}

int main(int argc, char** argv)
{
	single_thread_test();

	return 0;
}
#endif
