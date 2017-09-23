#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_LOAD_GAME_LIST			1									//�����б�
#define IDI_CONNECT_CORRESPOND		2									//������ʶ
#define IDI_COLLECT_ONLINE_INFO		3									//ͳ������

//ʱ�䶨��
#define	TIME_VALID_VERIFY_CODE		0									//������
//////////////////////////////////////////////////////////////////////////////////

// void ConvertUtf8ToGBK(CString &strUtf8)
// {
// 	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
// 	wchar_t * wszGBK = new wchar_t[len];
// 	memset(wszGBK,0,len);
// 	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszGBK, len); 
// 
// 	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
// 	char *szGBK=new char[len + 1];
// 	memset(szGBK, 0, len + 1);
// 	WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);
// 
// 	strUtf8 = szGBK;
// 	delete[] szGBK;
// 	delete[] wszGBK;
// }
// void ConvertUtf8ToGBK(char* pChar,int iLen)
// {
// 	CString kString(pChar);
// 	ConvertUtf8ToGBK(kString);
// 	strncpy(pChar,kString.GetString(),iLen);
// }
//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//״̬����
	m_bNeekCorrespond=true;
	m_bShowServerStatus=false;

	//״̬����
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//��Ƶ����
	m_wAVServerPort=0;
	m_dwAVServerAddr=0;

	//�������
	m_wTaskCount=0;
	m_wLevelCount=0;
	m_wMemberCount=0;	
	m_wTaskCountBuffer=0;
	ZeroMemory(m_lCheckInReward,sizeof(m_lCheckInReward));
	ZeroMemory(m_TaskParameter,sizeof(m_TaskParameter));	
	ZeroMemory(m_MemberParameter,sizeof(m_MemberParameter));
	ZeroMemory(m_GrowLevelConfig,sizeof(m_GrowLevelConfig));
	ZeroMemory(&m_PlatformParameter,sizeof(m_PlatformParameter));
	ZeroMemory(m_TaskParameterBuffer,sizeof(m_TaskParameterBuffer));	
	ZeroMemory(&m_BaseEnsureParameter,sizeof(m_BaseEnsureParameter));

	//��������
	m_VerifyCodeArray.RemoveAll();

	//::AllocConsole();
	//::freopen("CON", "wt", stdout);

	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
	//������Դ
	for (INT i=0; i<m_VerifyCodeArray.GetCount();i++)
	{
		tagVerifyCode *pValidateCode=m_VerifyCodeArray[i];
		if (pValidateCode!=NULL)
		{
			SafeDelete(pValidateCode);
		}
		m_VerifyCodeArray.RemoveAt(i);
	}
}

//�ӿڲ�ѯ
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

	//����ʱ��
	ASSERT(m_pITimerEngine!=NULL);
#ifndef _DEBUG
	//m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,m_pInitParameter->m_wCollectTime*1000L,TIMES_INFINITY,0);
#else
	//m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,20*1000L,TIMES_INFINITY,0);
#endif

	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	CString szFileName;
	GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
	szFileName=szPath;
	int nIndex = szFileName.ReverseFind(TEXT('\\'));
	szFileName = szFileName.Left(nIndex);
	szFileName += TEXT("\\PlazaOptionConfig.ini");

	//��ȡ����
	m_bShowServerStatus = (GetPrivateProfileInt(TEXT("ServerStatus"),TEXT("ShowServerStatus"),0,szFileName) != 0);

	//��ȡĿ¼
	TCHAR szServerAddr[LEN_SERVER]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//��ȡ����
	TCHAR szVideoFileName[MAX_PATH]=TEXT("");
	_sntprintf(szVideoFileName,CountArray(szVideoFileName),TEXT("%s\\VideoOption.ini"),szPath);
	m_wAVServerPort=GetPrivateProfileInt(TEXT("VideoOption"),TEXT("ServerPort"), 0,szVideoFileName);
	DWORD dwAddrLen=GetPrivateProfileString(TEXT("VideoOption"),TEXT("ServerAddr"), TEXT(""), szServerAddr,LEN_SERVER,szVideoFileName);
	if(dwAddrLen>0)
	{
		CT2CA strServerDomain(szServerAddr);
		m_dwAVServerAddr=inet_addr(strServerDomain);
	}
	else
	{
		m_dwAVServerAddr=0;
	}

	return true;
}

//ֹͣ�¼�
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//״̬����
	m_bNeekCorrespond=true;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//�������
	m_wTaskCount=0;
	m_wTaskCountBuffer=0;
	ZeroMemory(&m_TaskParameter,sizeof(m_TaskParameter));
	ZeroMemory(&m_TaskParameterBuffer,sizeof(m_TaskParameterBuffer));

	//��Ա����
	m_wMemberCount=0;
	ZeroMemory(m_MemberParameter,sizeof(m_MemberParameter));

	//�ȼ�����
	m_wLevelCount=0;	
	ZeroMemory(m_GrowLevelConfig,sizeof(m_GrowLevelConfig));

	//ǩ������
	ZeroMemory(m_lCheckInReward,sizeof(m_lCheckInReward));	

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	//�б����
	m_ServerListManager.ResetServerList();

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_LOAD_DB_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			//ƽ̨����
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_PLATFORM_PARAMETER,0,NULL,0);			

			//���ؽ���
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_CHECKIN_REWARD,0,NULL,0);

			//��������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_TASK_LIST,0,NULL,0);

			//���صͱ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_BASEENSURE,0,NULL,0);

			//��Ա����
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_MEMBER_PARAMETER,0,NULL,0);		

			//�ɳ�����
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GROWLEVEL_CONFIG,0,NULL,0);	

			return true;
		}
	case CT_CONNECT_CORRESPOND:		//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//ʱ���¼�
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_LOAD_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case IDI_CONNECT_CORRESPOND:	//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case IDI_COLLECT_ONLINE_INFO:	//ͳ������
		{
			//��������
			DBR_GP_OnLineCountInfo OnLineCountInfo;
			ZeroMemory(&OnLineCountInfo,sizeof(OnLineCountInfo));

			//��ȡ����
			OnLineCountInfo.dwOnLineCountSum=m_ServerListManager.CollectOnlineInfo(false);
			OnLineCountInfo.dwAndroidCountSum=m_ServerListManager.CollectOnlineInfo(true);

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=OnLineCountInfo.wKindCount++;
					OnLineCountInfo.OnLineCountKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;

					//Ŀ¼����
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwAndroidCount=pGameKindItem->m_GameKind.dwAndroidCount;
				}

				//����ж�
				if (OnLineCountInfo.wKindCount>=CountArray(OnLineCountInfo.OnLineCountKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��������
			WORD wHeadSize=sizeof(OnLineCountInfo)-sizeof(OnLineCountInfo.OnLineCountKind);
			WORD wSendSize=wHeadSize+OnLineCountInfo.wKindCount*sizeof(OnLineCountInfo.OnLineCountKind[0]);
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ONLINE_COUNT_INFO,0,&OnLineCountInfo,wSendSize);

			
			return true;
		}
	}

	return false;
}

