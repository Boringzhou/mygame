#include "StdAfx.h"
#include "ImmediateMatch.h"
#include "..\��Ϸ������\DataBasePacket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
#define INVALID_VALUE				0xFFFF								//��Чֵ

//ȡ������ԭ��
#define UNSIGNUP_REASON_PLAYER		0									//���ȡ��
#define UNSIGNUP_REASON_SYSTEM 		1									//ϵͳȡ��

//////////////////////////////////////////////////////////////////////////

//ʱ�Ӷ���
#define IDI_DISTRIBUTE_USER		    (IDI_MATCH_MODULE_START+100)		//�����û�

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CImmediateMatch::CImmediateMatch()
{
	//��������
	m_pMatchOption=NULL;	
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;

	//����ָ��
	m_ppITableFrame=NULL;
	m_pCurMatchGroup=NULL;

	//�ں˽ӿ�
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngineEvent=NULL;

	//����ӿ�
	m_pIGameServiceFrame=NULL;
	m_pIServerUserManager=NULL;
	m_pAndroidUserManager=NULL;
	m_pIServerUserItemSink=NULL;
}

CImmediateMatch::~CImmediateMatch(void)
{
	SafeDeleteArray(m_ppITableFrame);
	
	m_LoopTimer.RemoveAll();
	SafeDelete(m_pCurMatchGroup);
	m_MatchGroup.Append(m_OverMatchGroup);
	for (int i=0; i<m_MatchGroup.GetCount(); i++)
	{
		SafeDelete(m_MatchGroup[i]);
	}

	m_MatchGroup.RemoveAll();
	m_OverMatchGroup.RemoveAll();
}

//�ӿڲ�ѯ
VOID* CImmediateMatch::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{	
	QUERYINTERFACE(IGameMatchItem,Guid,dwQueryVer);	
	QUERYINTERFACE(IServerUserItemSink,Guid,dwQueryVer);	
	QUERYINTERFACE_IUNKNOWNEX(IGameMatchItem,Guid,dwQueryVer);
	return NULL;
}

//����֪ͨ
void CImmediateMatch::OnStartService()
{
	//��������
	if (m_pCurMatchGroup==NULL)
	{
		m_pCurMatchGroup=new CImmediateGroup(CalcMatchNo());
		if (m_pCurMatchGroup!=NULL)
		{
			tagImmediateMatchParameter ImmediateMatchParameter;
			ImmediateMatchParameter.pGameMatchOption=m_pMatchOption;
			ImmediateMatchParameter.pGameServiceAttrib=m_pGameServiceAttrib;
			ImmediateMatchParameter.pGameServiceOption=m_pGameServiceOption;
			ImmediateMatchParameter.pIImmediateGroupSink=this;
			m_pCurMatchGroup->SetImmediateGroup(ImmediateMatchParameter);
		}
	}

	//���ͬʱ9����
	for (int i=0;i<9;i++)
	{
		m_LoopTimer.Add(i);
	}
	m_pITimerEngine->SetTimer(IDI_CHECK_OVER_MATCH,60000L,TIMES_INFINITY,0);
	m_pITimerEngine->SetTimer(IDI_DELETE_OVER_MATCH, 6000,TIMES_INFINITY,0);

	InsertNullTable();
}

//������
bool CImmediateMatch::BindTableFrame(ITableFrame * pTableFrame,WORD wTableID)
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
bool CImmediateMatch::InitMatchInterface(tagMatchManagerParameter & MatchManagerParameter)
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

	//������������
	CImmediateGroup::m_wChairCount=m_pGameServiceAttrib->wChairCount;
	if (m_ppITableFrame==NULL)
	{
		m_ppITableFrame=new ITableFrame*[m_pGameServiceOption->wTableCount];
	}

	return true;
}

//ʱ���¼�
bool CImmediateMatch::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{	
	if(dwTimerID>=IDI_GROUP_TIMER_START)
	{
		for (INT_PTR i=m_MatchGroup.GetCount()-1;i>=0;i--)
		{
			//�鿴���ĳ������Ķ�ʱ��
			if (m_MatchGroup[i]->m_LoopTimer==((dwTimerID-IDI_GROUP_TIMER_START)/10))
			{
				m_MatchGroup[i]->OnTimeMessage(dwTimerID-(m_MatchGroup[i]->m_LoopTimer*10),dwBindParameter);
			}
		}
		return true;
	}

	switch(dwTimerID)
	{
	case IDI_CHECK_OVER_MATCH:
		{
			//��ʱ���һ�� ��Щ����������������������
			for (INT_PTR i=m_MatchGroup.GetCount()-1;i>=0;i--)
			{
				//�����û�С���������� ��Ҫ����
				m_MatchGroup[i]->CheckMatchUser();
			}
			return true;
		}
	case IDI_DELETE_OVER_MATCH:
		{
			for (int i=0;m_OverMatchGroup.GetCount();i++)
			{
				CImmediateGroup * pOverMatch=m_OverMatchGroup[i];
				m_OverMatchGroup.RemoveAt(i--);
				SafeDelete(pOverMatch);
			}
			return true;
		}
	}

	return true;
}


