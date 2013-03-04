#pragma once
#define SEND_TIME 12
#define INTERVALTIME 20
struct Product
{
	string name;
	int per_day;
	int price;
};
class NPCMgr
{
public:
	NPCMgr(void);
	~NPCMgr(void);
	void Add_NewGame(int i_RoomID);
	void init_OrderList(int i_RoomID);
	bool StartGameThread();
	void SaleOrderCmd(char *msgBuf,long client);
	void FinanceOrderCmd(char *msgBuf,long client);
	void GameTalkCmd(char *msgBuf,long client);
	void BuyOrderCmd(char *msgBuf,long client);
	void ProduceOrderCmd(char *msgBuf,long client);
	void CompleteProduceCmd(char *msgBuf,long client);
	void Hand_In_OrderCmd(char *msgBuf,long client);
	void Game_Over(int i_RoomID);
	string TimeManage();
	void GameBeginTime(int i_RoomID);
	void Actor_Info(int i_RoomID);
	void init_playing_group(int i_RoomID);
	void Get_MoneyCmd(char *msgBuf,long client);
	void Send_Winner(int i_RoomID);
	void Order_OverTimeCmd(char *msgBuf,long client);
	void Get_LoanCmd(char *msgBuf,long client);
	void Pay_LoanCmd(char *msgBuf,long client);
	void Big_ThingCmd(char *msgBuf,long client);
	void Send_BigThing(int i_RoomID);
	HANDLE m_GameThread;
private:
	friend unsigned int WINAPI GameThread(LPVOID lpParam);
	Product product[5];
};
