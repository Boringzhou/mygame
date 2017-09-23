#include "StdAfx.h"
#include "ImmediateGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//��������
#define INVALID_VALUE				0xFFFF								//��Чֵ

//��̬����
WORD CImmediateGroup::m_wChairCount;									//������Ŀ

//////////////////////////////////////////////////////////////////////////
//���캯��
CImmediateGroup::CImmediateGroup(DWORD dwMatchNO)
{
	//ָ�����
	m_pGameServiceOption=NULL;
	m_pMatchOption=NULL;
	m_pMatchSink=NULL;

	//״̬����
	m_dwMatchNO=dwMatchNO;
	m_enMatchStatus=MatchStatus_Signup;
	m_dwCurBase=0;

	//���ñ���
	ZeroMemory(&m_MatchStartTime,sizeof(m_MatchStartTime));

	//�Ƴ�Ԫ��
	m_OnMatchUserMap.RemoveAll();
	m_FreeUserArray.RemoveAll();
	m_MatchTableArray.RemoveAll();
    
	//���ñ���
	m_LoopTimer=0;
	m_wAndroidUserCount=0;	
}

//��������
CImmediateGroup::~CImmediateGroup()
{
	//ɱ����ʱ��
	AllKillMatchTimer();

	//����״̬
	m_wAndroidUserCount=0;
	m_enMatchStatus=MatchStatus_Null;

	//�ͷ�����
	for (int i=0; i<m_MatchTableArray.GetCount(); i++)
	{
		SafeDelete(m_MatchTableArray[i]);
	}

	//�Ƴ�Ԫ��
	m_MatchTableArray.RemoveAll();
	m_OnMatchUserMap.RemoveAll();
	m_FreeUserArray.RemoveAll();
}

//�ӿڲ�ѯ
VOID* CImmediateGroup::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IMatchEventSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IMatchEventSink,Guid,dwQueryVer);
	return NULL;
}

//���ñ���
void CImmediateGroup::SetImmediateGroup(tagImmediateMatchParameter & ImmediateMatchParameter)
{
	//ָ�����
	m_pGameServiceAttrib=ImmediateMatchParameter.pGameServiceAttrib;
	m_pGameServiceOption=ImmediateMatchParameter.pGameServiceOption;
	m_pMatchOption=ImmediateMatchParameter.pGameMatchOption;
	m_pMatchSink=ImmediateMatchParameter.pIImmediateGroupSink;

	m_dwCurBase=m_pMatchOption->MatchType.Immediate.dwInitalBase;
}

