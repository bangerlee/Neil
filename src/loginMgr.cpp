//date@ 2011-03-24
#include "loginMgr.h"
#include <iostream>

using namespace std;

map<long, string> Global_Var::client_name_map;
map<string, long> Global_Var::name_client_map;

Epoll_Server Global_Var::m_server;

void LoginMgr::LoginCmd(char* msgBuf, long clientID)
{
	login_get_name_pass(msgBuf, login_usrname, login_pass);

	string str_usrname = string(login_usrname);
	string str_pass = string(login_pass);

	if(login_dup_log(clientID, str_usrname))
	{
		return;
	}

	CMySQLMan CMySQL;

	CMySQL.SetCon("127.0.0.1", "root", "ss", "WebGame", 3306);
	CMySQL.ConnectDB();

	string query = "select * from Login where usrname = '" + str_usrname + "' and password = '" + str_pass + "'";

	MYSQL_RES* res = CMySQL.SelectRecord(query.c_str());

	if(CMySQL.GetRowNum(res) > 0)
	{
		login_success(clientID);
		
		Global_Var::client_name_map[clientID] = str_usrname;
		Global_Var::name_client_map[str_usrname] = clientID;

		login_get_info(clientID, str_usrname);

		login_add_name(clientID);

		login_send_namelist(clientID);

		//To_do Global_Var::Lobby.get_RoomList(client);
	}
	else
	{
		login_fail(clientID);
	}
	return;
}

void LoginMgr::login_get_name_pass(char* msg, char* usr, char* pass)
{
	memcpy(usr, (msg + 4), 2 * sizeof(long));
	memcpy(pass, (msg + 12), 2* sizeof(long));

	cout << usr << " login in" << endl;

	return;
}

bool LoginMgr::login_dup_log(long client, string usr)
{
	string msg = "20012";
	char c_msg[5];

	memcpy(c_msg, msg.c_str(), msg.length());

	if(Global_Var::name_client_map.count(usr) != 0)
	{
		Global_Var::m_server.sendMsg(client, c_msg, msg.length());

		return true;
	}

	return false;
}

void LoginMgr::login_success(long client)
{
	char success[5] = {'2', '0', '0', '1', '1'};

	Global_Var::m_server.sendMsg(client, success, sizeof(success));

	return;
}

void LoginMgr::login_get_info(long client, string usr)
{
	string r_usrname = "2009";
	string msg = r_usrname + usr;
	char c_msg[12];

	memcpy(c_msg, msg.c_str(), msg.length());

	Global_Var::m_server.sendMsg(client, c_msg, msg.length());

	return;
}

void LoginMgr::login_add_name(long client)
{
	char add_name[5] = {'2', '0', '0', '3', '\0'};
	string msg = string(add_name) + Global_Var::client_name_map[client];
	char c_msg[2046];

	memcpy(c_msg, msg.c_str(), msg.length());

	map<long, string>::iterator ite = Global_Var::client_name_map.begin();

	for(; ite != Global_Var::client_name_map.end(); ite++)
	{
		if(ite->second == Global_Var::client_name_map[client])
		{
			continue;
		}

		Global_Var::m_server.sendMsg(ite->first, c_msg, msg.length());
	}

	return;
}

void LoginMgr::login_send_namelist(long client)
{
	char name_list_id[5] = {'2', '0', '0', '2', '\0'};

	map<long, string>::iterator ite = Global_Var::client_name_map.begin();

	string msg_name = "";

	for(; ite != Global_Var::client_name_map.end(); ite++)
	{
		msg_name += ite->second;
	}

	string msg = string(name_list_id) + msg_name;

	char c_msg[2048];

	memcpy(c_msg, msg.c_str(), msg.length());

	Global_Var::m_server.sendMsg(client, c_msg, msg.length());

	return;
}

void LoginMgr::login_fail(long client)
{
	char fail[5] = {'2', '0', '0', '1', '0'};

	Global_Var::m_server.sendMsg(client, fail, sizeof(fail));

	return;
}
