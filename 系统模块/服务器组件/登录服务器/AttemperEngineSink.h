#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerListManager.h"

//////////////////////////////////////////////////////////////////////////////////
//���ݶ���

//��������
#define CLIENT_KIND_FALSH			1									//��ҳ����
#define CLIENT_KIND_MOBILE			2									//�ֻ�����
#define CLIENT_KIND_COMPUTER		3									//��������

//�󶨲���
struct tagBindParameter
{
	//�������
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��

	//������Ϣ
	WORD							wModuleID;							//ģ���ʶ
	BYTE							cbClientKind;						//��������
	DWORD							dwPlazaVersion;						//�����汾
};

//ƽ̨����
struct tagPlatformParameter
{
	DWORD							dwExchangeRate;						//�һ�����
	DWORD							dwPresentExchangeRate;				//������Ϸ�Ҷһ���
	DWORD							dwRateGold;							//��Ϸ����Ϸ�Ҷһ���
};


//��֤�����
struct tagVerifyCode
{
	DWORD							dwClientAddr;						//�ͻ���IP
	DWORD							dwUpdateTime;						//����ʱ��
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������
	TCHAR							szValidateCode[LEN_VERIFY_CODE];	//��֤��
};

//////////////////////////////////////////////////////////////////////////////////

typedef CWHArray<tagVerifyCode *>	CVerifyCodeArray;					//��֤���б�
typedef CMap<DWORD,DWORD,DWORD,DWORD>	CRoomListQueryMap;				//���󷿼��б�
//////////////////////////////////////////////////////////////////////////////////

//���ȹ���
class CAttemperEngineSink : public IAttemperEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;

	//״̬����
protected:
	bool							m_bNeekCorrespond;					//Э����־
	bool                            m_bShowServerStatus;                //��ʾ������״̬

	//���ñ���
protected:
	SCORE							m_lCheckInReward[LEN_WEEK];			//ǩ������
	tagPlatformParameter			m_PlatformParameter;				//ƽ̨����
	tagBaseEnsureParameter			m_BaseEnsureParameter;				//�ͱ�����	

	//�����б�����
protected:
	CRoomListQueryMap				m_RoomListQueryMap;					//�����б�����
	//�������
protected:
	WORD							m_wTaskCount;						//������Ŀ
	WORD							m_wTaskCountBuffer;					//������Ŀ
	tagTaskParameter				m_TaskParameter[TASK_MAX_COUNT];	//�������
	tagTaskParameter				m_TaskParameterBuffer[TASK_MAX_COUNT];//�������

	//��Ա����
protected:
	WORD							m_wMemberCount;						//��Ա��Ŀ
	tagMemberParameter				m_MemberParameter[10];				//��Ա����

	//�ȼ�����
protected:
	WORD							m_wLevelCount;						//�ȼ���Ŀ
	tagGrowLevelConfig				m_GrowLevelConfig[60];				//�ȼ�����

	//��������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	tagBindParameter *				m_pBindParameter;					//��������

	//�������
protected:
	CServerListManager				m_ServerListManager;				//�б����
	CVerifyCodeArray				m_VerifyCodeArray;					//��֤����
	
	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IDataBaseEngine *				m_pIDataBaseEngine;					//��������
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������
	ITCPSocketService *				m_pITCPSocketService;				//Э������

	//��Ƶ����
protected:
	WORD							m_wAVServerPort;					//��Ƶ�˿�
	DWORD							m_dwAVServerAddr;					//��Ƶ��ַ

	//��������
public:
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�첽�ӿ�
public:
	//�����¼�
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//�Զ��¼�
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//Ӧ���¼�
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//�ر��¼�
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//��ȡ�¼�
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���Ӵ���
protected:
	//ע���¼�
	bool OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//Զ�̷���
	bool OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//�������
	bool OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//�������
	bool OnTCPSocketMainRoomService(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//��¼����
	bool OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�б���
	bool OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������
	bool OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//Զ�̴���
	bool OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��������
	bool OnTCPNetworkMainAndroidService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�û�������Ϣ
	bool OnDBPCUserAccountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ֻ��¼�
protected:
	//��¼����
	bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);	
	//�б���
	bool OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�����¼�
