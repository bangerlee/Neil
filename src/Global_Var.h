#include <iostream>
#include <map>
#include <sstream>
#include <time.h>
#include <list>
#include <set>
#include <string>
#include "./net/epoll_server.h"

class Process;
class Epoll_Server;
class LobbyMgr;

using namespace std;

#define MAXSIZE 2048

typedef void (Process::*pFn)(char*, long);

typedef struct _Index
{
	int room_id;
	int group_id;
	int actor_id;
}Index;

class Epoll_Server;

class Global_Var
{
	public:
		Global_Var(){};
		~Global_Var(){};

		static Epoll_Server m_server;
		static map<long, pFn> g_process_fn;
		
		static map<long, string> client_name_map;
		static map<string, long> name_client_map;

		static map<long, Index> client_index;

		static LobbyMgr lobby;
};
