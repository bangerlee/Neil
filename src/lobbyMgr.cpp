#include "lobbyMgr.h"

map<long, Index> Global_Var::client_index;
LobbyMgr Global_Var::lobby;

Epoll_Server Global_Var::m_server;

void LobbyMgr::lobby_init()
{
	int i = 0;
	int j = 0;

	for(i = 0; i < ROOM_NUM; i++)
	{
		Global_Var::lobby.lobby_room[i].r_id                = i;
		Global_Var::lobby.lobby_room[i].r_num               = 0;
		Global_Var::lobby.lobby_room[i].r_flag              = false;
		Global_Var::lobby.lobby_room[i].r_group_playing_num = 0;

		for(j = 0; j < GROUP_NUM; j++)
		{
			Global_Var::lobby.lobby_room[i].r_group[j].g_id   = j;
			Global_Var::lobby.lobby_room[i].r_group[j].g_flag = false;
			Global_Var::lobby.lobby_room[i].r_group[j].g_num  = 0;

			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.ceo.id      = 0;
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.product.id  = 1;
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.finance.id  = 2;
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.purchase.id = 3;
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.sale.id     = 4;

			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.ceo.ceo_flag           = false;
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.product.product_flag   = false;
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.finance.finance_flag   = false;
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.purchase.purchase_flag = false;
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.sale.sale_flag         = false;

			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.ceo.usrname            = "        ";
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.product.usrname        = "        ";
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.finance.usrname        = "        ";
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.purchase.usrname       = "        ";
			Global_Var::lobby.lobby_room[i].r_group[j].g_actor.sale.usrname           = "        ";
		}
	}

	return;
}

void LobbyMgr::lobby_enter_room_Cmd(char* msgBuf, long client)
{
	char usrname[8];
	char roomId[4];
	int room_id = 0;
	string str_usrname;

	memcpy(usrname, (msgBuf + 4), 2 * sizeof(long));
	memcpy(roomId,  (msgBuf + 12), sizeof(long));

	room_id = atoi(roomId);

	str_usrname = string(usrname).substr(0, 8);

	if((Global_Var::lobby.lobby_room[room_id].r_num < 30) && (Global_Var::lobby.lobby_room[room_id].r_flag == false))
	{
		Global_Var::lobby.lobby_room[room_id].r_num++;

		lobby_enter_room_success(client);

		Global_Var::lobby.lobby_room[room_id].r_client_name_map[client] = str_usrname;
		Global_Var::lobby.lobby_room[room_id].r_name_client_map[str_usrname] = client;

		Global_Var::client_index[client].room_id  = room_id;
		Global_Var::client_index[client].group_id = -1;
		Global_Var::client_index[client].actor_id = -1;

		lobby_send_playerlist(client, room_id);
		lobby_enter_room_tellothers(client, room_id);
		lobby_update(client, room_id);
		lobby_inside_room_info(client, room_id);
	}
	else
	{
		lobby_enter_room_fail(client);
	}
}

void LobbyMgr::lobby_enter_room_success(long client)
{
	char success[5] = {'2', '0', '0', '8', '1'};

	Global_Var::m_server.sendMsg(client, success, sizeof(success));

	return;
}

void LobbyMgr::lobby_send_playerlist(long client, int room_id)
{
	char player_list_id[5] = {'2', '0', '1', '2','\0'};
	string str_usrname_total     = "";
	string msg;
	char c_msg[2048];

	map<long, string>::iterator ite = Global_Var::lobby.lobby_room[room_id].r_client_name_map.begin();

	map<long, string>::iterator ite_end = Global_Var::lobby.lobby_room[room_id].r_client_name_map.end();

	for(; ite != ite_end; ite++)
	{
		str_usrname_total += ite->second;
	}

	msg = string(player_list_id) + str_usrname_total;

	memcpy(c_msg, msg.c_str(), msg.length());

	Global_Var::m_server.sendMsg(client, c_msg, msg.length());

	return;
}