//�û�����
bool CImmediateGroup::OnUserSignUpMatch(IServerUserItem * pUserItem)
{
	//�û��ж�
	ASSERT(pUserItem!=NULL);
	if (pUserItem==NULL) return false;

	//�����û�
	IServerUserItem * pIServerUserExist=NULL;
	m_OnMatchUserMap.Lookup(pUserItem->GetUserID(),pIServerUserExist);

	//�û��ж�
	ASSERT(pIServerUserExist==NULL);
	if (pIServerUserExist!=NULL) return false;

	//����Ч��
	ASSERT(m_OnMatchUserMap.GetCount()<m_pMatchOption->MatchType.Immediate.wStartUserCount);
	if (m_OnMatchUserMap.GetCount()>=m_pMatchOption->MatchType.Immediate.wStartUserCount) return false;

	//�����ж�
	if (pUserItem->IsAndroidUser()==true)
	{
		if (m_wAndroidUserCount>=m_pMatchOption->MatchType.Immediate.wAndroidUserCount)
		{
			return false;
		}
		m_wAndroidUserCount++;
	}

	//���óɼ�
	tagUserInfo *pUserScore=pUserItem->GetUserInfo();
	pUserScore->dwWinCount=0L;
	pUserScore->dwLostCount=0L;
	pUserScore->dwFleeCount=0L;
	pUserScore->dwDrawCount=0L;
	pUserScore->lGrade=0L;
	pUserScore->lScore=(SCORE)m_pMatchOption->MatchType.Immediate.dwInitalScore;

	//���ͳɼ�
	m_pMatchSink->SendGroupUserScore(pUserItem,NULL);

	//�����û�
	m_OnMatchUserMap[pUserItem->GetUserID()]=pUserItem;

	//���ñ���״̬
	pUserItem->SetSignUpStatus(MUS_SIGNUP);

	//���ñ���ʱ��
	pUserItem->SetSignUpTime(GetTickCount());

	//���ͱ���״̬
	BYTE cbMatchStatus=MS_SIGNUP;
	m_pMatchSink->SendData(pUserItem, MDM_GR_MATCH, SUB_GR_MATCH_STATUS, &cbMatchStatus, sizeof(cbMatchStatus));

	//��������
	if (m_OnMatchUserMap.GetCount()>=m_pMatchOption->MatchType.Immediate.wStartUserCount)
	{
		//�¼�֪ͨ
		ASSERT(m_pMatchSink!=NULL);
		if (m_pMatchSink!=NULL) m_pMatchSink->SendMatchInfo(NULL);
		EfficacyStartMatch();
	}

	return true;
}
//������ʼ
void CImmediateGroup::EfficacyStartMatch()
{
	m_pMatchSink->InsertNullTable();

	//û�����ӵ����
	if( m_MatchTableArray.GetCount()<m_pMatchOption->MatchType.Immediate.wStartUserCount/m_wChairCount )
	{
		if(m_OnMatchUserMap.GetCount()<m_wChairCount)
		{
			//�������ݰ�
			CMD_CM_SystemMessage Message;
			Message.wType=SMT_EJECT|SMT_CHAT|SMT_CLOSE_GAME;
			lstrcpyn(Message.szString,TEXT("��Ǹ������ĳ�û�ǿ���˳��������ó�������ǰ����������Լ����ȴ�������"),CountArray(Message.szString));
			Message.wLength=CountStringBuffer(Message.szString);
			//��������
			WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
			m_pMatchSink->SendGroupData(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&Message,wSendSize, this);
			m_enMatchStatus=MatchStatus_Signup;
			return ;
		}

		m_enMatchStatus=MatchStatus_Wait;
		//3���������øú���ֱ��������
		SetMatchGameTimer(IDI_WAITTABLE_FIRST_ROUND,3000,1,NULL);

		//�������ݰ�
		CMD_CM_SystemMessage Message;
		Message.wType=SMT_CHAT;
		_sntprintf(Message.szString,CountArray(Message.szString),
			TEXT("��Ǹ�����ڹ����û������������������� %d �����ӿ���,��ȴ�������"),m_pMatchOption->MatchType.Immediate.wStartUserCount/m_wChairCount-m_MatchTableArray.GetCount());
		Message.wLength=CountStringBuffer(Message.szString);

		//��������
		WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
		m_pMatchSink->SendGroupData(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&Message,wSendSize, this);
		return ;
	}

	//Ԥ��������λ
	DWORD dwUserID=0L;
	IServerUserItem *pIServerUserItem=NULL;
	POSITION pos=m_OnMatchUserMap.GetStartPosition();

	WORD wChairID=0;
	while (pos!=NULL)
	{
		//��ȡ�û�
		m_OnMatchUserMap.GetNextAssoc(pos,dwUserID,pIServerUserItem);
		ASSERT(pIServerUserItem!=NULL);

		InsertFreeGroup(pIServerUserItem);

		//�����û�������
		pIServerUserItem->SetSignUpStatus(MUS_MATCHING);
		pIServerUserItem->SetMatchData(this);
	}

	//�����Զ�����
	TabelRequestUser(false);

	//����״̬	
	m_enMatchStatus=MatchStatus_FirstRound;

	//��¼ʱ��
	CTime::GetCurrentTime().GetAsSystemTime(m_MatchStartTime);
	
	//��ʼ֪ͨ
	if (m_pMatchSink!=NULL) m_pMatchSink->OnEventMatchStart(this);
		
	//���ö�ʱ��
	SetMatchGameTimer(IDI_CHECK_MATCH_GROUP,1000,TIMES_INFINITY,0);
}

//�û�����
BOOL CImmediateGroup::OnUserQuitMatch(IServerUserItem *pUserItem)
{
	//�û��ж�
	ASSERT(pUserItem!=NULL);
	if (pUserItem==NULL) return false;

	//ɾ���û�
	BOOL bSuccess=m_OnMatchUserMap.RemoveKey(pUserItem->GetUserID());
	if(bSuccess==false)return false;

	if (pUserItem->IsAndroidUser())
	{
		ASSERT(m_wAndroidUserCount>0);
		if (m_wAndroidUserCount>0) m_wAndroidUserCount--;
	}

	return bSuccess;
}

//��ʱ��
bool CImmediateGroup::OnTimeMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//״̬У��
	if(m_enMatchStatus==MatchStatus_Null) return true;

	switch(dwTimerID)
	{
	case IDI_CHECK_MATCH_GROUP:
		{
			CaptureMatchTimer();

			return true;
		}
	case IDI_WAITTABLE_FIRST_ROUND:
		{
			if(m_enMatchStatus==MatchStatus_Wait)
			{
				EfficacyStartMatch();
			}
			return true;
		}
	case IDI_SWITCH_WAIT_TIME:
		{
			//�������û��Զ�����
			RearrangeUserSeat();
			TabelRequestUser();
			//ContinueGame((ITableFrame*)dwBindParameter);
			return true;
		}
	case IDI_TABLE_REQUEST_USER:
		{
			TabelRequestUser();
			return true;
		}
	case IDI_ENTER_NEXT_ROUND:
		{
			//if(m_enMatchStatus==MS_START_NEXT_ROUND)
			//{
			//	//������һ��
			//	ContinueRound();
			//}
			return true;
		}
	case IDI_ALL_DISMISS_GAME:
		{
			RearrangeUserSeat();
			return true;
		}
	case IDI_LASTMATCHRESULT:
		{
			//������
			MatchOver();

			return true;
		}
	}

	return true;
}

