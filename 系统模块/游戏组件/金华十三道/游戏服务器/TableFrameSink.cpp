#include "StdAfx.h"
#include "TableFrameSink.h"
#include "BaseConfig.h"

//////////////////////////////////////////////////////////////////////////
#define USE_CARD_FILE_DEBUG
//构造函数
CTableFrameSink::CTableFrameSink()
{
#ifdef _DEBUG
	AllocConsole();
	freopen("CON", "wt", stdout);
#else

#endif
	

	m_cbGameCount = 0;
	m_cbAllCount = 0;
	m_cbBossCount = 0;
	m_bFive = false;
	m_cbBankerModel = enBankerType_Null;
	m_wAllSpecialType = TYPE_NULL;
	m_wBanker = INVALID_CHAIR;
	m_bBidao = false;
	m_bBihua = false;

	
	///////-----3
	m_wCurPlayerCount = 0;
	m_cbReadyCount = 0;
	m_bStartTimer = false;
	m_bIsEnd = false;
	///////-----3
	memset(m_bShowCard, 0, sizeof(m_bShowCard));
	memset(m_bPlayer, 0, sizeof(m_bPlayer));
	memset(m_bContinue, 0, sizeof(m_bContinue));

	m_wRunUser = INVALID_CHAIR;
	memset(m_lGameScore, 0, sizeof(m_lGameScore));
	memset(m_nToltalWinDaoShu, 0, sizeof(m_nToltalWinDaoShu));
	memset(m_nRewardDaoShu,0,sizeof(m_nRewardDaoShu));

	memset(m_bResponse, 0, sizeof(m_bResponse));

	memset(m_lUserScore, 0, sizeof(m_lUserScore));
	memset(m_cbShootCount, 0, sizeof(m_cbShootCount));
	memset(m_cbRunCount, 0, sizeof(m_cbRunCount));
	memset(m_cbSpecialCount, 0, sizeof(m_cbSpecialCount));

	memset(m_lUserScoreBak, 0, sizeof(m_lUserScoreBak));
	memset(m_cbShootCountBak, 0, sizeof(m_cbShootCountBak));
	memset(m_cbRunCountBak, 0, sizeof(m_cbRunCountBak));
	memset(m_cbSpecialCountBak, 0, sizeof(m_cbSpecialCountBak));

	memset(m_cbFrontCard, 0, sizeof(m_cbFrontCard));
	memset(m_cbMidCard, 0, sizeof(m_cbMidCard));
	memset(m_cbBackCard, 0, sizeof(m_cbBackCard));
	memset(m_wSpecialType, 0, sizeof(m_wSpecialType));

	memset(m_cbHandCard, 0, sizeof(m_cbHandCard));
	memset(m_wHandCardType, 0, sizeof(m_wHandCardType));
	memset(m_nSpecialCompareResult, 0, sizeof(m_nSpecialCompareResult));
	memset(m_nCompareResult, 0, sizeof(m_nCompareResult));
	memset(m_bShoot, 0, sizeof(m_bShoot));
	memset(m_bDaoshui, 0, sizeof(m_bDaoshui));

	m_pHead = nullptr;
	m_pCurrent = nullptr;
	memset(m_cbAutoCard, 0, sizeof(m_cbAutoCard));
	memset(m_cbAutoFrontCard, 0, sizeof(m_cbAutoFrontCard));
	memset(m_cbAutoMidCard, 0, sizeof(m_cbAutoMidCard));
	memset(m_cbAutoBackCard, 0, sizeof(m_cbAutoBackCard));
// 	BYTE cbCard1[5] = { 0x38,0x28,0x18,0x23,0x13};
// 	BYTE cbCard2[5] = { 0x0B,0x2B,0x3B,0x16,0x26};
// 	if (m_GameLogic.IsBigAwithB(cbCard1,5,cbCard2,5, false))
// 	{
// 		printf("前大  后小\n");
// 	}
// 	else
// 	{
// 		printf("前小  后大\n");
// 	}

//  	BYTE m_cbTestData[13]=
//  	{
//  		0x29,0x0B,0x1D,0x07,0x0C,0x0A,0x27,0x35,0x06,0x36,0x04,0x25,0x13,
//  	};
//  
//  
//  	WORD wType = m_GameLogic.GetSpecialType(m_cbTestData, 13, m_wAllSpecialType);
//  
//  	printf("特殊牌型 %d\n",wType);
//  	for (int i=0 ;i<13; i++)
//  	{
//  		printf("%d ",m_cbTestData[i]);
//  	}
//  	printf("\n");
// 	
	
	//		0x29,0x0B,0x1D,0x08,0x0C,0x0A,0x24,0x37,0x06,0x33,0x04,0x25,0x12,	//方块 A - K
	//		0x01,0x2B,0x03,0x14,0x05,0x16,0x26,0x28,0x19,0x07,0x28,0x29,0x0A,	//方块 A - K
	
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	m_pFangkaOption=m_pITableFrame->GetFangkaGameOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	m_cbGameCount = 0;
	m_cbAllCount = 0;
	m_cbBossCount = 0;
	m_cbReadyCount = 0;
	m_bFive = false;
	m_cbBankerModel = enBankerType_Null;
	m_wAllSpecialType = TYPE_NULL;
	m_wBanker = INVALID_CHAIR;
	m_bBidao = false;
	m_bBihua = false;
	memset(m_bShowCard, 0, sizeof(m_bShowCard));
	memset(m_bContinue, 0, sizeof(m_bContinue));

	m_wRunUser = INVALID_CHAIR;
	memset(m_lGameScore, 0, sizeof(m_lGameScore));
	memset(m_nToltalWinDaoShu, 0, sizeof(m_nToltalWinDaoShu));
	memset(m_nRewardDaoShu,0,sizeof(m_nRewardDaoShu));

	memcpy(m_lUserScoreBak,m_lUserScore,sizeof(m_lUserScore));
	memcpy(m_cbShootCountBak,m_cbShootCount,sizeof(m_lUserScore));
	memcpy(m_cbRunCountBak,m_cbRunCount,sizeof(m_lUserScore));
	memcpy(m_cbSpecialCountBak,m_cbSpecialCount,sizeof(m_lUserScore));

	memset(m_lUserScore, 0, sizeof(m_lUserScore));
	memset(m_cbShootCount, 0, sizeof(m_cbShootCount));
	memset(m_cbRunCount, 0, sizeof(m_cbRunCount));
	memset(m_cbSpecialCount, 0, sizeof(m_cbSpecialCount));

	memset(m_cbFrontCard, 0, sizeof(m_cbFrontCard));
	memset(m_cbMidCard, 0, sizeof(m_cbMidCard));
	memset(m_cbBackCard, 0, sizeof(m_cbBackCard));
	memset(m_wSpecialType, 0, sizeof(m_wSpecialType));

	memset(m_cbHandCard, 0, sizeof(m_cbHandCard));
	memset(m_wHandCardType, 0, sizeof(m_wHandCardType));
	memset(m_nSpecialCompareResult, 0, sizeof(m_nSpecialCompareResult));
	memset(m_nCompareResult, 0, sizeof(m_nCompareResult));
	memset(m_bShoot, 0, sizeof(m_bShoot));
	memset(m_bDaoshui, 0, sizeof(m_bDaoshui));

	return;
}

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
	printf("------------OnEventGameStart---\n");
	m_cbGameCount = 0;
	m_cbAllCount = m_pFangkaOption->cbGameOptionType[enOption_GameCount];

	m_cbBossCount = m_pFangkaOption->cbGameOptionType[enOption_sss_BossCount];

//	if(m_pFangkaOption->cbGameOptionType[enOption_sss_Five] == 0)
		m_bFive = false;
// 	else
// 		m_bFive = true;

	m_cbBankerModel = m_pFangkaOption->cbGameOptionType[enOption_sss_BankerType];

	m_bBidao = m_pFangkaOption->cbGameOptionType[enOption_sss_Bidao];

	m_bBihua = m_pFangkaOption->cbGameOptionType[enOption_sss_BiHua];

	m_bBihua = false; 
	if (m_pFangkaOption->cbGameOptionType[enOption_sss_SpecialType] == 0)
	{
		m_wAllSpecialType |= TYPE_SANTONGHUA;
		m_wAllSpecialType |= TYPE_SANSHUNZI;
		m_wAllSpecialType |= TYPE_LIUDUIBAN;
		m_wAllSpecialType |= TYPE_SITAOSANTIAO;
		m_wAllSpecialType |= TYPE_YITIAOLONG;
		m_wAllSpecialType |= TYPE_ZHIZUNQINGLONG;
		m_wAllSpecialType |= TYPE_SANZHADAN;
		m_wAllSpecialType |= TYPE_SANTONGHUASHUN;
	}
	else if (m_pFangkaOption->cbGameOptionType[enOption_sss_SpecialType] == 1)
	{
		m_wAllSpecialType |= TYPE_SANTONGHUA;
		m_wAllSpecialType |= TYPE_SANSHUNZI;
		m_wAllSpecialType |= TYPE_LIUDUIBAN;
		m_wAllSpecialType |= TYPE_WUDUISANTIAO;
		m_wAllSpecialType |= TYPE_SITAOSANTIAO;
		m_wAllSpecialType |= TYPE_COUYISE;
		m_wAllSpecialType |= TYPE_QUANXIAO;
		m_wAllSpecialType |= TYPE_QUANDA;
		m_wAllSpecialType |= TYPE_SANZHADAN;
		m_wAllSpecialType |= TYPE_SANTONGHUASHUN;
		m_wAllSpecialType |= TYPE_SHIERHUANGZU;
		m_wAllSpecialType |= TYPE_YITIAOLONG;
		m_wAllSpecialType |= TYPE_ZHIZUNQINGLONG;
	}
	else if (m_pFangkaOption->cbGameOptionType[enOption_sss_SpecialType] == 2)
	{
		m_wAllSpecialType = TYPE_NULL;
	}
	m_wAllSpecialType = TYPE_NULL;
	m_wAllSpecialType |= TYPE_YITIAOLONG;      
	m_wAllSpecialType |= TYPE_ZHIZUNQINGLONG; 

	m_wAllSpecialType |= TYPE_SANTONGHUA; 
	m_wAllSpecialType |= TYPE_SANSHUNZI; 
	m_wAllSpecialType |= TYPE_LIUDUIBAN; 
	m_wAllSpecialType |= TYPE_WUDUISANTIAO; 

	m_wMaiPaiType = m_pFangkaOption->cbGameOptionType[enOption_sss_MaPaiType];   //马牌
	//m_cbBossCount = 0;
		m_pFangkaOption->cbGameOptionType[enOption_PayType]==0?0:2;
	m_wHongbolang = 1;  //红波浪
	m_wSpec = m_pFangkaOption->cbGameOptionType[enOption_Spec];//特殊
	if (m_wSpec == 1)
	{
		m_wAllSpecialType = TYPE_NULL;
	}
	m_wFive = m_pFangkaOption->cbGameOptionType[enOption_Five_Min];//五分钟自动
	memset(m_cbShootCount, 0, sizeof(m_cbShootCount));
	memset(m_cbRunCount, 0, sizeof(m_cbRunCount));
	memset(m_cbSpecialCount, 0, sizeof(m_cbSpecialCount));

	CMD_S_GameStart nStart;
	nStart.wSpecialType = m_wAllSpecialType;
	nStart.lUserScore = m_pFangkaOption->cbGameOptionType[enOption_Score] * 100;
	nStart.cbBossCount = m_cbBossCount;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&nStart,sizeof(nStart));

	m_pITableFrame->SetFangkaStart(true);

	SendGameCount();

	return true;
}
void CTableFrameSink::OfflineGameConclude(WORD wChairID)
{
	CMD_S_Conclude nGameConclude;
	nGameConclude.bJixv = m_pITableFrame->AnyOneContinue();
	nGameConclude.bDismiss = false;

	for (BYTE i = 0; i < GAME_PLAYER; i++)
	{
		nGameConclude.lUserScore[i] = m_lUserScoreBak[i];
		nGameConclude.cbShootCount[i] = m_cbShootCountBak[i];
		nGameConclude.cbRunCount[i] = m_cbRunCountBak[i];
		nGameConclude.cbSpecialCount[i] = m_cbSpecialCountBak[i];
	}

	m_pITableFrame->SendTableData(wChairID, TH_SUB_S_GAME_CONCLUDE_OFFLINE, &nGameConclude, sizeof(nGameConclude));

}
//游戏结束
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	
	m_pITableFrame->KillGameTimer(IDI_AUTO_SHOW_CARD);
	m_pITableFrame->KillGameTimer(IDI_SHOW_SCORE);
	m_pITableFrame->KillGameTimer(IDI_NEW_START_GAME);
	m_pITableFrame->KillGameTimer(IDI_NEW_TIP_GAME);
	printf("-----------OnEventGameConclude---\n");
	memset(m_bResponse, 0, sizeof(m_bResponse));
	memset(m_bShowCard, 0, sizeof(m_bShowCard));
	memset(m_cbHandCard, 0, sizeof(m_cbHandCard));

	m_cbReadyCount = 0;
	m_bIsEnd = false;
	CMD_S_Conclude nGameConclude;

	if(cbReason == 0xFE)
		nGameConclude.bDismiss = true;
	else
		nGameConclude.bDismiss = false;

	for (BYTE i = 0; i < GAME_PLAYER; i++)
	{
		nGameConclude.lUserScore[i] = m_lUserScore[i];
		nGameConclude.cbShootCount[i] = m_cbShootCount[i];
		nGameConclude.cbRunCount[i] = m_cbRunCount[i];
		nGameConclude.cbSpecialCount[i] = m_cbSpecialCount[i];
	}

	m_pITableFrame->SendTableData(wChairID, SUB_S_GAME_CONCLUDE, &nGameConclude, sizeof(nGameConclude));

	m_pITableFrame->SetGameStatus(GS_WK_FREE);
	m_pITableFrame->SetFangkaStart(false);
	m_pITableFrame->ConcludeGame(cbReason);

	//打完全局断线
	/*for (BYTE i = 0; i < GAME_PLAYER; i++)
	{
		m_lUserScore[i] = 0;
		m_cbShootCount[i] = 0;
		m_cbRunCount[i] = 0;
		m_cbSpecialCount[i] = 0;
	}*/

	return true;
}

//发送场景
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{

	CMD_S_GameCount sGameCount;
	ZeroMemory(&sGameCount, sizeof(sGameCount));
	sGameCount.cbGameCount = m_cbGameCount;
	sGameCount.cbAllGameCount = m_cbAllCount;
	CopyMemory(sGameCount.bPlayer,m_bPlayer,sizeof(m_bPlayer));
	CopyMemory(sGameCount.lGameScore,m_lUserScore,sizeof(m_lUserScore));
	CopyMemory(sGameCount.nToltalWinDaoShu,m_nToltalWinDaoShu,sizeof(m_nToltalWinDaoShu));	
	m_pITableFrame->SendTableData(wChiarID, SUB_S_GAME_COUNT, &sGameCount, sizeof(sGameCount));

	switch (bGameStatus)
	{
	case GS_WK_FREE:
		{
// 			if (m_wCurPlayerCount >= 2)
// 			{
// 				CMD_S_StartTime sStartGame;
// 				ZeroMemory(&sStartGame, sizeof(sStartGame));
// 				DWORD dwPassTime = (GetTickCount() - m_dwStartTime) / 1000;
// 				sStartGame.cbStartTime = 20 - dwPassTime;
// 				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_START_TIME, &sStartGame, sizeof(sStartGame));
// 			}
			m_pITableFrame->SendGameScene(pIServerUserItem,NULL,0);
			//如果存在解散房间定时器 则通知解散房间消息
			m_pITableFrame->SendDismissInfo(wChiarID);
			if(m_cbGameCount >= m_cbAllCount&&m_pITableFrame->HasGameTimer(24))
			{
				OfflineGameConclude(wChiarID);
			}
			
			break;
		}
	case GS_WK_START:		//空闲状态
		{
			m_pITableFrame->SendGameScene(pIServerUserItem,NULL,0);
			//如果存在解散房间定时器 则通知解散房间消息
			m_pITableFrame->SendDismissInfo(wChiarID);
			SendGameCount(wChiarID);
			break;
		}
	case GS_WK_BIPAI:		//空闲状态
		{
			m_pITableFrame->SendGameScene(pIServerUserItem,NULL,0);
			//如果存在解散房间定时器 则通知解散房间消息
			m_pITableFrame->SendDismissInfo(wChiarID);
			GameCompare(wChiarID,true);
			break;
		}
	case GS_WK_END:		//空闲状态
		{
			m_pITableFrame->SendGameScene(pIServerUserItem,NULL,0);
			//如果存在解散房间定时器 则通知解散房间消息
			m_pITableFrame->SendDismissInfo(wChiarID);
			GameCompare(wChiarID,true);
			GameEnd(wChiarID,true);
			break;
		}
	case GS_WK_SENDCARD:		//游戏状态
		{
			m_pITableFrame->SendGameScene(pIServerUserItem,NULL,0);
			//如果存在解散房间定时器 则通知解散房间消息
			m_pITableFrame->SendDismissInfo(wChiarID);
			if (!m_bShowCard[wChiarID])
			{
				if (m_bFive)
				{
					CMD_S_SendCard nSend;
					memset(&nSend, 0, sizeof(nSend));

					nSend.wBanker = m_wBanker;
					for (BYTE i=0; i<GAME_PLAYER; i++)
					{
						nSend.bPlayer[i] = m_bPlayer[i];
					}

					for (BYTE j=0; j<HAND_CARD_COUNT; j++)
					{
						nSend.cbHandCardData[j] = m_cbHandCard[wChiarID][j];
					}
					nSend.dwTimeLeave = m_pITableFrame->GetLeaveTime(IDI_AUTO_SHOW_CARD);
					nSend.wSpecialType = m_wSpecialType[wChiarID];
					m_pITableFrame->SendTableData(wChiarID, SUB_S_SEND_CARD, &nSend, sizeof(nSend));
				}
				else
				{
					CMD_S_SendCard nSend;
					memset(&nSend, 0, sizeof(nSend));
					nSend.wBanker = m_wBanker;
					for (BYTE i=0; i<GAME_PLAYER; i++)
					{
						nSend.bPlayer[i] = m_bPlayer[i];
					}
					for (BYTE j=0; j<HAND_CARD_COUNT; j++)
					{
						nSend.cbHandCardData[j] = m_cbSaveHandCard[wChiarID][j];
					}
					nSend.dwTimeLeave = m_pITableFrame->GetLeaveTime(IDI_AUTO_SHOW_CARD);
					nSend.wSpecialType = m_wSpecialType[wChiarID];
					m_pITableFrame->SendTableData(wChiarID, SUB_S_SEND_CARD, &nSend, sizeof(nSend));
				}	
			}
			else
			{
				ShowCard(wChiarID);
			}
			break;
		}
	default:break;
	}

	return true;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch(wTimerID)
	{
	case IDI_AUTO_SHOW_CARD:
		{
			m_pITableFrame->KillGameTimer(IDI_AUTO_SHOW_CARD);
			for (int i=0;i<GAME_PLAYER;i++)
			{
				if(!m_bPlayer[i])continue;
				if (m_bResponse[i]) continue;
				AutoShowCard(i);
			}
			return true;
		}
	case IDI_SHOW_SCORE:
		{
			GameEnd();
			m_pITableFrame->KillGameTimer(IDI_SHOW_SCORE);
			return true;
		}
	case IDI_NEW_START_GAME:
		{
			m_pITableFrame->KillGameTimer(IDI_NEW_START_GAME);
			m_pITableFrame->StartGame();
			return true;
		}
	case IDI_NEW_TIP_GAME:
		{
			m_pITableFrame->KillGameTimer(IDI_NEW_TIP_GAME);
			//构造数据
			CMD_S_StartTime sStartGame;
			ZeroMemory(&sStartGame, sizeof(sStartGame));
			DWORD dwPassTime = (GetTickCount() - m_dwStartTime) / 1000;
			sStartGame.cbStartTime = 20 - dwPassTime;
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_START_TIME, &sStartGame, sizeof(sStartGame));
			return true;
		}
	default:
		break;
	}
	return false;
}