void LobbyMgr::lobby_enter_room_tellothers(long client, int room_id)
{
	char tell_others[5] = {'2', '0', '1', '4', '\0'};
	string str_usrname = Global_Var::lobby.lobby_room[room_id].r_client_name_map[client];
	string msg = string(tell_others) + str_usrname;
	char c_msg[2048];

	memcpy(c_msg, msg.c_str(), msg.length());

	map<long, string>::iterator ite = Global_Var::lobby.lobby_room[room_id].r_client_name_map.begin();

	for(; ite != Global_Var::lobby.lobby_room[room_id].r_client_name_map.end(); ite++)
	{
		if(ite->first == client)
		{
			continue;
		}

		Global_Var::m_server.sendMsg(ite->first, c_msg, msg.length());
	}

	return;
}

void LobbyMgr::lobby_update(long client, int room_id)
{
	string update = "2010";
	char c_num[4];
	char c_room_id[4];
	char c_msg[2048];
	int index = 0;

	string msg;
	string str_num;
	string str_room_id;

	//_itoa(Global_Var::lobby.lobby_room[room_id].r_num, num, 10);
	sprintf(c_num, "%d", Global_Var::lobby.lobby_room[room_id].r_num);

	//_itoa(room_id, c_room_id, 10);
	sprintf(c_room_id, "%d", room_id);

	str_num     = string(c_num);
	str_room_id = string(c_room_id);

	for(index = str_num.length(); index < 4; index++)
	{
		str_num += " ";
	}

	for(index = str_room_id.length(); index < 4; index++)
	{
		str_room_id += " ";
	}

	msg = update + str_room_id + str_num;

	memcpy(c_msg, msg.c_str(), msg.length());

	map<long, string>::iterator ite = Global_Var::client_name_map.begin();

	for(; ite != Global_Var::client_name_map.end(); ite++)
	{
		Global_Var::m_server.sendMsg(ite->first, c_msg, msg.length());
	}

	return;
}

void LobbyMgr::lobby_inside_room_info(long client, int room_id)
{
	string inside_info = "2020";
	string str_usrname_total = "";
	string msg;
	char c_msg[2048];
	int i = 0;

	for(i = 0; i < GROUP_NUM; i++)
	{
		str_usrname_total += Global_Var::lobby.lobby_room[room_id].r_group[i].g_actor.ceo.usrname;
		str_usrname_total += Global_Var::lobby.lobby_room[room_id].r_group[i].g_actor.sale.usrname;
		str_usrname_total += Global_Var::lobby.lobby_room[room_id].r_group[i].g_actor.finance.usrname;
		str_usrname_total += Global_Var::lobby.lobby_room[room_id].r_group[i].g_actor.purchase.usrname;
		str_usrname_total += Global_Var::lobby.lobby_room[room_id].r_group[i].g_actor.product.usrname;
	}

	msg = inside_info + str_usrname_total;

	memcpy(c_msg, msg.c_str(), msg.length());

	Global_Var::m_server.sendMsg(client, c_msg, msg.length());

	return;
}

void LobbyMgr::lobby_enter_room_fail(long client)
{
	char fail[5] = {'2', '0', '0', '8', '0'};

	Global_Var::m_server.sendMsg(client, fail, sizeof(fail));

	return;
}
/*for lobby_quit_room_Cmd
 *
 *
 */
void LobbyMgr::lobby_quit_room_Cmd(char* msgBuf, long client)
{
	char c_usrname[20];
	char c_room_id[4];
	int  room_id;

	memcpy(c_usrname, (msgBuf + 4), 2 * sizeof(long));
	memcpy(c_room_id, (msgBuf + 12), sizeof(long));

	room_id = atoi(c_room_id);

	lobby_del_player(client, room_id);

	return;
}