//Ӧ���¼�
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//��ȡ����
	ASSERT(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//���ñ���
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:			//��¼����
		{
			return OnTCPNetworkMainPCLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainPCServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_USER_SERVICE:	//��������
		{
			return OnTCPNetworkMainPCUserService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_REMOTE_SERVICE:	//Զ�̷���
		{
			return OnTCPNetworkMainPCRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_ANDROID_SERVICE: //��������
		{
			return OnTCPNetworkMainAndroidService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_LOGON:			//��¼����
		{
			return OnTCPNetworkMainMBLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainMBServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//���ݿ��¼�
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{try{
	switch (wRequestID)
	{
	case DBO_GP_USER_ACCOUNTINFO:		//�û�������Ϣ
		{
			return OnDBPCUserAccountInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBPCLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBPCLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_VALIDATE_MBCARD:		//У���ܱ�
		{
			return OnDBPCLogonValidateMBCard(dwContextID,pData,wDataSize);
		}
	case DBO_GP_VALIDATE_PASSPORT:		//У��֤��
		{
			return OnDBPCLogonValidatePassPort(dwContextID,pData,wDataSize);
		}
	case DBO_GP_VERIFY_RESULT:			//��֤���
		{
			return OnDBPCLogonVerifyResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_PLATFORM_PARAMETER:		//ƽ̨����
		{
			return OnDBPCPlatformParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_FACE_INFO:			//�û�ͷ��
		{
			return OnDBPCUserFaceInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INDIVIDUAL:		//�û���Ϣ
		{
			return OnDBPCUserIndividual(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_INFO:		//��������
		{
			return OnDBPCUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_SUCCESS:	//���гɹ�
		{
			return OnDBPCUserInsureSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_FAILURE:	//����ʧ��
		{
			return OnDBPCUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_USER_INFO:  //�û���Ϣ
		{
			return OnDBPCUserInsureUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_MATCH_SIGNUP_RESULT:	//�������
		{
			return OnDBPCUserMatchSignupResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_ENABLE_RESULT: //��ͨ���
		{
			return OnDBPCUserInsureEnableResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ANDROID_PARAMETER:		//��������
		{
			return OnDBAndroidParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_SUCCESS:		//�����ɹ�
		{
			return OnDBPCOperateSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_FAILURE:		//����ʧ��
		{
			return OnDBPCOperateFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_TYPE_ITEM:			//��������
		{
			return OnDBPCGameTypeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_KIND_ITEM:			//��������
		{
			return OnDBPCGameKindItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_NODE_ITEM:			//�ڵ�����
		{
			return OnDBPCGameNodeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_PAGE_ITEM:			//��������
		{
			return OnDBPCGamePageItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_LIST_RESULT:		//���ؽ��
		{
			return OnDBPCGameListResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHECKIN_INFO:			//ǩ����Ϣ
		{
			return OnDBPCUserCheckInInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHECKIN_RESULT:			//ǩ�����
		{
			return OnDBPCUserCheckInResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHECKIN_REWARD:			//ǩ������
		{
			return OnDBPCCheckInReward(dwContextID,pData,wDataSize);
		}
	case DBO_GP_TASK_LIST:				//�����б�
		{
			return OnDBPCTaskList(dwContextID,pData,wDataSize);
		}
	case DBO_GP_TASK_LIST_END:			//�������
		{
			return OnDBPCTaskListEnd(dwContextID,pData,wDataSize);
		}
	case DBO_GP_TASK_INFO:				//������Ϣ
		{
			return OnDBPCTaskInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_TASK_RESULT:			//������
		{
			return OnDBPCTaskResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BASEENSURE_PARAMETER:	//�ͱ�����
		{
			return OnDBPCBaseEnsureParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BASEENSURE_RESULT:		//�ͱ����
		{
			return OnDBPCBaseEnsureResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GROWLEVEL_CONFIG:		//�ȼ�����
		{
			return OnDBPCGrowLevelConfig(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GROWLEVEL_PARAMETER:	//�ȼ�����
		{
			return OnDBPCGrowLevelParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GROWLEVEL_UPGRADE:		//�ȼ�����
		{
			return OnDBPCGrowLevelUpgrade(dwContextID,pData,wDataSize);
		}
	case DBO_GP_MEMBER_PARAMETER:		//��Ա����
		{
			return OnDBPCMemberParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_PURCHASE_RESULT:		//������
		{
			return OnDBPCPurchaseResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_EXCHANGE_RESULT:		//�һ����
		{
			return OnDBPCExChangeResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SPREAD_INFO:			//�ƹ���Ϣ
		{
			return OnDBPCUserSpreadInfo(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBMBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBMBLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOTTERY_CONFIG:			//�齱����
		{
			return OnDBPCLotteryConfig(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOTTERY_USER_INFO:		//�齱��Ϣ
		{
			return OnDBPCLotteryUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SIMPLE_LOTTERY_RESULT:
		{
		 return OnDBPCSimpleLotteryResult(dwContextID, pData, wDataSize);
		}
	case DBO_GP_LOTTERY_RESULT:			//�齱���
		{
			return OnDBPCLotteryResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_USER_GAME_DATA:	//��Ϸ����
		{
			return OnDBPCQueryUserGameData(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INGAME_SERVER_ID:		//�Զ������ݲ�ѯ
		{
			return OnDBPCInGameSevrerID(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SYSTEM_QQ:				// ��ѯϵͳQQ:DCY by 2017.3.9
		{
			return OnDBSystemQQ(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SYSTEM_INFO:			//��ѯϵͳ����:DCY by 2017.3.9
		{
			return OnDBSystemInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SYSTEM_CONTACT:
		{
			return OnDBSystemContact(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SYSTEM_SHOP:			//��ѯ��Ϸ�̳�:DCY by 2017.3.11
		{
			return OnDBGameShop(dwContextID,pData,wDataSize);
		}
	case DBO_GP_EXCHANGE_SHOP:			//�һ���Ϸ��Ʒ:DCY by 2017.3.11
		{
			return OnDBExchangeShop(dwContextID,pData,wDataSize);
		}
	}
	}
	catch(...)
	{
		TCHAR szMessage[128]=TEXT("");
		_sntprintf(szMessage,CountArray(szMessage),TEXT("OnEventDataBase:%d"),wRequestID);
		CTraceService::TraceString(szMessage,TraceLevel_Warning);
	}

	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (m_bNeekCorrespond==true)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Э�������������ӹر��ˣ�%ld �����������"),m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (nErrorCode!=0)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("Э������������ʧ�� [ %ld ]��%ld �����������"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//��ʾ��Ϣ
		CTraceService::TraceString(TEXT("����ע����Ϸ��¼������..."),TraceLevel_Normal);

		//��������
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//���ñ���
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//��������
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

		return true;
	}

	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//ע�����
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//������Ϣ
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_REMOTE_SERVICE:	//Զ�̷���
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //�������
			{
				return OnTCPSocketMainManagerService(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_ROOM_SERVICE:	//�������
			{
				return OnTCPSocketMainRoomService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//ע���¼�
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//ע��ʧ��
		{
			//��������
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//Ч�����
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//�رմ���
			m_bNeekCorrespond=false;
			printf("ע��ʧ�� �ر�����\n");
			m_pITCPSocketService->CloseSocket();

			//��ʾ��Ϣ
			LPCTSTR pszDescribeString=pRegisterFailure->szDescribeString;
			if (lstrlen(pszDescribeString)>0) CTraceService::TraceString(pszDescribeString,TraceLevel_Exception);

			//�¼�֪ͨ
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//�б��¼�
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//������Ϣ
		{
			//�����б�
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//��������
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//���ҷ���
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);
			if(pGameServerItem == NULL) return true;

			//��ȡ����
			tagGameServer * pGameServer=&pGameServerItem->m_GameServer; 

			//��������
			DWORD dwOldOnlineCount=0,dwOldAndroidCount=0;
			dwOldOnlineCount = pGameServer->dwOnLineCount;
			dwOldAndroidCount = pGameServer->dwAndroidCount;

			//��������
			pGameServer->dwAndroidCount=pServerOnLine->dwAndroidCount;
			pGameServer->dwOnLineCount=pServerOnLine->dwOnLineCount;

			//Ŀ¼����
			CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServer->wKindID);
			if (pGameKindItem!=NULL)
			{
				//Ŀ¼����
				pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
				pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;

				//��������
				pGameKindItem->m_GameKind.dwAndroidCount -= dwOldAndroidCount;
				pGameKindItem->m_GameKind.dwAndroidCount += pServerOnLine->dwAndroidCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//�������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//�����޸�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//��������
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//���ҷ���
			ASSERT(m_ServerListManager.SearchGameServer(pServerModify->wServerID));
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//���÷���
			if (pGameServerItem!=NULL)
			{
				//��������
				DWORD dwOldOnlineCount=0,dwOldAndroidCount=0,dwOldFullCount=0;
				dwOldAndroidCount = pGameServerItem->m_GameServer.dwAndroidCount;
				dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;				
				dwOldFullCount   = pGameServerItem->m_GameServer.dwFullCount;

				//�޸ķ�����Ϣ
				pGameServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGameServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				pGameServerItem->m_GameServer.dwAndroidCount=pServerModify->dwAndroidCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));

				//Ŀ¼����
				CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
				if (pGameKindItem!=NULL)
				{
					tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
					pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
					pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;

					pGameKindItem->m_GameKind.dwFullCount -= dwOldFullCount;
					pGameKindItem->m_GameKind.dwFullCount += pGameServer->dwFullCount;

					pGameKindItem->m_GameKind.dwAndroidCount -= dwOldAndroidCount;
					pGameKindItem->m_GameKind.dwAndroidCount += pServerModify->dwAndroidCount;
				}
			}

			return true;
		}
	case SUB_CS_S_SERVER_REMOVE:	//����ɾ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//��������
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//��������
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//�������
		{
			//�����б�
			m_ServerListManager.CleanServerItem();

			//�¼�����
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	case SUB_CS_S_MATCH_INSERT:		//��������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameMatch)==0);
			if (wDataSize%sizeof(tagGameMatch)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameMatch);
			tagGameMatch * pGameMatch=(tagGameMatch *)pData;

			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				CGameServerItem * pGameServerItem = m_ServerListManager.SearchGameServer(pGameMatch->wServerID);
				if(pGameServerItem!=NULL)
				{
					CopyMemory(&pGameServerItem->m_GameMatch,pGameMatch++,sizeof(pGameServerItem->m_GameMatch));
				}
			}

			return true;
		}
	}

	return true;
}

//Զ�̷���
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SEARCH_CORRESPOND:	//Э������
		{
			//��������
			CMD_CS_S_SearchCorrespond * pSearchCorrespond=(CMD_CS_S_SearchCorrespond *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_CS_S_SearchCorrespond));
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
			ASSERT(wDataSize==(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

			//Ч�����
			if (wDataSize>sizeof(CMD_CS_S_SearchCorrespond)) return false;
			if (wDataSize<(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
			if (wDataSize!=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

			//�ж�����
			ASSERT(LOWORD(pSearchCorrespond->dwSocketID)<m_pInitParameter->m_wMaxConnect);
			if ((m_pBindParameter+LOWORD(pSearchCorrespond->dwSocketID))->dwSocketID!=pSearchCorrespond->dwSocketID) return true;

			//��������
			CMD_GP_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ñ���
			for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
			{
				//����Ч��
				ASSERT(SearchCorrespond.wUserCount<CountArray(SearchCorrespond.UserRemoteInfo));
				if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

				//���ñ���
				WORD wIndex=SearchCorrespond.wUserCount++;
				CopyMemory(&SearchCorrespond.UserRemoteInfo[wIndex],&pSearchCorrespond->UserRemoteInfo[i],sizeof(SearchCorrespond.UserRemoteInfo[wIndex]));
			}

			//��������
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(pSearchCorrespond->dwSocketID,MDM_GP_REMOTE_SERVICE,SUB_GP_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	}

	return true;
}

//�������
bool CAttemperEngineSink::OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_PLATFORM_PARAMETER:	//ƽ̨����
		{
			//�������
			m_wTaskCountBuffer = 0;
			ZeroMemory(m_TaskParameterBuffer, sizeof(m_TaskParameterBuffer));

			//ƽ̨����
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_PLATFORM_PARAMETER,0,NULL,0);		

			//��������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_TASK_LIST, 0, NULL, 0);

			//��Ա����
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_MEMBER_PARAMETER,0,NULL,0);	

			//ǩ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_CHECKIN_REWARD, 0, NULL, 0);			

			//���صͱ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_BASEENSURE, 0, NULL, 0);	

			return true;
		}
	}

	return true;
}

//�����б�
bool CAttemperEngineSink::OnTCPSocketMainRoomService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_QUERYROOMLIST:
		{
			CMD_CS_S_QueryRoomList_Result recvRoomResult;
			datastream recvData;
			recvRoomResult.StreamValue(recvData,false);

			DWORD dwClientSocketID = recvRoomResult.dwClientSocketID;
			
			//��������
			CMD_GP_QueryRoomList_Result sendRoomResult;
			sendRoomResult.roomList.swap(recvRoomResult.roomList);

			datastream sendData;
			sendRoomResult.StreamValue(sendData,true);
			m_pITCPNetworkEngine->SendData(dwClientSocketID,MDM_GP_USER_SERVICE, SUB_GP_QUERY_ROOM_LIST,sendData.data(),sendData.size());
		}
	}

	return true;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D ��¼
		{
			return OnTCPNetworkSubPCLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubPCLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_MANAGETOOL:	//���ߵ�¼
		{
			return OnTCPNetworkSubPCLogonManageTool(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubPCRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_VERIFY_INDIVIDUAL:	//У������
		{
			return OnTCPNetworkSubPCVerifyIndividual(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_VISITOR:      //�ο͵�¼
		{
			return OnTCPNetworkSubPCLogonVisitor(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_QUERY_VERIFY_CODE:	//��ѯ��֤��
		{
			return OnTCPNetworkSubPCQueryVerifyCode(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//�б���
bool CAttemperEngineSink::OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_LIST:			//��ȡ�б�
		{
			//�����б�
			SendGameTypeInfo(dwSocketID);
			SendGameKindInfo(dwSocketID);

			//�����б�
			if (m_pInitParameter->m_cbDelayList==FALSE)
			{
				//�����б�
				SendGamePageInfo(dwSocketID,INVALID_WORD);
				SendGameNodeInfo(dwSocketID,INVALID_WORD);
				SendGameServerInfo(dwSocketID,INVALID_WORD);				
			}
			else
			{
				//����ҳ��
				SendGamePageInfo(dwSocketID,0);
			}

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);

			return true;
		}
	case SUB_GP_GET_SERVER:			//��ȡ����
		{
			//Ч������
			ASSERT(wDataSize%sizeof(WORD)==0);
			if (wDataSize%sizeof(WORD)!=0) return false;

			//�����б�
			UINT nKindCount=wDataSize/sizeof(WORD);
			for (UINT i=0;i<nKindCount;i++)
			{
				SendGameNodeInfo(dwSocketID,((WORD *)pData)[i]);
				SendGamePageInfo(dwSocketID,((WORD *)pData)[i]);
				SendGameServerInfo(dwSocketID,((WORD *)pData)[i]);
			}

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_SERVER_FINISH,pData,wDataSize);

			return true;
		}
	case SUB_GP_GET_MATCH:			//��ȡ����
		{
			//�����б�
			SendGameServerInfo(dwSocketID,INVALID_WORD,true);

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_MATCH_FINISH,pData,wDataSize);

			return true;
		}
	case SUB_GP_GET_ONLINE:			//��ȡ����
		{
			//��������
			CMD_GP_GetOnline * pGetOnline=(CMD_GP_GetOnline *)pData;
			WORD wHeadSize=(sizeof(CMD_GP_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

			//��������
			CMD_GP_KindOnline KindOnline;
			CMD_GP_ServerOnline ServerOnline;
			ZeroMemory(&KindOnline,sizeof(KindOnline));
			ZeroMemory(&ServerOnline,sizeof(ServerOnline));

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=KindOnline.wKindCount++;
					KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount+pGameKindItem->m_GameKind.dwAndroidCount;
				}

				//����ж�
				if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��ȡ����
			for (WORD i=0;i<pGetOnline->wServerCount;i++)
			{
				//��ȡ����
				WORD wServerID=pGetOnline->wOnLineServerID[i];
				CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

				//���ñ���
				if (pGameServerItem!=NULL)
				{
					WORD wServerIndex=ServerOnline.wServerCount++;
					ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
					ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount+pGameServerItem->m_GameServer.dwAndroidCount;
				}

				//����ж�
				if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
				{
					ASSERT(FALSE);
					break;
				}
			}

			//��������
			if (KindOnline.wKindCount>0)
			{
				WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
				WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_KINE_ONLINE,&KindOnline,wSendSize);
			}

			//��������
			if (ServerOnline.wServerCount>0)
			{
				WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
				WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_SERVER_ONLINE,&ServerOnline,wSendSize);
			}

			return true;
		}
	case SUB_GP_GET_COLLECTION:		//��ȡ�ղ�
		{
			return true;
		}
	}

	return false;
}

//������
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_QUERY_ACCOUNTINFO:	//��ѯ������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryAccountInfo));
			if (wDataSize!=sizeof(CMD_GP_QueryAccountInfo)) return false;

			//������Ϣ
			CMD_GP_QueryAccountInfo * pQueryIndividual=(CMD_GP_QueryAccountInfo *)pData;

			//��������
			DBR_GP_QueryAccountInfo QueryIndividual;
			ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

			//��������
			QueryIndividual.dwUserID=pQueryIndividual->dwUserID;
			QueryIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_ACCOUNTINFO,dwSocketID,&QueryIndividual,sizeof(QueryIndividual));

			return true;
		}
	case SUB_GP_QUERY_ROOM_LIST:
		{
			ASSERT(wDataSize== sizeof(CMD_GP_QueryRoomList));
			if(wDataSize!=sizeof(CMD_GP_QueryRoomList)) return false;
	
			//������Ϣ
			CMD_GP_QueryRoomList *pUser=(CMD_GP_QueryRoomList*)pData;
	 
			CMD_CS_C_QueryRoomList kNetData;
			kNetData.dwUserID = pUser->dwUserID;
			kNetData.dwSocketID = dwSocketID;

			m_pITCPSocketService->SendData(MDM_CS_ROOM_SERVICE,SUB_CS_C_QUERYROOMLIST,&kNetData,sizeof(kNetData));
			return true;
		}
	case SUB_GP_MODIFY_MACHINE:			  //�󶨻���
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyMachine));
			if (wDataSize!=sizeof(CMD_GP_ModifyMachine)) return false;

			//������Ϣ
			CMD_GP_ModifyMachine * pModifyMachine=(CMD_GP_ModifyMachine *)pData;
			pModifyMachine->szPassword[CountArray(pModifyMachine->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyMachine ModifyMachine;
			ZeroMemory(&ModifyMachine,sizeof(ModifyMachine));

			//��������
			ModifyMachine.cbBind=pModifyMachine->cbBind;
			ModifyMachine.dwUserID=pModifyMachine->dwUserID;
			ModifyMachine.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyMachine.szPassword,pModifyMachine->szPassword,CountArray(ModifyMachine.szPassword));
			lstrcpyn(ModifyMachine.szMachineID,pModifyMachine->szMachineID,CountArray(ModifyMachine.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_MACHINE,dwSocketID,&ModifyMachine,sizeof(ModifyMachine));

			return true;
		}
	
	case SUB_GP_MODIFY_LOGON_PASS:		  //�޸�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyLogonPass));
			if (wDataSize!=sizeof(CMD_GP_ModifyLogonPass)) return false;

			//������Ϣ
			CMD_GP_ModifyLogonPass * pModifyLogonPass=(CMD_GP_ModifyLogonPass *)pData;
			pModifyLogonPass->szDesPassword[CountArray(pModifyLogonPass->szDesPassword)-1]=0;
			pModifyLogonPass->szScrPassword[CountArray(pModifyLogonPass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyLogonPass ModifyLogonPass;
			ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

			//��������
			ModifyLogonPass.dwUserID=pModifyLogonPass->dwUserID;
			ModifyLogonPass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyLogonPass.szDesPassword,pModifyLogonPass->szDesPassword,CountArray(ModifyLogonPass.szDesPassword));
			lstrcpyn(ModifyLogonPass.szScrPassword,pModifyLogonPass->szScrPassword,CountArray(ModifyLogonPass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_LOGON_PASS,dwSocketID,&ModifyLogonPass,sizeof(ModifyLogonPass));

			return true;
		}
	case SUB_GP_MODIFY_INSURE_PASS:		  //�޸�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyInsurePass));
			if (wDataSize!=sizeof(CMD_GP_ModifyInsurePass)) return false;

			//������Ϣ
			CMD_GP_ModifyInsurePass * pModifyInsurePass=(CMD_GP_ModifyInsurePass *)pData;
			pModifyInsurePass->szDesPassword[CountArray(pModifyInsurePass->szDesPassword)-1]=0;
			pModifyInsurePass->szScrPassword[CountArray(pModifyInsurePass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyInsurePass ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//��������
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szDesPassword,pModifyInsurePass->szDesPassword,CountArray(ModifyInsurePass.szDesPassword));
			lstrcpyn(ModifyInsurePass.szScrPassword,pModifyInsurePass->szScrPassword,CountArray(ModifyInsurePass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INSURE_PASS,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
	case SUB_GP_MODIFY_UNDER_WRITE:		  //�޸�ǩ��
		{
			//��������
			CMD_GP_ModifyUnderWrite * pModifyUnderWrite=(CMD_GP_ModifyUnderWrite *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_GP_ModifyUnderWrite));
			ASSERT(wDataSize>=(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)));
			ASSERT(wDataSize==(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)+CountStringBuffer(pModifyUnderWrite->szUnderWrite)));

			//Ч�����
			if (wDataSize>sizeof(CMD_GP_ModifyUnderWrite)) return false;
			if (wDataSize<(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite))) return false;
			if (wDataSize!=(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)+CountStringBuffer(pModifyUnderWrite->szUnderWrite))) return false;

			//������Ϣ
			pModifyUnderWrite->szPassword[CountArray(pModifyUnderWrite->szPassword)-1]=0;
			pModifyUnderWrite->szUnderWrite[CountArray(pModifyUnderWrite->szUnderWrite)-1]=0;

			//��������
			DBR_GP_ModifyUnderWrite ModifyUnderWrite;
			ZeroMemory(&ModifyUnderWrite,sizeof(ModifyUnderWrite));

			//��������
			ModifyUnderWrite.dwUserID=pModifyUnderWrite->dwUserID;
			ModifyUnderWrite.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyUnderWrite.szPassword,pModifyUnderWrite->szPassword,CountArray(ModifyUnderWrite.szPassword));
			lstrcpyn(ModifyUnderWrite.szUnderWrite,pModifyUnderWrite->szUnderWrite,CountArray(ModifyUnderWrite.szUnderWrite));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_UNDER_WRITE,dwSocketID,&ModifyUnderWrite,sizeof(ModifyUnderWrite));

			return true;
		}
	case SUB_GP_SYSTEM_FACE_INFO:		  //�޸�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_SystemFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_SystemFaceInfo)) return false;

			//������Ϣ
			CMD_GP_SystemFaceInfo * pSystemFaceInfo=(CMD_GP_SystemFaceInfo *)pData;

			//��������
			DBR_GP_ModifySystemFace ModifySystemFace;
			ZeroMemory(&ModifySystemFace,sizeof(ModifySystemFace));

			//��������
			ModifySystemFace.wFaceID=pSystemFaceInfo->wFaceID;
			ModifySystemFace.dwUserID=pSystemFaceInfo->dwUserID;
			ModifySystemFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifySystemFace.szPassword,pSystemFaceInfo->szPassword,CountArray(ModifySystemFace.szPassword));
			lstrcpyn(ModifySystemFace.szMachineID,pSystemFaceInfo->szMachineID,CountArray(ModifySystemFace.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_SYSTEM_FACE,dwSocketID,&ModifySystemFace,sizeof(ModifySystemFace));

			return true;
		}
	case SUB_GP_CUSTOM_FACE_INFO:		  //�޸�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_CustomFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_CustomFaceInfo)) return false;

			//������Ϣ
			CMD_GP_CustomFaceInfo * pCustomFaceInfo=(CMD_GP_CustomFaceInfo *)pData;

			//��������
			DBR_GP_ModifyCustomFace ModifyCustomFace;
			ZeroMemory(&ModifyCustomFace,sizeof(ModifyCustomFace));

			//��������
			ModifyCustomFace.dwUserID=pCustomFaceInfo->dwUserID;
			ModifyCustomFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyCustomFace.szPassword,pCustomFaceInfo->szPassword,CountArray(ModifyCustomFace.szPassword));
			lstrcpyn(ModifyCustomFace.szMachineID,pCustomFaceInfo->szMachineID,CountArray(ModifyCustomFace.szMachineID));
			CopyMemory(ModifyCustomFace.dwCustomFace,pCustomFaceInfo->dwCustomFace,sizeof(ModifyCustomFace.dwCustomFace));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_CUSTOM_FACE,dwSocketID,&ModifyCustomFace,sizeof(ModifyCustomFace));

			return true;
		}
	case SUB_GP_QUERY_INDIVIDUAL:		  //��ѯ��Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryIndividual));
			if (wDataSize!=sizeof(CMD_GP_QueryIndividual)) return false;

			//������Ϣ
			CMD_GP_QueryIndividual * pQueryIndividual=(CMD_GP_QueryIndividual *)pData;

			//��������
			DBR_GP_QueryIndividual QueryIndividual;
			ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

			//��������
			QueryIndividual.dwUserID=pQueryIndividual->dwUserID;
			QueryIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(QueryIndividual.szPassword,pQueryIndividual->szPassword,CountArray(QueryIndividual.szPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INDIVIDUAL,dwSocketID,&QueryIndividual,sizeof(QueryIndividual));

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL:		  //�޸�����
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_ModifyIndividual));
			if (wDataSize<sizeof(CMD_GP_ModifyIndividual)) return false;

			//������Ϣ
			CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szPassword[CountArray(pModifyIndividual->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyIndividual ModifyIndividual;
			ZeroMemory(&ModifyIndividual,sizeof(ModifyIndividual));

			//���ñ���
			ModifyIndividual.dwUserID=pModifyIndividual->dwUserID;
			ModifyIndividual.cbGender=pModifyIndividual->cbGender;
			ModifyIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyIndividual.szPassword,pModifyIndividual->szPassword,CountArray(ModifyIndividual.szPassword));

			//��������
			VOID * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pModifyIndividual+1,wDataSize-sizeof(CMD_GP_ModifyIndividual));

			//��չ��Ϣ
			while (true)
			{
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_GP_UI_NICKNAME:		//�û��ǳ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szNickName));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szNickName))
						{
							CopyMemory(&ModifyIndividual.szNickName,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szNickName[CountArray(ModifyIndividual.szNickName)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_UNDER_WRITE:		//����ǩ��
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUnderWrite));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUnderWrite))
						{
							CopyMemory(&ModifyIndividual.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUnderWrite[CountArray(ModifyIndividual.szUnderWrite)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_USER_NOTE:		//�û���ע
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserNote));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserNote))
						{
							CopyMemory(&ModifyIndividual.szUserNote,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUserNote[CountArray(ModifyIndividual.szUserNote)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_COMPELLATION:	//��ʵ����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szCompellation));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szCompellation))
						{
							CopyMemory(&ModifyIndividual.szCompellation,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szCompellation[CountArray(ModifyIndividual.szCompellation)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_SEAT_PHONE:		//�̶��绰
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szSeatPhone));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szSeatPhone))
						{
							CopyMemory(ModifyIndividual.szSeatPhone,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szSeatPhone[CountArray(ModifyIndividual.szSeatPhone)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_MOBILE_PHONE:	//�ƶ��绰
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szMobilePhone));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szMobilePhone))
						{
							CopyMemory(ModifyIndividual.szMobilePhone,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szMobilePhone[CountArray(ModifyIndividual.szMobilePhone)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_QQ:				//Q Q ����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szQQ));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szQQ))
						{
							CopyMemory(ModifyIndividual.szQQ,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szQQ[CountArray(ModifyIndividual.szQQ)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_EMAIL:			//�����ʼ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szEMail));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szEMail))
						{
							CopyMemory(ModifyIndividual.szEMail,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szEMail[CountArray(ModifyIndividual.szEMail)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_DWELLING_PLACE:	//��ϵ��ַ
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szDwellingPlace));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szDwellingPlace))
						{
							CopyMemory(ModifyIndividual.szDwellingPlace,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szDwellingPlace[CountArray(ModifyIndividual.szDwellingPlace)-1]=0;
						}
						break;
					}
				}
			}

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL,dwSocketID,&ModifyIndividual,sizeof(ModifyIndividual));

			return true;
		}
	case SUB_GP_USER_ENABLE_INSURE:		  //��ͨ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserEnableInsure));
			if (wDataSize!=sizeof(CMD_GP_UserEnableInsure)) return false;

			//������Ϣ
			CMD_GP_UserEnableInsure * pUserEnableInsure=(CMD_GP_UserEnableInsure *)pData;
			pUserEnableInsure->szLogonPass[CountArray(pUserEnableInsure->szLogonPass)-1]=0;
			pUserEnableInsure->szInsurePass[CountArray(pUserEnableInsure->szInsurePass)-1]=0;
			pUserEnableInsure->szMachineID[CountArray(pUserEnableInsure->szMachineID)-1]=0;

			//��������
			DBR_GP_UserEnableInsure UserEnableInsure;
			ZeroMemory(&UserEnableInsure,sizeof(UserEnableInsure));

			//��������
			UserEnableInsure.dwUserID=pUserEnableInsure->dwUserID;			
			UserEnableInsure.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserEnableInsure.szLogonPass,pUserEnableInsure->szLogonPass,CountArray(UserEnableInsure.szLogonPass));
			lstrcpyn(UserEnableInsure.szInsurePass,pUserEnableInsure->szInsurePass,CountArray(UserEnableInsure.szInsurePass));
			lstrcpyn(UserEnableInsure.szMachineID,pUserEnableInsure->szMachineID,CountArray(UserEnableInsure.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_ENABLE_INSURE,dwSocketID,&UserEnableInsure,sizeof(UserEnableInsure));

			return true;
		}
	case SUB_GP_USER_SAVE_SCORE:		  //������Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserSaveScore));
			if (wDataSize!=sizeof(CMD_GP_UserSaveScore)) return false;

			//������Ϣ
			CMD_GP_UserSaveScore * pUserSaveScore=(CMD_GP_UserSaveScore *)pData;
			pUserSaveScore->szMachineID[CountArray(pUserSaveScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserSaveScore UserSaveScore;
			ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

			//��������
			UserSaveScore.dwUserID=pUserSaveScore->dwUserID;
			UserSaveScore.lSaveScore=pUserSaveScore->lSaveScore;
			UserSaveScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserSaveScore.szMachineID,pUserSaveScore->szMachineID,CountArray(UserSaveScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

			return true;
		}
	case SUB_GP_USER_TAKE_SCORE:		  //ȡ����Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTakeScore));
			if (wDataSize!=sizeof(CMD_GP_UserTakeScore)) return false;

			//������Ϣ
			CMD_GP_UserTakeScore * pUserTakeScore=(CMD_GP_UserTakeScore *)pData;
			pUserTakeScore->szPassword[CountArray(pUserTakeScore->szPassword)-1]=0;
			pUserTakeScore->szMachineID[CountArray(pUserTakeScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTakeScore UserTakeScore;
			ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

			//��������
			UserTakeScore.dwUserID=pUserTakeScore->dwUserID;
			UserTakeScore.lTakeScore=pUserTakeScore->lTakeScore;
			UserTakeScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTakeScore.szPassword,pUserTakeScore->szPassword,CountArray(UserTakeScore.szPassword));
			lstrcpyn(UserTakeScore.szMachineID,pUserTakeScore->szMachineID,CountArray(UserTakeScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	case SUB_GP_USER_TRANSFER_SCORE:	  //ת����Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTransferScore));
			if (wDataSize!=sizeof(CMD_GP_UserTransferScore)) return false;

			//������Ϣ
			CMD_GP_UserTransferScore * pUserTransferScore=(CMD_GP_UserTransferScore *)pData;
			pUserTransferScore->szAccounts[CountArray(pUserTransferScore->szAccounts)-1]=0;
			pUserTransferScore->szPassword[CountArray(pUserTransferScore->szPassword)-1]=0;
			pUserTransferScore->szMachineID[CountArray(pUserTransferScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTransferScore UserTransferScore;
			ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

			//��������
			UserTransferScore.dwUserID=pUserTransferScore->dwUserID;
			UserTransferScore.lTransferScore=pUserTransferScore->lTransferScore;
			UserTransferScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTransferScore.szAccounts,pUserTransferScore->szAccounts,CountArray(UserTransferScore.szAccounts));
			lstrcpyn(UserTransferScore.szPassword,pUserTransferScore->szPassword,CountArray(UserTransferScore.szPassword));
			lstrcpyn(UserTransferScore.szMachineID,pUserTransferScore->szMachineID,CountArray(UserTransferScore.szMachineID));
			lstrcpyn(UserTransferScore.szTransRemark,pUserTransferScore->szTransRemark,CountArray(UserTransferScore.szTransRemark));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

			return true;
		}
	case SUB_GP_QUERY_INSURE_INFO:		  //��ѯ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryInsureInfo));
			if (wDataSize!=sizeof(CMD_GP_QueryInsureInfo)) return false;

			//������Ϣ
			CMD_GP_QueryInsureInfo * pQueryInsureInfo=(CMD_GP_QueryInsureInfo *)pData;

			//��������
			DBR_GP_QueryInsureInfo QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//��������
			QueryInsureInfo.dwUserID=pQueryInsureInfo->dwUserID;
			QueryInsureInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(QueryInsureInfo.szPassword,pQueryInsureInfo->szPassword,CountArray(QueryInsureInfo.szPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
	case SUB_GP_QUERY_USER_INFO_REQUEST:  //��ѯ�û�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryUserInfoRequest));
			if (wDataSize!=sizeof(CMD_GP_QueryUserInfoRequest)) return false;

			//������Ϣ
			CMD_GP_QueryUserInfoRequest * pQueryUserInfoRequest=(CMD_GP_QueryUserInfoRequest *)pData;

			//ID�ж�
			if(pQueryUserInfoRequest->cbByNickName==FALSE)
			{
				//�����ж�
				int nLen=lstrlen(pQueryUserInfoRequest->szAccounts);
				if(nLen>=8)
				{
					SendInsureFailure(dwSocketID,0,TEXT("������Ϸ������ID��"));
					return true;
				}

				//�Ϸ��ж�
				for(int i=0; i<nLen; i++)
				{
					if(pQueryUserInfoRequest->szAccounts[i] < TEXT('0') || pQueryUserInfoRequest->szAccounts[i] > TEXT('9'))
					{
						SendInsureFailure(dwSocketID,0,TEXT("������Ϸ������ID��"));
						return true;
					}
				}
			}

			//��������
			DBR_GP_QueryInsureUserInfo QueryInsureUserInfo;
			ZeroMemory(&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			//��������
			QueryInsureUserInfo.cbByNickName=pQueryUserInfoRequest->cbByNickName;
			lstrcpyn(QueryInsureUserInfo.szAccounts,pQueryUserInfoRequest->szAccounts,CountArray(QueryInsureUserInfo.szAccounts));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_USER_INFO,dwSocketID,&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			return true;
		}
	case  SUB_GP_MATCH_SIGNUP:			//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_MatchSignup));
			if (wDataSize!=sizeof(CMD_GP_MatchSignup)) return false;

			//������Ϣ
			CMD_GP_MatchSignup * pMatchSignup=(CMD_GP_MatchSignup *)pData;
			pMatchSignup->szPassword[CountArray(pMatchSignup->szPassword)-1]=0;
			pMatchSignup->szMachineID[CountArray(pMatchSignup->szMachineID)-1]=0;		

			//���ҷ���
			CGameServerItem * pGameServerItem = m_ServerListManager.SearchGameServer(pMatchSignup->wServerID);
			if(pGameServerItem==NULL || pGameServerItem->IsMatchServer()==false)
			{
				//����ʧ��
				SendOperateFailure(dwSocketID, 0,TEXT("��Ǹ���������ı��������ڻ����Ѿ�������"));
				return true;
			}

			//����ṹ
			DBR_GP_MatchSignup MatchSignup;

			//������Ϣ
			MatchSignup.wServerID = pMatchSignup->wServerID;
			MatchSignup.dwMatchID = pMatchSignup->dwMatchID;
			MatchSignup.dwMatchNO = pMatchSignup->dwMatchNO;
			
			//�û���Ϣ
			MatchSignup.dwUserID = pMatchSignup->dwUserID;
			lstrcpyn(MatchSignup.szPassword,pMatchSignup->szPassword,CountArray(MatchSignup.szPassword));

			//������Ϣ
			MatchSignup.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(MatchSignup.szMachineID,pMatchSignup->szMachineID,CountArray(MatchSignup.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MATCH_SIGNUP,dwSocketID,&MatchSignup,sizeof(MatchSignup));

			return true;
		}
	case SUB_GP_MATCH_UNSIGNUP:			//ȡ������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_MatchUnSignup));
			if (wDataSize!=sizeof(CMD_GP_MatchUnSignup)) return false;

			//������Ϣ
			CMD_GP_MatchUnSignup * pMatchUnSignup=(CMD_GP_MatchUnSignup *)pData;
			pMatchUnSignup->szPassword[CountArray(pMatchUnSignup->szPassword)-1]=0;
			pMatchUnSignup->szMachineID[CountArray(pMatchUnSignup->szMachineID)-1]=0;			

			//����ṹ
			DBR_GP_MatchUnSignup MatchUnSignup;

			//������Ϣ
			MatchUnSignup.wServerID = pMatchUnSignup->wServerID;
			MatchUnSignup.dwMatchID = pMatchUnSignup->dwMatchID;
			MatchUnSignup.dwMatchNO = pMatchUnSignup->dwMatchNO;

			//�û���Ϣ
			MatchUnSignup.dwUserID = pMatchUnSignup->dwUserID;
			lstrcpyn(MatchUnSignup.szPassword,pMatchUnSignup->szPassword,CountArray(MatchUnSignup.szPassword));

			//������Ϣ
			MatchUnSignup.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(MatchUnSignup.szMachineID,pMatchUnSignup->szMachineID,CountArray(MatchUnSignup.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MATCH_UNSIGNUP,dwSocketID,&MatchUnSignup,sizeof(MatchUnSignup));

			return true;
		}
	case SUB_GP_CHECKIN_QUERY:		  //��ѯǩ��
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_CheckInQueryInfo));
			if(wDataSize!=sizeof(CMD_GP_CheckInQueryInfo)) return false;

			//��ȡ����
			CMD_GP_CheckInQueryInfo *pCheckInQueryInfo = (CMD_GP_CheckInQueryInfo *)pData;
			pCheckInQueryInfo->szPassword[CountArray(pCheckInQueryInfo->szPassword)-1]=0;

			//����ṹ
			DBR_GP_CheckInQueryInfo CheckInQueryInfo;
			CheckInQueryInfo.dwUserID = pCheckInQueryInfo->dwUserID;
			lstrcpyn(CheckInQueryInfo.szPassword,pCheckInQueryInfo->szPassword,CountArray(CheckInQueryInfo.szPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHECKIN_QUERY_INFO,dwSocketID,&CheckInQueryInfo,sizeof(CheckInQueryInfo));

			return true;
		}
	case SUB_GP_CHECKIN_DONE:			  //ִ��ǩ��
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_CheckInDone));
			if(wDataSize!=sizeof(CMD_GP_CheckInDone)) return false;

			//��ȡ����
			CMD_GP_CheckInDone *pCheckInDone = (CMD_GP_CheckInDone *)pData;
			pCheckInDone->szPassword[CountArray(pCheckInDone->szPassword)-1]=0;
			pCheckInDone->szMachineID[CountArray(pCheckInDone->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_CheckInDone CheckInDone;
			CheckInDone.dwUserID = pCheckInDone->dwUserID;
			CheckInDone.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(CheckInDone.szPassword,pCheckInDone->szPassword,CountArray(CheckInDone.szPassword));
			lstrcpyn(CheckInDone.szMachineID,pCheckInDone->szMachineID,CountArray(CheckInDone.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHECKIN_DONE,dwSocketID,&CheckInDone,sizeof(CheckInDone));

			return true;
		}
	case SUB_GP_TASK_LOAD:				//��������
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_TaskLoadInfo));
			if(wDataSize!=sizeof(CMD_GP_TaskLoadInfo)) return false;

			//��ȡ����
			CMD_GP_TaskLoadInfo * pTaskLoadInfo = (CMD_GP_TaskLoadInfo *)pData;
			pTaskLoadInfo->szPassword[CountArray(pTaskLoadInfo->szPassword)-1]=0;

			//����ṹ
			DBR_GP_TaskQueryInfo TaskQueryInfo;
			TaskQueryInfo.dwUserID = pTaskLoadInfo->dwUserID;
			lstrcpyn(TaskQueryInfo.szPassword,pTaskLoadInfo->szPassword,CountArray(TaskQueryInfo.szPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TASK_QUERY_INFO,dwSocketID,&TaskQueryInfo,sizeof(TaskQueryInfo));

			return true;
		}
	case SUB_GP_TASK_TAKE:			      //��ȡ����
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_TaskTake));
			if(wDataSize!=sizeof(CMD_GP_TaskTake)) return false;

			//��ȡ����
			CMD_GP_TaskTake * pTaskTake = (CMD_GP_TaskTake *)pData;
			pTaskTake->szPassword[CountArray(pTaskTake->szPassword)-1]=0;
			pTaskTake->szMachineID[CountArray(pTaskTake->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_TaskTake TaskTake;
			TaskTake.dwUserID = pTaskTake->dwUserID;
			TaskTake.wTaskID = pTaskTake->wTaskID;
			TaskTake.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(TaskTake.szPassword,pTaskTake->szPassword,CountArray(TaskTake.szPassword));
			lstrcpyn(TaskTake.szMachineID,pTaskTake->szMachineID,CountArray(TaskTake.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TASK_TAKE,dwSocketID,&TaskTake,sizeof(TaskTake));

			return true;
		}
	case SUB_GP_TASK_REWARD:			  //��ȡ����
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_TaskReward));
			if(wDataSize!=sizeof(CMD_GP_TaskReward)) return false;

			//��ȡ����
			CMD_GP_TaskReward * pTaskReward = (CMD_GP_TaskReward *)pData;
			pTaskReward->szPassword[CountArray(pTaskReward->szPassword)-1]=0;
			pTaskReward->szMachineID[CountArray(pTaskReward->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_TaskReward TaskReward;
			TaskReward.dwUserID = pTaskReward->dwUserID;
			TaskReward.wTaskID = pTaskReward->wTaskID;
			TaskReward.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(TaskReward.szPassword,pTaskReward->szPassword,CountArray(TaskReward.szPassword));
			lstrcpyn(TaskReward.szMachineID,pTaskReward->szMachineID,CountArray(TaskReward.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TASK_REWARD,dwSocketID,&TaskReward,sizeof(TaskReward));

			return true;
		}
	case SUB_GP_BASEENSURE_LOAD:		//���صͱ�
		{
			//����ṹ
			CMD_GP_BaseEnsureParamter BaseEnsureParameter;
			BaseEnsureParameter.cbTakeTimes=m_BaseEnsureParameter.cbTakeTimes;
			BaseEnsureParameter.lScoreAmount=m_BaseEnsureParameter.lScoreAmount;
			BaseEnsureParameter.lScoreCondition=m_BaseEnsureParameter.lScoreCondition;

			//Ͷ������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_PARAMETER,&BaseEnsureParameter,sizeof(BaseEnsureParameter));

			return true;
		}
	case SUB_GP_BASEENSURE_TAKE:		  //��ȡ�ͱ�
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_BaseEnsureTake));
			if(wDataSize!=sizeof(CMD_GP_BaseEnsureTake)) return false;

			//��ȡ����
			CMD_GP_BaseEnsureTake * pBaseEnsureTake = (CMD_GP_BaseEnsureTake *)pData;
			pBaseEnsureTake->szPassword[CountArray(pBaseEnsureTake->szPassword)-1]=0;
			pBaseEnsureTake->szMachineID[CountArray(pBaseEnsureTake->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_TakeBaseEnsure TakeBaseEnsure;
			TakeBaseEnsure.dwUserID = pBaseEnsureTake->dwUserID;
			TakeBaseEnsure.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(TakeBaseEnsure.szPassword,pBaseEnsureTake->szPassword,CountArray(TakeBaseEnsure.szPassword));
			lstrcpyn(TakeBaseEnsure.szMachineID,pBaseEnsureTake->szMachineID,CountArray(TakeBaseEnsure.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BASEENSURE_TAKE,dwSocketID,&TakeBaseEnsure,sizeof(TakeBaseEnsure));

			return true;
		}
	case SUB_GP_SPREAD_QUERY:		//�ƹ����
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_UserSpreadQuery));
			if(wDataSize!=sizeof(CMD_GP_UserSpreadQuery)) return false;

			//��ȡ����
			CMD_GP_UserSpreadQuery * pUserSpreadQuery=(CMD_GP_UserSpreadQuery*)pData;

			//����ṹ
			DBR_GP_QuerySpreadInfo QuerySpreadInfo;
			QuerySpreadInfo.dwUserID=pUserSpreadQuery->dwUserID;

			//�ƹ㽱��
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_SPREAD_INFO,dwSocketID,&QuerySpreadInfo,sizeof(QuerySpreadInfo));

			return true;
		}
	case SUB_GP_GROWLEVEL_QUERY:		  //��ѯ�ȼ�
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_GrowLevelQueryInfo));
			if(wDataSize!=sizeof(CMD_GP_GrowLevelQueryInfo)) return false;

			//��ȡ����
			CMD_GP_GrowLevelQueryInfo * pGrowLevelQueryInfo= (CMD_GP_GrowLevelQueryInfo *)pData;
			pGrowLevelQueryInfo->szPassword[CountArray(pGrowLevelQueryInfo->szPassword)-1]=0;
			pGrowLevelQueryInfo->szMachineID[CountArray(pGrowLevelQueryInfo->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_GrowLevelQueryInfo GrowLevelQueryInfo;
			GrowLevelQueryInfo.dwUserID = pGrowLevelQueryInfo->dwUserID;
			GrowLevelQueryInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(GrowLevelQueryInfo.szPassword,pGrowLevelQueryInfo->szPassword,CountArray(GrowLevelQueryInfo.szPassword));
			lstrcpyn(GrowLevelQueryInfo.szMachineID,pGrowLevelQueryInfo->szMachineID,CountArray(GrowLevelQueryInfo.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GROWLEVEL_QUERY_IFNO,dwSocketID,&GrowLevelQueryInfo,sizeof(GrowLevelQueryInfo));

			return true;
		}
	case SUB_GP_EXCHANGE_QUERY:		//�һ�����
		{
			//����ṹ
			CMD_GP_ExchangeParameter ExchangeParameter;
			ZeroMemory(&ExchangeParameter,sizeof(ExchangeParameter));

			//���ñ���
			ExchangeParameter.dwExchangeRate=m_PlatformParameter.dwExchangeRate;
			ExchangeParameter.wMemberCount=m_wMemberCount;
			ExchangeParameter.dwPresentExchangeRate=m_PlatformParameter.dwPresentExchangeRate;
			ExchangeParameter.dwRateGold=m_PlatformParameter.dwRateGold;
			CopyMemory(ExchangeParameter.MemberParameter,m_MemberParameter,sizeof(tagMemberParameter)*m_wMemberCount);

			//�����С
			WORD wSendDataSize = sizeof(ExchangeParameter)-sizeof(ExchangeParameter.MemberParameter);
			wSendDataSize += sizeof(tagMemberParameter)*ExchangeParameter.wMemberCount;

			//��������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_EXCHANGE_PARAMETER,&ExchangeParameter,wSendDataSize);

			return true;
		}
	case SUB_GP_PURCHASE_MEMBER:	//�����Ա
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_PurchaseMember));
			if(wDataSize!=sizeof(CMD_GP_PurchaseMember)) return false;

			//����ṹ
			CMD_GP_PurchaseMember * pPurchaseMember = (CMD_GP_PurchaseMember*)pData;
			pPurchaseMember->szMachineID[CountArray(pPurchaseMember->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_PurchaseMember PurchaseMember;
			ZeroMemory(&PurchaseMember,sizeof(PurchaseMember));

			//���ñ���
			PurchaseMember.dwUserID=pPurchaseMember->dwUserID;
			PurchaseMember.cbMemberOrder=pPurchaseMember->cbMemberOrder;
			PurchaseMember.wPurchaseTime=pPurchaseMember->wPurchaseTime;
			PurchaseMember.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(PurchaseMember.szMachineID,pPurchaseMember->szMachineID,CountArray(PurchaseMember.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_PURCHASE_MEMBER,dwSocketID,&PurchaseMember,sizeof(PurchaseMember));

			return true;
		}
	case SUB_GP_EXCHANGE_SCORE_BYINGOT:		//�һ���Ϸ��
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_ExchangeScoreByIngot));
			if(wDataSize!=sizeof(CMD_GP_ExchangeScoreByIngot)) return false;

			//����ṹ
			CMD_GP_ExchangeScoreByIngot * pExchangeScore = (CMD_GP_ExchangeScoreByIngot*)pData;
			pExchangeScore->szMachineID[CountArray(pExchangeScore->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_ExchangeScoreByIngot ExchangeScore;
			ZeroMemory(&ExchangeScore,sizeof(ExchangeScore));

			//���ñ���
			ExchangeScore.dwUserID=pExchangeScore->dwUserID;
			ExchangeScore.lExchangeIngot=pExchangeScore->lExchangeIngot;
			ExchangeScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ExchangeScore.szMachineID,pExchangeScore->szMachineID,CountArray(ExchangeScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_EXCHANGE_SCORE_INGOT,dwSocketID,&ExchangeScore,sizeof(ExchangeScore));

			return true;
		}
	case SUB_GP_EXCHANGE_SCORE_BYBEANS:		//�һ���Ϸ��
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_ExchangeScoreByBeans));
			if(wDataSize!=sizeof(CMD_GP_ExchangeScoreByBeans)) return false;

			//����ṹ
			CMD_GP_ExchangeScoreByBeans * pExchangeScore = (CMD_GP_ExchangeScoreByBeans*)pData;
			pExchangeScore->szMachineID[CountArray(pExchangeScore->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_ExchangeScoreByBeans ExchangeScore;
			ZeroMemory(&ExchangeScore,sizeof(ExchangeScore));

			//���ñ���
			ExchangeScore.dwUserID=pExchangeScore->dwUserID;
			ExchangeScore.dExchangeBeans=pExchangeScore->dExchangeBeans;
			ExchangeScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ExchangeScore.szMachineID,pExchangeScore->szMachineID,CountArray(ExchangeScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_EXCHANGE_SCORE_BEANS,dwSocketID,&ExchangeScore,sizeof(ExchangeScore));

			return true;
		}
	case SUB_GP_LOTTERY_CONFIG_REQ:			//��������
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_LotteryConfigReq));
			if(wDataSize!=sizeof(CMD_GP_LotteryConfigReq)) return false;

			//����ṹ
			CMD_GP_LotteryConfigReq * pLotteryConfigReq = (CMD_GP_LotteryConfigReq*)pData;
			pLotteryConfigReq->szLogonPass[CountArray(pLotteryConfigReq->szLogonPass)-1]=0;

			//����ṹ
			DBR_GP_LotteryConfigReq LotteryConfigReq;
			ZeroMemory(&LotteryConfigReq,sizeof(LotteryConfigReq));

			//���ñ���
			LotteryConfigReq.dwUserID=pLotteryConfigReq->dwUserID;
			LotteryConfigReq.wKindID=pLotteryConfigReq->wKindID;
			LotteryConfigReq.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(LotteryConfigReq.szLogonPass,pLotteryConfigReq->szLogonPass,CountArray(LotteryConfigReq.szLogonPass));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOTTERY_CONFIG_REQ,dwSocketID,&LotteryConfigReq,sizeof(LotteryConfigReq));

			return true;
		}
	case SUB_GP_LOTTERY_START:				//�齱��ʼ
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_LotteryStart));
			if(wDataSize!=sizeof(CMD_GP_LotteryStart)) return false;

			//����ṹ
			CMD_GP_LotteryStart * pLotteryStart = (CMD_GP_LotteryStart*)pData;
			pLotteryStart->szLogonPass[CountArray(pLotteryStart->szLogonPass)-1]=0;
			pLotteryStart->szMachineID[CountArray(pLotteryStart->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_LotteryStart LotteryStart;
			ZeroMemory(&LotteryStart,sizeof(LotteryStart));

			//���ñ���
			LotteryStart.dwUserID=pLotteryStart->dwUserID;
			LotteryStart.wKindID=pLotteryStart->wKindID;
			LotteryStart.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(LotteryStart.szLogonPass,pLotteryStart->szLogonPass,CountArray(LotteryStart.szLogonPass));
			lstrcpyn(LotteryStart.szMachineID,pLotteryStart->szMachineID,CountArray(LotteryStart.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOTTERY_START,dwSocketID,&LotteryStart,sizeof(LotteryStart));

			return true;
		}
		//
	case SUB_GP_SIMPLE_LOTERY:
		{
			//����У��
			ASSERT(wDataSize == sizeof(CMD_GP_SimpleLotteryStart));
			if (wDataSize != sizeof(CMD_GP_SimpleLotteryStart)) return false;
			CMD_GP_SimpleLotteryStart * pLottery = (CMD_GP_SimpleLotteryStart *)pData;

			LONGLONG   lLottery[12] = { 0,1,2,3,4,5,6,7,8,9,10,11};
			WORD   wIndex = 0;
			int random = (rand()+1000)%1000;
			if (random <= 40)
			{
				//28��ʯ
				wIndex = 0;
			}
			else if (random <= 540)
			{
				//5����
				wIndex = 11;
			}
			else if (random <= 550)
			{
				//88����
				wIndex = 10;
			}
			else if (random <= 560)
			{
				//88��ʯ
				wIndex = 9;
			}
			else if (random <= 760)
			{
				//1����
				wIndex = 8;
			}
			else if (random <= 765)
			{
				//288��ʯ
				wIndex = 7;
			}
			else if (random <= 770)
			{
				//10����
				wIndex = 6;
			}
			else if (random <= 970)
			{
				//10����	
				wIndex = 5;
			}
			else if (random <= 980)
			{
				//3����
				wIndex = 4;
			}
			else if (random <= 981)
			{
				//999��ʯ
				wIndex = 3;
			}
			else if (random <= 995)
			{
				//288����
				wIndex = 2;
			}
			else if (random <= 1000)
			{
				//888����
				wIndex = 1;
			}

			

			DBR_GP_SimpleLotteryStart   LotteryResult;
			LotteryResult.dwUserID = pLottery->dwUserID;
			LotteryResult.lLottery = lLottery[wIndex];
			LotteryResult.dwIndex = wIndex;
			strcpy(LotteryResult.szName,pLottery->szName);
			
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SIMPLE_LOTTERY, dwSocketID, &LotteryResult, sizeof(LotteryResult));

			return true;
			//��������
			//m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GP_USER_SERVICE, SUB_GP_LOTTERY_RESULT, &LotteryResult, sizeof(LotteryResult));
		}
	case SUB_GP_QUERY_USER_GAME_DATA:		//��Ϸ����
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_QueryUserGameData));
			if(wDataSize!=sizeof(CMD_GP_QueryUserGameData)) return false;

			//����ṹ
			CMD_GP_QueryUserGameData * pQueryUserGameData = (CMD_GP_QueryUserGameData*)pData;
			pQueryUserGameData->szDynamicPass[CountArray(pQueryUserGameData->szDynamicPass)-1]=0;

			//����ṹ
			DBR_GP_QueryUserGameData QueryUserGameData;
			ZeroMemory(&QueryUserGameData,sizeof(QueryUserGameData));

			//���ñ���
			QueryUserGameData.dwUserID=pQueryUserGameData->dwUserID;
			QueryUserGameData.wKindID=pQueryUserGameData->wKindID;
			lstrcpyn(QueryUserGameData.szDynamicPass,pQueryUserGameData->szDynamicPass,CountArray(QueryUserGameData.szDynamicPass));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_USER_GAME_DATA,dwSocketID,&QueryUserGameData,sizeof(QueryUserGameData));

			return true;
		}
	case SUB_GP_QUERY_SYSTEMQQ:			//ϵͳQQ:DCY by 2017.3.9
		{
			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_SYSTEM_QQ,dwSocketID,0,0);
			return true;
		}
	case SUB_GP_QUERY_SYSTEMINFO:		//ϵͳ��Ϣ:DCY by 2017.3.9
		{
			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_SYSTEM_INFO,dwSocketID,0,0);
			return true;
		}
	case SUB_GP_QUERY_SYSTEMCONTACT:
		{
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_SYSTEM_CONTACT,dwSocketID,0,0);
			return true;
		}
	case SUB_GP_QUERY_SHOP:				//ϵͳ�̳�:DCY by 2017.3.11
		{
			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_SYSTEM_SHOP,dwSocketID,0,0);
			return true;
		}
	case SUB_GP_EXCHANGE_SHOP:			//�һ���Ʒ:DCY by 2017.3.11
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GS_ExchangeShop));
			if (wDataSize!=sizeof(CMD_GS_ExchangeShop)) return false;

			//������Ϣ
			CMD_GS_ExchangeShop * pNetInfo=(CMD_GS_ExchangeShop *)pData;

			//��������
			DBR_GP_ExchangeGameShop tmpDBInfo;
			memset(&tmpDBInfo,0,sizeof(tmpDBInfo));

			tmpDBInfo.dwUserID = pNetInfo->dwUserID;
			tmpDBInfo.dwItemID = pNetInfo->dwItemID;
			tmpDBInfo.dwCount = pNetInfo->dwCount;
			tmpDBInfo.llNumber = pNetInfo->llNumber;
			memcpy(tmpDBInfo.strName,pNetInfo->strName,sizeof(tmpDBInfo.strName));
			memcpy(tmpDBInfo.strAddress,pNetInfo->strAddress,sizeof(tmpDBInfo.strAddress));

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_EXCHANGE_GAME_SHOP,dwSocketID,&tmpDBInfo,sizeof(tmpDBInfo));
			return true;
		}
	case SUB_GP_QUERY_INGAME_SEVERID:	//��ѯ��Ϸ״̬
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserInGameServerID));
			if (wDataSize!=sizeof(CMD_GP_UserInGameServerID)) return false;

			//������Ϣ
			CMD_GP_UserInGameServerID * pNetInfo=(CMD_GP_UserInGameServerID *)pData;

			//��������
			DBR_GP_QueryUserInGameServerID kDBInfo;
			ZeroMemory(&kDBInfo,sizeof(kDBInfo));
			kDBInfo.dwUserID=pNetInfo->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_INGAME_SERVERID,dwSocketID,&kDBInfo,sizeof(kDBInfo));

			return true;
		}
	}

	return false;
}

//Զ�̴���
bool CAttemperEngineSink::OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_C_SEARCH_CORRESPOND:	//Э������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_GP_C_SearchCorrespond)) return false;

			//������Ϣ
			CMD_GP_C_SearchCorrespond * pSearchCorrespond=(CMD_GP_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//��������
			CMD_CS_C_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ӱ���
			SearchCorrespond.dwSocketID=dwSocketID;
			SearchCorrespond.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//���ұ���
			SearchCorrespond.dwGameID=pSearchCorrespond->dwGameID;
			lstrcpyn(SearchCorrespond.szNickName,pSearchCorrespond->szNickName,CountArray(SearchCorrespond.szNickName));

			//��������
			m_pITCPSocketService->SendData(MDM_CS_REMOTE_SERVICE,SUB_CS_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

			return true;
		}
	}

	return false;
}


//��������
bool CAttemperEngineSink::OnTCPNetworkMainAndroidService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_PARAMETER:		//��ȡ����
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_GetParameter));
			if(wDataSize!=sizeof(CMD_GP_GetParameter)) return false;

			//��ȡ����
			CMD_GP_GetParameter * pGetParameter = (CMD_GP_GetParameter *)pData;
			ASSERT(pGetParameter!=NULL);

			//����ṹ
			DBR_GP_GetParameter GetParameter;
			GetParameter.wServerID = pGetParameter->wServerID;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_PARAMETER,dwSocketID,&GetParameter,sizeof(GetParameter));

			return true;
		}
	case SUB_GP_ADD_PARAMETER:		//��Ӳ���
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_AddParameter));
			if(wDataSize!=sizeof(CMD_GP_AddParameter)) return false;

			//��ȡ����
			CMD_GP_AddParameter * pAddParameter = (CMD_GP_AddParameter *)pData;
			ASSERT(pAddParameter!=NULL);

			//����ṹ
			DBR_GP_AddParameter AddParameter;
			AddParameter.wServerID = pAddParameter->wServerID;
			CopyMemory(&AddParameter.AndroidParameter,&pAddParameter->AndroidParameter,sizeof(tagAndroidParameter));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ADD_PARAMETER,dwSocketID,&AddParameter,sizeof(AddParameter));

			return true;
		}
	case SUB_GP_MODIFY_PARAMETER:	//�޸Ĳ���
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyParameter));
			if(wDataSize!=sizeof(CMD_GP_ModifyParameter)) return false;

			//��ȡ����
			CMD_GP_ModifyParameter * pModifyParameter = (CMD_GP_ModifyParameter *)pData;
			ASSERT(pModifyParameter!=NULL);

			//����ṹ
			DBR_GP_ModifyParameter ModifyParameter;
			ModifyParameter.wServerID=pModifyParameter->wServerID;
			CopyMemory(&ModifyParameter.AndroidParameter,&pModifyParameter->AndroidParameter,sizeof(tagAndroidParameter));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_PARAMETER,dwSocketID,&ModifyParameter,sizeof(ModifyParameter));
			
			return true;
		}
	case SUB_GP_DELETE_PARAMETER:	//ɾ������
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_DeleteParameter));
			if(wDataSize!=sizeof(CMD_GP_DeleteParameter)) return false;

			//��ȡ����
			CMD_GP_DeleteParameter * pDeleteParameter = (CMD_GP_DeleteParameter *)pData;
			ASSERT(pDeleteParameter!=NULL);

			//����ṹ
			DBR_GP_DeleteParameter DeleteParameter;
			DeleteParameter.wServerID = pDeleteParameter->wServerID;
			DeleteParameter.dwBatchID = pDeleteParameter->dwBatchID;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_DELETE_PARAMETER,dwSocketID,&DeleteParameter,sizeof(DeleteParameter));
			
			return true;
		}
	}

	return false;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_MB_LOGON_GAMEID:		//I D ��¼
		{
			return OnTCPNetworkSubMBLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubMBLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubMBRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_LOGON_OTHERPLATFORM: //����ƽ̨
		{
			return OnTCPNetworkSubMBLogonOtherPlatform(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_LOGON_VISITOR:      //�ο͵�¼
		{
			return OnTCPNetworkSubMBLogonVisitor(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//�б���
bool CAttemperEngineSink::OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch(wSubCmdID)
	{
	case SUB_MB_GET_ONLINE:
		{
			return OnTCPNetworkSubMBGetOnline(pData,wDataSize,dwSocketID);
		}
	}
	return false;
}

//I D ��¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonGameID));
	if (wDataSize<sizeof(CMD_GP_LogonGameID))
	{
		if (wDataSize<sizeof(CMD_GP_LogonGameID)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonGameID->dwPlazaVersion,dwSocketID,((pLogonGameID->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//������Ϣ
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	LogonGameID.cbNeeValidateMBCard=(pLogonGameID->cbValidateFlags&MB_VALIDATE_FLAGS);

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	printf("�ʺŵ�¼ %d %d\n",wDataSize,sizeof(CMD_GP_LogonAccounts));
	//Ч�����
	ASSERT(wDataSize<=sizeof(CMD_GP_LogonAccounts));
	if (wDataSize>sizeof(CMD_GP_LogonAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	pLogonAccounts->szPassPortID[CountArray(pLogonAccounts->szPassPortID)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonAccounts->dwPlazaVersion,dwSocketID,((pLogonAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwCheckUserRight=0;
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	LogonAccounts.cbNeeValidateMBCard=(pLogonAccounts->cbValidateFlags&MB_VALIDATE_FLAGS);
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));	
	lstrcpyn(LogonAccounts.szPassPortID,pLogonAccounts->szPassPortID,CountArray(LogonAccounts.szPassPortID));

	printf("�ʺŵ�¼ %d 1\n",dwSocketID);

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//���ߵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonManageTool(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonAccounts));
	if (wDataSize<sizeof(CMD_GP_LogonAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_LogonAccounts)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonAccounts->dwPlazaVersion,dwSocketID,((pLogonAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwCheckUserRight=UR_CAN_MANAGER_ANDROID;
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	LogonAccounts.cbNeeValidateMBCard=(pLogonAccounts->cbValidateFlags&MB_VALIDATE_FLAGS);

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_RegisterAccounts));
	if (wDataSize<sizeof(CMD_GP_RegisterAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_RegisterAccounts)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)pData;
	//ConvertUtf8ToGBK(pRegisterAccounts->szNickName,LEN_NICKNAME);
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szSpreader[CountArray(pRegisterAccounts->szSpreader)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szPassPortID[CountArray(pRegisterAccounts->szPassPortID)-1]=0;
	pRegisterAccounts->szCompellation[CountArray(pRegisterAccounts->szCompellation)-1]=0;
	pRegisterAccounts->szVerifyCode[CountArray(pRegisterAccounts->szVerifyCode)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	DWORD dwCurrentTime=(DWORD)time(NULL);
	DWORD dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

	//��֤�Ƿ���ȷ
	for (INT i=0; i<m_VerifyCodeArray.GetCount();i++)
	{
		tagVerifyCode *pVerifyCode=m_VerifyCodeArray[i];
		if (pVerifyCode==NULL) continue;

		if (pVerifyCode->dwClientAddr==dwClientAddr && lstrcmp(pVerifyCode->szMachineID,pRegisterAccounts->szMachineID)==0)
		{
			if (CheckVerifyCode(pRegisterAccounts->szVerifyCode,pVerifyCode->szValidateCode)==false)
			{
				//��������
				DBO_GP_LogonFailure LogonFailure;
				ZeroMemory(&LogonFailure,sizeof(LogonFailure));

				//��������
				LogonFailure.lResultCode=100;
				lstrcpyn(LogonFailure.szDescribeString,TEXT("У����У��ʧ�ܣ�"),CountArray(LogonFailure.szDescribeString));
				OnEventDataBase(DBO_GP_LOGON_FAILURE,dwSocketID,&LogonFailure,sizeof(LogonFailure));

				return true;
			}
			else
			{
				//SafeDelete(pVerifyCode);
				//m_VerifyCodeArray.RemoveAt(i);
				
				pVerifyCode->dwUpdateTime=dwCurrentTime;
				RandVerifyCode(pVerifyCode->szValidateCode,CountArray(pVerifyCode->szValidateCode));
				SendVerifyCode(dwSocketID,pVerifyCode->szValidateCode,false);
			}
		
			break;
		}
	}
	//Ч��汾
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pRegisterAccounts->dwPlazaVersion,dwSocketID,((pRegisterAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szPassPortID,pRegisterAccounts->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	lstrcpyn(RegisterAccounts.szCompellation,pRegisterAccounts->szCompellation,CountArray(RegisterAccounts.szCompellation));


	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//��֤����
bool CAttemperEngineSink::OnTCPNetworkSubPCVerifyIndividual(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_VerifyIndividual));
	if (wDataSize<sizeof(CMD_GP_VerifyIndividual)) return false;

	//������Ϣ
	CMD_GP_VerifyIndividual * pVerifyIndividual=(CMD_GP_VerifyIndividual *)pData;

	//Ч��汾
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pVerifyIndividual->dwPlazaVersion,dwSocketID,true)==false)
	{
		return true;
	}

	//��������
	DBR_GP_VerifyIndividual VerifyIndividual;
	ZeroMemory(&VerifyIndividual,sizeof(VerifyIndividual));

	//���ñ���
	VerifyIndividual.wVerifyMask=pVerifyIndividual->wVerifyMask;
	VerifyIndividual.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pVerifyIndividual+1,wDataSize-sizeof(CMD_GP_VerifyIndividual));

	//��չ��Ϣ
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		if(DataDescribe.wDataDescribe==DTP_GP_UI_NICKNAME)	//�û��ǳ�
		{
			ASSERT(pDataBuffer!=NULL);
			ASSERT(DataDescribe.wDataSize<=sizeof(VerifyIndividual.szVerifyContent));
			if (DataDescribe.wDataSize<=sizeof(VerifyIndividual.szVerifyContent))
			{
				CopyMemory(&VerifyIndividual.szVerifyContent,pDataBuffer,DataDescribe.wDataSize);
				VerifyIndividual.szVerifyContent[CountArray(VerifyIndividual.szVerifyContent)-1]=0;
			}
			break;
		}

		if(DataDescribe.wDataDescribe==DTP_GP_UI_ACCOUNTS)	//�û��ʺ�
		{
			ASSERT(pDataBuffer!=NULL);
			ASSERT(DataDescribe.wDataSize<=sizeof(VerifyIndividual.szVerifyContent));
			if (DataDescribe.wDataSize<=sizeof(VerifyIndividual.szVerifyContent))
			{
				CopyMemory(&VerifyIndividual.szVerifyContent,pDataBuffer,DataDescribe.wDataSize);
				VerifyIndividual.szVerifyContent[CountArray(VerifyIndividual.szVerifyContent)-1]=0;
			}
			break;
		}
	}

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_VERIFY_INDIVIDUAL,dwSocketID,&VerifyIndividual,sizeof(VerifyIndividual));

	return true;
}

//�ο͵�¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonVisitor(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonVisitor));
	if (wDataSize<sizeof(CMD_GP_LogonVisitor))return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonVisitor * pLogonVisitor=(CMD_GP_LogonVisitor *)pData;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonVisitor->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonVisitor->dwPlazaVersion,dwSocketID,((pLogonVisitor->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_LogonVisitor LogonVisitor;
	ZeroMemory(&LogonVisitor,sizeof(LogonVisitor));

	//������Ϣ
	LogonVisitor.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonVisitor.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	LogonVisitor.cbPlatformID=ULMByVisitor;
	lstrcpyn(LogonVisitor.szMachineID,pLogonVisitor->szMachineID,CountArray(LogonVisitor.szMachineID));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_VISITOR,dwSocketID,&LogonVisitor,sizeof(LogonVisitor));

	return true;
}

//��ѯ��֤��
bool CAttemperEngineSink::OnTCPNetworkSubPCQueryVerifyCode(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	ASSERT(wDataSize==sizeof(CMD_GP_QueryVerifyCode));
	if (wDataSize!=sizeof(CMD_GP_QueryVerifyCode)) return false;

	CMD_GP_QueryVerifyCode *pQueryVerifyCode=(CMD_GP_QueryVerifyCode *)pData;
	pQueryVerifyCode->szMachineID[CountArray(pQueryVerifyCode->szMachineID)-1]=0;
	pQueryVerifyCode->szVerifyCode[CountArray(pQueryVerifyCode->szVerifyCode)-1]=0;

	DWORD dwCurrentTime=(DWORD)time(NULL);
	DWORD dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	TCHAR szSendVerifyCode[LEN_VERIFY_CODE]=TEXT("");

	//�����Ƿ�������
	INT i=0;
	for (i=0; i<m_VerifyCodeArray.GetCount();i++)
	{
		tagVerifyCode *pVerifyCode=m_VerifyCodeArray[i];
		if (pVerifyCode==NULL) continue;

		if (pVerifyCode->dwClientAddr==dwClientAddr && lstrcmp(pVerifyCode->szMachineID,pQueryVerifyCode->szMachineID)==0)
		{
			if (dwCurrentTime-pVerifyCode->dwUpdateTime<TIME_VALID_VERIFY_CODE)
			{
				CMD_GP_VerifyCodeResult VerifyCodeResult;
				ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

				VerifyCodeResult.cbResultCode=2;
				lstrcpy(VerifyCodeResult.szDescString,TEXT("��Ǹ�����ڶ�ʱ���ڲ�ѯ��������Ƶ�������Ժ����ԣ�"));

				//������Ϣ
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
				return true;
			}

			//��������
			pVerifyCode->dwUpdateTime=dwCurrentTime;
			RandVerifyCode(pVerifyCode->szValidateCode,CountArray(pVerifyCode->szValidateCode));
			lstrcpyn(szSendVerifyCode,pVerifyCode->szValidateCode,CountArray(szSendVerifyCode));
			break;
		}
	}

	//����������
	if (i==m_VerifyCodeArray.GetCount())
	{
		tagVerifyCode * pNewVerifyCode=new tagVerifyCode;
		pNewVerifyCode->dwClientAddr=dwClientAddr;
		pNewVerifyCode->dwUpdateTime=dwCurrentTime;
		lstrcpyn(pNewVerifyCode->szMachineID,pQueryVerifyCode->szMachineID,CountArray(pNewVerifyCode->szMachineID));
		RandVerifyCode(pNewVerifyCode->szValidateCode,CountArray(pNewVerifyCode->szValidateCode));
		m_VerifyCodeArray.Add(pNewVerifyCode);

		lstrcpyn(szSendVerifyCode,pNewVerifyCode->szValidateCode,CountArray(szSendVerifyCode));
	}

	//������֤��
	SendVerifyCode(dwSocketID,szSendVerifyCode,true);

	return true;
}

//I D ��¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonGameID));
	if (wDataSize<sizeof(CMD_MB_LogonGameID)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonGameID * pLogonGameID=(CMD_MB_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;
	pLogonGameID->szMobilePhone[CountArray(pLogonGameID->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonGameID->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pLogonGameID->cbDeviceType,pLogonGameID->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//������Ϣ
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	lstrcpyn(LogonGameID.szMobilePhone,pLogonGameID->szMobilePhone,CountArray(LogonGameID.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	if (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonAccounts->cbDeviceType,pLogonAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone,pLogonAccounts->szMobilePhone,CountArray(LogonAccounts.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_RegisterAccounts));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_RegisterAccounts * pRegisterAccounts=(CMD_MB_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pRegisterAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pRegisterAccounts->cbDeviceType,pRegisterAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szMobilePhone,pRegisterAccounts->szMobilePhone,CountArray(RegisterAccounts.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//������¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonOtherPlatform(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonOtherPlatform));
	if (wDataSize<sizeof(CMD_MB_LogonOtherPlatform)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonOtherPlatform * pLogonOtherPlatform=(CMD_MB_LogonOtherPlatform *)pData;
	pLogonOtherPlatform->szUserUin[CountArray(pLogonOtherPlatform->szUserUin)-1]=0;
	pLogonOtherPlatform->szNickName[CountArray(pLogonOtherPlatform->szNickName)-1]=0;
	pLogonOtherPlatform->szMachineID[CountArray(pLogonOtherPlatform->szMachineID)-1]=0;
	pLogonOtherPlatform->szMobilePhone[CountArray(pLogonOtherPlatform->szMobilePhone)-1]=0;
	pLogonOtherPlatform->szCompellation[CountArray(pLogonOtherPlatform->szCompellation)-1]=0;

	//ƽ̨�ж�
	ASSERT(pLogonOtherPlatform->cbPlatformID==ULMBySina || pLogonOtherPlatform->cbPlatformID==ULMByTencent || pLogonOtherPlatform->cbPlatformID==ULMByRenRen);
	if (pLogonOtherPlatform->cbPlatformID!=ULMBySina && pLogonOtherPlatform->cbPlatformID!=ULMByTencent && pLogonOtherPlatform->cbPlatformID!=ULMByRenRen) return false;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonOtherPlatform->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonOtherPlatform->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonOtherPlatform->cbDeviceType,pLogonOtherPlatform->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonOtherPlatform LogonOtherPlatform;
	ZeroMemory(&LogonOtherPlatform,sizeof(LogonOtherPlatform));

	//������Ϣ
	LogonOtherPlatform.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonOtherPlatform.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	LogonOtherPlatform.cbGender=pLogonOtherPlatform->cbGender;
	LogonOtherPlatform.cbPlatformID=pLogonOtherPlatform->cbPlatformID;
	lstrcpyn(LogonOtherPlatform.szUserUin,pLogonOtherPlatform->szUserUin,CountArray(LogonOtherPlatform.szUserUin));
	lstrcpyn(LogonOtherPlatform.szNickName,pLogonOtherPlatform->szNickName,CountArray(LogonOtherPlatform.szNickName));
	lstrcpyn(LogonOtherPlatform.szMachineID,pLogonOtherPlatform->szMachineID,CountArray(LogonOtherPlatform.szMachineID));
	lstrcpyn(LogonOtherPlatform.szMobilePhone,pLogonOtherPlatform->szMobilePhone,CountArray(LogonOtherPlatform.szMobilePhone));
	lstrcpyn(LogonOtherPlatform.szCompellation,pLogonOtherPlatform->szCompellation,CountArray(LogonOtherPlatform.szCompellation));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_OTHERPLATFORM,dwSocketID,&LogonOtherPlatform,sizeof(LogonOtherPlatform));

	return true;
}

//�ο͵�¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonVisitor(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonVisitor));
	if (wDataSize<sizeof(CMD_MB_LogonVisitor))return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonVisitor * pLogonVisitor=(CMD_MB_LogonVisitor *)pData;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonVisitor->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonVisitor->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonVisitor->cbDeviceType,pLogonVisitor->dwPlazaVersion,dwSocketID)==false)
	{
		return true;
	}

	//��������
	DBR_MB_LogonVisitor LogonVisitor;
	ZeroMemory(&LogonVisitor,sizeof(LogonVisitor));

	//������Ϣ
	LogonVisitor.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonVisitor.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	LogonVisitor.cbPlatformID=ULMByVisitor;
	lstrcpyn(LogonVisitor.szMachineID,pLogonVisitor->szMachineID,CountArray(LogonVisitor.szMachineID));
	lstrcpyn(LogonVisitor.szMobilePhone,pLogonVisitor->szMobilePhone,CountArray(LogonVisitor.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_VISITOR,dwSocketID,&LogonVisitor,sizeof(LogonVisitor));

	return true;
}

//��ȡ��������
bool CAttemperEngineSink::OnTCPNetworkSubMBGetOnline(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��������
	CMD_MB_GetOnline * pGetOnline=(CMD_MB_GetOnline *)pData;
	WORD wHeadSize=(sizeof(CMD_MB_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

	//Ч������
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

	//��������
	CMD_MB_KindOnline KindOnline;
	CMD_MB_ServerOnline ServerOnline;
	ZeroMemory(&KindOnline,sizeof(KindOnline));
	ZeroMemory(&ServerOnline,sizeof(ServerOnline));

	//��ȡ����
	POSITION KindPosition=NULL;
	do
	{
		//��ȡ����
		CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

		//���ñ���
		if (pGameKindItem!=NULL)
		{
			WORD wKindIndex=KindOnline.wKindCount++;
			KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
			KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount+pGameKindItem->m_GameKind.dwAndroidCount;
		}

		//����ж�
		if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
		{
			ASSERT(FALSE);
			break;
		}

	} while (KindPosition!=NULL);

	//��ȡ����
	for (WORD i=0;i<pGetOnline->wServerCount;i++)
	{
		//��ȡ����
		WORD wServerID=pGetOnline->wOnLineServerID[i];
		CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

		//���ñ���
		if (pGameServerItem!=NULL)
		{
			WORD wServerIndex=ServerOnline.wServerCount++;
			ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
			ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount+pGameServerItem->m_GameServer.dwAndroidCount;
		}

		//����ж�
		if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
		{
			ASSERT(FALSE);
			break;
		}
	}

	//��������
	if (KindOnline.wKindCount>0)
	{
		WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
		WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_KINE_ONLINE,&KindOnline,wSendSize);
	}

	//��������
	if (ServerOnline.wServerCount>0)
	{
		WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
		WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_SERVER_ONLINE,&ServerOnline,wSendSize);
	}

	return true;
}


//��¼�ɹ�
bool CAttemperEngineSink::OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_LogonSuccess * pDBOLogonSuccess=(DBO_GP_LogonSuccess *)pData;
	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbDataBuffer;

	//���Ͷ���
	WORD wHeadSize=sizeof(CMD_GP_LogonSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//���ñ���
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_GP_LogonSuccess));

	//��������
	pCMDLogonSuccess->wFaceID=pDBOLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender=pDBOLogonSuccess->cbGender;
	pCMDLogonSuccess->dwGameID=pDBOLogonSuccess->dwGameID;
	pCMDLogonSuccess->dwUserID=pDBOLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwCustomID=pDBOLogonSuccess->dwCustomID;
	pCMDLogonSuccess->dwExperience=pDBOLogonSuccess->dwExperience;
	pCMDLogonSuccess->dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	pCMDLogonSuccess->cbMoorMachine=pDBOLogonSuccess->cbMoorMachine;
	lstrcpyn(pCMDLogonSuccess->szAccounts,pDBOLogonSuccess->szAccounts,CountArray(pCMDLogonSuccess->szAccounts));
	lstrcpyn(pCMDLogonSuccess->szNickName,pDBOLogonSuccess->szNickName,CountArray(pCMDLogonSuccess->szNickName));
	lstrcpyn(pCMDLogonSuccess->szDynamicPass,pDBOLogonSuccess->szDynamicPass,CountArray(pCMDLogonSuccess->szDynamicPass));	
	lstrcpyn(pCMDLogonSuccess->szHeadUrl,pDBOLogonSuccess->szUnderWrite,CountArray(pCMDLogonSuccess->szHeadUrl));	

	//�û��ɼ�
	pCMDLogonSuccess->lUserScore=pDBOLogonSuccess->lUserScore;
	pCMDLogonSuccess->lUserIngot=pDBOLogonSuccess->lUserIngot;
	pCMDLogonSuccess->lUserInsure=pDBOLogonSuccess->lUserInsure;	
	pCMDLogonSuccess->dUserBeans=pDBOLogonSuccess->dUserBeans;	

	//������Ϣ
	pCMDLogonSuccess->cbInsureEnabled=pDBOLogonSuccess->cbInsureEnabled;
	pCMDLogonSuccess->cbShowServerStatus=m_bShowServerStatus?1:0;
	
	// DCY Client Addr
	pCMDLogonSuccess->dwAddress = (m_pBindParameter+LOWORD(dwContextID))->dwClientAddr;

	//��Ա��Ϣ
	if (pDBOLogonSuccess->cbMemberOrder!=0 || pDBOLogonSuccess->dwCheckUserRight!=0)
	{
		DTP_GP_MemberInfo MemberInfo;
		ZeroMemory(&MemberInfo,sizeof(MemberInfo));
		MemberInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
		MemberInfo.MemberOverDate=pDBOLogonSuccess->MemberOverDate;
		SendPacket.AddPacket(&MemberInfo,sizeof(MemberInfo),DTP_GP_MEMBER_INFO);
	}

	//����ǩ��
	if (pDBOLogonSuccess->szUnderWrite[0]!=0)
	{
		SendPacket.AddPacket(pDBOLogonSuccess->szUnderWrite,CountStringBuffer(pDBOLogonSuccess->szUnderWrite),DTP_GP_UNDER_WRITE);
	}

	//��¼�ɹ�
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_LogonSuccess);

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbDataBuffer,wSendSize);

	//�����б�
	if (m_pInitParameter->m_cbDelayList==FALSE || 
		pDBOLogonSuccess->dwCheckUserRight==UR_CAN_MANAGER_ANDROID)
	{
		//�����б�
		SendGameTypeInfo(dwContextID);
		SendGameKindInfo(dwContextID);
		SendGamePageInfo(dwContextID,INVALID_WORD);
		SendGameNodeInfo(dwContextID,INVALID_WORD);
		SendGameServerInfo(dwContextID,INVALID_WORD);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}
	else
	{
		SendGameTypeInfo(dwContextID);
		SendGameKindInfo(dwContextID);
		SendGamePageInfo(dwContextID,0);
		SendGameServerInfo(dwContextID,INVALID_WORD/*,true*/);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}
	
	//�ȼ�����
	SendGrowLevelConfig(dwContextID);

	//�����б�
	SendUserSignupInfo(dwContextID,pDBOLogonSuccess->wSignupCount,pDBOLogonSuccess->SignupMatchInfo,false);

	//��¼���
	CMD_GP_LogonFinish LogonFinish;
	ZeroMemory(&LogonFinish,sizeof(LogonFinish));
	LogonFinish.wIntermitTime=m_pInitParameter->m_wIntermitTime;
	LogonFinish.wOnLineCountTime=m_pInitParameter->m_wOnLineCountTime;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH,&LogonFinish,sizeof(LogonFinish));

	//���������:DCY by 2017.3.9-----------------------------------------------------------
// 	OnTCPNetworkMainPCUserService(SUB_GP_QUERY_SYSTEMQQ,0,0,dwContextID);
// 	OnTCPNetworkMainPCUserService(SUB_GP_QUERY_SYSTEMINFO,0,0,dwContextID);
// 	OnTCPNetworkMainPCUserService(SUB_GP_QUERY_SHOP,0,0,dwContextID);

// 	CMD_GS_ExchangeShop exInfo;
// 	memset(&exInfo,0,sizeof(exInfo));
// 
// 	exInfo.dwUserID = pDBOLogonSuccess->dwUserID;
// 	exInfo.dwItemID =1;
// 	exInfo.dwCount = 1;
// 	exInfo.llNumber = 18359256606;
// 	_sntprintf(exInfo.strName,sizeof(exInfo.strName),TEXT("������"));
// 	_sntprintf(exInfo.strAddress,sizeof(exInfo.strAddress),TEXT("����ʡ�����к���������������Ϣ�Ƽ����޹�˾"));
// 	OnTCPNetworkMainPCUserService(SUB_GP_EXCHANGE_SHOP,&exInfo,sizeof(exInfo),dwContextID);
	//���������:DCY by 2017.3.9-----------------------------------------------------------
	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_GP_LogonFailure * pLogonFailure=(DBO_GP_LogonFailure *)pData;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBPCLogonValidateMBCard(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GP_ValidateMBCard));
	if (wDataSize!=sizeof(DBO_GP_ValidateMBCard)) return false;

	DBO_GP_ValidateMBCard * pValidateMBCard=(DBO_GP_ValidateMBCard *)pData;

	//����ṹ
	CMD_GP_ValidateMBCard ValidateMBCard;
	ValidateMBCard.uMBCardID=pValidateMBCard->uMBCardID;

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_VALIDATE_MBCARD,&ValidateMBCard,sizeof(ValidateMBCard));

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBPCLogonValidatePassPort(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_VALIDATE_PASSPORT,NULL,0);

	return true;
}

//��֤���
bool CAttemperEngineSink::OnDBPCLogonVerifyResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize<=sizeof(DBO_GP_VerifyIndividualResult));
	if (wDataSize>sizeof(DBO_GP_VerifyIndividualResult)) return false;

	//��ȡ����
	DBO_GP_VerifyIndividualResult *pVerifyIndividualResult=(DBO_GP_VerifyIndividualResult *)pData;

	//����ṹ
	CMD_GP_VerifyIndividualResult VerifyIndividualResult;
	VerifyIndividualResult.wVerifyMask=pVerifyIndividualResult->wVerifyMask;
	VerifyIndividualResult.bVerifyPassage=pVerifyIndividualResult->bVerifyPassage;
	lstrcpyn(VerifyIndividualResult.szErrorMsg,pVerifyIndividualResult->szErrorMsg,CountArray(VerifyIndividualResult.szErrorMsg));

	//��������
	WORD wStringSize=CountStringBuffer(VerifyIndividualResult.szErrorMsg);
	WORD wSendSize=sizeof(VerifyIndividualResult)-sizeof(VerifyIndividualResult.szErrorMsg)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_VERIFY_RESULT,&VerifyIndividualResult,wSendSize);

	return true;
}

//ƽ̨����
bool CAttemperEngineSink::OnDBPCPlatformParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GP_PlatformParameter * pPlatformParameter=(DBO_GP_PlatformParameter *)pData;

	//���ñ���
	m_PlatformParameter.dwExchangeRate = pPlatformParameter->dwExchangeRate;
	m_PlatformParameter.dwPresentExchangeRate = pPlatformParameter->dwPresentExchangeRate;
	m_PlatformParameter.dwRateGold = pPlatformParameter->dwRateGold;

	return true;
}

//�û�ͷ��
bool CAttemperEngineSink::OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserFaceInfo UserFaceInfo;
	ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));
	DBO_GP_UserFaceInfo * pUserFaceInfo=(DBO_GP_UserFaceInfo *)pData;

	//���ñ���
	UserFaceInfo.wFaceID=pUserFaceInfo->wFaceID;
	UserFaceInfo.dwCustomID=pUserFaceInfo->dwCustomID;

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_FACE_INFO,&UserFaceInfo,sizeof(UserFaceInfo));

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserIndividual * pDBOUserIndividual=(DBO_GP_UserIndividual *)pData;
	CMD_GP_UserIndividual * pCMDUserIndividual=(CMD_GP_UserIndividual *)cbDataBuffer;
	CSendPacketHelper SendPacket(cbDataBuffer+sizeof(CMD_GP_UserIndividual),sizeof(cbDataBuffer)-sizeof(CMD_GP_UserIndividual));

	//���ñ���
	ZeroMemory(pCMDUserIndividual,sizeof(CMD_GP_UserIndividual));

	//��������
	pCMDUserIndividual->dwUserID=pDBOUserIndividual->dwUserID;

	//�û���Ϣ
	if (pDBOUserIndividual->szUserNote[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szUserNote);
		SendPacket.AddPacket(pDBOUserIndividual->szUserNote,wBufferSize,DTP_GP_UI_USER_NOTE);
	}

	//��ʵ����
	if (pDBOUserIndividual->szCompellation[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szCompellation);
		SendPacket.AddPacket(pDBOUserIndividual->szCompellation,wBufferSize,DTP_GP_UI_COMPELLATION);
	}

	//�绰����
	if (pDBOUserIndividual->szSeatPhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szSeatPhone);
		SendPacket.AddPacket(pDBOUserIndividual->szSeatPhone,wBufferSize,DTP_GP_UI_SEAT_PHONE);
	}

	//�ƶ��绰
	if (pDBOUserIndividual->szMobilePhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szMobilePhone);
		SendPacket.AddPacket(pDBOUserIndividual->szMobilePhone,wBufferSize,DTP_GP_UI_MOBILE_PHONE);
	}

	//��ϵ����
	if (pDBOUserIndividual->szQQ[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szQQ);
		SendPacket.AddPacket(pDBOUserIndividual->szQQ,wBufferSize,DTP_GP_UI_QQ);
	}

	//�����ʼ�
	if (pDBOUserIndividual->szEMail[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szEMail);
		SendPacket.AddPacket(pDBOUserIndividual->szEMail,wBufferSize,DTP_GP_UI_EMAIL);
	}

	//��ϵ��ַ
	if (pDBOUserIndividual->szDwellingPlace[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szDwellingPlace);
		SendPacket.AddPacket(pDBOUserIndividual->szDwellingPlace,wBufferSize,DTP_GP_UI_DWELLING_PLACE);
	}

	//������Ϣ
	WORD wSendSize=sizeof(CMD_GP_UserIndividual)+SendPacket.GetDataSize();
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INDIVIDUAL,cbDataBuffer,wSendSize);

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureInfo * pUserInsureInfo=(DBO_GP_UserInsureInfo *)pData;

	//��������
	CMD_GP_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//��������
	UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
	UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
	UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
	UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//���гɹ�
