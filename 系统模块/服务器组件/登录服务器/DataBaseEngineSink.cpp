#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//�������
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//��������
CDataBaseEngineSink::~CDataBaseEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//��������
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_TreasureDBModule.GetInterface()==NULL)&&(m_TreasureDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_PlatformDBModule.GetInterface()==NULL)&&(m_PlatformDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	try
	{
		//������Ϣ
		tagDataBaseParameter * pAccountsDBParameter=&m_pInitParameter->m_AccountsDBParameter;
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
		tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

		//��������
		m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr,pAccountsDBParameter->wDataBasePort,
			pAccountsDBParameter->szDataBaseName,pAccountsDBParameter->szDataBaseUser,pAccountsDBParameter->szDataBasePass);
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
			pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass);
		m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
			pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass);

		//��������
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		//��������
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//��������
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return true;
}

//ֹͣ�¼�
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//���ö���
	m_AccountsDBAide.SetDataBase(NULL);
	m_TreasureDBAide.SetDataBase(NULL);
	m_PlatformDBAide.SetDataBase(NULL);

	//�ر�����
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
	}

	//�ر�����
	if (m_TreasureDBModule.GetInterface()!=NULL)
	{
		m_TreasureDBModule->CloseConnection();
		m_TreasureDBModule.CloseInstance();
	}

	//�ر�����
	if (m_PlatformDBModule.GetInterface()!=NULL)
	{
		m_PlatformDBModule->CloseConnection();
		m_PlatformDBModule.CloseInstance();
	}

	//�������
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return true;
}