WORD CImmediateGroup::SortMapUser(tagMatchScore score[])
{
	INT_PTR nCount=m_OnMatchUserMap.GetCount();
	POSITION pos=m_OnMatchUserMap.GetStartPosition();
	nCount=0;
	while(pos!=NULL)
	{
		m_OnMatchUserMap.GetNextAssoc(pos,score[nCount].dwUserID, score[nCount].pUserItem);
		score[nCount].dwUserID=score[nCount].pUserItem->GetUserID();
		score[nCount].lScore=score[nCount].pUserItem->GetUserScore();
		nCount++;
	}
	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  Temp = score[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(score[i].lScore > score[mid].lScore)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}

	return (WORD)nCount;
}

//������λ
void CImmediateGroup::RearrangeUserSeat(bool bDismissGame)
{
	KillMatchGameTimer(IDI_SWITCH_WAIT_TIME);
	for(WORD i=0; i<m_MatchTableArray.GetCount();i++)
	{
		tagTableFrameInfo *ptagTableFrameInfo=m_MatchTableArray[i];
		ASSERT(ptagTableFrameInfo!=NULL);
		if (ptagTableFrameInfo==NULL) continue;

		ITableFrame* pTableFrame=ptagTableFrameInfo->pTableFrame;
		ASSERT(pTableFrame!=NULL);
		if (pTableFrame==NULL) continue;

		if (bDismissGame==false)
		{
			ptagTableFrameInfo->cbPlayCount=0;
			ptagTableFrameInfo->bRoundTableFinish=false;
			ptagTableFrameInfo->bSwtichTableFinish=false;
			ptagTableFrameInfo->bWaitForNextInnings=false;
		}

		//ǿ�ƽ�ɢ��Ϸ
		if (pTableFrame->IsGameStarted()==true)
		{
			if (bDismissGame==true)
			{
				bool bSuccess=pTableFrame->DismissGame();
				if (bSuccess==false)
					CTraceService::TraceString(TEXT("��ɢ��Ϸʧ�ܣ�"),TraceLevel_Exception);
			}
			else
			{
				TCHAR szMessage[64]=TEXT("");
				_sntprintf(szMessage,CountArray(szMessage),TEXT("������λʱ,�� %d ��������Ϸ״̬"),pTableFrame->GetTableID());
				CTraceService::TraceString(szMessage,TraceLevel_Exception);
			}
		}

		//�����û�ȫ���뿪
		AllUserExitTable(pTableFrame);
	}
	return;
}

//��Ϸ��ʼ
bool CImmediateGroup::OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount)
{
	pITableFrame->SetCellScore(m_dwCurBase);
	KillMatchTimer(IDI_CHECK_TABLE_START,(WPARAM)pITableFrame);

	tagTableFrameInfo *pTableFrameInfo=GetTableInterface(pITableFrame);
	pTableFrameInfo->bRoundTableFinish=false;
	pTableFrameInfo->bSwtichTableFinish=false;
	pTableFrameInfo->bWaitForNextInnings=false;

	//����ɾ�����1
	pTableFrameInfo->cbPlayCount++;

	//���ͱ�����Ϣ�������ϵ��û�
	SendTableUserMatchInfo(pITableFrame, INVALID_CHAIR);

	//�������ݰ�
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	TCHAR szMsg[256]=TEXT("");
	_sntprintf(szMsg, CountArray(szMsg),TEXT("������������%d�ֿ�ʼ����%d�֣���"),pTableFrameInfo->cbPlayCount,m_pMatchOption->MatchType.Immediate.cbPlayCount);
	lstrcpyn(SystemMessage.szString,szMsg,CountArray(SystemMessage.szString));
	SystemMessage.wLength=CountStringBuffer(SystemMessage.szString);
	SystemMessage.wType=SMT_CHAT;

	//��������
	WORD wSendSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString)+SystemMessage.wLength*sizeof(TCHAR);
	pITableFrame->SendTableData(INVALID_CHAIR,SUB_GF_SYSTEM_MESSAGE, (void*)&SystemMessage,wSendSize,MDM_GF_FRAME);

	return true;
}

//��Ϸ����
bool CImmediateGroup::OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//��¼���ֺ���Щ�û����
	tagTableFrameInfo * ptagTableFrameInfo=GetTableInterface(pITableFrame);
	ASSERT(ptagTableFrameInfo);
	if (ptagTableFrameInfo==NULL)return false;
	
	if(m_enMatchStatus==MatchStatus_FirstRound)
	{
		FirstRoundRuleInningsGameEnd(pITableFrame);
	}
	//CheckExitMatch();

	//��������ֱ�ӷ���
	if(m_enMatchStatus==MatchStatus_Null)return true;

	if(m_OnMatchUserMap.GetCount()>0)
	{
		//����֪ͨ
		tagMatchScore *pScore = new tagMatchScore[m_OnMatchUserMap.GetCount()];
		ZeroMemory(pScore,sizeof(tagMatchScore)*m_OnMatchUserMap.GetCount());
		WORD wCount=SortMapUser(pScore);
		for (WORD i=0;i<wCount;i++)
		{
			m_pMatchSink->SendGroupUserScore(pScore[i].pUserItem,this);
		}
		SafeDeleteArray(pScore);
	}

	return true;
}

//��ҷ���
bool CImmediateGroup::OnEventUserReturnMatch(ITableFrame *pITableFrame,IServerUserItem * pIServerUserItem)
{
	//���ͱ�����Ϣ
	SendTableUserMatchInfo(pITableFrame,pIServerUserItem->GetChairID());

	return true;
}