bool CAttemperEngineSink::OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;

	//��������
	CMD_GP_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//���ñ���
	UserInsureSuccess.dwUserID=pUserInsureSuccess->dwUserID;
	UserInsureSuccess.lUserScore=pUserInsureSuccess->lSourceScore+pUserInsureSuccess->lVariationScore;
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//������Ϣ
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//��������
	DBO_GP_UserInsureFailure * pUserInsureFailure=(DBO_GP_UserInsureFailure *)pData;

	//��������
	UserInsureFailure.lResultCode=pUserInsureFailure->lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pUserInsureFailure->szDescribeString,CountArray(UserInsureFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserTransferUserInfo * pTransferUserInfo=(DBO_GP_UserTransferUserInfo *)pData;

	//��������
	CMD_GP_UserTransferUserInfo UserTransferUserInfo;
	ZeroMemory(&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	//�������
	UserTransferUserInfo.dwTargetGameID=pTransferUserInfo->dwGameID;
	lstrcpyn(UserTransferUserInfo.szAccounts,pTransferUserInfo->szAccounts,CountArray(UserTransferUserInfo.szAccounts));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_RESULT,&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	return true;
}

//�������
bool CAttemperEngineSink::OnDBPCUserMatchSignupResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_MatchSignupResult * pMatchSignupResult=(DBO_GP_MatchSignupResult *)pData;

	//��������
	CMD_GP_MatchSignupResult MatchSignupResult;
	ZeroMemory(&MatchSignupResult,sizeof(MatchSignupResult));

	//����ṹ
	MatchSignupResult.bSignup = pMatchSignupResult->bSignup;
	MatchSignupResult.bSuccessed = pMatchSignupResult->bSuccessed;
	MatchSignupResult.wServerID = pMatchSignupResult->wServerID;
	MatchSignupResult.lCurrScore = pMatchSignupResult->lCurrScore;
	lstrcpyn(MatchSignupResult.szDescribeString,pMatchSignupResult->szDescribeString,CountArray(MatchSignupResult.szDescribeString));

	//��������
	WORD wSendSize = sizeof(MatchSignupResult)-sizeof(MatchSignupResult.szDescribeString)+CountStringBuffer(MatchSignupResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_MATCH_SIGNUP_RESULT,&MatchSignupResult,sizeof(MatchSignupResult));

	return true;
}