void LobbyMgr::lobby_del_player(long client, int room_id)
{
	map<long, string>::iterator ite1 = Global_Var::lobby.lobby_room[room_id].r_client_name_map.find(client);

	map<string, long>::iterator ite2 = Global_Var::lobby.lobby_room[room_id].r_name_client_map.find(ite1->second);

	lobby_del_player_quit_room(client, room_id);

	Global_Var::lobby.lobby_room[room_id].r_num--;

	lobby_update(client, room_id);

	Global_Var::lobby.lobby_room[room_id].r_client_name_map.erase(ite1);
	Global_Var::lobby.lobby_room[room_id].r_name_client_map.erase(ite2);

	map<long, Index>::iterator ite3 = Global_Var::client_index.find(client);

	if(ite3->second.group_id != -1)
	{
		Global_Var::lobby.lobby_room[room_id].r_group[ite3->second.group_id].g_num--;

		ite1 = Global_Var::lobby.lobby_room[room_id].r_group[ite3->second.group_id].g_client_name_map.find(client);

		ite2 = Global_Var::lobby.lobby_room[room_id].r_group[ite3->second.group_id].g_name_client_map.find(ite1->second);

		Global_Var::lobby.lobby_room[room_id].r_group[ite3->second.group_id].g_client_name_map.erase(ite1);

		Global_Var::lobby.lobby_room[room_id].r_group[ite3->second.group_id].g_name_client_map.erase(ite2);
	}

	if(ite3->second.actor_id != -1)
	{
		lobby_del_player_del_actor(client, ite3->second.actor_id);
		lobby_del_player_quit_actor(client);
	}

	Global_Var::client_index.erase(ite3);

	return;
}

void LobbyMgr::lobby_del_player_quit_room(long client, int room_id)
{
	char quit_room[5] = {'2', '0', '1', '3', '\0'};
	string str_usrname = Global_Var::lobby.lobby_room[room_id].r_client_name_map[client];
	string msg = string(quit_room) + str_usrname;
	char c_msg[2048];

	map<long, string>::iterator ite = Global_Var::lobby.lobby_room[room_id].r_client_name_map.begin();

	memcpy(c_msg, msg.c_str(), msg.length());

	for(; ite != Global_Var::lobby.lobby_room[room_id].r_client_name_map.end(); ite++)
	{
		Global_Var::m_server.sendMsg(ite->first, c_msg, msg.length());
	}

	return;
}

void LobbyMgr::lobby_del_player_del_actor(long client, int actor_id)
{
	Actor actor = Global_Var::lobby.lobby_room[Global_Var::client_index[client].room_id].r_group[Global_Var::client_index[client].group_id].g_actor;

	switch(actor_id)
	{
		case 0:
			{
				CEO ceo = actor.ceo;

				ceo.usrname  = "        ";
				ceo.ceo_flag = false;

				break;
			}

		case 1:
			{
				Sale sale = actor.sale;

				sale.usrname = "        ";
				sale.sale_flag = false;

				break;
			}
		case 2:
			{
				Finance finance = actor.finance;

				finance.usrname = "        ";
				finance.finance_flag = false;

				break;
			}
		case 3:
			{
				Purchase purchase = actor.purchase;

				purchase.usrname = "        ";
				purchase.purchase_flag = false;

				break;
			}
		case 4:
			{
				Product product = actor.product;

				product.usrname = "        ";
				product.product_flag = false;

				break;
			}
		default:
			break;
	}

	return;
}

void LobbyMgr::lobby_del_player_quit_actor(long client)
{
	string quit_actor = "2023";
	char c_room_id[4];
	char c_group_id[4];
	char c_actor_id[4];
	char c_msg[2048];

	string str_room_id;
	string str_group_id;
	string str_actor_id;
	string msg;

	sprintf(c_room_id, "%-8d", Global_Var::client_index[client].room_id);
	sprintf(c_group_id, "%-8d", Global_Var::client_index[client].group_id);
	sprintf(c_actor_id, "%-8d", Global_Var::client_index[client].actor_id);

	str_room_id = string(c_room_id);
	str_group_id = string(c_group_id);
	str_actor_id = string(c_actor_id);

	msg = quit_actor + str_room_id + str_group_id + str_actor_id;

	memcpy(c_msg, msg.c_str(), msg.length());

	map<long, string>::iterator ite = Global_Var::lobby.lobby_room[Global_Var::client_index[client].room_id].r_client_name_map.begin();

	for(; ite != Global_Var::lobby.lobby_room[Global_Var::client_index[client].room_id].r_client_name_map.end(); ite++)
	{
		Global_Var::m_server.sendMsg(ite->first, c_msg, msg.length());
	}

	return;
}