//游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_GETBANK:
	{
		CMD_C_GetBank* pGetBank = (CMD_C_GetBank*)pDataBuffer;
		return OnUserGetBanker(pIServerUserItem->GetChairID(), pGetBank->bGet);
	}
	case SUB_C_SHOWCARD:
	{
		CMD_C_ShowCard* pShowCard = (CMD_C_ShowCard*)pDataBuffer;
		return OnUserShowCard(pIServerUserItem->GetChairID(), pShowCard);
	}
	case SUB_C_CONTINUE:
	{
		return OnUserContinue(pIServerUserItem->GetChairID());
	}
	case SUB_C_READY_MESSAGE:
	{
		ASSERT(wDataSize == sizeof(CMD_C_Ready));
		if(wDataSize!=sizeof(CMD_C_Ready)) return false;
		//变量定义
		CMD_C_Ready * cReady=(CMD_C_Ready *)pDataBuffer;

		if (cReady->bReady)
		{
			m_cbReadyCount++;
			if (m_cbReadyCount >= m_wCurPlayerCount&&m_wCurPlayerCount > 1 && m_bIsEnd)
			{
				m_cbReadyCount = 0;
			}
		}
		return true;
	}
	default:break;
	}

	return false;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//数据事件
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
//积分事件
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	///////-----3     17.3.5 by tank
	if(!bLookonUser)
	{
		printf("桌子 %d 椅子 %d 坐下\n",m_pITableFrame->GetTableID(),wChairID);
		m_bPlayer[wChairID] = true;
		printf("当前玩家 %d %d %d %d\n",m_bPlayer[0],m_bPlayer[1],m_bPlayer[2],m_bPlayer[3]);
		if (m_wCurPlayerCount <= m_pFangkaOption->cbGameOptionType[enOption_PlayerCount])
		{
			m_wCurPlayerCount++;
		}
		if(m_pFangkaOption->cbGameOptionType[enOption_GameCount] == 0)
		{
			return true;
		}

// 		if (m_wCurPlayerCount == m_pFangkaOption->cbGameOptionType[enOption_PlayerCount])
// 		{
// 			m_bStartTimer = false;
// 			m_pITableFrame->KillGameTimer(IDI_NEW_START_GAME);
// 			m_pITableFrame->SetGameTimer(IDI_NEW_START_GAME, 1500, 1, 0);//延迟3秒后发送 时间显示 和 更新
// 			return true;
// 		}
	}
	///////-----3
	return true;
}

bool CTableFrameSink::OnActionUserStandUp( WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser )
{
	if (!bLookonUser)
	{
		printf("桌子 %d 椅子 %d q起立\n",m_pITableFrame->GetTableID(),wChairID);
		m_bPlayer[wChairID] = false;
		printf("当前玩家 %d %d %d %d\n",m_bPlayer[0],m_bPlayer[1],m_bPlayer[2],m_bPlayer[3]);
		///////-----4
		if (m_wCurPlayerCount > 0)
		{
			m_wCurPlayerCount--;
		}
		if (m_wCurPlayerCount < 2)
		{
			m_bStartTimer = false;
			m_pITableFrame->KillGameTimer(IDI_NEW_START_GAME);
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_STOP_TIME,0,0);
		}
		m_bIsEnd = false;
	}
	return true;
}

bool CTableFrameSink::OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	CMD_GF_User_Ready userReady;
	userReady.wChairID = pIServerUserItem->GetChairID();
	m_pITableFrame->SendTableData(wChairID,SUB_C_READY,&userReady,sizeof(userReady));

	return true;
}

void CTableFrameSink::SendGameCount(WORD wChairID)
{
	m_cbGameCount++;
	m_cbRun = 0;
	m_cbGun = 0;

	CMD_GF_RECORD nRecord;
	nRecord.cbGameCount = m_cbGameCount;
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_GF_START_RECORD, &nRecord, sizeof(CMD_GF_RECORD), MDM_GF_FRAME);

	if (m_cbBankerModel == enBankerType_QiangZhuang)
	{
		m_wBanker = INVALID_CHAIR;
		m_pITableFrame->SetGameStatus(GS_WK_START);
		m_pITableFrame->SendTableData(wChairID, SUB_S_BANKER, NULL, 0);
		return;
	}

	if (m_cbBankerModel == enBankerType_Suiji)
	{
		m_wBanker = rand() % GAME_PLAYER;

		while(!m_bPlayer[m_wBanker])
		{
			m_wBanker = (m_wBanker + 1) % GAME_PLAYER;
		}
	}
	else if (m_cbBankerModel == enBankerType_Lunliu)
	{
		if (m_cbGameCount == 1)
		{
			m_wBanker = 0;
		}
		else
		{
			m_wBanker = (m_wBanker + 1) % GAME_PLAYER;
		}
		
		while (!m_bPlayer[m_wBanker])
		{
			m_wBanker = (m_wBanker + 1) % GAME_PLAYER;
		}
	}
	else
	{
		m_wBanker = INVALID_CHAIR;
	}

	SendCard();
}