//ʱ���¼�
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GP_LOGON_GAMEID:			//��ʶ��¼
		{
			return OnRequestLogonGameID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_ACCOUNTS:			//�ʺŵ�¼
		{
			return OnRequestLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			return OnRequestRegisterAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_PLATFORM_PARAMETER: //ƽ̨����
		{
			return OnRequestPlatformParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_VERIFY_INDIVIDUAL:		//��֤����
		{
			return OnRequestVerifyIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_VISITOR:			//�ο͵�½
		{
			return OnRequestLogonVisitor(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_MACHINE:			//�޸Ļ���
		{
			return OnRequestModifyMachine(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_LOGON_PASS:		//�޸�����
		{
			return OnRequestModifyLogonPass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INSURE_PASS:		//�޸�����
		{
			return OnRequestModifyInsurePass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_UNDER_WRITE:		//�޸�ǩ��
		{
			return OnRequestModifyUnderWrite(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_SYSTEM_FACE:		//�޸�ͷ��
		{
			return OnRequestModifySystemFace(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_CUSTOM_FACE:		//�޸�ͷ��
		{
			return OnRequestModifyCustomFace(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INDIVIDUAL:		//�޸�����
		{
			return OnRequestModifyIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_ENABLE_INSURE:		//��ͨ����
		{
			return OnRequestUserEnableInsure(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_SAVE_SCORE:		//������Ϸ��
		{
			return OnRequestUserSaveScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TAKE_SCORE:		//��ȡ��Ϸ��
		{
			return OnRequestUserTakeScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TRANSFER_SCORE:	//ת����Ϸ��
		{
			return OnRequestUserTransferScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INDIVIDUAL:		//��ѯ����
		{
			return OnRequestQueryIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_ACCOUNTINFO:		//��ѯ����
		{
			return OnRequestQueryAccountInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INSURE_INFO:		//��ѯ����
		{
			return OnRequestQueryInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_USER_INFO:		//��ѯ�û�
		{
			return OnRequestQueryTransferUserInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_GAME_LIST:			//�����б�
		{
			return OnRequestLoadGameList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ONLINE_COUNT_INFO:		//������Ϣ
		{
			return OnRequestOnLineCountInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_CHECKIN_REWARD:	//ǩ������
		{
			return OnRequestCheckInReward(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_TASK_LIST:			//��������
		{
			return OnRequestLoadTaskList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_BASEENSURE:		//���صͱ�
		{
			return OnRequestLoadBaseEnsure(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_SPREAD_INFO:		//�ƹ���Ϣ
		{
			return OnRequestQuerySpreadInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_PARAMETER:			//��ȡ����
		{
			return OnRequestGetParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ADD_PARAMETER:			//��Ӳ���
		{
			return OnRequestAddParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_PARAMETER:		//�޸Ĳ���
		{
			return OnRequestModifyParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_DELETE_PARAMETER:		//ɾ������
		{
			return OnRequestDeleteParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHECKIN_QUERY_INFO:		//��ѯǩ��
		{
			return OnRequestCheckInQueryInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHECKIN_DONE:			//ִ��ǩ��
		{
			return OnRequestCheckInDone(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TASK_QUERY_INFO:		//��ѯ����
		{
			return OnRequestTaskQueryInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TASK_TAKE:				//��ȡ����
		{
			return OnRequestTaskTake(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TASK_REWARD:			//��ȡ����
		{
			return OnRequestTaskReward(dwContextID,pData,wDataSize);
		}
	case DBR_GP_BASEENSURE_TAKE:		//��ȡ�ͱ�
		{
			return OnRequestTakeBaseEnsure(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GROWLEVEL_QUERY_IFNO:	//��ѯ�ȼ�
		{
			return OnRequestQueryGrowLevelParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_MEMBER_PARAMETER:	//��Ա����
		{
			return OnRequestLoadMemberParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_GROWLEVEL_CONFIG:	//�ȼ�����
		{
			return OnRequestLoadGrowLevelConfig(dwContextID,pData,wDataSize);
		}
	case DBR_GP_PURCHASE_MEMBER:		//�����Ա
		{
			return OnRequestPurchaseMember(dwContextID,pData,wDataSize);
		}
	case DBR_GP_EXCHANGE_SCORE_INGOT:	//�һ���Ϸ��
		{
			return OnRequestExchangeScoreByIngot(dwContextID,pData,wDataSize);
		}
	case DBR_GP_EXCHANGE_SCORE_BEANS:	//�һ���Ϸ��
		{
			return OnRequestExchangeScoreByBeans(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_GAMEID:			//��ʶ��¼
		{
			return OnMobileLogonGameID(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_ACCOUNTS:			//�ʺŵ�¼
		{
			return OnMobileLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_MB_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			return OnMobileRegisterAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_OTHERPLATFORM:	//������¼
		{
			return OnMobileLogonOtherPlatform(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_VISITOR:			//�ο͵�½
		{
			return OnMobileLogonVisitor(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MATCH_SIGNUP:			//��������
		{
			return OnRequestMatchSignup(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MATCH_UNSIGNUP:			//ȡ������
		{
			return OnRequestMatchUnSignup(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOTTERY_CONFIG_REQ:		//��������
		{
			return OnRequestLotteryConfigReq(dwContextID,pData,wDataSize);
		}

	case DBR_GP_SIMPLE_LOTTERY:
		{
		return OnRequestSimpleLotteryStart(dwContextID, pData, wDataSize);
		}
	case DBR_GP_LOTTERY_START:			//�齱��ʼ
		{
			return OnRequestLotteryStart(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_USER_GAME_DATA:	//��Ϸ����
		{
			return OnRequestQueryUserGameData(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_INGAME_SERVERID:		//��Ϸ״̬
		{
			return OnRequestUserInGameServerID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_SYSTEM_QQ:			//ϵͳQQ:DCY by 2017.3.9
		{
			return OnRequestSystemQQ(dwContextID);
		}
	case DBR_GP_QUERY_SYSTEM_INFO:			//ϵͳ��Ϣ:DCY by 2017.3.9
		{
			return OnRequestSystemInfo(dwContextID);
		}
	case DBR_GP_QUERY_SYSTEM_CONTACT:
		{
			return OnRequestSystemContact(dwContextID);
		}
	case DBR_GP_QUERY_SYSTEM_SHOP:			//ϵͳ�̳�:DCY by 2017.3.11
		{
			return OnRequestGameShop(dwContextID);
		}
	case DBR_GP_EXCHANGE_GAME_SHOP:			//�һ���Ʒ:DCY by 2017.3.11
		{
			return OnRequestExchangeShop(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//I D ��¼
bool CDataBaseEngineSink::OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_LogonGameID));
		if (wDataSize!=sizeof(DBR_GP_LogonGameID)) return false;

		//ִ�в�ѯ
		DBR_GP_LogonGameID * pLogonGameID=(DBR_GP_LogonGameID *)pData;

		//��������
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonGameID->cbNeeValidateMBCard);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,false);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_LogonAccounts));
		if (wDataSize!=sizeof(DBR_GP_LogonAccounts)) return false;

		//������
		DBR_GP_LogonAccounts * pLogonAccounts=(DBR_GP_LogonAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//��������
		LONG lResultCode=DB_SUCCESS;

		//�������
		TCHAR szDescribeString[128]=TEXT("");

		//���Ȩ��
		//if(pLogonAccounts->dwCheckUserRight!=0)
		//{
		//	//�������
		//	m_AccountsDBAide.ResetParameter();
		//	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		//	m_AccountsDBAide.AddParameter(TEXT("@dwcheckRight"),pLogonAccounts->dwCheckUserRight);
		//	m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//	//ִ�й���
		//	lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CheckUserRight"),false);
		//}

		//��֤�ʺ�
		if(lResultCode==DB_SUCCESS)
		{
			//�������
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
			m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
			m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
			m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);		
			m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pLogonAccounts->szPassPortID);
			m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

			//ִ�в�ѯ
			lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);
		}

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),pLogonAccounts->dwCheckUserRight,false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,false);

		return false;
	}

	return true;
}
//ע�ᴦ��
bool CDataBaseEngineSink::OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

		//������
		DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,false);

		return false;
	}

	return true;
}

//��֤����
bool CDataBaseEngineSink::OnRequestVerifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_VerifyIndividual));
		if (wDataSize!=sizeof(DBR_GP_VerifyIndividual)) return false;

		//������
		DBR_GP_VerifyIndividual * pVerifyIndividual=(DBR_GP_VerifyIndividual *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pVerifyIndividual->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@wVerifyMask"),pVerifyIndividual->wVerifyMask);
		m_AccountsDBAide.AddParameter(TEXT("@strVerifyContent"),pVerifyIndividual->szVerifyContent);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_VerifyIndividual"),false);

		//�������
		DBO_GP_VerifyIndividualResult VerifyIndividualResult;
		VerifyIndividualResult.szErrorMsg[0]=0;
		VerifyIndividualResult.wVerifyMask=pVerifyIndividual->wVerifyMask;
		VerifyIndividualResult.bVerifyPassage=lResultCode==DB_SUCCESS;

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		lstrcpyn(VerifyIndividualResult.szErrorMsg,CW2CT(DBVarValue.bstrVal),CountArray(VerifyIndividualResult.szErrorMsg));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(VerifyIndividualResult.szErrorMsg);
		WORD wHeadSize=sizeof(VerifyIndividualResult)-sizeof(VerifyIndividualResult.szErrorMsg);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VERIFY_RESULT,dwContextID,&VerifyIndividualResult,wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		DBR_GP_VerifyIndividual * pVerifyIndividual=(DBR_GP_VerifyIndividual *)pData;

		//�������
		DBO_GP_VerifyIndividualResult VerifyIndividualResult;
		VerifyIndividualResult.bVerifyPassage=false;
		VerifyIndividualResult.wVerifyMask=pVerifyIndividual->wVerifyMask;		
		lstrcpyn(VerifyIndividualResult.szErrorMsg,TEXT("���ݿ�����쳣,��֤ʧ��!"),CountArray(VerifyIndividualResult.szErrorMsg));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(VerifyIndividualResult.szErrorMsg);
		WORD wHeadSize=sizeof(VerifyIndividualResult)-sizeof(VerifyIndividualResult.szErrorMsg);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VERIFY_RESULT,dwContextID,&VerifyIndividualResult,wHeadSize+wDataSize);

		return false;
	}

	return true;
}

//�ο͵�¼
bool CDataBaseEngineSink::OnRequestLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_LogonVisitor));
		if (wDataSize!=sizeof(DBR_GP_LogonVisitor)) return false;

		//������
		DBR_GP_LogonVisitor * pVisitorLogon=(DBR_GP_LogonVisitor *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pVisitorLogon->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pVisitorLogon->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@cbPlatformID"),pVisitorLogon->cbPlatformID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pVisitorLogon->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyLogonVisitor"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,false);

		return false;
	}

	return true;
}

//I D ��¼
bool CDataBaseEngineSink::OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_LogonGameID));
		if (wDataSize!=sizeof(DBR_MB_LogonGameID)) return false;

		//ִ�в�ѯ
		DBR_MB_LogonGameID * pLogonGameID=(DBR_MB_LogonGameID *)pData;

		//��������
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonGameID->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyGameID"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,true);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_LogonAccounts));
		if (wDataSize!=sizeof(DBR_MB_LogonAccounts)) return false;

		//������
		DBR_MB_LogonAccounts * pLogonAccounts=(DBR_MB_LogonAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,true);

		return false;
	}

	return true;
}

//������¼
bool CDataBaseEngineSink::OnMobileLogonOtherPlatform(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_LogonOtherPlatform));
		if (wDataSize!=sizeof(DBR_MB_LogonOtherPlatform)) return false;

		//������
		DBR_MB_LogonOtherPlatform * pLogonOtherPlatform=(DBR_MB_LogonOtherPlatform *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonOtherPlatform->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonOtherPlatform->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@cbPlatformID"),pLogonOtherPlatform->cbPlatformID);
		m_AccountsDBAide.AddParameter(TEXT("@strUserUin"),pLogonOtherPlatform->szUserUin);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pLogonOtherPlatform->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pLogonOtherPlatform->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pLogonOtherPlatform->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonOtherPlatform->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonOtherPlatform->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyOtherPlatform"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,true);

		return false;
	}

	return true;
}

//�ο͵�¼
bool CDataBaseEngineSink::OnMobileLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_LogonVisitor));
		if (wDataSize!=sizeof(DBR_MB_LogonVisitor)) return false;

		//������
		DBR_MB_LogonVisitor * pVisitorLogon=(DBR_MB_LogonVisitor *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pVisitorLogon->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pVisitorLogon->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@cbPlatformID"),pVisitorLogon->cbPlatformID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pVisitorLogon->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pVisitorLogon->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyLogonVisitor"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,true);

		return false;
	}

	return true;
}

//�ʺ�ע��
bool CDataBaseEngineSink::OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_MB_RegisterAccounts)) return false;

		//������
		DBR_MB_RegisterAccounts * pRegisterAccounts=(DBR_MB_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,true);

		return false;
	}

	return true;
}

//�޸Ļ���
bool CDataBaseEngineSink::OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyMachine));
		if (wDataSize!=sizeof(DBR_GP_ModifyMachine)) return false;

		//������
		DBR_GP_ModifyMachine * pModifyMachine=(DBR_GP_ModifyMachine *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

		//�������
		TCHAR szDescribe[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�󶨲���
		if (pModifyMachine->cbBind==TRUE)
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachine"),false);
		}
		else
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UnMoorMachine"),false);
		}

		//�������
		if (m_AccountsDBAide.GetReturnValue()==DB_SUCCESS)
		{
			//��������
			DBO_GP_OperateSuccess OperateSuccess;
			ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateSuccess.lResultCode=m_AccountsDBModule->GetReturnValue();
			lstrcpyn(OperateSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateSuccess.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateSuccess.szDescribeString);
			WORD wSendSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wSendSize);
		}
		else
		{
			//��������
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateFailure.lResultCode=m_AccountsDBModule->GetReturnValue();
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifySystemFace));
		if (wDataSize!=sizeof(DBR_GP_ModifySystemFace)) return false;

		//������
		DBR_GP_ModifySystemFace * pModifySystemFace=(DBR_GP_ModifySystemFace *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifySystemFace->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifySystemFace->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifySystemFace->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pModifySystemFace->wFaceID);

		//������Ϣ
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifySystemFace->szMachineID);

		//�������
		TCHAR szDescribe[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SystemFaceInsert"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

			//��ȡ��Ϣ
			UserFaceInfo.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyCustomFace));
		if (wDataSize!=sizeof(DBR_GP_ModifyCustomFace)) return false;

		//������
		DBR_GP_ModifyCustomFace * pModifyCustomFace=(DBR_GP_ModifyCustomFace *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyCustomFace->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//��������
		SAFEARRAYBOUND SafeArrayBound;
		ZeroMemory(&SafeArrayBound,sizeof(SafeArrayBound));

		//���ñ���
		SafeArrayBound.lLbound=0L;
		SafeArrayBound.cElements=sizeof(pModifyCustomFace->dwCustomFace);

		//��������
		BYTE * pcbCustomFace=(BYTE *)pModifyCustomFace->dwCustomFace;
		SAFEARRAY * pSafeArray=SafeArrayCreate(VT_UI1,1,&SafeArrayBound);

		//��������
		for (LONG nIndex=0;nIndex<sizeof(pModifyCustomFace->dwCustomFace);nIndex++)
		{
			SafeArrayPutElement(pSafeArray,&nIndex,pcbCustomFace+nIndex);
		}

		//���ñ���
		VARIANT VarChunk;
		VarChunk.parray=pSafeArray;
		VarChunk.vt=VT_ARRAY|VT_UI1;

		//��ȡ����
		ASSERT(m_AccountsDBModule.GetInterface()!=NULL);
		IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyCustomFace->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyCustomFace->szPassword);

		//ͷ����Ϣ
		pIDataBase->AddParameter(TEXT("@dwCustomFace"),adLongVarBinary,adParamInput,sizeof(pModifyCustomFace->dwCustomFace),CDBVarValue(VarChunk));

		//������Ϣ
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyCustomFace->szMachineID);

		//�������
		TCHAR szDescribe[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�ͷ���Դ
		SafeArrayDestroy(pSafeArray);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CustomFaceInsert"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

			//��ȡ��Ϣ
			UserFaceInfo.wFaceID=INVALID_WORD;
			UserFaceInfo.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyLogonPass));
		if (wDataSize!=sizeof(DBR_GP_ModifyLogonPass)) return false;

		//������
		DBR_GP_ModifyLogonPass * pModifyLogonPass=(DBR_GP_ModifyLogonPass *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyLogonPass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyLogonPass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyLogonPass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyLogonPass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribe[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyLogonPassword"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyInsurePass));
		if (wDataSize!=sizeof(DBR_GP_ModifyInsurePass)) return false;

		//������
		DBR_GP_ModifyInsurePass * pModifyInsurePass=(DBR_GP_ModifyInsurePass *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyInsurePass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribe[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyInsurePassword"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ǩ��
bool CDataBaseEngineSink::OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyUnderWrite));
		if (wDataSize!=sizeof(DBR_GP_ModifyUnderWrite)) return false;

		//������
		DBR_GP_ModifyUnderWrite * pModifyUnderWrite=(DBR_GP_ModifyUnderWrite *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyUnderWrite->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyUnderWrite->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyUnderWrite->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyUnderWrite->szUnderWrite);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribe[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUnderWrite"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyIndividual));
		if (wDataSize!=sizeof(DBR_GP_ModifyIndividual)) return false;

		//������
		DBR_GP_ModifyIndividual * pModifyIndividual=(DBR_GP_ModifyIndividual *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyIndividual->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pModifyIndividual->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pModifyIndividual->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyIndividual->szUnderWrite);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pModifyIndividual->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strQQ"),pModifyIndividual->szQQ);
		m_AccountsDBAide.AddParameter(TEXT("@strEMail"),pModifyIndividual->szEMail);
		m_AccountsDBAide.AddParameter(TEXT("@strSeatPhone"),pModifyIndividual->szSeatPhone);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pModifyIndividual->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strDwellingPlace"),pModifyIndividual->szDwellingPlace);
		m_AccountsDBAide.AddParameter(TEXT("@strUserNote"),pModifyIndividual->szUserNote);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribe[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUserIndividual"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ͨ����
bool CDataBaseEngineSink::OnRequestUserEnableInsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserEnableInsure));
		if (wDataSize!=sizeof(DBR_GP_UserEnableInsure)) return false;

		//������
		DBR_GP_UserEnableInsure * pUserEnableInsure=(DBR_GP_UserEnableInsure *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserEnableInsure->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserEnableInsure->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strLogonPass"),pUserEnableInsure->szLogonPass);
		m_TreasureDBAide.AddParameter(TEXT("@strInsurePass"),pUserEnableInsure->szInsurePass);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserEnableInsure->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserEnableInsure"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		//�������
		DBO_GP_UserInsureEnableResult UserEnableInsureResult;
		ZeroMemory(&UserEnableInsureResult,sizeof(UserEnableInsureResult));

		//���ñ���
		UserEnableInsureResult.cbInsureEnabled=(lResultCode==DB_SUCCESS)?TRUE:FALSE;
		lstrcpyn(UserEnableInsureResult.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(UserEnableInsureResult.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserEnableInsureResult.szDescribeString);
		WORD wHeadSize=sizeof(UserEnableInsureResult)-sizeof(UserEnableInsureResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_ENABLE_RESULT,dwContextID,&UserEnableInsureResult,wHeadSize+wDataSize);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		DBO_GP_UserInsureEnableResult UserEnableInsureResult;
		ZeroMemory(&UserEnableInsureResult,sizeof(UserEnableInsureResult));

		//���ñ���
		UserEnableInsureResult.cbInsureEnabled=FALSE;
		lstrcpyn(UserEnableInsureResult.szDescribeString,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(UserEnableInsureResult.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserEnableInsureResult.szDescribeString);
		WORD wHeadSize=sizeof(UserEnableInsureResult)-sizeof(UserEnableInsureResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_ENABLE_RESULT,dwContextID,&UserEnableInsureResult,wHeadSize+wDataSize);

		return false;
	}
}

//������Ϸ��
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserSaveScore));
		if (wDataSize!=sizeof(DBR_GP_UserSaveScore)) return false;

		//������
		DBR_GP_UserSaveScore * pUserSaveScore=(DBR_GP_UserSaveScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserSaveScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ȡ��Ϸ��
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserTakeScore));
		if (wDataSize!=sizeof(DBR_GP_UserTakeScore)) return false;

		//������
		DBR_GP_UserTakeScore * pUserTakeScore=(DBR_GP_UserTakeScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTakeScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//ת����Ϸ��
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserTransferScore));
		if (wDataSize!=sizeof(DBR_GP_UserTransferScore)) return false;

		//������
		DBR_GP_UserTransferScore * pUserTransferScore=(DBR_GP_UserTransferScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransferScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTransferScore"),pUserTransferScore->lTransferScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTransferScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pUserTransferScore->szAccounts);
		m_TreasureDBAide.AddParameter(TEXT("@strTransRemark"),pUserTransferScore->szTransRemark);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransferScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}


//��������
bool CDataBaseEngineSink::OnRequestMatchSignup(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�����ʶ
	WORD wServerID=0;

	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_MatchSignup));
		if (wDataSize!=sizeof(DBR_GP_MatchSignup)) return false;

		//������
		DBR_GP_MatchSignup * pMatchSignup=(DBR_GP_MatchSignup *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pMatchSignup->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pMatchSignup->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pMatchSignup->szPassword);
		m_PlatformDBAide.AddParameter(TEXT("@wServerID"),pMatchSignup->wServerID);
		m_PlatformDBAide.AddParameter(TEXT("@dwMatchID"),pMatchSignup->dwMatchID);
		m_PlatformDBAide.AddParameter(TEXT("@dwMatchNO"),pMatchSignup->dwMatchNO);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pMatchSignup->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�����ʶ
		wServerID=pMatchSignup->wServerID;

		//ִ�в�ѯ
		LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_DeductMatchFee"),true);

		//�������
		CDBVarValue DBVarValue;
		m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		//������
		DBO_GP_MatchSignupResult MatchSignupResult;
		ZeroMemory(&MatchSignupResult,sizeof(MatchSignupResult));
		MatchSignupResult.bSignup = true;
		MatchSignupResult.bSuccessed = lResultCode==DB_SUCCESS;
		MatchSignupResult.wServerID=wServerID;
		if (MatchSignupResult.bSuccessed) MatchSignupResult.lCurrScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("UserScore"));
		lstrcpyn(MatchSignupResult.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(MatchSignupResult.szDescribeString));

		//Ͷ�ݽ��
		WORD wDataSize=CountStringBuffer(MatchSignupResult.szDescribeString);
		WORD wHeadSize=sizeof(MatchSignupResult)-sizeof(MatchSignupResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MATCH_SIGNUP_RESULT,dwContextID,&MatchSignupResult,wHeadSize+wDataSize);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		DBO_GP_MatchSignupResult MatchSignupResult;
		MatchSignupResult.bSignup = true;
		MatchSignupResult.bSuccessed = false;
		MatchSignupResult.wServerID=wServerID;
		MatchSignupResult.lCurrScore = 0;
		lstrcpyn(MatchSignupResult.szDescribeString,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(MatchSignupResult.szDescribeString));

		//Ͷ�ݽ��
		WORD wDataSize=CountStringBuffer(MatchSignupResult.szDescribeString);
		WORD wHeadSize=sizeof(MatchSignupResult)-sizeof(MatchSignupResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MATCH_SIGNUP_RESULT,dwContextID,&MatchSignupResult,wHeadSize+wDataSize);

		return false;
	}

	return true;
}

//ȡ������
bool CDataBaseEngineSink::OnRequestMatchUnSignup(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�����ʶ
	WORD wServerID=0;

	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_MatchUnSignup));
		if (wDataSize!=sizeof(DBR_GP_MatchUnSignup)) return false;

		//������
		DBR_GP_MatchUnSignup * pMatchUnSignup=(DBR_GP_MatchUnSignup *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pMatchUnSignup->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pMatchUnSignup->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pMatchUnSignup->szPassword);
		m_PlatformDBAide.AddParameter(TEXT("@wServerID"),pMatchUnSignup->wServerID);
		m_PlatformDBAide.AddParameter(TEXT("@dwMatchID"),pMatchUnSignup->dwMatchID);
		m_PlatformDBAide.AddParameter(TEXT("@dwMatchNO"),pMatchUnSignup->dwMatchNO);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pMatchUnSignup->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�����ʶ
		wServerID=pMatchUnSignup->wServerID;

		//ִ�в�ѯ
		LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_ReturnMatchFee"),true);

		//�������
		CDBVarValue DBVarValue;
		m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		
		//������
		DBO_GP_MatchSignupResult MatchSignupResult;
		ZeroMemory(&MatchSignupResult,sizeof(MatchSignupResult));
		MatchSignupResult.bSignup = false;
		MatchSignupResult.bSuccessed = lResultCode==DB_SUCCESS;
		MatchSignupResult.wServerID = wServerID;
		if (MatchSignupResult.bSuccessed) MatchSignupResult.lCurrScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("UserScore"));
		lstrcpyn(MatchSignupResult.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(MatchSignupResult.szDescribeString));

		//Ͷ�ݽ��
		WORD wDataSize=CountStringBuffer(MatchSignupResult.szDescribeString);
		WORD wHeadSize=sizeof(MatchSignupResult)-sizeof(MatchSignupResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MATCH_SIGNUP_RESULT,dwContextID,&MatchSignupResult,wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		////������
		//OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT(""),false);

		//������
		DBO_GP_MatchSignupResult MatchSignupResult;
		MatchSignupResult.bSignup = false;
		MatchSignupResult.bSuccessed = false;
		MatchSignupResult.wServerID = wServerID;
		MatchSignupResult.lCurrScore = 0;
		lstrcpyn(MatchSignupResult.szDescribeString,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(MatchSignupResult.szDescribeString));

		//Ͷ�ݽ��
		WORD wDataSize=CountStringBuffer(MatchSignupResult.szDescribeString);
		WORD wHeadSize=sizeof(MatchSignupResult)-sizeof(MatchSignupResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MATCH_SIGNUP_RESULT,dwContextID,&MatchSignupResult,wHeadSize+wDataSize);

		return false;
	}

	return true;
}

//��������
bool CDataBaseEngineSink::OnRequestLotteryConfigReq(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_LotteryConfigReq));
		if (wDataSize!=sizeof(DBR_GP_LotteryConfigReq)) return false;

		//������
		DBR_GP_LotteryConfigReq * pLotteryConfigReq=(DBR_GP_LotteryConfigReq *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLotteryConfigReq->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@wKindID"),pLotteryConfigReq->wKindID);


		//�齱����
		LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadLotteryConfig"),true);
		ASSERT(lResultCode==DB_SUCCESS);
		if (lResultCode==DB_SUCCESS)
		{
			//������
			DBO_GP_LotteryConfig LotteryConfig;
			ZeroMemory(&LotteryConfig,sizeof(LotteryConfig));
			
			//��������
			WORD wItemCount=0;
			while (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				//��ȡ����
				LotteryConfig.LotteryItem[wItemCount].cbItemIndex = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemIndex"));
				LotteryConfig.LotteryItem[wItemCount].cbItemType = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemType"));
				LotteryConfig.LotteryItem[wItemCount].lItemQuota = m_PlatformDBAide.GetValue_WORD(TEXT("ItemQuota"));
				wItemCount++;

				//�ƶ���¼
				m_PlatformDBModule->MoveToNext();
			}

			LotteryConfig.wLotteryCount = wItemCount;

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_CONFIG,dwContextID,&LotteryConfig,sizeof(LotteryConfig));
		}
		else
		{
			CTraceService::TraceString(TEXT("�齱���ü���ʧ�ܣ�"),TraceLevel_Exception);
		}

		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@wKindID"),pLotteryConfigReq->wKindID);
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pLotteryConfigReq->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strLogonPass"),pLotteryConfigReq->szLogonPass);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�齱��Ϣ
		lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LotteryUserInfo"),true);
		if (lResultCode==DB_SUCCESS)
		{
			//������
			DBO_GP_LotteryUserInfo LotteryUserInfo;
			ZeroMemory(&LotteryUserInfo,sizeof(LotteryUserInfo));

			LotteryUserInfo.wKindID = pLotteryConfigReq->wKindID;
			LotteryUserInfo.dwUserID = pLotteryConfigReq->dwUserID;
			LotteryUserInfo.cbFreeCount = m_PlatformDBAide.GetValue_BYTE(TEXT("FreeCount"));
			LotteryUserInfo.cbAlreadyCount = m_PlatformDBAide.GetValue_BYTE(TEXT("AlreadyCount"));
			LotteryUserInfo.lChargeFee = m_PlatformDBAide.GetValue_LONGLONG(TEXT("ChargeFee"));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_USER_INFO,dwContextID,&LotteryUserInfo,sizeof(LotteryUserInfo));
		}
		else
		{
			//��������
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateFailure.lResultCode=m_PlatformDBModule->GetReturnValue();
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestSimpleLotteryStart(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GP_SimpleLotteryStart));
		if (wDataSize != sizeof(DBR_GP_SimpleLotteryStart)) return false;

		//������
		DBR_GP_SimpleLotteryStart * pLotteryStart = (DBR_GP_SimpleLotteryStart *)pData;
		//
		int nExp = 0;
		int nInsure = 0;
		int nScore = -20;

		int tmp = pLotteryStart->lLottery;
		switch (tmp)
		{
			case 0:
				{
					nScore+=28;
				}
			break;
			case 1:
				{
					nExp+=888;
				}
				break;
			case 2:
				{
					nExp+=288;
				}
				break;
			case 3:
				{
					nScore+=999;
				}
				break;
			case 4:
				{
					nInsure+=3;
				}
				break;
			case 5:
				{
					nExp+=10;
				}
				break;
			case 6:
				{
					nInsure+=10;
				}
				break;
			case 7:
				{
					nScore+=288;
				}
				break;
			case 8:
				{
					nInsure+=1;
				}
				break;
			case 9:
				{
					nScore+=88;
				}
				break;
			case 10:
				{
					nExp+=88;
				}
				break;
			case 11:
				{
					nExp+=5;
				}
				break;

		}
		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"), pLotteryStart->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@dwAddUserScore"), nScore);
		m_PlatformDBAide.AddParameter(TEXT("@dwAddUserInsure"), nInsure);
		m_PlatformDBAide.AddParameter(TEXT("@dwAddUserExp"), nExp);
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_ADDLotteryScore"), true);
		if (lResultCode==DB_SUCCESS)
		{
			DBO_GP_SimpleLotteryResult  LotteryResult;
			LotteryResult.dwErrorCode = 0;
			LotteryResult.dwIndex = pLotteryStart->dwIndex;
			LotteryResult.dwUserID = pLotteryStart->dwUserID;
			LotteryResult.lUserScore = nScore;
			LotteryResult.lInsure = nInsure;
			LotteryResult.lExp = nExp;
			strcpy(LotteryResult.szName,pLotteryStart->szName);
			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SIMPLE_LOTTERY_RESULT, dwContextID, &LotteryResult, sizeof(LotteryResult));
		} 
		else if(lResultCode == 4)
		{
			DBO_GP_SimpleLotteryResult  LotteryResult;
			LotteryResult.dwErrorCode = lResultCode;
			LotteryResult.dwIndex = pLotteryStart->dwIndex;
			LotteryResult.dwUserID = pLotteryStart->dwUserID;
			LotteryResult.lUserScore = nScore+20;
			LotteryResult.lInsure = nInsure;
			LotteryResult.lExp = nExp;
			strcpy(LotteryResult.szName,pLotteryStart->szName);
			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SIMPLE_LOTTERY_RESULT, dwContextID, &LotteryResult, sizeof(LotteryResult));
		}
		else
		{
			DBO_GP_SimpleLotteryResult  LotteryResult;
			LotteryResult.dwErrorCode = lResultCode;
			LotteryResult.dwIndex = pLotteryStart->dwIndex;
			LotteryResult.dwUserID = pLotteryStart->dwUserID;
			LotteryResult.lUserScore = nScore;
			LotteryResult.lInsure = nInsure;
			LotteryResult.lExp = nExp;
			strcpy(LotteryResult.szName,pLotteryStart->szName);
			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SIMPLE_LOTTERY_RESULT, dwContextID, &LotteryResult, sizeof(LotteryResult));
		}
		
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;
	
}

//�齱��ʼ
bool CDataBaseEngineSink::OnRequestLotteryStart(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_LotteryStart));
		if (wDataSize!=sizeof(DBR_GP_LotteryStart)) return false;

		//������
		DBR_GP_LotteryStart * pLotteryStart=(DBR_GP_LotteryStart *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLotteryStart->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@wKindID"),pLotteryStart->wKindID);
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pLotteryStart->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strLogonPass"),pLotteryStart->szLogonPass);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pLotteryStart->szMachineID);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LotteryStart"),true);

		if (lResultCode==DB_SUCCESS)
		{
			//������
			DBO_GP_LotteryResult LotteryResult;
			ZeroMemory(&LotteryResult,sizeof(LotteryResult));

			LotteryResult.wKindID = pLotteryStart->wKindID;
			LotteryResult.dwUserID = pLotteryStart->dwUserID;
			LotteryResult.bWined = m_PlatformDBAide.GetValue_BYTE(TEXT("Wined"));
			LotteryResult.lUserScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("UserScore"));
			LotteryResult.dUserBeans = m_PlatformDBAide.GetValue_DOUBLE(TEXT("UserBeans"));
			LotteryResult.LotteryItem.cbItemIndex = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemIndex"));
			LotteryResult.LotteryItem.cbItemType = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemType"));
			LotteryResult.LotteryItem.lItemQuota = m_PlatformDBAide.GetValue_LONGLONG(TEXT("ItemQuota"));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_RESULT,dwContextID,&LotteryResult,sizeof(LotteryResult));
		}
		else
		{
			//��������
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateFailure.lResultCode=m_PlatformDBModule->GetReturnValue();
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��Ϸ����
bool CDataBaseEngineSink::OnRequestQueryUserGameData(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryUserGameData));
		if (wDataSize!=sizeof(DBR_GP_QueryUserGameData)) return false;

		//������
		DBR_GP_QueryUserGameData * pQueryUserGameData=(DBR_GP_QueryUserGameData *)pData;

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),pQueryUserGameData->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryUserGameData->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strDynamicPass"),pQueryUserGameData->szDynamicPass);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserGameData"),true);

		if (lResultCode==DB_SUCCESS)
		{
			//������
			DBO_GP_QueryUserGameData QueryUserGameData;
			ZeroMemory(&QueryUserGameData,sizeof(QueryUserGameData));

			QueryUserGameData.wKindID=pQueryUserGameData->wKindID;
			QueryUserGameData.dwUserID=pQueryUserGameData->dwUserID;
			m_TreasureDBAide.GetValue_String(TEXT("UserGameData"),QueryUserGameData.szUserGameData,CountArray(QueryUserGameData.szUserGameData));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_USER_GAME_DATA,dwContextID,&QueryUserGameData,sizeof(QueryUserGameData));
		}
		else
		{
			//��������
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateFailure.lResultCode=m_TreasureDBModule->GetReturnValue();
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}


