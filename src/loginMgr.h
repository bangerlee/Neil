//date@ 2011-03-24
#include <string>
#include "Global_Var.h"
#include "CMySQLMan.h"

using namespace std;

#ifndef LOGINMGR_H
#define LOGINMGR_H

class LoginMgr
{
	public:
		LoginMgr(){};
		~LoginMgr(){};

		void LoginCmd(char* msgBuf, long clientID);
	private:
		void login_get_name_pass(char* msg, char* usr, char* pass);
		void login_get_para(char* parameter);
		void login_success(long client);
		void login_fail(long client);
		void login_send_namelist(long client);
		void login_add_name(long client);
		bool login_dup_log(long client, string usr);
		void login_get_info(long client, string usr);
	private:
		char login_usrname[20];
		char login_pass[20];
};		

#endif
