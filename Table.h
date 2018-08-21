#pragma once
#include "afxwin.h"
#include "Game.h"
#include "GoBang.h"
#include "Message.h"


class CTable : public CWnd
{
public:
	CTable();
	virtual ~CTable();

private:
	CImageList m_iml;		//棋子图像
	int m_color;			//玩家颜色
	bool m_bWait;			//等待标志
	CGame *m_pGame;			//游戏基类指针
	void Draw(int x, int y, int color);

public:
	bool m_bOldWait;		//先前的等待标志
	CString m_strMe;		//玩家名字
	CString m_strAgainst;	//对方名字
	int m_data[MAX_TABLE_LENGTH][MAX_TABLE_LENGTH];		//棋盘数据

public:
	void SetMenuState(bool bEnable);
	void RestoreWait();
	void Clear(bool bWait);
	void SetColor(int color);
	int  GetColor() const;		//成员函数后缀const说明函数的成员对象是不允许修改的
	bool SetWait(bool bWait);
	void SetData(int x, int y, int color);
	bool Win(int color) const;
	void SetGameMode(int nGameMode);
	void Back();
	void StepOver();
	void Accept(int nGameMode);
	void Receive();

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

};

