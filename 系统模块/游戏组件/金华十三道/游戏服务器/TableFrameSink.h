#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once
#pragma pack(1)
#include "Stdafx.h"
#include "DlgCustomRule.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

#define MAX_ROBOTCOUNT		1000

//////////////////////////////////////////////////////////////////////////
#define IDI_AUTO_SHOW_CARD		5				//自动摊牌功能
#define IDI_SHOW_SCORE			6				//结算弹出时间
//#define IDI_WRITE_SCORE			10				//比牌消息ID
#define	IDI_NEW_START_GAME		11
#define	IDI_NEW_TIP_GAME		12		

// #define IDT_WRITE_SCORE1		15				//比牌时间
// #define IDT_WRITE_SCORE2		19				//全部特殊牌或者倒水时间

#define CHEAT_TYPE_LOST			0				//输
#define CHEAT_TYPE_WIN			1				//赢
#define	TIME_START_GAME			17000			//定时器
 enum enShisanshuiOption
 {
 	enOption_sss_BossCount = enOption_custom,
 	enOption_sss_Five,
 	enOption_sss_BankerType,
 	enOption_sss_SpecialType,
 	enOption_sss_Bidao,
 	enOption_sss_BiHua,
	enOption_sss_GunType,
	enOption_sss_MaPaiType,
	enOption_HongBoLangType,
 };

enum enBankerType
{
	enBankerType_Null = 0,
	enBankerType_Lunliu,
	enBankerType_Suiji,
	enBankerType_QiangZhuang,
};

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//组件变量
protected:
	CGameLogic							m_GameLogic;										//游戏逻辑
	ITableFrame							* m_pITableFrame;									//框架接口
	const tagGameServiceOption			* m_pGameServiceOption;								//配置参数
	tagCustomRule *						m_pGameCustomRule;									//自定规则
	tagFangkaGameOption *				m_pFangkaOption;									//房卡配置 

	//属性变量
