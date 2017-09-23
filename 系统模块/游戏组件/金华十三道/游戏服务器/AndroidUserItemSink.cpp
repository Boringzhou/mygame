#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////
#define TIME_AN_START_GAME					4									//��ʼʱ��
#define IDI_READY                  100
#define IDI_SHOW_CARD              101
#define IDI_SHOW_CARD2              102
#define IDI_BANKER						105									//�����˴�ȡ��
//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//AllocConsole();
	//freopen("CON","wt",stdout);
   ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
   ZeroMemory(m_bSegmentCard,sizeof(m_bSegmentCard));
   ZeroMemory(m_bSpecialCardData,sizeof(m_bSpecialCardData));
   ZeroMemory(m_bPlayer,sizeof(m_bPlayer));
   ZeroMemory(m_bAllUserCardData,sizeof(m_bAllUserCardData));
   m_bHandCardCount=0;
   static LONG dwRandCount=0L;
   srand((unsigned)time(NULL)+dwRandCount++);
	m_bWait=false;
   m_nRobotPlayMaxCount=2+rand()%3;
   m_nRobotPlayCount=0;
   m_nAndroidNum=0;

}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	ZeroMemory(m_bSegmentCard,sizeof(m_bSegmentCard));
	ZeroMemory(m_bSpecialCardData,sizeof(m_bSpecialCardData));
	ZeroMemory(m_bPlayer,sizeof(m_bPlayer));
	ZeroMemory(m_bAllUserCardData,sizeof(m_bAllUserCardData));
	m_bHandCardCount=0;
	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch(nTimerID)
	{
	case IDI_READY:
		{
			//����׼��
			m_pIAndroidUserItem->SendUserReady(NULL,0);
            m_pIAndroidUserItem->KillGameTimer(IDI_READY);

			return true;
		}
	case IDI_SHOW_CARD:
		{
            ShowCard();
            m_pIAndroidUserItem->KillGameTimer(IDI_SHOW_CARD);
			return true;
		}
	case IDI_BANKER:
		{
			BankOperate(2);
			return true;
		}
	case IDI_SHOW_CARD2:
		{
			m_nAndroidNum++;
			m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_CARD2,1);
			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	
	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;    
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	

	return false;
}


//���в���
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
// 	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
// 	if(pUserItem->GetUserStatus()>=US_SIT)
// 	{
// 		if(cbType==1)
// 		{
// 			CString strInfo;
// 			strInfo.Format(TEXT("������״̬���ԣ���ִ�д�ȡ��"));
// 			NcaTextOut(strInfo);
// 			return;
// 		}
// 	}
// 
// 	//��������
// 	SCORE lRobotScore = pUserItem->GetUserScore();
// 
// 	{
// 		CString strInfo;
// 		strInfo.Format(TEXT("[%s] ����(%I64d)"),pUserItem->GetNickName(),lRobotScore);
// 
// 		if (lRobotScore > m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore)
// 		{
// 			CString strInfo1;
// 			strInfo1.Format(TEXT("����������(%I64d)"),m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore);
// 			strInfo+=strInfo1;
// 
// 			NcaTextOut(strInfo);
// 		}
// 		else if (lRobotScore < m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore)
// 		{
// 			CString strInfo1;
// 			strInfo1.Format(TEXT("����ȡ������(%I64d)"),m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore);
// 			strInfo+=strInfo1;
// 
// 			NcaTextOut(strInfo);
// 		}
// 
// 		//�жϴ�ȡ
// 		if (lRobotScore >m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore)
// 		{			
// 			SCORE lSaveScore=0L;
// 			lSaveScore = lRobotScore - m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore;
// 
// 			if (lSaveScore > 0) 
// 			{
// 				lSaveScore += (rand()%(m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore - m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore));
// 				m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
// 			}
// 			SCORE lRobotNewScore = pUserItem->GetUserScore();
// 
// 			CString strInfo;
// 			strInfo.Format(TEXT("[%s] ִ�д����ǰ���(%I64d)��������(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);
// 
// 			NcaTextOut(strInfo);
// 		}
// 		else if (lRobotScore < m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore)
// 		{
// 			CString strInfo;
// 			SCORE lScore=m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore-lRobotScore;
// 			if (lScore > 0)
// 			{
// 				lScore += (rand()%(m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore - m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore));
// 				m_pIAndroidUserItem->PerformTakeScore(lScore);
// 			}
// 
// 			SCORE lRobotNewScore = pUserItem->GetUserScore();
// 			strInfo.Format(TEXT("[%s] ִ��ȡ�ȡ��ǰ���(%I64d)��ȡ�����(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);
// 
// 			NcaTextOut(strInfo);
// 		}
// 	}
}

