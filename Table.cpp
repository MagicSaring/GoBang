#include "stdafx.h"
#include "Table.h"


CTable::CTable()
{
	//m_color = 0;
	//m_strMe = _T("");
	//m_strAgainst = _T("");

	//初始化玩家姓名
	TCHAR str[50];
	CGoBangApp *pApp = (CGoBangApp *)AfxGetApp();
	::GetPrivateProfileString(_T("Options"), _T("Name"), _T("玩家"), str, 15, pApp->m_szIni);
	m_strMe = str;

	//初始化图像列表
	m_iml.Create(24, 24, ILC_COLOR24 | ILC_MASK, 0, 2);

	//载入黑,白棋子掩码位图
	CBitmap bmpBlack, bmpWhite;
	bmpBlack.LoadBitmap(IDB_BMP_BLACK);
	m_iml.Add(&bmpBlack, 0xff00ff);
	bmpWhite.LoadBitmap(IDB_BMP_WHITE);
	m_iml.Add(&bmpWhite, 0xff00ff);

	//初始化游戏模式
	m_pGame = NULL;
}

// 析构函数，释放m_pGame指针
CTable::~CTable()
{
	// 写入玩家姓名
	CGoBangApp *pApp = (CGoBangApp *)AfxGetApp();
	::WritePrivateProfileString(_T("Options"), _T("Name"), m_strMe, pApp->m_szIni);

	// 写入战绩统计
	TCHAR str[10];
	wsprintf(str, _T("%d"), pApp->m_nWin);
	::WritePrivateProfileString(_T("Stats"), _T("Win"), str, pApp->m_szIni);
	wsprintf(str, _T("%d"), pApp->m_nDraw);
	::WritePrivateProfileString(_T("Stats"), _T("Draw"), str, pApp->m_szIni);
	wsprintf(str, _T("%d"), pApp->m_nLost);
	::WritePrivateProfileString(_T("Stats"), _T("Lost"), str, pApp->m_szIni);

	if (NULL != m_pGame)
		delete m_pGame;
}



void CTable::SetMenuState(bool bEnable)
{

}

// 重新设置先前的等待标志
void CTable::RestoreWait()
{
	SetWait(m_bOldWait);

}
void CTable::Draw(int x, int y, int color)
{
	POINT pt;
	pt.x = 54 + 25 * x;
	pt.y = 101 + 25 * y;
	CDC *pDC = GetDC();
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, 0xff);
	pDC->SelectObject(&pen);
	pDC->SetROP2(R2_NOTXORPEN);
	m_iml.Draw(pDC, color, pt, ILD_TRANSPARENT);
	STEP step;
	// 利用R2_NOTXORPEN擦除先前画出的矩形
	if (!m_pGame->m_StepList.empty())
	{
		// 获取最后一个点
		step = *(m_pGame->m_StepList.begin());
		pDC->MoveTo(54 + 25 * step.x, 101 + 25 * step.y);
		pDC->LineTo(79 + 25 * step.x, 101 + 25 * step.y);
		pDC->LineTo(79 + 25 * step.x, 126 + 25 * step.y);
		pDC->LineTo(54 + 25 * step.x, 126 + 25 * step.y);
		pDC->LineTo(54 + 25 * step.x, 101 + 25 * step.y);
	}
	// 更新最后落子坐标数据，画新的矩形
	step.color = color;
	step.x = x;
	step.y = y;
	m_pGame->m_StepList.push_front(step);
	pDC->MoveTo(54 + 25 * step.x, 101 + 25 * step.y);
	pDC->LineTo(79 + 25 * step.x, 101 + 25 * step.y);
	pDC->LineTo(79 + 25 * step.x, 126 + 25 * step.y);
	pDC->LineTo(54 + 25 * step.x, 126 + 25 * step.y);
	pDC->LineTo(54 + 25 * step.x, 101 + 25 * step.y);
	ReleaseDC(pDC);

}

