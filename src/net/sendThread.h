#include "thread.h"
#include "pubSource.h"
#include <unistd.h>

#ifndef SENDTHREAD_H
#define SENDTHREAD_H

class SendThread : public Thread
{
	public:
		SendThread(PubSource* source);
		~SendThread();
	protected:
		Ret run(void);
	private:
		SendThread(){};
		PubSource* m_source;
};

#endif
