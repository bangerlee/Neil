#include "acceptThread.h"

#define EPOLLSIZE 2000
#define SERV_PORT 5555 
#define LISTENQ 20
#define MAXLINE 1024

AcceptThread::AcceptThread(PubSource* source)
{
	m_source = source;	
}

AcceptThread::~AcceptThread()
{
}

Ret AcceptThread::setnonblock(int sock)
{
	int opts = 0;

	if( (opts = fcntl(sock, F_GETFL)) < 0)
	{
		return RET_FAIL;		
	}

	opts = opts | O_NONBLOCK;

	if(fcntl(sock, F_SETFL, opts) < 0)
	{
		return RET_FAIL;
	}

	return RET_OK;
}

Ret AcceptThread::run()
{
	//for listen
	int                listenfd;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	socklen_t          clilen;
	int                connfd;

	//for epoll
	int                epfd;
	int                nfds   = 0;
	int                curfds = 0;
	struct epoll_event ev;
	struct epoll_event events[20];
	int                sockfd;

	int i = 0;
	int n = 0;
	char* str = NULL;

	//recvline sendline
	char recvline[MAXLINE];
	//char sendline[MAXLINE];

	char tempbuff[30];

	epfd = epoll_create(EPOLLSIZE);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	setnonblock(listenfd);

	ev.data.fd = listenfd;
	ev.events  = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

	bzero(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family      = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port        = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

	listen(listenfd, LISTENQ);

	curfds = 1;

	for(;;)
	{
		nfds = epoll_wait(epfd, events, curfds, -1);

		for(i = 0; i < nfds; i++)
		{
			if(events[i].data.fd == listenfd)
			{
				clilen = sizeof(clientaddr);

				connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clilen);

				if(connfd < 0)
				{
					return RET_FAIL;
				}

				setnonblock(connfd);

				ev.data.fd = connfd;
				ev.events  = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
				
				lock_write(m_source->m_mapLock);/*Lock_write*/

				m_source->m_IdSockfdMap[m_source->m_lClientCurNum] = connfd;
				m_source->m_SockfdIdMap[connfd] = m_source->m_lClientCurNum;

				lock_unwrite(m_source->m_mapLock);/*Lock_unwrite*/

				nfds++;
				curfds++;
	
			        str = inet_ntoa(clientaddr.sin_addr);
			
				m_source->writeLog("connection from ", str);
				
				sprintf(tempbuff, "%ld", m_source->m_lClientCurNum);
				m_source->writeLog("new clientID ", tempbuff);

				m_source->m_lClientCurNum++;

			}
			else if(events[i].events & EPOLLIN)
			{
				if( (sockfd = events[i].data.fd) < 0)
				{
					continue;
				}

				n = read(sockfd, recvline, MAXLINE);

				if(n == 0)    //客户端断开连接
				{
					sprintf(tempbuff, "%ld", m_source->m_SockfdIdMap[sockfd]);
					m_source->writeLog("connection closed: clientID ", tempbuff);
					
					//long msgDropID = CLIENT_DROP_ID;

					if(!queue_isFull(m_source->m_qMsgIn))    //将断开消息放入消息队列
					{
						sprintf(tempbuff, "%d", CLIENT_DROP_ID);
						queue_push(m_source->m_qMsgIn, sizeof(long), tempbuff, m_source->m_SockfdIdMap[sockfd]); 
					}

					lock_write(m_source->m_mapLock);    //Lock_write

					m_source->m_IdSockfdMap.erase(m_source->m_SockfdIdMap[sockfd]);
					m_source->m_SockfdIdMap.erase(sockfd);

					lock_unwrite(m_source->m_mapLock);    //Lock_unwrite

					close(sockfd);
					events[i].data.fd = -1;

					curfds--;

					continue;
				}

				if(!queue_isFull(m_source->m_qMsgIn))    //将消息放入消息队列
				{
					queue_push(m_source->m_qMsgIn, strlen(recvline), recvline, m_source->m_SockfdIdMap[sockfd]);
				}
			//	printf("%s", recvline);
			}
			else if(events[i].events & EPOLLOUT)
			{
			}
		}
	}	
}
