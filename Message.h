#pragma once

#define  MAX_TABLE_LENGTH 15
#define  CONNECTION_LENGTH 5
#define  WIN_COMBINATION_LENGTH	572

typedef struct tagStep			//����һ��������ݽṹ
{
	int x;
	int y;
	int color;
}STEP;

typedef struct tagMsgStruct
{
	//��ϢID
	UINT uMsg;

	//������Ϣ
	int x;
	int y;
	int color;

	//��Ϣ����
	TCHAR szMsg[128];

	struct tagMsgStruct()
	{
		uMsg = NULL;
		x = -1;
		y = -1;
		color = -1;
		memset(szMsg, 0, sizeof(szMsg));
	};

}MSGSTRUCT;

//������Ϣ
#define MSG_DROPDOWN			0x00000001