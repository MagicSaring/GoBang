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
// COneGame类的实现部分
///////////////////////////////////////////////////////////////

COneGame:: ~COneGame()
{

}

//游戏初始化
void COneGame::Init()
{
	//设置对手姓名
	m_pTable->GetParent()->SetDlgItemTextW(IDC_ST_ENEMY, _T("计算机")); 

	//初始化获胜组合数组
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

	//判断横向
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

	//判断纵向
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

	//判断斜方向"\"
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

	//判断斜方向"/"
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

	//电脑先走
	if (1 == m_pTable->GetColor())
	{
		//电脑占据天元
		m_pTable->SetData(7, 7, 0);
		PlaySound(MAKEINTRESOURCE(IDR_WAVE_PUT), NULL, SND_RESOURCE | SND_SYNC);
		m_bStart = false;

		int z;
		for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
		{
			//保存先前数据,做悔棋之用
			m_nOldWin[0][z] = m_Win[0][z];
			m_nOldWin[1][z] = m_Win[1][z];
			m_bOldPlayer[z] = m_Player[7][7][z];
		}

		for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
		{
			// 修改计算机下子后，棋盘的变化状况
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
	int m, n, temp1[20], temp2[20]; // 暂存第一步搜索的信息

	AfxGetMainWnd()->GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);

	//保存先前数据,做悔棋只用
	for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
	{
		m_nOldWin[0][z] = m_Win[0][z];
		m_nOldWin[1][z] = m_Win[1][z];
		m_bOldPlayer[z] = m_Player[stepSend.x][stepSend.y][z];
		m_bOldComputer[z] = m_Computer[stepSend.x][stepSend.y][z];
	}

	// 修改玩家下子后棋盘状态的变化
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

	//确定电脑走法
	if (m_bStart)
	{
		// 手动确定第一步：天元或(8, 8)
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
		//寻找最佳位置
		GetTable(ctempTable, m_pTable->m_data);
		
		while (SearchBlank(x, y, ctempTable))
		{
			n = 0;
            pscore = 10;
			GetTable(ptempTable, m_pTable->m_data);

			//标记已被查找
			ctempTable[x][y] = 2;
			step.color = 1 - m_pTable->GetColor();
			step.x = x;
			step.y = y;

			//给这个空位打分
			ctemp = GiveScore(step);
			for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
			{
				//暂时更改玩家信息
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
				ptempTable[x][y] = 2;	//标记已被查找

				step.color = m_pTable->GetColor();
				step.x = x;
				step.y = y;

				ptemp = GiveScore(step);
				if (pscore > ptemp)		// 此时为玩家下子，运用极小极大法时应选取最小值
				{
					pscore = ptemp;
				}
			}

			for (m = 0; m < n; ++m)
			{
				// 恢复玩家信息
				m_Player[px][py][temp1[m]] = true;
				m_Win[0][temp1[m]] = temp2[m];
			}
			if (ctemp + pscore > cscore)	// 此时为计算机下子，运用极小极大法时应选取最最大值
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
		// 修改计算机下子后，棋盘的变化状况
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

	//由于是单人游戏,所以直接接收数据
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

	//悔第一步(电脑落子)
	step = *(m_StepList.begin());
	m_StepList.pop_front();
	m_pTable->m_data[step.x][step.y] = -1;

	int z;
	//恢复原有胜负布局
	for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
	{
		m_Win[0][z] = m_nOldWin[0][z];
		m_Win[1][z] = m_nOldWin[1][z];
		m_Player[step.x][step.y][z] = m_bOldPlayer[z];
	}

	//悔第二步 (玩家落子)
	step = *(m_StepList.begin());
	m_StepList.pop_front();
	m_pTable->m_data[step.x][step.y] = -1;

	//恢复原有胜负布局
	for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
	{
		m_Computer[step.x][step.y][z] = m_bOldComputer[z];
	}

	m_pTable->Invalidate();

	//只准悔一步棋
	AfxGetMainWnd()->GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);

}

int COneGame::GiveScore(const STEP &setPut)
{
	int z, nScore = 0;
	for (z = 0; z < WIN_COMBINATION_LENGTH; ++z)
	{
		if (m_pTable->GetColor() == setPut.color)
		{
			//玩家下
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
			//计算机下
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