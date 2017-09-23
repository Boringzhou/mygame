#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "Stdafx.h"
#pragma pack(1)

//特殊牌型定义
#define TYPE_NULL						0x0000							//
#define TYPE_SANTONGHUA					0x0001							//三同花
#define TYPE_SANSHUNZI					0x0002							//三顺子
#define TYPE_LIUDUIBAN					0x0004							//六对半
#define TYPE_WUDUISANTIAO				0x0008							//五对三条
#define TYPE_SITAOSANTIAO				0x0010							//四套三条
#define TYPE_COUYISE					0x0020							//凑一色
#define TYPE_QUANXIAO					0x0040							//全小
#define TYPE_QUANDA						0x0080							//全大
#define TYPE_SANZHADAN					0x0100							//三炸弹
#define TYPE_SANTONGHUASHUN				0x0200							//三同花顺
#define TYPE_SHIERHUANGZU				0x0400							//十二皇族
#define TYPE_YITIAOLONG					0x0800							//一条龙	
#define TYPE_ZHIZUNQINGLONG				0x1000							//至尊清龙

//普通牌型定义
#define HT_INVALID						0x0000							//错误类型
#define HT_SINGLE						0x0001							//单牌类型
#define HT_ONE_DOUBLE					0x0002							//只有一对
#define HT_TWO_DOUBLE					0x0004							//两对牌型
#define HT_THREE						0x0008							//三张牌型
#define HT_LINE							0x0010							//顺子
#define HT_COLOR						0x0020							//同花
#define HT_THREE_DEOUBLE 				0x0040							//葫芦
#define HT_FOUR_BOOM					0x0080							//铁枝
#define HT_LINE_COLOR					0x0100							//同花顺
#define HT_FIVE_BOOM					0x0200							//五炸
#define HT_SIX_BOOM						0x0400							//六炸

#pragma once
//////////////////////////////////////////////////////////////////////////
//自动配牌分析牌
struct AnalyseResult
{
	WORD wType;
	BYTE cbCard[5];
	int nSortValue;
	AnalyseResult *pNext;
};




//游戏逻辑类
class CGameLogic
{
public:
	//变量定义
protected:
	static const BYTE				m_cbCardListData[52];				//扑克数据
	static const BYTE				m_cbCardListDataFive[65];			//扑克数据

protected:
    
	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();
	//类型函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBossCount,bool bFive);
	//添加王牌
	void AddBoss(BYTE cbBossCount, BYTE cbCardData[], BYTE cbCount);
	//删牌验证
	bool DeleteCard(BYTE cbCard[], BYTE cbCount, BYTE cbDeleteCard[], BYTE cbDeleteCount);
	//排序函数
	void SortCardByValue(BYTE cbCard[], BYTE cbCount);
	//类型排序
	void SortCardByType(BYTE cbCard[], BYTE cbCount);
	//数量排序
	void SortCardByNum(BYTE cbCard[], BYTE cbCount, BYTE cbNum);
	void SortBySpecial(BYTE cbCard[], BYTE cbCount, WORD wType);
	void SortByColor(BYTE cbCard[], BYTE cbCount);
	//获取大小
	BYTE GetCardValue(BYTE cbCard);
	//获取花色
	BYTE GetCardColor(BYTE cbCard);
	//获取排序大小
	BYTE GetSortValue(BYTE cbCard);
	//获取最后一张同值的牌序号
	BYTE GetLastIndex(BYTE cbCard[], BYTE cbCount, BYTE cbTemp);
	//获取牌型
	WORD GetCardType(BYTE cbCard[], BYTE cbCount, BYTE cbBossCount = 0,bool bFive=false);
	//获取特殊牌型
	WORD GetSpecialType(BYTE cbCard[], BYTE cbCount, WORD wAllType);
	//获取单色牌
	BYTE GetColorCard(BYTE cbCard[], BYTE cbolor, BYTE cbGetOutCard[]);
	//顺子
	bool IsAllLine(BYTE cbCard[], BYTE cbCount, bool bSameColor = false);
	//顺子
	bool IsLine(BYTE cbCard[], BYTE cbCount, bool bSameColor = false);
	//同花
	bool IsSameColor(BYTE cbCard[], BYTE cbCount);
	//获取其他类型
	WORD GetOtherType(BYTE cbCard[], BYTE cbCount, BYTE cbBossCount = 0, bool bFive = false);
	//获取牌
	BYTE GetCardWithValue(BYTE cbCard[],BYTE cbCount,BYTE cbValue);
	//比牌
	bool IsBigAwithB(BYTE cbCardA[], BYTE cbCountA, BYTE cbCardB[], BYTE cbCountB,bool bColor = false);
	//比牌
	bool IsBigLineAwithB(BYTE cbCardA[], BYTE cbCountA, BYTE cbCardB[], BYTE cbCountB, bool bColor = false);
	//特殊牌比大小
	bool IsBigSpecialAwithB(WORD wTypeA, WORD wTypeB);
	//王数量
	BYTE GetBossCount(BYTE cbCard[], BYTE cbCount);
//服务器自动配牌需要
public:

	WORD GetCardType(AnalyseResult* pHead, BYTE cbCard[], BYTE cbCount, bool bHaveBoss = false);
	void PushResult(AnalyseResult* pHead, AnalyseResult*pResult);
	void PushResult(AnalyseResult* pHead, BYTE cbCard[], WORD wType, int nSortValue=0);
	void PushResult(AnalyseResult* pHead, BYTE cbCard1, BYTE cbCard2, BYTE cbCard3,BYTE cbCard4, BYTE cbCard5, WORD wType);
	WORD SaveOtherType(AnalyseResult* pHead, BYTE cbCard[], BYTE cbCount, bool bHaveBoss);
	AnalyseResult* GetNextData(AnalyseResult* pHead, AnalyseResult* pCurrent, WORD wType);
	int	 IsSameColorLine(BYTE cbCard[], BYTE cbCount);
};

//////////////////////////////////////////////////////////////////////////

#endif