//���ݿ��¼�
bool CImmediateMatch::OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,DWORD dwContextID)
{
	switch (wRequestID)
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

			//�жϳ���
			if (m_pCurMatchGroup->m_dwMatchNO!=pMatchSignupResult->dwMatchNO)
			{
				CTraceService::TraceString(TEXT("�����Ѿ���ʼ,����ʧ��,�����±�����һ��������"),TraceLevel_Exception);
				SendRoomMessage(pIServerUserItem, TEXT("�����Ѿ���ʼ,����ʧ��,�����±�����һ��������"),SMT_CHAT|SMT_EJECT);

				//�˻�����
				if (m_pMatchOption->lMatchFee>0)
				{
					//��������
					DBR_GR_MatchUnSignup MatchUnSignup;
					MatchUnSignup.dwReason=UNSIGNUP_REASON_SYSTEM;
					MatchUnSignup.dwUserID=pIServerUserItem->GetUserID();

					//����ṹ
					MatchUnSignup.lMatchFee=m_pMatchOption->lMatchFee;
					MatchUnSignup.dwInoutIndex=pIServerUserItem->GetInoutIndex();
					MatchUnSignup.dwClientAddr=pIServerUserItem->GetClientAddr();
					MatchUnSignup.dwMatchID=m_pMatchOption->dwMatchID;
					MatchUnSignup.dwMatchNo=pMatchSignupResult->dwMatchNO;
					lstrcpyn(MatchUnSignup.szMachineID,pIServerUserItem->GetMachineID(),CountArray(MatchUnSignup.szMachineID));

					//Ͷ������
					m_pIDataBaseEngine->PostDataBaseRequest(MatchUnSignup.dwUserID,DBR_GR_MATCH_UNSIGNUP,dwContextID,&MatchUnSignup,sizeof(MatchUnSignup));
				}

				return true;
			}

			//�������
			if (m_pCurMatchGroup->OnUserSignUpMatch(pIServerUserItem)==false)
			{
				SendRoomMessage(pIServerUserItem, TEXT("����ʧ��"),SMT_CHAT);

				return false;
			}

			//�����ɹ�
			m_OnMatchUserItem.Add(pIServerUserItem);
			SendMatchInfo(NULL);
			if(m_pIGameServiceFrame!=NULL)
			{
				SendRoomMessage(pIServerUserItem, TEXT("��ϲ�������ɹ�����ȴ�ϵͳΪ��������λ��"),SMT_CHAT);
			}

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
				return SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_GOLDUPDATE,&MatchGoldUpdate,sizeof(MatchGoldUpdate));
			}

			break;
		}
	case DBO_GR_MATCH_UNSIGNUP_RESULT:		//�������
		{
			//����Ч��
			if(pIServerUserItem==NULL) return true;
			if(wDataSize>sizeof(DBO_GR_MatchSingupResult)) return false;

			//��ȡ����
			DBO_GR_MatchSingupResult * pMatchSignupResult = (DBO_GR_MatchSingupResult*)pData;

			//ϵͳȡ��
			if (pMatchSignupResult->dwReason==UNSIGNUP_REASON_SYSTEM) return true;

			//�˷�ʧ��
			if (pMatchSignupResult->bResultCode==false)
			{
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,pMatchSignupResult->szDescribeString,SMT_EJECT);

				return true;
			}

			//��������
			LPCTSTR pszMatchFeeType[]={TEXT("��Ϸ��"),TEXT("Ԫ��")};
			if(m_pMatchOption->cbMatchFeeType<CountArray(pszMatchFeeType))
			{
				//�˷�֪ͨ
				TCHAR szDescribe[128]=TEXT("");
				_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�����ɹ����˻�������%I64d%s��"),m_pMatchOption->lMatchFee,pszMatchFeeType[m_pMatchOption->cbMatchFeeType]);
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,szDescribe,SMT_EJECT|SMT_CHAT);
			}

			//����״̬
			pIServerUserItem->SetSignUpStatus(MUS_NULL);

			//������Ϣ
			SendMatchInfo(NULL);

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
				return SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_GOLDUPDATE,&MatchGoldUpdate,sizeof(MatchGoldUpdate));
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
			IServerUserItem * pIRewardUserItem=NULL;
			tagMatchRankInfo * pMatchRankInfo=NULL;

			//���Ž���
			for (WORD wIndex=0;wIndex<pMatchRankList->wUserCount;wIndex++)
			{
				//�������
				pMatchRankInfo=&pMatchRankList->MatchRankInfo[wIndex];
				pIRewardUserItem=m_pIServerUserManager->SearchUserItem(pMatchRankInfo->dwUserID);
				if(pIRewardUserItem==NULL || pIRewardUserItem->IsAndroidUser()==true) continue;

				//д�뽱��
				WriteUserAward(pIRewardUserItem,pMatchRankInfo);
			}

			//��������
			CImmediateGroup * pImmediateGroup=NULL;

			//���ҷ���
			for (int i=0;i<m_MatchGroup.GetCount();i++)
			{
				if(m_MatchGroup[i]->m_dwMatchNO==pMatchRankList->dwMatchNO)
				{
					pImmediateGroup=m_MatchGroup[i];

					if(pImmediateGroup!=NULL)
					{
						//ȫ������
						pImmediateGroup->RearrangeUserSeat();

						//��ԭ���
						m_LoopTimer.Add(pImmediateGroup->m_LoopTimer);

						//�����������
						POSITION pos=pImmediateGroup->m_OnMatchUserMap.GetStartPosition();
						IServerUserItem *pITempUserItem=NULL;
						DWORD dwUserID=0;
						WORD wQuitUserCount=0;
						WORD wMatchUserCount=(WORD)pImmediateGroup->m_OnMatchUserMap.GetCount();
						while(pos!=NULL)
						{
							pImmediateGroup->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pITempUserItem);
							if (pITempUserItem!=NULL)
							{
								pITempUserItem->SetSignUpStatus(MUS_NULL);
								if (true==OnEventUserQuitMatch(pITempUserItem,0))
									wQuitUserCount++;

								//����������������δ���أ����߳�����
								bool bAndroidUser=pITempUserItem->IsAndroidUser();
								bool bTrusteeUser=pITempUserItem->IsTrusteeUser();
								bool bClientReady=pITempUserItem->IsClientReady();
								if (bAndroidUser==false && bTrusteeUser==true)
								{
									//�뿪����
									WORD wTableID=pITempUserItem->GetTableID();
									if (wTableID!=INVALID_TABLE)
									{
										//״̬�ж�
										ASSERT (pITempUserItem->GetUserStatus()!=US_PLAYING);
										SendGameMessage(pITempUserItem,TEXT(""),SMT_CLOSE_GAME);
										
										//�������Ӳ�����
										for(int m=0;m<pImmediateGroup->m_MatchTableArray.GetCount();m++)
										{
											if(pITempUserItem->GetTableID()==pImmediateGroup->m_MatchTableArray[m]->wTableID)
											{
												pImmediateGroup->m_MatchTableArray[m]->pTableFrame->PerformStandUpAction(pITempUserItem,true);
												break;
											}
										}
									}

									//�뿪����
									if (pITempUserItem->GetBindIndex()==INVALID_WORD)
									{
										pITempUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
									}
								}
							}
						}
						
						//У���������
						ASSERT(wQuitUserCount==wMatchUserCount);
						if (wQuitUserCount!=wMatchUserCount)
						{
							CTraceService::TraceString(TEXT("�������������������쳣"),TraceLevel_Exception);
						}

						//����ȫ������
						for (int j=0;pImmediateGroup->m_MatchTableArray.GetCount();)
						{
							tagTableFrameInfo* pTempTable=pImmediateGroup->m_MatchTableArray[j];
							pImmediateGroup->m_MatchTableArray.RemoveAt(j);
							SafeDelete(pTempTable);
						}
					}
					//�Ƴ�����
					m_MatchGroup.RemoveAt(i);
					m_OverMatchGroup.Add(pImmediateGroup);

					break;
				}
			}


			break;
		}
	case DBO_GR_MATCH_REWARD_RESULT:		//�������
		{
			//����Ч��			
			if(wDataSize>sizeof(DBO_GR_MatchRewardResult)) return false;

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
				SendGroupUserScore(pIRewardUserItem, NULL);

				//����ṹ
				CMD_GR_MatchGoldUpdate MatchGoldUpdate;

				//���ñ���
				MatchGoldUpdate.lCurrGold=pMatchRewardResult->lCurrGold;
				MatchGoldUpdate.lCurrIngot=pMatchRewardResult->lCurrIngot;
				MatchGoldUpdate.dwCurrExprience=pMatchRewardResult->dwCurrExperience;

				//��������
				SendData(pIRewardUserItem,MDM_GR_MATCH,SUB_GR_MATCH_GOLDUPDATE,&MatchGoldUpdate,sizeof(MatchGoldUpdate));
			}

			break;
		}
	}
	return true;
}

