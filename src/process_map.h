//@date 2011-03-24
#include "process.h"
#include "./net/epoll_server.h"
#include "./net/typedef.h"

#ifndef PROCESS_MAP_H
#define PROCESS_MAP_H

typedef void (Process::*pFn)(char*, long);

class ProcessMap
{
	public:
		ProcessMap();
		~ProcessMap();

		Epoll_Server epoll_server;
		map<long, pFn> m_processFn;

		void msg_process(void);

	private:
		Process m_process;
};

#endif
