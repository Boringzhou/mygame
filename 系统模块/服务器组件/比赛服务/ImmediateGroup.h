#ifndef IMMEDIATE_GROUP_HEAD_FILE
#define IMMEDIATE_GROUP_HEAD_FILE
#pragma once

//�����ļ�
#include "TableFrameHook.h"
#include "MatchServiceHead.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
//ʱ�Ӷ���
#define IDI_DELETE_OVER_MATCH		(IDI_MATCH_MODULE_START+1)					//ɾ�������ı�����
#define IDI_CHECK_OVER_MATCH		(IDI_MATCH_MODULE_START+2)					//���һ������ 
#define IDI_GROUP_TIMER_START		(IDI_MATCH_MODULE_START+10)					//�������ڵĶ�ʱ��

//ʱ�Ӷ���
#define IDI_CHECK_MATCH_GROUP		(IDI_GROUP_TIMER_START+1)					//��Ѳ���п��������� 
#define IDI_WAITTABLE_FIRST_ROUND	(IDI_GROUP_TIMER_START+2)					//�ȴ����� 
#define IDI_WAITTABLE_RAND_TABLE	(IDI_GROUP_TIMER_START+3)					//�ȴ�������� 
#define IDI_SWITCH_WAIT_TIME		(IDI_GROUP_TIMER_START+4)					//�����ȴ�ʱ�� 
#define IDI_ALL_DISMISS_GAME		(IDI_GROUP_TIMER_START+5)					//ȫ����ɢ��Ϸ
#define IDI_LASTMATCHRESULT			(IDI_GROUP_TIMER_START+6)					//��������
#define IDI_ENTER_NEXT_ROUND		(IDI_GROUP_TIMER_START+7)					//������һ��ѭ��
#define IDI_TABLE_REQUEST_USER		(IDI_GROUP_TIMER_START+8)					//�Զ���������

//ʱ�Ӷ���
#define IDI_CONTINUE_GAME			1											//������Ϸ
#define IDI_LEAVE_TABLE				2											//�뿪����
#define IDI_CHECK_TABLE_START		3											//�����������Ƿ�ʼ(����п���׼���󲻿�ʼ���)

///////////////////////////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//����״̬
enum enMatchStatus
{
	MatchStatus_Null=0,										//��״̬
	MatchStatus_Signup,										//������
	MatchStatus_Wait,										//�ȴ�����
	MatchStatus_FirstRound									//���ֱ���
};

//������ʱ��
struct tagMatchTimer
{
	DWORD					dwTimerID;						//��ʱ��ID
	int						iElapse;						//ִ�м����	
	WPARAM					wParam;							//���Ӳ���
	LPARAM					lParam;							//���Ӳ���
};

//��������
struct tagMatchScore
{
	DWORD					dwUserID;						//�û�I D
	SCORE					lScore;							//�û��ɼ�	
	IServerUserItem *		pUserItem;						//�û��ӿ�
};

//��������
struct tagMatchRanking
{
	DWORD					dwUserID;						//�û�I D
	SCORE					lScore;							//�û�����
	LONG					lExpand;						//��չֵ
	IServerUserItem *		pUserItem;						//�û��ӿ�
};

//����������
struct tagTableFrameInfo 
{
	ITableFrame		*		pTableFrame;					//���ӽӿ�
	WORD					wTableID;						//���ӱ��
	DWORD					dwBaseScore;					//��������	
	BYTE					cbPlayCount;					//��Ϸ����
	bool					bRoundTableFinish;				//��ɱ�ʶ
	bool					bSwtichTableFinish;				//�������
	bool					bWaitForNextInnings;			//�ȴ��¾�
};
///////////////////////////////////////////////////////////////////////////////////////////////////////
//���Ͷ���

//���鶨��
typedef CWHArray<tagMatchTimer *>	CUserMatchTimerArray;
typedef CWHArray<tagMatchScore *>	CUserMatchScoreArray;
typedef CWHArray<IServerUserItem *>	CMatchUserItemArray;
typedef CWHArray<tagTableFrameInfo *>	CTableFrameMananerArray;

//�ֵ䶨��
typedef CMap<DWORD,DWORD,IServerUserItem *, IServerUserItem *>		CUserItemMap;
typedef CMap<IServerUserItem *, IServerUserItem *, DWORD, DWORD>	CUserSeatMap;

///////////////////////////////////////////////////////////////////////////////////////////////////////
//�ӿڶ���

//��������
class CImmediateGroup;

//����ص�
interface IImmediateGroupSink
{
	//�����¼�
public:
	//��ʼ����
	virtual bool OnEventMatchStart(CImmediateGroup *pMatch)=NULL;
	//��������
	virtual bool OnEventMatchOver(CImmediateGroup *pMatch)=NULL;

	//��ʱ���ӿ�
public:
	//ɾ����ʱ��
	virtual bool KillGameTimer(DWORD dwTimerID,CImmediateGroup *pMatch)=NULL;
	//���ö�ʱ��
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CImmediateGroup *pMatch)=NULL;	

	//���ͺ���
public:
	//��������
	virtual bool SendMatchInfo(IServerUserItem * pIServerUserItem)=NULL;
	//������Ϣ
	virtual bool SendGroupUserMessage(LPCTSTR pStrMessage,CImmediateGroup *pMatch)=NULL;
	//���ͷ���
	virtual bool SendGroupUserScore(IServerUserItem * pIServerUserItem,CImmediateGroup *pMatch)=NULL;
	//����״̬
	virtual bool SendGroupUserStatus(IServerUserItem * pIServerUserItem,CImmediateGroup *pMatch)=NULL;	
	//������Ϣ
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)=NULL;
	//������Ϣ
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)=NULL;
	//��������
	virtual bool SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CImmediateGroup *pMatch)=NULL;
	//��������
	virtual bool SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;	

	//���ܺ���
public:
	//����յ�����
	virtual void InsertNullTable()=NULL;
	//��ȡ������
	virtual IAndroidUserItem * GetFreeAndroidUserItem()=NULL;	
};

//���ò���
struct tagImmediateMatchParameter
{
	tagGameServiceAttrib *			pGameServiceAttrib;
	tagGameServiceOption *			pGameServiceOption;
	tagGameMatchOption *			pGameMatchOption;
	IImmediateGroupSink *			pIImmediateGroupSink;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
class CImmediateGroup: public IMatchEventSink
{
	friend class CImmediateMatch;

	//״̬��Ϣ
protected:	
	DWORD							m_dwCurBase;					//��ǰ����
	DWORD							m_dwMatchNO;					//�������
	enMatchStatus					m_enMatchStatus;				//�����׶�
	BYTE							m_LoopTimer;					//ѭ����ʱ��
	WORD							m_wAndroidUserCount;			//�����˸���
	SYSTEMTIME						m_MatchStartTime;				//������ʼ

    //ָ�����
protected:
	tagGameServiceAttrib *			m_pGameServiceAttrib;			//��������
	tagGameServiceOption *			m_pGameServiceOption;			//��������
	tagGameMatchOption *			m_pMatchOption;					//��������
	IImmediateGroupSink *			m_pMatchSink;					//����ص�

	//�洢����
protected:
	CUserItemMap					m_OnMatchUserMap;				//�����û�
	CMatchUserItemArray				m_FreeUserArray;				//�����û�
	CTableFrameMananerArray			m_MatchTableArray;				//Ԥ����������
	CUserMatchTimerArray			m_MatchTimerArray;				//����ר�ö�ʱ��	

	//��̬����
protected:
	static WORD						m_wChairCount;					//������Ŀ
	
	//��������
public:
	//���캯��
	CImmediateGroup(DWORD dwMatchNO);
	//��������
	virtual ~CImmediateGroup();

	 //�����ӿ�
public:
 	//�ͷŶ���
 	virtual VOID Release(){}
 	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܺ���
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount);
	//��Ϸ����
	virtual bool OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	 //�û��¼�
public:
	 //��ҷ���
	 virtual bool OnEventUserReturnMatch(ITableFrame *pITableFrame,IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

 	//�û��¼�
public:
	//�û�����
	bool OnUserSignUpMatch(IServerUserItem *pUserItem);
	//�û�����
	BOOL OnUserQuitMatch(IServerUserItem *pUserItem);

	//���ú���
public:
	//���ñ���
	void SetImmediateGroup(tagImmediateMatchParameter & ImmediateMatchParameter);

	//��������
protected:
	//��������
	void MatchOver();
	//Ч�������ʼ
	void EfficacyStartMatch();
	//����������
	bool CheckMatchUser();
	//��������Ƿ�ʼ��Ϸ
	void CheckTableStart(ITableFrame *pITableFrame);
	//�������
	WORD SortMapUser(tagMatchScore score[]);
	//�����������
	void RearrangeUserSeat(bool bDismissGame=true);
	//��ʱ����Ϣ
	bool OnTimeMessage(DWORD dwTimerID, WPARAM dwBindParameter);
	WORD GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame=NULL);	
	//������ʾ
	VOID SendWaitTip(ITableFrame *pTableFrame, WORD wChairID=INVALID_CHAIR);
	//�����������
	void AddMatchTable(tagTableFrameInfo* pTableFrameInfo){m_MatchTableArray.Add(pTableFrameInfo);}
	//��ȡ����ר�ýӿ�
	tagTableFrameInfo* GetTableInterface(ITableFrame* ITable);
	//���������������û�
	void TabelRequestUser(bool bPrecedeSit=true);
	//����һ����Ϣ
	void SendGroupBox(LPCTSTR pStrMessage);	
	//���������û��뿪
	void AllUserExitTable(ITableFrame *pITableFrame);
	//����������Ϸ
	void ContinueGame(ITableFrame *pITableFrame);
	//��ȡ���������û�
	WORD GetPrecedeSitUser(DWORD dwCurTime);
	//ʵ�ָú����ɱ�֤Free��Ψһ
	void InsertFreeGroup(IServerUserItem *pUserItem);
	//���ͱ�����Ϣ
	void SendTableUserMatchInfo(ITableFrame *pITableFrame, WORD wChairID);
	//����������Ϸ����
	void FirstRoundRuleInningsGameEnd(ITableFrame *pITableFrame);

	//��Ǻ���
protected:	
	//�����
	bool CheckRoundTableFinish();
	//�����
	bool CheckSwitchTableFinish();
	//���ñ��
	void SetRoundTableFinish(ITableFrame *pITableFrame);
	//���ñ��
	void SetSwitchTableFinish(ITableFrame *pITableFrame);	

	//��ʱ������
protected:
	//����ʱ��
	void CaptureMatchTimer();
	//ɱ����ʱ��
	void AllKillMatchTimer();
	//ɱ����ʱ��
	bool KillMatchTimer(INT_PTR dwIndexID);
	//ɱ����ʱ��
	bool KillMatchTimer(DWORD dwTimerID, WPARAM wParam);
	//Ͷ�ݶ�ʱ��
	void PostMatchTimer(DWORD dwTimerID, int iElapse, WPARAM wParam=0, LPARAM lParam=0);	
	//ɱ����ʱ��
	void KillMatchGameTimer(DWORD dwTimerID);	
	//�趨��ʱ��
	void SetMatchGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter);	
};

#endif