//������Ϣ
bool CImmediateMatch::OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
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
			MatchSignup.dwMatchNO=m_pCurMatchGroup->m_dwMatchNO;
			lstrcpyn(MatchSignup.szMachineID,pIServerUserItem->GetMachineID(),CountArray(MatchSignup.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(MatchSignup.dwUserID,DBR_GR_MATCH_SIGNUP,dwSocketID,&MatchSignup,sizeof(MatchSignup));

			return true;
		}
	case SUB_GR_LEAVE_MATCH:	//�˳�����
		{
			//����Ч��
			ASSERT(pIServerUserItem!=NULL);
			if(pIServerUserItem==NULL) return false;

			OnEventUserQuitMatch(pIServerUserItem,0,0,dwSocketID);

			return true;
		}
	}
	return true;
}

//�û���¼
bool CImmediateMatch::OnEventUserLogon(IServerUserItem * pIServerUserItem)
{
	return SendMatchInfo(pIServerUserItem);
}

//�û��ǳ�
bool CImmediateMatch::OnEventUserLogout(IServerUserItem * pIServerUserItem)
{
	return true;
}

//�����¼�
bool CImmediateMatch::OnEventEnterMatch(DWORD dwSocketID ,VOID* pData,DWORD dwUserIP, bool bIsMobile)
{
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
bool CImmediateMatch::OnEventUserJoinMatch(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem == NULL)
	{
		CTraceService::TraceString(TEXT("�û�ָ��Ϊ��!"),TraceLevel_Warning);
		return true;
	}

	if(CServerRule::IsForfendRoomEnter(m_pGameServiceOption->dwServerRule))
	{
		SendRoomMessage(pIServerUserItem, TEXT("��Ǹ������ϵͳά�����÷����ֹ�û�������������"),SMT_CHAT|SMT_EJECT);
		return true;
	}
	WORD wTableCount=GetNullTable();
	if(wTableCount<m_pMatchOption->MatchType.Immediate.wStartUserCount/CImmediateGroup::m_wChairCount)
	{
		CString str;
		str.Format( TEXT("��Ǹ����ǰ���Ӳ�����ȴ������� %d �����ӿ��Ա���"),m_pMatchOption->MatchType.Immediate.wStartUserCount/CImmediateGroup::m_wChairCount-wTableCount);
		SendRoomMessage(pIServerUserItem,str,SMT_CHAT);
		return true;
	}
	if(m_pCurMatchGroup==NULL || m_pCurMatchGroup->m_enMatchStatus==MatchStatus_Wait)
	{
		SendRoomMessage(pIServerUserItem, TEXT("��Ǹ���÷�������������������Ժ��ٱ������߽�����һ���������"),SMT_CHAT);
		return true;
	}

	INT_PTR dwUserCount=m_OnMatchUserItem.GetCount();
	for (INT_PTR i=0;i<dwUserCount;i++)
	{
		if(m_OnMatchUserItem[i]->GetUserID()==pIServerUserItem->GetUserID())
		{
			SendRoomMessage(pIServerUserItem, TEXT("���Ѿ��ɹ������������ظ�������"),SMT_CHAT|SMT_EJECT);
			return true;
		}
	}

	if(m_LoopTimer.GetCount()==0)
	{
		SendRoomMessage(pIServerUserItem, TEXT("��Ǹ��������Ų������Եȡ�"),SMT_CHAT);
		return true;
	}

	if(pIServerUserItem->GetUserStatus()!=US_FREE || pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		SendRoomMessage(pIServerUserItem, TEXT("�Բ�������ǰ��״̬������μӱ�����"),SMT_CHAT);
		return true;
	}

	if (m_pMatchOption->MatchType.Immediate.dwInitalScore==0&&pIServerUserItem->GetUserScore()<m_pMatchOption->MatchType.Immediate.dwInitalBase*m_pMatchOption->MatchType.Immediate.wMinEnterGold+m_pMatchOption->lMatchFee)
	{
		TCHAR szMsg[256]=TEXT("");
		_sntprintf(szMsg,CountArray(szMsg),TEXT("�Բ�������ǰ�Ľ�ҵ�����%d�����ܲμӱ�����"),
			m_pMatchOption->MatchType.Immediate.dwInitalBase*m_pMatchOption->MatchType.Immediate.wMinEnterGold+m_pMatchOption->lMatchFee);
		SendRoomMessage(pIServerUserItem, szMsg,SMT_CHAT|SMT_EJECT);
		return true;
	}

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
		if(pIServerUserItem->IsAndroidUser()==true && (m_pCurMatchGroup->m_wAndroidUserCount >= 
		   m_pMatchOption->MatchType.Immediate.wAndroidUserCount))
		{
			return true;
		}

		return OnEventSocketMatch(SUB_GR_MATCH_FEE,&m_pMatchOption->lMatchFee,sizeof(SCORE),pIServerUserItem,dwSocketID);
	}

	//�����ɹ�
	m_OnMatchUserItem.Add(pIServerUserItem);
	SendMatchInfo(NULL);
	if(m_pIGameServiceFrame!=NULL)
	{
		SCORE lScore=SCORE(m_pMatchOption->lMatchFee)*-1;
		/*if(m_pMatchOption->MatchType.Immediate.dwInitalScore==0)
		{
			pIServerUserItem->GetUserInfo()->lScore+=lScore;
			m_pIServerUserItemSink->OnEventUserItemScore(pIServerUserItem,SCORE_REASON_MATCH_FEE);
		}*/
		SendRoomMessage(pIServerUserItem, TEXT("��ϲ�������ɹ�����ȴ�ϵͳΪ��������λ��"),SMT_CHAT);
	}

	return true;
}

