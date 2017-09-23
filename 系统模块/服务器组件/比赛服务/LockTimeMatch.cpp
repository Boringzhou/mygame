#include "StdAfx.h"
#include "LockTimeMatch.h"
#include "..\��Ϸ������\DataBasePacket.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define INVALID_VALUE				0xFFFF								//��Чֵ

//////////////////////////////////////////////////////////////////////////

//ʱ�Ӷ���
#define IDI_DISTRIBUTE_USER		    (IDI_MATCH_MODULE_START+100)			//�����û�

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CLockTimeMatch::CLockTimeMatch()
{
	//״̬����
	m_MatchStatus=MatchStatus_Free;

	//��������
	m_pMatchOption=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;

	//�ں˽ӿ�
	m_ppITableFrame=NULL;
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngineEvent=NULL;

	//����ӿ�
	m_pIGameServiceFrame=NULL;
	m_pIServerUserManager=NULL;
	m_pAndroidUserManager=NULL;
}

CLockTimeMatch::~CLockTimeMatch(void)
{
	//�ͷ���Դ
	SafeDeleteArray(m_ppITableFrame);

	//�رն�ʱ��
	m_pITimerEngine->KillTimer(IDI_CHECK_END_MATCH);
	m_pITimerEngine->KillTimer(IDI_DISTRIBUTE_USER);

	//�Ƴ�Ԫ��
	m_DistributeManage.RemoveAll();
}

//�ӿڲ�ѯ
VOID* CLockTimeMatch::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{	
	QUERYINTERFACE(IGameMatchItem,Guid,dwQueryVer);
	QUERYINTERFACE(IMatchEventSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink,Guid,dwQueryVer);	
	QUERYINTERFACE_IUNKNOWNEX(IGameMatchItem,Guid,dwQueryVer);
	return NULL;
}

//������
bool CLockTimeMatch::BindTableFrame(ITableFrame * pTableFrame,WORD wTableID)
{
	if(pTableFrame==NULL || wTableID>m_pGameServiceOption->wTableCount)
	{
		ASSERT(false);
		return false;
	}

	//��������
	CTableFrameHook * pTableFrameHook=new CTableFrameHook();
	pTableFrameHook->InitTableFrameHook(QUERY_OBJECT_PTR_INTERFACE(pTableFrame,IUnknownEx));
	pTableFrameHook->SetMatchEventSink(QUERY_OBJECT_PTR_INTERFACE(this,IUnknownEx));

	//���ýӿ�
	pTableFrame->SetTableFrameHook(QUERY_OBJECT_PTR_INTERFACE(pTableFrameHook,IUnknownEx));
	m_ppITableFrame[wTableID]=pTableFrame;

	return true;
}

//��ʼ���ӿ�
bool CLockTimeMatch::InitMatchInterface(tagMatchManagerParameter & MatchManagerParameter)
{
	//��������
	m_pMatchOption=MatchManagerParameter.pGameMatchOption;
	m_pGameServiceOption=MatchManagerParameter.pGameServiceOption;
	m_pGameServiceAttrib=MatchManagerParameter.pGameServiceAttrib;

	//�ں����
	m_pITimerEngine=MatchManagerParameter.pITimerEngine;
	m_pIDataBaseEngine=MatchManagerParameter.pICorrespondManager;
	m_pITCPNetworkEngineEvent=MatchManagerParameter.pTCPNetworkEngine;

	//�������		
	m_pIGameServiceFrame=MatchManagerParameter.pIMainServiceFrame;		
	m_pIServerUserManager=MatchManagerParameter.pIServerUserManager;
	m_pAndroidUserManager=MatchManagerParameter.pIAndroidUserManager;
	m_pIServerUserItemSink=MatchManagerParameter.pIServerUserItemSink;

	//��������
	m_DistributeManage.SetDistributeRule(m_pGameServiceOption->cbDistributeRule);

	//��������
	if (m_ppITableFrame==NULL)
	{
		m_ppITableFrame=new ITableFrame*[m_pGameServiceOption->wTableCount];
	}

	//��ʼ���
	DWORD dwStartInterval = GetMatchStartInterval();
	if(dwStartInterval>0)
	{
		//��ʼ���	
		m_pITimerEngine->SetTimer(IDI_CHECK_START_MATCH,dwStartInterval*1000L,TIMES_INFINITY,NULL);
		
		CString strMessage;
		strMessage.Format(TEXT("1 ��������ʱ��ʼ:%d�룡"),dwStartInterval);
		CTraceService::TraceString(strMessage,TraceLevel_Info);
	}

	return true;
}

//ʱ���¼�
bool CLockTimeMatch::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{	
	switch(dwTimerID)
	{
	case IDI_DISTRIBUTE_USER:				//�����û�
		{
			//ִ�з���
			PerformDistribute();

			return true;
		}
	case IDI_CHECK_START_MATCH:				//��ʼ����
		{
			//����״̬
			m_MatchStatus=MatchStatus_Playing;

			//����״̬
			BYTE cbMatchStatus=MS_MATCHING;
			m_pIGameServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_MATCH,SUB_GR_MATCH_STATUS,&cbMatchStatus,sizeof(cbMatchStatus));

			//�رն�ʱ��
			m_pITimerEngine->KillTimer(IDI_CHECK_START_MATCH);

			//����������
			m_pITimerEngine->SetTimer(IDI_CHECK_END_MATCH,5000,TIMES_INFINITY,0);

			//ִ�з���
			m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_USER,m_pGameServiceOption->wDistributeTimeSpace*1000,TIMES_INFINITY,NULL);

			//����ṹ
			CMD_GR_Match_Num MatchNum;
			ZeroMemory(&MatchNum,sizeof(MatchNum));

			//���ñ���		
			MatchNum.dwWaitting=m_pIServerUserManager->GetUserItemCount();

			//������Ϣ
			m_pIGameServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_MATCH,SUB_GR_MATCH_NUM,&MatchNum, sizeof(MatchNum));

			return true;
		}
	case IDI_CHECK_END_MATCH:				//��������
		{
			//��ȡʱ��
			CTime CurTime=CTime::GetCurrentTime();
			SYSTEMTIME MatchEndTime=m_pMatchOption->MatchType.LockTime.MatchEndTime;

			//�������
			if (CurTime.GetHour()*3600+CurTime.GetMinute()*60+CurTime.GetSecond() >= 
				MatchEndTime.wHour*3600+MatchEndTime.wMinute*60+MatchEndTime.wSecond)
			{
				//�رն�ʱ��
				m_pITimerEngine->KillTimer(IDI_CHECK_END_MATCH);
				m_pITimerEngine->KillTimer(IDI_DISTRIBUTE_USER);

				//����״̬
				m_MatchStatus=MatchStatus_End;	

				//����״̬
				BYTE cbMatchStatus=MS_MATCHEND;
				m_pIGameServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_MATCH,SUB_GR_MATCH_STATUS,&cbMatchStatus,sizeof(cbMatchStatus));

				//��ɢ��Ϸ
				for(INT i=0; i<m_pGameServiceOption->wTableCount; i++)
				{
					//��ȡ����
					ITableFrame * pITableFrame=m_ppITableFrame[i];					
					ASSERT(pITableFrame!=NULL);
					if (pITableFrame==NULL) continue;
					if (pITableFrame->IsGameStarted()==false) continue;

					//��ɢ��Ϸ
					pITableFrame->DismissGame();
				}

				//

				//��������
				if(CurTime >= CTime(MatchEndTime))
				{
					//��������
					DBR_GR_MatchOver MatchOver;
					ZeroMemory(&MatchOver,sizeof(MatchOver));

					//��������				
					MatchOver.cbMatchType=MATCH_TYPE_LOCKTIME;
					MatchOver.dwMatchID=m_pMatchOption->dwMatchID;
					MatchOver.dwMatchNO=m_pMatchOption->dwMatchNO;
					MatchOver.wServerID=m_pGameServiceOption->wServerID;
					MatchOver.MatchEndTime=m_pMatchOption->MatchType.LockTime.MatchEndTime;
					MatchOver.MatchStartTime=m_pMatchOption->MatchType.LockTime.MatchStartTime;

					//Ͷ������
					m_pIDataBaseEngine->PostDataBaseRequest(MatchOver.dwMatchNO,DBR_GR_MATCH_OVER,0,&MatchOver,sizeof(MatchOver));

					//���ͽ�����Ϣ
					TCHAR szMessage[128]=TEXT("�����������˽�����лл���Ĳ��룡");
					m_pIGameServiceFrame->SendGameMessage(szMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_GAME);
					
					return true;
				}

				//���ͽ�����Ϣ
				TCHAR szMessage[128]=TEXT("����ı������˽������������������μӣ�");
				m_pIGameServiceFrame->SendGameMessage(szMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_GAME);

				//�������
				DWORD dwStartInterval = GetMatchStartInterval();
				if(dwStartInterval>0)
				{
					m_pITimerEngine->SetTimer(IDI_CHECK_START_MATCH,dwStartInterval*1000L,TIMES_INFINITY,NULL);

					CString strMessage;
					strMessage.Format(TEXT("2 ��������ʱ��ʼ:%d�룡"),dwStartInterval);
					CTraceService::TraceString(strMessage,TraceLevel_Info);
				}
			}
			return true;
		}
	}
	return true;
}

