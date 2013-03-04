#include "StdAfx.h"
#include "NPCMgr.h"
#include <math.h>
map<int,pair<long,long>> GlobalVar::TimeTracer;
NPCMgr::NPCMgr(void)
{
	product[0].name="¸Ö±Ê";
	product[0].per_day=20;
	product[0].price=55;

	product[1].name="Ì×Ì×";
	product[1].per_day=20;
	product[1].price=45;

	product[2].name="ÉñÓÍ";
	product[2].per_day=10;
	product[2].price=60;

	product[3].name="ÍÞÍÞ";
	product[3].per_day=10;
	product[3].price=65;

	product[4].name="Î°¸ç";
	product[4].per_day=10;
	product[4].price=60;
}

NPCMgr::~NPCMgr(void)
{
}
void NPCMgr::Add_NewGame(int i_RoomID)
{
	//SYSTEMTIME sys_now;
	//GetLocalTime(&sys_now);
	long curTime=GetTickCount();
	pair<long,long> Tracer(curTime/1000,11);
	GlobalVar::TimeTracer[i_RoomID]=Tracer;
	init_playing_group(i_RoomID);
	GameBeginTime(i_RoomID);
	Actor_Info(i_RoomID);
	init_OrderList(i_RoomID);
}
void NPCMgr::init_playing_group(int i_RoomID)
{
	int index=0;
	for(index=0;index<GROUP_NUM;index++)
	{
		if(GlobalVar::Lobby.LobbyRoom[i_RoomID].group[index].G_flag==true)
		{
			GlobalVar::Lobby.LobbyRoom[i_RoomID].Playing_Group[GlobalVar::Lobby.LobbyRoom[i_RoomID].num_playing_group]=index;
			GlobalVar::Lobby.LobbyRoom[i_RoomID].num_playing_group++;
		}
	}
}
void NPCMgr::GameBeginTime(int i_RoomID)
{
	string msg="2199"+TimeManage();
	char c_msg[2048];
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].Room_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].Room_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}
void NPCMgr::Actor_Info(int i_RoomID)
{
	string r_actorinfo="2198";
	int index=0;
	int index_1=0;
	char c_RoomID[4];
	itoa(i_RoomID,c_RoomID,10);
	string s_RoomID(c_RoomID);
	string s_GroupID;
	char c_GroupID[4];
	string msg="";
	string act_usrname="";
	char c_msg[2048];
	for(index=s_RoomID.length();index<4;index++)
	{
		s_RoomID+=" ";
	}
	for(index=0;index<GROUP_NUM;index++)
	{
		if(GlobalVar::Lobby.LobbyRoom[i_RoomID].group[index].G_flag==true)
		{
			itoa(index,c_GroupID,10);
			s_GroupID=string(c_GroupID);
			for(index_1=s_GroupID.length();index_1<4;index_1++)
			{
				s_GroupID+=" ";
			}
			act_usrname=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[index].actor.ceo.usrname+GlobalVar::Lobby.LobbyRoom[i_RoomID].group[index].actor.sales.usrname+GlobalVar::Lobby.LobbyRoom[i_RoomID].group[index].actor.finance.usrname+GlobalVar::Lobby.LobbyRoom[i_RoomID].group[index].actor.purchase.usrname+GlobalVar::Lobby.LobbyRoom[i_RoomID].group[index].actor.poduce.usrname;
			msg=r_actorinfo+s_RoomID+s_GroupID+act_usrname;
			memcpy(c_msg,msg.c_str(),msg.length());
			map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[index].Group_Client_Namemap.begin();
			for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[index].Group_Client_Namemap.end();ite++)
			{
				GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
			}
		}
	}
}
void NPCMgr::init_OrderList(int i_RoomID)
{
	string r_init_orderlist="2200";
	int index=0;
	int index_1=0;
	string temp_msg="";

	for(index=0;index<10;index++)
	{
		int id=index+(11-GlobalVar::TimeTracer[i_RoomID].second)*10;
		int product_id=rand()%5;
		int num=(rand()%401);
		if(num<100)
			num+=100;
		if(num>300)
			num-=100;
		int product_time=ceil(float(num/product[product_id].per_day))+20;
		float profit=float(rand()%8+8)/float(10);
		long money=(num*product[product_id].price)*profit;
		
		char c_id[4];
		char c_product_id[4];
		char c_num[8];
		char c_product_time[4];
		char c_money[8];

		_itoa(id,c_id,10);
		_itoa(product_id,c_product_id,10);
		_itoa(num,c_num,10);
		_itoa(product_time,c_product_time,10);
		_itoa(money,c_money,10);

		string s_id(c_id);
		string s_product_id(c_product_id);
		string s_num(c_num);
		string s_product_time(c_product_time);
		string s_money(c_money);

		for(index_1=s_id.length();index_1<4;index_1++)
		{
			s_id+=" ";
		}
		for(index_1=s_product_id.length();index_1<4;index_1++)
		{
			s_product_id+=" ";
		}
		for(index_1=s_num.length();index_1<8;index_1++)
		{
			s_num+=" ";
		}
		for(index_1=s_product_time.length();index_1<4;index_1++)
		{
			s_product_time+=" ";
		}
		for(index_1=s_money.length();index_1<8;index_1++)
		{
			s_money+=" ";
		}
		temp_msg+=s_id+s_product_id+s_num+s_product_time+s_money;
	}
	char c_msg[2048];
	//string msg=string(r_init_orderlist)+Order_0+Order_1+Order_2+Order_3+Order_4+Order_5+Order_6+Order_7+Order_8+Order_9;
	string msg=r_init_orderlist+temp_msg;
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].Room_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].Room_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}
unsigned int WINAPI GameThread(LPVOID lpParam)
{
	while(1)
	{
		srand((unsigned int)GetCurrentThreadId());
		if(GlobalVar::TimeTracer.size()>0)
		{
			//SYSTEMTIME sys_now;
			//GetLocalTime(&sys_now);
			bool flag=false;
			map<int,pair<long,long>>::iterator ite=GlobalVar::TimeTracer.begin();
			for(;ite!=GlobalVar::TimeTracer.end();ite++)
			{
				if(GetTickCount()/1000-ite->second.first>=INTERVALTIME&&ite->second.second>=0)
				{
					ite->second.second--;
					ite->second.first+=INTERVALTIME;			
					//if(ite->second.second==0)
					//	continue;
					if(ite->second.second<0)
					{
						GlobalVar::NPCController.Game_Over(ite->first);
						//·¢ËÍÓÎÏ·½áÊøÐÅÏ¢
						//GlobalVar::Lobby.LobbyRoom[ite->first].num_playing_group=0;
						//GlobalVar::Lobby.LobbyRoom[ite->first].s_GroupID_Moneymap.clear();
						//GlobalVar::TimeTracer.erase(ite);
						flag=true;
						continue;
					}
					if(ite->second.second>=0)
						GlobalVar::NPCController.init_OrderList(ite->first);
				}
			}
			//if(flag==true)
			//	GlobalVar::TimeTracer.erase(ite--);
		}
		Sleep(1);
	}
	_endthreadex( 0 );
	return 0;
}

