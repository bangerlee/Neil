#pragma once

class ChartMgr
{
public:
	ChartMgr(void);
	~ChartMgr(void);
	void ChartCmd(char *msgBuf, long client);
};