void CTableFrameSink::SendCard()
{
	m_pITableFrame->SetGameTimer(IDI_AUTO_SHOW_CARD,300*1000,1,NULL);
//	if (m_bFive)
//	{
//		BYTE cbCardData[65];
//
//
//#ifdef USE_CARD_FILE_DEBUG
//		m_GameLogic.RandCardList(cbCardData, m_cbBossCount, true);//混乱扑克
//		//readFile(cbCardData, 65,m_wBanker);
//#else
//		m_GameLogic.RandCardList(cbCardData, m_cbBossCount, true);//混乱扑克
//#endif
//
//		CMD_S_SendCard nSend;
//		memset(&nSend, 0, sizeof(nSend));
//		nSend.wBanker = m_wBanker;
//		nSend.dwTimeLeave = m_pITableFrame->GetLeaveTime(IDI_AUTO_SHOW_CARD);
//		for (BYTE i=0; i<GAME_PLAYER; i++)
//		{
//			nSend.bPlayer[i] = m_bPlayer[i];
//		}
//		for (BYTE i = 0; i < GAME_PLAYER; i++)
//		{
//			if (!m_bPlayer[i]) continue;
//			for (BYTE j=0; j<HAND_CARD_COUNT; j++)
//			{
//				m_cbHandCard[i][j] = cbCardData[j + i*HAND_CARD_COUNT];
//				m_cbSaveHandCard[i][j] = m_cbHandCard[i][j];
//				nSend.cbHandCardData[j] = m_cbHandCard[i][j];
//			}
//			m_GameLogic.SortCardByValue(m_cbHandCard[i], 13);
//			m_wSpecialType[i] = m_GameLogic.GetSpecialType(m_cbHandCard[i], 13, m_wAllSpecialType);
//
//			nSend.wSpecialType = m_wSpecialType[i];
//			m_pITableFrame->SendTableData(i, SUB_S_SEND_CARD, &nSend, sizeof(nSend));
//
//			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
//			if (pIServerUserItem)
//			{
//				pIServerUserItem->SetUserStatus(US_PEIPAI,m_pITableFrame->GetTableID(),i);
//			}
//			
//		}
//	}
//	else
//	{
		BYTE cbCardData[52];
#ifdef USE_CARD_FILE_DEBUG
		m_GameLogic.RandCardList(cbCardData, m_cbBossCount, false);//混乱扑克
		//readFile(cbCardData, 52,m_wBanker);
#else
		m_GameLogic.RandCardList(cbCardData, m_cbBossCount, false);//混乱扑克
#endif
		CMD_S_SendCard nSend;
		memset(&nSend, 0, sizeof(nSend));
		nSend.wBanker = m_wBanker;
		nSend.dwTimeLeave = m_pITableFrame->GetLeaveTime(IDI_AUTO_SHOW_CARD);
		for (BYTE i=0; i<GAME_PLAYER; i++)
		{
			nSend.bPlayer[i] = m_bPlayer[i];
		}
		//////////////////////////////////////////////////////////////////////////
		//test
		for (BYTE i = 0; i < GAME_PLAYER; i++)
		{
			if (!m_bPlayer[i]) continue;
			for (BYTE j=0; j<HAND_CARD_COUNT; j++)
			{
				m_cbHandCard[i][j] = cbCardData[j + i*HAND_CARD_COUNT];
				m_cbSaveHandCard[i][j] = m_cbHandCard[i][j];
			}
			m_GameLogic.SortCardByValue(m_cbHandCard[i], 13);
			m_wSpecialType[i] = m_GameLogic.GetSpecialType(m_cbHandCard[i], 13, m_wAllSpecialType);
		}

		int dstChairId = -1;
		std::vector<unsigned int> vUserId;
		unsigned int rate = 0;//10000单位
		//find user and rate
		char szBuf[32] = { 0 };
		unsigned wBanker = 0;
		sprintf_s(szBuf, sizeof(szBuf) - 1, "test.ini");
		CBaseConfig::SetConfigPathName(szBuf);
		if (CBaseConfig::GetRateUser(vUserId, rate))
		{
			//
			if (!vUserId.empty())
			{
				for (int z = 0;z < GAME_PLAYER;z++)
				{
					IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(z);
					if (nullptr == pIServerUserItem)
					{
						continue;
					}
					int nUserId =  pIServerUserItem->GetUserID();
					for (auto iter = vUserId.begin(); iter != vUserId.end(); iter++)
					{
						if (*iter == nUserId)
						{
							dstChairId = z;
							break;
						}
					}
					if (dstChairId != -1)
					{
						break;
					}
				}
				if (dstChairId != -1)
				{
					int maxChairID = FindMaxChair(cbCardData);
					if (maxChairID != -1 && maxChairID != dstChairId )
					{
						int random = (rand()+10000)%10000;
						if (random <=rate)
						{
							//临时数组
							BYTE tmpCard[HAND_CARD_COUNT] = {0};
							//最大牌拷贝到临时数组
							int maxOffset = maxChairID*HAND_CARD_COUNT;
							memcpy(tmpCard,cbCardData+maxOffset,sizeof(BYTE)*HAND_CARD_COUNT);
							//把目标位置牌拷贝到原最大数组位置
							int dstOffset = dstChairId*HAND_CARD_COUNT;
							memcpy(cbCardData+maxOffset,cbCardData+dstOffset,sizeof(BYTE)*HAND_CARD_COUNT);
							//把临时拷贝道目标位置
							memcpy(cbCardData+dstOffset,tmpCard,sizeof(BYTE)*HAND_CARD_COUNT);
						}
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		for (BYTE i = 0; i < GAME_PLAYER; i++)
		{
			if (!m_bPlayer[i]) continue;
			//printf("************服务器发送 数据 to 玩家 %d \n",i);//..........................................................
			for (BYTE j=0; j<HAND_CARD_COUNT; j++)
			{
				m_cbHandCard[i][j] = cbCardData[j + i*HAND_CARD_COUNT];
				m_cbSaveHandCard[i][j] = m_cbHandCard[i][j];
				nSend.cbHandCardData[j] = m_cbHandCard[i][j];
			}
		
			m_GameLogic.SortCardByValue(m_cbHandCard[i], 13);

			m_wSpecialType[i] = m_GameLogic.GetSpecialType(m_cbHandCard[i], 13, m_wAllSpecialType);

			nSend.wSpecialType = m_wSpecialType[i];
			m_pITableFrame->SendTableData(i, SUB_S_SEND_CARD, &nSend, sizeof(nSend));

			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem)
			{
				pIServerUserItem->SetUserStatus(US_PEIPAI,m_pITableFrame->GetTableID(),i);
			}
			
		}
	//}	
	
	m_pITableFrame->SetGameStatus(GS_WK_SENDCARD);
}

void CTableFrameSink::ShowCard(WORD wChairID)
{
	CMD_S_ShowCard nShowCard;
	memset(&nShowCard, 0, sizeof(nShowCard));
	nShowCard.wChairID = wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SHOW_CARD, &nShowCard, sizeof(nShowCard));

	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if (pIServerUserItem ==nullptr)
	{
		return;
	}
	pIServerUserItem->SetUserStatus(US_WANCHENG,m_pITableFrame->GetTableID(),wChairID);



	m_bShowCard[wChairID] = true;
	bool bAllShow = true;

	for (BYTE i=0; i<GAME_PLAYER; i++)
	{
		if(!m_bPlayer[i]) continue;
		if (!m_bShowCard[i])
		{
			bAllShow = false;
			break;
		}
	}
	if (bAllShow)
	{
		GameCompare();
	}
}

void CTableFrameSink::GameCompare(WORD wChairID,bool bShow)
{
	m_pITableFrame->KillGameTimer(IDI_AUTO_SHOW_CARD);
	if(wChairID == INVALID_CHAIR)
		GetCompareResult();

	//比牌结果
	CMD_S_Compare nCompare;
	memset(&nCompare, 0, sizeof(nCompare));
	for (BYTE i=0;i<GAME_PLAYER; i++)
	{
		if(!m_bPlayer[i]) continue;
		nCompare.bPlayer[i] = m_bPlayer[i];
		nCompare.wSpecialType[i] = m_wSpecialType[i];

		nCompare.nSpecialCompareResult[i] = m_nSpecialCompareResult[i];

		if(m_wSpecialType[i] != TYPE_NULL)
		{
			m_GameLogic.SortBySpecial(m_cbHandCard[i],13,m_wSpecialType[i]);

			for (BYTE j = 0; j < 3; j++)
			{
				nCompare.cbFrontCard[i][j] = m_cbHandCard[i][j];
			}
			for (BYTE j = 0; j < 5; j++)
			{
				nCompare.cbMidCard[i][j] = m_cbHandCard[i][j+3];
				nCompare.cbBackCard[i][j] = m_cbHandCard[i][j+8];
			}
		}
		else
		{
			for (BYTE j = 0; j < 3; j++)
			{
				nCompare.cbFrontCard[i][j] = m_cbFrontCard[i][j];
			}
			for (BYTE j = 0; j < 5; j++)
			{
				nCompare.cbMidCard[i][j] = m_cbMidCard[i][j];
				nCompare.cbBackCard[i][j] = m_cbBackCard[i][j];
			}
			for (BYTE j = 0; j < 3; j++)
			{
				nCompare.nCompareResult[i][j] = m_nCompareResult[i][j];
			}
			for (BYTE j=0; j<GAME_PLAYER; j++)
			{
				nCompare.bShoot[i][j] = m_bShoot[i][j];
			}
		}		
	}
	int nPlayerCount = 0;
	for (int i=0;i<GAME_PLAYER;i++)
	{
		if(m_bPlayer[i]) nPlayerCount++;
	}
	if (bShow)
	{
		m_pITableFrame->SendTableData(wChairID, SUB_S_GAME_COMPARE, &nCompare, sizeof(nCompare));
		return;
	}
	m_pITableFrame->SetGameTimer(IDI_SHOW_SCORE,(nPlayerCount*3300+m_cbRun*1200+m_cbGun*1200+1000),1,NULL);
	m_pITableFrame->SendTableData(wChairID, SUB_S_GAME_COMPARE, &nCompare, sizeof(nCompare));
	m_pITableFrame->SetGameStatus(GS_WK_BIPAI);
	
}
	
void CTableFrameSink::GameEnd(WORD wChairID,bool bShow)
{
	m_pITableFrame->KillGameTimer(IDI_AUTO_SHOW_CARD);
	CMD_S_GameEnd nGameEnd;
	memset(&nGameEnd, 0, sizeof(nGameEnd));
	memset(m_bResponse, 0, sizeof(m_bResponse));
	memset(m_bShowCard, 0, sizeof(m_bShowCard));
	memset(m_cbHandCard, 0, sizeof(m_cbHandCard));
	nGameEnd.wRunUser = m_wRunUser;
	m_bIsEnd = true;

	for (BYTE i=0; i<GAME_PLAYER; i++)
	{
		nGameEnd.lGameScore[i] = m_nToltalWinDaoShu[i];//m_lGameScore[i]
		nGameEnd.nToltalWinDaoShu[i] = m_nToltalWinDaoShu[i];
		if(wChairID==INVALID_CHAIR)
			m_lUserScore[i] += m_nToltalWinDaoShu[i];

		nGameEnd.lUserScore[i] = m_lUserScore[i];
		printf("游戏结算  下传 数据   %lld     保存数据   %lld\n",nGameEnd.lUserScore[i],m_lUserScore[i]);
	}
	if (bShow)
	{
		m_pITableFrame->SendTableData(wChairID, SUB_S_GAME_END, &nGameEnd, sizeof(nGameEnd));
		return;
	}
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_GF_STOP_RECORD, NULL, 0, MDM_GF_FRAME);

	m_pITableFrame->SendTableData(wChairID, SUB_S_GAME_END, &nGameEnd, sizeof(nGameEnd));

	m_pITableFrame->SetGameStatus(GS_WK_END);

	if(m_cbGameCount >= m_cbAllCount)
	{
		OnEventGameConclude(wChairID,NULL, 0xFF);
	}
}
	
bool CTableFrameSink::OnUserGetBanker(WORD wChairID,bool bGet)
{
	if (m_wBanker != INVALID_CHAIR) return true;
	if (m_bResponse[wChairID]) return true;
	m_bResponse[wChairID] = true;
	if(bGet)
		m_wBanker = wChairID;

	if (m_wBanker == INVALID_CHAIR)
	{
		for (BYTE i=0;i<GAME_PLAYER; i++)
		{
			if(!m_bPlayer[i]) continue;
			if (!m_bResponse[i])
			{
				return true;
			}
		}
		m_wBanker = 0;
	}

	SendCard();
	memset(m_bResponse, 0, sizeof(m_bResponse));

	return true;
}

bool CTableFrameSink::OnUserShowCard(WORD wChairID, CMD_C_ShowCard *pShowCard)
{
	if (m_bResponse[wChairID]) return true;
	m_bResponse[wChairID] = true;

	if (pShowCard->wSpecialType != TYPE_NULL)
	{
		if (pShowCard->wSpecialType != m_GameLogic.GetSpecialType(m_cbHandCard[wChairID], 13, m_wAllSpecialType))
			return false;
		else
			m_wSpecialType[wChairID] = pShowCard->wSpecialType;

		m_cbSpecialCount[wChairID]++;

		if (pShowCard->wSpecialType != TYPE_SANTONGHUASHUN && pShowCard->wSpecialType != TYPE_SANSHUNZI&& pShowCard->wSpecialType != TYPE_SANTONGHUA)
		{
			m_GameLogic.SortCardByType(m_cbHandCard[wChairID], 13);
			m_GameLogic.SortCardByType(m_cbSaveHandCard[wChairID], 13);
		}

		for (BYTE i = 0; i < 3; i++)
		{
		 	m_cbFrontCard[wChairID][i] = m_cbSaveHandCard[wChairID][i];//m_cbHandCard
		}
		for (BYTE i = 0; i < 5; i++)
		{
		 	m_cbMidCard[wChairID][i] = m_cbSaveHandCard[wChairID][i+3];//m_cbHandCard
		 	m_cbBackCard[wChairID][i] = m_cbSaveHandCard[wChairID][i+8];//m_cbHandCard
		}
	}
	else
	{
		m_wSpecialType[wChairID] = TYPE_NULL;
		for (BYTE i = 0; i < 3; i++)
		{
			m_cbFrontCard[wChairID][i] = pShowCard->cbFrontCard[i];
		}
		for (BYTE i = 0; i < 5; i++)
		{
			m_cbMidCard[wChairID][i] = pShowCard->cbMidCard[i];
			m_cbBackCard[wChairID][i] = pShowCard->cbBackCard[i];
		}
		if (!m_GameLogic.DeleteCard(m_cbHandCard[wChairID], 13, m_cbFrontCard[wChairID], 3))
			return false;
		if (!m_GameLogic.DeleteCard(m_cbHandCard[wChairID], 13, m_cbMidCard[wChairID], 5))
			return false;
		if (!m_GameLogic.DeleteCard(m_cbHandCard[wChairID], 13, m_cbBackCard[wChairID], 5))
			return false;
	}

	ShowCard(wChairID);
	
	return true;
}

bool CTableFrameSink::OnUserContinue(WORD wChairID)
{
	m_bContinue[wChairID] = true;

	CMD_S_Continue nContinue;
	nContinue.wUser = wChairID;

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CONTINUE, &nContinue, sizeof(nContinue));

	for (BYTE i=0;i <GAME_PLAYER; i++)
	{
		if(!m_bPlayer[i]) continue;
		if (!m_bContinue[i])
		{
			return true;
		}
	}
	
	ResetGame();
	SendGameCount();
	return true;
}

