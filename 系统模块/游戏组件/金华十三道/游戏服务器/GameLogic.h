#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "Stdafx.h"
#pragma pack(1)

//�������Ͷ���
#define TYPE_NULL						0x0000							//
#define TYPE_SANTONGHUA					0x0001							//��ͬ��
#define TYPE_SANSHUNZI					0x0002							//��˳��
#define TYPE_LIUDUIBAN					0x0004							//���԰�
#define TYPE_WUDUISANTIAO				0x0008							//�������
#define TYPE_SITAOSANTIAO				0x0010							//��������
#define TYPE_COUYISE					0x0020							//��һɫ
#define TYPE_QUANXIAO					0x0040							//ȫС
#define TYPE_QUANDA						0x0080							//ȫ��
#define TYPE_SANZHADAN					0x0100							//��ը��
#define TYPE_SANTONGHUASHUN				0x0200							//��ͬ��˳
#define TYPE_SHIERHUANGZU				0x0400							//ʮ������
#define TYPE_YITIAOLONG					0x0800							//һ����	
#define TYPE_ZHIZUNQINGLONG				0x1000							//��������

//��ͨ���Ͷ���
#define HT_INVALID						0x0000							//��������
#define HT_SINGLE						0x0001							//��������
#define HT_ONE_DOUBLE					0x0002							//ֻ��һ��
#define HT_TWO_DOUBLE					0x0004							//��������
#define HT_THREE						0x0008							//��������
#define HT_LINE							0x0010							//˳��
#define HT_COLOR						0x0020							//ͬ��
#define HT_THREE_DEOUBLE 				0x0040							//��«
#define HT_FOUR_BOOM					0x0080							//��֦
#define HT_LINE_COLOR					0x0100							//ͬ��˳
#define HT_FIVE_BOOM					0x0200							//��ը
#define HT_SIX_BOOM						0x0400							//��ը

#pragma once
//////////////////////////////////////////////////////////////////////////
//�Զ����Ʒ�����
struct AnalyseResult
{
	WORD wType;
	BYTE cbCard[5];
	int nSortValue;
	AnalyseResult *pNext;
};




//��Ϸ�߼���
class CGameLogic
{
public:
	//��������
protected:
	static const BYTE				m_cbCardListData[52];				//�˿�����
	static const BYTE				m_cbCardListDataFive[65];			//�˿�����

protected:
    
	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();
	//���ͺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBossCount,bool bFive);
	//�������
	void AddBoss(BYTE cbBossCount, BYTE cbCardData[], BYTE cbCount);
	//ɾ����֤
	bool DeleteCard(BYTE cbCard[], BYTE cbCount, BYTE cbDeleteCard[], BYTE cbDeleteCount);
	//������
	void SortCardByValue(BYTE cbCard[], BYTE cbCount);
	//��������
	void SortCardByType(BYTE cbCard[], BYTE cbCount);
	//��������
	void SortCardByNum(BYTE cbCard[], BYTE cbCount, BYTE cbNum);
	void SortBySpecial(BYTE cbCard[], BYTE cbCount, WORD wType);
	void SortByColor(BYTE cbCard[], BYTE cbCount);
	//��ȡ��С
	BYTE GetCardValue(BYTE cbCard);
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCard);
	//��ȡ�����С
	BYTE GetSortValue(BYTE cbCard);
	//��ȡ���һ��ֵͬ�������
	BYTE GetLastIndex(BYTE cbCard[], BYTE cbCount, BYTE cbTemp);
	//��ȡ����
	WORD GetCardType(BYTE cbCard[], BYTE cbCount, BYTE cbBossCount = 0,bool bFive=false);
	//��ȡ��������
	WORD GetSpecialType(BYTE cbCard[], BYTE cbCount, WORD wAllType);
	//��ȡ��ɫ��
	BYTE GetColorCard(BYTE cbCard[], BYTE cbolor, BYTE cbGetOutCard[]);
	//˳��
	bool IsAllLine(BYTE cbCard[], BYTE cbCount, bool bSameColor = false);
	//˳��
	bool IsLine(BYTE cbCard[], BYTE cbCount, bool bSameColor = false);
	//ͬ��
	bool IsSameColor(BYTE cbCard[], BYTE cbCount);
	//��ȡ��������
	WORD GetOtherType(BYTE cbCard[], BYTE cbCount, BYTE cbBossCount = 0, bool bFive = false);
	//��ȡ��
	BYTE GetCardWithValue(BYTE cbCard[],BYTE cbCount,BYTE cbValue);
	//����
	bool IsBigAwithB(BYTE cbCardA[], BYTE cbCountA, BYTE cbCardB[], BYTE cbCountB,bool bColor = false);
	//����
	bool IsBigLineAwithB(BYTE cbCardA[], BYTE cbCountA, BYTE cbCardB[], BYTE cbCountB, bool bColor = false);
	//�����Ʊȴ�С
	bool IsBigSpecialAwithB(WORD wTypeA, WORD wTypeB);
	//������
	BYTE GetBossCount(BYTE cbCard[], BYTE cbCount);
//�������Զ�������Ҫ
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