//���ݿ��¼�
bool CLockTimeMatch::OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,DWORD dwContextID)
{
	switch(wRequestID)
	{
	case DBO_GR_MATCH_SIGNUP_RESULT:		//�������
		{
			//����Ч��
			if(pIServerUserItem==NULL) return true;
			if(wDataSize>sizeof(DBO_GR_MatchSingupResult)) return false;

			//��ȡ����
			DBO_GR_MatchSingupResult * pMatchSignupResult = (DBO_GR_MatchSingupResult*)pData;			

			//����ʧ��
			if(pMatchSignupResult->bResultCode==false)
			{
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,pMatchSignupResult->szDescribeString,SMT_EJECT);

				return true;
			}

			//�����ɹ�
			if(m_pIGameServiceFrame!=NULL)
			{
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem, TEXT("��ϲ�������ɹ�����ȴ�ϵͳΪ��������λ��"),SMT_CHAT);
			}

			//����״̬
			pIServerUserItem->SetSignUpStatus(MUS_SIGNUP);
			pIServerUserItem->SetSignUpTime(GetTickCount());

			//���óɼ�
			tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

			//��ʼ���ɼ�
			pUserInfo->lGrade=0L;
			pUserInfo->dwWinCount=0L;
			pUserInfo->dwLostCount=0L;
			pUserInfo->dwFleeCount=0L;
			pUserInfo->dwDrawCount=0L;		
			pUserInfo->lScore=(SCORE)m_pMatchOption->MatchType.LockTime.lMatchInitScore;

			//���ͳɼ�
			SendMatchUserScore(pIServerUserItem);

			//�������
			OnEventUserJoinMatch(pIServerUserItem,0,dwContextID);

			//��Ҹ���
			if(pIServerUserItem->IsAndroidUser()==false)
			{
				//����ṹ
				CMD_GR_MatchGoldUpdate MatchGoldUpdate;

				//���ñ���
				MatchGoldUpdate.lCurrGold=pMatchSignupResult->lCurrGold;
				MatchGoldUpdate.lCurrIngot=pMatchSignupResult->lCurrIngot;
				MatchGoldUpdate.dwCurrExprience=pIServerUserItem->GetUserInfo()->dwExperience;

				//��������
				m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_GOLDUPDATE,&MatchGoldUpdate,sizeof(MatchGoldUpdate));

				return true;
			}

			break;
		}
	case DBO_GR_MATCH_RANK_LIST:			//��������
		{
			//����У��
			ASSERT(wDataSize<=sizeof(DBO_GR_MatchRankList));
			if(wDataSize>sizeof(DBO_GR_MatchRankList)) return false;

			//��ȡ����
			DBO_GR_MatchRankList * pMatchRankList = (DBO_GR_MatchRankList*)pData;

			//��������
			bool bRewardUser=true;
			tagMatchRankInfo * pMatchRankInfo=NULL;
			IServerUserItem * pIRewardUserItem=NULL;

			//���Ž���
			for(WORD wIndex=0;wIndex<pMatchRankList->wUserCount;wIndex++)
			{
				//�������
				pMatchRankInfo=&pMatchRankList->MatchRankInfo[wIndex];
				pIRewardUserItem=m_pIServerUserManager->SearchUserItem(pMatchRankInfo->dwUserID);
				if (pIRewardUserItem!=NULL && pIRewardUserItem->IsAndroidUser()==true) continue;

				//д�뽱��
				WriteUserAward(pIRewardUserItem,pMatchRankInfo);

				//���ñ�ʶ
				if(pIRewardUserItem) pIRewardUserItem->SetMatchData(&bRewardUser);
			}

			//�����û�
			WORD wUserIndex=0;			
			while (true)
			{
				pIRewardUserItem=m_pIServerUserManager->EnumUserItem(wUserIndex++);
				if (pIRewardUserItem==NULL) break;
				if (pIRewardUserItem->IsAndroidUser()==true) continue;

				//���ñ���״̬
				pIRewardUserItem->SetSignUpStatus(MUS_NULL);
				if (pIRewardUserItem->GetMatchData()==&bRewardUser)
				{
					pIRewardUserItem->SetMatchData(NULL);
				}

				//���ͽ�����Ϣ
				TCHAR szMessage[128]=TEXT("���α������˽���������ǰ����վ�鿴ϵͳ�������Ƽ���������������������");
				if (pIRewardUserItem->GetTableID()!=INVALID_WORD)
				{
					m_pIGameServiceFrame->SendGameMessage(pIRewardUserItem,szMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_GAME);
				}
				else
				{
					m_pIGameServiceFrame->SendRoomMessage(pIRewardUserItem,szMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL);
				}
			}			

			break;
		}
	case DBO_GR_MATCH_REWARD_RESULT:	//�������
		{
			//����Ч��
			ASSERT(wDataSize==sizeof(DBO_GR_MatchRewardResult));
			if(wDataSize!=sizeof(DBO_GR_MatchRewardResult)) return false;

			//��ȡ����
			DBO_GR_MatchRewardResult * pMatchRewardResult = (DBO_GR_MatchRewardResult*)pData;
			IServerUserItem * pIRewardUserItem=m_pIServerUserManager->SearchUserItem(pMatchRewardResult->dwUserID);

			//�����ɹ�
			if(pMatchRewardResult->bResultCode==true && pIRewardUserItem!=NULL)
			{
				//�����û���Ϣ
				tagUserInfo * pUserInfo=pIRewardUserItem->GetUserInfo();
				pUserInfo->lIngot=pMatchRewardResult->lCurrIngot;
				pUserInfo->dwExperience=pMatchRewardResult->dwCurrExperience;

				//����֪ͨ
				OnEventUserItemScore(pIRewardUserItem,0);

				//����ṹ
				CMD_GR_MatchGoldUpdate MatchGoldUpdate;

				//���ñ���
				MatchGoldUpdate.lCurrGold=pMatchRewardResult->lCurrGold;
				MatchGoldUpdate.lCurrIngot=pMatchRewardResult->lCurrIngot;
				MatchGoldUpdate.dwCurrExprience=pMatchRewardResult->dwCurrExperience;

				//��������
				m_pIGameServiceFrame->SendData(pIRewardUserItem,MDM_GR_MATCH,SUB_GR_MATCH_GOLDUPDATE,&MatchGoldUpdate,sizeof(MatchGoldUpdate));
			}

			break;
		}
	}
	return true;
}

