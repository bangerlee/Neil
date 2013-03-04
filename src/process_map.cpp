//date@ 2011-03-24
#include "process_map.h"
#include <iostream>

using namespace std;

ProcessMap::ProcessMap()
{
	m_processFn[LOGIN] = &Process::LoginCmd;
	
//	m_processFn[EnterRoom] = &Process::lobby_enter_room_Cmd;
//	m_processFn[QuitRoom] = &Process::lobby_quit_room_Cmd;
//	m_processFn[ChooseActor] = &Process::lobby_choose_actor_Cmd;
//	m_processFn[ChangeActor] = &Process::lobby_change_actor_Cmd;
//	m_processFn[Ready] = &Process::lobby_ready_Cmd;
//	m_processFn[RoomTalk] = &Process::lobby_room_talk_Cmd;
}

ProcessMap::~ProcessMap()
{
}

void ProcessMap::msg_process()
{
	unsigned long clientID;
	char msgBuf[MSGBUF_SIZE];
	size_t length;
	long msgType;
	char temp[20];

	for(;;)
	{
		if(epoll_server.getMsg(&clientID, msgBuf, &length) == RET_OK)
		{
			memcpy(temp, msgBuf, sizeof(long));

			msgType = atol(temp);

			if(m_processFn.count(msgType) != 0)
			{
				(m_process.*m_processFn[msgType])(msgBuf, clientID);
			}
			

			cout << "receive from " << clientID << " " <<  msgBuf << endl;
		}
		else
		{
			sleep(1);
		}
	}
}
