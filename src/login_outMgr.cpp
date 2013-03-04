#include "login_outMgr.h"

void Login_OutMgr::login_out_Cmd(char* msgBuf, long client)
{
	login_out_del_name(client);

	map<long, string>::iterator ite1 = Global_Var::client_name_map.find(client);
	map<string, long>::iterator ite2 = Global_Var::name_client_map.find(ite1->second);

	if(ite1 != Global_Var::client_name_map.end())
	{
		if(Global_Var::client_index.find(client) != Global_Var::client_index.end())
		{
			map<long, Index>::iterator ite3 = Global_Var::client_index.find(client);

			Global_Var::lobby.lobby_del_player(client, ite3->second.room_id);
		}
	}

	Global_Var::client_name_map.erase(ite1);

	if(ite2 != Global_Var::name_client_map.end())
	{
		Global_Var::name_client_map.erase(ite1);
	}

	return;
}

void Login_OutMgr::login_out_del_name(long client)
{
	char add_name[5] = {'2', '0', '1', '1', '\0'};
	string msg = string(add_name) + Global_Var::client_name_map[client];
	char c_msg[2048];

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