//�����¼�
bool CLockTimeMatch::OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_MATCH_FEE:	//��������
		{
			//����Ч��
			ASSERT(wDataSize==sizeof(SCORE));
			if(wDataSize!=sizeof(SCORE)) return false;

			//��ȡ����
			SCORE lMatchFee=*(SCORE*)pData;
			if(lMatchFee!=m_pMatchOption->lMatchFee)return false;

			//��������
			DBR_GR_MatchSignup MatchSignup;
			ZeroMemory(&MatchSignup,sizeof(MatchSignup));

			//����ṹ
			MatchSignup.dwUserID=pIServerUserItem->GetUserID();
			MatchSignup.lMatchFee=m_pMatchOption->lMatchFee;
			MatchSignup.cbMatchType=m_pMatchOption->cbMatchType;
			MatchSignup.dwInoutIndex=pIServerUserItem->GetInoutIndex();
			MatchSignup.dwClientAddr=pIServerUserItem->GetClientAddr();
			MatchSignup.dwMatchID=m_pMatchOption->dwMatchID;
			MatchSignup.dwMatchNO=m_pMatchOption->dwMatchNO;
			lstrcpyn(MatchSignup.szMachineID,pIServerUserItem->GetMachineID(),CountArray(MatchSignup.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(MatchSignup.dwUserID,DBR_GR_MATCH_SIGNUP,dwSocketID,&MatchSignup,sizeof(MatchSignup));

			return true;
		}
	}

	return true;
}
//�û�����
bool CLockTimeMatch::OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_UserScore UserScore;
	ZeroMemory(&UserScore,sizeof(UserScore));
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	//��������
	UserScore.dwUserID=pUserInfo->dwUserID;
	UserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;	
	UserScore.UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.UserScore.lLoveLiness=pUserInfo->lLoveLiness;

	//�������
	UserScore.UserScore.lGrade=pUserInfo->lGrade;
	UserScore.UserScore.lInsure=pUserInfo->lInsure;
	UserScore.UserScore.lIngot=pUserInfo->lIngot;
	UserScore.UserScore.dBeans=pUserInfo->dBeans;

	//�������
	UserScore.UserScore.lScore=pUserInfo->lScore;
	UserScore.UserScore.lScore+=pIServerUserItem->GetTrusteeScore();
	UserScore.UserScore.lScore+=pIServerUserItem->GetFrozenedScore();

	//��������
	m_pIGameServiceFrame->SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));

	//��������
	CMD_GR_MobileUserScore MobileUserScore;
	ZeroMemory(&MobileUserScore,sizeof(MobileUserScore));

	//��������
	MobileUserScore.dwUserID=pUserInfo->dwUserID;
	MobileUserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	MobileUserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	MobileUserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	MobileUserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	MobileUserScore.UserScore.dwExperience=pUserInfo->dwExperience;

	//�������
	MobileUserScore.UserScore.lScore=pUserInfo->lScore;
	MobileUserScore.UserScore.lScore+=pIServerUserItem->GetTrusteeScore();
	MobileUserScore.UserScore.lScore+=pIServerUserItem->GetFrozenedScore();
	MobileUserScore.UserScore.dBeans=pUserInfo->dBeans;

	//��������
	m_pIGameServiceFrame->SendDataBatchToMobileUser(pIServerUserItem->GetTableID(),MDM_GR_USER,SUB_GR_USER_SCORE,&MobileUserScore,sizeof(MobileUserScore));

	//��ʱд��
	if ((CServerRule::IsImmediateWriteScore(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->IsVariation()==true))
	{
		//��������
		DBR_GR_WriteGameScore WriteGameScore;
		ZeroMemory(&WriteGameScore,sizeof(WriteGameScore));

		//�û���Ϣ
		WriteGameScore.dwUserID=pIServerUserItem->GetUserID();
		WriteGameScore.dwDBQuestID=pIServerUserItem->GetDBQuestID();
		WriteGameScore.dwClientAddr=pIServerUserItem->GetClientAddr();
		WriteGameScore.dwInoutIndex=pIServerUserItem->GetInoutIndex();

		//��ȡ����
		pIServerUserItem->DistillVariation(WriteGameScore.VariationInfo);

		//������Ϣ
		WriteGameScore.dwMatchID=m_pMatchOption->dwMatchID;
		WriteGameScore.dwMatchNO=m_pMatchOption->dwMatchNO;

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(WriteGameScore.dwUserID,DBR_GR_WRITE_GAME_SCORE,0L,&WriteGameScore,sizeof(WriteGameScore),TRUE);
	}

	return true;
}

//�û�״̬
bool CLockTimeMatch::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID)
{
	//�������
	if(pIServerUserItem->GetUserStatus()==US_NULL) pIServerUserItem->SetMatchData(NULL);

	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemStatus(pIServerUserItem,wOldTableID,wOldChairID);
	}

	return true;
}

//�û�Ȩ��
bool CLockTimeMatch::OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,BYTE cbRightKind)
{
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemRight(pIServerUserItem,dwAddRight,dwRemoveRight,cbRightKind);
	}

	return true;
}

//�û���Ϸ����
bool CLockTimeMatch::OnEventUserItemGameData(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemGameData(pIServerUserItem,cbReason);
	}

	return true;
}