void LobbyMgr::lobby_choose_actor_Cmd(char* msgBuf, long client)
{
	char c_usrname[8];
	char c_room_id[4];
	char c_group_id[4];
	char c_actor_id[4];

	int room_id = 0;
	int group_id = 0;
	int actor_id = 0;

	string str_usrname;
	string str_room_id;
	string str_group_id;
	string str_actor_id;

	memcpy(c_usrname, (msgBuf + 4), 2 * sizeof(long));
	memcpy(c_room_id, (msgBuf + 12), sizeof(long));
	memcpy(c_group_id, (msgBuf + 16), sizeof(long));
	memcpy(c_actor_id, (msgBuf + 20), sizeof(long));

	room_id = atoi(c_room_id);
	group_id = atoi(c_group_id);
	actor_id = atoi(c_actor_id);

	if(lobby_actor_selected(room_id, group_id, actor_id))
	{
		return;
	}

	str_usrname = string(c_usrname).substr(0, 8);
	str_room_id = string(c_room_id).substr(0, 4);
	str_group_id = string(c_group_id).substr(0, 4);
	str_actor_id = string(c_actor_id).substr(0, 4);

	Group group = Global_Var::lobby.lobby_room[room_id].r_group[group_id];

	group.g_num++;
	group.g_client_name_map[client] = str_usrname;
	group.g_name_client_map[str_usrname] = client;

	Global_Var::client_index[client].room_id = room_id;
	Global_Var::client_index[client].group_id = group_id;
	Global_Var::client_index[client].actor_id = actor_id;

	lobby_add_actor(client, actor_id, str_usrname);

	lobby_update_actor_info(client, str_usrname, str_room_id, str_group_id, str_actor_id);

	return;
}

bool LobbyMgr::lobby_actor_selected(int room_id, int group_id, int actor_id)
{
	Actor actor = Global_Var::lobby.lobby_room[room_id].r_group[group_id].g_actor;

	switch(actor_id)
	{
		case 0:
			return actor.ceo.ceo_flag;
		case 1:
			return actor.sale.sale_flag;
		case 2:
			return actor.finance.finance_flag;
		case 3:
			return actor.purchase.purchase_flag;
		case 4:
			return actor.product.product_flag;
	}

	return true;
}

void LobbyMgr::lobby_add_actor(long client, int actor_id, string str_usrname)
{
	Actor actor = Global_Var::lobby.lobby_room[Global_Var::client_index[client].room_id].r_group[Global_Var::client_index[client].group_id].g_actor;

	switch(actor_id)
	{
		case 0:
			actor.ceo.usrname = str_usrname;
			actor.ceo.ceo_flag = true;

			break;
		case 1:
			actor.sale.usrname = str_usrname;
			actor.sale.sale_flag = true;

			break;
		case 2:
			actor.finance.usrname = str_usrname;
			actor.finance.finance_flag = true;

			break;
		case 3:
			actor.purchase.usrname = str_usrname;
			actor.purchase.purchase_flag = true;

			break;
		case 4:
			actor.product.usrname = str_usrname;
			actor.product.product_flag = true;

			break;
		default:
			break;
	}

	return;
}

