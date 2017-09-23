#ifndef CMD_LIANGFUTOU_HEAD_FILE
#define CMD_LIANGFUTOU_HEAD_FILE
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID							502									//游戏  ID
#define GAME_PLAYER						4									//游戏人数
#define GAME_NAME						TEXT("十三水")						//游戏名字
#define GAME_GENRE						(GAME_GENRE_SCORE|GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型
#define HAND_CARD_COUNT					13									//扑克数目

#define FONT_NUM						5									//建立字体数目

#define VERSION_SERVER					PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//程序版本

//游戏状态
#define GS_WK_FREE				    	GAME_STATUS_FREE					//等待开始
#define GS_WK_START				    	GAME_STATUS_PLAY					//游戏进行
#define GS_WK_BIPAI						GAME_STATUS_PLAY+1					//比牌
#define GS_WK_END						GAME_STATUS_PLAY+2					//结束
#define GS_WK_SENDCARD					GAME_STATUS_PLAY+3					//发牌

//////////////////////////////////////////////////////////////////////////////////
//状态定义

#define GAME_SCENE_PLACE_JETTON				GAME_STATUS_PLAY					//等待开始
#define GAME_SCENE_GAME_END					GAME_STATUS_PLAY+1					//游戏进行

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//服务器命令结构
#define SUB_S_GAME_START				(0x0004)								//游戏开始
#define SUB_S_GAME_COUNT				(0x0008)								//当前局数
#define SUB_S_BANKER					(0x0010)								//抢庄命令
#define SUB_S_SEND_CARD					(0x0020)								//发牌命令
#define SUB_S_SHOW_CARD					(0x0040)								//玩家摊牌
#define SUB_S_GAME_COMPARE              (0x0080)								//游戏比牌
#define SUB_S_GAME_END					(0x0100)								//游戏结束
#define SUB_S_GAME_CONCLUDE				(0x0200)								//游戏解散
#define SUB_S_START_TIME				(0x0400)								//游戏开始倒计时
#define SUB_S_STOP_TIME					(0x4000)								//停止计时器
#define SUB_S_CONTINUE					(0x8000)								//玩家继续
#define TH_SUB_S_GAME_CONCLUDE_OFFLINE			(0x0800)						//游戏解散断线后
//////////////////////////////////////////////////////////////////////////

//游戏开始
struct CMD_S_GameStart 
{
	SCORE								lUserScore;							//玩家积分
	WORD								wSpecialType;						//特殊牌型
	BYTE								cbBossCount;						//王数量
};

//继续游戏
struct CMD_S_Continue
{
	WORD								wUser;								//准备玩家
};


//局数
struct CMD_S_GameCount
{
	BYTE								cbGameCount;						//当前局数
	BYTE								cbAllGameCount;						//总局数
	SCORE								lGameScore[GAME_PLAYER];			//游戏积分
	bool								bPlayer[GAME_PLAYER];				//是否有玩家存在
	int									nToltalWinDaoShu[GAME_PLAYER];		//总共道数
};

//发送扑克
struct CMD_S_SendCard
{
	WORD								wBanker;							//当前庄家
	BYTE                                cbHandCardData[HAND_CARD_COUNT];	//扑克数据
	bool								bPlayer[GAME_PLAYER];				//是否有玩家存在
	WORD								wSpecialType;						//特殊牌型
	DWORD								dwTimeLeave;						//剩余时间
};

//玩家摊牌
struct CMD_S_ShowCard
{
	WORD								wChairID;							//摊牌玩家
};

//玩家比牌
struct CMD_S_Compare
{
	bool								bPlayer[GAME_PLAYER];				//是否存在玩家
	WORD								wSpecialType[GAME_PLAYER];			//特殊牌型
	int									nSpecialCompareResult[GAME_PLAYER];	//特殊牌型比较结果
	BYTE								cbFrontCard[GAME_PLAYER][3];		//玩家前墩
	BYTE								cbMidCard[GAME_PLAYER][5];			//玩家中墩
	BYTE								cbBackCard[GAME_PLAYER][5];			//玩家后墩
	int									nCompareResult[GAME_PLAYER][3];		//每一道比较结果
	bool								bShoot[GAME_PLAYER][GAME_PLAYER];	//打枪
	bool								bDaoshui[GAME_PLAYER];				//倒水
};

struct CMD_S_StartTime
{
	DWORD									cbStartTime;
};

//游戏结束
struct CMD_S_GameEnd
{
	SCORE								lGameScore[GAME_PLAYER];			//游戏积分
	int									nToltalWinDaoShu[GAME_PLAYER];		//总共道数
	WORD								wRunUser;							//全垒打用户
	SCORE								lUserScore[GAME_PLAYER];			//玩家分数
};

//游戏状态
struct CMD_S_Conclude
{
	SCORE								lUserScore[GAME_PLAYER];			//玩家分数
	BYTE								cbShootCount[GAME_PLAYER];			//打枪次数
	BYTE								cbRunCount[GAME_PLAYER];			//全垒打次数
	BYTE								cbSpecialCount[GAME_PLAYER];		//特殊牌次数
	bool								bDismiss;							//解散
	bool								bJixv;
};

//玩家准备
struct CMD_GF_User_Ready
{
	WORD							wChairID;							//玩家座位ID
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_GETBANK					301									//抢庄命令
#define SUB_C_SHOWCARD					302									//玩家摊牌
#define SUB_C_CONTINUE					303									//继续游戏
#define SUB_C_READY_MESSAGE				310									//客户端发送准备消息
#define SUB_C_READY						311									//玩家准备

struct CMD_C_GetBank
{
	bool bGet;
};

struct CMD_C_Ready
{
	bool								bReady;
};

//分段信息
struct CMD_C_ShowCard
{
	BYTE								cbFrontCard[3];						//前墩扑克
	BYTE								cbMidCard[5];						//中墩扑克
	BYTE								cbBackCard[5];						//后墩扑克
	WORD                                wSpecialType;                       //是否是特殊牌
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif