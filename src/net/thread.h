//@date 2011-03-13
#include "typedef.h"
#include <signal.h>
#include <pthread.h>

#ifndef THREAD_H
#define THREAD_H

class Thread
{
	private:
		pthread_t     m_pid;
		sigset_t      m_waitSig;
		void          maskSIGUSR1(void);
		static void*  thread_func_run(void* thiz); 
	public:
		Thread();
		~Thread();
		
		Ret getThreadId(pthread_t& pid);
		Ret start(void);
		Ret pause(void);
		Ret continues(void);
	protected:
		virtual Ret run(void) = 0;
};
#endif