void LobbyMgr::lobby_update_actor_info(long client, string str_usrname, string str_room_id, string str_group_id, string str_actor_id)
{
	char update[5] = {'2', '0', '2', '1', '\0'};
	string msg = string(update) + str_usrname + str_room_id + str_group_id + str_actor_id;
	char c_msg[2048];

	map<long, string>::iterator ite = Global_Var::lobby.lobby_room[Global_Var::client_index[client].room_id].r_client_name_map.begin();

	memcpy(c_msg, msg.c_str(), msg.length());

	for(; ite != Global_Var::lobby.lobby_room[Global_Var::client_index[client].room_id].r_client_name_map.end(); ite++)
	{
		Global_Var::m_server.sendMsg(ite->first, c_msg, msg.length());
	}

	return;
}

void LobbyMgr::lobby_change_actor_Cmd(char* msgBuf, long client)
{
	char c_usrname[8];
	char c_room_id[4];
	char c_group_id[4];
	char c_actor_id[4];
	char c_prv_group_id[4];
	char c_prv_actor_id[4];

	int room_id = 0;
	int group_id = 0;
	int actor_id = 0;
	int prv_group_id = 0;
	int prv_actor_id = 0;

	memcpy(c_usrname, (msgBuf + 4), 2 * sizeof(long));
	memcpy(c_room_id, (msgBuf + 12), sizeof(long));
	memcpy(c_group_id, (msgBuf + 16), sizeof(long));
	memcpy(c_actor_id, (msgBuf + 20), sizeof(long));
	memcpy(c_prv_group_id, (msgBuf + 24), sizeof(long));
	memcpy(c_prv_actor_id, (msgBuf + 28), sizeof(long));

	string str_usrname = string(c_usrname).substr(0, 8);
	string str_room_id = string(c_room_id).substr(0, 4);
	string str_group_id = string(c_group_id).substr(0, 4);
	string str_actor_id = string(c_actor_id).substr(0, 4);
	string str_prv_group_id = string(c_prv_group_id).substr(0, 4);
	string str_prv_actor_id = string(c_prv_actor_id).substr(0, 4);

	room_id = atoi(c_room_id);
	group_id = atoi(c_group_id);
	actor_id = atoi(c_actor_id);
	prv_group_id = atoi(c_prv_group_id);
	prv_actor_id = atoi(c_prv_actor_id);

	if(lobby_actor_selected(room_id, group_id, actor_id))
	{
		return;
	}

	Group prv_group = Global_Var::lobby.lobby_room[room_id].r_group[prv_group_id];
	map<long, string>::iterator ite1 = prv_group.g_client_name_map.find(client);
	map<string, long>::iterator ite2 = prv_group.g_name_client_map.find(ite1->second);

	prv_group.g_num--;
	prv_group.g_client_name_map.erase(ite1);	
	prv_group.g_name_client_map.erase(ite2);

	lobby_del_actor(client, prv_actor_id);

	Group group = Global_Var::lobby.lobby_room[room_id].r_group[group_id];
	group.g_num++;
	group.g_client_name_map[client] = str_usrname;
	group.g_name_client_map[str_usrname] = client;

	Global_Var::client_index[client].group_id = group_id;
	Global_Var::client_index[client].actor_id = actor_id;

	lobby_add_actor(client, actor_id, str_usrname);

	lobby_change_actor_tellothers(client, str_usrname, str_room_id, str_group_id, str_actor_id, str_prv_group_id, str_prv_actor_id);

	return;
}

void LobbyMgr::lobby_del_actor(long client, int prv_actor_id)
{
	Actor actor = Global_Var::lobby.lobby_room[Global_Var::client_index[client].room_id].r_group[Global_Var::client_index[client].group_id].g_actor;

	switch(prv_actor_id)
	{
		case 0:
			actor.ceo.usrname = "        ";
			actor.ceo.ceo_flag = false;

			break;
		case 1:
			actor.sale.usrname = "        ";
			actor.sale.sale_flag = false;

			break;
		case 2:
			actor.finance.usrname = "        ";
			actor.finance.finance_flag = false;

			break;
		case 3:
			actor.purchase.usrname = "        ";
			actor.purchase.purchase_flag = false;

			break;
		case 4:
			actor.product.usrname = "        ";
			actor.product.product_flag = false;

			break;
		default:
			break;
	}

	return;
}