protected:
	bool								m_bIsEnd;											//游戏结束过
	DWORD								m_dwStartTime;										//开始时间
	bool								m_bStartTimer;
	BYTE								m_cbReadyCount;
	WORD								m_wCurPlayerCount;									//当前玩家数
	BYTE								m_cbGameCount;										//游戏局数
	BYTE								m_cbAllCount;										//总局数
	BYTE								m_cbBossCount;										//王场
	bool								m_bFive;											//五人场
	BYTE								m_cbBankerModel;									//庄家模式
	WORD								m_wBanker;											//庄家
	WORD								m_wAllSpecialType;									//特殊牌型
	bool								m_bBidao;											//比道
	bool								m_bBihua;											//比花色

	WORD								m_wMaiPaiType;										//特殊牌型
	WORD								m_wHongbolang;										//红波浪
	WORD								m_wSpec;
	WORD								m_wFive;
	//游戏变量
	bool								m_bShowCard[GAME_PLAYER];							//摊牌
	bool								m_bPlayer[GAME_PLAYER];								//玩家
	bool								m_bContinue[GAME_PLAYER];							//继续游戏
	bool								m_bResponse[GAME_PLAYER];							//响应

	BYTE								m_cbRun;
	BYTE								m_cbGun;

	WORD								m_wRunUser;
	SCORE								m_lGameScore[GAME_PLAYER];							//游戏积分
	int									m_nToltalWinDaoShu[GAME_PLAYER];					//总共道数
	int									m_nRewardDaoShu[GAME_PLAYER];					//总共道数

	SCORE								m_lUserScore[GAME_PLAYER];							//玩家分数
	BYTE								m_cbShootCount[GAME_PLAYER];						//打枪次数
	BYTE								m_cbRunCount[GAME_PLAYER];							//全垒打次数
	BYTE								m_cbSpecialCount[GAME_PLAYER];						//特殊牌次数

	SCORE								m_lUserScoreBak[GAME_PLAYER];							//玩家分数
	BYTE								m_cbShootCountBak[GAME_PLAYER];						//打枪次数
	BYTE								m_cbRunCountBak[GAME_PLAYER];							//全垒打次数
	BYTE								m_cbSpecialCountBak[GAME_PLAYER];						//特殊牌次数


	BYTE								m_cbFrontCard[GAME_PLAYER][3];						//前墩扑克
	BYTE								m_cbMidCard[GAME_PLAYER][5];						//中墩扑克
	BYTE								m_cbBackCard[GAME_PLAYER][5];						//后墩扑克
	WORD                                m_wSpecialType[GAME_PLAYER];						//是否是特殊牌

	BYTE								m_cbHandCard[GAME_PLAYER][HAND_CARD_COUNT];			//玩家手牌
	BYTE								m_cbSaveHandCard[GAME_PLAYER][HAND_CARD_COUNT];			//玩家手牌
	WORD								m_wHandCardType[GAME_PLAYER][3];					//玩家手牌类型
	int									m_nSpecialCompareResult[GAME_PLAYER];				//特殊牌型比较结果
	int									m_nCompareResult[GAME_PLAYER][3];					//每一道比较结果
	bool								m_bShoot[GAME_PLAYER][GAME_PLAYER];					//打枪
	bool								m_bDaoshui[GAME_PLAYER];							//倒水
	//服务器自动配牌需要
	BYTE								m_cbAutoCard[13];							//需要自动配牌的玩家手牌
	WORD								m_wAutoCardType;							//需要自动配牌的所有手牌类型
	BYTE								m_cbAutoFrontCard[3];						//需要自动配牌的前墩扑克
	BYTE								m_cbAutoMidCard[5];							//需要自动配牌的中墩扑克
	BYTE								m_cbAutoBackCard[5];						//需要自动配牌的后墩扑克
	AnalyseResult*						m_pHead;
	AnalyseResult*						m_pCurrent;

	void DeleteData();
	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//是否含有马牌
	bool HasMapai(WORD wChairID, WORD wMaiPaiType);
	void ResetbPlayer(){memset(m_bPlayer, 0, sizeof(m_bPlayer));};
	BYTE GetBaseFenByShoot(BYTE cbShootCnt);
	BYTE GetRewardByHandType(BYTE iChair);
	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//是否有效
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//查询服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}
	//设置基数
	virtual void SetGameBaseScore(SCORE lBaseScore){}
	virtual void SetGameBaseScore(LONG lBaseScore){}

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

protected:
	//获取捕鱼库存
	virtual SCORE UpdateFishStock()  {return 0;}

	//////////////////////////////////////////////////////////////////////////

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//用户发送喇叭消息
	virtual bool OnSendTrumpetChat(VOID * pData, WORD wDataSize) { return true; }
	//用户下注
	virtual bool OnActionUserPour(WORD wChairID, IServerUserItem * pIServerUserItem){return true;}

//过程函数
private:
	//发送局数
	void SendGameCount(WORD wChairID= INVALID_CHAIR);
	//发牌
	void SendCard();
	//玩家摊牌
	void ShowCard(WORD wChairID);
	//玩家比牌
	void GameCompare(WORD wChairID= INVALID_CHAIR,bool bShow=false);
	//游戏结束
	void GameEnd(WORD wChairID= INVALID_CHAIR,bool bShow=false);

	//打完全局后断线重连
	void OfflineGameConclude(WORD wChairID);
	
//消息处理
private:
	//玩家抢庄
	bool OnUserGetBanker(WORD wChairID,bool bGet);
	//玩家摊牌
	bool OnUserShowCard(WORD wChairID, CMD_C_ShowCard *pShowCard);
	//继续游戏
	bool OnUserContinue(WORD wChairID);

//功能函数
private:
	//重置游戏数据
	void ResetGame();
	//玩家比道
	void GetCompareResult();
	//玩家比道test
	int GetCompareResultTest();

	//特殊牌水数
	BYTE GetSpecialShui(WORD wType);
	//普通牌水数
	BYTE GetShui(WORD wType);
	//自动摆牌
	void AutoShowCard(WORD wChairID,bool testOnly = false);
	//读取配置
	bool readFile(BYTE *pCardData, BYTE cbCardCnt, WORD &zhuang);

	int FindMaxChair(BYTE * cbCardBuffer);
};

//////////////////////////////////////////////////////////////////////////

#endif