void CTableFrameSink::ResetGame()
{
	memset(m_bShowCard, 0, sizeof(m_bShowCard));
	memset(m_bContinue, 0, sizeof(m_bContinue));

	m_wRunUser = INVALID_CHAIR;
	memset(m_lGameScore, 0, sizeof(m_lGameScore));
	memset(m_nToltalWinDaoShu, 0, sizeof(m_nToltalWinDaoShu));
	memset(m_nRewardDaoShu,0,sizeof(m_nRewardDaoShu));

	memset(m_cbFrontCard, 0, sizeof(m_cbFrontCard));
	memset(m_cbMidCard, 0, sizeof(m_cbMidCard));
	memset(m_cbBackCard, 0, sizeof(m_cbBackCard));
	memset(m_wSpecialType, 0, sizeof(m_wSpecialType));

	memset(m_cbHandCard, 0, sizeof(m_cbHandCard));
	memset(m_wHandCardType, 0, sizeof(m_wHandCardType));
	memset(m_nSpecialCompareResult, 0, sizeof(m_nSpecialCompareResult));
	memset(m_nCompareResult, 0, sizeof(m_nCompareResult));
	memset(m_bShoot, 0, sizeof(m_bShoot));
	memset(m_bDaoshui, 0, sizeof(m_bDaoshui));
}
int CTableFrameSink::GetCompareResultTest()
{
	int ret = -1;
	int		nToltalWinDaoShu[GAME_PLAYER] = {0};					//总共道数
	bool	bDaoshui[GAME_PLAYER] = {0};							//倒水
	int		nSpecialCompareResult[GAME_PLAYER] = {0};				//特殊牌型比较结果
	int		nCompareResult[GAME_PLAYER][3] = {0};					//每一道比较结果
	//////////////////////////////////////////////////////////////////////////
	BYTE cbFrontCardData[GAME_PLAYER][3] = {0};
	BYTE cbMidCardData[GAME_PLAYER][5] = {0};
	BYTE cbBackCardData[GAME_PLAYER][5] = {0};
	for (int i=0; i<GAME_PLAYER; i++)
	{
		memcpy(cbFrontCardData[i],m_cbFrontCard[i],sizeof(cbFrontCardData[i]));
		memcpy(cbMidCardData[i],m_cbMidCard[i],sizeof(cbMidCardData[i]));
		memcpy(cbBackCardData[i],m_cbBackCard[i],sizeof(cbBackCardData[i]));
	}

	for (BYTE i=0; i<GAME_PLAYER; i++)
	{
		if(!m_bPlayer[i]) continue;
		if(m_wSpecialType[i] != TYPE_NULL) continue;
		
		m_wHandCardType[i][0] = m_GameLogic.GetCardType(cbFrontCardData[i], 3, m_cbBossCount, m_bFive);
		m_wHandCardType[i][1] = m_GameLogic.GetCardType(cbMidCardData[i], 5, m_cbBossCount, m_bFive);
		m_wHandCardType[i][2] = m_GameLogic.GetCardType(cbBackCardData[i], 5, m_cbBossCount, m_bFive);

		bool bGreat = false;
		if (m_wHandCardType[i][0] > m_wHandCardType[i][1] || m_wHandCardType[i][1] > m_wHandCardType[i][2])
		{
			bGreat = true;
		}
		else if (m_wHandCardType[i][0] == m_wHandCardType[i][1])
		{
			if (m_GameLogic.IsBigAwithB(cbFrontCardData[i], 3, cbMidCardData[i], 5, m_bBihua))
			{
				bGreat = true;
			}
		}
		else if (m_wHandCardType[i][1] == m_wHandCardType[i][2])
		{
			if (m_GameLogic.IsBigAwithB(cbMidCardData[i], 5, cbBackCardData[i], 5, m_bBihua))
			{
				bGreat = true;
			}
		}
		bDaoshui[i] = bGreat;
	}

	int cbShui[GAME_PLAYER][GAME_PLAYER][3];
	ZeroMemory(cbShui, sizeof(cbShui));

	//三道输赢记录
	int iWinScoreRecord[GAME_PLAYER][GAME_PLAYER];
	ZeroMemory(iWinScoreRecord, sizeof(iWinScoreRecord));
	for (BYTE i = 0; i< GAME_PLAYER; i++)
	{
		if (!m_bPlayer[i]) continue;
		for (BYTE j = 0; j < GAME_PLAYER; j++)
		{
			if (!m_bPlayer[j]) continue;
			if(j == i) continue;
			if (m_GameLogic.IsBigSpecialAwithB(m_wSpecialType[i], m_wSpecialType[j]))
			{
				int nShui = GetSpecialShui(m_wSpecialType[i]);
				
				if (HasMapai(j, m_wMaiPaiType)|| HasMapai(i, m_wMaiPaiType))
				{
					nSpecialCompareResult[i] += nShui*2;
					nSpecialCompareResult[j] -= nShui*2;

					nToltalWinDaoShu[i] += nShui*2;
					nToltalWinDaoShu[j] -= nShui*2;
				}
				else
				{
					nSpecialCompareResult[i] += nShui;
					nSpecialCompareResult[j] -= nShui;

					nToltalWinDaoShu[i] += nShui;
					nToltalWinDaoShu[j] -= nShui;
				}

				continue;
			}
			if(m_wSpecialType[j]) continue;
			BYTE cbShoot = 0;
			WORD bGreates[3] = { 0 };
			for (BYTE k=0; k<3; k++)
			{
				bool bGreat = false;
				if (m_wHandCardType[i][k] > m_wHandCardType[j][k])
				{
					bGreat = true;
				}
				else if (m_wHandCardType[i][k] == m_wHandCardType[j][k])
				{
					if (m_wHandCardType[i][k] == HT_LINE || m_wHandCardType[i][k] == HT_LINE_COLOR)
					{
						if (k == 1)
						{
							if (m_GameLogic.IsBigLineAwithB(cbMidCardData[i],5,cbMidCardData[j],5, m_bBihua))
							{
								bGreat = true;
							}
						}
						else if (k == 2)
						{
							if (m_GameLogic.IsBigLineAwithB(cbBackCardData[i],5,cbBackCardData[j],5, m_bBihua))
							{
								bGreat = true;
							}
						}
					}
					else
					{
						if (k == 0)
						{
							if (m_GameLogic.IsBigAwithB(cbFrontCardData[i],3,cbFrontCardData[j],3, m_bBihua))
							{
								bGreat = true;
							}
						}
						else if (k == 1)
						{
							if (m_GameLogic.IsBigAwithB(cbMidCardData[i],5,cbMidCardData[j],5, m_bBihua))
							{
								bGreat = true;
							}
						}
						else if (k == 2)
						{
							if (m_GameLogic.IsBigAwithB(cbBackCardData[i],5,cbBackCardData[j],5, m_bBihua))
							{
								bGreat = true;
							}
						}
					}
				}
				
				if (bGreat)
				{
					cbShoot++;
					cbShui[i][j][k] = GetShui(m_wHandCardType[i][k]);
					if (k == 1 && m_wHandCardType[i][k] >= HT_THREE_DEOUBLE)
					{
						cbShui[i][j][k] *= 2;
					}
					if (k == 0 && m_wHandCardType[i][k] >= HT_THREE)
					{
						if (m_wHandCardType[i][k] == HT_THREE)
						{
							cbShui[i][j][k] = 3;
						}
					}
					LONGLONG  lTmpFenshu = cbShui[i][j][k];
					bGreates[k] = 1;
					nCompareResult[i][k] += cbShui[i][j][k];
					nCompareResult[j][k] -= cbShui[i][j][k];
				}
			}
			int   lTotalshufen = 0;
			for (WORD m = 0; m < 3; m++)
			{
				if (bGreates[m] >0  )
				{
					lTotalshufen += cbShui[i][j][m];
				}
				
			}
			
			LONGLONG  ltmpRecordFenshu = lTotalshufen;
			if (cbShoot == 3)
			{
				if (m_pFangkaOption->cbGameOptionType[enOption_sss_GunType] == 0)
				{
					ltmpRecordFenshu += 1;
				}
				else
				{
					ltmpRecordFenshu += lTotalshufen;
				}
			}
			iWinScoreRecord[i][j] = iWinScoreRecord[i][j] + ltmpRecordFenshu;
			iWinScoreRecord[j][i] = iWinScoreRecord[j][i] - ltmpRecordFenshu;
		}
	}

	int nPlayerCount = 0;
	for (BYTE i = 0; i < GAME_PLAYER; i++)
	{
		if (m_bPlayer[i]) nPlayerCount++;
	}

	//计算马牌 
	bool bHasMapai = false;
	int iMaipaiIndex = 0;
	for (BYTE i = 0; i < GAME_PLAYER; i++)
	{
		if (m_wMaiPaiType > 0  && HasMapai(i, m_wMaiPaiType))
		{
			bHasMapai = true;
			iMaipaiIndex = i;
		}
	}
	if ( bHasMapai )
	{
		for (BYTE j = 0; j < GAME_PLAYER; j++)
		{
			if (!m_bPlayer[j]) continue;
			if (j == iMaipaiIndex) continue;
			iWinScoreRecord[iMaipaiIndex][j] *= 2;
			iWinScoreRecord[j][iMaipaiIndex] *= 2;
		}
	}

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		for (int j = 0; j < GAME_PLAYER; j++)
		{
			nToltalWinDaoShu[i] += iWinScoreRecord[i][j];
		}
	}
	
	//for(int i =0;i<GAME_PLAYER;i++)
	//{
	//	m_lGameScore[i] = nToltalWinDaoShu[i];
	//}
	int tmpScore = 0;
	for(int i =0;i<GAME_PLAYER;i++)
	{
		if (nToltalWinDaoShu[i] > tmpScore)
		{
			tmpScore = nToltalWinDaoShu[i];
			ret = i;
		}
	}

	return ret;
}
void CTableFrameSink::GetCompareResult()
{
	BYTE cbFrontCardData[GAME_PLAYER][3] = {0};
	BYTE cbMidCardData[GAME_PLAYER][5] = {0};
	BYTE cbBackCardData[GAME_PLAYER][5] = {0};
	for (int i=0; i<GAME_PLAYER; i++)
	{
		memcpy(cbFrontCardData[i],m_cbFrontCard[i],sizeof(cbFrontCardData[i]));
		memcpy(cbMidCardData[i],m_cbMidCard[i],sizeof(cbMidCardData[i]));
		memcpy(cbBackCardData[i],m_cbBackCard[i],sizeof(cbBackCardData[i]));
	}

	for (BYTE i=0; i<GAME_PLAYER; i++)
	{
		if(!m_bPlayer[i]) continue;
		if(m_wSpecialType[i] != TYPE_NULL) continue;
		
		m_wHandCardType[i][0] = m_GameLogic.GetCardType(cbFrontCardData[i], 3, m_cbBossCount, m_bFive);
		m_wHandCardType[i][1] = m_GameLogic.GetCardType(cbMidCardData[i], 5, m_cbBossCount, m_bFive);
		m_wHandCardType[i][2] = m_GameLogic.GetCardType(cbBackCardData[i], 5, m_cbBossCount, m_bFive);

		CString strMessage;
		strMessage.Format(TEXT("桌子--%d,椅子--%d: 前墩 %d--%d--%d 结果--%d"),m_pITableFrame->GetTableID(),i,cbFrontCardData[i][0],cbFrontCardData[i][1],cbFrontCardData[i][2],m_wHandCardType[i][0]);
		CTraceService::TraceString(strMessage,TraceLevel_Exception);

		strMessage.Format(TEXT("桌子--%d,椅子--%d: 中墩 %d--%d--%d--%d--%d 结果--%d"),m_pITableFrame->GetTableID(),i,cbMidCardData[i][0],cbMidCardData[i][1],cbMidCardData[i][2],cbMidCardData[i][3],cbMidCardData[i][4],m_wHandCardType[i][1]);
		CTraceService::TraceString(strMessage,TraceLevel_Exception);

		strMessage.Format(TEXT("桌子--%d,椅子--%d: 后墩 %d--%d--%d--%d--%d 结果--%d"),m_pITableFrame->GetTableID(),i,cbBackCardData[i][0],cbBackCardData[i][1],cbBackCardData[i][2],cbBackCardData[i][3],cbBackCardData[i][4],m_wHandCardType[i][2]);
		CTraceService::TraceString(strMessage,TraceLevel_Exception);

		bool bGreat = false;
		if (m_wHandCardType[i][0] > m_wHandCardType[i][1] || m_wHandCardType[i][1] > m_wHandCardType[i][2])
		{
			bGreat = true;
		}
		else if (m_wHandCardType[i][0] == m_wHandCardType[i][1])
		{
			if (m_GameLogic.IsBigAwithB(cbFrontCardData[i], 3, cbMidCardData[i], 5, m_bBihua))
			{
				bGreat = true;
			}
		}
		else if (m_wHandCardType[i][1] == m_wHandCardType[i][2])
		{
			if (m_GameLogic.IsBigAwithB(cbMidCardData[i], 5, cbBackCardData[i], 5, m_bBihua))
			{
				bGreat = true;
			}
		}
		m_bDaoshui[i] = bGreat;
	}

	int cbShui[GAME_PLAYER][GAME_PLAYER][3];
	ZeroMemory(cbShui, sizeof(cbShui));

	//三道输赢记录
	int iWinScoreRecord[GAME_PLAYER][GAME_PLAYER];
	ZeroMemory(iWinScoreRecord, sizeof(iWinScoreRecord));
	for (BYTE i = 0; i< GAME_PLAYER; i++)
	{
		if (!m_bPlayer[i]) continue;
		for (BYTE j = 0; j < GAME_PLAYER; j++)
		{
			if (!m_bPlayer[j]) continue;
			if(j == i) continue;
			if (m_GameLogic.IsBigSpecialAwithB(m_wSpecialType[i], m_wSpecialType[j]))
			{
				int nShui = GetSpecialShui(m_wSpecialType[i]);
				
				if (HasMapai(j, m_wMaiPaiType)|| HasMapai(i, m_wMaiPaiType))
				{
					m_nSpecialCompareResult[i] += nShui*2;
					m_nSpecialCompareResult[j] -= nShui*2;

					m_nToltalWinDaoShu[i] += nShui*2;
					m_nToltalWinDaoShu[j] -= nShui*2;
				}
				else
				{
					m_nSpecialCompareResult[i] += nShui;
					m_nSpecialCompareResult[j] -= nShui;

					m_nToltalWinDaoShu[i] += nShui;
					m_nToltalWinDaoShu[j] -= nShui;
				}

				continue;
			}
			if(m_wSpecialType[j]) continue;
			BYTE cbShoot = 0;
			WORD bGreates[3] = { 0 };
			for (BYTE k=0; k<3; k++)
			{
				bool bGreat = false;
				if (m_wHandCardType[i][k] > m_wHandCardType[j][k])
				{
					bGreat = true;
				}
				else if (m_wHandCardType[i][k] == m_wHandCardType[j][k])
				{
					if (m_wHandCardType[i][k] == HT_LINE || m_wHandCardType[i][k] == HT_LINE_COLOR)
					{
						if (k == 1)
						{
							if (m_GameLogic.IsBigLineAwithB(cbMidCardData[i],5,cbMidCardData[j],5, m_bBihua))
							{
								bGreat = true;
							}
						}
						else if (k == 2)
						{
							if (m_GameLogic.IsBigLineAwithB(cbBackCardData[i],5,cbBackCardData[j],5, m_bBihua))
							{
								bGreat = true;
							}
						}
					}
					else
					{
						if (k == 0)
						{
							if (m_GameLogic.IsBigAwithB(cbFrontCardData[i],3,cbFrontCardData[j],3, m_bBihua))
							{
								bGreat = true;
							}
						}
						else if (k == 1)
						{
							if (m_GameLogic.IsBigAwithB(cbMidCardData[i],5,cbMidCardData[j],5, m_bBihua))
							{
								bGreat = true;
							}
						}
						else if (k == 2)
						{
							if (m_GameLogic.IsBigAwithB(cbBackCardData[i],5,cbBackCardData[j],5, m_bBihua))
							{
								bGreat = true;
							}
						}
					}
				}
				
				if (bGreat)
				{
					cbShoot++;
					cbShui[i][j][k] = GetShui(m_wHandCardType[i][k]);
					if (k == 1 && m_wHandCardType[i][k] >= HT_THREE_DEOUBLE)
					{
						cbShui[i][j][k] *= 2;
					}
					if (k == 0 && m_wHandCardType[i][k] >= HT_THREE)
					{
						if (m_wHandCardType[i][k] == HT_THREE)
						{
							cbShui[i][j][k] = 3;
						}
					}
					LONGLONG  lTmpFenshu = cbShui[i][j][k];
					bGreates[k] = 1;
					m_nCompareResult[i][k] += cbShui[i][j][k];
					m_nCompareResult[j][k] -= cbShui[i][j][k];
				}
			}
			int   lTotalshufen = 0;
			for (WORD m = 0; m < 3; m++)
			{
				if (bGreates[m] >0  )
				{
					lTotalshufen += cbShui[i][j][m];
				}
				
			}
			
			LONGLONG  ltmpRecordFenshu = lTotalshufen;
			if (cbShoot == 3)
			{
				m_bShoot[i][j] = true;
				m_cbShootCount[i]++;
				m_cbGun++;

				if (m_pFangkaOption->cbGameOptionType[enOption_sss_GunType] == 0)
				{
					ltmpRecordFenshu += 1;
				}
				else
				{
					ltmpRecordFenshu += lTotalshufen;
				}
			}
			
			////红波浪模式
			/*if (m_wHongbolang > 0  && ltmpRecordFenshu >12 )
			{
			ltmpRecordFenshu = 12;
			}*/
			iWinScoreRecord[i][j] = iWinScoreRecord[i][j] + ltmpRecordFenshu;
			iWinScoreRecord[j][i] = iWinScoreRecord[j][i] - ltmpRecordFenshu;
		}
	}

	int nPlayerCount = 0;
	for (BYTE i = 0; i < GAME_PLAYER; i++)
	{
		if (m_bPlayer[i]) nPlayerCount++;
	}

	//全垒打
	if(nPlayerCount>2)
	{
		for (BYTE i = 0; i < GAME_PLAYER; i++)
		{
			if (!m_bPlayer[i]) continue;
			bool bRun = true;

			for (BYTE j = 0; j < GAME_PLAYER; j++)
			{
				if (!m_bPlayer[j]) continue;
				if (j == i) continue;
				if (!m_bShoot[i][j])
				{
					bRun = false;
					break;
				}
			}

			if (bRun)
			{
				m_wRunUser = i;
				m_cbRun++;
				m_cbRunCount[m_wRunUser]++;

				for (BYTE j = 0; j < GAME_PLAYER; j++)
				{
					if (!m_bPlayer[j]) continue;
					if (j == i) continue;

					iWinScoreRecord[i][j] *= 2;
					iWinScoreRecord[j][i] *= 2;
					
				}
				break;
			}
		}
	}
	

	//计算马牌 
	bool bHasMapai = false;
	int iMaipaiIndex = 0;
	for (BYTE i = 0; i < GAME_PLAYER; i++)
	{
		if (m_wMaiPaiType > 0  && HasMapai(i, m_wMaiPaiType))
		{
			bHasMapai = true;
			iMaipaiIndex = i;
		}
	}
	if ( bHasMapai )
	{
		for (BYTE j = 0; j < GAME_PLAYER; j++)
		{
			if (!m_bPlayer[j]) continue;
			if (j == iMaipaiIndex) continue;
			iWinScoreRecord[iMaipaiIndex][j] *= 2;
			iWinScoreRecord[j][iMaipaiIndex] *= 2;
		}
	}

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		for (int j = 0; j < GAME_PLAYER; j++)
		{
			m_nToltalWinDaoShu[i] += iWinScoreRecord[i][j];
		}
	}
	
	for(int i =0;i<GAME_PLAYER;i++)
	{
		m_lGameScore[i] = m_nToltalWinDaoShu[i];
	}
}