// 清空棋盘
void CTable::Clear(bool bWait)
{
	int x, y;
	for (x = 0; x < MAX_TABLE_LENGTH; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH; ++y)
		{
			m_data[x][y] = -1;
		}
	}

	//设置等待标志
	m_bWait = bWait;
	Invalidate();

	//删除游戏
	if (m_pGame != NULL)
	{
		delete m_pGame;
		m_pGame = NULL;
	}
}

// 设置玩家颜色
void CTable::SetColor(int color)
{
	m_color = color;
}
// 获取玩家颜色
int CTable::GetColor() const
{
	return m_color;
}

// 设置等待标志，返回先前的等待标志
bool CTable::SetWait(bool bWait)
{
    m_bOldWait = m_bWait;
    m_bWait = bWait;
    return m_bOldWait;
}

// 设置棋盘数据，并绘制棋子
void CTable::SetData(int x, int y, int color)
{
	if (x < 0 || x > 14 || y < 0 || y > 14)
	{
		MessageBeep(MB_OK);
		return;
	}

	m_data[x][y] = color;
	Draw(x, y, color);

}
// 判断指定颜色是否胜利
bool CTable::Win(int color) const
{
	int x, y, z;
	//判断横向
	for (x = 0; x < MAX_TABLE_LENGTH - 4; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH; ++y)
		{
			bool bFlag = true;
			for (z = 0; z < CONNECTION_LENGTH; ++z)
			{
				if (m_data[x + z][y] != color)
				{
					bFlag = false;
					break;
				}
					
			}
			if (bFlag)
			{
				return true;
			}
			
		}
	}

	//判断纵向
	for (x = 0; x < MAX_TABLE_LENGTH; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH - 4; ++y)
		{
			bool bFlag = true;
			for (z = 0; z < CONNECTION_LENGTH; ++z)
			{
				if (m_data[x][y + z] != color)
				{
					bFlag = false;
					break;
				}
			}
			if (bFlag)
			{
				return true;
			}
		}
	}

	//判断斜方向"\"
	for (x = 0; x < MAX_TABLE_LENGTH - 4; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH - 4; ++y)
		{
			bool bFlag = true;
			for (z = 0; z < CONNECTION_LENGTH; ++z)
			{
				if (m_data[x + z][y + z] != color)
				{
					bFlag = false;
					break;
				}
			}
			if (bFlag)
			{
				return true;
			}
		}
	}

	//判断斜方向"/"
	for (x = 4; x < MAX_TABLE_LENGTH; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH - 4; ++y)
		{
			bool bFlag = true;
			for (z = 0; z < CONNECTION_LENGTH; ++z)
			{
				if (m_data[x - z][y + z] != color)
				{
					bFlag = false;
					break;
				}
					
			}
			if (bFlag)
			{
				return true;
			}
		}
	}

	return false;

}

// 设置游戏模式，网络对战将共用此函数
void CTable::SetGameMode(int nGameMode)
{
	m_pGame = new COneGame(this);//创建人机游戏对象
	m_pGame->Init();//人机对战模式初始化

}
void CTable::Back()
{
	m_pGame->Back();
}

// 处理计算机落子后的工作
void CTable::StepOver()
{
	//判断计算机是否胜利
	if (Win(1 - m_color))
	{
		CGoBangApp *pApp = (CGoBangApp *)AfxGetApp();
		pApp->m_nLost++;
		
		CDialog *pDlg = (CDialog *)GetParent();
		PlaySound(MAKEINTRESOURCE(IDR_WAVE_LOST), NULL, SND_RESOURCE | SND_SYNC);
		pDlg->MessageBox(_T("您输了，不过不要灰心，失败乃成功之母哦！"), _T("失败"), MB_ICONINFORMATION);
		pDlg->GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);
		return;
	}
	m_bWait = false;

}

// 接受连接，网络对战将共用此函数
void CTable::Accept(int nGameMode)
{
	SetColor(0);	//设置当期行棋颜色
	Clear(FALSE);	//清空棋牌,等待标志m_bWait = FALSE
	SetGameMode(nGameMode);		//设置游戏模式为1
}