//�û�����
bool CImmediateMatch::OnEventUserQuitMatch(IServerUserItem * pIServerUserItem, BYTE cbReason, WORD *pBestRank, DWORD dwSocketID)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//������Ϸ״̬
	if (pIServerUserItem->GetUserStatus()>=US_PLAYING)
	{
		return true;
	}

	//���ڱ����׶�
	BYTE cbUserEnlist=pIServerUserItem->GetSignUpStatus();
	if (cbUserEnlist==MUS_MATCHING)
	{
		return true;
	}

	//�����������
	if(RemoveMatchUserItem(pIServerUserItem))
	{
		//�����ڱ���������ɾ�������
		INT_PTR nGroupCount=m_MatchGroup.GetCount();
		for (INT_PTR i=0;i<nGroupCount;i++)
		{
			CImmediateGroup *pMatch=m_MatchGroup[i];
			if(pMatch->OnUserQuitMatch(pIServerUserItem))
			{
				//����״̬
				pIServerUserItem->SetSignUpStatus(MUS_NULL);

				if (pMatch->m_enMatchStatus!=MatchStatus_Null)
				{
					SendRoomMessage(pIServerUserItem, TEXT("�����ɹ�����ӭ�������μ�����������"), SMT_CHAT|SMT_EJECT);
				}
				return true;
			}
		}

		//�����ڱ���������ɾ�������
		if(m_pCurMatchGroup->OnUserQuitMatch(pIServerUserItem))
		{
			//�˻�������
			if(m_pMatchOption->lMatchFee>0 && !pIServerUserItem->IsAndroidUser())
			{
				//��������
				DBR_GR_MatchUnSignup MatchUnSignup;
				MatchUnSignup.dwReason=UNSIGNUP_REASON_PLAYER;
				MatchUnSignup.dwUserID=pIServerUserItem->GetUserID();

				//����ṹ
				MatchUnSignup.lMatchFee=m_pMatchOption->lMatchFee;
				MatchUnSignup.dwInoutIndex=pIServerUserItem->GetInoutIndex();
				MatchUnSignup.dwClientAddr=pIServerUserItem->GetClientAddr();
				MatchUnSignup.dwMatchID=m_pMatchOption->dwMatchID;
				MatchUnSignup.dwMatchNo=m_pCurMatchGroup->m_dwMatchNO;
				lstrcpyn(MatchUnSignup.szMachineID,pIServerUserItem->GetMachineID(),CountArray(MatchUnSignup.szMachineID));

				//Ͷ������
				m_pIDataBaseEngine->PostDataBaseRequest(MatchUnSignup.dwUserID,DBR_GR_MATCH_UNSIGNUP,dwSocketID,&MatchUnSignup,sizeof(MatchUnSignup));

				return true;
			}

			//����״̬
			pIServerUserItem->SetSignUpStatus(MUS_NULL);

			if (m_pCurMatchGroup->m_enMatchStatus!=MatchStatus_Null)
			{
				SendRoomMessage(pIServerUserItem, TEXT("�����ɹ�����ӭ�������μ�����������"), SMT_CHAT|SMT_EJECT);
			}

			SendMatchInfo(NULL);
			return true;
		}
		SendMatchInfo(NULL);
	}

	return false;
}

