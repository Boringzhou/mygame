#include "StdAfx.h"
#include "TableFrame.h"
#include "DataBasePacket.h"
#include "AttemperEngineSink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////

//���߶���
#define IDI_OFF_LINE				(TIME_TABLE_SINK_RANGE+1)			//���߱�ʶ
#define MAX_OFF_LINE				3									//���ߴ���
#define TIME_OFF_LINE				300000L								//����ʱ��

//��ʼ��ʱ
#define IDI_START_OVERTIME		(TIME_TABLE_SINK_RANGE+2)			    //��ʼ��ʱ
#define IDI_START_OVERTIME_END	(TIME_TABLE_SINK_RANGE+22)			    //��ʼ��ʱ
#ifndef _DEBUG
#define TIME_OVERTIME				30000L								//��ʱʱ��
#else
#define TIME_OVERTIME               30000L                               //��ʱʱ��
#endif

//��ɢ�ȴ�
#define IDI_WAIT_DISMISS			(TIME_TABLE_SINK_RANGE+3)			//�ȴ���ɢ��ʶ
#define TIME_WAIT_DISMISS			300000L								//�ȴ���ɢʱ��

//���ѵȴ�
#define IDI_WAIT_CONTINUE			(TIME_TABLE_SINK_RANGE+4)			//�ȴ���ɢ��ʶ
#define TIME_WAIT_CONTINUE			300000L								//�ȴ���ɢʱ��

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��¼
CGameScoreRecordArray				CTableFrame::m_GameScoreRecordBuffer;

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrame::CTableFrame()
{
	//��������
	m_wTableID=0;
	m_wChairCount=0;
	m_cbStartMode=START_MODE_ALL_READY;
	m_wUserCount=0;

	//��־����
	m_bGameStarted=false;
	m_bDrawStarted=false;
	m_bTableStarted=false;
	m_bTableInitFinish=false;
	m_bWaitForContinue=false;
	ZeroMemory(m_bAllowLookon,sizeof(m_bAllowLookon));
	ZeroMemory(m_lFrozenedScore,sizeof(m_lFrozenedScore));
	ZeroMemory(m_bAgree,sizeof(m_bAgree));
	ZeroMemory(m_szHeadUrl,sizeof(m_szHeadUrl));

	//��Ϸ����
	m_lCellScore=0L;
	m_wDrawCount=0;
	m_cbGameStatus=GAME_STATUS_FREE;
	m_lZuanshi = 0;
	m_cbPayType = 0;

	//ʱ�����
	m_dwDrawStartTime=0L;
	ZeroMemory(&m_SystemTimeStart,sizeof(m_SystemTimeStart));

	//��̬����
	m_dwTableOwnerID=0L;
	ZeroMemory(m_szEnterPassword,sizeof(m_szEnterPassword));
	m_dwTablePassword = 0;

	//���߱���
	ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));
	ZeroMemory(m_dwOffLineTime,sizeof(m_dwOffLineTime));
	ZeroMemory(m_bContinue,sizeof(m_bContinue));

	//������Ϣ
	m_pGameParameter=NULL;
	m_pGameMatchOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//����ӿ�
	m_pITimerEngine=NULL;
	m_pITableFrameSink=NULL;
	m_pIMainServiceFrame=NULL;
	m_pIAndroidUserManager=NULL;

	//���Žӿ�
	m_pITableUserAction=NULL;
	m_pITableUserRequest=NULL;

	//���ݽӿ�
	m_pIKernelDataBaseEngine=NULL;
	m_pIRecordDataBaseEngine=NULL;

	//�����ӿ�
	m_pITableFrameHook=NULL;
	m_pIMatchTableAction=NULL;

	//�û�����
	ZeroMemory(m_TableUserItemArray,sizeof(m_TableUserItemArray));

	m_bFangkaGameStarted = false;

	m_bRefuseSameIP = false;
	m_bDistanceIn300 = false;

	ZeroMemory(m_dLongitude,sizeof(m_dLongitude));
	ZeroMemory(m_dLatitude,sizeof(m_dLatitude));

	return;
}

//��������
CTableFrame::~CTableFrame()
{
	//�ͷŶ���
	SafeRelease(m_pITableFrameSink);
	SafeRelease(m_pITableFrameHook);

	return;
}

//�ӿڲ�ѯ
VOID * CTableFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrame,Guid,dwQueryVer);
	QUERYINTERFACE(ICompilationSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrame,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��Ϸ
bool CTableFrame::StartGame()
{
	//��Ϸ״̬
	ASSERT(m_bDrawStarted==false);
	if (m_bDrawStarted==true) return false;

	//�������
	bool bGameStarted=m_bGameStarted;
	bool bTableStarted=m_bTableStarted;

	//����״̬
	m_bGameStarted=true;
	m_bDrawStarted=true;
	m_bTableStarted=true;

	//��ʼʱ��
	GetLocalTime(&m_SystemTimeStart);
	m_dwDrawStartTime=(DWORD)time(NULL);

	//��ʼ����
	if (bGameStarted==false)
	{
		//״̬����
		ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));
		ZeroMemory(m_dwOffLineTime,sizeof(m_dwOffLineTime));

		//�����û�
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);

			//�����û�
			if (pIServerUserItem!=NULL)
			{
				// �۳�����:DCY
				if(m_cbPayType == 1)
				{		
					DedultScore(pIServerUserItem,m_lZuanshi);
				}
				else if(i == m_wFangzhu)
				{
					DedultScore(pIServerUserItem,m_lZuanshi*m_wChairCount);
				}
				
				//������Ϸ��
				if (m_pGameServiceOption->lServiceScore>0L)
				{
					m_lFrozenedScore[i]=m_pGameServiceOption->lServiceScore;
					pIServerUserItem->FrozenedUserScore(m_pGameServiceOption->lServiceScore);
				}

				//����״̬
				BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
				if ((cbUserStatus!=US_OFFLINE)&&(cbUserStatus!=US_PLAYING))
				{
					pIServerUserItem->SetUserStatus(US_PLAYING,m_wTableID,i);
				}
			}
		}

		//����״̬
		if (bTableStarted!=m_bTableStarted) SendTableStatus();
	}

	//����֪ͨ
	bool bStart=true;
	//if(m_pITableFrameHook!=NULL) bStart=m_pITableFrameHook->OnEventGameStart(m_wChairCount);

	//֪ͨ�¼�
	ASSERT(m_pITableFrameSink!=NULL);
	printf("��Ϸ��ʼ�¼�\n");
	if (m_pITableFrameSink!=NULL&&bStart) m_pITableFrameSink->OnEventGameStart();

	return true;
}

//��ɢ��Ϸ
bool CTableFrame::DismissGame()
{
	//״̬�ж�
	ASSERT(m_bTableStarted==true);
	if (m_bTableStarted==false) return false;

	//������Ϸ
	if ((m_bGameStarted==true)&&(m_pITableFrameSink->OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS)==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//����״̬
	if ((m_bGameStarted==false)&&(m_bTableStarted==true))
	{
		//���ñ���
		m_bTableStarted=false;

		//����״̬
		SendTableStatus();
	}

	m_bFangkaGameStarted = false;

	return false;
}

bool CTableFrame::ContinueGame(WORD wChairID)
{
	if(m_bContinue[wChairID])
		return true;

	m_bContinue[wChairID] = true;

	IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);

	if(m_bWaitForContinue)
	{
		//ɾ�����Ѷ�ʱ��
		KillGameTimer(IDI_WAIT_CONTINUE);

		SCORE lZuanshi = m_cbPayType==1?m_lZuanshi:(m_lZuanshi*m_wChairCount);

		if(lZuanshi>pIServerUserItem->GetUserScore())
		{
			SendRequestFailure(pIServerUserItem,TEXT("��ʯ���㣬����ʧ��!"),REQUEST_FAILURE_NORMAL);

			if (m_lFrozenedScore[wChairID]!=0L)
			{
				pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[wChairID]);
				m_lFrozenedScore[wChairID]=0L;
			}

			//����״̬
			pIServerUserItem->SetUserStatus(US_NULL,m_wTableID,wChairID);	

			return true;
		}

		//�۳���������
		//DedultScore(pIServerUserItem,lZuanshi);
	
		//��һ������Ϊ����
		m_wFangzhu = wChairID;
		m_bWaitForContinue = false;

		//֪ͨ�����˷�����Ϣ��������
		for (int i=0; i<m_wChairCount; i++)
		{
			if(wChairID == i) continue;

			IServerUserItem *pUserItem = GetTableUserItem(i);
			if(pUserItem==NULL) continue;

			CMD_GF_XUFEI nXufei;
			ZeroMemory(&nXufei,sizeof(nXufei));
			nXufei.wChairID = wChairID;
			nXufei.bFangzhu = true;
			m_pIMainServiceFrame->SendData(pUserItem,MDM_GF_FRAME,SUB_GF_XUFEI,&nXufei,sizeof(nXufei));
		}

	}
	
	//�۳�����
 	if(wChairID!=m_wFangzhu)
 	{
		for (int i=0; i<m_wChairCount; i++)
		{
			if(wChairID == i) continue;

			IServerUserItem *pUserItem = GetTableUserItem(i);
			if(pUserItem==NULL) continue;

			CMD_GF_XUFEI nXufei;
			ZeroMemory(&nXufei,sizeof(nXufei));
			nXufei.wChairID = wChairID;
			nXufei.bFangzhu = false;
			m_pIMainServiceFrame->SendData(pUserItem,MDM_GF_FRAME,SUB_GF_XUFEI,&nXufei,sizeof(nXufei));
		}
 	}

	//���������ѣ���ʼ��Ϸ
	bool bAllContinue = true;
	for (int i=0; i<m_wChairCount; i++)
	{
		printf("��� %d ׼�� %d\n",i,m_bContinue[i]);
		if(!m_bContinue[i])
			bAllContinue = false;
	}

	if(bAllContinue) StartGame();

	return true;
}

