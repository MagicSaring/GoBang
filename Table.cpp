#include "stdafx.h"
#include "Table.h"


CTable::CTable()
{
	//m_color = 0;
	//m_strMe = _T("");
	//m_strAgainst = _T("");

	//��ʼ���������
	TCHAR str[50];
	CGoBangApp *pApp = (CGoBangApp *)AfxGetApp();
	::GetPrivateProfileString(_T("Options"), _T("Name"), _T("���"), str, 15, pApp->m_szIni);
	m_strMe = str;

	//��ʼ��ͼ���б�
	m_iml.Create(24, 24, ILC_COLOR24 | ILC_MASK, 0, 2);

	//�����,����������λͼ
	CBitmap bmpBlack, bmpWhite;
	bmpBlack.LoadBitmap(IDB_BMP_BLACK);
	m_iml.Add(&bmpBlack, 0xff00ff);
	bmpWhite.LoadBitmap(IDB_BMP_WHITE);
	m_iml.Add(&bmpWhite, 0xff00ff);

	//��ʼ����Ϸģʽ
	m_pGame = NULL;
}

// �����������ͷ�m_pGameָ��
CTable::~CTable()
{
	// д���������
	CGoBangApp *pApp = (CGoBangApp *)AfxGetApp();
	::WritePrivateProfileString(_T("Options"), _T("Name"), m_strMe, pApp->m_szIni);

	// д��ս��ͳ��
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

// ����������ǰ�ĵȴ���־
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
	// ����R2_NOTXORPEN������ǰ�����ľ���
	if (!m_pGame->m_StepList.empty())
	{
		// ��ȡ���һ����
		step = *(m_pGame->m_StepList.begin());
		pDC->MoveTo(54 + 25 * step.x, 101 + 25 * step.y);
		pDC->LineTo(79 + 25 * step.x, 101 + 25 * step.y);
		pDC->LineTo(79 + 25 * step.x, 126 + 25 * step.y);
		pDC->LineTo(54 + 25 * step.x, 126 + 25 * step.y);
		pDC->LineTo(54 + 25 * step.x, 101 + 25 * step.y);
	}
	// ������������������ݣ����µľ���
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

// �������
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

	//���õȴ���־
	m_bWait = bWait;
	Invalidate();

	//ɾ����Ϸ
	if (m_pGame != NULL)
	{
		delete m_pGame;
		m_pGame = NULL;
	}
}

// ���������ɫ
void CTable::SetColor(int color)
{
	m_color = color;
}
// ��ȡ�����ɫ
int CTable::GetColor() const
{
	return m_color;
}

// ���õȴ���־��������ǰ�ĵȴ���־
bool CTable::SetWait(bool bWait)
{
    m_bOldWait = m_bWait;
    m_bWait = bWait;
    return m_bOldWait;
}

// �����������ݣ�����������
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
// �ж�ָ����ɫ�Ƿ�ʤ��
bool CTable::Win(int color) const
{
	int x, y, z;
	//�жϺ���
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

	//�ж�����
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

	//�ж�б����"\"
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

	//�ж�б����"/"
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

// ������Ϸģʽ�������ս�����ô˺���
void CTable::SetGameMode(int nGameMode)
{
	m_pGame = new COneGame(this);//�����˻���Ϸ����
	m_pGame->Init();//�˻���սģʽ��ʼ��

}
void CTable::Back()
{
	m_pGame->Back();
}

// �����������Ӻ�Ĺ���
void CTable::StepOver()
{
	//�жϼ�����Ƿ�ʤ��
	if (Win(1 - m_color))
	{
		CGoBangApp *pApp = (CGoBangApp *)AfxGetApp();
		pApp->m_nLost++;
		
		CDialog *pDlg = (CDialog *)GetParent();
		PlaySound(MAKEINTRESOURCE(IDR_WAVE_LOST), NULL, SND_RESOURCE | SND_SYNC);
		pDlg->MessageBox(_T("�����ˣ�������Ҫ���ģ�ʧ���˳ɹ�֮ĸŶ��"), _T("ʧ��"), MB_ICONINFORMATION);
		pDlg->GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);
		return;
	}
	m_bWait = false;

}

// �������ӣ������ս�����ô˺���
void CTable::Accept(int nGameMode)
{
	SetColor(0);	//���õ���������ɫ
	Clear(FALSE);	//�������,�ȴ���־m_bWait = FALSE
	SetGameMode(nGameMode);		//������ϷģʽΪ1
}

// �������ԶԷ������ݣ������ս���Դ˺�����չ������������Ϣ
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
			//����1�����ܻ���
			//TODO::
			StepOver();
		}break;
	//�����ս�ڴ˴���������Ϣ
	}

}

// ��Ϣӳ���
BEGIN_MESSAGE_MAP(CTable, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CTable::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages

	//װ������
	CBitmap bmp;
	CPen pen;
	bmp.LoadBitmap(IDB_BMP_TABLE);
					   
	CDC deMem;
	deMem.CreateCompatibleDC(&dc);
					   
	pen.CreatePen(PS_SOLID, 1, 0xff);
	deMem.SelectObject(&bmp);
	deMem.SelectObject(&pen);
	deMem.SetROP2(R2_NOTXORPEN);
					   
	// �����������ݻ�������
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
	// ����������ӵ�ָʾ����
	if (NULL != m_pGame && !m_pGame->m_StepList.empty())
	{
		STEP step = *(m_pGame->m_StepList.begin());
		deMem.MoveTo(54 + 25 * step.x, 101 + 25 * step.y);
		deMem.LineTo(79 + 25 * step.x, 101 + 25 * step.y);
		deMem.LineTo(79 + 25 * step.x, 126 + 25 * step.y);
		deMem.LineTo(54 + 25 * step.x, 126 + 25 * step.y);
		deMem.LineTo(54 + 25 * step.x, 101 + 25 * step.y);
	}
					   
	//��ɻ���
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
	//�����(0, 0) �� (14, 14)��Χ��,�Ҹ�����û������,�����ڴ���,���������沢�˳�����
	if (x < 0 || x > 14 || y < 0 || y > 14 || m_data[x][y] != -1)
	{
		MessageBeep(MB_OK);
		return;
	}
	else
	{
		//���λ�úϷ�,������
		SetData(x, y, m_color);
		stepPut.x = x;
		stepPut.y = y;
		stepPut.color = m_color;
	
		//����1�����ܻ���
		GetParent()->GetDlgItem(IDC_BTN_BACK)->EnableWindow(m_pGame->m_StepList.size() > 1);
	}
		
	//�ж�ʤ�������
	if (Win(m_color))
	{
		CGoBangApp *pApp = (CGoBangApp *)AfxGetApp();
		pApp->m_nWin++;
		m_pGame->Win( stepPut );
	
		CDialog *pDlg = (CDialog *)GetParent();
		PlaySound(MAKEINTRESOURCEW(IDR_WAVE_WIN), NULL, SND_RESOURCE | SND_SYNC);
		pDlg->MessageBox(_T("��ϲ���������ʤ����"), _T("ʤ��"), MB_ICONINFORMATION);
	
		pDlg->GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);
		m_bWait = true;
		return;
	}
	else
	{
		//��ʼ�ȴ�
		m_bWait = true;
		//����������Ϣ
		PlaySound(MAKEINTRESOURCEW(IDR_WAVE_PUT), NULL, SND_RESOURCE | SND_SYNC);
		m_pGame->SendStep(stepPut);
	}
}