//�û�״̬
bool CImmediateMatch::OnEventMatchUserStatus(IServerUserItem * pIServerUserItem, BYTE cbOldUserStatus, BYTE cbCurrUserStatus)
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

//�û�����
bool CImmediateMatch::OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��ȡ����
	CImmediateGroup * pImmediateGroup = static_cast<CImmediateGroup *>(pIServerUserItem->GetMatchData());
	if(pImmediateGroup==NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemScore(pIServerUserItem,cbReason);
	}

	//���ͷ���
	SendGroupUserScore(pIServerUserItem,pImmediateGroup);

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
		WriteGameScore.dwMatchNO=pImmediateGroup->m_dwMatchNO;

		//Ͷ������
		printf("OnEventUserItemScore �û�����\n");
		m_pIDataBaseEngine->PostDataBaseRequest(WriteGameScore.dwUserID,DBR_GR_WRITE_GAME_SCORE,0L,&WriteGameScore,sizeof(WriteGameScore), TRUE);
	}

	return true;
}

//�û�״̬
bool CImmediateMatch::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID)
{
	//�������
	if(pIServerUserItem->GetUserStatus()==US_NULL) pIServerUserItem->SetMatchData(NULL);

	//�ص��ӿ�
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemStatus(pIServerUserItem,wOldTableID,wOldChairID);
	}

	return true;
}

//�û�Ȩ��
bool CImmediateMatch::OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight, BYTE cbRightKind)
{
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemRight(pIServerUserItem,dwAddRight,dwRemoveRight,cbRightKind);
	}

	return true;
}

//�û���Ϸ����
bool CImmediateMatch::OnEventUserItemGameData(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemGameData(pIServerUserItem,cbReason);
	}

	return true;
}

//������������ʼ����
bool CImmediateMatch::OnEventMatchStart(CImmediateGroup *pMatch)
{
	ASSERT(pMatch==m_pCurMatchGroup&&m_LoopTimer.GetCount()>0);
	
	if (pMatch!=NULL)
	{
		//��������
		DBR_GR_MatchStart MatchStart;

		//����ṹ
		MatchStart.dwMatchID=m_pMatchOption->dwMatchID;
		MatchStart.dwMatchNO=pMatch->m_dwMatchNO;
		MatchStart.cbMatchType=m_pMatchOption->cbMatchType;
		MatchStart.wServerID=m_pGameServiceOption->wServerID;

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(MatchStart.dwMatchNO,DBR_GR_MATCH_START,0,&MatchStart,sizeof(MatchStart));
	
		//��Ų��� ���ܱ�����
		pMatch->m_LoopTimer=m_LoopTimer[0];
		m_LoopTimer.RemoveAt(0);
		m_MatchGroup.Add(pMatch);

		//׼���µ�һ�����
		m_pCurMatchGroup=new CImmediateGroup(CalcMatchNo());
		if (m_pCurMatchGroup!=NULL)
		{
			tagImmediateMatchParameter ImmediateMatchParameter;
			ImmediateMatchParameter.pGameMatchOption=m_pMatchOption;
			ImmediateMatchParameter.pGameServiceAttrib=m_pGameServiceAttrib;
			ImmediateMatchParameter.pGameServiceOption=m_pGameServiceOption;
			ImmediateMatchParameter.pIImmediateGroupSink=this;
			m_pCurMatchGroup->SetImmediateGroup(ImmediateMatchParameter);
		}

		SendMatchInfo(NULL);
		SetUserMatchStatus(pMatch,MUS_MATCHING);
	}

	return true;
}


//��������
bool CImmediateMatch::OnEventMatchOver(CImmediateGroup *pMatch)
{
	//��������
	if(pMatch!=NULL)
	{
		DBR_GR_MatchOver MatchOver;
		ZeroMemory(&MatchOver,sizeof(MatchOver));

		//��������						
		MatchOver.dwMatchID=m_pMatchOption->dwMatchID;
		MatchOver.dwMatchNO=pMatch->m_dwMatchNO;		
		MatchOver.cbMatchType=MATCH_TYPE_IMMEDIATE;
		MatchOver.wServerID=m_pGameServiceOption->wServerID;
		
		//��ȡʱ��
		MatchOver.MatchStartTime=pMatch->m_MatchStartTime;
		CTime::GetCurrentTime().GetAsSystemTime(MatchOver.MatchEndTime);

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(MatchOver.dwMatchNO,DBR_GR_MATCH_OVER,0,&MatchOver,sizeof(MatchOver));
	}

	return true;
}