//�û���¼
bool CLockTimeMatch::OnEventUserLogon(IServerUserItem * pIServerUserItem)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//����״̬
	if(m_MatchStatus>MatchStatus_Free)
	{
		//����״̬
		BYTE cbMatchStatus=(m_MatchStatus==MatchStatus_Playing)?MS_MATCHING:MS_MATCHEND;
		m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_STATUS,&cbMatchStatus,sizeof(cbMatchStatus));

		//����ṹ
		CMD_GR_Match_Num MatchNum;
		ZeroMemory(&MatchNum,sizeof(MatchNum));

		//���ñ���		
		MatchNum.dwWaitting=m_pIServerUserManager->GetUserItemCount();

		//������Ϣ
		m_pIGameServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_MATCH,SUB_GR_MATCH_NUM,&MatchNum, sizeof(MatchNum));

		BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
		bool bUserSignup=CUserRight::IsGameMatchUser(pIServerUserItem->GetUserRight());
		pIServerUserItem->SetSignUpStatus(cbUserStatus>=US_PLAYING?MUS_MATCHING:(bUserSignup==true?MUS_SIGNUP:MUS_NULL));
	}

	return true;
}

//�û��ǳ�
bool CLockTimeMatch::OnEventUserLogout(IServerUserItem * pIServerUserItem)
{
	//����ṹ
	CMD_GR_Match_Num MatchNum;
	ZeroMemory(&MatchNum,sizeof(MatchNum));

	//���ñ���		
	MatchNum.dwWaitting=m_pIServerUserManager->GetUserItemCount();

	//������Ϣ
	m_pIGameServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_MATCH,SUB_GR_MATCH_NUM,&MatchNum, sizeof(MatchNum));

	return true;
}

//�����¼�
bool CLockTimeMatch::OnEventEnterMatch(DWORD dwSocketID ,VOID* pData,DWORD dwUserIP, bool bIsMobile)
{
	//�ж�ʱ��
	TCHAR szDescribe[256]=TEXT("");
	WORD wMessageType=0;
	if (VerifyMatchTime(szDescribe,CountArray(szDescribe),wMessageType)==false)
	{
		m_pIGameServiceFrame->SendRoomMessage(dwSocketID,szDescribe,wMessageType|SMT_GLOBAL,dwUserIP==0);
		return true;
	}

	//�ֻ��û�
	if(bIsMobile == true)
	{
		//������Ϣ
		CMD_GR_LogonMobile * pLogonMobile=(CMD_GR_LogonMobile *)pData;
		pLogonMobile->szPassword[CountArray(pLogonMobile->szPassword)-1]=0;
		pLogonMobile->szMachineID[CountArray(pLogonMobile->szMachineID)-1]=0;

		//��������
		DBR_GR_LogonMobile LogonMobile;
		ZeroMemory(&LogonMobile,sizeof(LogonMobile));

		//��������
		LogonMobile.dwUserID=pLogonMobile->dwUserID;
		LogonMobile.dwClientAddr=dwUserIP;		
		LogonMobile.dwMatchID=m_pMatchOption->dwMatchID;	
		LogonMobile.dwMatchNO=m_pMatchOption->dwMatchNO;
		LogonMobile.cbDeviceType=pLogonMobile->cbDeviceType;
		LogonMobile.wBehaviorFlags=pLogonMobile->wBehaviorFlags;
		LogonMobile.wPageTableCount=pLogonMobile->wPageTableCount;		
		lstrcpyn(LogonMobile.szPassword,pLogonMobile->szPassword,CountArray(LogonMobile.szPassword));
		lstrcpyn(LogonMobile.szMachineID,pLogonMobile->szMachineID,CountArray(LogonMobile.szMachineID));

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(LogonMobile.dwUserID,DBR_GR_LOGON_MOBILE,dwSocketID,&LogonMobile,sizeof(LogonMobile));
	}
	else
	{
		//������Ϣ
		CMD_GR_LogonUserID * pLogonUserID=(CMD_GR_LogonUserID *)pData;
		pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword)-1]=0;
		pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID)-1]=0;

		//��������
		DBR_GR_LogonUserID LogonUserID;
		ZeroMemory(&LogonUserID,sizeof(LogonUserID));

		//��������
		LogonUserID.dwClientAddr=dwUserIP;
		LogonUserID.dwUserID=pLogonUserID->dwUserID;
		LogonUserID.dwMatchID=m_pMatchOption->dwMatchID;	
		LogonUserID.dwMatchNO=m_pMatchOption->dwMatchNO;	
		lstrcpyn(LogonUserID.szPassword,pLogonUserID->szPassword,CountArray(LogonUserID.szPassword));
		lstrcpyn(LogonUserID.szMachineID,pLogonUserID->szMachineID,CountArray(LogonUserID.szMachineID));

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(LogonUserID.dwUserID,DBR_GR_LOGON_USERID,dwSocketID,&LogonUserID,sizeof(LogonUserID));
	}

	return true;
}

//�û�����
bool CLockTimeMatch::OnEventUserJoinMatch(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID)
{
	//����У��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem == NULL) return false;

	//�ж�״̬
	if(pIServerUserItem->GetUserStatus()!=US_FREE || pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		return false;
	}

	//����δ��ʼ
	if(m_MatchStatus==MatchStatus_Free)
	{
		//������Ϣ
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("������δ��ʼ�����Ժ�������"),SMT_EJECT);

		return true;
	}

	//��������
	if(m_MatchStatus==MatchStatus_End)
	{
		//������Ϣ
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("�����Ѿ������������Ƽ�����������������������"),SMT_EJECT|SMT_CLOSE_ROOM);
		return true;
	}

	//����״̬
	if (pIServerUserItem->GetSignUpStatus()==MUS_NULL)
	{
		//���˿۱�����
		if(m_pMatchOption->lMatchFee>0 && pIServerUserItem->IsAndroidUser()==false)
		{
			ASSERT(m_pIGameServiceFrame!=NULL);
			if(m_pIGameServiceFrame==NULL)
			{
				CTraceService::TraceString(TEXT("������ָ��Ϊ��!"),TraceLevel_Warning);
				return true;
			}

			//����ṹ
			CMD_GR_Match_Fee MatchFee;
			MatchFee.lMatchFee=m_pMatchOption->lMatchFee;

			//������ʾ
			if(MatchFee.lMatchFee>0)
			{
				//��������
				LPCTSTR pszMatchFeeType[]={TEXT("��Ϸ��"),TEXT("Ԫ��")};
				if(m_pMatchOption->cbMatchFeeType<CountArray(pszMatchFeeType))
				{
					_sntprintf(MatchFee.szNotifyContent,CountArray(MatchFee.szNotifyContent),TEXT("�������۳������� %I64d %s��ȷ��Ҫ������"),m_pMatchOption->lMatchFee,pszMatchFeeType[m_pMatchOption->cbMatchFeeType]);
				}
			}

			//������Ϣ
			WORD wDataSize=sizeof(MatchFee)-sizeof(MatchFee.szNotifyContent);
			wDataSize+=CountStringBuffer(MatchFee.szNotifyContent);
			m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_FEE,&MatchFee,wDataSize);

			return true;
		}

		//�����˱���
		if(m_pMatchOption->lMatchFee==0 || pIServerUserItem->IsAndroidUser()==true)
		{
			//�������ж�
			/*if (pIServerUserItem->IsAndroidUser()==true)
			{
				return true;
			}*/

			return OnEventSocketMatch(SUB_GR_MATCH_FEE,&m_pMatchOption->lMatchFee,sizeof(SCORE),pIServerUserItem,dwSocketID);
		}
	}	

	//�������
	if(InsertDistribute(pIServerUserItem)==false) return false;
	//pIServerUserItem->SetSignUpStatus(MUS_SIGNUP);
	
	//���ͳɼ�
	SendMatchUserScore(pIServerUserItem);

	//������Ϣ
	m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("ϵͳ����Ŭ��Ϊ��������λ�����Ժ󡣡���"),SMT_CHAT);

	return true;
}