//�û�����
bool CImmediateGroup::OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{ 
	return true; 
}

//�û�����
bool CImmediateGroup::OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{

	return true;
}

 //�û�ͬ��
bool CImmediateGroup::OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{ 
	return false; 
}

//��ȡ����
WORD CImmediateGroup::GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame)
{
	//����Ч��
	ASSERT(pUserItem!=NULL);
	if(pUserItem==NULL) return INVALID_WORD;
	//�������
	SCORE lUserScore=pUserItem->GetUserScore();
	WORD wRank=1;
	DWORD dwUserID=0;
	IServerUserItem *pLoopUserItem=NULL;

	if(pITableFrame==NULL)
	{
		//��������
		POSITION pos=m_OnMatchUserMap.GetStartPosition();
		while(pos!=NULL)
		{
			m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pLoopUserItem);
			if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>lUserScore)
				wRank++;
		}
	}
	else
	{
		ASSERT(pUserItem->GetTableID()==pITableFrame->GetTableID());
		if(pUserItem->GetTableID()!=pITableFrame->GetTableID()) return INVALID_WORD;

		for(WORD i=0; i<m_wChairCount; i++)
		{
			pLoopUserItem=pITableFrame->GetTableUserItem(i);
			if(pLoopUserItem==NULL) continue;

			if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>lUserScore)
				wRank++;
		}
	}

	return wRank;
}

//��ȡ����ר�ýӿ�
tagTableFrameInfo* CImmediateGroup::GetTableInterface(ITableFrame* ITable)
{
	for (int i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if (m_MatchTableArray[i]->pTableFrame==ITable)return m_MatchTableArray[i];
	}
	return NULL;
}

//��������
void CImmediateGroup::MatchOver()
{
	//ɾ����ʱ��
	AllKillMatchTimer();
	KillMatchGameTimer(IDI_CHECK_MATCH_GROUP);
	KillMatchGameTimer(IDI_WAITTABLE_FIRST_ROUND);
	KillMatchGameTimer(IDI_WAITTABLE_RAND_TABLE);
	KillMatchGameTimer(IDI_SWITCH_WAIT_TIME);
	KillMatchGameTimer(IDI_ALL_DISMISS_GAME);
	KillMatchGameTimer(IDI_LASTMATCHRESULT);
	KillMatchGameTimer(IDI_TABLE_REQUEST_USER);

	//���ñ���״̬
	ASSERT(m_enMatchStatus==MatchStatus_FirstRound);
	if(m_enMatchStatus==MatchStatus_FirstRound)
	{
		m_enMatchStatus=MatchStatus_Null;
		m_pMatchSink->OnEventMatchOver(this);
	}
}

//����������Ϸ����
void CImmediateGroup::FirstRoundRuleInningsGameEnd(ITableFrame *pITableFrame)
{
	//��ȡ����
	tagTableFrameInfo * ptagTableFrameInfo=GetTableInterface(pITableFrame);
	ASSERT(ptagTableFrameInfo);
	if (ptagTableFrameInfo==NULL) return;

	//���õȴ��¾ֱ�ʶ
	ptagTableFrameInfo->bWaitForNextInnings=true;

	TCHAR szMessage[128]=TEXT("");

	//��������ָ������
	if (ptagTableFrameInfo->cbPlayCount>=m_pMatchOption->MatchType.Immediate.cbPlayCount)
	{
		_sntprintf(szMessage, CountArray(szMessage), TEXT("��ϲ��,��������%d����Ϸ,����������ɺ�ϵͳ����������,�������ĵȴ���"),m_pMatchOption->MatchType.Immediate.cbPlayCount);

		//������ʾ��Ϣ
		for (WORD j=0;j<m_wChairCount;j++)
		{
			IServerUserItem *pIServerUserItem=pITableFrame->GetTableUserItem(j);
			if(pIServerUserItem!=NULL)
			{
				m_pMatchSink->SendGameMessage(pIServerUserItem,szMessage,SMT_CHAT|SMT_TABLE_ROLL);

				//��������б�
				InsertFreeGroup(pIServerUserItem);
			}
			else
			{
				ASSERT(FALSE);
				CTraceService::TraceString(TEXT("�����ϢΪ��1!"),TraceLevel_Exception);
			}
		}

		//������ɱ�ʶ
		SetRoundTableFinish(pITableFrame);
		SendWaitTip(pITableFrame);
		ptagTableFrameInfo->cbPlayCount=0;
	}
	else
	{
		if(m_pMatchOption->MatchType.Immediate.cbSwitchTableCount>0&&(ptagTableFrameInfo->cbPlayCount%m_pMatchOption->MatchType.Immediate.cbSwitchTableCount)==0)
			_sntprintf(szMessage,CountArray(szMessage),TEXT("%s"),TEXT("�����ѽ����������ĵȴ���������ɣ��Ժ�Ϊ������..."));
		else
			_sntprintf(szMessage,CountArray(szMessage),TEXT("%s"),TEXT("�����ѽ��������Ժ�ϵͳ������..."));

		//������ʾ��Ϣ
		for (WORD j=0;j<m_wChairCount;j++)
		{
			IServerUserItem *pIServerUserItem=pITableFrame->GetTableUserItem(j);
			if(pIServerUserItem!=NULL)
			{
				m_pMatchSink->SendGameMessage(pIServerUserItem, szMessage, SMT_CHAT|SMT_TABLE_ROLL);

				//��������б�
				InsertFreeGroup(pIServerUserItem);
			}
			else
			{
				ASSERT(FALSE);
				CTraceService::TraceString(TEXT("�����ϢΪ��2!"),TraceLevel_Exception);
			}
		}
	}

	//�����������ָ������
	if (CheckRoundTableFinish())
	{
		SetMatchGameTimer(IDI_LASTMATCHRESULT,5000,1,0);
		return ;
	}
	else if(ptagTableFrameInfo->bRoundTableFinish==false)
	{
		//�Զ������Ŀ���
		if(m_pMatchOption->MatchType.Immediate.cbSwitchTableCount>0&&(ptagTableFrameInfo->cbPlayCount%m_pMatchOption->MatchType.Immediate.cbSwitchTableCount)==0)
		{
			SetSwitchTableFinish(pITableFrame);
			SendWaitTip(pITableFrame);

			//���������Ƿ���Ի���
			if (CheckSwitchTableFinish())
			{
				SetMatchGameTimer(IDI_SWITCH_WAIT_TIME,5000,1,0);

				return;
			}
		}
		else //������
		{
			//���ö�ʱ��
			PostMatchTimer(IDI_CONTINUE_GAME,5,(WPARAM)pITableFrame);

			return;
		}
	}
}

