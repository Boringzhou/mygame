#ifndef TABLE_FRAME_HEAD_FILE
#define TABLE_FRAME_HEAD_FILE

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef IServerUserItem * CTableUserItemArray[MAX_CHAIR];				//��Ϸ����
typedef CWHArray<IServerUserItem *> CLookonUserItemArray;				//�Թ�����
typedef CWHArray<tagGameScoreRecord *> CGameScoreRecordArray;			//��¼����
interface IMatchTableFrameNotify;
//////////////////////////////////////////////////////////////////////////////////

//���ӿ��
class CTableFrame : public ITableFrame, public ICompilationSink
{
	//��Ϸ����
protected:
	WORD							m_wTableID;							//���Ӻ���
	WORD							m_wChairCount;						//������Ŀ
	BYTE							m_cbStartMode;						//��ʼģʽ
	WORD							m_wUserCount;						//�û���Ŀ
	bool							m_bRefuseSameIP;					//�ܾ�ͬIP
	bool							m_bDistanceIn300;					//�ܾ�300����һͬ��Ϸ
	double							m_dLongitude[MAX_CHAIR];			//����
	double							m_dLatitude[MAX_CHAIR];				//γ��

	//״̬����
protected:
	bool							m_bGameStarted;						//��Ϸ��־
	bool							m_bDrawStarted;						//��Ϸ��־
	bool							m_bTableStarted;					//��Ϸ��־
	bool							m_bTableInitFinish;					//��ʼ��ʶ
	bool							m_bFangkaGameStarted;				//������ʼ��ʶ

	//״̬����
protected:
	bool							m_bAllowLookon[MAX_CHAIR];			//�Թ۱�־
	SCORE							m_lFrozenedScore[MAX_CHAIR];		//�������
	TCHAR							m_szHeadUrl[MAX_CHAIR][LEN_URL];	//ͷ������

	//��Ϸ����
protected:
	SCORE							m_lCellScore;						//��Ԫ����
	BYTE							m_cbGameStatus;						//��Ϸ״̬
	bool							m_bAgree[MAX_CHAIR];				//ͬ���ɢ
	bool							m_bResponseDismiss[MAX_CHAIR];		//��Ӧ��ɢ
	SCORE							m_lZuanshi;							//��ʯ
	BYTE							m_cbPayType;						//֧����ʽ
	bool							m_bWaitForContinue;					//�ȴ�����
	WORD							m_wFangzhu;							//����
	bool							m_bContinue[MAX_CHAIR];				//����
	DWORD							m_dwFangka;							//����
	bool							m_bKouFangka;						//�Ƿ��ȿ۷���
	bool							m_bKing;							//�Ƿ��д�С��

	//ʱ�����
protected:
	DWORD							m_dwDrawStartTime;					//��ʼʱ��
	SYSTEMTIME						m_SystemTimeStart;					//��ʼʱ��
	WORD                            m_wDrawCount;                       //��Ϸ����

	//��̬����
protected:
	DWORD							m_dwTableOwnerID;					//�����û�
	TCHAR							m_szEnterPassword[LEN_PASSWORD];	//��������
	DWORD							m_dwTablePassword;					//��������

	//���߱���
protected:
	WORD							m_wOffLineCount[MAX_CHAIR];			//���ߴ���
	DWORD							m_dwOffLineTime[MAX_CHAIR];			//����ʱ��

	//�û�����
protected:
	CTableUserItemArray				m_TableUserItemArray;				//��Ϸ�û�
	CLookonUserItemArray			m_LookonUserItemArray;				//�Թ��û�

	//����ӿ�
public:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	ITableFrameSink	*				m_pITableFrameSink;					//���ӽӿ�
	IMainServiceFrame *				m_pIMainServiceFrame;				//����ӿ�
	IAndroidUserManager *			m_pIAndroidUserManager;				//�����ӿ�

	//��չ�ӿ�
protected:
	ITableUserAction *				m_pITableUserAction;				//�����ӿ�
	ITableUserRequest *				m_pITableUserRequest;				//����ӿ�	

	//���ݽӿ�
protected:
	IDataBaseEngine *				m_pIKernelDataBaseEngine;			//�ں�����
	IDataBaseEngine *				m_pIRecordDataBaseEngine;			//��¼����

	//������Ϣ
protected:
	tagGameParameter *				m_pGameParameter;					//���ò���
	tagGameMatchOption *			m_pGameMatchOption;					//��������	
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��������
	tagGameServiceOption *			m_pGameServiceOption;				//��������
	tagFangkaGameOption				m_nFangkaGameOption;				//��������

	//��Ϸ��¼
protected:
	CGameScoreRecordArray			m_GameScoreRecordActive;			//��Ϸ��¼
	static CGameScoreRecordArray	m_GameScoreRecordBuffer;			//��Ϸ��¼

	//�����ӿ�
protected:
	ITableFrameHook	*				m_pITableFrameHook;					//�����ӿ�
	ITableUserAction *				m_pIMatchTableAction;				//�����ӿ�

	//��������
public:
	//���캯��
	CTableFrame();
	//��������
	virtual ~CTableFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���Խӿ�
public:
	//���Ӻ���
	virtual WORD GetTableID() { return m_wTableID; }
	//��Ϸ����
	virtual WORD GetChairCount() { return m_wChairCount; }
	//��λ����Ŀ
	virtual WORD GetNullChairCount(){return m_wChairCount - m_wUserCount;}

	//���ò���
public:
	//�Զ�����
	virtual VOID * GetCustomRule() { return m_pGameServiceOption->cbCustomRule; };
	//��������
	virtual tagGameMatchOption * GetGameMatchOption() { return m_pGameMatchOption; };
	//��������
	virtual tagGameServiceAttrib * GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//��������
	virtual tagGameServiceOption * GetGameServiceOption() { return m_pGameServiceOption; }
	//��������
	virtual tagFangkaGameOption * GetFangkaGameOption(){ return &m_nFangkaGameOption; }

	//���ýӿ�
public:
	//��ʼģʽ
	virtual BYTE GetStartMode() { return m_cbStartMode; }
	//��ʼģʽ
	virtual VOID SetStartMode(BYTE cbStartMode) { m_cbStartMode=cbStartMode; }

	//��Ԫ����
public:
	//��Ԫ����
	virtual SCORE GetCellScore() { return m_lCellScore; }
	//��Ԫ����
	virtual VOID SetCellScore(SCORE lCellScore);
	//�۳�����
	virtual void DedultScore(IServerUserItem * pIServerUserItem,SCORE lScore);
	//�۳�����
	void DedultInsure(IServerUserItem * pIServerUserItem,DWORD dwInsure);
	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE GetGameStatus() { return m_cbGameStatus; }
	//����״̬
	virtual VOID SetGameStatus(BYTE cbGameStatus) { m_cbGameStatus=cbGameStatus; }
	//��ȡ����
	virtual DWORD GetPassword(){ return m_dwTablePassword; }
	//��������
	virtual void SetPassword(DWORD dwPassword){ m_dwTablePassword=dwPassword; }
	//��ȡIP����
	virtual bool GetRefuseSameIP(){ return m_bRefuseSameIP; }
	//����IP����
	virtual void SetRefuseSameIP(bool bRefuseSameIP){ m_bRefuseSameIP=bRefuseSameIP; }
	//��ȡ��������
	virtual bool GetDistanceIn300(){ return m_bDistanceIn300; }
	//���þ�������
	virtual void SetDistanceIn300(bool bDistanceIn300){ m_bDistanceIn300=bDistanceIn300; }
	//��ȡ����
	virtual double GetLongitude(WORD wChairID){ return m_dLongitude[wChairID]; }
	//���þ���
	virtual void SetLongitude(WORD wChairID,double dLongitude){ m_dLongitude[wChairID]=dLongitude; }
	//��ȡγ��
	virtual double GetLatitude(WORD wChairID){ return m_dLatitude[wChairID]; }
	//����γ��
	virtual void SetLatitude(WORD wChairID,double dLatitude){ m_dLatitude[wChairID]=dLatitude; }
	//������ʯ
	virtual void SetZuanshi(SCORE lZuanshi) { m_lZuanshi = lZuanshi;}
	virtual void SetInsure(DWORD lFangka){m_dwFangka = lFangka;}
	virtual BYTE GetZuanshi(){ if (m_cbPayType == 0)
	{
		return m_lZuanshi;
	} 
	else
	{
		return m_lZuanshi/m_wChairCount;
	}}
	//֧����ʽ
	virtual void SetPayType(BYTE cbType){m_cbPayType = cbType;}
	//���÷���
	virtual void SetFangzhu(WORD wUser){m_wFangzhu = wUser;}
	virtual WORD GetFangzhu(){return m_wFangzhu;}
	//���÷���ID
	virtual void SetCreatorID(DWORD dwCreatorID){m_dwTableOwnerID = dwCreatorID;}
	virtual DWORD GetCreatorID(){return m_dwTableOwnerID;}

	//��������
	virtual void SetContinue(WORD wUser){m_bContinue[wUser]=true;}
	void SetKouFangka(bool bKou){m_bKouFangka = bKou;}
	void SetKing(bool bKing){m_bKing = bKing;}
	//��Ϣ�ӿ�
public:
	//��Ϸ״̬
	virtual bool IsGameStarted() { return m_bGameStarted; }
	//��Ϸ״̬
	virtual bool IsDrawStarted() { return m_bDrawStarted; }
	//��Ϸ״̬
	virtual bool IsTableStarted() { return m_bTableStarted; }
	//����״̬
	virtual bool IsTableLocked() { return (m_szEnterPassword[0]!=0); }

