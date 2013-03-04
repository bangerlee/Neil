//@date 2011-03-13
#include "thread.h"

Thread::Thread()
{
	maskSIGUSR1();

	sigemptyset(&m_waitSig);
	sigaddset(&m_waitSig, SIGUSR1);
}

Thread::~Thread()
{
}

void Thread::maskSIGUSR1()
{
	sigset_t   sig;
	
	sigemptyset(&sig);
	sigaddset(&sig, SIGUSR1);

	pthread_sigmask(SIG_BLOCK, &sig, NULL);
}

Ret Thread::getThreadId(pthread_t& pid)
{
	pid = m_pid;

	return RET_OK;
}

void* Thread::thread_func_run(void* thiz)
{
	((Thread*)thiz)->run();

	return NULL;
}

Ret Thread::start()
{
	Ret ret = RET_OK;
	int n   = 0;

	if( (n = pthread_create(&m_pid, NULL, thread_func_run, this))  != 0)
	{
		ret = RET_FAIL;
	}

	if( (n = pthread_detach(m_pid)) != 0)
	{
		ret = RET_FAIL;
	}

	return ret;
}

Ret Thread::pause()
{
	int sig = 0;	
	
	sigwait(&m_waitSig, &sig);

	return RET_OK;
}

Ret Thread::continues()
{
	pthread_kill(m_pid, SIGUSR1);

	return RET_OK;
}

#ifdef THREAD_TEST
#include <unistd.h>

class Thread_test:public Thread
{
	public:
		Thread_test();
		~Thread_test();
	protected:
		Ret run(void);	
};

Thread_test::Thread_test()
{
	printf("Thread_test constructing.\n");
}

Thread_test::~Thread_test()
{
	printf("Thread_test deconstructing.\n");
}

Ret Thread_test::run()
{
	printf("Thread_test run.\n");
	sleep(2);
	pause();
	return RET_OK;
}

void printids(const char* s)
{
	pid_t     pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();

	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid, 
			(unsigned int)tid, (unsigned int)tid);

	return;
}
int main(int argc, char** argv)
{
	Thread_test thread_test;
	pthread_t   tid;

	printids("Thread_main:");

	assert(thread_test.start() == RET_OK);
	assert(thread_test.getThreadId(tid) == RET_OK);		
//	assert(thread_test.pause() == RET_OK);
	assert(thread_test.continues() == RET_OK);
	sleep(5);	
	return 0;
}
#endif