//�û�����
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}


//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubSendCard(void * pData, WORD wDataSize)
{
	
	return true;
}


//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(void * pData, WORD wDataSize)
{

	return true;
}

void CAndroidUserItemSink::GetSpecialCard( BYTE CardData[],BYTE CardCount )
{
	
}
 void CAndroidUserItemSink::ShowCard()
 {
 }
 //void CAndroidUserItemSink::ShowCard()
 //{
 // 	bool bSpecialCard = false;
 // 	if(m_GameLogic.GetCardType(m_bHandCardData,HAND_CARD_COUNT)!=CT_INVALID)
 // 	{
 // 		//�õ������û���ˮ��
 // 		BYTE btCardData[52];
 // 		CopyMemory(btCardData,m_bAllUserCardData,sizeof(btCardData));
 // 		LONG lUserDaoShu[GAME_PLAYER];
 // 		ZeroMemory(lUserDaoShu, sizeof(lUserDaoShu));
 // 		WORD wMeChairID = m_pIAndroidUserItem->GetChairID();
 // 		//��������������Լ���ˮ��
 // 		GetAllUserDaoShu(btCardData,lUserDaoShu,wMeChairID,true);
 // 		LONG lUserDaoShu1[GAME_PLAYER];
 // 		ZeroMemory(lUserDaoShu1, sizeof(lUserDaoShu1));
 // 		//�����ͨ�������Լ���ˮ��
 // 		GetAllUserDaoShu(btCardData,lUserDaoShu1,wMeChairID,false);
 // 		if (lUserDaoShu[wMeChairID] > lUserDaoShu1[wMeChairID])
 // 			bSpecialCard = true;
 // 		else
 // 			bSpecialCard = false;
 // 
 // 		m_GameLogic.SortCardList(m_bHandCardData,HAND_CARD_COUNT);
 // 		BYTE cbHandCardCount=HAND_CARD_COUNT;
 // 		BYTE cbSegmentCard[3][5];
 // 		ZeroMemory(cbSegmentCard,sizeof(cbSegmentCard));
 // 		tagAnalyseType tagCardType11;                            //���ͼ����
 // 		ZeroMemory(&tagCardType11,sizeof(tagCardType11));
 // 		tagCardType11=m_GameLogic.GetType(m_bHandCardData,cbHandCardCount);
 // 		m_GameLogic.TheBestCard(tagCardType11,m_bHandCardData,cbHandCardCount,cbSegmentCard[0],cbSegmentCard[1],cbSegmentCard[2]);
 // 		if (CT_FIVE_FOUR_ONE<=m_GameLogic.GetCardType(cbSegmentCard[2],5/*,m_GameLogic.btCardSpecialData*/) && lUserDaoShu[wMeChairID] - lUserDaoShu1[wMeChairID] < 10)
 // 		{
 // 			bSpecialCard = false;
 // 		}
 // 	}
 // 
 // 
 // 	if(m_GameLogic.GetCardType(m_bHandCardData,HAND_CARD_COUNT/*,m_GameLogic.btCardSpecialData*/)!=CT_INVALID && bSpecialCard == true)
 // 	{
 // 		m_GameLogic.SortCardList(m_bHandCardData,HAND_CARD_COUNT,enAscend);
 // 		GetSpecialCard(m_bHandCardData,HAND_CARD_COUNT);
 // 
 // 		WORD wType = m_GameLogic.GetSpecialType(m_bHandCardData);
 // 		m_GameLogic.SortSpacialCard(m_bHandCardData,wType);
 // 
 // 		CMD_C_ShowCard AllSegmentCard ;
 // 		memset(&AllSegmentCard , 0 , sizeof(AllSegmentCard)) ;
 // 		for(int i=0;i<3;i++)
 // 		{
 // 			m_bSegmentCard[0][i]=m_bHandCardData[i];
 // 		}
 // 
 // 		for(int i=0;i<5;i++)
 // 		{
 // 			m_bSegmentCard[1][i]=m_bHandCardData[3+i];
 // 		}
 // 
 // 		for(int i=0;i<5;i++)
 // 		{
 // 			m_bSegmentCard[2][i]=m_bHandCardData[8+i];
 // 		}
 // 		m_GameLogic.SortCardList(m_bSegmentCard[0],3);
 // 		m_GameLogic.SortCardList(m_bSegmentCard[1],5);
 // 		m_GameLogic.SortCardList(m_bSegmentCard[2],5);
 // 		CopyMemory(AllSegmentCard.bFrontCard,m_bSegmentCard[0],CountArray(AllSegmentCard.bFrontCard));
 // 		CopyMemory(AllSegmentCard.bMidCard,m_bSegmentCard[1],CountArray(AllSegmentCard.bMidCard));
 // 		CopyMemory(AllSegmentCard.bBackCard,m_bSegmentCard[2],CountArray(AllSegmentCard.bBackCard));
 // 
 // 		AllSegmentCard.bSpecialType=true;
 // 
 // 		CopyMemory(m_bSpecialCardData,m_bHandCardData,sizeof(m_bSpecialCardData));
 // 		CopyMemory(AllSegmentCard.btSpecialData,m_bSpecialCardData,sizeof(AllSegmentCard.btSpecialData));
 // 
 // 		m_pIAndroidUserItem->SendSocketData(SUB_C_SHOWCARD,&AllSegmentCard,sizeof(AllSegmentCard));
 // 
 // 	}
 // 	else
 // 	{
 // 		m_GameLogic.SortCardList(m_bHandCardData,HAND_CARD_COUNT);
 // 
 // 		m_bHandCardCount=HAND_CARD_COUNT;
 // 		tagAnalyseType tagCardType;                            //���ͼ����
 // 		ZeroMemory(&tagCardType,sizeof(tagCardType));
 // 		tagCardType=m_GameLogic.GetType(m_bHandCardData,m_bHandCardCount);
 // 		m_GameLogic.TheBestCard(tagCardType,m_bHandCardData,m_bHandCardCount,m_bSegmentCard[0],m_bSegmentCard[1],m_bSegmentCard[2]);
 // 
 // 		CMD_C_ShowCard AllSegmentCard ;
 // 		memset(&AllSegmentCard , 0 , sizeof(AllSegmentCard)) ;
 // 		CopyMemory(AllSegmentCard.bFrontCard,m_bSegmentCard[0],sizeof(AllSegmentCard.bFrontCard));
 // 		CopyMemory(AllSegmentCard.bMidCard,m_bSegmentCard[1],sizeof(AllSegmentCard.bMidCard));
 // 		CopyMemory(AllSegmentCard.bBackCard,m_bSegmentCard[2],sizeof(AllSegmentCard.bBackCard));
 // 		//�����ж�
 // 		bool bDragon = false ;
 // 		if((true==m_GameLogic.CompareCard(AllSegmentCard.bFrontCard , AllSegmentCard.bMidCard , 3 , 5 , false)) &&
 // 			(true==m_GameLogic.CompareCard(AllSegmentCard.bMidCard, AllSegmentCard.bBackCard , 5 , 5 , false)))
 // 			bDragon = false ;
 // 		else
 // 			bDragon = true ;
 // 		AllSegmentCard.bDragon=bDragon;
 // 		if(AllSegmentCard.bDragon)
 // 		{
 // 			memset(&AllSegmentCard , 0 , sizeof(AllSegmentCard)) ;
 // 			//�ָ��˿�
 // 			BYTE bCardCount=HAND_CARD_COUNT;
 // 
 // 			const BYTE SEARCHE_COUNT = 10 ;
 // 			ASSERT(bCardCount==13) ;
 // 			if(bCardCount!=13) return ;
 // 			BYTE bAllSegmentCardIndex[SEARCHE_COUNT][3][5] ;			//�ֶ��˿�
 // 
 // 			BYTE bSegmentScore[SEARCHE_COUNT] ;							//�ֶ�Ȩ��
 // 			BYTE bCardDataIndex[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12} ;	//�˿��±�
 // 			BYTE bFrontCardType ,										//ǰ������
 // 				bMidCardType,											//�ж�����
 // 				bBackCardType;											//�������
 // 			BYTE bCardData[5] ;
 // 			ZeroMemory(bAllSegmentCardIndex , sizeof(bAllSegmentCardIndex)) ;
 // 			ZeroMemory(bSegmentScore , sizeof(bSegmentScore)) ;
 // 
 // 			srand((unsigned)time( NULL ));
 // 
 // 			int  bSegCount=0 ;
 // 			LONG bSearchCount = 0 ;
 // 			bool bStop = false ;
 // 			while(false==bStop)
 // 			{
 // 				++bSearchCount ;
 // 				BYTE bCardIndex=0 ;										//�˿��±�
 // 				//ǰ���˿�
 // 
 // 
 // 				for(BYTE bFrontCard=0 ; bFrontCard<3 ; ++bFrontCard)
 // 				{
 // 					bCardIndex = rand()%(13-bFrontCard) ;
 // 					bAllSegmentCardIndex[bSegCount][0][bFrontCard] = bCardDataIndex[bCardIndex] ;
 // 					bCardData[bFrontCard] = m_bHandCardData[bCardDataIndex[bCardIndex]] ;
 // 					bCardDataIndex[bCardIndex] = bCardDataIndex[13-bFrontCard-1] ;
 // 				}
 // 
 // 				m_GameLogic.SortCardList(bCardData , 3 , enDescend) ;
 // 				bFrontCardType = m_GameLogic.GetCardType(bCardData , 3/*,m_GameLogic.btCardSpecialData*/) ;
 // 
 // 				//�ж��˿�
 // 				for(BYTE bMidCard=0 ; bMidCard<5 ; ++bMidCard)
 // 				{
 // 					bCardIndex = rand()%(10-bMidCard) ;
 // 					bAllSegmentCardIndex[bSegCount][1][bMidCard] = bCardDataIndex[bCardIndex] ;
 // 					bCardData[bMidCard] = m_bHandCardData[bCardDataIndex[bCardIndex]] ;
 // 					bCardDataIndex[bCardIndex] = bCardDataIndex[10-bMidCard-1] ;
 // 				}
 // 
 // 				m_GameLogic.SortCardList(bCardData , 5 , enDescend) ;
 // 				bMidCardType = m_GameLogic.GetCardType(bCardData , 5/*,m_GameLogic.btCardSpecialData*/) ;
 // 
 // 				//����˿�
 // 				for(BYTE bBackCard=0 ; bBackCard<5 ; ++bBackCard)
 // 				{
 // 					bAllSegmentCardIndex[bSegCount][2][bBackCard] = bCardDataIndex[bBackCard] ;
 // 					bCardData[bBackCard] = m_bHandCardData[bCardDataIndex[bBackCard]] ;
 // 				}
 // 				m_GameLogic.SortCardList(bCardData , 5 , enDescend) ;
 // 
 // 
 // 				bBackCardType = m_GameLogic.GetCardType(bCardData , 5/*,m_GameLogic.btCardSpecialData*/) ;
 // 
 // 
 // 				if(bBackCardType>bMidCardType && bMidCardType>bFrontCardType) 
 // 				{
 // 					bSegmentScore[bSegCount] = bFrontCardType+bMidCardType+bBackCardType ;
 // 					bSegCount++;				
 // 				}
 // 
 // 				//�ָ�����
 // 				for(BYTE i=0 ; i<13 ; ++i)
 // 					bCardDataIndex[i] = i ;
 // 
 // 				//ֹͣ����
 // 				if(bSegCount>=SEARCHE_COUNT || bSearchCount>=10000) bStop = true ;
 // 				//�ѵ�һ��
 // 				if(true==bStop && 0==bSegCount) bStop = false ; 
 // 
 // 				//��������
 // 				if(bSearchCount>=100000)
 // 				{
 // 					BYTE bIndex=0 ; 
 // 					for(BYTE i=0 ; i<3 ; ++i)
 // 						for(BYTE j=0 ; j<5 ; ++j)
 // 							bAllSegmentCardIndex[0][i][j]=bIndex++ ;
 // 					bStop = true ;
 // 					break ;
 // 				}
 // 			}
 // 
 // 			//���Ȩ��
 // 			BYTE bMaxScore=bSegmentScore[0] ;
 // 			BYTE bIndex=0 ;
 // 			for(BYTE i=0 ; i<bSegCount ; ++i)
 // 				if(bMaxScore<bSegmentScore[i])
 // 				{
 // 					bIndex=i ;
 // 					bMaxScore=bSegmentScore[i] ;
 // 				}
 // 				//����ǰ��
 // 				for(int i=0;i<3;i++)
 // 				{
 // 					AllSegmentCard.bFrontCard[i]=m_bHandCardData[bAllSegmentCardIndex[bIndex][0][i]];
 // 				}
 // 				//�����жأ����
 // 				for(int i=0;i<5;i++)
 // 				{
 // 					AllSegmentCard.bMidCard[i]=m_bHandCardData[bAllSegmentCardIndex[bIndex][1][i]];
 // 					AllSegmentCard.bBackCard[i]=m_bHandCardData[bAllSegmentCardIndex[bIndex][2][i]];
 // 				}
 // 		}
 // 		m_GameLogic.SortCardList(AllSegmentCard.bFrontCard,3);
 // 		m_GameLogic.SortCardList(AllSegmentCard.bMidCard,5);
 // 		m_GameLogic.SortCardList(AllSegmentCard.bBackCard,5);
 // 		m_pIAndroidUserItem->SendSocketData(SUB_C_SHOWCARD,&AllSegmentCard,sizeof(AllSegmentCard));
 // 	}
 //}

void CAndroidUserItemSink::GetAllUserDaoShu( BYTE btCard[],LONG lUserDaoShu[],WORD wChaiID,bool bSpecial)
{
	
	return ;
}