bool NPCMgr::StartGameThread()
{
	m_GameThread=(HANDLE)_beginthreadex(NULL, 0, &GameThread, this, 0, NULL);
	if(!m_GameThread)
		return false;
	//printf("Start up SendThread\n");
	//WriteLog("Start up SendThread");
	return true;
}

void NPCMgr::SaleOrderCmd(char *msgBuf,long client)
{
	string r_saleorder="2201";
	char c_RoomID[4];
	char c_GroupID[4];
	char c_OrderID[4];
	string msg;

	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));
	memcpy(c_OrderID,msgBuf+12,sizeof(long));

	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);
	string s_OrderID=string(c_OrderID).substr(0,4);

	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	int i_OrderID=atoi(s_OrderID.c_str());

	msg=r_saleorder+s_RoomID+s_GroupID+s_OrderID+TimeManage();
	char c_msg[2048];
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}
void NPCMgr::FinanceOrderCmd(char *msgBuf,long client)
{
	string r_financeorder="2202";
	char c_RoomID[4];
	char c_GroupID[4];
	char c_OrderID[4];
	string msg;

	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));
	memcpy(c_OrderID,msgBuf+12,sizeof(long));

	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);
	string s_OrderID=string(c_OrderID).substr(0,4);

	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	int i_OrderID=atoi(s_OrderID.c_str());

	msg=r_financeorder+s_GroupID+s_OrderID+TimeManage();
	char c_msg[2048];
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].Room_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].Room_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}
void NPCMgr::GameTalkCmd(char *msgBuf,long client)
{
	string r_gametalk="2030";
	char c_RoomID[4];
	char c_GroupID[4];
	char c_ActorID[4];
	char c_msg[2048];
	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));
	memcpy(c_ActorID,msgBuf+12,sizeof(long));

	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);
	string s_ActorID=string(c_ActorID).substr(0,4);

	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	string msg;

	msg=r_gametalk+s_RoomID+s_GroupID+s_ActorID+GlobalVar::Client_Namemap[client]+string(msgBuf+16);
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}

