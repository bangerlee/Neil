#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Global_Var.h"

using namespace std;

#define ROOM_NUM  5
#define GROUP_NUM 6
#define NAME_SIZE 6
#define ACTOR_NUM 5

class LobbyMgr
{
	private:
		typedef struct _CEO
		{
			string usrname;
			bool ceo_flag;
			int id;
		}CEO;

		typedef struct _Sale
		{
			string usrname;
			bool sale_flag;
			int id;
		}Sale;

		typedef struct _Product
		{
			string usrname;
			bool product_flag;
			int id;
		}Product;

		typedef struct _Finance
		{
			string usrname;
			bool finance_flag;
			int id;
		}Finance;

		typedef struct _Purchase
		{
			string usrname;
			bool purchase_flag;
			int id;
		}Purchase;

	public:
		typedef struct _Actor
		{
			CEO       ceo;
			Product   product;
			Purchase  purchase;
			Finance   finance;
			Sale      sale;
		}Actor;

		typedef struct _Group
		{
			int  g_id;
			char g_name[NAME_SIZE];
			int  g_num;
			bool g_flag;

			map<long, string> g_client_name_map;
			map<string, long> g_name_client_map;

			Actor g_actor;
		}Group;

		typedef struct _Room
		{
			int  r_id;
			char r_name[NAME_SIZE];
			int  r_num;
			bool r_flag;

			map<long, string> r_client_name_map;
			map<string, long> r_name_client_map;

			Group r_group[GROUP_NUM];

			int r_group_playing[GROUP_NUM];
		
			int r_group_playing_num;
		//	map<string, string> s_groupid_money_map;

		//	map<string, string> s_groupid_bigThing_map;
		}Room;

	public:
		Room lobby_room[ROOM_NUM];

	public:
		LobbyMgr(){};
		~LobbyMgr(){};

		void lobby_init(void);
		
		void lobby_enter_room_Cmd(char* msgBuf, long client);

		void lobby_quit_room_Cmd(char* msgBuf, long client);

		void lobby_choose_actor_Cmd(char* msgBuf, long client);

		void lobby_change_actor_Cmd(char* msgBuf, long client);

		void lobby_ready_Cmd(char* msgBuf, long client);

		void lobby_room_talk_Cmd(char* msgBuf, long client);
	
	private:
		/*for lobby_enter_room_cmd
		 *
		 */
		void lobby_enter_room_success(long client);
		void lobby_send_playerlist(long client, int room_id);
		void lobby_enter_room_tellothers(long client, int room_id);
		void lobby_update(long client, int room_id);
		void lobby_inside_room_info(long client, int room_id);
		void lobby_enter_room_fail(long client);

		/*for lobby_quit_room_cmd
		 *
		 */
	public:
		void lobby_del_player(long client, int room_id);
	private:
		void lobby_del_player_quit_room(long client, int room_id);
		void lobby_del_player_del_actor(long client, int actor_id);
		void lobby_del_player_quit_actor(long client);

		/*for lobby_choose_actor_Cmd
		 *
		 */
		bool lobby_actor_selected(int room_id, int group_id, int actor_id);
		void lobby_add_actor(long client, int actor_id, string str_usrname);
		void lobby_update_actor_info(long client, string str_usrname, string str_room_id, string str_group_id, string str_actor_id);

		/*for lobby_change_actor_Cmd
		 *
		 */
		void lobby_del_actor(long client, int prv_actor_id);
		void lobby_change_actor_tellothers(long client, string str_usrname, string str_room_id, string str_group_id, string str_actor_id, string str_prv_group_id, string str_prv_actor_id);

		/*for lobby_ready_Cmd
		 *
		 */
		void lobby_begin_game(string str_room_id);
};