//������Ϸ
bool CTableFrame::ConcludeGame(BYTE cbGameStatus)
{
	if (cbGameStatus == 0xFF)
	{
		if(!m_bWaitForContinue)
		{
			m_bGameStarted=false;

			m_bDrawStarted=false;

			m_cbGameStatus=GAME_STATUS_FREE;

			m_bTableStarted=false;

			SetFangkaStart(false);

			ZeroMemory(m_bAgree,sizeof(m_bAgree));

			if (m_pITableFrameSink!=NULL) m_pITableFrameSink->RepositionSink();

			ZeroMemory(m_bContinue,sizeof(m_bContinue));
			m_bWaitForContinue = true;

			//�������Ѷ�ʱ��
			SetGameTimer(IDI_WAIT_CONTINUE,TIME_WAIT_CONTINUE,1,NULL);

			return true;
		}

		for (WORD wChairID = 0;wChairID<m_wChairCount;wChairID++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
			//�û�����
			if (pIServerUserItem!=NULL)
			{
				//������Ϸ��
				if (m_lFrozenedScore[wChairID]!=0L)
				{
					pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[wChairID]);
					m_lFrozenedScore[wChairID]=0L;
				}
				//����״̬
				pIServerUserItem->SetUserStatus(US_NULL,m_wTableID,wChairID);	
			}
		}

		return true;
	}

	if(cbGameStatus == 0xFE)
	{
		for (WORD wChairID = 0;wChairID<m_wChairCount;wChairID++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
			//�û�����
			if (pIServerUserItem!=NULL)
			{
				//������Ϸ��
				if (m_lFrozenedScore[wChairID]!=0L)
				{
					pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[wChairID]);
					m_lFrozenedScore[wChairID]=0L;
				}
				//����״̬
				pIServerUserItem->SetUserStatus(US_NULL,m_wTableID,wChairID);	
			}
		}

		m_bGameStarted=false;

		m_bDrawStarted=false;

		m_cbGameStatus=GAME_STATUS_FREE;

		m_bTableStarted=false;

		SetFangkaStart(false);

		ZeroMemory(m_bAgree,sizeof(m_bAgree));

		if (m_pITableFrameSink!=NULL) m_pITableFrameSink->RepositionSink();

		ZeroMemory(m_bContinue,sizeof(m_bContinue));

		return true;
	}

	//Ч��״̬
	ASSERT(m_bGameStarted==true);
	if (m_bGameStarted==false) return false;

	//�������
	bool bDrawStarted=m_bDrawStarted;

	//����״̬
	m_bDrawStarted=false;
	m_cbGameStatus=cbGameStatus;
	m_bGameStarted=(cbGameStatus>=GAME_STATUS_PLAY)?true:false;
	m_wDrawCount++;

	//��Ϸ��¼
	RecordGameScore(bDrawStarted);
	
	//��������
	if (m_bGameStarted==false)
	{
		//��������
		bool bOffLineWait=false;

		//�����û�
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);

			//�û�����
			if (pIServerUserItem!=NULL)
			{
				//������Ϸ��
				if (m_lFrozenedScore[i]!=0L)
				{
					pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[i]);
					m_lFrozenedScore[i]=0L;
				}

				//����״̬
				if (pIServerUserItem->GetUserStatus()==US_OFFLINE)
				{
					//���ߴ���
					bOffLineWait=true;
					if(m_pGameServiceOption->wServerType!=GAME_GENRE_MATCH)
					{
						PerformStandUpAction(pIServerUserItem);
					}
					else
					{
						pIServerUserItem->SetClientReady(true);
						pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,i);
					}
				}
				else
				{
					//����״̬
					pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,i);					
				}
			}
		}

		//ɾ��ʱ��
		if (bOffLineWait==true) KillGameTimer(IDI_OFF_LINE);
	}

	//֪ͨ����
	if(m_pITableFrameHook!=NULL) m_pITableFrameHook->OnEventGameEnd(0, NULL, cbGameStatus);

	//��������
	ASSERT(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink!=NULL) m_pITableFrameSink->RepositionSink();

	//�߳����
	if (m_bGameStarted==false)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			if (m_TableUserItemArray[i]==NULL) continue;
			IServerUserItem * pIServerUserItem=m_TableUserItemArray[i];

			//��������
			if ((m_pGameServiceOption->lMinTableScore!=0L)&&(pIServerUserItem->GetUserScore()<m_pGameServiceOption->lMinTableScore))
			{
				//������ʾ
				TCHAR szDescribe[128]=TEXT("");
				if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
				{
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������Ϸ������ ") SCORE_STRING TEXT("�����ܼ�����Ϸ��"),m_pGameServiceOption->lMinTableScore);
				}
				else
				{
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������Ϸ�������� ") SCORE_STRING TEXT("�����ܼ�����Ϸ��"),m_pGameServiceOption->lMinTableScore);
				}

				//������Ϣ
				if (pIServerUserItem->IsAndroidUser()==true)
					SendGameMessage(pIServerUserItem,szDescribe,SMT_CHAT|SMT_CLOSE_GAME|SMT_CLOSE_ROOM|SMT_EJECT);
				else
					SendGameMessage(pIServerUserItem,szDescribe,SMT_CHAT|SMT_CLOSE_GAME|SMT_EJECT);

				//�û�����
				PerformStandUpAction(pIServerUserItem);

				continue;
			}

			//�ر��ж�
			if ((CServerRule::IsForfendGameEnter(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
			{
				//������Ϣ
				LPCTSTR pszMessage=TEXT("����ϵͳά������ǰ��Ϸ���ӽ�ֹ�û�������Ϸ��");
				SendGameMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_CHAT|SMT_CLOSE_GAME);

				//�û�����
				PerformStandUpAction(pIServerUserItem);

				continue;
			}

			//��������
			if (pIServerUserItem->GetUserStatus()!=US_OFFLINE && (m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0)
			{
				//��������
				if (pIServerUserItem->IsAndroidUser()==true)
				{
					//����Ϣ
					CAttemperEngineSink * pAttemperEngineSink=(CAttemperEngineSink *)m_pIMainServiceFrame;
					tagBindParameter * pBindParameter=pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());

					//���һ���
					IAndroidUserItem * pIAndroidUserItem=m_pIAndroidUserManager->SearchAndroidUserItem(pIServerUserItem->GetUserID(),pBindParameter->dwSocketID);
					if(pIAndroidUserItem==NULL) continue;

					//�뿪�ж�
					if(pIAndroidUserItem->JudgeAndroidActionAndRemove(ANDROID_WAITLEAVE))
					{
						//��������
						PerformStandUpAction(pIServerUserItem);

						continue;
					}

					//�����ж�
					if(pIAndroidUserItem->JudgeAndroidActionAndRemove(ANDROID_WAITSTANDUP))
					{
						//��������
						PerformStandUpAction(pIServerUserItem);

						continue;
					}												
				}
			}

			//������ʱ
			if(IsGameStarted()==false && pIServerUserItem->IsMobileUser() && m_pGameServiceAttrib->wChairCount<MAX_CHAIR)						SetGameTimer(IDI_START_OVERTIME+i,TIME_OVERTIME,1,i);
		}		
	}

	//��������
	ConcludeTable();

	//����״̬
	SendTableStatus();

	return true;
}

//��������
bool CTableFrame::ConcludeTable()
{
	//��������
	if ((m_bGameStarted==false)&&(m_bTableStarted==true))
	{
		//�����ж�
		WORD wTableUserCount=GetSitUserCount();
		if (wTableUserCount==0) m_bTableStarted=false;
		if (m_pGameServiceAttrib->wChairCount==MAX_CHAIR) m_bTableStarted=false;

		//ģʽ�ж�
		if (m_cbStartMode==START_MODE_FULL_READY) m_bTableStarted=false;
		if (m_cbStartMode==START_MODE_PAIR_READY) m_bTableStarted=false;
		if (m_cbStartMode==START_MODE_ALL_READY) m_bTableStarted=false;
	}

	ZeroMemory(m_bAgree,sizeof(m_bAgree));

	return true;
}

//��ʼ����ģʽ
void CTableFrame::SetFangkaStart(bool bStart)
{
	m_bFangkaGameStarted = bStart;
	if(!bStart)
	{
		ZeroMemory(m_bResponseDismiss,sizeof(m_bResponseDismiss));
		ZeroMemory(m_bAgree,sizeof(m_bAgree));
		KillGameTimer(IDI_WAIT_DISMISS);
	}
}

//д�����
bool CTableFrame::WriteUserScore(WORD wChairID, tagScoreInfo & ScoreInfo, DWORD dwGameMemal, DWORD dwPlayGameTime)
{
	printf("CTableFrame::WriteUserScore\n");
	//Ч�����
	ASSERT((wChairID<m_wChairCount)&&(ScoreInfo.cbType!=SCORE_TYPE_NULL));
	if ((wChairID>=m_wChairCount)&&(ScoreInfo.cbType==SCORE_TYPE_NULL)) return false;

	//��ȡ�û�
	ASSERT(GetTableUserItem(wChairID)!=NULL);
	IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
	TCHAR szMessage[128]=TEXT("");

	//д�����
	if (pIServerUserItem!=NULL)
	{
		//��������
		DWORD dwUserMemal=0L;
		SCORE lRevenueScore=__min(m_lFrozenedScore[wChairID],m_pGameServiceOption->lServiceScore);

		//�۷����
		if (m_pGameServiceOption->lServiceScore>0L 
			&& m_pGameServiceOption->wServerType == GAME_GENRE_GOLD
			&& m_pITableFrameSink->QueryBuckleServiceCharge(wChairID))
		{
			//�۷����
			ScoreInfo.lScore-=lRevenueScore;
			ScoreInfo.lRevenue+=lRevenueScore;

			//������Ϸ��
			pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[wChairID]);
			m_lFrozenedScore[wChairID]=0L;
		}

		//���Ƽ���
		if(dwGameMemal != INVALID_DWORD)
		{
			dwUserMemal = dwGameMemal;
		}
		else if (ScoreInfo.lRevenue>0L)
		{
			DWORD dwMedalRate=m_pGameParameter->dwMedalRate;
			dwUserMemal=(DWORD)(ScoreInfo.lRevenue*dwMedalRate/1000L);
		}

		//��Ϸʱ��
		DWORD dwCurrentTime=(DWORD)time(NULL);
		DWORD dwPlayTimeCount=((m_bDrawStarted==true)?(dwCurrentTime-m_dwDrawStartTime):0L);
		if(dwPlayGameTime!=INVALID_DWORD) dwPlayTimeCount=dwPlayGameTime;

		//��������
		tagUserProperty * pUserProperty=pIServerUserItem->GetUserProperty();

		//�����ж�
		if(m_pGameServiceOption->wServerType == GAME_GENRE_SCORE)
		{
			if (ScoreInfo.lScore>0L)
			{
				//�ı�����
				if ((pUserProperty->wPropertyUseMark&PT_USE_MARK_FOURE_SCORE)!=0)
				{
					//��������
					DWORD dwValidTime=pUserProperty->PropertyInfo[1].wPropertyCount*pUserProperty->PropertyInfo[1].dwValidNum;
					if(pUserProperty->PropertyInfo[1].dwEffectTime+dwValidTime>dwCurrentTime)
					{
						//���ַ���
						ScoreInfo.lScore *= 4;
						_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] ʹ����[ �ı����ֿ� ]���÷ַ��ı���)"),pIServerUserItem->GetNickName());
					}
					else
					{
						pUserProperty->wPropertyUseMark&=~PT_USE_MARK_FOURE_SCORE;
					}
				} //˫������
				else if ((pUserProperty->wPropertyUseMark&PT_USE_MARK_DOUBLE_SCORE)!=0)
				{
					//��������
					DWORD dwValidTime=pUserProperty->PropertyInfo[0].wPropertyCount*pUserProperty->PropertyInfo[0].dwValidNum;
					if (pUserProperty->PropertyInfo[0].dwEffectTime+dwValidTime>dwCurrentTime)
					{
						//���ַ���
						ScoreInfo.lScore*=2L;
						_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] ʹ����[ ˫�����ֿ� ]���÷ַ�����"), pIServerUserItem->GetNickName());
					}
					else
					{
						pUserProperty->wPropertyUseMark&=~PT_USE_MARK_DOUBLE_SCORE;
					}
				}
			}
			else
			{
				//�����
				if ((pUserProperty->wPropertyUseMark&PT_USE_MARK_POSSESS)!=0)
				{
					//��������
					DWORD dwValidTime=pUserProperty->PropertyInfo[3].wPropertyCount*pUserProperty->PropertyInfo[3].dwValidNum;
					if(pUserProperty->PropertyInfo[3].dwEffectTime+dwValidTime>dwCurrentTime)
					{
						//���ַ���
						ScoreInfo.lScore = 0;
						_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] ʹ����[ ������� ]�����ֲ��䣡"),pIServerUserItem->GetNickName());
					}
					else
					{
						pUserProperty->wPropertyUseMark &= ~PT_USE_MARK_POSSESS;
					}
				}
			}
		}

		//д�����
		DWORD dwWinExperience=ScoreInfo.cbType==SCORE_TYPE_WIN?m_pGameParameter->dwWinExperience:0;
		pIServerUserItem->WriteUserScore(ScoreInfo.lScore,ScoreInfo.lGrade,ScoreInfo.lRevenue,dwUserMemal,ScoreInfo.cbType,dwPlayTimeCount,dwWinExperience);

		//��Ϸ��¼
		if (pIServerUserItem->IsAndroidUser()==false && CServerRule::IsRecordGameScore(m_pGameServiceOption->dwServerRule)==true)
		{
			//��������
			tagGameScoreRecord * pGameScoreRecord=NULL;

			//��ѯ���
			if (m_GameScoreRecordBuffer.GetCount()>0L)
			{
				//��ȡ����
				INT_PTR nCount=m_GameScoreRecordBuffer.GetCount();
				pGameScoreRecord=m_GameScoreRecordBuffer[nCount-1];

				//ɾ������
				m_GameScoreRecordBuffer.RemoveAt(nCount-1);
			}

			//��������
			if (pGameScoreRecord==NULL)
			{
				try
				{
					//��������
					pGameScoreRecord=new tagGameScoreRecord;
					if (pGameScoreRecord==NULL) throw TEXT("��Ϸ��¼���󴴽�ʧ��");
				}
				catch (...)
				{
					ASSERT(FALSE);
				}
			}

			//��¼����
			if (pGameScoreRecord!=NULL)
			{
				//�û���Ϣ
				pGameScoreRecord->wChairID=wChairID;
				pGameScoreRecord->dwUserID=pIServerUserItem->GetUserID();
				pGameScoreRecord->cbAndroid=(pIServerUserItem->IsAndroidUser()?TRUE:FALSE);

				//�û���Ϣ
				pGameScoreRecord->dwDBQuestID=pIServerUserItem->GetDBQuestID();
				pGameScoreRecord->dwInoutIndex=pIServerUserItem->GetInoutIndex();

				//�ɼ���Ϣ
				pGameScoreRecord->lScore=ScoreInfo.lScore;
				pGameScoreRecord->lGrade=ScoreInfo.lGrade;
				pGameScoreRecord->lRevenue=ScoreInfo.lRevenue;

				//������Ϣ
				pGameScoreRecord->dwUserMemal=dwUserMemal;
				pGameScoreRecord->dwPlayTimeCount=dwPlayTimeCount;

				//��������˰
				if(pIServerUserItem->IsAndroidUser())
				{
					pGameScoreRecord->lScore += pGameScoreRecord->lRevenue;
					pGameScoreRecord->lRevenue = 0L;
				}

				//��������
				m_GameScoreRecordActive.Add(pGameScoreRecord);
			}
		}

		//��Ϸ��¼
		if(dwGameMemal != INVALID_DWORD || dwPlayGameTime!=INVALID_DWORD)
		{
			DWORD dwStartGameTime=INVALID_DWORD;
			if(dwPlayGameTime!=INVALID_DWORD)
			{
				dwStartGameTime=(dwCurrentTime-dwPlayGameTime);
			}
			RecordGameScore(true, dwStartGameTime);
		}
	}
	else
	{
		//�뿪�û�
		CTraceService::TraceString(TEXT("ϵͳ��ʱδ֧���뿪�û��Ĳ��ֲ�������"),TraceLevel_Exception);

		return false;
	}

	//�㲥��Ϣ
	if (szMessage[0]!=0)
	{
		//��������
		IServerUserItem * pISendUserItem = NULL;
		WORD wEnumIndex=0;

		//��Ϸ���
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			pISendUserItem=GetTableUserItem(i);
			if(pISendUserItem==NULL) continue;

			//������Ϣ
			SendGameMessage(pISendUserItem, szMessage, SMT_CHAT);
		}

		//�Թ��û�
		do
		{
			pISendUserItem=EnumLookonUserItem(wEnumIndex++);
			if(pISendUserItem!=NULL) 
			{
				//������Ϣ
				SendGameMessage(pISendUserItem, szMessage, SMT_CHAT);
			}
		} while (pISendUserItem!=NULL);
	}

	return true;
}

