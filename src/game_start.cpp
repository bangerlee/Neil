#include <iostream>
#include "process_map.h"

using namespace std;

int main(int argc, char* argv[])
{
	ProcessMap m_processMap;

	m_processMap.epoll_server.init();

	cout << "Start" << endl;

//	Global_Var::lobby.lobby_init();

	m_processMap.msg_process();

	return 0;
}