// 接收来自对方的数据，网络对战将对此函数扩展，处理更多的消息
void CTable::Receive()
{
	MSGSTRUCT msgRecv;
	m_pGame->ReceiveMsg(&msgRecv);
	switch(msgRecv.uMsg)
	{
	case MSG_DROPDOWN:
		{
			PlaySound(MAKEINTRESOURCEW(IDR_WAVE_PUT), NULL, SND_RESOURCE | SND_SYNC);
			SetData(msgRecv.x, msgRecv.y, msgRecv.color);
			//大于1步才能悔棋
			//TODO::
			StepOver();
		}break;
	//网络对战在此处理网络消息
	}

}

// 消息映射表
BEGIN_MESSAGE_MAP(CTable, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CTable::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages

	//装载棋盘
	CBitmap bmp;
	CPen pen;
	bmp.LoadBitmap(IDB_BMP_TABLE);
					   
	CDC deMem;
	deMem.CreateCompatibleDC(&dc);
					   
	pen.CreatePen(PS_SOLID, 1, 0xff);
	deMem.SelectObject(&bmp);
	deMem.SelectObject(&pen);
	deMem.SetROP2(R2_NOTXORPEN);
					   
	// 根据棋盘数据绘制棋子
	int x, y;
	POINT pt;
	for (x = 0; x < MAX_TABLE_LENGTH; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH; ++y)
		{
			if(m_data[x][y] != -1)
			{ 
				pt.x = 54 + 25 * x;
				pt.y = 101 + 25 * y;
				m_iml.Draw(&deMem, m_data[x][y], pt, ILD_TRANSPARENT);
			}
		}
	}
	// 绘制最后落子的指示矩形
	if (NULL != m_pGame && !m_pGame->m_StepList.empty())
	{
		STEP step = *(m_pGame->m_StepList.begin());
		deMem.MoveTo(54 + 25 * step.x, 101 + 25 * step.y);
		deMem.LineTo(79 + 25 * step.x, 101 + 25 * step.y);
		deMem.LineTo(79 + 25 * step.x, 126 + 25 * step.y);
		deMem.LineTo(54 + 25 * step.x, 126 + 25 * step.y);
		deMem.LineTo(54 + 25 * step.x, 101 + 25 * step.y);
	}
					   
	//完成绘制
	dc.BitBlt(0, 0, 480, 509, &deMem, 0, 0, SRCCOPY);
	deMem.SelectObject(bmp);
}


void CTable::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CWnd::OnLButtonUp(nFlags, point);
	STEP stepPut;
	if (m_bWait)
	{
		MessageBeep(MB_OK);
		return;
	}
	
	int x, y;
	x = (point.x - 54) / 25;
	y = (point.y - 101) / 25;
	//如果在(0, 0) ～ (14, 14)范围内,且该坐标没有落子,则落于此子,否则发声警告并退出过程
	if (x < 0 || x > 14 || y < 0 || y > 14 || m_data[x][y] != -1)
	{
		MessageBeep(MB_OK);
		return;
	}
	else
	{
		//如果位置合法,则落子
		SetData(x, y, m_color);
		stepPut.x = x;
		stepPut.y = y;
		stepPut.color = m_color;
	
		//大于1步才能悔棋
		GetParent()->GetDlgItem(IDC_BTN_BACK)->EnableWindow(m_pGame->m_StepList.size() > 1);
	}
		
	//判端胜利的情况
	if (Win(m_color))
	{
		CGoBangApp *pApp = (CGoBangApp *)AfxGetApp();
		pApp->m_nWin++;
		m_pGame->Win( stepPut );
	
		CDialog *pDlg = (CDialog *)GetParent();
		PlaySound(MAKEINTRESOURCEW(IDR_WAVE_WIN), NULL, SND_RESOURCE | SND_SYNC);
		pDlg->MessageBox(_T("恭喜，您获得了胜利！"), _T("胜利"), MB_ICONINFORMATION);
	
		pDlg->GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);
		m_bWait = true;
		return;
	}
	else
	{
		//开始等待
		m_bWait = true;
		//发送落子信息
		PlaySound(MAKEINTRESOURCEW(IDR_WAVE_PUT), NULL, SND_RESOURCE | SND_SYNC);
		m_pGame->SendStep(stepPut);
	}
}