	//���ƽӿ�
public:
	//��ʼ��Ϸ
	virtual bool StartGame();
	//��ɢ��Ϸ
	virtual bool DismissGame();
	//������Ϸ
	virtual bool ContinueGame(WORD wChairID);
	//������Ϸ
	virtual bool ConcludeGame(BYTE cbGameStatus);
	//��������
	virtual bool ConcludeTable();
	//��ʼ����ģʽ
	virtual void SetFangkaStart(bool bStart);
	//�Ƿ����κ�������
	virtual bool AnyOneContinue();
	//�û��ӿ�
public:
	//Ѱ���û�
	virtual IServerUserItem * SearchUserItem(DWORD dwUserID);
	//��Ϸ�û�
	virtual IServerUserItem * GetTableUserItem(WORD wChairID);
	//�Թ��û�
	virtual IServerUserItem * EnumLookonUserItem(WORD wEnumIndex);

	//д�ֽӿ�
public:
	//д�����
	virtual bool WriteUserScore(WORD wChairID, tagScoreInfo & ScoreInfo, DWORD dwGameMemal=INVALID_DWORD, DWORD dwPlayGameTime=INVALID_DWORD);
	//д�����
	virtual bool WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount);

	//����ӿ�
public:
	//����˰��
	virtual SCORE CalculateRevenue(WORD wChairID, SCORE lScore);
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);

	//ʱ��ӿ�
public:
	//����ʱ��
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter);
	//ɾ��ʱ��
	virtual bool KillGameTimer(DWORD dwTimerID);
	//�Ƿ��м�ʱ��
	virtual bool HasGameTimer(DWORD dwTimerID);
	//��ȡ��ʱ��ʣ��ʱ��
	virtual DWORD GetLeaveTime(DWORD dwTimerID);
	//��Ϸ�û�
public:
	//��������
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID);
	//��������
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize,WORD wMainCmdID=MDM_GF_GAME);

	//�Թ��û�
public:
	//��������
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID);
	//��������
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����û�
public:
	//��������
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID);
	//��������
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//ϵͳ��Ϣ
public:
	//������Ϣ
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType);
	//��Ϸ��Ϣ
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);
	//������Ϣ
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);

	//��������
public:
	//��������
	virtual bool PerformStandUpAction(IServerUserItem * pIServerUserItem, bool bInitiative=false);
	//�Թ۶���
	virtual bool PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	//���¶���
	virtual bool PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem, LPCTSTR lpszPassword=NULL);

	//���ܽӿ�
public:
	//���ͳ���
	virtual bool SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//�����ӿ�
public:
	//���ýӿ�
	virtual bool SetTableFrameHook(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	virtual IUnknownEx * GetTableFrameHook(){ return m_pITableFrameHook;}
	//α������
	virtual bool ImitateGameOption(IServerUserItem * pIServerUserItem);

	//��ȡ��Ϣ
public:
	//��ȡ��Ϣ
	virtual LPCTSTR GetCompilation(){return szCompilation;}

	//���ܺ���
public:
	//��Ϸ����
	WORD GetDrawCount();
	//��ȡ��λ
	WORD GetNullChairID();
	//�����λ
	WORD GetRandNullChairID();
	//�û���Ŀ
	WORD GetSitUserCount();
	//�Թ���Ŀ
	WORD GetLookonUserCount();
	//������Ŀ
	WORD GetOffLineUserCount();
	//ͬ���ɢ
	bool OnEventDismissRoom(WORD wChairID,bool bAgree);
	//���ͽ�ɢ������Ϣ
	void SendDismissInfo(WORD wChairID);
	//���ܺ���
public:
	//�û�״��
	WORD GetTableUserInfo(tagTableUserInfo & TableUserInfo);
	//��������
	bool InitializationFrame(WORD wTableID, tagTableFrameParameter & TableFrameParameter);

	//�û��¼�
public:
	//�����¼�
	bool OnEventUserOffLine(IServerUserItem * pIServerUserItem);
	//�����¼�
	bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//ϵͳ�¼�
public:
	//ʱ���¼�
	bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//��Ϸ�¼�
	bool OnEventSocketGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//����¼�
	bool OnEventSocketFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��������
public:
	//����״̬
	bool SendTableStatus();
	//����ʧ��
	bool SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode);

	//Ч�麯��
public:
	//��ʼЧ��
	bool EfficacyStartGame(WORD wReadyChairID);
	//��ַЧ��
	bool EfficacyIPAddress(IServerUserItem * pIServerUserItem);
	//����Ч��
	bool EfficacyScoreRule(IServerUserItem * pIServerUserItem);
	//����Ч��
	bool EfficacyEnterTableScoreRule(WORD wChairID, IServerUserItem * pIServerUserItem);

public:
	//������
	bool CheckDistribute();
	//��Ϸ��¼
	void RecordGameScore(bool bDrawStarted, DWORD dwStartGameTime=INVALID_DWORD);


	//׼����ʱʱ��
	void StartReadyOutTime(BYTE wChairID);
};

//////////////////////////////////////////////////////////////////////////////////

#endif