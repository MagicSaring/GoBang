#include "stdafx.h"
#include "Game.h"
#include "Table.h"

CGame::~CGame()
{
}

void CGame::Win(const STEP &stepSend)
{

}

///////////////////////////////////////////////////////////////
// COneGame���ʵ�ֲ���
///////////////////////////////////////////////////////////////

COneGame:: ~COneGame()
{

}

//��Ϸ��ʼ��
void COneGame::Init()
{
	//���ö�������
	m_pTable->GetParent()->SetDlgItemTextW(IDC_ST_ENEMY, _T("�����")); 

	//��ʼ����ʤ�������
	int x, y, z, count = 0;
	for (x = 0; x < MAX_TABLE_LENGTH; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH; ++y)
		{
			for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
			{
				m_Computer[x][y][z] = false;
				m_Player[x][y][z] = false;
			}
		}
	}

	for (x = 0; x < 2; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH; ++y)
		{
			m_Win[x][y] = 0;
		}
	}

	//�жϺ���
	for (x = 0; x < MAX_TABLE_LENGTH - 4; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH; ++y)
		{
			for (z = 0; z < CONNECTION_LENGTH; ++z)
			{
				m_Computer[x + z][y][count] = true;
				m_Player[x + z][y][count] = true;
			}
			count++;
		}
	}

	//�ж�����
	for (x = 0; x < MAX_TABLE_LENGTH; ++x)
	{
		for(y = 0; y < MAX_TABLE_LENGTH - 4; ++y)
		{
			for (z = 0; z < CONNECTION_LENGTH; ++z)
			{
				m_Computer[x][y + z][count] = true;
				m_Player[x][y + z][count] = true;
			}
			count++;
		}
	}

	//�ж�б����"\"
	for (x = 0; x < MAX_TABLE_LENGTH - 4; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH - 4; ++y)
		{
			for (z = 0; z < CONNECTION_LENGTH; ++z)
			{
				m_Computer[x + z][y + z][count] = true;
				m_Player[x + z][y + z][count] = true;
			}
			count++;
		}
	}

	//�ж�б����"/"
	for (x = 4; x < MAX_TABLE_LENGTH; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH - 4; ++y)
		{
			for (z = 0; z < CONNECTION_LENGTH; ++z)
			{
				m_Computer[x - z][y + z][count] = true;
				m_Player[x - z][y + z][count] = true;
			}
			count++;
		}
	}

	//��������
	if (1 == m_pTable->GetColor())
	{
		//����ռ����Ԫ
		m_pTable->SetData(7, 7, 0);
		PlaySound(MAKEINTRESOURCE(IDR_WAVE_PUT), NULL, SND_RESOURCE | SND_SYNC);
		m_bStart = false;

		int z;
		for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
		{
			//������ǰ����,������֮��
			m_nOldWin[0][z] = m_Win[0][z];
			m_nOldWin[1][z] = m_Win[1][z];
			m_bOldPlayer[z] = m_Player[7][7][z];
		}

		for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
		{
			// �޸ļ�������Ӻ����̵ı仯״��
			if (m_Computer[7][7][z] && m_Win[1][z] != -1)
			{
				m_Win[1][z] ++;
			}

			if (m_Player[7][7][z])
			{
				m_Player[7][7][z] = false;
				m_Win[0][z] = -1;
			}
		}
	}
	else
	{
		m_bStart = true;
	}



}
void COneGame::SendStep(const STEP &stepSend)
{
	int bestx, besty, x, y, z, px, py, ptemp, ctemp, pscore = 10, cscore = -10000;
	int ctempTable[MAX_TABLE_LENGTH][MAX_TABLE_LENGTH], ptempTable[MAX_TABLE_LENGTH][MAX_TABLE_LENGTH];
	int m, n, temp1[20], temp2[20]; // �ݴ��һ����������Ϣ

	AfxGetMainWnd()->GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);

	//������ǰ����,������ֻ��
	for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
	{
		m_nOldWin[0][z] = m_Win[0][z];
		m_nOldWin[1][z] = m_Win[1][z];
		m_bOldPlayer[z] = m_Player[stepSend.x][stepSend.y][z];
		m_bOldComputer[z] = m_Computer[stepSend.x][stepSend.y][z];
	}

	// �޸�������Ӻ�����״̬�ı仯
	for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
	{
		if (m_Player[stepSend.x][stepSend.y][z] && m_Win[0][z] != -1)
		{
			m_Win[0][z] ++;
		}

		if (m_Computer[stepSend.x][stepSend.y][z])
		{
			m_Player[stepSend.x][stepSend.y][z] = false;
			m_Win[1][z] = -1;
		}
	}

	//ȷ�������߷�
	if (m_bStart)
	{
		// �ֶ�ȷ����һ������Ԫ��(8, 8)
		if (-1 == m_pTable->m_data[7][7])
		{
			bestx = 7;
			besty = 7;
		}
		else
		{
			bestx = 8;
			besty = 8;
		}

		m_bStart = false;
	}
	else
	{
		STEP step;
		//Ѱ�����λ��
		GetTable(ctempTable, m_pTable->m_data);
		
		while (SearchBlank(x, y, ctempTable))
		{
			n = 0;
            pscore = 10;
			GetTable(ptempTable, m_pTable->m_data);

			//����ѱ�����
			ctempTable[x][y] = 2;
			step.color = 1 - m_pTable->GetColor();
			step.x = x;
			step.y = y;

			//�������λ���
			ctemp = GiveScore(step);
			for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
			{
				//��ʱ���������Ϣ
				if (m_Player[x][y][z])
				{
					temp1[n] = z;
					m_Player[x][y][z] = false;
					temp2[n] = m_Win[0][z];
					m_Win[0][z] = -1;
					n++;
				}
			}

			ptempTable[x][y] = 0;

			px = x;
			py = y;

			while (SearchBlank(x, y, ptempTable))
			{
				ptempTable[x][y] = 2;	//����ѱ�����

				step.color = m_pTable->GetColor();
				step.x = x;
				step.y = y;

				ptemp = GiveScore(step);
				if (pscore > ptemp)		// ��ʱΪ������ӣ����ü�С����ʱӦѡȡ��Сֵ
				{
					pscore = ptemp;
				}
			}

			for (m = 0; m < n; ++m)
			{
				// �ָ������Ϣ
				m_Player[px][py][temp1[m]] = true;
				m_Win[0][temp1[m]] = temp2[m];
			}
			if (ctemp + pscore > cscore)	// ��ʱΪ��������ӣ����ü�С����ʱӦѡȡ�����ֵ
			{
				cscore = ctemp + pscore;
				bestx = px;
				besty = py;
			}
		}
	}
	m_step.color = 1 - m_pTable->GetColor();
	m_step.x = bestx;
	m_step.y = besty;

	for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
	{
		// �޸ļ�������Ӻ����̵ı仯״��
		if (m_Computer[bestx][besty][z] && m_Win[1][z] != -1)
		{
			m_Win[1][z] ++;
		}
		if (m_Player[bestx][besty][z])
		{
			m_Player[bestx][besty][z] = false;
			m_Win[0][z] = -1;
		} 
		
	}
	
	m_pTable->GetParent()->GetDlgItem(IDC_BTN_BACK)->EnableWindow();

	//�����ǵ�����Ϸ,����ֱ�ӽ�������
	m_pTable->Receive();
}
void COneGame::ReceiveMsg(MSGSTRUCT *pMsg)
{
	pMsg->x = m_step.x;
	pMsg->y = m_step.y;
	pMsg->color = m_step.color;
	pMsg->uMsg = MSG_DROPDOWN;

}
void COneGame::Back()
{
	STEP step;

	//�ڵ�һ��(��������)
	step = *(m_StepList.begin());
	m_StepList.pop_front();
	m_pTable->m_data[step.x][step.y] = -1;

	int z;
	//�ָ�ԭ��ʤ������
	for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
	{
		m_Win[0][z] = m_nOldWin[0][z];
		m_Win[1][z] = m_nOldWin[1][z];
		m_Player[step.x][step.y][z] = m_bOldPlayer[z];
	}

	//�ڵڶ��� (�������)
	step = *(m_StepList.begin());
	m_StepList.pop_front();
	m_pTable->m_data[step.x][step.y] = -1;

	//�ָ�ԭ��ʤ������
	for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
	{
		m_Computer[step.x][step.y][z] = m_bOldComputer[z];
	}

	m_pTable->Invalidate();

	//ֻ׼��һ����
	AfxGetMainWnd()->GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);

}

