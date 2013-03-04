#include "queue.h"
#include <map>
#include <arpa/inet.h>
#include <fstream>
#include <time.h>

using namespace std;

#ifndef PUBSOURCE_H
#define PUBSOURCE_H

//typedef std::map<unsigned long, struct sockaddr_in> IdSockMap;
//typedef std::map<struct sockaddr_in, unsigned long> SockIdMap;
#define CLIENT_DROP_ID 1111
#define LOG_TIME_BUFSIZE 30

typedef std::map<int, unsigned long> SockfdIdMap;
typedef std::map<unsigned long, int> IdSockfdMap;

class PubSource
{
	public:
		Queue* m_qMsgIn;
		Queue* m_qMsgOut;

//		IdSockMap m_IdSockMap;
//		SockIdMap m_SockIdMap;
		SockfdIdMap m_SockfdIdMap;
		IdSockfdMap m_IdSockfdMap;
		unsigned long m_lClientCurNum;

		Lock* m_mapLock;

		std::ofstream m_logFile;

		Lock* m_logLock;

		Ret writeLog(const char* str);
		Ret writeLog(const char* str, const char* ctx);
		PubSource();
		~PubSource();
};

#endif