//������ʾ
VOID CImmediateGroup::SendWaitTip(ITableFrame *pTableFrame, WORD wChairID/*=INVALID_CHAIR*/)
{
	//״̬У��
	if(m_enMatchStatus==MatchStatus_Null || m_enMatchStatus==MatchStatus_Signup )
		return;

	tagTableFrameInfo *pITableFameEx=GetTableInterface(pTableFrame);
	ASSERT(pITableFameEx!=NULL);
	if (pITableFameEx==NULL) return;

	//��������
	WORD wPlayingTable=0;
	for(WORD i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(m_MatchTableArray[i]->pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
			wPlayingTable++;
	}

	//��������
	CMD_GR_Match_Wait_Tip WaitTip;
	ZeroMemory(&WaitTip,sizeof(WaitTip));
	WaitTip.wPlayingTable=wPlayingTable;
	WaitTip.wUserCount=(WORD)m_OnMatchUserMap.GetCount();
	WaitTip.wCurGameCount=pITableFameEx->cbPlayCount;
	WaitTip.wGameCount=m_pMatchOption->MatchType.Immediate.cbPlayCount;

	lstrcpyn(WaitTip.szMatchName, m_pMatchOption->szMatchName,LEN_SERVER);

	//���ͱ�����Ϣ�����������
	for(WORD i=0;i<m_wChairCount;i++)
	{
		if(wChairID!=INVALID_CHAIR && wChairID!=i) continue;
		IServerUserItem *pTableUserItem=pTableFrame->GetTableUserItem(i);
		if(pTableUserItem==NULL) continue;
		WaitTip.wCurTableRank=GetUserRank(pTableUserItem,pTableFrame);
		if(WaitTip.wCurTableRank==INVALID_WORD) continue;


		WaitTip.wRank=GetUserRank(pTableUserItem);
		WaitTip.lScore=pTableUserItem->GetUserScore();

		m_pMatchSink->SendData(pTableUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,&WaitTip,sizeof(WaitTip));
	}

	//���ͱ�����Ϣ�������������
	for(WORD i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(pTableFrame->GetTableID()==m_MatchTableArray[i]->pTableFrame->GetTableID())continue;
		if(m_MatchTableArray[i]->pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY) continue; 
		for(WORD j=0;j<m_wChairCount;j++)
		{
			IServerUserItem *pTableUserItem=m_MatchTableArray[i]->pTableFrame->GetTableUserItem(j);
			if(pTableUserItem==NULL) continue;
			WaitTip.wCurTableRank=GetUserRank(pTableUserItem,m_MatchTableArray[i]->pTableFrame);
			if(WaitTip.wCurTableRank==INVALID_WORD) continue;

			WaitTip.wRank=GetUserRank(pTableUserItem);
			WaitTip.lScore=pTableUserItem->GetUserScore();

			m_pMatchSink->SendData(pTableUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,&WaitTip,sizeof(WaitTip));
		}
	}
}

//���������������û�
void CImmediateGroup::TabelRequestUser(bool bPrecedeSit)
{
	//�����û������������� ��Ҫ��ֹ����
	if (CheckMatchUser())
	{
		return;
	}
	CTableFrameMananerArray			FreeTableArray;				//�ձ�������
	//���Ȼ�ȡ������
	for (INT_PTR i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(m_MatchTableArray[i]->pTableFrame->IsGameStarted()==false&&m_MatchTableArray[i]->bRoundTableFinish==false)
		{
			//��ȡ����
			INT_PTR cbTimer=m_MatchTimerArray.GetCount();
			bool cbIsInsert=false;
			for(int j=0;j<cbTimer;j++)
			{
				//�ж�һ����û�п����������ڵȴ�
				if((m_MatchTimerArray[j]->dwTimerID==IDI_CHECK_TABLE_START||m_MatchTimerArray[j]->dwTimerID==IDI_CONTINUE_GAME)&&(ITableFrame*)(m_MatchTimerArray[j]->wParam)==m_MatchTableArray[i]->pTableFrame)
				{
					cbIsInsert=true;
					break;
				}
			}
			if(cbIsInsert==false)
				FreeTableArray.Add(m_MatchTableArray[i]);
		}
	}

	DWORD dwCurTime=(DWORD)time(NULL);

	bool bIsContinueCheck=false;//�Ż��������� ԭ����true
	for (INT_PTR i=0;i<FreeTableArray.GetCount();i++)
	{
		DWORD nCurUserCount=(DWORD)m_FreeUserArray.GetCount();
		//�ҵ�һ������ �����û�������ڵ�������
		if(nCurUserCount/m_wChairCount>0)
		{
			AllUserExitTable(FreeTableArray[i]->pTableFrame);
			for (int j=0;j<m_wChairCount;j++)
			{
				//�û��������
				WORD wRandID=INVALID_VALUE;
				if(bPrecedeSit&&bIsContinueCheck)
				{
					wRandID=GetPrecedeSitUser(dwCurTime);
				}
				if(wRandID==INVALID_VALUE)
				{
					bIsContinueCheck=false;//�Ѿ�û�������û���
					wRandID=rand()%(WORD)m_FreeUserArray.GetCount();
				}

				IServerUserItem *pIServerUserItem=m_FreeUserArray[wRandID];
				ASSERT(pIServerUserItem!=NULL);
				if (pIServerUserItem==NULL) continue;
				
				//������������ �ø�������ȫ������
				if(pIServerUserItem->GetTableID()!=INVALID_TABLE)
				{
					for(int m=0;m<m_MatchTableArray.GetCount();m++)
					{
						if(pIServerUserItem->GetTableID()==m_MatchTableArray[m]->wTableID)
						{
							m_MatchTableArray[m]->pTableFrame->PerformStandUpAction(pIServerUserItem,true);
							break;
						}
					}
				}
				if (FreeTableArray[i]->pTableFrame->PerformSitDownAction(j,pIServerUserItem))
				{
					m_FreeUserArray.RemoveAt(wRandID);
				}
				else
				{
					CTraceService::TraceString(TEXT("����ʧ�ܣ�"),TraceLevel_Exception);
				}
			}

			//10�����һ����� ���ָ�����û�п�ʼ��Ϸ ˵���Ѿ�������
			PostMatchTimer(IDI_CHECK_TABLE_START,10,(WPARAM)FreeTableArray[i]->pTableFrame);
		}
	}

	return ;
}
//����һ����Ϣ
void CImmediateGroup::SendGroupBox(LPCTSTR pStrMessage)
{
	//�������ݰ�
	CMD_CM_SystemMessage Message;
	Message.wType=SMT_EJECT|SMT_CHAT;
	lstrcpyn(Message.szString,pStrMessage,CountArray(Message.szString));
	Message.wLength=CountStringBuffer(Message.szString);
	//��������
	WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
	m_pMatchSink->SendGroupData(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&Message,wSendSize, this);
}

//����һ����ɱ��
void CImmediateGroup::SetRoundTableFinish(ITableFrame *pITableFrame)
{
	tagTableFrameInfo * ptagTableFrameInfo=GetTableInterface(pITableFrame);
	ASSERT(ptagTableFrameInfo);
	if (ptagTableFrameInfo)ptagTableFrameInfo->bRoundTableFinish=true;
}

//���һ���Ƿ����
bool CImmediateGroup::CheckRoundTableFinish()
{
	for (int i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(m_MatchTableArray[i]->bRoundTableFinish==false)return false;
	}
	return true;
}

//���û�����ɱ��
void CImmediateGroup::SetSwitchTableFinish(ITableFrame *pITableFrame)
{
	tagTableFrameInfo * ptagTableFrameInfo=GetTableInterface(pITableFrame);
	ASSERT(ptagTableFrameInfo);
	if (ptagTableFrameInfo)ptagTableFrameInfo->bSwtichTableFinish=true;
}

//��黻���Ƿ����
bool CImmediateGroup::CheckSwitchTableFinish()
{
	for (int i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(m_MatchTableArray[i]->bSwtichTableFinish==false)return false;
	}
	return true;
}

//���������û��뿪
void CImmediateGroup::AllUserExitTable(ITableFrame *pITableFrame)
{
	ASSERT(pITableFrame!=NULL);
	if(pITableFrame)
	{
		//�����û�ȫ���뿪
		for (int i=0;i<m_wChairCount;i++)
		{
			IServerUserItem* pUserItem=pITableFrame->GetTableUserItem(i);
			if(pUserItem&&pUserItem->GetTableID()!=INVALID_TABLE)pITableFrame->PerformStandUpAction(pUserItem,true);
		}
	}
}
//����������Ϸ
void CImmediateGroup::ContinueGame(ITableFrame *pITableFrame)
{
	if (CheckMatchUser())
	{
		return;
	}
	
	tagTableFrameInfo * ptagTableFrameInfo=GetTableInterface(pITableFrame);
	ASSERT(ptagTableFrameInfo);
	if (ptagTableFrameInfo)
	{
		AllUserExitTable(pITableFrame);
		for(int i=0;i<m_wChairCount;i++)
		{
			IServerUserItem* pIServerUserItem=m_FreeUserArray[0];
			if(pIServerUserItem==NULL)
			{				
				ASSERT(FALSE);
				CTraceService::TraceString(TEXT("�յ��û������ڵȴ��б��С�"),TraceLevel_Exception);
				return;
			}

			//������������ �ø�������ȫ������
			if(pIServerUserItem->GetTableID()!=INVALID_TABLE)
			{
				for(int m=0;m<m_MatchTableArray.GetCount();m++)
				{
					if(pIServerUserItem->GetTableID()==m_MatchTableArray[m]->wTableID)
					{
						m_MatchTableArray[m]->pTableFrame->PerformStandUpAction(pIServerUserItem,true);

						break;
					}
				}
			}

			if (pITableFrame->PerformSitDownAction(i,pIServerUserItem))
			{
				m_FreeUserArray.RemoveAt(0);
			}
			else
			{
				CTraceService::TraceString(TEXT("����ʧ�ܣ�1"),TraceLevel_Exception);
			}
		}

		//30�����һ����� ���ָ�����û�п�ʼ��Ϸ ˵���Ѿ�������
		PostMatchTimer(IDI_CHECK_TABLE_START,30,(WPARAM)pITableFrame);
	}
}

void CImmediateGroup::SetMatchGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)
{
	if(m_pMatchSink)
		m_pMatchSink->SetGameTimer(dwTimerID+10*m_LoopTimer,dwElapse,dwRepeat,dwBindParameter,this);
}