//�����Ϸ״̬
bool CDataBaseEngineSink::OnRequestUserInGameServerID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryUserInGameServerID));
		if (wDataSize!=sizeof(DBR_GP_QueryUserInGameServerID)) return false;

		//������
		DBR_GP_QueryUserInGameServerID * pModifyIndividual=(DBR_GP_QueryUserInGameServerID *)pData;


		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserInGameServerID"),true);

		if (lResultCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserInGameServerID kGPNetInfo;
			ZeroMemory(&kGPNetInfo,sizeof(kGPNetInfo));

			//�������
			kGPNetInfo.LockKindID=m_AccountsDBAide.GetValue_DWORD(TEXT("LockKindID"));
			kGPNetInfo.LockServerID=m_AccountsDBAide.GetValue_DWORD(TEXT("LockServerID"));
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INGAME_SERVER_ID,dwContextID,&kGPNetInfo,sizeof(kGPNetInfo));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//��ѯϵͳ�ѣ�:DCY by 2017.3.9
bool CDataBaseEngineSink::OnRequestSystemQQ(DWORD dwContextID)
{
	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QuerySystemQQ"),true);

		if (lResultCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_SystemQQ tmpResult;
			memset(&tmpResult,0,sizeof(tmpResult));

			//�������
			m_AccountsDBAide.GetValue_String(TEXT("SystemQQ"),tmpResult.szSystemQQ,CountArray(tmpResult.szSystemQQ)-1);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SYSTEM_QQ,dwContextID,&tmpResult,sizeof(tmpResult));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}
	return true;
}

