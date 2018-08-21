#pragma once
#include "afx.h"
#include <list>
#include "Message.h"	//消息类定义文件
#include "GoBang.h"

class CTable;
class CGame : public CObject

{
public:
	CGame(CTable *pTable) : m_pTable(pTable) {}
	virtual ~CGame();

protected:
	CTable *m_pTable;

public:
	//落子步骤
	std::list<STEP> m_StepList;

public:
	//初始化工作,不同的游戏方式初始化也不一样
	virtual void Init() = 0;
	//处理胜利后的情况，CTwoGame需要改写此函数完成善后工作
	virtual void Win(const STEP &stepSend);
	//发送己方落子
	virtual void SendStep(const STEP &stepSend) = 0;
	//接收对方消息
	virtual void ReceiveMsg(MSGSTRUCT *pMsg) = 0;
	//发送悔棋请求
	virtual void Back() = 0;
	
};

// COneGame 类定义
class COneGame : public CGame
{

public:
	COneGame(CTable *pTable) : CGame(pTable) {}
	virtual ~COneGame();

	virtual void Init();
	virtual void SendStep(const STEP &stepSend);
	virtual void ReceiveMsg(MSGSTRUCT *pMsg);
	virtual void Back();

private:
	//给出下了一个子后的分数
	int GiveScore(const STEP &setPut);
	void GetTable(int tempTable[][MAX_TABLE_LENGTH], int nowTable[][MAX_TABLE_LENGTH]);
	//找空的位置
	bool SearchBlank(int &x, int &y, const int nowTable[][MAX_TABLE_LENGTH]);

private:
	bool	m_Computer[MAX_TABLE_LENGTH][MAX_TABLE_LENGTH][WIN_COMBINATION_LENGTH];	//电脑获胜组合
	bool	m_Player[MAX_TABLE_LENGTH][MAX_TABLE_LENGTH][WIN_COMBINATION_LENGTH];		//玩家获胜组合
	int		m_Win[2][WIN_COMBINATION_LENGTH];		//各个获胜组合填入的棋子数
	bool	m_bStart;			//游戏是否刚刚开始											
	STEP	m_step;				//保存落子结果

	//以下三个成员做悔棋之用
	bool	m_bOldPlayer[WIN_COMBINATION_LENGTH];
	bool	m_bOldComputer[WIN_COMBINATION_LENGTH];
	int		m_nOldWin[2][WIN_COMBINATION_LENGTH];
};