void NPCMgr::BuyOrderCmd(char *msgBuf,long client)
{
	string r_buyorder="2204";
	char c_RoomID[4];
	char c_GroupID[4];
	char c_msg[2048];
	string msg;
	string buy_info(msgBuf+sizeof(long)*3);
	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));
	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);
	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	msg=r_buyorder+s_RoomID+s_GroupID+buy_info+TimeManage();
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}

void NPCMgr::ProduceOrderCmd(char *msgBuf,long client)
{
	string r_produceorder="2206";
	char c_RoomID[4];
	char c_GroupID[4];
	char c_msg[2048];
	string msg=r_produceorder+string(msgBuf+sizeof(long))+TimeManage();
	//string buy_info(msgBuf+sizeof(long)*3);
	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));
	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);
	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}
void NPCMgr::CompleteProduceCmd(char *msgBuf,long client)
{
	string r_completeproduce="2208";
	char c_RoomID[4];
	char c_GroupID[4];
	char c_msg[2048];
	string msg=r_completeproduce+string(msgBuf+sizeof(long))+TimeManage();
	//string buy_info(msgBuf+sizeof(long)*3);
	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));
	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);
	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}
void NPCMgr::Hand_In_OrderCmd(char *msgBuf,long client)
{
	string r_completeproduce="2210";

	char c_RoomID[4];
	char c_GroupID[4];
	char c_msg[2048];

	//string buy_info(msgBuf+sizeof(long)*3);
	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));
	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);
	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());

	string msg=r_completeproduce+string(msgBuf+sizeof(long))+TimeManage();
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}

void NPCMgr::Game_Over(int i_RoomID)
{
	string r_game_over="2212";
	char c_msg[2048];
	char c_RoomID[4];
	itoa(i_RoomID,c_RoomID,10);
	string s_RoomID(c_RoomID);
	int index=0;
	for(index=s_RoomID.length();index<4;index++)
	{
		s_RoomID+=" ";
	}
	string msg=r_game_over+s_RoomID+TimeManage();
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Client_Namemap.begin();
	for(;ite!=GlobalVar::Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}
string NPCMgr::TimeManage()
{
	SYSTEMTIME sys_now;
	GetLocalTime(&sys_now);
	int i_hours=int(sys_now.wHour);
	int i_Minutes=int(sys_now.wMinute);
	int i_Second=int(sys_now.wSecond);
	char c_hours[4];
	char c_minutes[4];
	char c_second[4];
	itoa(i_hours,c_hours,10);
	itoa(i_Minutes,c_minutes,10);
	itoa(i_Second,c_second,10);
	string s_hours(c_hours);
	string s_Minutes(c_minutes);
	string s_Second(c_second);
	int index=0;
	for(index=s_hours.length();index<4;index++)
	{
		s_hours+=" ";
	}
	for (index=s_Minutes.length();index<4;index++)
	{
		s_Minutes+=" ";
	}
	for(index=s_Second.length();index<4;index++)
	{
		s_Second+=" ";
	}
	return s_hours+s_Minutes+s_Second;
}
void NPCMgr::Get_MoneyCmd(char *msgBuf,long client)
{
	char c_RoomID[4];
	char c_GroupID[4];
	char c_money[8];
	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));
	memcpy(c_money,msgBuf+12,sizeof(long)*2);
	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);
	string s_money=string(c_money).substr(0,8);
	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_Moneymap[s_GroupID]=s_money;
	if(GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_Moneymap.size()==GlobalVar::Lobby.LobbyRoom[i_RoomID].num_playing_group)
	{
		Send_Winner(i_RoomID);
	}
}
void NPCMgr::Send_Winner(int i_RoomID)
{
	string r_sendwinner="2213";
	char c_RoomID[4];
	itoa(i_RoomID,c_RoomID,10);
	string s_RoomID(c_RoomID);
	int index=0;
	string msg="";
	char c_msg[2048];
	for(index=s_RoomID.length();index<4;index++)
	{
		s_RoomID+=" ";
	}
	char c_Num_Playing_Group[4];
	itoa(GlobalVar::Lobby.LobbyRoom[i_RoomID].num_playing_group,c_Num_Playing_Group,10);
	string s_Num_Playing_Group(c_Num_Playing_Group);
	for(index=s_Num_Playing_Group.length();index<4;index++)
	{
		s_Num_Playing_Group+=" ";
	}
	map<string,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_Moneymap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_Moneymap.end();ite++)
	{
		msg+=ite->first+ite->second;
	}
	msg=r_sendwinner+s_RoomID+s_Num_Playing_Group+msg;
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite_1=GlobalVar::Lobby.LobbyRoom[i_RoomID].Room_Client_Namemap.begin();
	for(;ite_1!=GlobalVar::Lobby.LobbyRoom[i_RoomID].Room_Client_Namemap.end();ite_1++)
	{
		GlobalVar::m_Server.SendMsg(ite_1->first,c_msg,msg.length());
	}
	//GlobalVar::Lobby.LobbyRoom[i_RoomID].num_playing_group=0;
	//GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_Moneymap.clear();
}