BYTE CTableFrameSink::GetBaseFenByShoot(BYTE cbShootCnt)
{
	BYTE cbBaseShoot = 0;
	BYTE nPlayerCount = 0;
	for (BYTE i = 0; i < GAME_PLAYER; i++)
	{
		if (m_bPlayer[i]) nPlayerCount++;
	}
	switch (nPlayerCount)
	{
	case 2:
		cbBaseShoot = 3;
		break;
	case 3:
		cbBaseShoot = (cbShootCnt==1 ? 6:(cbShootCnt==2 ? 13:0));
		break;
	case 4:
		cbBaseShoot = ((cbShootCnt==1||cbShootCnt==2)? 6:(cbShootCnt==3? 13:0));
		break;
	default:
		break;
	}
	return cbBaseShoot;
}

BYTE CTableFrameSink::GetRewardByHandType(BYTE iChair)
{
	BYTE cbReward = 0;
	if(!m_bPlayer[iChair]) return 0;

	if(m_wHandCardType[iChair][0] == HT_THREE) 
		cbReward = 3;

	if(m_wHandCardType[iChair][1] == HT_THREE_DEOUBLE) 
		cbReward += 2;
	else if(m_wHandCardType[iChair][1] == HT_FOUR_BOOM) 
		cbReward += 8;
	else if(m_wHandCardType[iChair][1] == HT_LINE_COLOR) 
		cbReward += 10;

	if(m_wHandCardType[iChair][2] == HT_FOUR_BOOM)
		cbReward += 4;
	else if(m_wHandCardType[iChair][2] == HT_LINE_COLOR) 
		cbReward += 5;

	return cbReward;
}