//��ͨ���
bool CAttemperEngineSink::OnDBPCUserInsureEnableResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureEnableResult * pUserInsureEnableResult=(DBO_GP_UserInsureEnableResult *)pData;

	//��������
	CMD_GP_UserInsureEnableResult UserInsureEnableResult;
	ZeroMemory(&UserInsureEnableResult,sizeof(UserInsureEnableResult));

	//�������
	UserInsureEnableResult.cbInsureEnabled=pUserInsureEnableResult->cbInsureEnabled;
	lstrcpyn(UserInsureEnableResult.szDescribeString,pUserInsureEnableResult->szDescribeString,CountArray(UserInsureEnableResult.szDescribeString));

	//��������
	WORD wHeadSize=CountStringBuffer(UserInsureEnableResult.szDescribeString);
	wHeadSize +=sizeof(UserInsureEnableResult)-sizeof(UserInsureEnableResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_ENABLE_RESULT,&UserInsureEnableResult,wHeadSize);

	return true;
}

//ǩ����Ϣ
bool CAttemperEngineSink::OnDBPCUserCheckInInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_CheckInInfo * pCheckInInfo=(DBO_GP_CheckInInfo *)pData;

	//��������
	CMD_GP_CheckInInfo CheckInInfo;
	ZeroMemory(&CheckInInfo,sizeof(CheckInInfo));

	//�������
	CheckInInfo.bTodayChecked=pCheckInInfo->bTodayChecked;
	CheckInInfo.wSeriesDate=pCheckInInfo->wSeriesDate;
	CopyMemory(CheckInInfo.lRewardGold,m_lCheckInReward,sizeof(CheckInInfo.lRewardGold));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_CHECKIN_INFO,&CheckInInfo,sizeof(CheckInInfo));

	return true;
}

