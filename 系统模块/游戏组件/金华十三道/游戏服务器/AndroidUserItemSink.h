#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE
#pragma once

#include "Stdafx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////
//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//扑克变量
protected:
    BYTE                            m_bHandCardData[HAND_CARD_COUNT];   //机器人手中的牌
	BYTE                            m_bHandCardCount;                   //牌的数量
	BYTE                            m_bSegmentCard[3][5];               //分段扑克
	BYTE                            m_bSpecialCardData[HAND_CARD_COUNT];//特殊牌的数据
	int                             m_nRobotPlayCount;                  //机器人玩的局数
	int                             m_nRobotPlayMaxCount;                //机器人最多玩的局数
	bool							m_bPlayer[GAME_PLAYER];				//是否有人
	BYTE							m_bAllUserCardData[GAME_PLAYER][HAND_CARD_COUNT];
	bool							m_bWait;				//是否有人
	TCHAR							m_szRoomName[32];					//配置房间

	int                             m_nAndroidNum;
	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;				//用户接口
	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();
private:
	//银行操作
	void BankOperate(BYTE cbType);
	//基础接口
public:
	//释放对象
	virtual VOID Release() { }
	//是否有效
	virtual bool IsValid() { return AfxIsValidAddress(this,sizeof(CAndroidUserItemSink))?true:false; }
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool RepositionSink();

	//游戏事件
public:
	//时间消息
	virtual bool OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//游戏消息
	virtual bool OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//用户事件
public:
	//用户进入
	virtual void  OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual void  OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual void  OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual void  OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户段位
	virtual void  OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

protected:
	//设置分段
	//bool OnSubSetSegment(const void * pBuffer , WORD wDataSize);
	//玩家摊牌
	//bool OnSubShowCard(const void * pBuffer , WORD wDataSize) ;
	//游戏开始
	bool OnSubSendCard(void * pData, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(void * pData, WORD wDataSize);
	void GetSpecialCard(BYTE CardData[],BYTE CardCount);
	void ShowCard();
	void GetAllUserDaoShu(BYTE btCard[],LONG lUserDaoShu[],WORD wChaiID,bool bSpecial);
};

//////////////////////////////////////////////////////////////////////////

#endif