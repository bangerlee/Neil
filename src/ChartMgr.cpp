#include "StdAfx.h"
#include "ChartMgr.h"

ChartMgr::ChartMgr(void)
{
}

ChartMgr::~ChartMgr(void)
{
}

void ChartMgr::ChartCmd(char *msgBuf, long client)
{
	char chart[5]={'2','0','0','4','\0'};
	string content(msgBuf+4);
	string msg=string(chart)+GlobalVar::Client_Namemap[client]+content;
	char c_msg[2048];
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Client_Namemap.begin();
	for(;ite!=GlobalVar::Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}
