#include "pubSource.h"

PubSource::PubSource()
{
	m_qMsgIn = queue_create(0);
	m_qMsgOut = queue_create(0);

	m_lClientCurNum = 50000001;

	m_mapLock = lock_create();

	m_logFile.open("ServerLog.txt", std::ios_base::out | std::ios_base::trunc);

	m_logLock = lock_create();
}

PubSource::~PubSource()
{
	queue_destroy(m_qMsgIn);
	queue_destroy(m_qMsgOut);

	m_logFile.close();

	lock_destroy(m_mapLock);
	lock_destroy(m_logLock);
}

Ret PubSource::writeLog(const char* str)
{
	time_t ticks;
	char   buff[LOG_TIME_BUFSIZE];

	ticks = time(NULL);

	snprintf(buff, sizeof(buff), "%.24s\r", ctime(&ticks));

	lock_write(m_logLock);  //Lock_write
	m_logFile << buff << str << endl;
	lock_unwrite(m_logLock);  //Lock_unwrite

	return RET_OK;
}

Ret PubSource::writeLog(const char* str, const char* ctx)
{
	time_t ticks;
//	char   buff[LOG_TIME_BUFSIZE];

	ticks = time(NULL);

//	snprintf(buff, sizeof(buff), "%.24s\r", ctime(&ticks));

	lock_write(m_logLock);  //Lock_write
//	m_logFile << buff << str << ctx << endl;
	m_logFile << str << ctx << endl;
	lock_unwrite(m_logLock);  //Lock_write

	return RET_OK;
}
