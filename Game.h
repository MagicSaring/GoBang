#pragma once
#include "afx.h"
#include <list>
#include "Message.h"	//��Ϣ�ඨ���ļ�
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
	//���Ӳ���
	std::list<STEP> m_StepList;

public:
	//��ʼ������,��ͬ����Ϸ��ʽ��ʼ��Ҳ��һ��
	virtual void Init() = 0;
	//����ʤ����������CTwoGame��Ҫ��д�˺�������ƺ���
	virtual void Win(const STEP &stepSend);
	//���ͼ�������
	virtual void SendStep(const STEP &stepSend) = 0;
	//���նԷ���Ϣ
	virtual void ReceiveMsg(MSGSTRUCT *pMsg) = 0;
	//���ͻ�������
	virtual void Back() = 0;
	
};

// COneGame �ඨ��
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
	//��������һ���Ӻ�ķ���
	int GiveScore(const STEP &setPut);
	void GetTable(int tempTable[][MAX_TABLE_LENGTH], int nowTable[][MAX_TABLE_LENGTH]);
	//�ҿյ�λ��
	bool SearchBlank(int &x, int &y, const int nowTable[][MAX_TABLE_LENGTH]);

private:
	bool	m_Computer[MAX_TABLE_LENGTH][MAX_TABLE_LENGTH][WIN_COMBINATION_LENGTH];	//���Ի�ʤ���
	bool	m_Player[MAX_TABLE_LENGTH][MAX_TABLE_LENGTH][WIN_COMBINATION_LENGTH];		//��һ�ʤ���
	int		m_Win[2][WIN_COMBINATION_LENGTH];		//������ʤ��������������
	bool	m_bStart;			//��Ϸ�Ƿ�ոտ�ʼ											
	STEP	m_step;				//�������ӽ��

	//����������Ա������֮��
	bool	m_bOldPlayer[WIN_COMBINATION_LENGTH];
	bool	m_bOldComputer[WIN_COMBINATION_LENGTH];
	int		m_nOldWin[2][WIN_COMBINATION_LENGTH];
};

