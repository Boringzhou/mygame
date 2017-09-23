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
#define IDI_AUTO_SHOW_CARD		5				//�Զ�̯�ƹ���
#define IDI_SHOW_SCORE			6				//���㵯��ʱ��
//#define IDI_WRITE_SCORE			10				//������ϢID
#define	IDI_NEW_START_GAME		11
#define	IDI_NEW_TIP_GAME		12		

// #define IDT_WRITE_SCORE1		15				//����ʱ��
// #define IDT_WRITE_SCORE2		19				//ȫ�������ƻ��ߵ�ˮʱ��

#define CHEAT_TYPE_LOST			0				//��
#define CHEAT_TYPE_WIN			1				//Ӯ
#define	TIME_START_GAME			17000			//��ʱ��
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

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�������
protected:
	CGameLogic							m_GameLogic;										//��Ϸ�߼�
	ITableFrame							* m_pITableFrame;									//��ܽӿ�
	const tagGameServiceOption			* m_pGameServiceOption;								//���ò���
	tagCustomRule *						m_pGameCustomRule;									//�Զ�����
	tagFangkaGameOption *				m_pFangkaOption;									//�������� 

	//���Ա���
protected:
	bool								m_bIsEnd;											//��Ϸ������
	DWORD								m_dwStartTime;										//��ʼʱ��
	bool								m_bStartTimer;
	BYTE								m_cbReadyCount;
	WORD								m_wCurPlayerCount;									//��ǰ�����
	BYTE								m_cbGameCount;										//��Ϸ����
	BYTE								m_cbAllCount;										//�ܾ���
	BYTE								m_cbBossCount;										//����
	bool								m_bFive;											//���˳�
	BYTE								m_cbBankerModel;									//ׯ��ģʽ
	WORD								m_wBanker;											//ׯ��
	WORD								m_wAllSpecialType;									//��������
	bool								m_bBidao;											//�ȵ�
	bool								m_bBihua;											//�Ȼ�ɫ

	WORD								m_wMaiPaiType;										//��������
	WORD								m_wHongbolang;										//�첨��
	WORD								m_wSpec;
	WORD								m_wFive;
	//��Ϸ����
	bool								m_bShowCard[GAME_PLAYER];							//̯��
	bool								m_bPlayer[GAME_PLAYER];								//���
	bool								m_bContinue[GAME_PLAYER];							//������Ϸ
	bool								m_bResponse[GAME_PLAYER];							//��Ӧ

	BYTE								m_cbRun;
	BYTE								m_cbGun;

	WORD								m_wRunUser;
	SCORE								m_lGameScore[GAME_PLAYER];							//��Ϸ����
	int									m_nToltalWinDaoShu[GAME_PLAYER];					//�ܹ�����
	int									m_nRewardDaoShu[GAME_PLAYER];					//�ܹ�����

	SCORE								m_lUserScore[GAME_PLAYER];							//��ҷ���
	BYTE								m_cbShootCount[GAME_PLAYER];						//��ǹ����
	BYTE								m_cbRunCount[GAME_PLAYER];							//ȫ�ݴ����
	BYTE								m_cbSpecialCount[GAME_PLAYER];						//�����ƴ���

	SCORE								m_lUserScoreBak[GAME_PLAYER];							//��ҷ���
	BYTE								m_cbShootCountBak[GAME_PLAYER];						//��ǹ����
	BYTE								m_cbRunCountBak[GAME_PLAYER];							//ȫ�ݴ����
	BYTE								m_cbSpecialCountBak[GAME_PLAYER];						//�����ƴ���


	BYTE								m_cbFrontCard[GAME_PLAYER][3];						//ǰ���˿�
	BYTE								m_cbMidCard[GAME_PLAYER][5];						//�ж��˿�
	BYTE								m_cbBackCard[GAME_PLAYER][5];						//����˿�
	WORD                                m_wSpecialType[GAME_PLAYER];						//�Ƿ���������

	BYTE								m_cbHandCard[GAME_PLAYER][HAND_CARD_COUNT];			//�������
	BYTE								m_cbSaveHandCard[GAME_PLAYER][HAND_CARD_COUNT];			//�������
	WORD								m_wHandCardType[GAME_PLAYER][3];					//�����������
	int									m_nSpecialCompareResult[GAME_PLAYER];				//�������ͱȽϽ��
	int									m_nCompareResult[GAME_PLAYER][3];					//ÿһ���ȽϽ��
	bool								m_bShoot[GAME_PLAYER][GAME_PLAYER];					//��ǹ
	bool								m_bDaoshui[GAME_PLAYER];							//��ˮ
	//�������Զ�������Ҫ
	BYTE								m_cbAutoCard[13];							//��Ҫ�Զ����Ƶ��������
	WORD								m_wAutoCardType;							//��Ҫ�Զ����Ƶ�������������
	BYTE								m_cbAutoFrontCard[3];						//��Ҫ�Զ����Ƶ�ǰ���˿�
	BYTE								m_cbAutoMidCard[5];							//��Ҫ�Զ����Ƶ��ж��˿�
	BYTE								m_cbAutoBackCard[5];						//��Ҫ�Զ����Ƶĺ���˿�
	AnalyseResult*						m_pHead;
	AnalyseResult*						m_pCurrent;

	void DeleteData();
	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�Ƿ�������
	bool HasMapai(WORD wChairID, WORD wMaiPaiType);
	void ResetbPlayer(){memset(m_bPlayer, 0, sizeof(m_bPlayer));};
	BYTE GetBaseFenByShoot(BYTE cbShootCnt);
	BYTE GetRewardByHandType(BYTE iChair);
	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�Ƿ���Ч
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//��ѯ�����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}
	//���û���
	virtual void SetGameBaseScore(SCORE lBaseScore){}
	virtual void SetGameBaseScore(LONG lBaseScore){}

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