//ɱ������ר�ö�ʱ��
void CImmediateGroup::KillMatchGameTimer(DWORD dwTimerID)
{
	if(m_pMatchSink)
		m_pMatchSink->KillGameTimer(dwTimerID+10*m_LoopTimer,this);
}

//����������
bool CImmediateGroup::CheckMatchUser()
{
	//�����û����� ����������Ҫ��ֹ����
	DWORD nCurUserCount=(DWORD)m_OnMatchUserMap.GetCount();
	if (nCurUserCount<m_wChairCount)
	{
		ASSERT(FALSE);
		CTraceService::TraceString(TEXT("�������..."),TraceLevel_Exception);
		return false;

		//�������ݰ�
		CString szString;
		szString.Format(TEXT("��Ǹ�����ڱ����û�ǿ���˳��������������� %d �ˣ���ǰ���㡣��ӭ�����������μӱ�����"),m_wChairCount);
		SendGroupBox(szString);

		MatchOver();

		return true;
	}
	return false;
}

//��������Ƿ�ʼ��Ϸ
void CImmediateGroup::CheckTableStart(ITableFrame *pITableFrame)
{
	//У�����
	ASSERT(pITableFrame!=NULL);
	if (pITableFrame==NULL) return;

	if(pITableFrame&&pITableFrame->IsGameStarted()==false)
	{
		for (int i=0;i<m_wChairCount;i++)
		{
			IServerUserItem* pIServerUserItem=pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;

			if(pIServerUserItem&&pIServerUserItem->GetTableID()!=INVALID_TABLE)
			{
				//��ֹ�������������˶����ߵ����޷�ģ�ⷢ��������Ϣ
				if(pIServerUserItem->IsClientReady()==false || pIServerUserItem->IsTrusteeUser() == true)
				{
					pITableFrame->ImitateGameOption(pIServerUserItem);
				}
			}
		}
	}
}