//ǩ�����
bool CAttemperEngineSink::OnDBPCUserCheckInResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_CheckInResult * pCheckInResult=(DBO_GP_CheckInResult *)pData;

	//��������
	CMD_GP_CheckInResult CheckInResult;
	ZeroMemory(&CheckInResult,sizeof(CheckInResult));

	//�������
	CheckInResult.bSuccessed=pCheckInResult->bSuccessed;
	CheckInResult.lScore=pCheckInResult->lScore;
	lstrcpyn(CheckInResult.szNotifyContent,pCheckInResult->szNotifyContent,CountArray(CheckInResult.szNotifyContent));

	//��������
	WORD wSendSize = sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_CHECKIN_RESULT,&CheckInResult,wSendSize);

	return true;
}

//ǩ������
bool CAttemperEngineSink::OnDBPCCheckInReward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize==sizeof(DBO_GP_CheckInReward));
	if(wDataSize!=sizeof(DBO_GP_CheckInReward)) return false;

	//��ȡ����
	DBO_GP_CheckInReward * pCheckInReward=(DBO_GP_CheckInReward *)pData;

	//��������
	CopyMemory(m_lCheckInReward,pCheckInReward->lRewardGold,sizeof(m_lCheckInReward));

	return true;
}

//�����б�
bool CAttemperEngineSink::OnDBPCTaskList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GP_TaskListInfo * pTaskListInfo=(DBO_GP_TaskListInfo *)pData;

	//��������	
	WORD wTaskCount = __min(pTaskListInfo->wTaskCount,CountArray(m_TaskParameterBuffer)-m_wTaskCountBuffer);
	CopyMemory(m_TaskParameterBuffer+m_wTaskCountBuffer,++pTaskListInfo,sizeof(tagTaskParameter)*wTaskCount);

	//���ñ���
	m_wTaskCountBuffer += wTaskCount;

	return true;
}