//��ѯϵͳ����:DCY by 2017.3.9
bool CDataBaseEngineSink::OnRequestSystemInfo(DWORD dwContextID)
{
	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QuerySystemInfo"),true);

		if (lResultCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_SystemInfo tmpResult;
			memset(&tmpResult,0,sizeof(tmpResult));

			//�������
			m_AccountsDBAide.GetValue_String(TEXT("SystemInfo"),tmpResult.szSystemInfo,CountArray(tmpResult.szSystemInfo)-1);
			
			//�������
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SYSTEM_INFO,dwContextID,&tmpResult,sizeof(tmpResult));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}
	return true;
}

//��ѯ��ϵ��ʽ
bool CDataBaseEngineSink::OnRequestSystemContact(DWORD dwContextID)
{
	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QuerySystemContact"),true);

		if (lResultCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_SystemContact tmpResult;
			memset(&tmpResult,0,sizeof(tmpResult));

			//�������
			m_AccountsDBAide.GetValue_String(TEXT("SystemContact"),tmpResult.szSystemContact,CountArray(tmpResult.szSystemContact)-1);

			//�������
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SYSTEM_CONTACT,dwContextID,&tmpResult,sizeof(tmpResult));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}
	return true;
}

bool CDataBaseEngineSink::OnRequestGameShop(DWORD dwContextID)
{
	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryGameShop"),true);

		if (lResultCode==DB_SUCCESS)
		{
			//��������
			tagShopInfo tmpResultList[0xFF];
			memset(tmpResultList,0,sizeof(tmpResultList));

			int nCount = 0;
			//��������
			while(m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				tmpResultList[nCount].dwItemID = m_AccountsDBAide.GetValue_DWORD(TEXT("ItemID"));
				tmpResultList[nCount].llPrice = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Price"));
				tmpResultList[nCount].dwCount = m_AccountsDBAide.GetValue_DWORD(TEXT("nCount"));
				m_AccountsDBAide.GetValue_String(TEXT("strPath"),tmpResultList[nCount].strPath,CountArray(tmpResultList[nCount].strPath)-1);

				tmpResultList[nCount].cbPriceType = m_AccountsDBAide.GetValue_DWORD(TEXT("PriceType"));
				m_AccountsDBAide.GetValue_String(TEXT("name"),tmpResultList[nCount].szName,CountArray(tmpResultList[nCount].szName)-1);
				tmpResultList[nCount].wGiveGold = m_AccountsDBAide.GetValue_DWORD(TEXT("GiveGold"));
				tmpResultList[nCount].wGiveScore = m_AccountsDBAide.GetValue_DWORD(TEXT("GiveScore"));

				//�ƶ���¼
				nCount++;
				m_AccountsDBModule->MoveToNext();
			}

			//�������
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SYSTEM_SHOP,dwContextID,tmpResultList,sizeof(tagShopInfo)*nCount);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}
	return true;
}

// �һ���Ʒ
bool CDataBaseEngineSink::OnRequestExchangeShop(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ExchangeGameShop));
		if (wDataSize!=sizeof(DBR_GP_ExchangeGameShop)) return false;

		//������
		DBR_GP_ExchangeGameShop * pDBRequest=(DBR_GP_ExchangeGameShop *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pDBRequest->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwItemID"),pDBRequest->dwItemID);
		m_AccountsDBAide.AddParameter(TEXT("@dwCount"),pDBRequest->dwCount);
		m_AccountsDBAide.AddParameter(TEXT("@llNumber"),pDBRequest->llNumber);
		m_AccountsDBAide.AddParameter(TEXT("@strName"),pDBRequest->strName);
		m_AccountsDBAide.AddParameter(TEXT("@strAddress"),pDBRequest->strAddress);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ExchangeShop"),true);

		//ִ�н��
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGE_SHOP,dwContextID,&lResultCode,sizeof(LONG));


		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("���ݿ��쳣,���Ժ����ԣ�"),false,false);

		return false;
	}

	return true;
}

//��ȡ����
bool CDataBaseEngineSink::OnRequestGetParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_GetParameter));
		if (wDataSize!=sizeof(DBR_GP_GetParameter)) return false;

		//������
		DBR_GP_GetParameter * pGetParameter=(DBR_GP_GetParameter *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@wServerID"),pGetParameter->wServerID);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidGetParameter"),true);

		//ִ�н��
		OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_GET_PARAMETER,pGetParameter->wServerID);		
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("���ݿ��쳣,���Ժ����ԣ�"),false,false);

		return false;
	}

	return true;
}

//��Ӳ���
bool CDataBaseEngineSink::OnRequestAddParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_AddParameter));
		if (wDataSize!=sizeof(DBR_GP_AddParameter)) return false;

		//������
		DBR_GP_AddParameter * pAddParameter=(DBR_GP_AddParameter *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@wServerID"),pAddParameter->wServerID);
		m_AccountsDBAide.AddParameter(TEXT("@dwServiceMode"),pAddParameter->AndroidParameter.dwServiceMode);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCount"),pAddParameter->AndroidParameter.dwAndroidCount);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterTime"),pAddParameter->AndroidParameter.dwEnterTime);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveTime"),pAddParameter->AndroidParameter.dwLeaveTime);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterMinInterval"),pAddParameter->AndroidParameter.dwEnterMinInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterMaxInterval"),pAddParameter->AndroidParameter.dwEnterMaxInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMinInterval"),pAddParameter->AndroidParameter.dwLeaveMinInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMaxInterval"),pAddParameter->AndroidParameter.dwLeaveMaxInterval);
		m_AccountsDBAide.AddParameter(TEXT("@lTakeMinScore"),pAddParameter->AndroidParameter.lTakeMinScore);
		m_AccountsDBAide.AddParameter(TEXT("@lTakeMaxScore"),pAddParameter->AndroidParameter.lTakeMaxScore);
		m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMinInnings"),pAddParameter->AndroidParameter.dwSwitchMinInnings);
		m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMaxInnings"),pAddParameter->AndroidParameter.dwSwitchMaxInnings);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidAddParameter"),true);

		//ִ�н��
		OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_ADD_PARAMETER,pAddParameter->wServerID);
		
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("���ݿ��쳣,���Ժ����ԣ�"),false,false);

		return false;
	}

	return true;
}

//�޸Ĳ���
bool CDataBaseEngineSink::OnRequestModifyParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyParameter));
		if (wDataSize!=sizeof(DBR_GP_ModifyParameter)) return false;

		//������
		DBR_GP_ModifyParameter * pModifyParameter=(DBR_GP_ModifyParameter *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwDatchID"),pModifyParameter->AndroidParameter.dwBatchID);
		m_AccountsDBAide.AddParameter(TEXT("@dwServiceMode"),pModifyParameter->AndroidParameter.dwServiceMode);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCount"),pModifyParameter->AndroidParameter.dwAndroidCount);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterTime"),pModifyParameter->AndroidParameter.dwEnterTime);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveTime"),pModifyParameter->AndroidParameter.dwLeaveTime);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterMinInterval"),pModifyParameter->AndroidParameter.dwEnterMinInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterMaxInterval"),pModifyParameter->AndroidParameter.dwEnterMaxInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMinInterval"),pModifyParameter->AndroidParameter.dwLeaveMinInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMaxInterval"),pModifyParameter->AndroidParameter.dwLeaveMaxInterval);
		m_AccountsDBAide.AddParameter(TEXT("@lTakeMinScore"),pModifyParameter->AndroidParameter.lTakeMinScore);
		m_AccountsDBAide.AddParameter(TEXT("@lTakeMaxScore"),pModifyParameter->AndroidParameter.lTakeMaxScore);
		m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMinInnings"),pModifyParameter->AndroidParameter.dwSwitchMinInnings);
		m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMaxInnings"),pModifyParameter->AndroidParameter.dwSwitchMaxInnings);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidModifyParameter"),true);

		//ִ�н��
		OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_MODIFY_PARAMETER,pModifyParameter->wServerID);
		
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("���ݿ��쳣,���Ժ����ԣ�"),false,false);

		return false;
	}

	return true;
}

