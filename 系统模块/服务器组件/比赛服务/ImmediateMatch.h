#ifndef IMMEDIATE_MATCH_HEAD_FILE
#define IMMEDIATE_MATCH_HEAD_FILE

#pragma once

//�����ļ�
#include "TableFrameHook.h"
#include "MatchServiceHead.h"
#include "MatchServiceManager.h"
#include "ImmediateGroup.h"

///////////////////////////////////////////////////////////////////////////////////////////

//ʱ�Ӷ���
#define IDI_CHECK_START_MATCH		(IDI_MATCH_MODULE_START+1)					//��ʼʱ��
#define IDI_CHECK_END_MATCH			(IDI_MATCH_MODULE_START+2)					//����ʱ��

//�Ƴ�����
#define REMOVE_BY_ENDMATCH			250											//��������

///////////////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʱ������
struct tagMatchTimerItem
{
	//DWORD								dwTimerID;						//��ʱ�� ID
	DWORD								dwRealID;						//ʵ��ID
	DWORD								dwRepeatTimes;					//�ظ�����
	WPARAM								wBindParam;						//�󶨲���
};

//��������
struct tagMatchReward
{
	DWORD								dwGold;							//��ҽ���
	DWORD								dwMedal;						//���ƽ���
	DWORD								dwExperience;					//���齱��
};

///////////////////////////////////////////////////////////////////////////////////////////
//���Ͷ���

//���鶨��
typedef CWHArray<BYTE> CMatchLoopTimerArray;
typedef CWHArray<CImmediateGroup *> CMatchGroupArray;
typedef CWHArray<tagMatchReward*> CMatchRewardArray;
typedef CWHArray<tagMatchTimerItem *> CTimerItemPtr;
typedef CWHArray<CTableFrameHook *> CMatchTableArray;
typedef CWHArray<IServerUserItem *> CMatchUserItemArray;

///////////////////////////////////////////////////////////////////////////////////////////

//��ʱ��
class CImmediateMatch : public IGameMatchItem,public IImmediateGroupSink,IServerUserItemSink
{
	//��������
protected:
	CMatchTableArray					m_MatchTableArray;				//��������

	//�洢����
protected:
	CMatchUserItemArray					m_OnMatchUserItem;				//�����û�
	CMatchLoopTimerArray				m_LoopTimer;					//ѭ����ʱ��

	//��������
protected:	
	CMatchGroupArray					m_MatchGroup;					//���еı�����
	CMatchGroupArray					m_OverMatchGroup;				//�����ı�����
	CImmediateGroup *					m_pCurMatchGroup;				//��ǰ������

	//��������
protected:
	tagGameMatchOption *				m_pMatchOption;					//��������	
	tagGameServiceOption *				m_pGameServiceOption;			//��������
	tagGameServiceAttrib *				m_pGameServiceAttrib;			//��������

	//�ں˽ӿ�
protected:
	ITableFrame	**						m_ppITableFrame;				//��ܽӿ�
	ITimerEngine *						m_pITimerEngine;				//ʱ������
	IDBCorrespondManager *				m_pIDataBaseEngine;				//��������	
	ITCPNetworkEngineEvent *			m_pITCPNetworkEngineEvent;		//��������

	//����ӿ�
protected:
	IMainServiceFrame *					m_pIGameServiceFrame;			//���ܽӿ�
	IServerUserManager *				m_pIServerUserManager;			//�û�����
	IAndroidUserManager	*				m_pAndroidUserManager;			//��������
	IServerUserItemSink *				m_pIServerUserItemSink;			//�û��ص�

	//��������
public:
	//���캯��
	CImmediateMatch();
	//��������
	virtual ~CImmediateMatch(void);

	//�����ӿ�
public:
 	//�ͷŶ���
 	virtual VOID Release(){ delete this; }
 	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//����֪ͨ
	virtual void OnStartService();

	//����ӿ�
public:
	//������
	virtual bool BindTableFrame(ITableFrame * pTableFrame,WORD wTableID);
	//��ʼ���ӿ�
	virtual bool InitMatchInterface(tagMatchManagerParameter & MatchManagerParameter);	

	//ϵͳ�¼�
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,DWORD dwContextID);

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);	

	//�û��¼�
public:
	//�û���¼
	virtual bool OnEventUserLogon(IServerUserItem * pIServerUserItem);
	//�û��ǳ�
	virtual bool OnEventUserLogout(IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnEventEnterMatch(DWORD dwSocketID ,VOID* pData,DWORD dwUserIP, bool bIsMobile);	
	//�û�����
	virtual bool OnEventUserJoinMatch(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID);
	//�û�����
	virtual bool OnEventUserQuitMatch(IServerUserItem * pIServerUserItem, BYTE cbReason, WORD *pBestRank=NULL, DWORD dwSocketID=INVALID_WORD);
	//�û�״̬
	virtual bool OnEventMatchUserStatus(IServerUserItem * pIServerUserItem, BYTE cbOldUserStatus, BYTE cbCurrUserStatus);

	//�û��ӿ�
public:
	//�û�����
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason);
	//�û�״̬
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID=INVALID_TABLE, WORD wOldChairID=INVALID_CHAIR);
	//�û�Ȩ��
	virtual bool OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight, BYTE cbRightKind);
	//�û���Ϸ����
	virtual bool OnEventUserItemGameData(IServerUserItem * pIServerUserItem, BYTE cbReason);
	
	//�����¼�
public:
	//��ʼ����
	virtual bool OnEventMatchStart(CImmediateGroup *pMatch);
	//��������
	virtual bool OnEventMatchOver(CImmediateGroup *pMatch);
		 
	//��ʱ���ӿ�
public:
	//ɾ����ʱ��
	virtual bool KillGameTimer(DWORD dwTimerID,CImmediateGroup *pMatch);
	//���ö�ʱ��
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CImmediateGroup *pMatch);

	//���ͺ���
public:
	//��������
	virtual bool SendMatchInfo(IServerUserItem * pIServerUserItem);
	//������Ϣ
	virtual bool SendGroupUserMessage(LPCTSTR pStrMessage,CImmediateGroup *pMatch);
	//���ͷ���
	virtual bool SendGroupUserScore(IServerUserItem * pIServerUserItem,CImmediateGroup *pMatch);
	//����״̬
	virtual bool SendGroupUserStatus(IServerUserItem * pIServerUserItem,CImmediateGroup *pMatch);	
	//������Ϣ
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);
	//������Ϣ
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);
	//��������
	virtual bool SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CImmediateGroup *pMatch);
	//��������
	virtual bool SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);	

	//���ܺ���
public:
	//����յ�����
	virtual void InsertNullTable();
	//��ȡ������
	virtual IAndroidUserItem * GetFreeAndroidUserItem();	
	
	//�ڲ�����
private:
	//���㳡��
	DWORD CalcMatchNo();
	//��ȡ�յ�����
	WORD GetNullTable();		
	//ɾ���û�
	bool DeleteUserItem(DWORD dwUserIndex);
	//�Ƴ������û�
	bool RemoveMatchUserItem(IServerUserItem *pIServerUserItem);
	//д�뽱��
	bool WriteUserAward(IServerUserItem *pIServerUserItem,tagMatchRankInfo * pMatchRankInfo);
	//��������״̬
	VOID SetUserMatchStatus(CImmediateGroup *pMatch, BYTE cbUserMatchStatus);
};

#endif