//д�����
bool CTableFrame::WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount)
{
	printf("CTableFrame::WriteTableScore\n");
	//Ч�����
	ASSERT(wScoreCount==m_wChairCount);
	if (wScoreCount!=m_wChairCount) return false;

	//д�����
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (ScoreInfoArray[i].cbType!=SCORE_TYPE_NULL)
		{
			WriteUserScore(i,ScoreInfoArray[i]);
		}
	}

	return true;
}

//����˰��
SCORE CTableFrame::CalculateRevenue(WORD wChairID, SCORE lScore)
{
	//Ч�����
	ASSERT(wChairID<m_wChairCount);
	if (wChairID>=m_wChairCount) return 0L;

	//����˰��
	if ((m_pGameServiceOption->wRevenueRatio>0)&&(lScore>=REVENUE_BENCHMARK))
	{
		//��ȡ�û�
		ASSERT(GetTableUserItem(wChairID)!=NULL);
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);

		//����˰��
		SCORE lRevenue=lScore*m_pGameServiceOption->wRevenueRatio/REVENUE_DENOMINATOR;

		return lRevenue;
	}

	return 0L;
}

//�����޶�
SCORE CTableFrame::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	//�û�Ч��
	ASSERT(pIServerUserItem->GetTableID()==m_wTableID);
	if (pIServerUserItem->GetTableID()!=m_wTableID) return 0L;

	//��ѯ���
	SCORE lTrusteeScore=pIServerUserItem->GetTrusteeScore();
	SCORE lMinEnterScore=m_pGameServiceOption->lMinTableScore;
	SCORE lUserConsumeQuota=m_pITableFrameSink->QueryConsumeQuota(pIServerUserItem);

	//Ч����
	ASSERT((lUserConsumeQuota>=0L)&&(lUserConsumeQuota<=pIServerUserItem->GetUserScore()-lMinEnterScore));
	if ((lUserConsumeQuota<0L)||(lUserConsumeQuota>pIServerUserItem->GetUserScore()-lMinEnterScore)) return 0L;

	return lUserConsumeQuota+lTrusteeScore;
}

//Ѱ���û�
IServerUserItem * CTableFrame::SearchUserItem(DWORD dwUserID)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//�����û�
	for (WORD i=0;i<m_wChairCount;i++)
	{
		pIServerUserItem=GetTableUserItem(i);
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserID()==dwUserID)) return pIServerUserItem;
	}

	//�Թ��û�
	do
	{
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserID()==dwUserID)) return pIServerUserItem;
	} while (pIServerUserItem!=NULL);

	return NULL;
}

//��Ϸ�û�
IServerUserItem * CTableFrame::GetTableUserItem(WORD wChairID)
{
	//Ч�����
	ASSERT(wChairID<m_wChairCount);
	if (wChairID>=m_wChairCount) return NULL;

	//��ȡ�û�
	return m_TableUserItemArray[wChairID];
}

//�Թ��û�
IServerUserItem * CTableFrame::EnumLookonUserItem(WORD wEnumIndex)
{
	if (wEnumIndex>=m_LookonUserItemArray.GetCount()) return NULL;
	return m_LookonUserItemArray[wEnumIndex];
}

//����ʱ��
bool CTableFrame::SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)
{
	//Ч�����
	ASSERT((dwTimerID>0)&&(dwTimerID<TIME_TABLE_MODULE_RANGE));
	if ((dwTimerID<=0)||(dwTimerID>=TIME_TABLE_MODULE_RANGE)) return false;

	//����ʱ��
	DWORD dwEngineTimerID=IDI_TABLE_MODULE_START+m_wTableID*TIME_TABLE_MODULE_RANGE;
	if (m_pITimerEngine!=NULL) m_pITimerEngine->SetTimer(dwEngineTimerID+dwTimerID,dwElapse,dwRepeat,dwBindParameter);

	return true;
}

//ɾ��ʱ��
bool CTableFrame::KillGameTimer(DWORD dwTimerID)
{
	//Ч�����
	ASSERT((dwTimerID>0)&&(dwTimerID<=TIME_TABLE_MODULE_RANGE));
	if ((dwTimerID<=0)||(dwTimerID>TIME_TABLE_MODULE_RANGE)) return false;

	//ɾ��ʱ��
	DWORD dwEngineTimerID=IDI_TABLE_MODULE_START+m_wTableID*TIME_TABLE_MODULE_RANGE;
	if (m_pITimerEngine!=NULL) m_pITimerEngine->KillTimer(dwEngineTimerID+dwTimerID);

	return true;
}

//��������
bool CTableFrame::SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)
{
	//���⴦��
	if (pIServerUserItem==NULL)
	{
		return m_pIMainServiceFrame->SendData(BG_ALL_CLIENT,MDM_GF_GAME,wSubCmdID,NULL,0);
	}

	//״̬Ч��
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)&&(pIServerUserItem->IsClientReady()==false)) return false;

	//��������
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);

	return true;
}

//��������
bool CTableFrame::SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//���⴦��
	if (pIServerUserItem==NULL)
	{
		return m_pIMainServiceFrame->SendData(BG_ALL_CLIENT,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
	}

	//״̬Ч��
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)&&(pIServerUserItem->IsClientReady()==false)) return false;

	//��������
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);

	return true;
}

//��������
bool CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID)
{
	//�û�Ⱥ��
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;

			//Ч��״̬
			ASSERT(pIServerUserItem->IsClientReady()==true);
			if (pIServerUserItem->IsClientReady()==false) continue;

			//��������
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);
		}

		return true;
	}
	else
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) return false;

		//Ч��״̬
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//��������
		m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);

		return true;
	}

	return false;
}

//��������
bool CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize,WORD wMainCmdID)
{
	//�û�Ⱥ��
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);
			if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

			//��������
			m_pIMainServiceFrame->SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
		}

		return true;
	}
	else
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
		if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) return false;

		//��������
		m_pIMainServiceFrame->SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);

		return true;
	}

	return false;
}

//��������
bool CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//ö���û�
	do
	{
		//��ȡ�û�
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//Ч��״̬
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//��������
		if ((wChairID==INVALID_CHAIR)||(pIServerUserItem->GetChairID()==wChairID))
		{
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);
		}

	} while (true);

	return true;
}

//��������
bool CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//ö���û�
	do
	{
		//��ȡ�û�
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//Ч��״̬
		//ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//��������
		if ((wChairID==INVALID_CHAIR)||(pIServerUserItem->GetChairID()==wChairID))
		{
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
		}

	} while (true);

	return true;
}

//������Ϣ
bool CTableFrame::SendGameMessage(LPCTSTR lpszMessage, WORD wType)
{
	//��������
	WORD wEnumIndex=0;

	//������Ϣ
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(i);
		if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

		//������Ϣ
		m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);
	}

	//ö���û�
	do
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//Ч��״̬
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//������Ϣ
		m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);

	} while (true);

	return true;
}

//������Ϣ
bool CTableFrame::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//���⴦��
	if (pIServerUserItem==NULL)
	{
		return m_pIMainServiceFrame->SendRoomMessage(lpszMessage,wType);
	}

	//�û�Ч��
	/*ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;*/

	//������Ϣ
	m_pIMainServiceFrame->SendRoomMessage(pIServerUserItem,lpszMessage,wType);

	return true;
}

//��Ϸ��Ϣ
bool CTableFrame::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//������Ϣ
	return m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);
}

//���ͳ���
bool CTableFrame::SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	//�û�Ч��
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) return false;

	//���ͳ���
	ASSERT(m_pIMainServiceFrame!=NULL);
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_GAME_SCENE,pData,wDataSize);

	return true;
}

//���ýӿ�
bool CTableFrame::SetTableFrameHook(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	if (pIUnknownEx==NULL) return false;

	//�����ж�
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0) return false;

	//��ѯ�ӿ�
	m_pITableFrameHook=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameHook);
	m_pIMatchTableAction=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableUserAction);

	return true;
}

//α������
bool CTableFrame::ImitateGameOption(IServerUserItem * pIServerUserItem)
{
	//����У��
	ASSERT(pIServerUserItem!=NULL);	
	if (pIServerUserItem==NULL) return false;

	//״̬�ж�
	if (m_bGameStarted==true) return true;
	if (pIServerUserItem->GetSignUpStatus()!=MUS_MATCHING) return false;

	//�����ж�
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0) return false;	
	
	//����״̬
	pIServerUserItem->SetClientReady(true);

	//��ʼ�ж�
	if ((pIServerUserItem->GetUserStatus()==US_READY)&&(EfficacyStartGame(pIServerUserItem->GetChairID())==true))
	{
		StartGame(); 
	}

	return true;
}

//�����¼�
bool CTableFrame::OnEventUserOffLine(IServerUserItem * pIServerUserItem)
{
	//����Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�û�����
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	IServerUserItem * pITableUserItem=m_TableUserItemArray[pUserInfo->wChairID];

	//�û�����
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//��Ϸ�û�
	if (cbUserStatus!=US_LOOKON)
	{
		//Ч���û�
		ASSERT(pIServerUserItem==GetTableUserItem(wChairID));
		if (pIServerUserItem!=GetTableUserItem(wChairID)) return false;

		//���ߴ���
		//if ((cbUserStatus==US_PLAYING))
		//{
			//�û�����
			//pIServerUserItem->SetClientReady(false);
			pIServerUserItem->SetUserStatus(US_OFFLINE,m_wTableID,wChairID);

			//����֪ͨ
			if(m_pITableUserAction!=NULL) m_pITableUserAction->OnActionUserOffLine(wChairID,pIServerUserItem);

			if(GetSitUserCount() == 1 && wChairID == 0)
			{
				SetGameTimer(IDI_OFF_LINE,TIME_OFF_LINE,1,wChairID);
			}

			//return true;
			
			//���ߴ���
 			//if (m_dwOffLineTime[wChairID]==0L)
 			//{
 			//	//���ñ���
 			//	m_wOffLineCount[wChairID]++;
 			//	m_dwOffLineTime[wChairID]=(DWORD)time(NULL);
 
 			//	//ʱ������
 			//	WORD wOffLineCount=GetOffLineUserCount();
 			//	if (wOffLineCount==1) SetGameTimer(IDI_OFF_LINE,TIME_OFF_LINE,1,wChairID);
 			//}

			return true;
		//}
	}

	//�û�����
	PerformStandUpAction(pIServerUserItem);
	
	if (m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
	{
		if (pIServerUserItem->IsTrusteeUser()==true) return true;
	}

	//ɾ���û�
	ASSERT(pIServerUserItem->GetUserStatus()==US_FREE);

	pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);

	return true;
}