//ɾ������
bool CDataBaseEngineSink::OnRequestDeleteParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_DeleteParameter));
		if (wDataSize!=sizeof(DBR_GP_DeleteParameter)) return false;

		//������
		DBR_GP_DeleteParameter * pDeleteParameter=(DBR_GP_DeleteParameter *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwBatchID"),pDeleteParameter->dwBatchID);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidDeleteParameter"),true);

		//ִ�н��
		OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_DELETE_PARAMETER,pDeleteParameter->wServerID);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("���ݿ��쳣,���Ժ����ԣ�"),false,false);

		return false;
	}

	return true;
}

//��ѯ������Ϣ
bool CDataBaseEngineSink::OnRequestQueryAccountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryAccountInfo));
		if (wDataSize!=sizeof(DBR_GP_QueryAccountInfo)) return false;

		//������
		DBR_GP_QueryAccountInfo * pQueryAccountInfo=(DBR_GP_QueryAccountInfo *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryAccountInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQueryAccountInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserAccountInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserAccountInfo kUserAccountInfo;
			ZeroMemory(&kUserAccountInfo,sizeof(kUserAccountInfo));

			//��������
			kUserAccountInfo.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			kUserAccountInfo.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			kUserAccountInfo.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			kUserAccountInfo.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			kUserAccountInfo.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			kUserAccountInfo.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			kUserAccountInfo.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			kUserAccountInfo.dwSpreaderID=m_AccountsDBAide.GetValue_DWORD(TEXT("SpreaderID"));

			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),kUserAccountInfo.szAccounts,CountArray(kUserAccountInfo.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),kUserAccountInfo.szNickName,CountArray(kUserAccountInfo.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("LogonIP"),kUserAccountInfo.szLogonIp,CountArray(kUserAccountInfo.szLogonIp));

			//�û��ɼ�
			kUserAccountInfo.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			kUserAccountInfo.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			kUserAccountInfo.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			kUserAccountInfo.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),kUserAccountInfo.szUnderWrite,CountArray(kUserAccountInfo.szUnderWrite));

			//��Ա����
			kUserAccountInfo.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),kUserAccountInfo.MemberOverDate);

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_ACCOUNTINFO,dwContextID,&kUserAccountInfo,sizeof(kUserAccountInfo));
		}
		else
		{
			//������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),szDescribeString,false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryIndividual));
		if (wDataSize!=sizeof(DBR_GP_QueryIndividual)) return false;

		//������
		DBR_GP_QueryIndividual * pQueryIndividual=(DBR_GP_QueryIndividual *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQueryIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pQueryIndividual->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserIndividual"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserIndividual UserIndividual;
			ZeroMemory(&UserIndividual,sizeof(UserIndividual));

			//�û���Ϣ
			UserIndividual.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"),UserIndividual.szUserNote,CountArray(UserIndividual.szUserNote));
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"),UserIndividual.szCompellation,CountArray(UserIndividual.szCompellation));

			//�绰����
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),UserIndividual.szSeatPhone,CountArray(UserIndividual.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),UserIndividual.szMobilePhone,CountArray(UserIndividual.szMobilePhone));

			//��ϵ����
			m_AccountsDBAide.GetValue_String(TEXT("QQ"),UserIndividual.szQQ,CountArray(UserIndividual.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"),UserIndividual.szEMail,CountArray(UserIndividual.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),UserIndividual.szDwellingPlace,CountArray(UserIndividual.szDwellingPlace));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INDIVIDUAL,dwContextID,&UserIndividual,sizeof(UserIndividual));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureInfo));
		if (wDataSize!=sizeof(DBR_GP_QueryInsureInfo)) return false;

		//������
		DBR_GP_QueryInsureInfo * pQueryInsureInfo=(DBR_GP_QueryInsureInfo *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryInsureInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pQueryInsureInfo->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserInsureInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//������Ϣ
			UserInsureInfo.cbEnjoinTransfer=m_TreasureDBAide.GetValue_BYTE(TEXT("EnjoinTransfer"));
			UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wRevenueTransferMember=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransferMember"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
			UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferPrerequisite"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ�û�
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureUserInfo));
	if (wDataSize!=sizeof(DBR_GP_QueryInsureUserInfo)) return false;

	//������
	DBR_GP_QueryInsureUserInfo * pQueryTransferUserInfo=(DBR_GP_QueryInsureUserInfo *)pData;

	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pQueryTransferUserInfo->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@strAccounts"),pQueryTransferUserInfo->szAccounts);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTransferUserInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserTransferUserInfo TransferUserInfo;
			ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

			//������Ϣ
			TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
			m_TreasureDBAide.GetValue_String(TEXT("Accounts"), TransferUserInfo.szAccounts, CountArray(TransferUserInfo.szAccounts));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
}

//���ؽ���
bool CDataBaseEngineSink::OnRequestCheckInReward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_PlatformDBAide.ResetParameter();

		//ִ������
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadCheckInReward"),true);

		//����ṹ
		DBO_GP_CheckInReward CheckInReward;
		ZeroMemory(&CheckInReward,sizeof(CheckInReward));

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			WORD wDayIndex=0;
			while (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				wDayIndex = m_PlatformDBAide.GetValue_WORD(TEXT("DayID"));
				if(wDayIndex<=LEN_WEEK)
				{
					CheckInReward.lRewardGold[wDayIndex-1] = m_PlatformDBAide.GetValue_LONGLONG(TEXT("RewardGold")); 
				}

				//�ƶ���¼
				m_PlatformDBModule->MoveToNext();
			}

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_REWARD,dwContextID,&CheckInReward,sizeof(CheckInReward));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//��ѯǩ��
bool CDataBaseEngineSink::OnRequestCheckInQueryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_CheckInQueryInfo));
		if (wDataSize!=sizeof(DBR_GP_CheckInQueryInfo)) return false;

		//������
		DBR_GP_CheckInQueryInfo * pCheckInQueryInfo=(DBR_GP_CheckInQueryInfo *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pCheckInQueryInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pCheckInQueryInfo->szPassword);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_CheckInQueryInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_CheckInInfo CheckInInfo;
			ZeroMemory(&CheckInInfo,sizeof(CheckInInfo));

			//������Ϣ
			CheckInInfo.wSeriesDate=m_AccountsDBAide.GetValue_WORD(TEXT("SeriesDate"));
			CheckInInfo.bTodayChecked = (bool)m_AccountsDBAide.GetValue_BYTE(TEXT("TodayCheckIned"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_INFO,dwContextID,&CheckInInfo,sizeof(CheckInInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//����ṹ
			DBO_GP_CheckInResult CheckInResult;
			CheckInResult.bSuccessed=false;
			lstrcpyn(CheckInResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(CheckInResult.szNotifyContent));

			//���ͽ��
			WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_CheckInResult CheckInResult;
		CheckInResult.bSuccessed=false;
		lstrcpyn(CheckInResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(CheckInResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

		return false;
	}
}

//ִ��ǩ��
bool CDataBaseEngineSink::OnRequestCheckInDone(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_CheckInDone));
		if (wDataSize!=sizeof(DBR_GP_CheckInDone)) return false;

		//������
		DBR_GP_CheckInDone * pCheckInDone=(DBR_GP_CheckInDone *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pCheckInDone->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pCheckInDone->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pCheckInDone->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pCheckInDone->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_CheckInDone"),true);

		//��������
		DBO_GP_CheckInResult CheckInResult;
		ZeroMemory(&CheckInResult,sizeof(CheckInResult));

		//��ȡ����
		if(lResultCode==DB_SUCCESS && m_AccountsDBModule->IsRecordsetEnd()==false)
		{
			CheckInResult.lScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
		}

		//��ȡ����
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		//������Ϣ
		CheckInResult.bSuccessed=lResultCode==DB_SUCCESS;
		lstrcpyn(CheckInResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(CheckInResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_CheckInResult CheckInResult;
		CheckInResult.bSuccessed=false;
		lstrcpyn(CheckInResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(CheckInResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

		return false;
	}
}

//��ȡ����
bool CDataBaseEngineSink::OnRequestTaskTake(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_TaskTake));
		if (wDataSize!=sizeof(DBR_GP_TaskTake)) return false;

		//������
		DBR_GP_TaskTake * pTaskTake=(DBR_GP_TaskTake *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pTaskTake->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pTaskTake->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@wTaskID"),pTaskTake->wTaskID);
		m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pTaskTake->szPassword);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pTaskTake->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_TaskTake"),false);

		//��������
		DBO_GP_TaskResult TaskResult;
		ZeroMemory(&TaskResult,sizeof(TaskResult));

		//��ȡ����
		CDBVarValue DBVarValue;
		m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		//������Ϣ
		TaskResult.wCommandID=SUB_GP_TASK_TAKE;
		TaskResult.bSuccessed=lResultCode==DB_SUCCESS;
		lstrcpyn(TaskResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(TaskResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_TaskResult TaskResult;
		TaskResult.bSuccessed=false;
		TaskResult.wCommandID=SUB_GP_TASK_TAKE;
		lstrcpyn(TaskResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(TaskResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

		return false;
	}
}

//��ȡ����
bool CDataBaseEngineSink::OnRequestTaskReward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_TaskReward));
		if (wDataSize!=sizeof(DBR_GP_TaskReward)) return false;

		//������
		DBR_GP_TaskReward * pTaskReward=(DBR_GP_TaskReward *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pTaskReward->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pTaskReward->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@wTaskID"),pTaskReward->wTaskID);
		m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pTaskReward->szPassword);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pTaskReward->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_TaskReward"),true);

		//��������
		DBO_GP_TaskResult TaskResult;
		ZeroMemory(&TaskResult,sizeof(TaskResult));

		//��ȡ����
		CDBVarValue DBVarValue;
		m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		//������Ϣ
		TaskResult.wCommandID=SUB_GP_TASK_REWARD;
		TaskResult.bSuccessed=lResultCode==DB_SUCCESS;
		lstrcpyn(TaskResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(TaskResult.szNotifyContent));

		//��ȡ����
		if(TaskResult.bSuccessed==true)
		{
			TaskResult.lCurrScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("Score"));
			TaskResult.lCurrIngot= m_PlatformDBAide.GetValue_LONGLONG(TEXT("Ingot"));
		}

		//���ͽ��
		WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_TaskResult TaskResult;
		TaskResult.bSuccessed=false;
		TaskResult.wCommandID=SUB_GP_TASK_REWARD;
		lstrcpyn(TaskResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(TaskResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

		return false;
	}
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestTaskQueryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_TaskQueryInfo));
		if (wDataSize!=sizeof(DBR_GP_TaskQueryInfo)) return false;

		//������
		DBR_GP_TaskQueryInfo * pTaskQueryInfo=(DBR_GP_TaskQueryInfo *)pData;

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@wKindID"),0);
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pTaskQueryInfo->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pTaskQueryInfo->szPassword);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTaskInfo"),true);

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_TaskInfo TaskInfo;
			tagTaskStatus * pTaskStatus=NULL;
			ZeroMemory(&TaskInfo,sizeof(TaskInfo));			

			//��������
			while (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				//���ñ���
				pTaskStatus = &TaskInfo.TaskStatus[TaskInfo.wTaskCount++];

				//��ȡ����
				pTaskStatus->wTaskID = m_PlatformDBAide.GetValue_WORD(TEXT("TaskID"));
				pTaskStatus->cbTaskStatus = m_PlatformDBAide.GetValue_BYTE(TEXT("TaskStatus"));
				pTaskStatus->wTaskProgress = m_PlatformDBAide.GetValue_WORD(TEXT("Progress"));

				//�ƶ���¼
				m_PlatformDBModule->MoveToNext();
			}

			//���ͽ��
			WORD wSendDataSize = sizeof(TaskInfo)-sizeof(TaskInfo.TaskStatus);
			wSendDataSize += sizeof(TaskInfo.TaskStatus[0])*TaskInfo.wTaskCount;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_INFO,dwContextID,&TaskInfo,wSendDataSize);
		}
		else
		{
			//��������
			DBO_GP_TaskResult TaskResult;
			ZeroMemory(&TaskResult,sizeof(TaskResult));

			//��ȡ����
			CDBVarValue DBVarValue;
			m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������Ϣ
			TaskResult.bSuccessed=false;
			TaskResult.wCommandID=SUB_GP_TASK_LOAD;
			lstrcpyn(TaskResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(TaskResult.szNotifyContent));

			//���ͽ��
			WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_TaskResult TaskResult;
		TaskResult.bSuccessed=false;
		TaskResult.wCommandID=SUB_GP_TASK_LOAD;
		lstrcpyn(TaskResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(TaskResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

		return false;
	}
}

//�ͱ�����
bool CDataBaseEngineSink::OnRequestLoadBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();

		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_LoadBaseEnsure"),true);

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//����ṹ
			DBO_GP_BaseEnsureParameter BaseEnsureParameter;
			ZeroMemory(&BaseEnsureParameter,sizeof(BaseEnsureParameter));

			//��������
			if(m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				BaseEnsureParameter.cbTakeTimes= m_AccountsDBAide.GetValue_WORD(TEXT("TakeTimes"));
				BaseEnsureParameter.lScoreAmount = m_AccountsDBAide.GetValue_LONGLONG(TEXT("ScoreAmount"));
				BaseEnsureParameter.lScoreCondition = m_AccountsDBAide.GetValue_LONGLONG(TEXT("ScoreCondition"));				

				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_PARAMETER,dwContextID,&BaseEnsureParameter,sizeof(BaseEnsureParameter));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}
}