//�������
bool CAttemperEngineSink::OnDBPCTaskListEnd(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������	
	m_wTaskCount = m_wTaskCountBuffer;
	CopyMemory(m_TaskParameter, m_TaskParameterBuffer, sizeof(tagTaskParameter)*m_wTaskCount);

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBPCTaskInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_TaskInfo * pTaskInfo=(DBO_GP_TaskInfo *)pData;

	//��������
	WORD wTaskCount = m_wTaskCount;

	//��������
	WORD wSendSize=0,wParameterSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	LPBYTE pDataBuffer = cbDataBuffer;

	//��������
	pDataBuffer += sizeof(wTaskCount);
	wSendSize = wSendSize;

	//�ְ�����
	for(WORD i=0;i<wTaskCount;i++)
	{
		//Խ���ж�
		if(wSendSize+sizeof(tagTaskParameter)>SOCKET_TCP_PACKET)
		{
			//���ñ���
			pDataBuffer = cbDataBuffer;
			wTaskCount -= i;
			*(WORD*)pDataBuffer = i; 

			//��������
			m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_TASK_LIST,&cbDataBuffer,wSendSize);

			//���ñ���
			pDataBuffer += sizeof(wSendSize);
			wSendSize = wSendSize;
		}

		//�����С
		wParameterSize = sizeof(tagTaskParameter)-sizeof(m_TaskParameter[i].szTaskDescribe)+CountStringBuffer(m_TaskParameter[i].szTaskDescribe);
		*(WORD *)pDataBuffer = wParameterSize;
		pDataBuffer += sizeof(wParameterSize);
		wSendSize += sizeof(wParameterSize);

		//��������
		CopyMemory(pDataBuffer,&m_TaskParameter[i],wParameterSize);

		//�ƽ�ָ��
		pDataBuffer += wParameterSize;
		wSendSize += wParameterSize;
	}

	//ʣ�෢��
	if(wTaskCount>0 && wSendSize)
	{
		//���ñ���
		pDataBuffer = cbDataBuffer;
		*(WORD *)pDataBuffer = wTaskCount; 

		//��������
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_TASK_LIST,&cbDataBuffer,wSendSize);
	}

	//����ṹ
	CMD_GP_TaskInfo TaskInfo;
	TaskInfo.wTaskCount = pTaskInfo->wTaskCount;
	CopyMemory(TaskInfo.TaskStatus,pTaskInfo->TaskStatus,sizeof(TaskInfo.TaskStatus[0])*pTaskInfo->wTaskCount);

	//��������
	WORD wSendDataSize = sizeof(TaskInfo)-sizeof(TaskInfo.TaskStatus);
	wSendDataSize += sizeof(TaskInfo.TaskStatus[0])*TaskInfo.wTaskCount;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_TASK_INFO,&TaskInfo,wSendDataSize);

	return true;
}

//������
bool CAttemperEngineSink::OnDBPCTaskResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_TaskResult * pTaskResult=(DBO_GP_TaskResult *)pData;

	//��������
	CMD_GP_TaskResult TaskResult;
	ZeroMemory(&TaskResult,sizeof(TaskResult));

	//�������
	TaskResult.bSuccessed=pTaskResult->bSuccessed;
	TaskResult.wCommandID=pTaskResult->wCommandID;
	TaskResult.lCurrScore=pTaskResult->lCurrScore;
	TaskResult.lCurrIngot=pTaskResult->lCurrIngot;	
	lstrcpyn(TaskResult.szNotifyContent,pTaskResult->szNotifyContent,CountArray(TaskResult.szNotifyContent));

	//��������
	WORD wSendSize = sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_TASK_RESULT,&TaskResult,wSendSize);

	return true;
}

//�ͱ�����
bool CAttemperEngineSink::OnDBPCBaseEnsureParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_BaseEnsureParameter * pEnsureParameter=(DBO_GP_BaseEnsureParameter *)pData;

	//���ñ���
	m_BaseEnsureParameter.cbTakeTimes = pEnsureParameter->cbTakeTimes;
	m_BaseEnsureParameter.lScoreAmount = pEnsureParameter->lScoreAmount;
	m_BaseEnsureParameter.lScoreCondition = pEnsureParameter->lScoreCondition;

	return true;
}

//�ͱ����
bool CAttemperEngineSink::OnDBPCBaseEnsureResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_BaseEnsureResult * pBaseEnsureResult=(DBO_GP_BaseEnsureResult *)pData;

	//����ṹ
	CMD_GP_BaseEnsureResult BaseEnsureResult;
	BaseEnsureResult.bSuccessed=pBaseEnsureResult->bSuccessed;
	BaseEnsureResult.lGameScore=pBaseEnsureResult->lGameScore;
	lstrcpyn(BaseEnsureResult.szNotifyContent,pBaseEnsureResult->szNotifyContent,CountArray(BaseEnsureResult.szNotifyContent));

	//��������
	WORD wSendDataSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
	wSendDataSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_RESULT,&BaseEnsureResult,wSendDataSize);

	return true;
}

//�ƹ����
bool CAttemperEngineSink::OnDBPCUserSpreadInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserSpreadInfo * pSpreadParameter=(DBO_GP_UserSpreadInfo *)pData;

	//����ṹ
	CMD_GP_UserSpreadInfo UserSpreadInfo;
	UserSpreadInfo.dwSpreadCount=pSpreadParameter->dwSpreadCount;
	UserSpreadInfo.lSpreadReward=pSpreadParameter->lSpreadReward;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_SPREAD_INFO,&UserSpreadInfo,sizeof(UserSpreadInfo));

	return true;	
}

//�ȼ�����
bool CAttemperEngineSink::OnDBPCGrowLevelConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_GrowLevelConfig * pGrowLevelConfig=(DBO_GP_GrowLevelConfig *)pData;

	//��������	
	m_wLevelCount = pGrowLevelConfig->wLevelCount;
	CopyMemory(m_GrowLevelConfig,&pGrowLevelConfig->GrowLevelConfig,sizeof(tagGrowLevelConfig)*m_wLevelCount);

	return true;
}

