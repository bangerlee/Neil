#include "epoll_server.h"
#include <iostream>

using namespace std;

Epoll_Server::Epoll_Server()
{
	pubSource = new PubSource();
	
	acceptThread = new AcceptThread(pubSource);
	sendThread   = new SendThread(pubSource);
}

Epoll_Server::~Epoll_Server()
{
	delete pubSource;
	delete acceptThread;
	delete sendThread;
}

Ret Epoll_Server::StartUpAcceptThread()
{
	return acceptThread->start();
}

Ret Epoll_Server::StartUpSendThread()
{
	return sendThread->start();
}

Ret Epoll_Server::init()
{
	Ret ret = RET_OK;

	if(StartUpAcceptThread() != RET_OK)
	{
		ret = RET_FAIL;
		pubSource->writeLog("StartUpAcceptThread failed\n");
	}

	if(StartUpSendThread() != RET_OK)
	{
		ret = RET_FAIL;
		pubSource->writeLog("StartUpSendThread failed\n");
	}

	pubSource->writeLog("Epoll server init successfully\n");

	return ret;
}

Ret Epoll_Server::sendMsg(const unsigned long client, char* data, const size_t length)
{
	Ret ret = RET_OK;
	
	if(!queue_isFull(pubSource->m_qMsgOut))
	{
		queue_push(pubSource->m_qMsgOut, length, data, client);

		return ret;
	}

	ret = RET_FAIL;

	return ret;
}

Ret Epoll_Server::getMsg(unsigned long* client, char* data, size_t* length)
{
	Ret ret = RET_OK;

	if(!queue_isEmpty(pubSource->m_qMsgIn))
	{
		queue_pop(pubSource->m_qMsgIn, length, data, client);
		
		data[*length] = '\0';

		return ret;
	}

	ret = RET_FAIL;

	return ret;
}

Ret Epoll_Server::get_clientID_by_sockfd(const int sockfd, unsigned long* clientID)
{
	Ret ret = RET_OK;

	SockfdIdMap::iterator ite = pubSource->m_SockfdIdMap.find(sockfd);

	if(ite != pubSource->m_SockfdIdMap.end())
	{
		*clientID = pubSource->m_SockfdIdMap[sockfd];
	}
	else
	{
		ret = RET_FAIL;
	}

	return ret;
}

Ret Epoll_Server::get_sockfd_by_clientID(const unsigned long clientID, int* sockfd)
{
	Ret ret = RET_OK;

	IdSockfdMap::iterator ite = pubSource->m_IdSockfdMap.find(clientID);

	if(ite != pubSource->m_IdSockfdMap.end())
	{
		*sockfd = pubSource->m_IdSockfdMap[clientID];
	}
	else
	{
		ret = RET_FAIL;
	}

	return ret;
}