//�û�����
bool CLockTimeMatch::OnEventUserQuitMatch(IServerUserItem * pIServerUserItem, BYTE cbReason, WORD *pBestRank, DWORD dwSocketID)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�Ƴ�����
	m_DistributeManage.RemoveDistributeNode(pIServerUserItem);

	//�Ƴ�ͬ����Ϣ
	m_DistributeManage.RemoveUserSameTableInfo(pIServerUserItem->GetUserID());

	return true;
}

//�û�״̬
bool CLockTimeMatch::OnEventMatchUserStatus(IServerUserItem * pIServerUserItem, BYTE cbOldUserStatus, BYTE cbCurrUserStatus)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return true;

	CMD_GR_UserMatchStatus UserMatchStatus;
	ZeroMemory(&UserMatchStatus,sizeof(UserMatchStatus));

	UserMatchStatus.dwUserID=pIServerUserItem->GetUserID();
	UserMatchStatus.cbOldStatus=cbOldUserStatus;
	UserMatchStatus.cbCurrStatus=cbCurrUserStatus;

	//������Ϣ
	ASSERT(m_pIGameServiceFrame!=NULL);
	if (m_pIGameServiceFrame==NULL) return false;
	m_pIGameServiceFrame->SendData(pIServerUserItem, MDM_GR_MATCH, SUB_GR_USER_MATCH_STATUS, &UserMatchStatus, sizeof(UserMatchStatus));

	return true;
}

//��Ϸ��ʼ
bool CLockTimeMatch::OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount)
{
	//���ͱ�����Ϣ
	SendTableUserMatchInfo(pITableFrame, INVALID_CHAIR);

	//�������ݰ�
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//��������
	IServerUserItem *pIServerUserItem=NULL;
	IServerUserItem *pITableUserItem=NULL;	

	for (INT i=0; i<pITableFrame->GetChairCount(); i++)
	{
		pIServerUserItem=pITableFrame->GetTableUserItem(i);
		//ASSERT(pIServerUserItem!=NULL);
		if (pIServerUserItem==NULL) continue;

		//������ʾ��Ϣ
		TCHAR szMessage[64]=TEXT("");
		_sntprintf(szMessage, CountArray(szMessage),TEXT("���ֱ��������ĵ� %d �֡�"),pIServerUserItem->GetUserPlayCount()+1);
		lstrcpyn(SystemMessage.szString,szMessage,CountArray(SystemMessage.szString));
		SystemMessage.wLength=CountStringBuffer(SystemMessage.szString);
		SystemMessage.wType=SMT_CHAT;

		//������ʾ����
		WORD wSendSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString)+SystemMessage.wLength*sizeof(TCHAR);
		pITableFrame->SendTableData(pIServerUserItem->GetChairID(),SUB_GF_SYSTEM_MESSAGE, &SystemMessage,wSendSize,MDM_GF_FRAME);

		//���ͬ��
		ClearSameTableUser(pIServerUserItem->GetUserID());

		for (INT j=0; j<pITableFrame->GetChairCount(); j++)
		{
			//��ȡ�û�
			pITableUserItem=pITableFrame->GetTableUserItem(j);
			if(pITableUserItem==NULL) continue;
			if(pITableUserItem==pIServerUserItem) continue;

			//����ͬ���û�
			InserSameTableUser(pIServerUserItem->GetUserID(),pITableUserItem->GetUserID());
		}
	}

	return true;
}

//��Ϸ����
bool CLockTimeMatch::OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	ASSERT(pITableFrame!=NULL);
	if (pITableFrame==NULL) return false;

	//��������
	for (INT i=0; i<pITableFrame->GetChairCount(); i++)
	{
		IServerUserItem *pIUserItem=pITableFrame->GetTableUserItem(i);
		//ASSERT(pIUserItem!=NULL);
		if (pIUserItem==NULL) continue;

		//��������
		if(cbReason!=REMOVE_BY_ENDMATCH)
		{
			//�ﵽ��̭��
			if (m_pMatchOption->MatchType.LockTime.lMatchCullScore>0 && pIUserItem->GetUserScore()<m_pMatchOption->MatchType.LockTime.lMatchCullScore)
			{
				//��������
				TCHAR szMessage[128]=TEXT("");
				_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ���������ı����ҵ��� %d���������ܼ�����Ϸ��"),m_pMatchOption->MatchType.LockTime.lMatchCullScore);
				pIUserItem->SetSignUpStatus(MUS_NULL);

				//������Ϣ
				m_pIGameServiceFrame->SendGameMessage(pIUserItem,szMessage,SMT_EJECT|SMT_CHAT);

				//����ṹ
				DBR_GR_MatchEliminate MatchEliminate;
				MatchEliminate.cbMatchType=MATCH_TYPE_LOCKTIME;
				MatchEliminate.wServerID=m_pGameServiceOption->wServerID;
				MatchEliminate.dwMatchID=m_pMatchOption->dwMatchID;
				MatchEliminate.dwMatchNO=m_pMatchOption->dwMatchNO;
				MatchEliminate.dwUserID=pIUserItem->GetUserID();

				//Ͷ������
				m_pIDataBaseEngine->PostDataBaseRequest(MatchEliminate.dwUserID,DBR_GR_MATCH_ELIMINATE,0,&MatchEliminate,sizeof(MatchEliminate));

				//��������
				m_pIGameServiceFrame->SendData(pIUserItem,MDM_GR_MATCH,SUB_GR_MATCH_ELIMINATE,NULL,0);

				continue;
			}

			//�������
			if(InsertDistribute(pIUserItem)==true)
			{
				m_pIGameServiceFrame->SendGameMessage(pIUserItem,TEXT("���ֱ���������ϵͳ����Ŭ��Ϊ��������λ�����Ժ�..."),SMT_CHAT|SMT_TABLE_ROLL);
			}

			//��ɾ���
			if(pIUserItem->GetUserPlayCount()-pIUserItem->GetUserInfo()->dwFleeCount==m_pMatchOption->MatchType.LockTime.wMatchPlayCount)
			{
				TCHAR szMessage[128]=TEXT("");
				_sntprintf(szMessage,CountArray(szMessage),TEXT("�������%d�ֱ��������Լ�����ǰ������ѡ���˳��ȴ����α����������������"),m_pMatchOption->MatchType.LockTime.wMatchPlayCount);
				m_pIGameServiceFrame->SendGameMessage(pIUserItem,szMessage,SMT_CHAT|SMT_TABLE_ROLL);
			}
		}
	}

	return true;
}