//��ȡ���������û�
WORD CImmediateGroup::GetPrecedeSitUser(DWORD dwCurTime)
{
	for(int i=0;i<m_FreeUserArray.GetCount();i++)
	{
		tagTimeInfo* pTimerInfo=m_FreeUserArray[i]->GetTimeInfo();
		if(dwCurTime-pTimerInfo->dwEndGameTimer>m_pMatchOption->MatchType.Immediate.wPrecedeTimer) return i;
	}
	return INVALID_VALUE;
}

//ʵ�ָú����ɱ�֤Free��Ψһ
void CImmediateGroup::InsertFreeGroup(IServerUserItem *pUserItem)
{
	if( pUserItem == NULL )return;
	for(int i=0;i<m_FreeUserArray.GetCount();i++)
	{
		if( m_FreeUserArray[i] != NULL && m_FreeUserArray[i]->GetUserID()==pUserItem->GetUserID())return;
	}
	m_FreeUserArray.Add(pUserItem);
}

//���ͱ�����Ϣ
void CImmediateGroup::SendTableUserMatchInfo(ITableFrame *pITableFrame, WORD wChairID)
{
	//��֤����
	ASSERT(pITableFrame!=NULL);
	if (pITableFrame==NULL) return;

	tagTableFrameInfo *ptagTableFrameInfo=GetTableInterface(pITableFrame);
	if (ptagTableFrameInfo==NULL) return;

	//��������
	WORD wChairCount=pITableFrame->GetChairCount();
	TCHAR szMsg[256]=TEXT("");

	if(m_enMatchStatus==MatchStatus_FirstRound)
	{
		//��������
		CMD_GR_Match_Info MatchInfo;
		ZeroMemory(&MatchInfo,sizeof(MatchInfo));
		_sntprintf(MatchInfo.szTitle[0], CountArray(MatchInfo.szTitle[0]),TEXT("���ַ�ʽ���ۼƻ���"));
		_sntprintf(MatchInfo.szTitle[1], CountArray(MatchInfo.szTitle[1]),TEXT("��Ϸ������%d"), m_dwCurBase);
		_sntprintf(MatchInfo.szTitle[2], CountArray(MatchInfo.szTitle[2]),TEXT("�������ƣ���������"));
		_sntprintf(MatchInfo.szTitle[3], CountArray(MatchInfo.szTitle[3]),TEXT("��Ҫ��ɣ�%d�ֱ���"), m_pMatchOption->MatchType.Immediate.cbPlayCount);
		MatchInfo.wGameCount=ptagTableFrameInfo->cbPlayCount;
		
		//wChairIDΪINVALID_CHAIR��������������ң�����ֻ����ָ�����
		for(WORD i=0; i<wChairCount; i++)
		{
			//��ȡ�û�
			if(wChairID!=INVALID_CHAIR && wChairID!=i) continue;
			IServerUserItem *pIServerUserItem=pITableFrame->GetTableUserItem(i);
			if(pIServerUserItem==NULL) continue;

			//��������
			pITableFrame->SendTableData(i, SUB_GR_MATCH_INFO, &MatchInfo, sizeof(MatchInfo), MDM_GF_FRAME);
		}

		//������ʾ
		if ((wChairID!=INVALID_CHAIR) && (ptagTableFrameInfo->bWaitForNextInnings==true))
		{
			SendWaitTip(pITableFrame,wChairID);
		}
	}

	return;
}