//�����¼�
bool CTableFrame::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//֪ͨ��Ϸ
	return m_pITableFrameSink->OnUserScroeNotify(wChairID,pIServerUserItem,cbReason);
}

//ʱ���¼�
bool CTableFrame::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//�ص��¼�
	if ((dwTimerID>=0)&&(dwTimerID<TIME_TABLE_SINK_RANGE))
	{
		ASSERT(m_pITableFrameSink!=NULL);
		return m_pITableFrameSink->OnTimerMessage(dwTimerID,dwBindParameter);
	}

	//�¼�����
	switch (dwTimerID)
	{
	case IDI_OFF_LINE:	//�����¼�
		{
			IServerUserItem * pIServerUserItem=GetTableUserItem(0);
			if(pIServerUserItem->GetUserStatus() == US_OFFLINE)
				PerformStandUpAction(pIServerUserItem);

			return true;
		}

	case IDI_WAIT_DISMISS:
		{
			KillGameTimer(IDI_WAIT_DISMISS);
			for (int i=0; i<m_wChairCount; i++)
			{
				IServerUserItem *pTableUserItem = GetTableUserItem(i);
				if(pTableUserItem == NULL) continue;
				if(m_bResponseDismiss[i]) continue;
				OnEventDismissRoom(i,true);
			}

			return true;
		}
	case IDI_WAIT_CONTINUE:
		{
			KillGameTimer(IDI_WAIT_CONTINUE);

			ConcludeGame(0xFF);

			return true;
		}

	default:break;
	}

	//δ��ʼ��ʱ
	if(dwTimerID >= IDI_START_OVERTIME && dwTimerID <=IDI_START_OVERTIME_END)
	{
		//��������
		WORD wChair=(WORD)dwBindParameter;

		//
		if(wChair != (WORD)(dwTimerID-IDI_START_OVERTIME)) return false;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChair);

		//��ʱ����
		if(pIServerUserItem && pIServerUserItem->GetUserStatus()==US_SIT)
		{
			//�û�����
			PerformStandUpAction(pIServerUserItem);
		}

		return false;
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��Ϸ�¼�
bool CTableFrame::OnEventSocketGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(m_pITableFrameSink!=NULL);

	//��Ϣ����
	return m_pITableFrameSink->OnGameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//����¼�
bool CTableFrame::OnEventSocketFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//��Ϸ����
	if (m_pITableFrameSink->OnFrameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem)==true) return true;

	//Ĭ�ϴ���
	switch (wSubCmdID)
	{
	case SUB_GF_GAME_OPTION:	//��Ϸ����
		{
			//Ч�����
 			ASSERT(wDataSize==sizeof(CMD_GF_GameOption));
			if (wDataSize!=sizeof(CMD_GF_GameOption)) return false;

			//��ȡ����
			WORD wChairID=pIServerUserItem->GetChairID();
			BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

			//δ���ѷ���
			if(!m_bContinue[wChairID]) return true;

			//��������
			CMD_GF_GameOption * pGameOption=(CMD_GF_GameOption *)pData;

			if(pGameOption->cbOptionType[enOption_FangZhu] == 1)
			{
				ZeroMemory(m_szHeadUrl,sizeof(m_szHeadUrl));
				memset(&m_nFangkaGameOption,0,sizeof(m_nFangkaGameOption));

				if(pGameOption->cbOptionType[enOption_PlayerCount]>1)
				{
					m_wChairCount = pGameOption->cbOptionType[enOption_PlayerCount];
				}
				printf("����������� %d\n",m_wChairCount);

				for (int i=0; i<20; i++)
				{
					if(pGameOption->cbOptionType[i] == 0) continue;
					m_nFangkaGameOption.cbGameOptionType[i] = pGameOption->cbOptionType[i];
				}		
			}		

			//��������
			if ((cbUserStatus!=US_LOOKON)&&((m_dwOffLineTime[wChairID]!=0L)))
			{
				//���ñ���
				m_dwOffLineTime[wChairID]=0L;

				//ɾ��ʱ��
				WORD wOffLineCount=GetOffLineUserCount();
				if (wOffLineCount==0) KillGameTimer(IDI_OFF_LINE);
			}

			//����״̬
			pIServerUserItem->SetClientReady(true);
			if (cbUserStatus!=US_LOOKON) m_bAllowLookon[wChairID]=pGameOption->cbAllowLookon?true:false;

			pIServerUserItem->SetUserStatus(cbUserStatus,m_wTableID,wChairID);

			for (int i=0; i<m_wChairCount; i++)
			{
				if(wChairID == i) continue;

				IServerUserItem *pUserItem = GetTableUserItem(i);
				if(pUserItem==NULL) continue;

				CMD_GR_UserStatus UserStatus;
				ZeroMemory(&UserStatus,sizeof(UserStatus));

				//��������
				UserStatus.dwUserID=pUserItem->GetUserID();
				CopyMemory(UserStatus.szSendNickName,pUserItem->GetNickName(),sizeof(UserStatus.szSendNickName));
				UserStatus.UserStatus.wTableID=pUserItem->GetTableID();
				UserStatus.UserStatus.wChairID=pUserItem->GetChairID();
				UserStatus.UserStatus.cbUserStatus=pUserItem->GetUserStatus();

				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
			}

			//����״̬
			CMD_GF_GameStatus GameStatus;
			GameStatus.cbGameStatus=m_cbGameStatus;
			GameStatus.cbAllowLookon=m_bAllowLookon[wChairID]?TRUE:FALSE;
			GameStatus.wFangzhu = m_wFangzhu;
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_GAME_STATUS,&GameStatus,sizeof(GameStatus));

			//������Ϣ
			//TCHAR szMessage[128]=TEXT("");
			//_sntprintf(szMessage,CountArray(szMessage),TEXT("��ӭ�����롰%s����Ϸ��ף����Ϸ��죡"),m_pGameServiceAttrib->szGameName);
			//m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_CHAT);

			//���ͳ���
			bool bSendSecret=((cbUserStatus!=US_LOOKON)||(m_bAllowLookon[wChairID]==true));

			m_pITableFrameSink->OnEventSendGameScene(wChairID,pIServerUserItem,m_cbGameStatus,bSendSecret);
					
			CMD_GF_GameOption nGameOption;
			memset(&nGameOption,0,sizeof(nGameOption));

			for (int i=0; i<20; i++)
			{
				nGameOption.cbOptionType[i] = m_nFangkaGameOption.cbGameOptionType[i];
			}		
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_GAME_OPTION,&nGameOption,sizeof(nGameOption));
				

			//ȡ���й�
			if(m_pGameServiceOption->wServerType==GAME_GENRE_MATCH && m_pGameMatchOption->cbMatchType==MATCH_TYPE_IMMEDIATE)
			{
				if(pIServerUserItem->IsTrusteeUser()==true)
				{
					//�����й�
					pIServerUserItem->SetTrusteeUser(false);
				}

				//�ع�֪ͨ
				if (cbUserStatus==US_PLAYING || cbUserStatus==US_OFFLINE)
				{
					if(m_pITableUserAction!=NULL) m_pITableUserAction->OnActionUserConnect(wChairID,pIServerUserItem);
					if(m_pITableFrameHook) m_pITableFrameHook->OnEventUserReturnMatch(pIServerUserItem);
				}
			}

			//��ʼ�ж�
			if ((cbUserStatus==US_READY)&&(EfficacyStartGame(wChairID)==true))
			{
				printf("�������ÿ�ʼ��Ϸ\n");
				StartGame(); 
			}

			return true;
		}
	case SUB_GF_USER_READY:		//�û�׼��
		{
			printf("���׼�� %d\n",pIServerUserItem->GetChairID());
			//��ȡ����
			WORD wChairID=pIServerUserItem->GetChairID();

			BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

			//Ч��״̬
			ASSERT(GetTableUserItem(wChairID)==pIServerUserItem);
			if (GetTableUserItem(wChairID)!=pIServerUserItem)
			{
				return false;
			}

			//Ч��״̬
			//ASSERT(cbUserStatus==US_SIT);
			//if (cbUserStatus!=US_SIT)
			//{
			//	printf("״̬����\n");
			//	return true;
			//}

			//�����׷����ж�
			//if((CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR))
			//	&& (m_wDrawCount >= m_pGameServiceOption->wDistributeDrawCount || CheckDistribute()))
			//{
			//	//������Ϣ
			//	LPCTSTR pszMessage=TEXT("ϵͳ���·������ӣ����Ժ�");
			//	SendGameMessage(pIServerUserItem,pszMessage,SMT_CHAT);

			//	//������Ϣ
			//	m_pIMainServiceFrame->InsertDistribute(pIServerUserItem);

			//	////�û�����
			//	//PerformStandUpAction(pIServerUserItem);

			//	return true;
			//}

			//�¼�֪ͨ
			/*if(m_pIMatchTableAction!=NULL && !m_pIMatchTableAction->OnActionUserOnReady(wChairID,pIServerUserItem, pData,wDataSize))
			{
				return true;
			}*/

			//�¼�֪ͨ
			if (m_pITableUserAction!=NULL)
			{
				m_pITableUserAction->OnActionUserOnReady(wChairID,pIServerUserItem,pData,wDataSize);
			}

			//ɾ����ʱ
			if(m_pGameServiceAttrib->wChairCount < MAX_CHAIR) KillGameTimer(IDI_START_OVERTIME+wChairID);

			//��ʼ�ж�
			if (EfficacyStartGame(wChairID)==false)
			{
				//����״̬
				pIServerUserItem->SetUserStatus(US_READY,m_wTableID,wChairID);
			}
			else
			{
				printf("����׼����ʼ��Ϸ\n");
				StartGame(); 
			}

			return true;
		}
	case SUB_GF_HEADURL:		//ͷ���ַ
		{
			printf("ת��ͷ���ַ\n");
			WORD wChairID=pIServerUserItem->GetChairID();

			CMD_GF_C_HeadUrl * pHead=(CMD_GF_C_HeadUrl *)pData;
	
			memcpy(m_szHeadUrl[wChairID],pHead->szHeadUrl,lstrlen(pHead->szHeadUrl));

			CTraceService::TraceString(pHead->szHeadUrl,TraceLevel_Info);
			CTraceService::TraceString(m_szHeadUrl[wChairID],TraceLevel_Info);

			//����ͷ���������
			CMD_GF_S_HeadUrl nHead;
			nHead.wChairID = wChairID;
			ZeroMemory(nHead.szHeadUrl,sizeof(nHead.szHeadUrl));
			memcpy(nHead.szHeadUrl,pHead->szHeadUrl,lstrlen(pHead->szHeadUrl));
		
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pUserItem=GetTableUserItem(i);
				if ((pUserItem==NULL)) continue;
				if(i == wChairID) continue;

				m_pIMainServiceFrame->SendData(pUserItem,MDM_GF_FRAME,SUB_GF_HEAD,&nHead,sizeof(CMD_GF_S_HeadUrl));
			}

			//����������ͷ����Լ�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pUserItem=GetTableUserItem(i);
				if ((pUserItem==NULL)) continue;
				if(i == wChairID) continue;

				nHead.wChairID = i;
				ZeroMemory(nHead.szHeadUrl,sizeof(nHead.szHeadUrl));
				memcpy(nHead.szHeadUrl,m_szHeadUrl[i],lstrlen(m_szHeadUrl[i]));

				printf("��� %d ͷ���ַ���� %d\n",i,lstrlen(m_szHeadUrl[i]));

				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_HEAD,&nHead,sizeof(CMD_GF_S_HeadUrl));
			}


			printf("ת�����綨λ\n");
			//����������λ�ø��Լ�
			for (int i=0; i<m_wChairCount; i++)
			{
				if(wChairID == i) continue;

				IServerUserItem *pUserItem = GetTableUserItem(i);
				if(pUserItem==NULL) continue;

				CMD_GR_Position UserPosition;
				ZeroMemory(&UserPosition,sizeof(UserPosition));

				//��������
				UserPosition.dwUserID = pUserItem->GetUserID();
				UserPosition.dLongitude=m_dLongitude[i];
				UserPosition.dLatitude=m_dLatitude[i];

				printf("ת�����綨λ %d -> %d\n",i,pIServerUserItem->GetChairID());

				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_POSITION,&UserPosition,sizeof(CMD_GR_Position));
			}

			//����λ�ø�������
			CMD_GR_Position myPosition;
			myPosition.dwUserID = pIServerUserItem->GetUserID();
			myPosition.dLongitude = m_dLongitude[wChairID];
			myPosition.dLatitude = m_dLatitude[wChairID];

			for (WORD i=0;i<m_wChairCount;i++)
			{
				if(wChairID == i) continue;

				//��ȡ�û�
				IServerUserItem * pUserItem=GetTableUserItem(i);
				if ((pUserItem==NULL)) continue;

				printf("ת�����綨λ %d -> %d\n",pIServerUserItem->GetChairID(),i);

				m_pIMainServiceFrame->SendData(pUserItem,MDM_GF_FRAME,SUB_GR_POSITION,&myPosition,sizeof(CMD_GR_Position));
			}
				
			printf("�������!\n");
			
			return true;
		}
	case SUB_GF_USER_CHAT:		//�û�����
		{
			//��������
			CMD_GF_C_UserChat * pUserChat=(CMD_GF_C_UserChat *)pData;

			//������Ϣ
			CMD_GF_S_UserChat UserChat;
			ZeroMemory(&UserChat,sizeof(UserChat));

			//�ַ�����
			m_pIMainServiceFrame->SensitiveWordFilter(pUserChat->szChatString,UserChat.szChatString,CountArray(UserChat.szChatString));

			//��������
			UserChat.dwSendUserID=pIServerUserItem->GetUserID();

			//��Ϸ�û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if (pIServerUserItem==NULL) continue;

				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,sizeof(CMD_GF_S_UserChat));
			}

			return true;
		}
	case SUB_GF_USER_EXPRESSION:	//�û�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GF_C_UserExpression));
			if (wDataSize!=sizeof(CMD_GF_C_UserExpression)) return false;

			//��������
			CMD_GF_C_UserExpression * pUserExpression=(CMD_GF_C_UserExpression *)pData;

