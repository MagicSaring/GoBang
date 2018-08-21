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
	CImageList m_iml;		//����ͼ��
	int m_color;			//�����ɫ
	bool m_bWait;			//�ȴ���־
	CGame *m_pGame;			//��Ϸ����ָ��
	void Draw(int x, int y, int color);

public:
	bool m_bOldWait;		//��ǰ�ĵȴ���־
	CString m_strMe;		//�������
	CString m_strAgainst;	//�Է�����
	int m_data[MAX_TABLE_LENGTH][MAX_TABLE_LENGTH];		//��������

public:
	void SetMenuState(bool bEnable);
	void RestoreWait();
	void Clear(bool bWait);
	void SetColor(int color);
	int  GetColor() const;		//��Ա������׺const˵�������ĳ�Ա�����ǲ������޸ĵ�
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

