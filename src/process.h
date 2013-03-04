//date@ 2011-03-24
#ifndef PROCESS_H
#define PROCESS_H

#define declare_cmd(function) \
	void function(char* msgBuf, long clientID);

#define realize_cmd(function, caller) \
	void Process::function(char* msgBuf, long clientID) \
	{ \
		caller.function(msgBuf, clientID); \
	} \

typedef long MSGTYPE;
enum
{
	/*loginMgr
	 */
	LOGIN       = 1001,
	
	/*lobbyMgr
	 */
	EnterRoom   = 1008,
	QuitRoom    = 1012,
	ChooseActor = 1021,
	ChangeActor = 1022,
	Ready       = 1024,
	RoomTalk    = 1025,
};

class Process
{
	public:
		Process(){};
		~Process(){};

		declare_cmd(LoginCmd);
		
		declare_cmd(lobby_enter_room_Cmd);
		declare_cmd(lobby_quit_room_Cmd);
		declare_cmd(lobby_choose_actor_Cmd);
		declare_cmd(lobby_change_actor_Cmd);
		declare_cmd(lobby_ready_Cmd);
		declare_cmd(lobby_room_talk_Cmd);

};

#endif
