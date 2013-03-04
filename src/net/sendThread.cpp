#include "sendThread.h"

SendThread::SendThread(PubSource* source)
{
	m_source = source;
}

SendThread::~SendThread()
{
}

Ret SendThread::run()
{
	unsigned long clientID = 0;
	size_t length = 0;
	char data[MSGBUF_SIZE];

	bzero(data, sizeof(data));

	for(;;)
	{
		if(queue_size(m_source->m_qMsgOut) > 0)
		{
			queue_pop(m_source->m_qMsgOut, &length, data, &clientID);		
			
			write(m_source->m_IdSockfdMap[clientID], data, strlen(data));
		}
	}
	return RET_OK;
}
