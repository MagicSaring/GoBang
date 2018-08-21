#pragma once

#define  MAX_TABLE_LENGTH 15
#define  CONNECTION_LENGTH 5
#define  WIN_COMBINATION_LENGTH	572

typedef struct tagStep			//定义一手棋的数据结构
{
	int x;
	int y;
	int color;
}STEP;

typedef struct tagMsgStruct
{
	//消息ID
	UINT uMsg;

	//落子信息
	int x;
	int y;
	int color;

	//消息内容
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

//落子消息
#define MSG_DROPDOWN			0x00000001