//�ȼ�����
bool CAttemperEngineSink::OnDBPCGrowLevelParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_GrowLevelParameter * pGrowLevelParameter=(DBO_GP_GrowLevelParameter *)pData;

	//����ṹ
	CMD_GP_GrowLevelParameter GrowLevelParameter;
	GrowLevelParameter.wCurrLevelID=pGrowLevelParameter->wCurrLevelID;	
	GrowLevelParameter.dwExperience=pGrowLevelParameter->dwExperience;
	GrowLevelParameter.dwUpgradeExperience=pGrowLevelParameter->dwUpgradeExperience;
	GrowLevelParameter.lUpgradeRewardGold=pGrowLevelParameter->lUpgradeRewardGold;
	GrowLevelParameter.lUpgradeRewardIngot=pGrowLevelParameter->lUpgradeRewardIngot;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GROWLEVEL_PARAMETER,&GrowLevelParameter,sizeof(GrowLevelParameter));

	return true;
}

//�ȼ�����
bool CAttemperEngineSink::OnDBPCGrowLevelUpgrade(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_GrowLevelUpgrade * pGrowLevelUpgrade=(DBO_GP_GrowLevelUpgrade *)pData;

	//����ṹ
	CMD_GP_GrowLevelUpgrade GrowLevelUpgrade;
	GrowLevelUpgrade.lCurrScore=pGrowLevelUpgrade->lCurrScore;
	GrowLevelUpgrade.lCurrIngot=pGrowLevelUpgrade->lCurrIngot;
	lstrcpyn(GrowLevelUpgrade.szNotifyContent,pGrowLevelUpgrade->szNotifyContent,CountArray(GrowLevelUpgrade.szNotifyContent));

	//��������
	WORD wSendDataSize = sizeof(GrowLevelUpgrade)-sizeof(GrowLevelUpgrade.szNotifyContent);
	wSendDataSize += CountStringBuffer(GrowLevelUpgrade.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GROWLEVEL_UPGRADE,&GrowLevelUpgrade,wSendDataSize);

	return true;
}

//��Ա����
bool CAttemperEngineSink::OnDBPCMemberParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_MemberParameter * pMemberParameter=(DBO_GP_MemberParameter *)pData;

	//��������	
	m_wMemberCount = pMemberParameter->wMemberCount;
	CopyMemory(m_MemberParameter,pMemberParameter->MemberParameter,sizeof(tagMemberParameter)*m_wMemberCount);

	return true;
}

//������
bool CAttemperEngineSink::OnDBPCPurchaseResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��ȡ����
	DBO_GP_PurchaseResult * pPurchaseResult=(DBO_GP_PurchaseResult *)pData;

	//����ṹ
	CMD_GP_PurchaseResult PurchaseResult;
	ZeroMemory(&PurchaseResult,sizeof(PurchaseResult));

	//���ñ���
	PurchaseResult.bSuccessed=pPurchaseResult->bSuccessed;
	PurchaseResult.lCurrScore=pPurchaseResult->lCurrScore;
	PurchaseResult.dCurrBeans=pPurchaseResult->dCurrBeans;
	PurchaseResult.cbMemberOrder=pPurchaseResult->cbMemberOrder;
	lstrcpyn(PurchaseResult.szNotifyContent,pPurchaseResult->szNotifyContent,CountArray(PurchaseResult.szNotifyContent));

	//��������
	WORD wSendDataSize = sizeof(PurchaseResult)-sizeof(PurchaseResult.szNotifyContent);
	wSendDataSize += CountStringBuffer(PurchaseResult.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_PURCHASE_RESULT,&PurchaseResult,wSendDataSize);

	return true;
}

//�һ����
bool CAttemperEngineSink::OnDBPCExChangeResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��ȡ����
	DBO_GP_ExchangeResult * pExchangeResult=(DBO_GP_ExchangeResult *)pData;

	//����ṹ
	CMD_GP_ExchangeResult ExchangeResult;
	ZeroMemory(&ExchangeResult,sizeof(ExchangeResult));

	//���ñ���
	ExchangeResult.bSuccessed=pExchangeResult->bSuccessed;
	ExchangeResult.lCurrScore=pExchangeResult->lCurrScore;
	ExchangeResult.lCurrIngot=pExchangeResult->lCurrIngot;
	ExchangeResult.dCurrBeans=pExchangeResult->dCurrBeans;
	lstrcpyn(ExchangeResult.szNotifyContent,pExchangeResult->szNotifyContent,CountArray(ExchangeResult.szNotifyContent));

	//��������
	WORD wSendDataSize = sizeof(ExchangeResult)-sizeof(ExchangeResult.szNotifyContent);
	wSendDataSize += CountStringBuffer(ExchangeResult.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_EXCHANGE_RESULT,&ExchangeResult,wSendDataSize);

	return true;
}

//��������
bool CAttemperEngineSink::OnDBAndroidParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;
	
	//��������
	DBO_GP_AndroidParameter * pAndroidParameter=(DBO_GP_AndroidParameter *)pData;

	//��������
	CMD_GP_AndroidParameter AndroidParameter;
	ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));

	//�������
	AndroidParameter.wSubCommdID=pAndroidParameter->wSubCommdID;
	AndroidParameter.wParameterCount=pAndroidParameter->wParameterCount;
	CopyMemory(AndroidParameter.AndroidParameter,pAndroidParameter->AndroidParameter,sizeof(tagAndroidParameter)*AndroidParameter.wParameterCount);

	//�����С
	WORD wSendDataSize = sizeof(AndroidParameter)-sizeof(AndroidParameter.AndroidParameter);
	wSendDataSize += sizeof(tagAndroidParameter)*AndroidParameter.wParameterCount;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_ANDROID_SERVICE,SUB_GP_ANDROID_PARAMETER,&AndroidParameter,wSendDataSize);

	//�����ж�
	if(AndroidParameter.wParameterCount==1)
	{
		//Э������
		if(AndroidParameter.wSubCommdID==SUB_GP_ADD_PARAMETER)
		{
			//����ṹ
			CMD_CS_C_AddParameter AddParameter;
			AddParameter.wServerID=pAndroidParameter->wServerID;
			CopyMemory(&AddParameter.AndroidParameter,&AndroidParameter.AndroidParameter[0],sizeof(tagAndroidParameter));

			//��������
			m_pITCPSocketService->SendData(MDM_CS_ANDROID_SERVICE,SUB_CS_C_ADDPARAMETER,&AddParameter,sizeof(AddParameter));
		}

		//Э������
		if(AndroidParameter.wSubCommdID==SUB_GP_MODIFY_PARAMETER)
		{
			//����ṹ
			CMD_CS_C_ModifyParameter ModifyParameter;
			ModifyParameter.wServerID=pAndroidParameter->wServerID;
			CopyMemory(&ModifyParameter.AndroidParameter,&AndroidParameter.AndroidParameter[0],sizeof(tagAndroidParameter));

			//��������
			m_pITCPSocketService->SendData(MDM_CS_ANDROID_SERVICE,SUB_CS_C_MODIFYPARAMETER,&ModifyParameter,sizeof(ModifyParameter));
		}

		//Э������
		if(AndroidParameter.wSubCommdID==SUB_GP_DELETE_PARAMETER)
		{
			//����ṹ
			CMD_CS_C_DeleteParameter DeleteParameter;
			DeleteParameter.wServerID=pAndroidParameter->wServerID;
			DeleteParameter.dwBatchID=AndroidParameter.AndroidParameter[0].dwBatchID;

			//��������
			m_pITCPSocketService->SendData(MDM_CS_ANDROID_SERVICE,SUB_CS_C_DELETEPARAMETER,&DeleteParameter,sizeof(DeleteParameter));
		}
	}

	return true;
}

//�����ɹ�
bool CAttemperEngineSink::OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//��������
	DBO_GP_OperateSuccess * pOperateSuccess=(DBO_GP_OperateSuccess *)pData;

	//��������
	OperateSuccess.lResultCode=pOperateSuccess->lResultCode;
	lstrcpyn(OperateSuccess.szDescribeString,pOperateSuccess->szDescribeString,CountArray(OperateSuccess.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	//�ر�����
	if(pOperateSuccess->bCloseSocket==true)
	{
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	}

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//��������
	DBO_GP_OperateFailure * pOperateFailure=(DBO_GP_OperateFailure *)pData;

	//��������
	OperateFailure.lResultCode=pOperateFailure->lResultCode;
	lstrcpyn(OperateFailure.szDescribeString,pOperateFailure->szDescribeString,CountArray(OperateFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	//�ر�����
	if(pOperateFailure->bCloseSocket==true)
	{
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	}

	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_MB_LogonSuccess * pDBOLogonSuccess=(DBO_MB_LogonSuccess *)pData;

	//��������
	CMD_MB_LogonSuccess LogonSuccess;
	ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

	//��������
	LogonSuccess.wFaceID=pDBOLogonSuccess->wFaceID;
	LogonSuccess.cbGender=pDBOLogonSuccess->cbGender;
	LogonSuccess.dwCustomID=pDBOLogonSuccess->dwCustomID;
	LogonSuccess.dwGameID=pDBOLogonSuccess->dwGameID;
	LogonSuccess.dwUserID=pDBOLogonSuccess->dwUserID;
	LogonSuccess.dwExperience=pDBOLogonSuccess->dwExperience;
	LogonSuccess.dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	lstrcpyn(LogonSuccess.szAccounts,pDBOLogonSuccess->szAccounts,CountArray(LogonSuccess.szAccounts));
	lstrcpyn(LogonSuccess.szNickName,pDBOLogonSuccess->szNickName,CountArray(LogonSuccess.szNickName));
	lstrcpyn(LogonSuccess.szDynamicPass,pDBOLogonSuccess->szDynamicPass,CountArray(LogonSuccess.szDynamicPass));
	DWORD dwClientAddr=(m_pBindParameter+LOWORD(dwContextID))->dwClientAddr;
	BYTE * pClientAddr=(BYTE *)&dwClientAddr;
	_sntprintf(LogonSuccess.szLoginIP,CountArray(LogonSuccess.szLoginIP),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

	//�û��ɼ�
	LogonSuccess.lUserScore=pDBOLogonSuccess->lUserScore;
	LogonSuccess.lUserIngot=pDBOLogonSuccess->lUserIngot;
	LogonSuccess.lUserInsure=pDBOLogonSuccess->lUserInsure;
	LogonSuccess.dUserBeans=pDBOLogonSuccess->dUserBeans;

	//��չ��Ϣ
	LogonSuccess.cbInsureEnabled=pDBOLogonSuccess->cbInsureEnabled;	

	//��¼�ɹ�
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

	//���ͷ���
	WORD wIndex=LOWORD(dwContextID);
	SendMobileKindInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	SendMobileServerInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);

	//�����б�
	SendUserSignupInfo(dwContextID,pDBOLogonSuccess->wSignupCount,pDBOLogonSuccess->SignupMatchInfo,true);

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_SERVER_LIST,SUB_MB_LIST_FINISH);
	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_MB_LogonFailure * pLogonFailure=(DBO_MB_LogonFailure *)pData;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_FAILURE,&LogonFailure,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�齱����
bool CAttemperEngineSink::OnDBPCLotteryConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_LotteryConfig * pLotteryConfig=(DBO_GP_LotteryConfig *)pData;

	//��������
	CMD_GP_LotteryConfig LotteryConfig;
	ZeroMemory(&LotteryConfig,sizeof(LotteryConfig));

	//�������
	LotteryConfig.wLotteryCount=pLotteryConfig->wLotteryCount;
	CopyMemory(LotteryConfig.LotteryItem,pLotteryConfig->LotteryItem,sizeof(LotteryConfig.LotteryItem));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_LOTTERY_CONFIG,&LotteryConfig,sizeof(LotteryConfig));

	return true;
}

//�齱��Ϣ
bool CAttemperEngineSink::OnDBPCLotteryUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_LotteryUserInfo * pLotteryConfig=(DBO_GP_LotteryUserInfo *)pData;

	//��������
	CMD_GP_LotteryUserInfo LotteryUserInfo;
	ZeroMemory(&LotteryUserInfo,sizeof(LotteryUserInfo));

	//�������
	LotteryUserInfo.cbFreeCount=pLotteryConfig->cbFreeCount;
	LotteryUserInfo.cbAlreadyCount=pLotteryConfig->cbAlreadyCount;
	LotteryUserInfo.wKindID=pLotteryConfig->wKindID;
	LotteryUserInfo.dwUserID=pLotteryConfig->dwUserID;
	LotteryUserInfo.lChargeFee=pLotteryConfig->lChargeFee;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_LOTTERY_USER_INFO,&LotteryUserInfo,sizeof(LotteryUserInfo));

	return true;
}


bool CAttemperEngineSink::OnDBPCSimpleLotteryResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GP_SimpleLotteryResult * pLotteryResult = (DBO_GP_SimpleLotteryResult *)pData;
	CMD_GP_Noti noti;
	char buf[512] = {0};
	if (pLotteryResult->dwIndex == 1)
	{
		sprintf(noti.szStr,"��ϲ���[%s]ת�̳鵽��[%s]",pLotteryResult->szName,"888����");
		m_pITCPNetworkEngine->SendDataBatch( MDM_GP_USER_SERVICE, SUB_GP_LOTTERY_NOTI, &noti, sizeof(noti),0);
	}
	if (pLotteryResult->dwIndex == 3)
	{
		sprintf(noti.szStr,"��ϲ���[%s]ת�̳鵽��[%s]",pLotteryResult->szName,"999��ʯ");
		m_pITCPNetworkEngine->SendDataBatch( MDM_GP_USER_SERVICE, SUB_GP_LOTTERY_NOTI, &noti, sizeof(noti),0);
	}
	if (pLotteryResult->dwIndex == 6)
	{
		sprintf(noti.szStr,"��ϲ���[%s]ת�̳鵽��[%s]",pLotteryResult->szName,"10����");
		m_pITCPNetworkEngine->SendDataBatch( MDM_GP_USER_SERVICE, SUB_GP_LOTTERY_NOTI, &noti, sizeof(noti),0);
	}


	

	CMD_GP_SimpleLotteryResult  result;
	result.dwErrorCode = pLotteryResult->dwErrorCode;
	result.dwIndex = pLotteryResult->dwIndex;
	result.dwUserID = pLotteryResult->dwUserID;
	result.lLottery = pLotteryResult->lUserScore;
	result.lInsure = pLotteryResult->lInsure;
	result.lExp = pLotteryResult->lExp;
	//��������
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_SIMPLE_LOTTERY_RESULT, &result, sizeof(result));
	return true;
}
//�齱���
bool CAttemperEngineSink::OnDBPCLotteryResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_LotteryResult * pLotteryResult=(DBO_GP_LotteryResult *)pData;

	//��������
	CMD_GP_LotteryResult LotteryResult;
	ZeroMemory(&LotteryResult,sizeof(LotteryResult));

	//�������
	LotteryResult.bWined=pLotteryResult->bWined;
	LotteryResult.wKindID=pLotteryResult->wKindID;
	LotteryResult.dwUserID=pLotteryResult->dwUserID;
	LotteryResult.lUserScore=pLotteryResult->lUserScore;
	LotteryResult.dUserBeans=pLotteryResult->dUserBeans;
	CopyMemory(&LotteryResult.LotteryItem,&pLotteryResult->LotteryItem,sizeof(LotteryResult.LotteryItem));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_LOTTERY_RESULT,&LotteryResult,sizeof(LotteryResult));

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCQueryUserGameData(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_QueryUserGameData * pDBOQueryUserGameData=(DBO_GP_QueryUserGameData *)pData;
	CMD_GP_QueryUserGameData * pCMDQueryUserGameData=(CMD_GP_QueryUserGameData *)cbDataBuffer;
	CSendPacketHelper SendPacket(cbDataBuffer+sizeof(CMD_GP_QueryUserGameData),sizeof(cbDataBuffer)-sizeof(CMD_GP_QueryUserGameData));

	//���ñ���
	ZeroMemory(pCMDQueryUserGameData,sizeof(CMD_GP_QueryUserGameData));

	//�������
	pCMDQueryUserGameData->wKindID=pDBOQueryUserGameData->wKindID;
	pCMDQueryUserGameData->dwUserID=pDBOQueryUserGameData->dwUserID;

	//��ϵ��ַ
	if (pDBOQueryUserGameData->szUserGameData[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOQueryUserGameData->szUserGameData);
		SendPacket.AddPacket(pDBOQueryUserGameData->szUserGameData,wBufferSize,DTP_GP_UI_USER_GAME_DATA);
	}

	//������Ϣ
	WORD wSendSize=sizeof(CMD_GP_QueryUserGameData)+SendPacket.GetDataSize();
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_GAME_DATA,cbDataBuffer,wSendSize);

	return true;
}

bool CAttemperEngineSink::OnDBPCInGameSevrerID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	DBO_GP_UserInGameServerID * pDBInfo=(DBO_GP_UserInGameServerID *)pData;

	if(pDBInfo->LockKindID == 0) return true;
	if(pDBInfo->LockServerID == 0) return true;

	//��������
	CMD_GP_InGameSeverID kNetInfo;
	ZeroMemory(&kNetInfo,sizeof(kNetInfo));

	kNetInfo.LockKindID=pDBInfo->LockKindID;
	kNetInfo.LockServerID=pDBInfo->LockServerID;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_INGAME_SEVERID,&kNetInfo,sizeof(kNetInfo));

	return true;
}

//ϵͳQQ:DCY by 2017.3.9
bool CAttemperEngineSink::OnDBSystemQQ(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	DBO_GP_SystemQQ * pDBInfo=(DBO_GP_SystemQQ *)pData;

	//��������
	CMD_GP_QuerySystemQQ tmpResult;
	ZeroMemory(&tmpResult,sizeof(tmpResult));

	memcpy(tmpResult.szSystemQQ,pDBInfo->szSystemQQ,sizeof(tmpResult.szSystemQQ));

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_SYSTEMQQ,&tmpResult,sizeof(tmpResult));

	return true;
}

//ϵͳ����:DCY by 2017.3.9
bool CAttemperEngineSink::OnDBSystemInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	DBO_GP_SystemInfo * pDBInfo=(DBO_GP_SystemInfo *)pData;
	
	//��������
	CMD_GP_QuerySystemInfo tmpResult;
	ZeroMemory(&tmpResult,sizeof(tmpResult));

	memcpy(tmpResult.szSystemInfo,pDBInfo->szSystemInfo,sizeof(tmpResult.szSystemInfo));

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_SYSTEMINFO,&tmpResult,sizeof(tmpResult));
	return true;
}