// 			//������Ϣ
// 			CMD_GR_S_UserExpression UserExpression;
// 			ZeroMemory(&UserExpression,sizeof(UserExpression));
// 
// 			//��������
// 			UserExpression.wItemIndex=pUserExpression->wItemIndex;
// 			UserExpression.dwSendUserID=pIServerUserItem->GetUserID();

			//������Ϣ
 			CMD_GF_S_UserExpression UserExpression;
 			ZeroMemory(&UserExpression,sizeof(UserExpression));
 
 			//��������
 			UserExpression.wItemIndex=pUserExpression->wItemIndex;
 			UserExpression.dwSendUserID=pIServerUserItem->GetUserID();


			//��Ϸ�û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)) continue;

				//��������
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));
			}

			return true;
		}
	case SUB_GF_LOOKON_CONFIG:		//�Թ�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GF_LookonConfig));
			if (wDataSize<sizeof(CMD_GF_LookonConfig)) return false;

			//��������
			CMD_GF_LookonConfig * pLookonConfig=(CMD_GF_LookonConfig *)pData;

			//Ŀ���û�
			if ((pLookonConfig->dwUserID!=0)&&(SearchUserItem(pLookonConfig->dwUserID)==NULL))
			{
				ASSERT(FALSE);
				return true;
			}

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()!=US_LOOKON);
			if (pIServerUserItem->GetUserStatus()==US_LOOKON) return false;

			//�Թ۴���
			if (pLookonConfig->dwUserID!=0L)
			{
				for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
				{
					//��ȡ�û�
					IServerUserItem * pILookonUserItem=m_LookonUserItemArray[i];
					if (pILookonUserItem->GetUserID()!=pLookonConfig->dwUserID) continue;
					if (pILookonUserItem->GetChairID()!=pIServerUserItem->GetChairID()) continue;

					//������Ϣ
					CMD_GF_LookonStatus LookonStatus;
					LookonStatus.cbAllowLookon=pLookonConfig->cbAllowLookon;

					//������Ϣ
					ASSERT(m_pIMainServiceFrame!=NULL);
					m_pIMainServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_STATUS,&LookonStatus,sizeof(LookonStatus));

					break;
				}
			}
			else
			{
				//�����ж�
				bool bAllowLookon=(pLookonConfig->cbAllowLookon==TRUE)?true:false;
				if (bAllowLookon==m_bAllowLookon[pIServerUserItem->GetChairID()]) return true;

				//���ñ���
				m_bAllowLookon[pIServerUserItem->GetChairID()]=bAllowLookon;

				//������Ϣ
				CMD_GF_LookonStatus LookonStatus;
				LookonStatus.cbAllowLookon=pLookonConfig->cbAllowLookon;

				//������Ϣ
				for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
				{
					//��ȡ�û�
					IServerUserItem * pILookonUserItem=m_LookonUserItemArray[i];
					if (pILookonUserItem->GetChairID()!=pIServerUserItem->GetChairID()) continue;

					//������Ϣ
					ASSERT(m_pIMainServiceFrame!=NULL);
					m_pIMainServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_STATUS,&LookonStatus,sizeof(LookonStatus));
				}
			}

			return true;
		}
	case 15:
		{
			//��Ϸ�û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)) continue;

				//��������
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,15,pData,wDataSize);
			}
			return true;
		}
	}

	return false;
}

//��Ϸ����
WORD CTableFrame::GetDrawCount()
{
	return m_wDrawCount;
}

//��ȡ��λ
WORD CTableFrame::GetNullChairID()
{
	//��������
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_TableUserItemArray[i]==NULL)
		{
			return i;
		}
	}

	return INVALID_CHAIR;
}

//�����λ
WORD CTableFrame::GetRandNullChairID()
{
	//��������
	WORD wIndex = rand()%m_wChairCount;
	for (WORD i=wIndex;i<m_wChairCount+wIndex;i++)
	{
		if (m_TableUserItemArray[i%m_wChairCount]==NULL)
		{
			return i%m_wChairCount;
		}
	}

	return INVALID_CHAIR;
}

//�û���Ŀ
WORD CTableFrame::GetSitUserCount()
{
	//��������
	WORD wUserCount=0;

	//��Ŀͳ��
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (GetTableUserItem(i)!=NULL)
		{
			wUserCount++;
		}
	}

	return wUserCount;
}

//�Թ���Ŀ
WORD CTableFrame::GetLookonUserCount()
{
	//��ȡ��Ŀ
	INT_PTR nLookonCount=m_LookonUserItemArray.GetCount();

	return (WORD)(nLookonCount);
}

//������Ŀ
WORD CTableFrame::GetOffLineUserCount()
{
	//��������
	WORD wOffLineCount=0;

	//��������
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_dwOffLineTime[i]!=0L)
		{
			wOffLineCount++;
		}
	}

	return wOffLineCount;
}

bool CTableFrame::OnEventDismissRoom(WORD wChairID,bool bAgree)
{
	printf("��� %d ��ɢ��Ϣ %d\n",wChairID,bAgree);

	if(!m_bAgree[wChairID]) m_bAgree[wChairID]=bAgree;
	if(m_bResponseDismiss[wChairID])
	{
		return true;
	}
	m_bResponseDismiss[wChairID] = true;

	if(m_bFangkaGameStarted)
	{
		bool bAllAgree = true;
		bool bResponse = true;
		int nResponse = 0;
		for (int i=0; i<m_wChairCount; i++)
		{
			IServerUserItem *pTableUserItem = GetTableUserItem(i);
			if(pTableUserItem == NULL) continue;

			if(!m_bAgree[i]) bAllAgree=false;
			if(!m_bResponseDismiss[i]) bResponse=false;
			else nResponse++;
		}

		if(nResponse == 1)
		{
			SetGameTimer(IDI_WAIT_DISMISS,TIME_WAIT_DISMISS,1,NULL);
		}

		if(bResponse)
		{
			printf("ȫ����Ӧ��\n");
			ZeroMemory(m_bResponseDismiss,sizeof(m_bResponseDismiss));
			ZeroMemory(m_bAgree,sizeof(m_bAgree));
			KillGameTimer(IDI_WAIT_DISMISS);

			if(bAllAgree)
			{
				printf("ȫ��ͬ��\n");

				SendTableData(INVALID_CHAIR,SUB_GR_DISMISS_SUCCESS,NULL,0,MDM_GR_USER);
				//����������ͬ���ɢ����
				for (int i=0; i<m_wChairCount; i++)
				{
					IServerUserItem *pTableUserItem = GetTableUserItem(i);
					if(pTableUserItem == NULL) continue;
					PerformStandUpAction(pTableUserItem);
				}

				ConcludeGame(0xFE);

				return true;
			}
			else
			{
				printf("���˾ܾ�\n");

				//���ͽ�ɢʧ��
				for (int i=0; i<m_wChairCount; i++)
				{
					IServerUserItem *pTableUserItem = GetTableUserItem(i);
					if(pTableUserItem == NULL) continue;
					m_pIMainServiceFrame->SendData(pTableUserItem,MDM_GR_USER,SUB_GR_DISMISS_FAIL,NULL,0);
				}
			}
		}
		else
		{
			CMD_GR_DismissRoom nDismissRoom;
			nDismissRoom.bAgree = bAgree;
			nDismissRoom.wUserID = GetTableUserItem(wChairID)->GetUserID();
			for (int i=0; i<m_wChairCount; i++)
			{
				IServerUserItem *pTableUserItem = GetTableUserItem(i);
				if(pTableUserItem == NULL) continue;
				m_pIMainServiceFrame->SendData(GetTableUserItem(i),MDM_GR_USER,SUB_GR_DISMISS_MSG,&nDismissRoom,sizeof(CMD_GR_DismissRoom));
			}
		}
	}
	else
	{
		printf("��Ϸδ��ʼ\n");
		ZeroMemory(m_bResponseDismiss,sizeof(m_bResponseDismiss));
		ZeroMemory(m_bAgree,sizeof(m_bAgree));

		if(wChairID == m_wFangzhu)
		{
			printf("������ɢ\n");
			SendTableData(INVALID_CHAIR,SUB_GR_FANGZHU_DISMISS,NULL,0,MDM_GR_USER);
			//���ͷ�����ɢ����
			for (int i=0; i<m_wChairCount; i++)
			{
				IServerUserItem *pTableUserItem = GetTableUserItem(i);
				if(pTableUserItem == NULL) continue;
				PerformStandUpAction(pTableUserItem);
			}

			ConcludeGame(0xFE);

			return true;
		}
	}
	return false;
}

//����״��
WORD CTableFrame::GetTableUserInfo(tagTableUserInfo & TableUserInfo)
{
	//���ñ���
	ZeroMemory(&TableUserInfo,sizeof(TableUserInfo));

	//�û�����
	for (WORD i=0;i<m_pGameServiceAttrib->wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//�û�����
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			TableUserInfo.wTableUserCount++;
		}
		else
		{
			TableUserInfo.wTableAndroidCount++;
		}

		//׼���ж�
		if (pIServerUserItem->GetUserStatus()==US_READY)
		{
			TableUserInfo.wTableReadyCount++;
		}
	}

	//������Ŀ
	switch (m_cbStartMode)
	{
	case START_MODE_ALL_READY:		//����׼��
		{
			TableUserInfo.wMinUserCount=2;
			break;
		}
	case START_MODE_PAIR_READY:		//��Կ�ʼ
		{
			TableUserInfo.wMinUserCount=2;
			break;
		}
	case START_MODE_TIME_CONTROL:	//ʱ�����
		{
			TableUserInfo.wMinUserCount=1;
			break;
		}
	default:						//Ĭ��ģʽ
		{
			TableUserInfo.wMinUserCount=m_pGameServiceAttrib->wChairCount;
			break;
		}
	}

	return TableUserInfo.wTableAndroidCount+TableUserInfo.wTableUserCount;
}