//��ҷ���
bool CLockTimeMatch::OnEventUserReturnMatch(ITableFrame *pITableFrame,IServerUserItem * pIServerUserItem)
{
	//���ͱ�����Ϣ
	SendTableUserMatchInfo(pITableFrame,pIServerUserItem->GetChairID());

	return true;
}

//�û�����
bool CLockTimeMatch::OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{ 
	return true; 
}

//�û�����
bool CLockTimeMatch::OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if ((m_pGameServiceOption->wKindID==510 || m_pGameServiceOption->wKindID==511)&& pIServerUserItem!=NULL)
	{
		//�ﵽ��̭��
		if (m_pMatchOption->MatchType.LockTime.lMatchCullScore>0 && pIServerUserItem->GetUserScore()<m_pMatchOption->MatchType.LockTime.lMatchCullScore)
		{
			//��������
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ���������ı����ҵ��� %d ����̭���������ܼ�����Ϸ��"),m_pMatchOption->MatchType.LockTime.lMatchCullScore);
			pIServerUserItem->SetSignUpStatus(MUS_NULL);

			//������Ϣ
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,szMessage,SMT_EJECT|SMT_CHAT);

			//����ṹ
			DBR_GR_MatchEliminate MatchEliminate;
			MatchEliminate.cbMatchType=MATCH_TYPE_LOCKTIME;
			MatchEliminate.wServerID=m_pGameServiceOption->wServerID;
			MatchEliminate.dwMatchID=m_pMatchOption->dwMatchID;
			MatchEliminate.dwMatchNO=m_pMatchOption->dwMatchNO;
			MatchEliminate.dwUserID=pIServerUserItem->GetUserID();

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(MatchEliminate.dwUserID,DBR_GR_MATCH_ELIMINATE,0,&MatchEliminate,sizeof(MatchEliminate));

			//��������
			m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_ELIMINATE,NULL,0);
		}

		//��ɾ���
		if(pIServerUserItem->GetUserPlayCount()-pIServerUserItem->GetUserInfo()->dwFleeCount==m_pMatchOption->MatchType.LockTime.wMatchPlayCount)
		{
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("�������%d�ֱ��������Լ�����ǰ������ѡ���˳��ȴ����α����������������"),m_pMatchOption->MatchType.LockTime.wMatchPlayCount);
			m_pIGameServiceFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_CHAT|SMT_TABLE_ROLL);
		}
	}
	//�Ƴ�����
	return RemoveDistribute(pIServerUserItem);
}

 //�û�ͬ��
bool CLockTimeMatch::OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{ 
	return true; 
}

//����Ȩ��
bool CLockTimeMatch::VerifyUserEnterMatch(IServerUserItem * pIServerUserItem)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�����˲�������
	if (pIServerUserItem->IsAndroidUser()==true)
		return true;

	//��������
	WORD wMessageType=0;
	TCHAR szMessage[128]=TEXT("");

	//����ʱ���ѵ�	
	if (VerifyMatchTime(szMessage,CountArray(szMessage),wMessageType)==false)
	{
		//������Ϣ
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,szMessage,wMessageType);

		return false;
	}

	BYTE cbSignUpStatus=pIServerUserItem->GetSignUpStatus();
	if (pIServerUserItem->GetSignUpStatus()==MUS_NULL)
	{
		if (m_pMatchOption->MatchType.LockTime.lMatchCullScore>0 && pIServerUserItem->GetUserScore()<m_pMatchOption->MatchType.LockTime.lMatchCullScore)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ���������ı����ҵ��� %d ����̭���������ܼ�����Ϸ��"),m_pMatchOption->MatchType.LockTime.lMatchCullScore);
		}
		else
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ�����Ѿ�����̭�������ٴβ������뷵����Ϸ�������±�����"),m_pMatchOption->MatchType.LockTime.lMatchCullScore);
		}

		//������Ϣ
		if(pIServerUserItem->GetTableID()==INVALID_TABLE)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,szMessage,SMT_EJECT|SMT_CHAT|SMT_CLOSE_ROOM);
		}
		else
		{
			m_pIGameServiceFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_EJECT|SMT_CHAT|SMT_CLOSE_ROOM);
		}

		return false;
	}

	if (m_pMatchOption->MatchType.LockTime.lMatchCullScore>0 && pIServerUserItem->GetUserScore()<m_pMatchOption->MatchType.LockTime.lMatchCullScore)
	{
		_sntprintf(szMessage,CountArray(szMessage),TEXT("��Ǹ���������ı����ҵ��� %d���������ܼ�����Ϸ��"),m_pMatchOption->MatchType.LockTime.lMatchCullScore);

		//������Ϣ
		if(pIServerUserItem->GetTableID()==INVALID_TABLE)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,szMessage,SMT_EJECT|SMT_CHAT);
		}
		else
		{
			m_pIGameServiceFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_EJECT|SMT_CHAT);
		}

		return false;
	}

	return true;
}

//��ʼ���
DWORD CLockTimeMatch::GetMatchStartInterval()
{
	//��ȡʱ��
	CTime CurTime=CTime::GetCurrentTime();
	CTime MatchStartTime(m_pMatchOption->MatchType.LockTime.MatchStartTime);
	CTime MatchEndTime(m_pMatchOption->MatchType.LockTime.MatchEndTime);

	//��������
	DWORD dwTimeInterval=1;
	DWORD dwCurrStamp,dwStartStamp,dwEndStamp;

	//����δ��ʼ
	if(CurTime<MatchStartTime)
	{
		CTimeSpan TimeSpan = MatchStartTime-CurTime;
		dwTimeInterval = TimeSpan.GetTotalSeconds();
	}

	//�����ѽ���
	if(CurTime>=MatchEndTime) dwTimeInterval=0;

	//����ʱ��
	dwCurrStamp = CurTime.GetHour()*3600+CurTime.GetMinute()*60+CurTime.GetSecond();
	dwStartStamp = m_pMatchOption->MatchType.LockTime.MatchStartTime.wHour*3600+m_pMatchOption->MatchType.LockTime.MatchStartTime.wMinute*60+m_pMatchOption->MatchType.LockTime.MatchStartTime.wSecond;
	dwEndStamp = m_pMatchOption->MatchType.LockTime.MatchEndTime.wHour*3600+m_pMatchOption->MatchType.LockTime.MatchEndTime.wMinute*60+m_pMatchOption->MatchType.LockTime.MatchEndTime.wSecond;

	//����δ��ʼ
	if(dwCurrStamp<dwStartStamp) 
	{
		dwTimeInterval = dwStartStamp-dwCurrStamp;
	}

	//�����ѽ���
	if(dwCurrStamp>=dwEndStamp)
	{
		dwTimeInterval = 24*3600-(dwCurrStamp-dwStartStamp);		
	}

	return dwTimeInterval;
}

