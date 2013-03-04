#include "acceptThread.h"
#include "sendThread.h"
#include "pubSource.h"

#ifndef EPOLL_SERVER_H
#define EPOLL_SERVER_H

class Epoll_Server
{
	public:
		Epoll_Server();
		~Epoll_Server();
		
		Ret init(void);

		Ret sendMsg(const unsigned long client, char* data, const size_t length);
		Ret getMsg(unsigned long* client, char* data, size_t* length);	
	
		Ret get_clientID_by_sockfd(const int sockfd, unsigned long* clientID);
		Ret get_sockfd_by_clientID(const unsigned long clientID, int* sockfd);
	private:
		PubSource*    pubSource;
		AcceptThread* acceptThread;
		SendThread*   sendThread;

		Ret StartUpAcceptThread(void);
		Ret StartUpSendThread(void);
};

#endif