//��������
bool CTableFrame::InitializationFrame(WORD wTableID, tagTableFrameParameter & TableFrameParameter)
{
	//���ñ���
	m_wTableID=wTableID;
	m_wChairCount=TableFrameParameter.pGameServiceAttrib->wChairCount;

	//���ò���
	m_pGameParameter=TableFrameParameter.pGameParameter;
	m_pGameMatchOption=TableFrameParameter.pGameMatchOption;
	m_pGameServiceAttrib=TableFrameParameter.pGameServiceAttrib;
	m_pGameServiceOption=TableFrameParameter.pGameServiceOption;

	//����ӿ�
	m_pITimerEngine=TableFrameParameter.pITimerEngine;
	m_pIMainServiceFrame=TableFrameParameter.pIMainServiceFrame;
	m_pIAndroidUserManager=TableFrameParameter.pIAndroidUserManager;
	m_pIKernelDataBaseEngine=TableFrameParameter.pIKernelDataBaseEngine;
	m_pIRecordDataBaseEngine=TableFrameParameter.pIRecordDataBaseEngine;

	//��������
	IGameServiceManager * pIGameServiceManager=TableFrameParameter.pIGameServiceManager;
	m_pITableFrameSink=(ITableFrameSink *)pIGameServiceManager->CreateTableFrameSink(IID_ITableFrameSink,VER_ITableFrameSink);

	//�����ж�
	if (m_pITableFrameSink==NULL)
	{
		ASSERT(FALSE);
		return false;
	}
	
	//���ñ���
	m_lCellScore=m_pGameServiceOption->lCellScore;

	//��������
	IUnknownEx * pITableFrame=QUERY_ME_INTERFACE(IUnknownEx);
	if (m_pITableFrameSink->Initialization(pITableFrame)==false) return false;

	//���ñ�ʶ
	m_bTableInitFinish=true;

	//��չ�ӿ�
	m_pITableUserAction=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrameSink,ITableUserAction);
	m_pITableUserRequest=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrameSink,ITableUserRequest);

	return true;
}

//��������
bool CTableFrame::PerformStandUpAction(IServerUserItem * pIServerUserItem, bool bInitiative/*=false*/)
{
	printf("��������\n");
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->GetTableID()==m_wTableID);
	ASSERT(pIServerUserItem->GetChairID()<=m_wChairCount);

	if(GetSitUserCount()<=1)
		SetPassword(0);

	//�û�����
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	printf("�û����� 0\n");
	//��Ϸ�û�
	if ((m_bGameStarted==true)&&((cbUserStatus==US_PLAYING)||(cbUserStatus==US_OFFLINE)))
	{	
		//������Ϸ
		//BYTE cbConcludeReason=(cbUserStatus==US_OFFLINE)?GER_NETWORK_ERROR:GER_USER_LEAVE;
		//m_pITableFrameSink->OnEventGameConclude(wChairID,pIServerUserItem,GER_DISMISS);
		//�뿪�ж�
		//if (m_TableUserItemArray[wChairID]!=pIServerUserItem) return true;
	}

	printf("�û����� 1\n");
	//���ñ���
	if (pIServerUserItem==pITableUserItem)
	{
		//ɾ����ʱ
		if(m_pGameServiceAttrib->wChairCount < MAX_CHAIR) KillGameTimer(IDI_START_OVERTIME+wChairID);

		//���ñ���
		m_TableUserItemArray[wChairID]=NULL;

		printf("�û����� 2\n");
		//������Ϸ��
		if (m_lFrozenedScore[wChairID]!=0L)
		{
			pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[wChairID]);
			m_lFrozenedScore[wChairID]=0L;
		}

		printf("�û����� 11\n");
		//�¼�֪ͨ
		if (m_pITableUserAction!=NULL)
		{
			m_pITableUserAction->OnActionUserStandUp(wChairID,pIServerUserItem,false);
		}

		//�¼�֪ͨ
		if(m_pIMatchTableAction!=NULL) m_pIMatchTableAction->OnActionUserStandUp(wChairID,pIServerUserItem,false);

		//�û�״̬
		BYTE cbMatchStatus = pIServerUserItem->GetSignUpStatus();
		bool bImmediteMatch = (m_pGameServiceOption->wServerType==GAME_GENRE_MATCH && m_pGameMatchOption->cbMatchType==MATCH_TYPE_IMMEDIATE);
		if(cbUserStatus == US_OFFLINE && cbMatchStatus == MUS_MATCHING && bImmediteMatch==true)
		{
			CString str;
			str.Format(TEXT("UserID��%d���������˳������޸�״̬��"),pIServerUserItem->GetUserID());
			CTraceService::TraceString(str,TraceLevel_Info);
		}


		printf("�û����� 22\n");
		//�û�״̬
		pIServerUserItem->SetClientReady(false);
		pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);

		//��������
		bool bTableLocked=IsTableLocked();
		bool bTableStarted=IsTableStarted();
		WORD wTableUserCount=GetSitUserCount();

		//���ñ���
		m_wUserCount=wTableUserCount;

		//������Ϣ
		if (wTableUserCount==0)
		{
			m_dwTableOwnerID=0L;
			m_szEnterPassword[0]=0;
		}

		//�����Թ�
		if (wTableUserCount==0)
		{
			for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
			{
				SendGameMessage(m_LookonUserItemArray[i],TEXT("����Ϸ������������Ѿ��뿪�ˣ�"),SMT_CLOSE_GAME|SMT_EJECT);
			}
		}

		printf("�û����� 33\n");
		//��������
		if(wTableUserCount == 0)
		{
			ConcludeTable();
		}

		printf("�û����� 44\n");
		//��ʼ�ж�
		if (EfficacyStartGame(INVALID_CHAIR)==true)
		{
			printf("�û�������ʼ��Ϸ\n");
			StartGame();
		}

		printf("�û����� 55\n");
		//����״̬
		if ((bTableLocked!=IsTableLocked())||(bTableStarted!=IsTableStarted()))
		{
			SendTableStatus();
		}

		printf("�û����� 66\n");
		return true;
	}
	else
	{
		//��������
		for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
		{
			if (pIServerUserItem==m_LookonUserItemArray[i])
			{
				//ɾ������
				m_LookonUserItemArray.RemoveAt(i);

				//�¼�֪ͨ
				if (m_pITableUserAction!=NULL)
				{
					m_pITableUserAction->OnActionUserStandUp(wChairID,pIServerUserItem,true);
				}

				//�¼�֪ͨ
				if(m_pIMatchTableAction!=NULL) m_pIMatchTableAction->OnActionUserStandUp(wChairID,pIServerUserItem,true);

				//�û�״̬
				pIServerUserItem->SetClientReady(false);
				pIServerUserItem->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR);

				return true;
			}
		}

		//�������
		ASSERT(FALSE);
	}

	printf("��������  33\n");
	return true;
}

//�Թ۶���
bool CTableFrame::PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT((pIServerUserItem!=NULL)&&(wChairID<m_wChairCount));
	ASSERT((pIServerUserItem->GetTableID()==INVALID_TABLE)&&(pIServerUserItem->GetChairID()==INVALID_CHAIR));

	//��������
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagUserRule * pUserRule=pIServerUserItem->GetUserRule();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//��Ϸ״̬
	if ((m_bGameStarted==false)&&(pIServerUserItem->GetMasterOrder()==0L))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ϸ��û�п�ʼ�������Թ۴���Ϸ����"),REQUEST_FAILURE_NORMAL);
		return false;
	}

	//ģ�⴦��
	if (m_pGameServiceAttrib->wChairCount < MAX_CHAIR && pIServerUserItem->IsAndroidUser()==false)
	{
		//�������
		CAttemperEngineSink * pAttemperEngineSink=(CAttemperEngineSink *)m_pIMainServiceFrame;

		//���һ���
		for (WORD i=0; i<m_pGameServiceAttrib->wChairCount; i++)
		{
			//��ȡ�û�
			IServerUserItem *pIUserItem=m_TableUserItemArray[i];
			if(pIUserItem==NULL) continue;
			if(pIUserItem->IsAndroidUser()==false)break;

			//��ȡ����
			tagBindParameter * pBindParameter=pAttemperEngineSink->GetBindParameter(pIUserItem->GetBindIndex());
			IAndroidUserItem * pIAndroidUserItem=m_pIAndroidUserManager->SearchAndroidUserItem(pIUserItem->GetUserID(),pBindParameter->dwSocketID);
			tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();

			//ģ���ж�
			if((pAndroidParameter->dwServiceMode&ANDROID_SIMULATE)!=0
				&& (pAndroidParameter->dwServiceMode&ANDROID_PASSIVITY)==0
				&& (pAndroidParameter->dwServiceMode&ANDROID_INITIATIVE)==0)
			{
				SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ���ӽ�ֹ�û��Թۣ�"),REQUEST_FAILURE_NORMAL);
				return false;
			}

			break;
		}
	}


	//�Թ��ж�
	if (CServerRule::IsAllowAndroidSimulate(m_pGameServiceOption->dwServerRule)==true
		&& (CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule)==false))
	{
		if ((pITableUserItem!=NULL)&&(pITableUserItem->IsAndroidUser()==true))
		{
			SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ�����ֹ�û��Թۣ�"),REQUEST_FAILURE_NORMAL);
			return false;
		}
	}

	//״̬�ж�
	if ((CServerRule::IsForfendGameLookon(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ�����ֹ�û��Թۣ�"),REQUEST_FAILURE_NORMAL);
		return false;
	}

	//�����ж�
	if ((pITableUserItem==NULL)&&(pIServerUserItem->GetMasterOrder()==0L))
	{
		SendRequestFailure(pIServerUserItem,TEXT("���������λ��û����Ϸ��ң��޷��Թ۴���Ϸ��"),REQUEST_FAILURE_NORMAL);
		return false;
	}

	//����Ч��
	if ((IsTableLocked()==true)&&(pIServerUserItem->GetMasterOrder()==0L)&&(lstrcmp(pUserRule->szPassword,m_szEnterPassword)!=0))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ϸ���������벻��ȷ�������Թ���Ϸ��"),REQUEST_FAILURE_PASSWORD);
		return false;
	}

	//��չЧ��
	if (m_pITableUserRequest!=NULL)
	{
		//��������
		tagRequestResult RequestResult;
		ZeroMemory(&RequestResult,sizeof(RequestResult));

		//����Ч��
		if (m_pITableUserRequest->OnUserRequestLookon(wChairID,pIServerUserItem,RequestResult)==false)
		{
			//������Ϣ
			SendRequestFailure(pIServerUserItem,RequestResult.szFailureReason,RequestResult.cbFailureCode);

			return false;
		}
	}

	//�����û�
	m_LookonUserItemArray.Add(pIServerUserItem);

	//�û�״̬
	pIServerUserItem->SetClientReady(false);
	pIServerUserItem->SetUserStatus(US_LOOKON,m_wTableID,wChairID);

	//�¼�֪ͨ
	if (m_pITableUserAction!=NULL)
	{
		m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,true);
	}

	//�¼�֪ͨ
	if(m_pIMatchTableAction!=NULL) m_pIMatchTableAction->OnActionUserSitDown(wChairID,pIServerUserItem,true);

	return true;
}