protected:
	//��ȡ������
	virtual SCORE UpdateFishStock()  {return 0;}

	//////////////////////////////////////////////////////////////////////////

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//�û�����������Ϣ
	virtual bool OnSendTrumpetChat(VOID * pData, WORD wDataSize) { return true; }
	//�û���ע
	virtual bool OnActionUserPour(WORD wChairID, IServerUserItem * pIServerUserItem){return true;}

//���̺���
private:
	//���;���
	void SendGameCount(WORD wChairID= INVALID_CHAIR);
	//����
	void SendCard();
	//���̯��
	void ShowCard(WORD wChairID);
	//��ұ���
	void GameCompare(WORD wChairID= INVALID_CHAIR,bool bShow=false);
	//��Ϸ����
	void GameEnd(WORD wChairID= INVALID_CHAIR,bool bShow=false);

	//����ȫ�ֺ��������
	void OfflineGameConclude(WORD wChairID);
	
//��Ϣ����
private:
	//�����ׯ
	bool OnUserGetBanker(WORD wChairID,bool bGet);
	//���̯��
	bool OnUserShowCard(WORD wChairID, CMD_C_ShowCard *pShowCard);
	//������Ϸ
	bool OnUserContinue(WORD wChairID);

//���ܺ���
private:
	//������Ϸ����
	void ResetGame();
	//��ұȵ�
	void GetCompareResult();
	//��ұȵ�test
	int GetCompareResultTest();

	//������ˮ��
	BYTE GetSpecialShui(WORD wType);
	//��ͨ��ˮ��
	BYTE GetShui(WORD wType);
	//�Զ�����
	void AutoShowCard(WORD wChairID,bool testOnly = false);
	//��ȡ����
	bool readFile(BYTE *pCardData, BYTE cbCardCnt, WORD &zhuang);

	int FindMaxChair(BYTE * cbCardBuffer);
};

//////////////////////////////////////////////////////////////////////////

#endif