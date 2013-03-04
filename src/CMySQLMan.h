#include <stdio.h>
#include <list>
#include <mysql.h>
#include <string.h>

typedef std::list<MYSQL *> CONNECTION_HANDLE_LIST;
typedef std::list<MYSQL *>::iterator ITER_CONNECTION_HANDLE_LIST;

#define CONNECTION_NUM 10 

class CMySQLMan 
{
public:
	CMySQLMan();
	CMySQLMan(const char *host, const char *user, const char *password, const char *db, unsigned int port=3306);
	virtual ~CMySQLMan();

public:
	void SetCon(const char *host, const char *user, const char *password, const char *db, unsigned int port);
	bool ConnectDB();       //连接数据库
	MYSQL_RES* SelectRecord(const char *szSql); //选择记录，返回结果集
	bool SelectDB(const char *szDB);   //选择数据库
	bool UpdateRecord(const char *szSql); //更新记录
	bool InsertRecord(const char *szSql); //插入记录
	bool DelRecord(const char *szSql);   //删除记录

	bool IsEnd(MYSQL_RES *myquery);        //是否最后
	void SeekData(MYSQL_RES *myquery, int offset);     //查找指定数据
	void FreeRecord(MYSQL_RES *myquery);       //释放结果集
	unsigned int GetFieldNum(MYSQL_RES *myquery);     //得到字段数
	MYSQL_ROW GetRecord(MYSQL_RES *myquery);      //得到结果（一个记录）
	my_ulonglong GetRowNum(MYSQL_RES *myquery);     //得到记录数
	char* OutErrors(MYSQL* pMySql);       //输出错误信息

	char* GetState();       //服务器状态
	char* GetServerInfo();      //服务器信息
	int GetProtocolInfo();      //协议信息
	char* GetHostInfo();      //主机信息
	char* GetClientInfo();      //客户机信息
	char* GetFieldName(MYSQL_RES *myquery, int FieldNum);   //字段名

	bool LockTable(const char *TableName, const char *Priority); //对特定表加锁
	bool UnlockTable();       //解锁
	bool SetCharset();
	//int CreateDB(char *db);     //创建数据库，返回错误信息
	//int DropDB(char *db);      //删除数据库,返回错误信息

	MYSQL* GetIdleMySql();      //提取一个空闲句柄供使用
	void SetIdleMysql(MYSQL* pMySql);   //从使用队列中释放一个使用完毕的句柄，插入到空闲队列

public:
	//MYSQL   m_mysql;       //数据库连接句柄
	MYSQL_ROW m_row;        //记录集(单行)
	MYSQL_FIELD *m_field;       //字段信息（结构体）

	//创建两个队列
	CONNECTION_HANDLE_LIST m_lsBusyList;                //正在使用的连接句柄
	CONNECTION_HANDLE_LIST m_lsIdleList;                //未使用的连接句柄

	//CRITICAL_SECTION m_csList;

public:
	char m_host[20];    //主机
	char m_user[20];    //用户名
	char m_password[20];   //密码
	char m_db[20];     //数据库名
	unsigned int m_port;   //端口
};