//���¶���
bool CTableFrame::PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem, LPCTSTR lpszPassword)
{
	//Ч�����
	ASSERT((pIServerUserItem!=NULL)&&(wChairID<m_wChairCount));
	ASSERT((pIServerUserItem->GetTableID()==INVALID_TABLE)&&(pIServerUserItem->GetChairID()==INVALID_CHAIR));

	//��������
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagUserRule * pUserRule=pIServerUserItem->GetUserRule();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//���ֱ���
	SCORE lUserScore=pIServerUserItem->GetUserScore();
	SCORE lMinTableScore=m_pGameServiceOption->lMinTableScore;
	SCORE lLessEnterScore=m_pITableFrameSink->QueryLessEnterScore(wChairID,pIServerUserItem);

	if(wChairID != 0)
		KillGameTimer(IDI_OFF_LINE);

	//״̬�ж�
	if ((CServerRule::IsForfendGameEnter(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ���ӽ�ֹ�û����룡"),REQUEST_FAILURE_NORMAL);
		return false;
	}

	//ģ�⴦��
	if (m_pGameServiceAttrib->wChairCount < MAX_CHAIR && pIServerUserItem->IsAndroidUser()==false)
	{
		//�������
		CAttemperEngineSink * pAttemperEngineSink=(CAttemperEngineSink *)m_pIMainServiceFrame;

		//���һ���
		for (WORD i=0; i<m_pGameServiceAttrib->wChairCount; i++)
		{
			//��ȡ�û�
			IServerUserItem *pIUserItem=m_TableUserItemArray[i];
			if(pIUserItem==NULL) continue;
			if(pIUserItem->IsAndroidUser()==false)break;

			//��ȡ����
			tagBindParameter * pBindParameter=pAttemperEngineSink->GetBindParameter(pIUserItem->GetBindIndex());
			IAndroidUserItem * pIAndroidUserItem=m_pIAndroidUserManager->SearchAndroidUserItem(pIUserItem->GetUserID(),pBindParameter->dwSocketID);
			tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();

			//ģ���ж�
			if((pAndroidParameter->dwServiceMode&ANDROID_SIMULATE)!=0
				&& (pAndroidParameter->dwServiceMode&ANDROID_PASSIVITY)==0
				&& (pAndroidParameter->dwServiceMode&ANDROID_INITIATIVE)==0)
			{
				SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ���ӽ�ֹ�û����룡"),REQUEST_FAILURE_NORMAL);
				return false;
			}

			break;
		}
	}

	//��̬����
	bool bDynamicJoin=true;
	if (m_pGameServiceAttrib->cbDynamicJoin==FALSE) bDynamicJoin=false;
	if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule)==false) bDynamicJoin=false;

	//��Ϸ״̬
	if ((m_bGameStarted==true)&&(bDynamicJoin==false))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ϸ�Ѿ���ʼ�ˣ����ڲ��ܽ�����Ϸ����"),REQUEST_FAILURE_NORMAL);
		return false;
	}

	//�����ж�
	if (pITableUserItem!=NULL)
	{
		//������
		if(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)) return false;

		//������Ϣ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�����Ѿ��� [ %s ] �����ȵ��ˣ��´ζ���Ҫ����ˣ�"),pITableUserItem->GetNickName());

		//������Ϣ
		SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NORMAL);

		return false;
	}

	//����Ч��
	if (m_wChairCount < MAX_CHAIR)
	{
		//������
		if(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule) == false)
		{
			if((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0 && ((IsTableLocked()==true)&&(pIServerUserItem->GetMasterOrder()==0L))
			&&((lpszPassword==NULL)||(lstrcmp(lpszPassword,m_szEnterPassword)!=0)))
			{
				SendRequestFailure(pIServerUserItem,TEXT("���벻�ԣ��޷����£�"),REQUEST_FAILURE_PASSWORD);
				return false;
			}
		}
	}

	//����Ч��
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0 && EfficacyEnterTableScoreRule(wChairID,pIServerUserItem)==false) return false;
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0 && EfficacyIPAddress(pIServerUserItem)==false) return false;
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0 && EfficacyScoreRule(pIServerUserItem)==false) return false;

	//��չЧ��
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0 && m_pITableUserRequest!=NULL)
	{
		//��������
		tagRequestResult RequestResult;
		ZeroMemory(&RequestResult,sizeof(RequestResult));

		//����Ч��
		if (m_pITableUserRequest->OnUserRequestSitDown(wChairID,pIServerUserItem,RequestResult)==false)
		{
			//������Ϣ
			SendRequestFailure(pIServerUserItem,RequestResult.szFailureReason,RequestResult.cbFailureCode);

			return false;
		}
	}

	//���ñ���
	m_TableUserItemArray[wChairID]=pIServerUserItem;
	m_wDrawCount=0;

	//�û�״̬
	if ((IsGameStarted()==false)||(m_cbStartMode!=START_MODE_TIME_CONTROL))
	{
		if (CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)==false && (m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0)
		{
			pIServerUserItem->SetClientReady(false);
			pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,wChairID);
		}
		else
		{
			if(m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
			{
				pIServerUserItem->SetClientReady(pIServerUserItem->IsTrusteeUser());
				pIServerUserItem->SetUserStatus(US_READY,m_wTableID,wChairID);
			}
			else
			{
				pIServerUserItem->SetClientReady(false);
				pIServerUserItem->SetUserStatus(US_READY,m_wTableID,wChairID);
			}
		}
	}
	else
	{
		//���ñ���
		m_wOffLineCount[wChairID]=0L;
		m_dwOffLineTime[wChairID]=0L;

		//������Ϸ��
		if (m_pGameServiceOption->lServiceScore>0L)
		{
			m_lFrozenedScore[wChairID]=m_pGameServiceOption->lServiceScore;
			pIServerUserItem->FrozenedUserScore(m_pGameServiceOption->lServiceScore);
		}

		//����״̬
		pIServerUserItem->SetClientReady(false);
		pIServerUserItem->SetUserStatus(US_PLAYING,m_wTableID,wChairID);
	}

	////////////////////////////////////2017.1.22 Add ������ʹ��////////////////////////////////////////////////////////////////////////

	//Ϊ����������ӵĲ���ʱ�����
	//�����û�״̬
	if(m_cbStartMode==START_MODE_TIME_FISHCONTROL)
	{
		if ((IsGameStarted()==false)||(m_cbStartMode!=START_MODE_TIME_FISHCONTROL))
		{
			if (CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)==false && (m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0)
			{
				pIServerUserItem->SetClientReady(false);
				pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,wChairID);
				//D_TRACE(L"777777������λID: %d ",wChairID);
			}
		}
		else
		{
			//���ñ���
			m_wOffLineCount[wChairID]=0L;
			m_dwOffLineTime[wChairID]=0L;

			//������
			if (m_pGameServiceOption->lServiceScore>0L)
			{
				m_lFrozenedScore[wChairID]=m_pGameServiceOption->lServiceScore;
				pIServerUserItem->FrozenedUserScore(m_pGameServiceOption->lServiceScore);
			}

			//����״̬
			pIServerUserItem->SetClientReady(true);
			pIServerUserItem->SetUserStatus(US_PLAYING,m_wTableID,wChairID);
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	//���ñ���
	m_wUserCount=GetSitUserCount();

	//������Ϣ
	if (GetSitUserCount()==1)
	{
		//״̬����
		bool bTableLocked=IsTableLocked();

		//���ñ���
		m_dwTableOwnerID=pIServerUserItem->GetUserID();
		lstrcpyn(m_szEnterPassword,pUserRule->szPassword,CountArray(m_szEnterPassword));

		//����״̬
		if (bTableLocked!=IsTableLocked()) SendTableStatus();
	}

	//������ʱ
	if(IsGameStarted()==false && pIServerUserItem->IsMobileUser() && m_pGameServiceAttrib->wChairCount<MAX_CHAIR) SetGameTimer(IDI_START_OVERTIME+wChairID,TIME_OVERTIME,1,wChairID);

	//�¼�֪ͨ
	if (m_pITableUserAction!=NULL)
	{
		m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,false);
	}

	//�¼�֪ͨ
	if(m_pIMatchTableAction!=NULL) m_pIMatchTableAction->OnActionUserSitDown(wChairID,pIServerUserItem,false);

	return true;
}


//���¶���
bool CTableFrame::PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem, DWORD dwPassword)
{
	if(m_dwTablePassword == 0)
	{
		m_dwTablePassword = dwPassword;
	}
	else if(dwPassword != m_dwTablePassword)
	{
		SendRequestFailure(pIServerUserItem,TEXT("���䲻���ڣ�"),REQUEST_FAILURE_PASSWORD);
		return false;
	}

	m_TableUserItemArray[wChairID]=pIServerUserItem;
	pIServerUserItem->SetClientReady(true);
	pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,wChairID);

	//�¼�֪ͨ
	if (m_pITableUserAction!=NULL)
	{
		m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,false);
	}

	return true;
}

bool CTableFrame::PerformSitDownAgain(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if(pIServerUserItem == NULL) return false;
	m_TableUserItemArray[wChairID]=pIServerUserItem;

	pIServerUserItem->SetClientReady(true);
	pIServerUserItem->SetUserStatus(US_READY,m_wTableID,wChairID);
	return true;
}

//��Ԫ����
VOID CTableFrame::SetCellScore(SCORE lCellScore)
{
	m_lCellScore=lCellScore;
	if (m_bTableInitFinish==true)
	{
		SendTableStatus();
		m_pITableFrameSink->SetGameBaseScore(lCellScore);
	}
}

void CTableFrame::DedultScore(IServerUserItem * pIServerUserItem,SCORE lScore)
{
	if (lScore<=0)
	{
		CTraceService::TraceString(TEXT("����ķ������֣�"),TraceLevel_Exception);
		return;
	}

	//ʹ��ԭ�нӿڣ�������д����֣�֪ͨǰ�Σ�д���¼

	//д�뷿��:DCY by 2017.4.8
	pIServerUserItem->WriteUserScore(-lScore,0,0,0,SCORE_TYPE_LOSE,0,0);
}


//����״̬
bool CTableFrame::SendTableStatus()
{
	//��������
	CMD_GR_TableStatus TableStatus;
	ZeroMemory(&TableStatus,sizeof(TableStatus));

	//��������
	TableStatus.wTableID=m_wTableID;
	TableStatus.TableStatus.lCellScore=m_lCellScore;
	TableStatus.TableStatus.cbTableLock=IsTableLocked()?TRUE:FALSE;
	TableStatus.TableStatus.cbPlayStatus=IsTableStarted()?TRUE:FALSE;

	//��������
	m_pIMainServiceFrame->SendData(BG_COMPUTER,MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));

	//�ֻ�����
	m_pIMainServiceFrame->SendData(BG_MOBILE,MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));

	return true;
}

//����ʧ��
bool CTableFrame::SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode)
{
	//��������
	CMD_GR_RequestFailure RequestFailure;
	ZeroMemory(&RequestFailure,sizeof(RequestFailure));

	//��������
	RequestFailure.lErrorCode=lErrorCode;
	lstrcpyn(RequestFailure.szDescribeString,pszDescribe,CountArray(RequestFailure.szDescribeString));

	//��������
	WORD wDataSize=CountStringBuffer(RequestFailure.szDescribeString);
	WORD wHeadSize=sizeof(RequestFailure)-sizeof(RequestFailure.szDescribeString);
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_REQUEST_FAILURE,&RequestFailure,wHeadSize+wDataSize);

	return true;
}

//��ʼЧ��
bool CTableFrame::EfficacyStartGame(WORD wReadyChairID)
{
	//״̬�ж�
	if (m_bGameStarted==true) return false;

	//ģʽ����
	if (m_cbStartMode==START_MODE_TIME_CONTROL) return false;
	if (m_cbStartMode==START_MODE_MASTER_CONTROL) return false;

	//׼������
	WORD wReadyUserCount=0;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pITableUserItem=GetTableUserItem(i);
		if (pITableUserItem==NULL) continue;

		//�û�ͳ��
		if (pITableUserItem!=NULL)
		{
			//״̬�ж�
			if (pITableUserItem->IsClientReady()==false) return false;
			if ((wReadyChairID!=i)&&(pITableUserItem->GetUserStatus()!=US_READY))
			{
				return false;
			}

			//�û�����
			wReadyUserCount++;
		}
	}

	printf("��ʼģʽ %d\n",m_cbStartMode);
	printf("������Ŀ %d\n",m_wChairCount);
	printf("׼������ %d\n",wReadyUserCount);

	//��ʼ����
	switch (m_cbStartMode)
	{
	case START_MODE_ALL_READY:			//����׼��
		{
			//��Ŀ�ж�
			if (wReadyUserCount>=2L) return true;

			return false;
		}
	case START_MODE_FULL_READY:			//���˿�ʼ
		{
			//�����ж�
			if (wReadyUserCount==m_wChairCount) return true;

			return false;
		}
	case START_MODE_PAIR_READY:			//��Կ�ʼ
		{
			//��Ŀ�ж�
			if (wReadyUserCount==m_wChairCount) return true;
			if ((wReadyUserCount<2L)||(wReadyUserCount%2)!=0) return false;

			//λ���ж�
			for (WORD i=0;i<m_wChairCount/2;i++)
			{
				//��ȡ�û�
				IServerUserItem * pICurrentUserItem=GetTableUserItem(i);
				IServerUserItem * pITowardsUserItem=GetTableUserItem(i+m_wChairCount/2);

				//λ�ù���
				if ((pICurrentUserItem==NULL)&&(pITowardsUserItem!=NULL)) return false;
				if ((pICurrentUserItem!=NULL)&&(pITowardsUserItem==NULL)) return false;
			}

			return true;
		}
	default:
		{
			ASSERT(FALSE);
			return false;
		}
	}

	return false;
}