//��ȡ�ͱ�
bool CDataBaseEngineSink::OnRequestTakeBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_TakeBaseEnsure));
		if (wDataSize!=sizeof(DBR_GP_TakeBaseEnsure)) return false;

		//��ȡ����
		DBR_GP_TakeBaseEnsure * pTakeBaseEnsure=(DBR_GP_TakeBaseEnsure *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pTakeBaseEnsure->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pTakeBaseEnsure->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pTakeBaseEnsure->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pTakeBaseEnsure->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_TakeBaseEnsure"),true);

		//��������
		DBO_GP_BaseEnsureResult BaseEnsureResult;
		ZeroMemory(&BaseEnsureResult,sizeof(BaseEnsureResult));

		//��ȡ����
		if(lResultCode==DB_SUCCESS && m_AccountsDBModule->IsRecordsetEnd()==false)
		{
			BaseEnsureResult.lGameScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
		}

		//��ȡ����
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);

		//������Ϣ
		BaseEnsureResult.bSuccessed=lResultCode==DB_SUCCESS;
		lstrcpyn(BaseEnsureResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(BaseEnsureResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
		wSendSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_RESULT,dwContextID,&BaseEnsureResult,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_BaseEnsureResult BaseEnsureResult;
		ZeroMemory(&BaseEnsureResult,sizeof(BaseEnsureResult));

		//���ñ���
		BaseEnsureResult.bSuccessed=false;
		lstrcpyn(BaseEnsureResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(BaseEnsureResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
		wSendSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_RESULT,dwContextID,&BaseEnsureResult,wSendSize);

		return false;
	}

	return true;
}

//�ƹ���Ϣ
bool CDataBaseEngineSink::OnRequestQuerySpreadInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��ȡ����
		DBR_GP_QuerySpreadInfo * pLoadSpreadInfo = (DBR_GP_QuerySpreadInfo*)pData;

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pLoadSpreadInfo->dwUserID);

		//ִ������
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_LoadSpreadInfo"),true);

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//����ṹ
			DBO_GP_UserSpreadInfo UserSpreadInfo;
			ZeroMemory(&UserSpreadInfo,sizeof(UserSpreadInfo));

			//��������
			if(m_TreasureDBModule->IsRecordsetEnd()==false)
			{
				UserSpreadInfo.dwSpreadCount = m_TreasureDBAide.GetValue_DWORD(TEXT("SpreadCount"));			
				UserSpreadInfo.lSpreadReward= m_TreasureDBAide.GetValue_LONGLONG(TEXT("SpreadReward"));

				//�ƶ���¼
				m_TreasureDBModule->MoveToNext();
			}

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SPREAD_INFO,dwContextID,&UserSpreadInfo,sizeof(UserSpreadInfo));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}
}

//�ȼ�����
bool CDataBaseEngineSink::OnRequestLoadGrowLevelConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_PlatformDBAide.ResetParameter();

		//ִ������
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadGrowLevelConfig"),true);

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_GrowLevelConfig GrowLevelConfig;
			ZeroMemory(&GrowLevelConfig,sizeof(GrowLevelConfig));

			//���ñ���
			WORD wLevelCount=0;
			tagGrowLevelConfig * pGrowLevelConfig=NULL;

			//��������
			while (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				//����ж�
				if(GrowLevelConfig.wLevelCount>=CountArray(GrowLevelConfig.GrowLevelConfig)) break;

				//���ñ���
				pGrowLevelConfig = &GrowLevelConfig.GrowLevelConfig[GrowLevelConfig.wLevelCount++];

				//��ȡ����
				pGrowLevelConfig->wLevelID= m_PlatformDBAide.GetValue_WORD(TEXT("LevelID"));
				pGrowLevelConfig->dwExperience= m_PlatformDBAide.GetValue_DWORD(TEXT("Experience"));

				//�ƶ���¼
				m_PlatformDBModule->MoveToNext();
			}

			//��������
			WORD wSendDataSize = sizeof(GrowLevelConfig)-sizeof(GrowLevelConfig.GrowLevelConfig);
			wSendDataSize += sizeof(tagGrowLevelConfig)*GrowLevelConfig.wLevelCount;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GROWLEVEL_CONFIG,dwContextID,&GrowLevelConfig,wSendDataSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//��ѯ�ȼ�
bool CDataBaseEngineSink::OnRequestQueryGrowLevelParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_GrowLevelQueryInfo));
		if (wDataSize!=sizeof(DBR_GP_GrowLevelQueryInfo)) return false;

		//������
		DBR_GP_GrowLevelQueryInfo * pGrowLevelQueryInfo=(DBR_GP_GrowLevelQueryInfo *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pGrowLevelQueryInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pGrowLevelQueryInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pGrowLevelQueryInfo->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pGrowLevelQueryInfo->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strUpgradeDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryGrowLevel"),true);

		//����ṹ
		DBO_GP_GrowLevelParameter GrowLevelParameter;
		ZeroMemory(&GrowLevelParameter,sizeof(GrowLevelParameter));

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			if (m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				//��ȡ����
				GrowLevelParameter.wCurrLevelID = m_AccountsDBAide.GetValue_WORD(TEXT("CurrLevelID"));				
				GrowLevelParameter.dwExperience = m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
				GrowLevelParameter.dwUpgradeExperience = m_AccountsDBAide.GetValue_DWORD(TEXT("UpgradeExperience"));
				GrowLevelParameter.lUpgradeRewardGold = m_AccountsDBAide.GetValue_LONGLONG(TEXT("RewardGold"));
				GrowLevelParameter.lUpgradeRewardIngot = m_AccountsDBAide.GetValue_LONGLONG(TEXT("RewardMedal"));			
			}	

			//����ṹ
			DBO_GP_GrowLevelUpgrade GrowLevelUpgrade;
			ZeroMemory(&GrowLevelUpgrade,sizeof(GrowLevelUpgrade));

			//������ʾ
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strUpGradeDescribe"),DBVarValue);
			lstrcpyn(GrowLevelUpgrade.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(GrowLevelUpgrade.szNotifyContent));
			if(GrowLevelUpgrade.szNotifyContent[0]!=0)
			{
				//��ȡ�Ƹ�
				GrowLevelUpgrade.lCurrScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
				GrowLevelUpgrade.lCurrIngot = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Ingot"));	

				//������ʾ
				WORD wSendDataSize = sizeof(GrowLevelUpgrade)-sizeof(GrowLevelUpgrade.szNotifyContent);
				wSendDataSize += CountStringBuffer(GrowLevelUpgrade.szNotifyContent);
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GROWLEVEL_UPGRADE,dwContextID,&GrowLevelUpgrade,wSendDataSize);
			}
		}

		//���Ͳ���
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GROWLEVEL_PARAMETER,dwContextID,&GrowLevelParameter,sizeof(GrowLevelParameter));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_GrowLevelParameter GrowLevelParameter;
		ZeroMemory(&GrowLevelParameter,sizeof(GrowLevelParameter));

		//���Ͳ���
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GROWLEVEL_PARAMETER,dwContextID,&GrowLevelParameter,sizeof(GrowLevelParameter));

		return false;
	}
}

//��Ա����
bool CDataBaseEngineSink::OnRequestLoadMemberParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();

		//ִ������
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_LoadMemberParameter"),true);

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_MemberParameter MemberParameter;
			ZeroMemory(&MemberParameter,sizeof(MemberParameter));

			//���ñ���
			WORD wMemberCount=0;
			tagMemberParameter * pMemberParameter=NULL;

			//��������
			while (m_TreasureDBModule->IsRecordsetEnd()==false)
			{
				//����ж�
				if(MemberParameter.wMemberCount>=CountArray(MemberParameter.MemberParameter)) break;

				//���ñ���
				pMemberParameter = &MemberParameter.MemberParameter[MemberParameter.wMemberCount++];

				//��ȡ����
				pMemberParameter->cbMemberOrder= m_TreasureDBAide.GetValue_BYTE(TEXT("MemberOrder"));
				pMemberParameter->lMemberPrice= m_TreasureDBAide.GetValue_LONGLONG(TEXT("MemberPrice"));
				pMemberParameter->lPresentScore= m_TreasureDBAide.GetValue_LONGLONG(TEXT("PresentScore"));
				m_TreasureDBAide.GetValue_String(TEXT("MemberName"),pMemberParameter->szMemberName,CountArray(pMemberParameter->szMemberName));

				//�ƶ���¼
				m_TreasureDBModule->MoveToNext();
			}

			//��������
			WORD wSendDataSize = sizeof(MemberParameter)-sizeof(MemberParameter.MemberParameter);
			wSendDataSize += sizeof(tagMemberParameter)*MemberParameter.wMemberCount;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MEMBER_PARAMETER,dwContextID,&MemberParameter,wSendDataSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//�����Ա