bool CTableFrameSink::HasMapai(WORD wChairID, WORD wMaiPaiType)
{
	if (wMaiPaiType == 0)  return false;

	for (WORD i = 0; i < 3; i++)
	{
		if (m_cbFrontCard[wChairID][i] == wMaiPaiType)  return true;
	}
	for (WORD i = 0; i < 5; i++)
	{
		if (m_cbMidCard[wChairID][i] == wMaiPaiType)  return true;
	}
	for (WORD i = 0; i < 5; i++)
	{
		if (m_cbBackCard[wChairID][i] == wMaiPaiType)  return true;
	}

	for (WORD i = 0; i < HAND_CARD_COUNT; i++)
	{
		if (m_cbHandCard[wChairID][i] == wMaiPaiType)  return true;
	}

	return false;
}

BYTE CTableFrameSink::GetSpecialShui(WORD wType)
{
	switch (wType)
	{
	case TYPE_ZHIZUNQINGLONG:
		return 26;
	case TYPE_YITIAOLONG:
		return 26;
	case TYPE_SANTONGHUA:
		return 6;
	case TYPE_SANSHUNZI:
		return 6;
	case TYPE_LIUDUIBAN:
		return 6;
	case TYPE_WUDUISANTIAO:
		return 6;
	case TYPE_NULL:
		return 0;
	default:
		break;
	}
	return 4;
}