protected:
	//I D ��¼
	bool OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺŵ�¼
	bool OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//���ߵ�¼
	bool OnTCPNetworkSubPCLogonManageTool(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��֤����
	bool OnTCPNetworkSubPCVerifyIndividual(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ο͵�¼
	bool OnTCPNetworkSubPCLogonVisitor(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ѯ��֤��
	bool OnTCPNetworkSubPCQueryVerifyCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�ֻ��¼�
protected:
	//I D ��¼
	bool OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺŵ�¼
	bool OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������¼
	bool OnTCPNetworkSubMBLogonOtherPlatform(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ο͵�¼
	bool OnTCPNetworkSubMBLogonVisitor(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ȡ��������
	bool OnTCPNetworkSubMBGetOnline(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//��Ϸ�¼�
protected:
	//��¼�ɹ�
	bool OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBPCLogonValidateMBCard(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBPCLogonValidatePassPort(DWORD dwContextID, VOID * pData, WORD wDataSize);	
	//��֤���
	bool OnDBPCLogonVerifyResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
    //ƽ̨����
	bool OnDBPCPlatformParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);	  
	//�û�ͷ��
	bool OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���гɹ�
	bool OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnDBPCUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ͨ���
	bool OnDBPCUserInsureEnableResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ����Ϣ
	bool OnDBPCUserCheckInInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ�����
	bool OnDBPCUserCheckInResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ������
	bool OnDBPCCheckInReward(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����б�
	bool OnDBPCTaskList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�������
	bool OnDBPCTaskListEnd(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnDBPCTaskInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������
	bool OnDBPCTaskResult(DWORD dwContextID, VOID * pData, WORD wDataSize);	
	//�ͱ�����
	bool OnDBPCBaseEnsureParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ͱ����
	bool OnDBPCBaseEnsureResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ƹ����
	bool OnDBPCUserSpreadInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�������
	bool OnDBPCUserMatchSignupResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ȼ�����
	bool OnDBPCGrowLevelConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ȼ�����
	bool OnDBPCGrowLevelParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ȼ�����
	bool OnDBPCGrowLevelUpgrade(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ա����
	bool OnDBPCMemberParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������
	bool OnDBPCPurchaseResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ����
	bool OnDBPCExChangeResult(DWORD dwContextID, VOID * pData, WORD wDataSize);	
	//��������
	bool OnDBAndroidParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ɹ�
	bool OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱����
	bool OnDBPCLotteryConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱��Ϣ
	bool OnDBPCLotteryUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱���
	bool OnDBPCLotteryResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	bool OnDBPCSimpleLotteryResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCQueryUserGameData(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ״̬
	bool OnDBPCInGameSevrerID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ϵͳQQ:DCY by 2017.3.9
	bool OnDBSystemQQ(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ϵͳ����:DCY by 2017.3.9
	bool OnDBSystemInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ϵ����
	bool OnDBSystemContact(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�̳�
	bool OnDBGameShop(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ���Ʒ
	bool OnDBExchangeShop(DWORD dwContextID, VOID * pData, WORD wDataSize);



	//�ֻ��¼�
protected:
	//��¼�ɹ�
	bool OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�б��¼�
protected:
	//��Ϸ����
	bool OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�ڵ�
	bool OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�б�
	bool OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��������
protected:
	//�汾���
	bool CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer=true);
	//��������
	bool SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�����б�
protected:
	//��������
	VOID SendGameTypeInfo(DWORD dwSocketID);
	//��������
	VOID SendGameKindInfo(DWORD dwSocketID);
	//���ͽڵ�
	VOID SendGameNodeInfo(DWORD dwSocketID, WORD wKindID);
	//���Ͷ���
	VOID SendGamePageInfo(DWORD dwSocketID, WORD wKindID);
	//���ͱ���
	VOID SendGameMatchInfo(DWORD dwSocketID, WORD wKindID);
	//���ͷ���
	VOID SendGameServerInfo(DWORD dwSocketID, WORD wKindID,bool bOnlyMatch=false);
	//�ȼ�����
	VOID SendGrowLevelConfig(DWORD dwSocketID);
	//�����б�
	VOID SendUserSignupInfo(DWORD dwSocketID,WORD wSignupCount,tagSignupMatchInfo * pSignupMatchInfo,bool bMobile);

	//�ֻ��б�
protected:
	//��������
	VOID SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID);
	//���ͷ���
	VOID SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID);

	//��������
protected:
	//����ʧ��
	bool SendInsureFailure(DWORD dwSocketID, LONG lResultCode, LPCTSTR pszDescribe);
	//�����ɹ�
	VOID SendOperateSuccess(DWORD dwContextID,LONG lResultCode, LPCTSTR pszDescribe);
	//����ʧ��
	VOID SendOperateFailure(DWORD dwContextID,LONG lResultCode, LPCTSTR pszDescribe);
	//������֤��
	VOID RandVerifyCode(LPTSTR pszVerifyCode, UINT nMaxCount);
	//Ч����֤
	bool CheckVerifyCode(LPCTSTR pszVerifyCode, LPCTSTR pszVerifyCodeBak);
	//������֤��
	VOID SendVerifyCode(DWORD dwSocketID, LPCTSTR pszVerifyCode, bool bCloseSocket);
};

//////////////////////////////////////////////////////////////////////////////////

#endif