void NPCMgr::Order_OverTimeCmd(char *msgBuf,long client)
{
	string r_order_overtime="2215";
	char c_RoomID[4];
	char c_GroupID[4];
	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));

	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);

	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	string msg=r_order_overtime+string(msgBuf+4)+TimeManage();
	char c_msg[2048];
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}

void NPCMgr::Get_LoanCmd(char *msgBuf,long client)
{
	string r_get_loan="2216";
	char c_RoomID[4];
	char c_GroupID[4];
	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));

	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);

	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	string msg=r_get_loan+string(msgBuf+4)+TimeManage();
	char c_msg[2048];
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}
void NPCMgr::Pay_LoanCmd(char *msgBuf,long client)
{
	string r_pay_loan="2217";
	char c_RoomID[4];
	char c_GroupID[4];
	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));

	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);

	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	string msg=r_pay_loan+string(msgBuf+4)+TimeManage();
	char c_msg[2048];
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].group[i_GroupID].Group_Client_Namemap.end();ite++)
	{
		GlobalVar::m_Server.SendMsg(ite->first,c_msg,msg.length());
	}
}

void NPCMgr::Big_ThingCmd(char *msgBuf,long client)
{
	char c_RoomID[4];
	char c_GroupID[4];
	//char c_content[2048];
	memcpy(c_RoomID,msgBuf+4,sizeof(long));
	memcpy(c_GroupID,msgBuf+8,sizeof(long));
	//memcpy(c_content,msgBuf+12,sizeof(long)*2);
	string content(msgBuf+12);
	string s_RoomID=string(c_RoomID).substr(0,4);
	string s_GroupID=string(c_GroupID).substr(0,4);
	//string s_money=string(c_money).substr(0,8);
	int i_RoomID=atoi(s_RoomID.c_str());
	int i_GroupID=atoi(s_GroupID.c_str());
	GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_BigThing[s_GroupID]=content;
	if(GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_BigThing.size()==GlobalVar::Lobby.LobbyRoom[i_RoomID].num_playing_group)
	{
		Send_BigThing(i_RoomID);
	}
}

void NPCMgr::Send_BigThing(int i_RoomID)
{
	string r_sendbigthing="2218";
	char c_RoomID[4];
	itoa(i_RoomID,c_RoomID,10);
	string s_RoomID(c_RoomID);
	int index=0;
	string msg="";
	char c_msg[2048];
	for(index=s_RoomID.length();index<4;index++)
	{
		s_RoomID+=" ";
	}
	char c_Num_Playing_Group[4];
	itoa(GlobalVar::Lobby.LobbyRoom[i_RoomID].num_playing_group,c_Num_Playing_Group,10);
	string s_Num_Playing_Group(c_Num_Playing_Group);
	for(index=s_Num_Playing_Group.length();index<4;index++)
	{
		s_Num_Playing_Group+=" ";
	}
	map<string,string>::iterator ite=GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_BigThing.begin();
	for(;ite!=GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_BigThing.end();ite++)
	{
		msg+=ite->first+ite->second;
	}
	msg=r_sendbigthing+s_RoomID+s_Num_Playing_Group+msg;
	memcpy(c_msg,msg.c_str(),msg.length());
	map<long,string>::iterator ite_1=GlobalVar::Lobby.LobbyRoom[i_RoomID].Room_Client_Namemap.begin();
	for(;ite_1!=GlobalVar::Lobby.LobbyRoom[i_RoomID].Room_Client_Namemap.end();ite_1++)
	{
		GlobalVar::m_Server.SendMsg(ite_1->first,c_msg,msg.length());
	}
	GlobalVar::Lobby.LobbyRoom[i_RoomID].num_playing_group=0;
	GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_Moneymap.clear();
	GlobalVar::Lobby.LobbyRoom[i_RoomID].s_GroupID_BigThing.clear();
	map<int,pair<long,long>>::iterator ite_2=GlobalVar::TimeTracer.find(i_RoomID);
	GlobalVar::TimeTracer.erase(ite_2);
	for(int index_1=0;index_1<GROUP_NUM;index_1++)
	{
		GlobalVar::Lobby.LobbyRoom[i_RoomID].group[index_1].G_flag=false;
	}

}