BYTE CTableFrameSink::GetShui(WORD wType)
{
	switch (wType)
	{
	case HT_SIX_BOOM:
		return 1;
	case HT_FIVE_BOOM:
		return 1;
	case HT_LINE_COLOR:
		return 5;
	case HT_FOUR_BOOM:
		return 4;
	default:
		break;
	}
	return 1;
}

void CTableFrameSink::AutoShowCard(WORD wChairID,bool testOnly)
{
	if (m_wFive == 0 && testOnly == false)
	{
		return;
	}
	//这里把对应的人牌 按照大小摆好
	//先将手牌复制到对应的自动出牌的手牌中
	for (int i = 0;i<13;i++)
	{
		m_cbAutoCard[i] = m_cbHandCard[wChairID][i];
	}
	//循环遍历类型最大的牌到对应的三道 
	for (int j=0;j!=3;j++)
	{
		DeleteData();
		//每次遍历计算数量和王的数量
		BYTE cbAutoCount = 0;
		BYTE cbBossCount = 0;
		BYTE cbCount = 0;
		if (j == 0)
		{
			cbCount = 13;
		}
		else if (j == 1)
		{
			cbCount = 8;
		}
		else
		{
			cbCount = 3;
		}
		for (int i = 0;i<cbCount;i++)
		{
			if (m_cbAutoCard[i] != 0)
			{
				cbAutoCount++;
			}
			if (m_cbAutoCard[i] >= 0x4E)
			{
				cbBossCount++;
			}
		}

		m_pHead = new AnalyseResult;
		m_pHead->pNext = nullptr;

		if (cbAutoCount > 0)
			m_wAutoCardType = m_GameLogic.GetCardType(m_pHead, m_cbAutoCard, cbAutoCount, cbBossCount);
		else
			m_wAutoCardType = HT_INVALID;

		m_pCurrent = m_pHead;
		m_pHead = m_pHead->pNext;
		delete m_pCurrent;
		m_pCurrent = nullptr;
		
		//遍历m_pHead里最大的类型
		if (m_wAutoCardType == HT_INVALID || m_wAutoCardType == HT_SINGLE)
		{
			m_GameLogic.SortCardByValue(m_cbAutoCard, cbAutoCount);
			break;
		}
		if (m_wAutoCardType != HT_INVALID&&m_wAutoCardType!=HT_SINGLE)
		{
			if (j == 0)
			{
				for (int iBack = 0;iBack != 5;iBack++)
				{
					m_cbBackCard[wChairID][iBack] = m_pHead->cbCard[iBack];
				}
				m_GameLogic.DeleteCard(m_cbAutoCard,13,m_pHead->cbCard,5);
				
			}
			if (j == 1)
			{
				for (int iMid = 0;iMid != 5;iMid++)
				{
					m_cbMidCard[wChairID][iMid] = m_pHead->cbCard[iMid];
				}
				m_GameLogic.DeleteCard(m_cbAutoCard,13,m_pHead->cbCard,5);
			}
			if (j == 2)
			{
				for (int iFront = 0;iFront != 3;iFront++)
				{
					m_cbFrontCard[wChairID][iFront] = m_cbAutoCard[iFront];
				}
				m_GameLogic.DeleteCard(m_cbAutoCard,13,m_cbAutoCard,3);
			}
		}
	}

	//如果类型取完了 剩余散牌按照大小放入对应的剩余道
	
	if(m_wAutoCardType == HT_INVALID || m_wAutoCardType == HT_SINGLE)
	{
		int iLeftCount = 0;
		for (int i =0;i!=13;i++)
		{
			if (m_cbAutoCard[i]!=0)
			{
				iLeftCount++;
			}
		}
		if (iLeftCount == 8)
		{
			for (int iMid = 0;iMid != 8;iMid++)
			{
				if (iMid<5)
				{
					m_cbMidCard[wChairID][iMid] = m_cbAutoCard[iMid];
				}
				else
				{
					m_cbFrontCard[wChairID][iMid-5] = m_cbAutoCard[iMid];
				}
			}
		}
		else if(iLeftCount == 3)
		{
			for (int iMid = 0;iMid != 3;iMid++)
			{
				m_cbFrontCard[wChairID][iMid] = m_cbAutoCard[iMid];
			}
		}
	}
	if (testOnly)
	{
		return;
	}
	ShowCard(wChairID);
}

bool CTableFrameSink::readFile(BYTE *pCardData, BYTE cbCardCnt, WORD &zhuang)
{
	std::vector<BYTE> cardVec;
	char szBuf[32] = { 0 };
	unsigned wBanker = 0;
	sprintf_s(szBuf, sizeof(szBuf) - 1, "sss_card.ini");
	CBaseConfig::SetConfigPathName(szBuf);
	if (CBaseConfig::GetCardConf(m_cbGameCount, GAME_PLAYER, cardVec, wBanker))
	{
		if(0!=wBanker)
			zhuang = wBanker-1;
		for (size_t i = 0; i < cardVec.size(); i++)
		{
			BYTE j = 0;
			BYTE cbCard = cardVec[i];
			if (0 == cbCard)
				continue;
			for (j = i; j < cbCardCnt; j++)
			{
				if (cbCard == pCardData[j])
				{
					break;
				}
			}
			if (j > cbCardCnt)
			{
				ASSERT(FALSE);
				printf("配牌错误\n");
			}
			BYTE tempCard = pCardData[i];
			pCardData[i] = pCardData[j];
			pCardData[j] = tempCard;
		}

		return true;
	}
	else
	{
		return false;
	}
}


void CTableFrameSink::DeleteData()
{
	AnalyseResult* p = m_pHead;
	while (p)
	{
		m_pHead = p->pNext;
		delete p;
		p = m_pHead;
	}
}

int CTableFrameSink::FindMaxChair(BYTE * cbCardBuffer)
{
	int ret = -1;
	
	for (int i=0;i<GAME_PLAYER;i++)
	{
		if(!m_bPlayer[i])continue;
		AutoShowCard(i,true);
	}
	ret = GetCompareResultTest();

	return ret;
}

//////////////////////////////////////////////////////////////////////////