//�رն�ʱ��
bool CImmediateMatch::KillGameTimer(DWORD dwTimerID,CImmediateGroup *pMatch)
{
	m_pITimerEngine->KillTimer(dwTimerID);

	return false;
}

//���ö�ʱ��
bool CImmediateMatch::SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CImmediateGroup *pMatch)
{
	return m_pITimerEngine->SetTimer(dwTimerID,dwElapse,dwRepeat,dwBindParameter);
}


//��������
bool CImmediateMatch::SendMatchInfo(IServerUserItem * pIServerUserItem)
{
	CMD_GR_Match_Num MatchNum;
	MatchNum.dwWaitting=(DWORD)m_pCurMatchGroup->m_OnMatchUserMap.GetCount();
	MatchNum.dwTotal=m_pMatchOption->MatchType.Immediate.wStartUserCount;
	if(pIServerUserItem==NULL)
	{
		m_pIGameServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_MATCH,SUB_GR_MATCH_NUM,&MatchNum, sizeof(MatchNum));
		return true;
	}

	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_NUM,&MatchNum, sizeof(MatchNum));
	INT_PTR dwUserCount=m_OnMatchUserItem.GetCount();
	for (INT_PTR i=0;i<dwUserCount;i++)
	{
		if(m_OnMatchUserItem[i]==pIServerUserItem)
		{
			BYTE cbUserMatchStatus=pIServerUserItem->GetSignUpStatus();
			pIServerUserItem->SetSignUpStatus(cbUserMatchStatus);
			return true;
		}
	}

	tagUserInfo *pUserScore=pIServerUserItem->GetUserInfo();
	pUserScore->dwWinCount=pUserScore->dwLostCount=pUserScore->dwFleeCount=pUserScore->dwDrawCount=pUserScore->cbGender=0;
	if((SCORE)m_pMatchOption->MatchType.Immediate.dwInitalScore!=0)
	{
		pUserScore->lScore=(LONG)m_pMatchOption->MatchType.Immediate.dwInitalScore;
	}

	return SendGroupUserScore(pIServerUserItem, NULL);
}


//������Ϣ
bool CImmediateMatch::SendGroupUserMessage(LPCTSTR pStrMessage,CImmediateGroup *pMatch)
{
	POSITION pos=pMatch->m_OnMatchUserMap.GetStartPosition();
	IServerUserItem *pUserItem=NULL;
	DWORD dwUserID=0;
	while(pos!=NULL)
	{
		pMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
		SendGameMessage(pUserItem,pStrMessage, SMT_CHAT|SMT_TABLE_ROLL);
	}
	return true;
}


//�����û�����
bool CImmediateMatch::SendGroupUserScore(IServerUserItem * pIServerUserItem,CImmediateGroup *pMatch)
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
	UserScore.UserScore.lIngot=pUserInfo->lIngot;
	UserScore.UserScore.lGrade=pUserInfo->lGrade;
	UserScore.UserScore.lInsure=pUserInfo->lInsure;	
	UserScore.UserScore.lScore=pUserInfo->lScore;
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

	if(pMatch!=NULL)
	{
		POSITION pos=pMatch->m_OnMatchUserMap.GetStartPosition();
		IServerUserItem *pUserItem=NULL;
		DWORD dwUserID=0;
		while(pos!=NULL)
		{
			pMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
			if (pUserItem!=NULL)
			{
				if (pUserItem->IsMobileUser()==true)
					m_pIGameServiceFrame->SendData(pUserItem,MDM_GR_USER,SUB_GR_USER_SCORE,&MobileUserScore,sizeof(MobileUserScore));
				else
					m_pIGameServiceFrame->SendData(pUserItem,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));
			}
		}
		return true;
	}

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


//����״̬
bool  CImmediateMatch::SendGroupUserStatus(IServerUserItem * pIServerUserItem,CImmediateGroup *pMatch)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->IsClientReady()==true);
	if (pIServerUserItem->IsClientReady()==false) return false;

	//��������
	CMD_GR_UserStatus UserStatus;
	ZeroMemory(&UserStatus,sizeof(UserStatus));

	tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

	UserStatus.dwUserIP = pIServerUserItem->GetClientAddr();

	//��������
	UserStatus.dwUserID=pUserData->dwUserID;
	UserStatus.UserStatus.wTableID=pUserData->wTableID;
	UserStatus.UserStatus.wChairID=pUserData->wChairID;
	UserStatus.UserStatus.cbUserStatus=pUserData->cbUserStatus;
	UserStatus.dwUserScore = pIServerUserItem->GetUserScore();
	POSITION pos=pMatch->m_OnMatchUserMap.GetStartPosition();
	IServerUserItem *pUserItem=NULL;
	DWORD dwUserID=0;
	while(pos!=NULL)
	{
		pMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
		if(pUserItem->IsClientReady())
			m_pIGameServiceFrame->SendData(pUserItem,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));	
	}
	return true;
}