//��ϵ����
bool CAttemperEngineSink::OnDBSystemContact(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	DBO_GP_SystemContact * pDBInfo=(DBO_GP_SystemContact *)pData;

	//��������
	CMD_GP_QuerySystemContact tmpResult;
	ZeroMemory(&tmpResult,sizeof(tmpResult));

	memcpy(tmpResult.szSystemContact,pDBInfo->szSystemContact,sizeof(tmpResult.szSystemContact));

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_SYSTEMCONTACT,&tmpResult,sizeof(tmpResult));
	return true;
}

//ϵͳ�̳�:DCY by 2017.3.11
bool CAttemperEngineSink::OnDBGameShop(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//���������:DCY by 2017.3.11-----------------------------------------------------------
	tagShopInfo * pDBInfo=(tagShopInfo *)pData;
	TCHAR szOutPut[300] = {0};
	_sntprintf(szOutPut,sizeof(szOutPut),TEXT("��Ʒ��Ϣ:һ��[%d]��,���Ա�THNativeWebDB.ShopInfo"),wDataSize/sizeof(tagShopInfo));
	//�������
	CTraceService::TraceString(szOutPut,TraceLevel_Exception);
	//���������:DCY by 2017.3.11-----------------------------------------------------------

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_SHOP,pData,wDataSize);
	return true;
}

bool CAttemperEngineSink::OnDBExchangeShop(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//���������:DCY by 2017.3.11-----------------------------------------------------------
// 	LONG lResult = *((LONG*)pData);
// 	TCHAR szOutPut[300] = {0};
// 	if (lResult == 1)
// 	{
// 		_sntprintf(szOutPut,sizeof(szOutPut),TEXT("�һ��ɹ�:���Ա�THNativeWebDB.ExchangeShop"));
// 	}
// 	else
// 	{
// 
// 		_sntprintf(szOutPut,sizeof(szOutPut),TEXT("�һ�ʧ��:���Ա�THNativeWebDB.ExchangeShop"));
// 	}
// 	//�������
// 	CTraceService::TraceString(szOutPut,TraceLevel_Exception);
	//���������:DCY by 2017.3.11-----------------------------------------------------------


	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_EXCHANGE_SHOP,pData,wDataSize);
	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameType)==0);
	if (wDataSize%sizeof(DBO_GP_GameType)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameType);
	DBO_GP_GameType * pGameType=(DBO_GP_GameType *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameType GameType;
		ZeroMemory(&GameType,sizeof(GameType));

		//��������
		GameType.wTypeID=(pGameType+i)->wTypeID;
		GameType.wJoinID=(pGameType+i)->wJoinID;
		GameType.wSortID=(pGameType+i)->wSortID;
		lstrcpyn(GameType.szTypeName,(pGameType+i)->szTypeName,CountArray(GameType.szTypeName));

		//�����б�
		m_ServerListManager.InsertGameType(&GameType);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameKind)==0);
	if (wDataSize%sizeof(DBO_GP_GameKind)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameKind);
	DBO_GP_GameKind * pGameKind=(DBO_GP_GameKind *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameKind GameKind;
		ZeroMemory(&GameKind,sizeof(GameKind));

		//��������
		GameKind.wTypeID=(pGameKind+i)->wTypeID;
		GameKind.wJoinID=(pGameKind+i)->wJoinID;
		GameKind.wSortID=(pGameKind+i)->wSortID;
		GameKind.wKindID=(pGameKind+i)->wKindID;
		GameKind.wGameID=(pGameKind+i)->wGameID;
		GameKind.cbSuportMobile=(pGameKind+i)->cbSuportMobile;
		GameKind.dwOnLineCount=m_ServerListManager.CollectOnlineInfo((pGameKind+i)->wKindID,false);
		GameKind.dwAndroidCount=m_ServerListManager.CollectOnlineInfo((pGameKind+i)->wKindID,true);
		lstrcpyn(GameKind.szKindName,(pGameKind+i)->szKindName,CountArray(GameKind.szKindName));
		lstrcpyn(GameKind.szProcessName,(pGameKind+i)->szProcessName,CountArray(GameKind.szProcessName));

		//�����б�
		m_ServerListManager.InsertGameKind(&GameKind);
	}

	return true;
}

//��Ϸ�ڵ�
bool CAttemperEngineSink::OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameNode)==0);
	if (wDataSize%sizeof(DBO_GP_GameNode)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameNode);
	DBO_GP_GameNode * pGameNode=(DBO_GP_GameNode *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameNode GameNode;
		ZeroMemory(&GameNode,sizeof(GameNode));

		//��������
		GameNode.wKindID=(pGameNode+i)->wKindID;
		GameNode.wJoinID=(pGameNode+i)->wJoinID;
		GameNode.wSortID=(pGameNode+i)->wSortID;
		GameNode.wNodeID=(pGameNode+i)->wNodeID;
		lstrcpyn(GameNode.szNodeName,(pGameNode+i)->szNodeName,CountArray(GameNode.szNodeName));

		//�����б�
		m_ServerListManager.InsertGameNode(&GameNode);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GamePage)==0);
	if (wDataSize%sizeof(DBO_GP_GamePage)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GamePage);
	DBO_GP_GamePage * pGamePage=(DBO_GP_GamePage *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGamePage GamePage;
		ZeroMemory(&GamePage,sizeof(GamePage));

		//��������
		GamePage.wKindID=(pGamePage+i)->wKindID;
		GamePage.wNodeID=(pGamePage+i)->wNodeID;
		GamePage.wSortID=(pGamePage+i)->wSortID;
		GamePage.wPageID=(pGamePage+i)->wPageID;
		GamePage.wOperateType=(pGamePage+i)->wOperateType;
		lstrcpyn(GamePage.szDisplayName,(pGamePage+i)->szDisplayName,CountArray(GamePage.szDisplayName));

		//�����б�
		m_ServerListManager.InsertGamePage(&GamePage);
	}

	return true;
}

//��Ϸ�б�
bool CAttemperEngineSink::OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GP_GameListResult));
	if (wDataSize!=sizeof(DBO_GP_GameListResult)) return false;

	//��������
	DBO_GP_GameListResult * pGameListResult=(DBO_GP_GameListResult *)pData;

	//��Ϣ����
	if (pGameListResult->cbSuccess==TRUE)
	{
		//�����б�
		m_ServerListManager.CleanKernelItem();

		//�¼�֪ͨ
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_LOAD_DB_LIST_RESULT,&ControlResult,sizeof(ControlResult));

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wLoadListTime*1000L,1,0);
	}
	else
	{
		//������ʾ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�������б����ʧ�ܣ�%ld ������¼���"),m_pInitParameter->m_wReLoadListTime);

		//��ʾ��Ϣ
		CTraceService::TraceString(szDescribe,TraceLevel_Warning);

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wReLoadListTime*1000L,1,0);
	}

	return true;
}

//�汾���
bool CAttemperEngineSink::CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer)
{
	//��������
	bool bMustUpdate=false;
	bool bAdviceUpdate=false;
	DWORD dwVersion=VERSION_PLAZA;

	//�ֻ��汾
	if(cbDeviceType >= DEVICE_TYPE_IPAD) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_IPHONE) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ITOUCH) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ANDROID) dwVersion=VERSION_MOBILE_ANDROID;
	else if(cbDeviceType == DEVICE_TYPE_PC) dwVersion=VERSION_PLAZA;

	//�汾�ж�
	if (bCheckLowVer && GetSubVer(dwPlazaVersion)<GetSubVer(dwVersion)) bAdviceUpdate=true;
	if (GetMainVer(dwPlazaVersion)!=GetMainVer(dwVersion)) bMustUpdate=true;
	if (GetProductVer(dwPlazaVersion)!=GetProductVer(dwVersion)) bMustUpdate=true;

	//�����ж�
	if ((bMustUpdate==true)||(bAdviceUpdate==true))
	{
		//��������
		CMD_GP_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//��������
		UpdateNotify.cbMustUpdate=bMustUpdate;
		UpdateNotify.cbAdviceUpdate=bAdviceUpdate;
		UpdateNotify.dwCurrentVersion=dwVersion;

		//������Ϣ
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//�ж��ж�
		if (bMustUpdate==true) 
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return false;
		}
	}

	return true;
}

//��������
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//��������
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//��������
VOID CAttemperEngineSink::SendGameTypeInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameTypeItem * pGameTypeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameTypeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameType))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameTypeItem=m_ServerListManager.EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameTypeItem->m_GameType,sizeof(tagGameType));
		wSendSize+=sizeof(tagGameType);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);

	return;
}

//��������
VOID CAttemperEngineSink::SendGameKindInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);

	return;
}

//���ͽڵ�
VOID CAttemperEngineSink::SendGameNodeInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameNodeItem * pGameNodeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameNodeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameNode))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameNodeItem=m_ServerListManager.EmunGameNodeItem(Position);
		if (pGameNodeItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameNodeItem->m_GameNode.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameNodeItem->m_GameNode,sizeof(tagGameNode));
			wSendSize+=sizeof(tagGameNode);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);

	return;
}

//���Ͷ���
VOID CAttemperEngineSink::SendGamePageInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGamePageItem * pGamePageItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGamePageCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGamePage))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGamePageItem=m_ServerListManager.EmunGamePageItem(Position);
		if (pGamePageItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGamePageItem->m_GamePage.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGamePageItem->m_GamePage,sizeof(tagGamePage));
			wSendSize+=sizeof(tagGamePage);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);

	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendGameServerInfo(DWORD dwSocketID, WORD wKindID,bool bOnlyMatch)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		if (pGameServerItem->IsMatchServer()==false && bOnlyMatch) continue;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);


    //���ñ���
	wSendSize=0;
	ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameMatch))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_MATCH,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		if (pGameServerItem->IsMatchServer()==false) continue;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameMatch,sizeof(tagGameMatch));
			wSendSize+=sizeof(tagGameMatch);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_MATCH,cbDataBuffer,wSendSize);

	if(m_wAVServerPort!=0 && m_dwAVServerAddr!=0)
	{
		//��������
		tagAVServerOption AVServerOption;
		AVServerOption.wAVServerPort=m_wAVServerPort;
		AVServerOption.dwAVServerAddr=m_dwAVServerAddr;

		//��������
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_VIDEO_OPTION,&AVServerOption,sizeof(AVServerOption));
	};

	return;
}

//���ͱ���
VOID CAttemperEngineSink::SendGameMatchInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameMatch))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_MATCH,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		if (pGameServerItem->IsMatchServer()==false) continue;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameMatch,sizeof(tagGameMatch));
			wSendSize+=sizeof(tagGameMatch);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_MATCH,cbDataBuffer,wSendSize);
}

//�ȼ�����
VOID CAttemperEngineSink::SendGrowLevelConfig(DWORD dwSocketID)
{
	//����ṹ
	CMD_GP_GrowLevelConfig GrowLevelConfig;
	GrowLevelConfig.wLevelCount = m_wLevelCount;
	CopyMemory(GrowLevelConfig.GrowLevelItem,m_GrowLevelConfig,sizeof(tagGrowLevelConfig)*GrowLevelConfig.wLevelCount);

	//��������
	WORD wDataSize = sizeof(GrowLevelConfig)-sizeof(GrowLevelConfig.GrowLevelItem);
	wDataSize += sizeof(tagGrowLevelConfig)*GrowLevelConfig.wLevelCount;
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_GROWLEVEL_CONFIG,&GrowLevelConfig,wDataSize);

	return;
}

//���ͱ���
VOID CAttemperEngineSink::SendUserSignupInfo(DWORD dwSocketID,WORD wSignupCount,tagSignupMatchInfo * pSignupMatchInfo,bool bMobile)
{
	//����У��
	if(wSignupCount==0 || pSignupMatchInfo==NULL) return;

	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//��������	
	while((wSignupCount*sizeof(tagSignupMatchInfo))>=sizeof(cbDataBuffer))
	{
		//��������
		wSendSize = (sizeof(cbDataBuffer)/sizeof(tagSignupMatchInfo))*sizeof(tagSignupMatchInfo);
		CopyMemory(cbDataBuffer,pSignupMatchInfo,wSendSize);

		//��������
		if (bMobile==false)
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_MATCH_SIGNUPINFO,cbDataBuffer,wSendSize);
		else
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_LOGON,SUB_MB_MATCH_SIGNUPINFO,cbDataBuffer,wSendSize);

		//����ָ��
		pSignupMatchInfo += sizeof(cbDataBuffer)/sizeof(tagSignupMatchInfo);
        wSignupCount -= sizeof(cbDataBuffer)/sizeof(tagSignupMatchInfo);		
	}

	//ʣ�෢��
	if(wSignupCount>0)
	{
		//��������
		wSendSize = wSignupCount*sizeof(tagSignupMatchInfo);
		CopyMemory(cbDataBuffer,pSignupMatchInfo,wSendSize);

		if (bMobile==false)
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_MATCH_SIGNUPINFO,cbDataBuffer,wSendSize);
		else
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_LOGON,SUB_MB_MATCH_SIGNUPINFO,cbDataBuffer,wSendSize);
	}

	return;
}

//��������
VOID CAttemperEngineSink::SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		//��������
		if (pGameKindItem->m_GameKind.cbSuportMobile==TRUE)
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
			wSendSize+=sizeof(tagGameKind);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_KIND,cbDataBuffer,wSendSize);

	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//��ȡ����
		BOOL cbSuportMobile=TRUE;
		pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
		if (pGameKindItem!=NULL) cbSuportMobile=pGameKindItem->m_GameKind.cbSuportMobile;

		//��������
		if ((cbSuportMobile==TRUE)&&((wModuleID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wModuleID)))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);

	//���ñ���
	wSendSize=0;
	ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameMatch))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_MATCH,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		if (pGameServerItem->IsMatchServer()==false) continue;

		//��ȡ����
		BOOL cbSuportMobile=TRUE;
		pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
		if (pGameKindItem!=NULL) cbSuportMobile=pGameKindItem->m_GameKind.cbSuportMobile;

		//��������
		if ((cbSuportMobile==TRUE)&&((wModuleID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wModuleID)))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameMatch,sizeof(tagGameMatch));
			wSendSize+=sizeof(tagGameMatch);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_MATCH,cbDataBuffer,wSendSize);

	return;
}

//����ʧ��
bool CAttemperEngineSink::SendInsureFailure(DWORD dwSocketID, LONG lResultCode, LPCTSTR pszDescribe)
{
	//��������
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//��������
	UserInsureFailure.lResultCode=lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pszDescribe,CountArray(UserInsureFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

	return true;
}

//�����ɹ�
VOID CAttemperEngineSink::SendOperateSuccess(DWORD dwContextID, LONG lResultCode, LPCTSTR pszDescribe)
{
	//Ч�����
	ASSERT(pszDescribe != NULL);
	if(pszDescribe == NULL) return;

	//��������
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//��������
	OperateSuccess.lResultCode=lResultCode;
	lstrcpyn(OperateSuccess.szDescribeString,pszDescribe,CountArray(OperateSuccess.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return;
}

//����ʧ��
VOID CAttemperEngineSink::SendOperateFailure(DWORD dwContextID, LONG lResultCode, LPCTSTR pszDescribe)
{
	//Ч�����
	ASSERT(pszDescribe != NULL);
	if(pszDescribe == NULL) return;

	//��������
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//��������
	OperateFailure.lResultCode=lResultCode;
	lstrcpyn(OperateFailure.szDescribeString,pszDescribe,CountArray(OperateFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return;
}

//������֤��
VOID CAttemperEngineSink::RandVerifyCode(LPTSTR pszVerifyCode, UINT nMaxCount)
{
	ASSERT(pszVerifyCode!=NULL && nMaxCount>=LEN_VERIFY_CODE);
	if (pszVerifyCode==NULL || nMaxCount<LEN_VERIFY_CODE) return;

	for (UINT i=0; i<nMaxCount-1; i++)  
	{  
		INT nflag=rand()%3;  
		switch (nflag)  
		{  
		case 0:  
			pszVerifyCode[i]=TEXT('A')+rand()%26;  
			break;  
		case 1:  
			pszVerifyCode[i]=TEXT('a')+rand()%26;  
			break;  
		case 2:  
			pszVerifyCode[i]=TEXT('0')+rand()%10;  
			break;  
		default:  
			pszVerifyCode[i]=TEXT('x');  
			break;  
		}  
	}  
	pszVerifyCode[nMaxCount-1]=TEXT('\0');  

	return;
}

//Ч����֤
bool CAttemperEngineSink::CheckVerifyCode(LPCTSTR pszValidateCode, LPCTSTR pszValidateCodeBak)
{
	//������֤
	if (pszValidateCode==NULL || pszValidateCodeBak==NULL) return false;

	//���ȶԱ�
	INT nTargetLen=lstrlen(pszValidateCode);
	INT nSourceLen=lstrlen(pszValidateCodeBak);

	//����Ա�
	if (nTargetLen!=nSourceLen) return false;
	if (CompareString(LOCALE_SYSTEM_DEFAULT,NORM_IGNORECASE,pszValidateCode,nTargetLen,pszValidateCodeBak,nSourceLen)!=CSTR_EQUAL) return false;

	return true;
}

//������֤��
VOID CAttemperEngineSink::SendVerifyCode(DWORD dwSocketID, LPCTSTR pszVerifyCode, bool bCloseSocket)
{
	//Ч�����
	ASSERT(pszVerifyCode != NULL);
	if(pszVerifyCode == NULL) return;

	//��������
	CMD_GP_QueryVerifyCode QueryVerifyCode;
	ZeroMemory(&QueryVerifyCode,sizeof(QueryVerifyCode));

	//��������
	QueryVerifyCode.bCloseSocket=bCloseSocket;
	lstrcpyn(QueryVerifyCode.szVerifyCode,pszVerifyCode,CountArray(QueryVerifyCode.szVerifyCode));

	//��������
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_QUERY_VERIFY_CODE,&QueryVerifyCode,sizeof(QueryVerifyCode));

	return;
}


//�û�������Ϣ
bool CAttemperEngineSink::OnDBPCUserAccountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserAccountInfo * pDBOUserIndividual=(DBO_GP_UserAccountInfo *)pData;
	//CMD_GP_UserAccountInfo kCMDUserIndividual;
	//CopyMemory(&kCMDUserIndividual,pDBOUserIndividual,sizeof(kCMDUserIndividual));
	CMD_GP_QueryAccountInfo_Result kCMDUserIndividual;
	kCMDUserIndividual.dwUserID = pDBOUserIndividual->dwUserID;
	kCMDUserIndividual.score = pDBOUserIndividual->lUserScore;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_ACCOUNTINFO,&kCMDUserIndividual,sizeof(kCMDUserIndividual));
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