//�������ר�ö�ʱ��
void CImmediateGroup::CaptureMatchTimer()
{
	for(int i=0;i<m_MatchTimerArray.GetCount();i++)
	{
		if(--m_MatchTimerArray[i]->iElapse<=0)
		{
			if(m_MatchTimerArray[i]->dwTimerID==IDI_CONTINUE_GAME)
			{
				//������Ϸ
				ContinueGame((ITableFrame*)m_MatchTimerArray[i]->wParam);
			}
			else if(m_MatchTimerArray[i]->dwTimerID==IDI_LEAVE_TABLE)
			{
				//�뿪����
				AllUserExitTable((ITableFrame*)m_MatchTimerArray[i]->wParam);

			}else if(m_MatchTimerArray[i]->dwTimerID==IDI_CHECK_TABLE_START)
			{
				//�������
				CheckTableStart((ITableFrame*)m_MatchTimerArray[i]->wParam);
			}
			//ɾ����ʱ��
			if(KillMatchTimer(i))i--;
		}
	}
}
//Ͷ�ݱ���ר�ö�ʱ��
void CImmediateGroup::PostMatchTimer(DWORD dwTimerID, int iElapse, WPARAM wParam, LPARAM lParam)
{
	tagMatchTimer* pMatchTimer=new tagMatchTimer;
	pMatchTimer->dwTimerID=dwTimerID;
	pMatchTimer->iElapse=iElapse;
	//pMatchTimer->dwRepeat=dwRepeat;
	pMatchTimer->wParam=wParam;
	pMatchTimer->lParam=lParam;
	m_MatchTimerArray.Add(pMatchTimer);
}
//ɱ������ר�ö�ʱ��
bool CImmediateGroup::KillMatchTimer(DWORD dwTimerID, WPARAM wParam)
{
	for(int i=0;i<m_MatchTimerArray.GetCount();i++)
	{
		if(m_MatchTimerArray[i]->dwTimerID==dwTimerID&&m_MatchTimerArray[i]->wParam==wParam)
		{
			tagMatchTimer* pMatchTimer=m_MatchTimerArray[i];
			m_MatchTimerArray.RemoveAt(i);
			SafeDelete(pMatchTimer);
			return true;
		}
	}
	return false;
}
//ɱ������ר�ö�ʱ��
bool CImmediateGroup::KillMatchTimer(INT_PTR dwIndexID)
{
	//ASSERT(dwIndexID >= 0 && dwIndexID < m_MatchTimerArray.GetCount());
	if( dwIndexID >= 0 && dwIndexID < m_MatchTimerArray.GetCount() )
	{
		tagMatchTimer* pMatchTimer=m_MatchTimerArray[dwIndexID];
		m_MatchTimerArray.RemoveAt(dwIndexID);
		SafeDelete(pMatchTimer);
		return true;
	}
	return false;
}
//ȫ��ɱ������ר�ö�ʱ��
void CImmediateGroup::AllKillMatchTimer()
{
	for(int i=0;i<m_MatchTimerArray.GetCount();/*i++*/)
	{
		tagMatchTimer* pMatchTimer=m_MatchTimerArray[i];
		m_MatchTimerArray.RemoveAt(i);
		SafeDelete(pMatchTimer);
	}
}