//������Ϸ��Ϣ
bool CImmediateMatch::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)
{
	ASSERT(pIServerUserItem!=-NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	if ((pIServerUserItem->GetBindIndex()!=INVALID_WORD)&&(pIServerUserItem->IsClientReady()==true)&&(pIServerUserItem->IsAndroidUser()==false))
	{
		//�������ݰ�
		CMD_CM_SystemMessage Message;
		Message.wType=wMessageType;
		lstrcpyn(Message.szString,lpszMessage,CountArray(Message.szString));
		Message.wLength=CountStringBuffer(Message.szString);

		//��������
		WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
		return m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&Message,wSendSize);
	}

	return true;
}


//������Ϣ
bool CImmediateMatch::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)
{
	ASSERT(pIServerUserItem!=-NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD && false==pIServerUserItem->IsAndroidUser())
	{
		//�������ݰ�
		CMD_CM_SystemMessage Message;
		Message.wType=wMessageType;

		lstrcpyn(Message.szString,lpszMessage,CountArray(Message.szString));
		Message.wLength=CountStringBuffer(Message.szString);

		//��������
		WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
		return m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&Message,wSendSize);
	}

	return true;
}

//�������ݵ�һ���û�
bool CImmediateMatch::SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CImmediateGroup *pMatch)
{
	POSITION pos=pMatch->m_OnMatchUserMap.GetStartPosition();
	IServerUserItem *pUserItem=NULL;
	DWORD dwUserID=0;
	while(pos!=NULL)
	{
		pMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
		m_pIGameServiceFrame->SendData(pUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);	
	}
	return true;
}

//��������
bool CImmediateMatch::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if(pIServerUserItem!=NULL)
		return m_pIGameServiceFrame->SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
	else
		return m_pIGameServiceFrame->SendData(BG_ALL_CLIENT, wMainCmdID,wSubCmdID, pData, wDataSize);
    
	return true;
}

//Ϊ���ֲ��������
void CImmediateMatch::InsertNullTable()
{
	for (int i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		//�������˳�
		WORD nCurTableCount=(WORD)m_pCurMatchGroup->m_MatchTableArray.GetCount();
		if(nCurTableCount>=m_pMatchOption->MatchType.Immediate.wStartUserCount/m_pGameServiceAttrib->wChairCount)break;

		BOOL bIsInsert=true;
		for (int j=0;j<m_MatchGroup.GetCount();j++)
		{
			WORD wMatchGroupTableCount=(WORD)m_MatchGroup[j]->m_MatchTableArray.GetCount();
			for (int k=0;k<wMatchGroupTableCount;k++)
			{
				WORD wTableID=m_MatchGroup[j]->m_MatchTableArray[k]->wTableID;
				if(i==wTableID){bIsInsert=false;break;}//�������Ѿ������˸���
			}
		}
		//��������
		if( bIsInsert&&m_ppITableFrame[i]->GetNullChairCount()==m_pGameServiceAttrib->wChairCount)
		{
			tagTableFrameInfo * pTableFrameInfo=new tagTableFrameInfo;
			ZeroMemory(pTableFrameInfo,sizeof(tagTableFrameInfo));			
			pTableFrameInfo->wTableID=i;
			pTableFrameInfo->pTableFrame=m_ppITableFrame[i];
			pTableFrameInfo->dwBaseScore=m_pMatchOption->MatchType.Immediate.dwInitalBase;
			ITableFrameHook * pFrameHook=QUERY_OBJECT_PTR_INTERFACE(m_ppITableFrame[i]->GetTableFrameHook(),ITableFrameHook);
			pFrameHook->SetMatchEventSink(QUERY_OBJECT_PTR_INTERFACE(m_pCurMatchGroup,IUnknownEx));
			m_pCurMatchGroup->AddMatchTable(pTableFrameInfo);
		}
	}
}

//��ȡһ�����еĻ�����
IAndroidUserItem * CImmediateMatch::GetFreeAndroidUserItem()
{

	if(m_pAndroidUserManager->GetAndroidCount()==0) return NULL;

	//����״̬
	tagAndroidUserInfo AndroidSimulate;
	m_pAndroidUserManager->GetAndroidUserInfo(AndroidSimulate,ANDROID_SIMULATE|ANDROID_PASSIVITY|ANDROID_INITIATIVE);

	if(AndroidSimulate.wFreeUserCount==0) 
		return NULL;
	else
	{
		//�����ѱ����Ļ������ظ�����
		for(WORD i=0; i<AndroidSimulate.wFreeUserCount;i++)
		{
			if(AndroidSimulate.pIAndroidUserFree[i]->GetMeUserItem()->GetSignUpStatus()==MUS_NULL)
			{
				ASSERT(AndroidSimulate.pIAndroidUserFree[i]->GetUserID()==AndroidSimulate.pIAndroidUserFree[i]->GetMeUserItem()->GetUserID());
				return AndroidSimulate.pIAndroidUserFree[i];
			}
		}
	}

	return NULL;
}