int COneGame::GiveScore(const STEP &setPut)
{
	int z, nScore = 0;
	for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
	{
		if (m_pTable->GetColor() == setPut.color)
		{
			//�����
			if (m_Player[setPut.x][setPut.y][z])
			{
				switch (m_Win[0][z])
				{
				case 1:
					nScore -= 5;
					break;
				case 2:
					nScore -= 50;
					break;
				case 3:
					nScore -= 500;
					break;
				case 4:
					nScore -= 5000;
					break;
				default:
					break;
				}
			}
		}
		else
		{
			//�������
			if (m_Computer[setPut.x][setPut.y][z])
			{
				switch (m_Win[1][z])
				{
				case 1:
					nScore += 5;
					break;
				case 2:
					nScore += 50;
					break;
				case 3:
					nScore += 100;
					break;
				case 4:
					nScore += 10000;
					break;
				default:
					break;
				}

			}
		}
	}

	return nScore;
}
void COneGame::GetTable(int tempTable[][MAX_TABLE_LENGTH], int nowTable[][MAX_TABLE_LENGTH])
{
	int x, y;
	for (x = 0; x < MAX_TABLE_LENGTH; ++x)
	{
		for (y = 0; y < MAX_TABLE_LENGTH; ++y)
		{
			tempTable[x][y] = nowTable[x][y];
		}
	}

}
bool COneGame::SearchBlank(int &x, int &y, const int nowTable[][MAX_TABLE_LENGTH])
{
	for (int i = 0; i < MAX_TABLE_LENGTH; ++i)
	{
		for (int j = 0; j < MAX_TABLE_LENGTH; ++j)
		{
			if (nowTable[i][j] == -1)
			{
				x = i;
				y = j;
				return true;
			}
		}
	}

	return false;
}