void LobbyMgr::lobby_change_actor_tellothers(long client, string str_usrname, string str_room_id, string str_group_id, string str_actor_id, string str_prv_group_id, string str_prv_actor_id)
{
	char tell_others[5] = {'2', '0', '2', '2', '\0'};
	char c_msg[2048];
	string msg = string(tell_others) + str_usrname + str_room_id + str_group_id + str_actor_id + str_prv_group_id + str_prv_actor_id;

	map<long, string>::iterator ite = Global_Var::lobby.lobby_room[Global_Var::client_index[client].room_id].r_client_name_map.begin();

	memcpy(c_msg, msg.c_str(), msg.length());

	for(; ite != Global_Var::lobby.lobby_room[Global_Var::client_index[client].room_id].r_client_name_map.end(); ite++)
	{
		Global_Var::m_server.sendMsg(ite->first, c_msg, msg.length());
	}

	return;
}

void LobbyMgr::lobby_ready_Cmd(char* msgBuf, long client)
{
	char c_room_id[4];
	char c_group_id[4];
	
	int room_id = 0;
	int group_id = 0;

	bool b_flag = true;

	int num_group_ready = 0;

	string str_room_id;

	int i = 0;

	memcpy(c_room_id, msgBuf + 4, sizeof(long));
	memcpy(c_group_id, msgBuf + 8, sizeof(long));
	
	str_room_id = string(c_room_id).substr(0, 4);

	room_id = atoi(c_room_id);
	group_id = atoi(c_group_id);

	Global_Var::lobby.lobby_room[room_id].r_group[group_id].g_flag = true;

	for(i = 0; i < GROUP_NUM; i++)
	{
		if(Global_Var::lobby.lobby_room[room_id].r_group[i].g_num > 0)
		{
			if(Global_Var::lobby.lobby_room[room_id].r_group[i].g_flag == false)
			{
				b_flag = false;
				break;
			}
			else
			{
				num_group_ready++;
			}
		}
	}

	if(b_flag == true && num_group_ready >= 1)
	{
		map<long, string>::iterator ite = Global_Var::lobby.lobby_room[room_id].r_client_name_map.begin();

		for(; ite != Global_Var::lobby.lobby_room[room_id].r_client_name_map.end(); ite++)
		{
			if(Global_Var::client_index[ite->first].group_id == -1)
			{
				lobby_del_player(ite->first, room_id);
			}
		}
		
		lobby_begin_game(str_room_id);
		//To_do
	}

	return;
}

void lobby_begin_game(string str_room_id)
{
	string b_game = "2024";
	string msg = b_game + str_room_id;
	int room_id = atoi(str_room_id.c_str());
	char c_msg[2048];

	memcpy(c_msg, msg.c_str(), msg.length());

	map<long, string>::iterator ite = Global_Var::client_name_map.begin();

	for(; ite != Global_Var::client_name_map.end(); ite++)
	{
		Global_Var::m_server.sendMsg(ite->first, c_msg, msg.length());
	}

	return;
}

void LobbyMgr::lobby_room_talk_Cmd(char* msgBuf, long client)
{
	char talk[5] = {'2', '0', '2', '5', '\0'};
	char c_room_id[4];
	int room_id = 0;
	char c_msg[2048];
	string msg;

	memcpy(c_room_id, msgBuf + 4, sizeof(long));

	room_id = atoi(c_room_id);

	msg = string(talk) + Global_Var::client_name_map[client] + string(msgBuf + 4);

	memcpy(c_msg, msg.c_str(), msg.length());

	map<long, string>::iterator ite = Global_Var::lobby.lobby_room[room_id].r_client_name_map.begin();

	for(; ite != Global_Var::lobby.lobby_room[room_id].r_client_name_map.end(); ite++)
	{
		Global_Var::m_server.sendMsg(ite->first, c_msg, msg.length());
	}

	return;
}