//д�뽱��
bool CImmediateMatch::WriteUserAward(IServerUserItem *pIServerUserItem,tagMatchRankInfo * pMatchRankInfo)
{
	//����Ч��
	ASSERT(pIServerUserItem!=NULL && pMatchRankInfo!=NULL);
	if(pIServerUserItem==NULL || pMatchRankInfo==NULL) return false;

	//д���ж�
	bool bWriteReward=(pMatchRankInfo->lRewardGold+pMatchRankInfo->dwRewardIngot+pMatchRankInfo->dwRewardExperience)>0;

	//д�뽱��
	if(bWriteReward==true)
	{
		//��������
		CMD_GR_MatchResult MatchResult;
		ZeroMemory(&MatchResult,sizeof(MatchResult));

		//��������
		MatchResult.lGold+=pMatchRankInfo->lRewardGold;
		MatchResult.dwIngot+=pMatchRankInfo->dwRewardIngot;
		MatchResult.dwExperience+=pMatchRankInfo->dwRewardExperience;

		//����ʾ
		_sntprintf(MatchResult.szDescribe,CountArray(MatchResult.szDescribe),TEXT("%s, ��ϲ����%s�л�õ�%d��,�������£�"),
			pIServerUserItem->GetNickName(),m_pMatchOption->szMatchName,pMatchRankInfo->wRankID);

		//��������
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			m_pIGameServiceFrame->SendData(pIServerUserItem, MDM_GF_FRAME, SUB_GR_MATCH_RESULT, &MatchResult, sizeof(MatchResult));
		}
	}
	else
	{
		TCHAR szMessage[256]=TEXT("");
		_sntprintf(szMessage,CountArray(szMessage),TEXT("�����ѽ�������ϲ����õ�%d������ӭ���μ��������εı�����"),pMatchRankInfo->wRankID);
		SendGameMessage(pIServerUserItem, szMessage, SMT_CLOSE_GAME);
		SendRoomMessage(pIServerUserItem, szMessage, SMT_CHAT|SMT_EJECT);

		return true;
	}

	//д���¼
	if(m_pIDataBaseEngine!=NULL)
	{
		//��������
		DBR_GR_MatchReward MatchReward;
		ZeroMemory(&MatchReward,sizeof(MatchReward));
		
		//����ṹ
		MatchReward.dwUserID=pIServerUserItem->GetUserID();	
		MatchReward.lRewardGold=pMatchRankInfo->lRewardGold;
		MatchReward.dwRewardIngot=pMatchRankInfo->dwRewardIngot;
		MatchReward.dwRewardExperience=pMatchRankInfo->dwRewardExperience;
		MatchReward.dwClientAddr=pIServerUserItem->GetClientAddr();

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(MatchReward.dwUserID,DBR_GR_MATCH_REWARD,0,&MatchReward,sizeof(MatchReward));
	}

	return true;
}

//��������״̬
VOID CImmediateMatch::SetUserMatchStatus(CImmediateGroup *pMatch, BYTE cbUserMatchStatus)
{
	POSITION pos=pMatch->m_OnMatchUserMap.GetStartPosition();
	IServerUserItem *pUserItem=NULL;
	DWORD dwUserID=0;
	while(pos!=NULL)
	{
		pMatch->m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
		ASSERT(pUserItem!=NULL);
		if (pUserItem==NULL) break;

		pUserItem->SetSignUpStatus(cbUserMatchStatus);
	}
	return;
}

//���㳡��
DWORD CImmediateMatch::CalcMatchNo()
{
	//��ȡʱ��
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime); 

	//ʱ�����
	WORD wDay=SystemTime.wDay;	
	WORD wMonth=SystemTime.wMonth;
	WORD wYear=SystemTime.wYear-1980;	

	//��¼����
	static DWORD wMatchCount=0;
	static DWORD wLastDay=SystemTime.wDay;	

	//���¼�¼
	if (SystemTime.wDay>wLastDay)
	{
		wMatchCount=1;
		wLastDay=SystemTime.wDay;
	}
	else
	{
		wMatchCount++;
	}

	return (DWORD(wYear)<<22|DWORD(wMonth)<<18|DWORD(wDay)<<14|DWORD(wMatchCount));
}

//��ȡ������
WORD CImmediateMatch::GetNullTable()
{
	WORD wTableCount=0;
	for (int i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		BOOL bIsInsert=true;
		for (int j=0;j<m_MatchGroup.GetCount();j++)
		{
			WORD wMatchGroupTableCount=(WORD)m_MatchGroup[j]->m_MatchTableArray.GetCount();
			for (int k=0;k<wMatchGroupTableCount;k++)
			{
				WORD wTableID=m_MatchGroup[j]->m_MatchTableArray[k]->wTableID;
				if(i==wTableID){bIsInsert=false;break;}//�������Ѿ������˸���
			}
		}
		//��������
		if( bIsInsert&&m_ppITableFrame[i]->GetNullChairCount()==m_pGameServiceAttrib->wChairCount)
		{
			wTableCount++;
		}
	}
	return wTableCount;
}

//ɾ���û�
bool CImmediateMatch::DeleteUserItem(DWORD dwUserIndex)
{
	try
	{
		if (m_pITCPNetworkEngineEvent == NULL)
		{
			throw 0;
		}
		m_pITCPNetworkEngineEvent->OnEventTCPNetworkShut(dwUserIndex,0,0L);
	}
	catch (...)
	{
		//�������
		ASSERT(FALSE);
		return false;
	}
	return true;
}

//�Ƴ������û�
bool CImmediateMatch::RemoveMatchUserItem(IServerUserItem *pIServerUserItem)
{
	for (INT_PTR i=0;i<m_OnMatchUserItem.GetCount();i++)
	{
		//��ȡ�û�
		IServerUserItem * pOnLineUserItem=m_OnMatchUserItem[i];
		
		//�û��ж�
		BYTE cbSignupStatus=pOnLineUserItem->GetSignUpStatus();
		if (pOnLineUserItem==pIServerUserItem && cbSignupStatus!=MUS_MATCHING)
		{
			ASSERT(pOnLineUserItem->GetUserID()==pIServerUserItem->GetUserID());

			m_OnMatchUserItem.RemoveAt(i);
			return true;
		}
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