//�����û�
bool CLockTimeMatch::PerformDistribute()
{
	//״̬�ж�
	if(m_MatchStatus!=MatchStatus_Playing) return true;

	//����У��
	if(m_DistributeManage.GetCount()<m_pGameServiceOption->wMinDistributeUser) return false;

	//�����û�
	while(true)
	{
		//����У��
		if(m_DistributeManage.GetCount()<m_pGameServiceOption->wMinPartakeGameUser) break;

		//��������
		CDistributeInfoArray DistributeInfoArray;

        //��ȡ�û�
		WORD wRandCount = __max(m_pGameServiceOption->wMaxPartakeGameUser-m_pGameServiceOption->wMinPartakeGameUser+1,1);
		WORD wChairCount = m_pGameServiceOption->wMinPartakeGameUser+rand()%wRandCount;
		WORD wDistributeCount = m_DistributeManage.PerformDistribute(DistributeInfoArray,wChairCount);
		if(wDistributeCount < wChairCount) break;

		//Ѱ��λ��
		ITableFrame * pICurrTableFrame=NULL;
		for (WORD i=0;i<m_pGameServiceOption->wTableCount;i++)
		{
			//��ȡ����
			ASSERT(m_ppITableFrame[i]!=NULL);
			ITableFrame * pITableFrame=m_ppITableFrame[i];

			//״̬�ж�
			if (pITableFrame->GetNullChairCount()>=wDistributeCount)
			{
				pICurrTableFrame=pITableFrame;
				break;
			}
		}

		//�����ж�
		if(pICurrTableFrame==NULL) break;

		//�������
		bool bSitSuccess=true;
		INT_PTR nSitFailedIndex=INVALID_CHAIR;
		for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
		{
			//��������
			WORD wChairID=INVALID_CHAIR;

			//��������
			for (WORD i=0;i<pICurrTableFrame->GetChairCount();i++)
			{
				if (pICurrTableFrame->GetTableUserItem(i)==NULL)
				{
					wChairID = i;
					break;
				}
			}

			//�����û�
			if (wChairID!=INVALID_CHAIR)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=DistributeInfoArray[nIndex].pIServerUserItem;

				//�û�����
				if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
				{
					WORD wTableID=pIServerUserItem->GetTableID();
					ITableFrame * pITableFrame = m_ppITableFrame[wTableID];
					pITableFrame->PerformStandUpAction(pIServerUserItem);
				}

				//�û�����
				if(pICurrTableFrame->PerformSitDownAction(wChairID,pIServerUserItem)==false)
				{
					bSitSuccess=false;
					nSitFailedIndex=nIndex;
					pIServerUserItem->SetSignUpStatus(MUS_MATCHING);
					break;
				}
			}
		}

		//���½��
		if(bSitSuccess)
		{
			//�Ƴ����
			while(DistributeInfoArray.GetCount()>0)
			{
				m_DistributeManage.RemoveDistributeNode((tagDistributeNode *)DistributeInfoArray[0].pPertainNode);
				DistributeInfoArray.RemoveAt(0);
			}
		}
		else
		{
			for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
			{
				if(nSitFailedIndex==nIndex)
				{
					m_DistributeManage.RemoveDistributeNode((tagDistributeNode *)DistributeInfoArray[nIndex].pPertainNode);
				}
				else
				{
					//�û�����
					if (DistributeInfoArray[nIndex].pIServerUserItem->GetTableID()!=INVALID_TABLE)
					{
						WORD wTableID=DistributeInfoArray[nIndex].pIServerUserItem->GetTableID();
						m_ppITableFrame[wTableID]->PerformStandUpAction(DistributeInfoArray[nIndex].pIServerUserItem);
					}
				}
			}
		}		
	}

	return true;
}

//�����û�
bool CLockTimeMatch::InsertDistribute(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//����״̬
 	if(m_MatchStatus!=MatchStatus_Playing) return false;

	//״̬�ж�
	ASSERT(pIServerUserItem->GetUserStatus()<US_PLAYING);
	if (pIServerUserItem->GetUserStatus()>=US_PLAYING) return false;

	//����У��
	if(VerifyUserEnterMatch(pIServerUserItem)==false) return false;

	//��������
	tagDistributeInfo DistributeInfo;
	ZeroMemory(&DistributeInfo,sizeof(DistributeInfo));

	//���ñ���
	DistributeInfo.pPertainNode=NULL;	
	DistributeInfo.dwInsertStamp = (DWORD)time(NULL);
	DistributeInfo.pIServerUserItem=pIServerUserItem;   

	//����ڵ�
	if(m_DistributeManage.InsertDistributeNode(DistributeInfo)==false)
	{
		return pIServerUserItem->IsAndroidUser()?false:true; 
	}

	return true;
}

//������
void CLockTimeMatch::ClearSameTableUser(DWORD dwUserID)
{
	//��������
	tagSameTableInfo * pSameTableInfo = m_DistributeManage.GetUserSameTableInfo(dwUserID);
	if(pSameTableInfo!=NULL)
	{
		pSameTableInfo->wPlayerCount=0;
		ZeroMemory(pSameTableInfo->wPlayerIDSet,sizeof(pSameTableInfo->wPlayerIDSet));
	}
}

//�����û�
void CLockTimeMatch::InserSameTableUser(DWORD dwUserID,DWORD dwTableUserID)
{
	//��������
	tagSameTableInfo * pSameTableInfo = m_DistributeManage.GetUserSameTableInfo(dwUserID);
	if(pSameTableInfo!=NULL)
	{
		//����û�
		pSameTableInfo->wPlayerIDSet[pSameTableInfo->wPlayerCount++] = dwTableUserID;
	}

	return;
}

//�¼�У��
bool CLockTimeMatch::VerifyMatchTime(LPTSTR pszMessage,WORD wMaxCount,WORD & wMessageType)
{
	//��������
	CTime CurTime=CTime::GetCurrentTime();
	CTime MatchStartTime(m_pMatchOption->MatchType.LockTime.MatchStartTime);
	CTime MatchEndTime(m_pMatchOption->MatchType.LockTime.MatchEndTime);

	//��������
	wMessageType = SMT_EJECT|SMT_CHAT;

	////����δ��ʼ
	//if(CurTime<MatchStartTime)
	//{
	//	wMessageType |= SMT_CLOSE_ROOM;
	//	_sntprintf(pszMessage,wMaxCount,TEXT("������δ��ʼ,������%d��%d��%d��-%dʱ%d��%d��ǰ���μӱ�����"),MatchStartTime.GetYear(),MatchStartTime.GetMonth(),MatchStartTime.GetDay(),MatchStartTime.GetHour(),MatchStartTime.GetMinute(),MatchStartTime.GetSecond());
	//	return false;
	//}

	//�����ѽ���
	if(CurTime>=MatchEndTime)
	{
		wMessageType |= SMT_CLOSE_ROOM|SMT_CLOSE_GAME;
		_sntprintf(pszMessage,wMaxCount,TEXT("�����Ѿ�����,�����Ƽ����������������������ɣ�"));
		return false;
	}

	////����δ��ʼ
	//if(CurTime.GetHour()*3600+CurTime.GetMinute()*60+CurTime.GetSecond()<
	//  m_pMatchOption->MatchType.LockTime.MatchStartTime.wHour*3600+m_pMatchOption->MatchType.LockTime.MatchStartTime.wMinute*60+m_pMatchOption->MatchType.LockTime.MatchStartTime.wSecond)
	//{
	//	wMessageType |= SMT_CLOSE_ROOM|SMT_CLOSE_GAME;
	//	_sntprintf(pszMessage,wMaxCount,TEXT("����ı�����δ��ʼ,������%dʱ%d��%d��ǰ���μӱ�����"),MatchStartTime.GetHour(),MatchStartTime.GetMinute(),MatchStartTime.GetSecond());
	//	return false;
	//}

	//�����ѽ���
	if(CurTime.GetHour()*3600+CurTime.GetMinute()*60+CurTime.GetSecond()>= m_pMatchOption->MatchType.LockTime.MatchEndTime.wHour*3600+m_pMatchOption->MatchType.LockTime.MatchEndTime.wMinute*60+m_pMatchOption->MatchType.LockTime.MatchEndTime.wSecond)
	{
		wMessageType |= SMT_CLOSE_ROOM|SMT_CLOSE_GAME;
		_sntprintf(pszMessage,wMaxCount,TEXT("����ı����Ѿ�����,����������%dʱ%d��%d��ǰ���μӱ�����"),MatchStartTime.GetHour(),MatchStartTime.GetMinute(),MatchStartTime.GetSecond());
		return false;
	}

	return true;
}

