//#include "stdafx.h"
#include "CMySQLMan.h"
CMySQLMan::CMySQLMan()
{

}

CMySQLMan::CMySQLMan(const char *host, const char *user, const char *password, const char *db, unsigned int port/* =3306 */)
{
	strcpy(m_host, host);
	strcpy(m_user, user);
	strcpy(m_password, password);
	strcpy(m_db, db);
	m_port = port;

	//InitializeCriticalSection(&m_csList);
}

CMySQLMan::~CMySQLMan()
{
	for (ITER_CONNECTION_HANDLE_LIST iter=m_lsBusyList.begin(); iter != m_lsBusyList.end(); iter++)
	{
		mysql_close((*iter));
	}

	for (ITER_CONNECTION_HANDLE_LIST iter=m_lsIdleList.begin(); iter != m_lsIdleList.end(); iter++)
	{
		mysql_close((*iter));
	}

	//DeleteCriticalSection(&m_csList);
}

void CMySQLMan::SetCon(const char *host, const char *user, const char *password, const char *db, unsigned int port=3306)
{
	strcpy(m_host, host);
	strcpy(m_user, user);
	strcpy(m_password, password);
	strcpy(m_db, db);
	m_port = port;
}

bool CMySQLMan::ConnectDB()
{
	//同时打开CONNECTION_NUM个连接
	try
	{
		for (int i=0; i<CONNECTION_NUM; ++i)
		{
			MYSQL *pMySql = mysql_init((MYSQL*)NULL);
			if (pMySql != NULL)
			{
				if (!mysql_real_connect(pMySql,m_host,m_user,m_password,m_db,m_port,NULL,0))
				{
					OutErrors(pMySql);
					return false;
				}
				m_lsIdleList.push_back(pMySql);
			}
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

MYSQL* CMySQLMan::GetIdleMySql()
{
	MYSQL* pMySql = NULL;
	//EnterCriticalSection(&m_csList);
	if (m_lsIdleList.size() > 0)
	{
		pMySql = m_lsIdleList.front();
		m_lsIdleList.pop_front();
		m_lsBusyList.push_back(pMySql);
	}
	else
	{
		pMySql = NULL;
	}
	//LeaveCriticalSection(&m_csList);

	return pMySql;
}

void CMySQLMan::SetIdleMysql(MYSQL* pMySql)
{
	//EnterCriticalSection(&m_csList);
	m_lsBusyList.remove(pMySql);
	m_lsIdleList.push_back(pMySql);
	//LeaveCriticalSection(&m_csList);
}

MYSQL_RES* CMySQLMan::SelectRecord(const char *szSql)
{
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return NULL;
	}
	if(mysql_query(pMySql,szSql))
		return NULL;
	MYSQL_RES *myquery = NULL;
	myquery = mysql_store_result(pMySql);
	SetIdleMysql(pMySql);

	return myquery;
}

bool CMySQLMan::InsertRecord(const char *szSql)
{
	bool bRet = false;
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return false;
	}
	if(mysql_query(pMySql,szSql))
	{
		bRet = true;
	}
	SetIdleMysql(pMySql);

	return bRet;
}

bool CMySQLMan::UpdateRecord(const char *szSql)
{
	bool bRet = false;
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return false;
	}
	if(mysql_query(pMySql,szSql))
	{
		bRet = true;
	}
	SetIdleMysql(pMySql);

	return bRet;
}

bool CMySQLMan::DelRecord(const char *szSql)
{
	bool bRet = false;
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return false;
	}
	if(mysql_query(pMySql,szSql))
	{
		bRet = true;
	}
	SetIdleMysql(pMySql);

	return bRet;
}

bool CMySQLMan::SelectDB(const char *szDB)
{
	bool bRet = false;
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return false;
	}
	if (mysql_select_db(pMySql,szDB))
		bRet = false; 
	else
		bRet = true;
	SetIdleMysql(pMySql);

	return bRet;
}

my_ulonglong CMySQLMan::GetRowNum(MYSQL_RES *myquery)
{
	return mysql_num_rows(myquery);
}

MYSQL_ROW CMySQLMan::GetRecord(MYSQL_RES *myquery)
{
	m_row = mysql_fetch_row(myquery);

	return m_row;
}

unsigned int CMySQLMan::GetFieldNum(MYSQL_RES *myquery)
{
	return mysql_num_fields(myquery);
}

void CMySQLMan::FreeRecord(MYSQL_RES *myquery)
{
	mysql_free_result(myquery);
}

//int CMySQLMan::CreateDB(char *db)
//{
// return mysql_create_db(&m_mysql,db);
//}

void CMySQLMan::SeekData(MYSQL_RES *myquery, int offset)
{
	mysql_data_seek(myquery,offset);
}


char * CMySQLMan::OutErrors(MYSQL *pMySql)
{
	return const_cast<char *>(mysql_error(pMySql));
}

bool CMySQLMan::IsEnd(MYSQL_RES *myquery)
{
	return mysql_eof(myquery);
}

char* CMySQLMan::GetFieldName(MYSQL_RES *myquery, int FieldNum)
{
	m_field = mysql_fetch_field_direct(myquery, FieldNum);

	return m_field->name;
}

char * CMySQLMan::GetClientInfo()
{
	return const_cast<char *>(mysql_get_client_info());
}

char* CMySQLMan::GetHostInfo()
{
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return NULL;
	}
	return const_cast<char *>(mysql_get_host_info(pMySql));
}

int CMySQLMan::GetProtocolInfo()
{
	int iRet = 0;
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return NULL;
	}
	iRet = mysql_get_proto_info(pMySql);
	SetIdleMysql(pMySql);

	return iRet;
}

char* CMySQLMan::GetServerInfo()
{
	static char szRet[1024];
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return NULL;
	}
	/*
	_tcscpy(szRet, const_cast<char *>(mysql_get_server_info(pMySql)));
	*/
	SetIdleMysql(pMySql);

	return szRet;
}

char* CMySQLMan::GetState()
{
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return NULL;
	}
	static char szRet[1024];
	/*
	以下代码出错
	_tcscpy(szRet,const_cast<char *>(mysql_stat(pMySql)));
	SetIdleMysql(pMySql);
	*/
	return szRet;
}

bool CMySQLMan::SetCharset()
{
	bool bRet = false;
	char szSql[50];
	strcpy(szSql, "set names gb2312");
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return false;
	}
	if (mysql_query(pMySql, szSql))
		bRet = true;
	SetIdleMysql(pMySql);

	return bRet;
}

//LOCK TABLES tbl1 READ, tbl2 WRITE
bool CMySQLMan::LockTable(const char *TableName, const char *Priority)
{
	bool bRet = false;
	char szSql[50];
	sprintf(szSql, "LOCK TABLES %s %s", TableName, Priority);
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return false;
	}
	if (mysql_query(pMySql, szSql))
		bRet = true;
	SetIdleMysql(pMySql);

	return bRet;
}

bool CMySQLMan::UnlockTable()
{
	bool bRet = false;
	MYSQL *pMySql = GetIdleMySql();
	if (pMySql == NULL)
	{
		return false;
	}
	if(mysql_query(pMySql,"UNLOCK TABLES"))
		bRet = true;
	SetIdleMysql(pMySql);

	return bRet;

}