//��ַЧ��
bool CTableFrame::EfficacyIPAddress(IServerUserItem * pIServerUserItem)
{
	//����Ա��������
	if(pIServerUserItem->GetMasterOrder()!=0) return true;

	//�����ж�
	if (CServerRule::IsForfendGameRule(m_pGameServiceOption->dwServerRule)==true) return true;

	//������
	if(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)) return true;

	//���˲�����
	if(m_wChairCount >= MAX_CHAIR) return true;

	//��ַЧ��
	const tagUserRule * pUserRule=pIServerUserItem->GetUserRule(),*pTableUserRule=NULL;
	bool bCheckSameIP=pUserRule->bLimitSameIP;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pITableUserItem=GetTableUserItem(i);
		if (pITableUserItem!=NULL && (!pITableUserItem->IsAndroidUser()) && (pITableUserItem->GetMasterOrder()==0))
		{
			pTableUserRule=pITableUserItem->GetUserRule();
			if (pTableUserRule->bLimitSameIP==true) 
			{
				bCheckSameIP=true;
				break;
			}
		}
	}

	//��ַЧ��
	if (bCheckSameIP==true)
	{
		DWORD dwUserIP=pIServerUserItem->GetClientAddr();
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pITableUserItem=GetTableUserItem(i);
			if ((pITableUserItem!=NULL)&&(pITableUserItem != pIServerUserItem)&&(!pITableUserItem->IsAndroidUser())&&(pITableUserItem->GetMasterOrder()==0)&&(pITableUserItem->GetClientAddr()==dwUserIP))
			{
				if (!pUserRule->bLimitSameIP)
				{
					//������Ϣ
					LPCTSTR pszDescribe=TEXT("����Ϸ����������˲�����ͬ IP ��ַ�������Ϸ���� IP ��ַ�����ҵ� IP ��ַ��ͬ�����ܼ�����Ϸ��");
					SendRequestFailure(pIServerUserItem,pszDescribe,REQUEST_FAILURE_NORMAL);
					return false;
				}
				else
				{
					//������Ϣ
					LPCTSTR pszDescribe=TEXT("�������˲�����ͬ IP ��ַ�������Ϸ������Ϸ���������� IP ��ַ��ͬ����ң����ܼ�����Ϸ��");
					SendRequestFailure(pIServerUserItem,pszDescribe,REQUEST_FAILURE_NORMAL);
					return false;
				}
			}
		}
		for (WORD i=0;i<m_wChairCount-1;i++)
		{
			//��ȡ�û�
			IServerUserItem * pITableUserItem=GetTableUserItem(i);
			if (pITableUserItem!=NULL && (!pITableUserItem->IsAndroidUser()) && (pITableUserItem->GetMasterOrder()==0))
			{
				for (WORD j=i+1;j<m_wChairCount;j++)
				{
					//��ȡ�û�
					IServerUserItem * pITableNextUserItem=GetTableUserItem(j);
					if ((pITableNextUserItem!=NULL) && (!pITableNextUserItem->IsAndroidUser()) && (pITableNextUserItem->GetMasterOrder()==0)&&(pITableUserItem->GetClientAddr()==pITableNextUserItem->GetClientAddr()))
					{
						LPCTSTR pszDescribe=TEXT("�������˲�����ͬ IP ��ַ�������Ϸ������Ϸ������ IP ��ַ��ͬ����ң����ܼ�����Ϸ��");
						SendRequestFailure(pIServerUserItem,pszDescribe,REQUEST_FAILURE_NORMAL);
						return false;
					}
				}
			}
		}
	}
	return true;
}

//����Ч��
bool CTableFrame::EfficacyScoreRule(IServerUserItem * pIServerUserItem)
{
	//����Ա��������
	if(pIServerUserItem->GetMasterOrder()!=0) return true;

	//�����ж�
	if (CServerRule::IsForfendGameRule(m_pGameServiceOption->dwServerRule)==true) return true;

	//������
	if(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)) return true;

	//���˲�����
	if(m_wChairCount >= MAX_CHAIR) return true;

	//��������
	WORD wWinRate=pIServerUserItem->GetUserWinRate();
	WORD wFleeRate=pIServerUserItem->GetUserFleeRate();

	//���ַ�Χ
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pITableUserItem=GetTableUserItem(i);

		//����Ч��
		if (pITableUserItem!=NULL)
		{
			//��ȡ����
			tagUserRule * pTableUserRule=pITableUserItem->GetUserRule();

			//����Ч��
			if ((pTableUserRule->bLimitFleeRate)&&(wFleeRate>pTableUserRule->wMaxFleeRate))
			{
				//������Ϣ
				TCHAR szDescribe[128]=TEXT("");
				_sntprintf(szDescribe,CountArray(szDescribe),TEXT("����������̫�ߣ��� %s ���õ����ò��������ܼ�����Ϸ��"),pITableUserItem->GetNickName());

				//������Ϣ
				SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NORMAL);

				return false;
			}

			//ʤ��Ч��
			if ((pTableUserRule->bLimitWinRate)&&(wWinRate<pTableUserRule->wMinWinRate))
			{
				//������Ϣ
				TCHAR szDescribe[128]=TEXT("");
				_sntprintf(szDescribe,CountArray(szDescribe),TEXT("����ʤ��̫�ͣ��� %s ���õ����ò��������ܼ�����Ϸ��"),pITableUserItem->GetNickName());

				//������Ϣ
				SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NORMAL);

				return false;
			}

			//����Ч��
			if (pTableUserRule->bLimitGameScore==true)
			{
				//��߻���
				if (pIServerUserItem->GetUserScore()>pTableUserRule->lMaxGameScore)
				{
					//������Ϣ
					TCHAR szDescribe[128]=TEXT("");
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("���Ļ���̫�ߣ��� %s ���õ����ò��������ܼ�����Ϸ��"),pITableUserItem->GetNickName());

					//������Ϣ
					SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NORMAL);

					return false;
				}

				//��ͻ���
				if (pIServerUserItem->GetUserScore()<pTableUserRule->lMinGameScore)
				{
					//������Ϣ
					TCHAR szDescribe[128]=TEXT("");
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("���Ļ���̫�ͣ��� %s ���õ����ò��������ܼ�����Ϸ��"),pITableUserItem->GetNickName());

					//������Ϣ
					SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NORMAL);

					return false;
				}
			}
		}
	}

	return true;
}

//����Ч��
bool CTableFrame::EfficacyEnterTableScoreRule(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//���ֱ���
	SCORE lUserScore=pIServerUserItem->GetUserScore();
	SCORE lMinTableScore=m_pGameServiceOption->lMinTableScore;
	SCORE lLessEnterScore=m_pITableFrameSink->QueryLessEnterScore(wChairID,pIServerUserItem);

	if (((lMinTableScore!=0L)&&(lUserScore<lMinTableScore))||((lLessEnterScore!=0L)&&(lUserScore<lLessEnterScore)))
	{
		//������Ϣ
		TCHAR szDescribe[128]=TEXT("");
		if(m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������Ϸ������Ҫ ") SCORE_STRING TEXT(" ����Ϸ�ң�������Ϸ�Ҳ��������ܼ��룡"),__max(lMinTableScore,lLessEnterScore));
		else if(m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������Ϸ������Ҫ ") SCORE_STRING TEXT(" �ı����ң����ı����Ҳ��������ܼ��룡"),__max(lMinTableScore,lLessEnterScore));
		else
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������Ϸ������Ҫ ") SCORE_STRING TEXT(" ����Ϸ���֣����Ļ��ֲ��������ܼ��룡"),__max(lMinTableScore,lLessEnterScore));

		//������Ϣ
		SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NOSCORE);

		return false;
	}

	return true;
}


//������
bool CTableFrame::CheckDistribute()
{
	//������
	if(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule))
	{
		//����״��
		tagTableUserInfo TableUserInfo;
		WORD wUserSitCount=GetTableUserInfo(TableUserInfo);

		//�û�����
		if(wUserSitCount < TableUserInfo.wMinUserCount)
		{
			return true;
		}
	}

	return false;
}

//��Ϸ��¼
void CTableFrame::RecordGameScore(bool bDrawStarted, DWORD dwStartGameTime)
{
	if (bDrawStarted==true)
	{
		//д���¼
		if (CServerRule::IsRecordGameScore(m_pGameServiceOption->dwServerRule)==true)
		{
			//��������
			DBR_GR_GameScoreRecord GameScoreRecord;
			ZeroMemory(&GameScoreRecord,sizeof(GameScoreRecord));

			//���ñ���
			GameScoreRecord.wTableID=m_wTableID;
			GameScoreRecord.dwPlayTimeCount=(bDrawStarted==true)?(DWORD)time(NULL)-m_dwDrawStartTime:0;

			//��Ϸʱ��
			if(dwStartGameTime!=INVALID_DWORD)
			{
				CTime startTime(dwStartGameTime);
				startTime.GetAsSystemTime(GameScoreRecord.SystemTimeStart);
			}
			else
			{
				GameScoreRecord.SystemTimeStart=m_SystemTimeStart;
			}
			GetLocalTime(&GameScoreRecord.SystemTimeConclude);

			//�û�����
			for (INT_PTR i=0;i<m_GameScoreRecordActive.GetCount();i++)
			{
				//��ȡ����
				ASSERT(m_GameScoreRecordActive[i]!=NULL);
				tagGameScoreRecord * pGameScoreRecord=m_GameScoreRecordActive[i];

				//�û���Ŀ
				if (pGameScoreRecord->cbAndroid==FALSE)
				{
					GameScoreRecord.wUserCount++;
				}
				else
				{
					GameScoreRecord.wAndroidCount++;
				}

				//����ͳ��
				GameScoreRecord.dwUserMemal+=pGameScoreRecord->dwUserMemal;

				//ͳ����Ϣ
				if (pGameScoreRecord->cbAndroid==FALSE)
				{
					GameScoreRecord.lWasteCount-=(pGameScoreRecord->lScore+pGameScoreRecord->lRevenue);
					GameScoreRecord.lRevenueCount+=pGameScoreRecord->lRevenue;
				}

				//�ɼ���Ϣ
				if (GameScoreRecord.wRecordCount<CountArray(GameScoreRecord.GameScoreRecord))
				{
					WORD wIndex=GameScoreRecord.wRecordCount++;
					CopyMemory(&GameScoreRecord.GameScoreRecord[wIndex],pGameScoreRecord,sizeof(tagGameScoreRecord));
				}
			}

			//Ͷ������
			if(GameScoreRecord.wUserCount > 0)
			{
				WORD wHeadSize=sizeof(GameScoreRecord)-sizeof(GameScoreRecord.GameScoreRecord);
				WORD wDataSize=sizeof(GameScoreRecord.GameScoreRecord[0])*GameScoreRecord.wRecordCount;
				m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GR_GAME_SCORE_RECORD,0,&GameScoreRecord,wHeadSize+wDataSize);
			}
		}

		//�����¼
		if (m_GameScoreRecordActive.GetCount()>0L)
		{
			m_GameScoreRecordBuffer.Append(m_GameScoreRecordActive);
			m_GameScoreRecordActive.RemoveAll();
		}
	}

}
//////////////////////////////////////////////////////////////////////////////////