bool CDataBaseEngineSink::OnRequestPurchaseMember(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_PurchaseMember));
		if (wDataSize!=sizeof(DBR_GP_PurchaseMember)) return false;

		//������
		DBR_GP_PurchaseMember * pPurchaseMember=(DBR_GP_PurchaseMember *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pPurchaseMember->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pPurchaseMember->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@cbMemberOrder"),pPurchaseMember->cbMemberOrder);
		m_TreasureDBAide.AddParameter(TEXT("@PurchaseTime"),pPurchaseMember->wPurchaseTime);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pPurchaseMember->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_PurchaseMember"),true);

		//����ṹ
		DBO_GP_PurchaseResult PurchaseResult;
		ZeroMemory(&PurchaseResult,sizeof(PurchaseResult));

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//���ñ���
			PurchaseResult.bSuccessed=true;

			//��������
			if (m_TreasureDBModule->IsRecordsetEnd()==false)
			{
				//��ȡ����
				PurchaseResult.lCurrScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrScore"));
				PurchaseResult.dCurrBeans = m_TreasureDBAide.GetValue_DOUBLE(TEXT("CurrBeans"));
				PurchaseResult.cbMemberOrder = m_TreasureDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			}	
		}

		//��ȡ��ʾ
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);
		lstrcpyn(PurchaseResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(PurchaseResult.szNotifyContent));

		//�����С
		WORD wSendDataSize = sizeof(PurchaseResult)-sizeof(PurchaseResult.szNotifyContent);
		wSendDataSize += CountStringBuffer(PurchaseResult.szNotifyContent);

		//���Ͳ���
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_PURCHASE_RESULT,dwContextID,&PurchaseResult,wSendDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_PurchaseResult PurchaseResult;
		ZeroMemory(&PurchaseResult,sizeof(PurchaseResult));

		//���ñ���
		lstrcpyn(PurchaseResult.szNotifyContent,TEXT("���ݿ��쳣�����Ժ����ԣ�"),CountArray(PurchaseResult.szNotifyContent));

		//�����С
		WORD wSendDataSize = sizeof(PurchaseResult)-sizeof(PurchaseResult.szNotifyContent);
		wSendDataSize += CountStringBuffer(PurchaseResult.szNotifyContent);

		//���Ͳ���
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_PURCHASE_RESULT,dwContextID,&PurchaseResult,wSendDataSize);

		return false;
	}
}

//�һ���Ϸ��
bool CDataBaseEngineSink::OnRequestExchangeScoreByIngot(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ExchangeScoreByIngot));
		if (wDataSize!=sizeof(DBR_GP_ExchangeScoreByIngot)) return false;

		//������
		DBR_GP_ExchangeScoreByIngot * pExchangeScore=(DBR_GP_ExchangeScoreByIngot *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pExchangeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pExchangeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@ExchangeIngot"),pExchangeScore->lExchangeIngot);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pExchangeScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_ExchangeScoreByIngot"),true);

		//����ṹ
		DBO_GP_ExchangeResult ExchangeResult;
		ZeroMemory(&ExchangeResult,sizeof(ExchangeResult));

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//���ñ���
			ExchangeResult.bSuccessed=true;

			//��������
			if (m_TreasureDBModule->IsRecordsetEnd()==false)
			{
				//��ȡ����
				ExchangeResult.lCurrScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrScore"));
				ExchangeResult.lCurrIngot = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrIngot"));
				ExchangeResult.dCurrBeans = m_TreasureDBAide.GetValue_DOUBLE(TEXT("CurrBeans"));
			}	
		}

		//��ʾ����
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);
		lstrcpyn(ExchangeResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(ExchangeResult.szNotifyContent));

		//�����С
		WORD wSendDataSize = sizeof(ExchangeResult)-sizeof(ExchangeResult.szNotifyContent);
		wSendDataSize += CountStringBuffer(ExchangeResult.szNotifyContent);

		//���Ͳ���
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGE_RESULT,dwContextID,&ExchangeResult,wSendDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_ExchangeResult ExchangeResult;
		ZeroMemory(&ExchangeResult,sizeof(ExchangeResult));

		//���ñ���
		lstrcpyn(ExchangeResult.szNotifyContent,TEXT("���ݿ��쳣�����Ժ����ԣ�"),CountArray(ExchangeResult.szNotifyContent));

		//�����С
		WORD wSendDataSize = sizeof(ExchangeResult)-sizeof(ExchangeResult.szNotifyContent);
		wSendDataSize += CountStringBuffer(ExchangeResult.szNotifyContent);

		//���Ͳ���
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGE_RESULT,dwContextID,&ExchangeResult,wSendDataSize);

		return false;
	}
}


//�һ���Ϸ��
bool CDataBaseEngineSink::OnRequestExchangeScoreByBeans(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ExchangeScoreByBeans));
		if (wDataSize!=sizeof(DBR_GP_ExchangeScoreByBeans)) return false;

		//������
		DBR_GP_ExchangeScoreByBeans * pExchangeScore=(DBR_GP_ExchangeScoreByBeans *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pExchangeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pExchangeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@ExchangeBeans"),pExchangeScore->dExchangeBeans);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pExchangeScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_ExchangeScoreByBeans"),true);

		//����ṹ
		DBO_GP_ExchangeResult ExchangeResult;
		ZeroMemory(&ExchangeResult,sizeof(ExchangeResult));

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//���ñ���
			ExchangeResult.bSuccessed=true;

			//��������
			if (m_TreasureDBModule->IsRecordsetEnd()==false)
			{
				//��ȡ����
				ExchangeResult.lCurrScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrScore"));
				ExchangeResult.lCurrIngot = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrIngot"));
				ExchangeResult.dCurrBeans = m_TreasureDBAide.GetValue_DOUBLE(TEXT("CurrBeans"));
			}	
		}

		//��ʾ����
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);
		lstrcpyn(ExchangeResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(ExchangeResult.szNotifyContent));

		//�����С
		WORD wSendDataSize = sizeof(ExchangeResult)-sizeof(ExchangeResult.szNotifyContent);
		wSendDataSize += CountStringBuffer(ExchangeResult.szNotifyContent);

		//���Ͳ���
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGE_RESULT,dwContextID,&ExchangeResult,wSendDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_ExchangeResult ExchangeResult;
		ZeroMemory(&ExchangeResult,sizeof(ExchangeResult));

		//���ñ���
		lstrcpyn(ExchangeResult.szNotifyContent,TEXT("���ݿ��쳣�����Ժ����ԣ�"),CountArray(ExchangeResult.szNotifyContent));

		//�����С
		WORD wSendDataSize = sizeof(ExchangeResult)-sizeof(ExchangeResult.szNotifyContent);
		wSendDataSize += CountStringBuffer(ExchangeResult.szNotifyContent);

		//���Ͳ���
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGE_RESULT,dwContextID,&ExchangeResult,wSendDataSize);

		return false;
	}
}

//�����б�
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//��������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameType * pGameType=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameType))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
			pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameType->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameType);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameKind * pGameKind=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameKind))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
			pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			pGameKind->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
			pGameKind->cbSuportMobile=m_PlatformDBAide.GetValue_BYTE(TEXT("SuportMobile"));
			m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
			m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameKind);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ�ڵ�
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameNodeItem"),true);

		//���ͽڵ�
		wPacketSize=0;
		DBO_GP_GameNode * pGameNode=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameNode))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameNode=(DBO_GP_GameNode *)(cbBuffer+wPacketSize);
			pGameNode->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameNode->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameNode->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameNode->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			m_PlatformDBAide.GetValue_String(TEXT("NodeName"),pGameNode->szNodeName,CountArray(pGameNode->szNodeName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameNode);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGamePageItem"),true);

		//���Ͷ���
		wPacketSize=0;
		DBO_GP_GamePage * pGamePage=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GamePage))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGamePage=(DBO_GP_GamePage *)(cbBuffer+wPacketSize);
			pGamePage->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGamePage->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			pGamePage->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGamePage->wPageID=m_PlatformDBAide.GetValue_WORD(TEXT("PageID"));
			pGamePage->wOperateType=m_PlatformDBAide.GetValue_WORD(TEXT("OperateType"));
			m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),pGamePage->szDisplayName,CountArray(pGamePage->szDisplayName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GamePage);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=TRUE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=FALSE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return false;
	}

	return true;
}