//�����û�����
bool CLockTimeMatch::SendMatchUserScore(IServerUserItem * pIServerUserItem)
{
	//��������
	CMD_GR_UserScore UserScore;
	ZeroMemory(&UserScore,sizeof(UserScore));

	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	//��������
	UserScore.dwUserID=pUserInfo->dwUserID;
	UserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;	
	UserScore.UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.UserScore.lLoveLiness=pUserInfo->lLoveLiness;

	//�������
	UserScore.UserScore.lGrade=pUserInfo->lGrade;
	UserScore.UserScore.lInsure=pUserInfo->lInsure;
	UserScore.UserScore.lScore=pUserInfo->lScore;
	UserScore.UserScore.lIngot=pUserInfo->lIngot;
	UserScore.UserScore.dBeans=pUserInfo->dBeans;

	//��������
	CMD_GR_MobileUserScore MobileUserScore;
	ZeroMemory(&MobileUserScore,sizeof(MobileUserScore));

	//��������
	MobileUserScore.dwUserID=pUserInfo->dwUserID;
	MobileUserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	MobileUserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	MobileUserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	MobileUserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	MobileUserScore.UserScore.dwExperience=pUserInfo->dwExperience;
	MobileUserScore.UserScore.lScore=pUserInfo->lScore;
	MobileUserScore.UserScore.dBeans=pUserInfo->dBeans;

	//ö���û�
	WORD wEnumIndex=0;
	while(wEnumIndex<m_pIServerUserManager->GetUserItemCount())
	{
		//�����û�
		IServerUserItem *pIUserItem=m_pIServerUserManager->EnumUserItem(wEnumIndex++);
		if (pIUserItem==NULL) continue;

		if (pIUserItem->IsMobileUser()==true)
			m_pIGameServiceFrame->SendData(pIUserItem,MDM_GR_USER,SUB_GR_USER_SCORE,&MobileUserScore,sizeof(MobileUserScore));
		else
			m_pIGameServiceFrame->SendData(pIUserItem,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));
	}
	return true;
}

//�Ƴ����
bool CLockTimeMatch::RemoveDistribute(IServerUserItem * pIServerUserItem)
{
	//��֤����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�Ƴ��û�
	m_DistributeManage.RemoveDistributeNode(pIServerUserItem);

	return true;
}


//���ͱ�����Ϣ
void CLockTimeMatch::SendTableUserMatchInfo(ITableFrame *pITableFrame, WORD wChairID)
{
	//��֤����
	ASSERT(pITableFrame!=NULL);
	if (pITableFrame==NULL) return;

	//��������
	CMD_GR_Match_Info MatchInfo;
	ZeroMemory(&MatchInfo,sizeof(MatchInfo));

	//���ñ���
	_sntprintf(MatchInfo.szTitle[0], CountArray(MatchInfo.szTitle[0]),TEXT("���ַ�ʽ���ۼƻ���"));
	_sntprintf(MatchInfo.szTitle[1], CountArray(MatchInfo.szTitle[1]),TEXT("��Ϸ������%d"), m_pMatchOption->MatchType.LockTime.lMatchInitScore);
	_sntprintf(MatchInfo.szTitle[2], CountArray(MatchInfo.szTitle[2]),TEXT("�������ƣ���ʱ����"));

	for(WORD i=0; i<pITableFrame->GetChairCount(); i++)
	{
		//��ȡ�û�
		if(wChairID!=INVALID_CHAIR && wChairID!=i) continue;
		IServerUserItem *pIServerUserItem=pITableFrame->GetTableUserItem(i);
		if(pIServerUserItem==NULL) continue;

		//���þ���
		MatchInfo.wGameCount=(WORD)pIServerUserItem->GetUserPlayCount()+1;

		//��������
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			pITableFrame->SendTableData(i,SUB_GR_MATCH_INFO,&MatchInfo,sizeof(MatchInfo),MDM_GF_FRAME);
		}
	}

	return;
}

//д�뽱��
bool CLockTimeMatch::WriteUserAward(IServerUserItem *pIServerUserItem,tagMatchRankInfo * pMatchRankInfo)
{
	//����Ч��
	ASSERT(pMatchRankInfo!=NULL);
	if(pMatchRankInfo==NULL) return false;

	//д���¼
	if(m_pIDataBaseEngine!=NULL)
	{
		//��������
		DBR_GR_MatchReward MatchReward;
		ZeroMemory(&MatchReward,sizeof(MatchReward));
		
		//����ṹ
		MatchReward.dwUserID=pMatchRankInfo->dwUserID;	
		MatchReward.lRewardGold=pMatchRankInfo->lRewardGold;
		MatchReward.dwRewardIngot=pMatchRankInfo->dwRewardIngot;
		MatchReward.dwRewardExperience=pMatchRankInfo->dwRewardExperience;
		MatchReward.dwClientAddr=pIServerUserItem?pIServerUserItem->GetClientAddr():0;

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(MatchReward.dwUserID,DBR_GR_MATCH_REWARD,0,&MatchReward,sizeof(MatchReward));
	}

	//�������
	if (pIServerUserItem!=NULL)
	{
		//��������
		CMD_GR_MatchResult MatchResult;
		ZeroMemory(&MatchResult,sizeof(MatchResult));

		//��������
		MatchResult.lGold=pMatchRankInfo->lRewardGold;
		MatchResult.dwIngot=pMatchRankInfo->dwRewardIngot;
		MatchResult.dwExperience=pMatchRankInfo->dwRewardExperience;

		//����ʾ
		_sntprintf(MatchResult.szDescribe,CountArray(MatchResult.szDescribe),TEXT("%s, ��ϲ����%s�л�õ�%d��,�������£�"),
		pIServerUserItem->GetNickName(),m_pMatchOption->szMatchName,pMatchRankInfo->wRankID);

		//��������
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			m_pIGameServiceFrame->SendData(pIServerUserItem, MDM_GF_FRAME, SUB_GR_MATCH_RESULT, &MatchResult, sizeof(MatchResult));
		}
	}		

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
