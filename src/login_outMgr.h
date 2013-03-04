#include <map>
#include "lobbyMgr.h"

class Login_OutMgr
{
	public:
		Login_OutMgr(){};
		~Login_OutMgr(){};

		void login_out_Cmd(char* msgBuf, long client);
	private:
		void login_out_del_name(long client);
};