//������Ϣ
bool CDataBaseEngineSink::OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		DBR_GP_OnLineCountInfo * pOnLineCountInfo=(DBR_GP_OnLineCountInfo *)pData;
		WORD wHeadSize=(sizeof(DBR_GP_OnLineCountInfo)-sizeof(pOnLineCountInfo->OnLineCountKind));

		//Ч������
		ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKindEx))));
		if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKindEx)))) return false;

		//������ʶ
		TCHAR szMachineID[LEN_MACHINE_ID];
		CWHService::GetMachineID(szMachineID);

		//������Ϣ
		TCHAR szOnLineCountKind[2048]=TEXT("");
		for (WORD i=0;i<pOnLineCountInfo->wKindCount;i++)
		{
			INT nLength=lstrlen(szOnLineCountKind);
			_sntprintf(&szOnLineCountKind[nLength],CountArray(szOnLineCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[i].wKindID,pOnLineCountInfo->OnLineCountKind[i].dwOnLineCount);
		}

		//������Ϣ
		TCHAR szAndroidCountKind[2048]=TEXT("");
		for (WORD j=0;j<pOnLineCountInfo->wKindCount;j++)
		{
			INT nLength=lstrlen(szAndroidCountKind);
			_sntprintf(&szAndroidCountKind[nLength],CountArray(szAndroidCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[j].wKindID,pOnLineCountInfo->OnLineCountKind[j].dwAndroidCount);
		}

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineServer"),m_pInitParameter->m_szServerName);
		m_PlatformDBAide.AddParameter(TEXT("@dwOnLineCountSum"),pOnLineCountInfo->dwOnLineCountSum);
		m_PlatformDBAide.AddParameter(TEXT("@dwAndroidCountSum"),pOnLineCountInfo->dwAndroidCountSum);
		m_PlatformDBAide.AddParameter(TEXT("@strOnLineCountKind"),szOnLineCountKind);
		m_PlatformDBAide.AddParameter(TEXT("@strAndroidCountKind"),szAndroidCountKind);

		//ִ������
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_OnLineCountInfo"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//ƽ̨����
bool CDataBaseEngineSink::OnRequestPlatformParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_PlatformDBAide.ResetParameter();

		//ִ������
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadPlatformParameter"),true);

		//����ṹ
		DBO_GP_PlatformParameter PlatformParameter;
		ZeroMemory(&PlatformParameter,sizeof(PlatformParameter));

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��¼�ж�
			if (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				//��ȡ����
				PlatformParameter.dwExchangeRate = m_PlatformDBAide.GetValue_DWORD(TEXT("ExchangeRate"));
				PlatformParameter.dwPresentExchangeRate = m_PlatformDBAide.GetValue_DWORD(TEXT("PresentExchangeRate"));
				PlatformParameter.dwRateGold = m_PlatformDBAide.GetValue_DWORD(TEXT("RateGold"));
			}						
		}

		//��������
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_PLATFORM_PARAMETER,dwContextID,&PlatformParameter,sizeof(PlatformParameter));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//��������
bool CDataBaseEngineSink::OnRequestLoadTaskList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_PlatformDBAide.ResetParameter();

		//ִ������
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadTaskList"),true);

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			tagTaskParameter TaskParameter;
			BYTE cbDataBuffer[SOCKET_TCP_PACKET-1024]={0};						
			DBO_GP_TaskListInfo * pTaskListInfo = (DBO_GP_TaskListInfo *)cbDataBuffer;
			LPBYTE pDataBuffer=cbDataBuffer+sizeof(DBO_GP_TaskListInfo);

			//���ñ���
			WORD wTaskCount=0;
			WORD wSendDataSize=sizeof(DBO_GP_TaskListInfo);		

			//��������
			while (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				//��ȡ����
				TaskParameter.wTaskID = m_PlatformDBAide.GetValue_WORD(TEXT("TaskID"));
				TaskParameter.wTaskType = m_PlatformDBAide.GetValue_WORD(TEXT("TaskType"));
				TaskParameter.cbPlayerType = m_PlatformDBAide.GetValue_BYTE(TEXT("UserType"));
				TaskParameter.wKindID = m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
				TaskParameter.wTaskObject= m_PlatformDBAide.GetValue_WORD(TEXT("Innings"));
				TaskParameter.dwTimeLimit = m_PlatformDBAide.GetValue_DWORD(TEXT("TimeLimit"));
				TaskParameter.lStandardAwardGold = m_PlatformDBAide.GetValue_LONGLONG(TEXT("StandardAwardGold"));
				TaskParameter.lStandardAwardMedal = m_PlatformDBAide.GetValue_LONGLONG(TEXT("StandardAwardMedal"));
				TaskParameter.lMemberAwardGold = m_PlatformDBAide.GetValue_LONGLONG(TEXT("MemberAwardGold"));
				TaskParameter.lMemberAwardMedal = m_PlatformDBAide.GetValue_LONGLONG(TEXT("MemberAwardMedal"));

				//������Ϣ
				m_PlatformDBAide.GetValue_String(TEXT("TaskName"),TaskParameter.szTaskName,CountArray(TaskParameter.szTaskName));
				m_PlatformDBAide.GetValue_String(TEXT("TaskDescription"),TaskParameter.szTaskDescribe,CountArray(TaskParameter.szTaskDescribe));

				//�����ж�				
				if(wSendDataSize+sizeof(tagTaskParameter)>sizeof(cbDataBuffer))
				{
					//���ñ���
					pTaskListInfo->wTaskCount = wTaskCount;

					//��������
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_LIST,dwContextID,pTaskListInfo,wSendDataSize);

					//���ñ���
					wTaskCount=0;
					wSendDataSize=sizeof(DBO_GP_TaskListInfo);	
					pDataBuffer = cbDataBuffer+sizeof(DBO_GP_TaskListInfo);
				}

				//��������
				CopyMemory(pDataBuffer,&TaskParameter,sizeof(tagTaskParameter));

				//���ñ���
				wTaskCount++;
				wSendDataSize += sizeof(tagTaskParameter);
				pDataBuffer += sizeof(tagTaskParameter);

				//�ƶ���¼
				m_PlatformDBModule->MoveToNext();
			}

			//ʣ�෢��
			if(wTaskCount>0 && wSendDataSize>0)
			{
				//���ñ���
				pTaskListInfo->wTaskCount = wTaskCount;

				//��������
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_LIST,dwContextID,pTaskListInfo,wSendDataSize);
			}

			//����֪ͨ
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_LIST_END,dwContextID,NULL,0);			
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//��¼�ɹ�
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString,DWORD dwCheckUserRight, bool bMobileClient)
{
	if (bMobileClient==false)
	{
		if (dwErrorCode==DB_SUCCESS)
		{ 
			//��������
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("DynamicPass"),LogonSuccess.szDynamicPass,CountArray(LogonSuccess.szDynamicPass));

			//�û��ɼ�
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserIngot=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Ingot"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));
			LogonSuccess.dUserBeans= m_AccountsDBAide.GetValue_DOUBLE(TEXT("Beans"));

			//�û�����
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			//m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//��Ա����
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			//��չ��Ϣ
			LogonSuccess.cbLogonMode=m_AccountsDBAide.GetValue_BYTE(TEXT("LogonMode"));
			LogonSuccess.cbInsureEnabled=m_AccountsDBAide.GetValue_BYTE(TEXT("InsureEnabled"));
			LogonSuccess.dwCheckUserRight=dwCheckUserRight;

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//���ر����б�
			try
			{
				//�������
				m_PlatformDBAide.ResetParameter();
				m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),LogonSuccess.dwUserID);
	
				//�������
				TCHAR szDescribeString[128]=TEXT("");
				m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

				//ִ�в�ѯ
				LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_LoadMatchSignupList"),true);
				if(lResultCode==DB_SUCCESS)
				{
					//��������
					tagSignupMatchInfo * pSignupMatchInfo=NULL;

					while (m_PlatformDBModule->IsRecordsetEnd()==false)
					{
						//����ж�
						if(LogonSuccess.wSignupCount>=CountArray(LogonSuccess.SignupMatchInfo)) break;

						//��ȡ����
						pSignupMatchInfo=&LogonSuccess.SignupMatchInfo[LogonSuccess.wSignupCount++];

						//��ȡ��¼
						pSignupMatchInfo->wServerID=m_PlatformDBAide.GetValue_WORD(TEXT("ServerID"));
						pSignupMatchInfo->dwMatchID=m_PlatformDBAide.GetValue_DWORD(TEXT("MatchID"));
						pSignupMatchInfo->dwMatchNO=m_PlatformDBAide.GetValue_DWORD(TEXT("MatchNo"));

						//�ƶ���¼
						m_PlatformDBModule->MoveToNext();
					}
				}
			}
			catch(...)
			{
				ASSERT(FALSE);
			};

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else if(dwErrorCode == DB_NEEDMB)
		{
			//��¼�ɹ�
			DBO_GP_ValidateMBCard ValidateMBCard;
			ZeroMemory(&ValidateMBCard,sizeof(ValidateMBCard));

			//��ȡ����
			ValidateMBCard.uMBCardID=m_AccountsDBAide.GetValue_UINT(TEXT("PasswordID"));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VALIDATE_MBCARD,dwContextID,&ValidateMBCard,sizeof(ValidateMBCard));
		}
		else if(dwErrorCode == DB_PASSPORT)
		{
			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VALIDATE_PASSPORT,dwContextID,NULL,0);			
		}
		else
		{
			//��������
			DBO_GP_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_MB_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("DynamicPass"),LogonSuccess.szDynamicPass,CountArray(LogonSuccess.szDynamicPass));

			//�û��ɼ�
 			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserIngot=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Ingot"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));
			LogonSuccess.dUserBeans= m_AccountsDBAide.GetValue_DOUBLE(TEXT("Beans"));

			//��չ��Ϣ
			LogonSuccess.cbInsureEnabled=m_AccountsDBAide.GetValue_BYTE(TEXT("InsureEnabled"));
			LogonSuccess.cbLogonMode=m_AccountsDBAide.GetValue_BYTE(TEXT("LogonMode"));

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//���ر����б�
			try
			{
				//�������
				m_PlatformDBAide.ResetParameter();
				m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),LogonSuccess.dwUserID);

				//�������
				TCHAR szDescribeString[128]=TEXT("");
				m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

				//ִ�в�ѯ
				LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_LoadMatchSignupList"),true);
				if(lResultCode==DB_SUCCESS)
				{
					//��������
					tagSignupMatchInfo * pSignupMatchInfo=NULL;

					while (m_PlatformDBModule->IsRecordsetEnd()==false)
					{
						//����ж�
						if(LogonSuccess.wSignupCount>=CountArray(LogonSuccess.SignupMatchInfo)) break;

						//��ȡ����
						pSignupMatchInfo=&LogonSuccess.SignupMatchInfo[LogonSuccess.wSignupCount++];

						//��ȡ��¼
						pSignupMatchInfo->wServerID=m_PlatformDBAide.GetValue_WORD(TEXT("ServerID"));
						pSignupMatchInfo->dwMatchID=m_PlatformDBAide.GetValue_DWORD(TEXT("MatchID"));
						pSignupMatchInfo->dwMatchNO=m_PlatformDBAide.GetValue_DWORD(TEXT("MatchNo"));

						//�ƶ���¼
						m_PlatformDBModule->MoveToNext();
					}
				}
			}
			catch(...)
			{
				ASSERT(FALSE);
			};

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else
		{
			//��������
			DBO_MB_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//���н��
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//��������
		DBO_GP_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//�������
		UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
		UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
		UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceInsure"));
		UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_LONGLONG(TEXT("InsureRevenue"));
		UserInsureSuccess.lVariationScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationScore"));
		UserInsureSuccess.lVariationInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationInsure"));
		lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//��������
		DBO_GP_UserInsureFailure UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//�������
		UserInsureFailure.lResultCode=dwErrorCode;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

//�������
VOID CDataBaseEngineSink::OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bCloseSocket)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//��������
		DBO_GP_OperateSuccess OperateSuccess;
		ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

		//�������
		OperateSuccess.bCloseSocket=bCloseSocket;
		OperateSuccess.lResultCode=dwErrorCode;
		lstrcpyn(OperateSuccess.szDescribeString,pszErrorString,CountArray(OperateSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(OperateSuccess.szDescribeString);
		WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//��������
		DBO_GP_OperateFailure OperateFailure;
		ZeroMemory(&OperateFailure,sizeof(OperateFailure));

		//�������
		OperateFailure.bCloseSocket=bCloseSocket;
		OperateFailure.lResultCode=dwErrorCode;
		lstrcpyn(OperateFailure.szDescribeString,pszErrorString,CountArray(OperateFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(OperateFailure.szDescribeString);
		WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wHeadSize+wDataSize);
	}

	return;
}

//�������
VOID CDataBaseEngineSink::OnAndroidDisposeResult(DWORD dwContextID,DWORD dwErrorCode,WORD wSubCommdID,WORD wServerID)
{
	//����ṹ
	DBO_GP_AndroidParameter AndroidParameter;
	ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));

	//���ñ���
	AndroidParameter.wSubCommdID=wSubCommdID;
	AndroidParameter.wServerID=wServerID;

	if(dwErrorCode==DB_SUCCESS)
	{
		//��������
		WORD wParameterCount=0;

		while(m_AccountsDBModule->IsRecordsetEnd()==false)		
		{
			wParameterCount = AndroidParameter.wParameterCount;
			AndroidParameter.AndroidParameter[wParameterCount].dwBatchID = m_AccountsDBAide.GetValue_DWORD(TEXT("BatchID"));
			AndroidParameter.AndroidParameter[wParameterCount].dwAndroidCount = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCount"));
			AndroidParameter.AndroidParameter[wParameterCount].dwServiceMode = m_AccountsDBAide.GetValue_DWORD(TEXT("ServiceMode"));
			AndroidParameter.AndroidParameter[wParameterCount].dwEnterTime = m_AccountsDBAide.GetValue_DWORD(TEXT("EnterTime"));
			AndroidParameter.AndroidParameter[wParameterCount].dwLeaveTime = m_AccountsDBAide.GetValue_DWORD(TEXT("LeaveTime"));
			AndroidParameter.AndroidParameter[wParameterCount].dwEnterMinInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("EnterMinInterval"));
			AndroidParameter.AndroidParameter[wParameterCount].dwEnterMaxInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("EnterMaxInterval"));
			AndroidParameter.AndroidParameter[wParameterCount].dwLeaveMinInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("LeaveMinInterval"));
			AndroidParameter.AndroidParameter[wParameterCount].dwLeaveMaxInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("LeaveMaxInterval"));
			AndroidParameter.AndroidParameter[wParameterCount].dwSwitchMinInnings = m_AccountsDBAide.GetValue_DWORD(TEXT("SwitchMinInnings"));
			AndroidParameter.AndroidParameter[wParameterCount].dwSwitchMaxInnings = m_AccountsDBAide.GetValue_DWORD(TEXT("SwitchMaxInnings"));
			AndroidParameter.AndroidParameter[wParameterCount].lTakeMinScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("TakeMinScore"));
			AndroidParameter.AndroidParameter[wParameterCount].lTakeMaxScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("TakeMaxScore"));

			//���ñ���
			AndroidParameter.wParameterCount++;

			//����ж�
			if(AndroidParameter.wParameterCount>=CountArray(AndroidParameter.AndroidParameter))
			{
				//��������
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ANDROID_PARAMETER,dwContextID,&AndroidParameter,sizeof(AndroidParameter));

				//���ñ���
				ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));
			}

			//�ƶ��α�
			m_AccountsDBModule->MoveToNext();
		}	
	}
	
	//�����С
	WORD wSendDataSize = sizeof(AndroidParameter)-sizeof(AndroidParameter.AndroidParameter);
	wSendDataSize += AndroidParameter.wParameterCount*sizeof(tagAndroidParameter);

	//��������
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ANDROID_PARAMETER,dwContextID,&AndroidParameter,wSendDataSize);
}

//////////////////////////////////////////////////////////////////////////////////
