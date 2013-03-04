#include "thread.h"
#include "pubSource.h"
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <map>

#ifndef ACCEPTTHREAD_H
#define ACCEPTTHREAD_h

class AcceptThread : public Thread
{
	public:
		AcceptThread(PubSource* source);
		~AcceptThread();
	protected:
		Ret run(void);
	private:
		AcceptThread(){};
		Ret setnonblock(int sock);
		PubSource* m_source;
};

#endif
