#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//静态变量

//扑克数据
const BYTE	CGameLogic::m_cbCardListData[52]=
{
 	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
 	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K	
};

//扑克数据
const BYTE	CGameLogic::m_cbCardListDataFive[65] =
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K	
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K	
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBossCount, bool bFive)
{
// 	if (bFive)
// 	{
// 		//混乱准备
// 		BYTE cbCardData[65];
// 		BYTE cbBufferCount;
// 		cbBufferCount = 65;
// 		CopyMemory(cbCardData, m_cbCardListDataFive, 65);
// 		AddBoss(cbBossCount, cbCardData, cbBufferCount);
// 
// 		static LONG dwRandCount = 0L;
// 		srand((unsigned)time(NULL) + dwRandCount++);
// 
// 		//混乱扑克
// 		BYTE cbRandCount = 0, bPosition = 0;
// 
// 		do
// 		{
// 			bPosition = rand() % (cbBufferCount - cbRandCount);
// 			cbCardBuffer[cbRandCount++] = cbCardData[bPosition];
// 			cbCardData[bPosition] = cbCardData[cbBufferCount - cbRandCount];
// 		} while (cbRandCount < cbBufferCount);
// 
// 	}
// 	else
// 	{
		//混乱准备
		BYTE cbCardData[52];
		BYTE cbBufferCount;
		cbBufferCount = 52;
		CopyMemory(cbCardData, m_cbCardListData, 52);

		//AddBoss(cbBossCount, cbCardData, cbBufferCount);

		static LONG dwRandCount = 0L;
		srand((unsigned)time(NULL) + dwRandCount++);

		//混乱扑克
		BYTE cbRandCount = 0, bPosition = 0;

		do
		{
			bPosition = rand() % (cbBufferCount - cbRandCount);
			cbCardBuffer[cbRandCount++] = cbCardData[bPosition];
			cbCardData[bPosition] = cbCardData[cbBufferCount - cbRandCount];
		} while (cbRandCount < cbBufferCount);

//	}
	
	return;
}

void CGameLogic::AddBoss(BYTE cbBossCount, BYTE cbCardData[], BYTE cbCount)
{
	for (BYTE i=0; i<cbBossCount; i++)
	{
		for (BYTE j=0; j<cbCount; j++)
		{
			if (cbCardData[j] >= 0x32 && cbCardData[j] < 0x40)
			{
				cbCardData[j] = (i % 2) == 0 ? 0x4E : 0x4F;
				break;
			}
		}
	}
}

bool CGameLogic::DeleteCard(BYTE cbCard[], BYTE cbCount, BYTE cbDeleteCard[], BYTE cbDeleteCount)
{
	for (BYTE i=0; i<cbDeleteCount; i++)
	{
		bool bDelete = false;
		for (BYTE j=0; j<cbCount; j++)
		{
			if (cbCard[j] == cbDeleteCard[i])
			{
				cbCard[j] = 0;
				bDelete = true;
				break;
			}
		}
		if (!bDelete) return false;
	}
	SortCardByValue(cbCard, cbCount);
	return true;
}

void CGameLogic::SortCardByValue(BYTE cbCard[], BYTE cbCount)
{
	for (BYTE i = 0; i < cbCount - 1; i++)
	{
		for (BYTE j = i; j < cbCount; j++)
		{
			if (GetSortValue(cbCard[i]) < GetSortValue(cbCard[j]))
			{
				BYTE cbTemp = cbCard[i];
				cbCard[i] = cbCard[j];
				cbCard[j] = cbTemp;
			}
		}
	}
}

void CGameLogic::SortCardByType(BYTE cbCard[], BYTE cbCount)
{
	SortCardByValue(cbCard, cbCount);
	for (BYTE i = 4; i > 0; i--)
	{
		SortCardByNum(cbCard, cbCount, i);
	}
}

void CGameLogic::SortCardByNum(BYTE cbCard[], BYTE cbCount, BYTE cbNum)
{
	if (cbCount < cbNum) return;

	BYTE cbSortCount = 0;
	BYTE cbMaxIndex = cbCount - cbNum + 1;

	for (BYTE i = 0; i < cbMaxIndex;)
	{
		if (cbCard[i] >= 0x4E)
		{
			i++;
			continue;
		}
		BYTE cbMaxCount = GetLastIndex(cbCard, cbCount, cbCard[i]) - i + 1;
		if (cbMaxCount != cbNum)
		{
			i += cbMaxCount;
			continue;
		}
		if (cbMaxCount == cbNum)
		{
			for (BYTE j = 0; j < cbNum; j++)
			{
				BYTE cbTemp = cbCard[i];
				for (BYTE k = i; k < cbCount - 1; k++)
				{
					cbCard[k] = cbCard[k + 1];
				}
				cbCard[cbCount - 1] = cbTemp;
			}
			if (cbMaxIndex < cbNum) cbMaxIndex = 0;
			else cbMaxIndex -= cbNum;
		}
	}
}


void CGameLogic::SortBySpecial(BYTE cbCard[], BYTE cbCount, WORD wType)
{
	SortCardByValue(cbCard, cbCount);

	if (wType&TYPE_ZHIZUNQINGLONG || wType&TYPE_YITIAOLONG)
	{
		return;
	}

	if (wType == TYPE_SANTONGHUA || wType == TYPE_SANTONGHUASHUN)
	{
		SortByColor(cbCard, cbCount);
		return;
	}

	if (wType != TYPE_SANSHUNZI)
	{
		SortCardByType(cbCard, cbCount);
		return;
	}

	IsAllLine(cbCard, cbCount);

	BYTE cbTemp[13];
	for (int i=0; i<3; i++)
	{
		cbTemp[i] = cbCard[10 + i];
	}
	for (int i = 0; i < 10; i++)
	{
		cbTemp[3+i] = cbCard[i];
	}
	for (int i = 0; i < 13; i++)
	{
		cbCard[i] = cbTemp[i];
	}
}


void CGameLogic::SortByColor(BYTE cbCard[], BYTE cbCount)
{
	BYTE cbColor[4][13];
	BYTE cbColorCount[4];
	memset(cbColor, 0, sizeof(cbColor));
	memset(cbColorCount, 0, sizeof(cbColorCount));

	for (int i = 0; i < cbCount; i++)
	{
		BYTE color = GetCardColor(cbCard[i]);
		cbColor[color][cbColorCount[color]++] = cbCard[i];
	}
	int nCount = 0;

	for (int i = 0; i < 4; i++)
	{
		if(cbColorCount[i] == 0) continue;
		if (cbColorCount[i] == 5) continue;
		if (cbColorCount[i] == 10) continue;

		for (int j = 0; j < cbColorCount[i]; j++)
		{
			cbCard[nCount++] = cbColor[i][j];
		}
	}

	for (int i=0; i<4; i++)
	{
		if (cbColorCount[i] != 5 && cbColorCount[i] != 10) continue;

		for (int j=0; j<cbColorCount[i]; j++)
		{
			cbCard[nCount++] = cbColor[i][j];
		}
	}
}

BYTE CGameLogic::GetCardValue(BYTE cbCard)
{
	BYTE cbValue = cbCard & 0x0F;
	if (cbValue == 14 || cbValue == 15) return 15;

	return (cbValue == 1) ? (cbValue + 13) : cbValue;
}

BYTE CGameLogic::GetCardColor(BYTE cbCard)
{
	return cbCard >> 4;
}

BYTE CGameLogic::GetSortValue(BYTE cbCard)
{
	if (cbCard >= 0x4E) return cbCard;
	return GetCardValue(cbCard) * 4 + GetCardColor(cbCard);
}

BYTE CGameLogic::GetLastIndex(BYTE cbTemp[], BYTE cbCount, BYTE cbCard)
{
	for (BYTE i = cbCount - 1; i >= 0; i--)
	{
		if (GetCardValue(cbTemp[i]) == GetCardValue(cbCard))
			return i;
	}
	return 0;
}

WORD CGameLogic::GetCardType(BYTE cbCard[], BYTE cbCount, BYTE cbBossCount, bool bFive)
{
	WORD wType = HT_INVALID;

	BYTE cbTemp[13];

	ZeroMemory(cbTemp,sizeof(cbTemp));

	for (BYTE i = 0; i < cbCount; i++)
	{
		cbTemp[i] = cbCard[i];
	}
	SortCardByValue(cbTemp, cbCount);

	BYTE cbCarResult[5];
	for (BYTE i = 0; i < cbCount; i++)
	{
		cbCarResult[i] = cbTemp[i];
	}

	if (cbCount == 5 && IsLine(cbCarResult, cbCount,true))
	{
		for (BYTE i = 0; i < cbCount; i++)
		{
			cbCard[i] = cbCarResult[i];
		}
		return HT_LINE_COLOR;
	}
	else
	{
		if (IsSameColor(cbCarResult, cbCount))
		{
			return HT_COLOR;
		}
		if (cbCount == 5 && IsLine(cbCarResult, cbCount))
		{
			for (BYTE i = 0; i < cbCount; i++)
			{
				cbCard[i] = cbCarResult[i];
			}
			return HT_LINE;
		}
	}

	return GetOtherType(cbCard, cbCount, cbBossCount,bFive);
}

WORD CGameLogic::GetSpecialType(BYTE cbCard[], BYTE cbCount,WORD wAllType)
{
	//return TYPE_NULL;		//浙江十三道去掉了特殊牌型 by dcj
	BYTE cbValueCount[13];
	BYTE cbColor[4];
	BYTE cbLineTemp[13];
	ZeroMemory(cbValueCount, sizeof(cbValueCount));
	ZeroMemory(cbColor, sizeof(cbColor));

	for (int i = 0; i < cbCount; i++)
	{
		cbLineTemp[i] = cbCard[i];
		if (cbCard[i] >= 0x4E) return TYPE_NULL;
		cbValueCount[GetCardValue(cbCard[i])-2]++;
		cbColor[GetCardColor(cbCard[i])]++;
	}
	
	bool bLine = true;
	for (BYTE i=0; i<13; i++)
	{
		if (cbValueCount[i] == 0 || cbValueCount[i] > 1)
		{
			bLine = false;
			break;
		}
	}
	bool bColor = false;
	if (cbColor[0] == 13 || cbColor[1] == 13 || cbColor[2] == 13 || cbColor[3] == 13)
	{
		bColor = true;
	}

	/*if (bLine && bColor)
	{
		if (wAllType&TYPE_ZHIZUNQINGLONG) return TYPE_ZHIZUNQINGLONG;
	}
	else */if(bLine)
	{
		if (wAllType&TYPE_YITIAOLONG) return TYPE_YITIAOLONG;
	}

	BYTE cbNotKing = 0;
	BYTE cbSmall = 0;
	BYTE cbBombCount = 0;
	BYTE cbThreeCount = 0;
	BYTE cbTwoCount = 0;

	for (int i = 0; i < 13; i++)
	{
		if (cbValueCount[i] == 4)
		{
			cbBombCount++;
		}
		if (cbValueCount[i] == 3)
		{
			cbThreeCount++;
		}
		if (cbValueCount[i] == 2)
		{
			cbTwoCount++;
		}
		if (i < 9)
		{
			cbNotKing += cbValueCount[i];
		}
		if (i < 6)
		{
			cbSmall += cbValueCount[i];
		}
	}
	/*if (cbNotKing < 1)
	{
		if (wAllType&TYPE_SHIERHUANGZU) return TYPE_SHIERHUANGZU;
	}*/

	BYTE GetOutCard[8];
	BYTE GetOutNum = 0;
	bool bThree_C_L = true;
	bool bThree_C = true;

	BYTE cbSortCard[13];
	BYTE cbSortCount = 0;
	ZeroMemory(cbSortCard, sizeof(cbSortCard));

	for (int i = 0; i < 4; i++)
	{
		ZeroMemory(GetOutCard, sizeof(GetOutCard));
		GetOutNum = GetColorCard(cbLineTemp, i, GetOutCard);

		if (GetOutNum != 0 && GetOutNum != 3 && GetOutNum != 5 && GetOutNum != 8 && GetOutNum != 10 && GetOutNum != 13)
		{
			bThree_C = false;
			bThree_C_L = false;
		}
		else if(GetOutNum>=3)
		{
			SortCardByValue(GetOutCard,GetOutNum);
			if (IsAllLine(GetOutCard, GetOutNum, true))
			{
				for (BYTE i = 0; i < GetOutNum; i++)
				{
					cbSortCard[cbSortCount++] = GetOutCard[i];
				}
			}
			else
			{
				for (BYTE i = 0; i < GetOutNum; i++)
				{
					cbSortCard[cbSortCount++] = GetOutCard[i];
				}
				bThree_C_L = false;
			}
		}
	}

	/*if (bThree_C_L)
	{
		if (wAllType&TYPE_SANTONGHUASHUN)
		{
			for (BYTE i = 0; i < cbCount; i++)
			{
				cbCard[i] = cbSortCard[i];
			}

			return TYPE_SANTONGHUASHUN;
		}
	}*/

	for (int i = 0; i < cbCount; i++)
	{
		cbLineTemp[i] = cbCard[i];
	}

	/*if (cbBombCount == 3)
	{
		if (wAllType&TYPE_SANZHADAN) return TYPE_SANZHADAN;
	}
	if (cbSmall == 0)
	{
		if (wAllType&TYPE_QUANDA) return TYPE_QUANDA;
	}
	if (cbSmall == 13)
	{
		if (wAllType&TYPE_QUANXIAO) return TYPE_QUANXIAO;
	}
	if (cbColor[0] == 0 && cbColor[2] == 0)
	{
		if (wAllType&TYPE_COUYISE) return TYPE_COUYISE;
	}
	if (cbColor[1] == 0 && cbColor[3] == 0)
	{
		if (wAllType&TYPE_COUYISE) return TYPE_COUYISE;
	}
	if (cbThreeCount == 4)
	{
		if (wAllType&TYPE_SITAOSANTIAO) return TYPE_SITAOSANTIAO;
	}
	if (cbThreeCount == 3 && cbBombCount == 1)
	{
		if (wAllType&TYPE_SITAOSANTIAO) return TYPE_SITAOSANTIAO;
	}*/
	if (cbThreeCount == 1 && cbTwoCount == 5)
	{
		if (wAllType&TYPE_WUDUISANTIAO) return TYPE_WUDUISANTIAO;
	}
	if (cbTwoCount + cbThreeCount + cbBombCount*2 == 6)
	{
		if (wAllType&TYPE_LIUDUIBAN) return TYPE_LIUDUIBAN;
	}	
	if (bThree_C)
	{
		if (wAllType&TYPE_SANTONGHUA) 
		{
			for (BYTE i = 0; i < cbCount; i++)
			{
				cbCard[i] = cbSortCard[i];
			}
			return TYPE_SANTONGHUA;
		}
	}

	SortCardByValue(cbLineTemp, cbCount);
	if (IsAllLine(cbLineTemp,cbCount))
	{
		if (wAllType&TYPE_SANSHUNZI)
		{
			for (BYTE i = 0; i < cbCount; i++)
			{
				cbCard[i] = cbLineTemp[i];
			}
			return TYPE_SANSHUNZI;
		}

	}
	
	return TYPE_NULL;
}

BYTE CGameLogic::GetColorCard(BYTE cbCard[], BYTE cbColor, BYTE cbGetOutCard[])
{
	BYTE GetOutNum = 0;
	for (int i = 0; i < 13; i++)
	{
		if (GetCardColor(cbCard[i]) == cbColor)
		{
			cbGetOutCard[GetOutNum++] = cbCard[i];
		}
	}
	return GetOutNum;
}

bool CGameLogic::IsAllLine(BYTE cbCard[], BYTE cbCount, bool bSameColor)
{
	if (cbCount == 0) return true;
	if (cbCount != 3 && cbCount != 5 && cbCount != 8 && cbCount != 10 && cbCount != 13)
	{
		return false;
	}

	if (cbCount == 3 || cbCount == 5)
	{
		return IsLine(cbCard, cbCount, bSameColor);
	}

	BYTE cbTemp[13];
	memset(cbTemp, 0, sizeof(cbTemp));

	for (BYTE i = 0; i < cbCount; i++)
	{
		cbTemp[i] = cbCard[i];
	}

	BYTE cbIndex[5] = { 0,1,2,3,4 };

	do
	{
		BYTE cbCarResult[5];
		for (BYTE i = 0; i < 5; i++)
		{
			cbCarResult[i] = cbTemp[cbIndex[i]];
			cbTemp[cbIndex[i]] = 0;
		}
		SortCardByValue(cbTemp, cbCount);

		if (IsLine(cbCarResult, 5, bSameColor) && IsAllLine(cbTemp, cbCount - 5, bSameColor))
		{
			BYTE cbSortCount = 0;
			for (BYTE i = 0; i < 5; i++)
			{
				cbCard[cbSortCount++] = cbCarResult[i];
			}
			for (BYTE i = 0; i < cbCount - 5; i++)
			{
				cbCard[cbSortCount++] = cbTemp[i];
			}
			return true;
		}
		else
		{
			for (BYTE i = 0; i < cbCount; i++)
			{
				cbTemp[i] = cbCard[i];
			}
		}

		if (cbIndex[4] == (cbCount - 1))
		{
			BYTE i = 4;
			for (; i > 0; i--)
			{
				if ((cbIndex[i - 1] + 1) != cbIndex[i])
				{
					BYTE cbNewIndex = cbIndex[i - 1];
					for (BYTE j = (i - 1); j < 5; j++) cbIndex[j] = cbNewIndex + j - (i - 1) + 1;
					break;
				}
			}
			if (i == 0) break;
		}
		else cbIndex[4]++;
	} while (true);

	return false;
}

bool CGameLogic::IsLine(BYTE cbCard[], BYTE cbCount, bool bSameColor)
{
	BYTE cbBossCount = 0;
	BYTE cbFirst = 0xFF;
	BYTE cbSecond = 0xFF;
	BYTE cbLast = GetCardValue(cbCard[cbCount - 1]);
	BYTE cbSortCard[5];
	BYTE cbSortCount = 0;
	ZeroMemory(cbSortCard, sizeof(cbSortCard));
	for (BYTE i = 0; i < cbCount - 1; i++)
	{
		if (cbCard[i] >= 0x4E)
		{
			cbBossCount++;
			continue;
		}
		if (cbFirst == 0xFF) cbFirst = GetCardValue(cbCard[i]);
		else if (cbSecond == 0xFF) cbSecond = GetCardValue(cbCard[i]);
		if (bSameColor && (GetCardColor(cbCard[i]) != GetCardColor(cbCard[i + 1])))
		{
			return false;
		}
		if (GetCardValue(cbCard[i]) == GetCardValue(cbCard[i + 1]))
		{
			return false;
		}
	}

	if (cbFirst == 14 && cbSecond < (2+cbCount-1))
	{
		cbSortCard[0] = GetCardWithValue(cbCard, cbCount, 14);

		for (BYTE i=2; i<(2 + cbCount - 1); i++)
		{
			cbSortCard[i-1] = GetCardWithValue(cbCard, cbCount, i);
		}
		for (BYTE i=0;i<cbCount; i++)
		{
			cbCard[i] = cbSortCard[i];
		}
		return true;
	}
	if (cbFirst - cbLast + 1 <= cbCount)
	{
		if (cbFirst < (2 + cbCount - 1))
		{
			cbSortCard[0] = GetCardWithValue(cbCard, cbCount, 14);

			for (BYTE i = 2; i < (2 + cbCount - 1); i++)
			{
				cbSortCard[i - 1] = GetCardWithValue(cbCard, cbCount, i);
			}
		}
		else if (cbLast > 14 - cbCount)
		{
			for (BYTE i = 0; i < cbCount; i++)
			{
				cbSortCard[i] = GetCardWithValue(cbCard, cbCount, 14-i);
			}
		}
		else
		{
			for (BYTE i = 0; i < cbCount; i++)
			{
				cbSortCard[i] = GetCardWithValue(cbCard, cbCount, cbLast + cbCount - 1 - i);
			}
		}
		for (BYTE i=0;i<cbCount; i++)
		{
			cbCard[i] = cbSortCard[i];
		}
		return true;
	}

	return false;
}

bool CGameLogic::IsSameColor(BYTE cbCard[], BYTE cbCount)
{
	int nValue = 0;
	if (cbCount == 3) return nValue;
	for (BYTE i = 0; i < cbCount - 1; i++)
	{
		if (cbCard[i] >= 0x4E) continue;
		if (GetCardColor(cbCard[i]) != GetCardColor(cbCard[i + 1]))
			return false;
	}

	return true;
}

WORD CGameLogic::GetOtherType(BYTE cbCard[], BYTE cbCount, BYTE cbBossCount, bool bFive)
{
	BYTE cbTemp[13];
	memset(cbTemp, 0, sizeof(cbTemp));

	for (BYTE i = 0; i < cbCount; i++)
	{
		cbTemp[i] = cbCard[i];
	}

	SortCardByType(cbTemp, cbCount);

	BYTE cbHaveBossCount = 0;
	BYTE cb3Count[8];


	BYTE cbCard3[8][3];
	memset(cbCard3, 0, sizeof(cbCard3));
	memset(cb3Count, 0, sizeof(cb3Count));

	BYTE cbHaveOne = 0;
	BYTE cbHaveTwo = 0;
	BYTE cbHaveThree = 0;
	BYTE cbHaveFour = 0;
	BYTE cbHaveFive = 0;

	BYTE cbOneCard[5];
	BYTE cbTwoCard[2][2];
	BYTE cbThreeCard[3];
	BYTE cbFourCard[4];
	BYTE cbFiveCard[5];

	for (BYTE i = 0; i < cbCount; i++)
	{
		/*if (15 == GetCardValue(cbTemp[i]))
		cbBossCard[cbHaveBossCount++] = cbTemp[i];*/
		if (cbTemp[i] >= 0x4E)
		{
			cbHaveBossCount++;
			continue;
		}

		BYTE cbValue = GetCardValue(cbTemp[i]);
		if (cbValue >= 2 && cbValue < cbHaveBossCount+2)
		{
			cbCard3[cbValue-2][cb3Count[cbValue-2]++] = cbTemp[i];
		}
	}

	for (BYTE i = 0; i < cbCount; i++)
	{
		if (cbTemp[i] >= 0x4E) continue;
		BYTE cbMaxCount = GetLastIndex(cbTemp, cbCount, cbTemp[i]) - i + 1;

		if (cbMaxCount == 2)
		{
			cbTwoCard[cbHaveTwo][0] = cbTemp[i];
			cbTwoCard[cbHaveTwo][1] = cbTemp[i + 1];
			cbHaveTwo++;
			i++;
		}
		else if (cbMaxCount == 3)
		{
			BYTE cbValue = GetCardValue(cbTemp[i]);
			if (!bFive && cbValue >= 2 && cbValue < cbHaveBossCount+2)
			{
				i += 2;
				continue;
			}
			cbThreeCard[0] = cbTemp[i];
			cbThreeCard[1] = cbTemp[i + 1];
			cbThreeCard[2] = cbTemp[i + 2];
			cbHaveThree++;
			i += 2;
		}
		else if (cbMaxCount == 4)
		{
			BYTE cbValue = GetCardValue(cbTemp[i]);
			if (bFive && cbValue >= 2 && cbValue < cbHaveBossCount + 2)
			{
				i += 3;
				continue;
			}
			cbFourCard[0] = cbTemp[i];
			cbFourCard[1] = cbTemp[i + 1];
			cbFourCard[2] = cbTemp[i + 2];
			cbFourCard[3] = cbTemp[i + 3];
			cbHaveFour++;
			i += 3;
		}
		else if (cbMaxCount == 5)
		{
			cbFiveCard[0] = cbTemp[i];
			cbFiveCard[1] = cbTemp[i + 1];
			cbFiveCard[2] = cbTemp[i + 2];
			cbFiveCard[3] = cbTemp[i + 3];
			cbFiveCard[4] = cbTemp[i + 4];
			cbHaveFive++;
			i += 3;
		}
		else
		{
			cbOneCard[cbHaveOne++] = cbTemp[i];
		}
	}

	for (BYTE i=0; i<cbBossCount; i++)
	{
		/*if (cb3Count[i] == 4 && cbBossCount == 1)
		{
		return HT_SIX_BOOM;
		}
		if (cb3Count[i] == 4)
		{
		return HT_FIVE_BOOM;
		}
		if (bFive) continue;
		if (cb3Count[i] == 3 && cbBossCount == 2)
		{
		return HT_SIX_BOOM;
		}
		if (cb3Count[i] == 3 && cbBossCount == 1)
		{
		return HT_FIVE_BOOM;
		}*/
		if (cb3Count[i] == 3)
		{
			return HT_FOUR_BOOM;
		}
	}
// 	if (cbHaveFive) return HT_FIVE_BOOM;
// 	if (cbHaveFour && cbBossCount) return HT_FIVE_BOOM;
// 	if (cbHaveThree && cbBossCount == 2) return HT_FIVE_BOOM;
	if (cbHaveFour) return HT_FOUR_BOOM;
 	if (cbHaveThree && cbHaveBossCount == 1) return HT_FOUR_BOOM;
 	if (cbHaveTwo && cbHaveBossCount == 2) return HT_FOUR_BOOM;
	if (cbHaveThree && cbHaveTwo) return HT_THREE_DEOUBLE;
	if (cbHaveTwo >= 2 && cbHaveBossCount) return HT_THREE_DEOUBLE;
	if (cbHaveThree) return HT_THREE;
	if (cbHaveTwo && cbHaveBossCount) return HT_THREE;
	if (cbHaveBossCount == 2) return HT_THREE;
	if (cbHaveTwo == 2) return HT_TWO_DOUBLE;
	if (cbHaveTwo == 1) return HT_ONE_DOUBLE;
	if (cbHaveOne && cbHaveBossCount) return HT_ONE_DOUBLE;

	return HT_SINGLE;
}

BYTE CGameLogic::GetCardWithValue(BYTE cbCard[], BYTE cbCount, BYTE cbValue)
{
	BYTE cbTemp = 0;
	for (BYTE i = 0; i < cbCount; i++)
	{
		if (GetCardValue(cbCard[i]) == cbValue)
		{
			cbTemp = cbCard[i];
			cbCard[i] = 0;
			return cbTemp;
		}
	}
	for (BYTE i = 0; i < cbCount; i++)
	{
		if (GetCardValue(cbCard[i]) >= 15)
		{
			cbTemp = cbCard[i];
			cbCard[i] = 0;
			return cbTemp;
		}
	}
	return cbTemp;
}

bool CGameLogic::IsBigAwithB(BYTE cbCardA[], BYTE cbCountA, BYTE cbCardB[], BYTE cbCountB, bool bColor)
{
	BYTE cbBossA = 0;
	BYTE cbBossB = 0;

	BYTE cbTempA[5];
	BYTE cbTempB[5];
	BYTE cbTempIdxA[15];
	BYTE cbTempIdxB[15];
	ZeroMemory(cbTempA, sizeof(cbTempA));
	ZeroMemory(cbTempB, sizeof(cbTempB));
	ZeroMemory(cbTempIdxA,sizeof(cbTempIdxA));
	ZeroMemory(cbTempIdxB,sizeof(cbTempIdxB));

	for (BYTE i = 0; i < cbCountA; i++)
	{
		BYTE cbValue = GetCardValue(cbCardA[i]);
		cbTempIdxA[cbValue]++;
		cbValue = GetCardValue(cbCardB[i]);
		cbTempIdxB[cbValue]++;
	}

	for (BYTE i = 0; i < cbCountA; i++)
	{
		if (cbCardA[i] >= 0x4E)
		{
			cbBossA++;
			cbTempA[i] = 0;
		}
		else
		{
			cbTempA[i] = cbCardA[i];
		}
	}
	for (BYTE i = 0; i < cbCountB; i++)
	{
		if (cbCardB[i] >= 0x4E)
		{
			cbBossB++;
			cbTempB[i] = 0;
		}
		else
		{
			cbTempB[i] = cbCardB[i];
		}
	}

	SortCardByType(cbTempA, cbCountA);
	SortCardByType(cbTempB, cbCountB);

	/*
	for (BYTE i=0; i<cbCountA; i++)
	{
		BYTE cbValue = GetCardValue(cbCardA[i]);
		if(cbTempIdxA[cbValue] > 1)
		{
			for(BYTE j = 0;j<cbCountB;i++)
			if(cbCardA[i] > cbCardB[i])
				return true;
			else if(cbCardA[i] < cbCardB[i])
				return false;
		}
	}
	for (BYTE i=0; i<cbCountA; i++)
	{
		BYTE cbValue = GetCardValue(cbCardA[i]);
		if(cbTempIdxA[cbValue] <= 1)
		{
			continue;
		}
		if(cbTempA[i] > cbTempB[i])
		{
			if (cbBossB == 0)
				return true;
			else
				cbBossB--;
		}
		else if (cbTempA[i] < cbTempB[i])
		{
			if (cbBossA == 0)
				return false;
			else
				cbBossA--;
		}
	}*/


	for (BYTE i=0; i<cbCountA; i++)
	{	
		if (bColor)
		{
			if(cbTempA[i] > cbTempB[i])
			{
				if (cbBossB == 0)
					return true;
				else
					cbBossB--;
			}
			else if (cbTempA[i] < cbTempB[i])
			{
				if (cbBossA == 0)
					return false;
				else
					cbBossA--;
			}
		}
		else
		{
			if (GetCardValue(cbTempA[i]) > GetCardValue(cbTempB[i]))
			{
				if (cbBossB == 0)
					return true;
				else
					cbBossB--;
			}
			else if (GetCardValue(cbTempA[i]) < GetCardValue(cbTempB[i]))
			{
				if (cbBossA == 0)
					return false;
				else
					cbBossA--;
			}
		}
	}

	//比较花色
	for (BYTE i = 0; i < cbCountA; i++)
	{
		BYTE cbValue = GetCardValue(cbTempA[i]);
		if(cbTempIdxA[cbValue] == 1)
		{
			if ( GetCardColor(cbTempA[i])> GetCardColor(cbTempB[i]))
			{
				return true;
			}
			else if (GetCardColor(cbTempA[i])< GetCardColor(cbTempB[i]))
			{
				return false;
			}
		}
	}
	return false;

}

bool CGameLogic::IsBigLineAwithB(BYTE cbCardA[], BYTE cbCountA, BYTE cbCardB[], BYTE cbCountB, bool bColor)
{
	BYTE cbValueA = 0;
	BYTE cbValueB = 0;

	bool bSpecial = true;
	for (BYTE i=0; i<cbCountA; i++)
	{
		if(cbCardA[i] >= 0x4E) continue;
		if (GetCardValue(cbCardA[i]) < 14 && GetCardValue(cbCardA[i]) > cbCountA)
		{
			bSpecial = false;
			break;
		}
	}
	if (bSpecial) cbValueA = 14;
	else
	{
		cbValueA = (GetCardValue(cbCardA[cbCountA - 1]) + 4) >= 14 ? 15 : (GetCardValue(cbCardA[cbCountA - 1]) + 4);
	}
	bSpecial = true;
	for (BYTE i = 0; i < cbCountB; i++)
	{
		if (cbCardB[i] >= 0x4E) continue;
		if (GetCardValue(cbCardB[i]) < 14 && GetCardValue(cbCardB[i]) > cbCountB)
		{
			bSpecial = false;
			break;
		}
	}
	if (bSpecial) cbValueB = 14;
	else
	{
		cbValueB = (GetCardValue(cbCardB[cbCountB - 1]) + 4) >= 14 ? 15 : (GetCardValue(cbCardB[cbCountB - 1]) + 4);
	}

	if (cbValueA > cbValueB) return true;

	if (cbValueA == cbValueB && bColor)
	{
		for (BYTE i=0;i<cbCountA; i++)
		{
			if (cbCardA[i] > cbCardB[i]) return true;
			if (cbCardA[i] < cbCardB[i]) return false;
		}
	}

	//比较花色
	if (cbValueA == cbValueB )
	{
		for (BYTE i = 0; i<cbCountA; i++)
		{
			if (cbCardA[i] > cbCardB[i]) return true;
			if (cbCardA[i] < cbCardB[i]) return false;
		}
	}

	return false;
}

bool CGameLogic::IsBigSpecialAwithB(WORD wTypeA, WORD wTypeB)
{
	WORD wValueA = wTypeA;
	WORD wValueB = wTypeB;

	if (wTypeA > TYPE_NULL && wTypeA < TYPE_WUDUISANTIAO)
		wValueA = TYPE_SANTONGHUA;

	if (wTypeB > TYPE_NULL && wTypeB < TYPE_WUDUISANTIAO)
		wValueB = TYPE_SANTONGHUA;

	return wValueA > wValueB;
}

BYTE CGameLogic::GetBossCount(BYTE cbCard[], BYTE cbCount)
{
	BYTE cbBossCount = 0;
	for (BYTE i=0; i<cbCount; i++)
	{
		if (cbCard[i] >= 0x4E)
			cbBossCount++;
	}
	return cbBossCount;
}




//自动配牌需要的函数

WORD CGameLogic::GetCardType(AnalyseResult* pHead, BYTE cbCard[], BYTE cbCount, bool bHaveBoss)
{
	WORD wType = HT_INVALID;
	BYTE cbTemp[13];
	memset(cbTemp, 0, sizeof(cbTemp));
	for (BYTE i = 0; i < cbCount; i++)
	{
		cbTemp[i] = cbCard[i];
	}
	SortCardByValue(cbTemp, cbCount);

	if (cbCount < 5)
	{
		return SaveOtherType(pHead, cbTemp, cbCount, bHaveBoss);
	}

	BYTE cbIndex[5] = { 0,1,2,3,4 };

	do
	{
		BYTE cbCarResult[5];
		for (BYTE i = 0; i < 5; i++)
		{
			cbCarResult[i] = cbTemp[cbIndex[i]];
		}

		int nValue;
		nValue = IsSameColorLine(cbCarResult, 5);
		if (nValue)
		{
			PushResult(pHead, cbCarResult, HT_LINE_COLOR, nValue);
			wType |= HT_LINE_COLOR;
		}
		else
		{
			nValue = IsSameColor(cbCarResult, 5);
			if (nValue)
			{
				PushResult(pHead, cbCarResult, HT_COLOR, nValue);
				wType |= HT_COLOR;
			}
			nValue = IsLine(cbCarResult, 5);
			if (nValue)
			{
				PushResult(pHead, cbCarResult, HT_LINE, nValue);
				wType |= HT_LINE;
			}
		}

		if (cbIndex[4] == (cbCount - 1))
		{
			BYTE i = 4;
			for (; i > 0; i--)
			{
				if ((cbIndex[i - 1] + 1) != cbIndex[i])
				{
					BYTE cbNewIndex = cbIndex[i - 1];
					for (BYTE j = (i - 1); j < 5; j++) cbIndex[j] = cbNewIndex + j - (i - 1) + 1;
					break;
				}
			}
			if (i == 0) break;
		}
		else cbIndex[4]++;

	} while (true);

	wType |= SaveOtherType(pHead, cbTemp, cbCount, bHaveBoss);

	return wType;
}

void CGameLogic::PushResult(AnalyseResult* pHead, AnalyseResult*pResult)
{
	AnalyseResult* p = pHead;
	if (p->pNext==nullptr)
	{
		p->pNext = pResult;
		return;
	}
	if (p->pNext->wType > pResult->wType)
	{
		return;
	}
	int iCount = 0;
	for (int i = 0;i!=5;i++)
	{
		if(pResult->cbCard[i] != 0)
		{
			iCount++;
		}
	}
	if (p->pNext->wType == pResult->wType)
	{
		if (p->pNext->wType == HT_LINE || p->pNext->wType == HT_LINE_COLOR)
		{
			if (IsBigLineAwithB(pResult->cbCard,iCount,p->pNext->cbCard,iCount))
			{
				p->pNext = pResult;
			}
		}
		else
		{
			if (IsBigAwithB(pResult->cbCard,iCount,p->pNext->cbCard,iCount))
			{
				p->pNext = pResult;
			}
		}
		return;
	}
	p->pNext=pResult;
}

void CGameLogic::PushResult(AnalyseResult* pHead, BYTE cbCard[], WORD wType, int nSortValue)
{
	AnalyseResult*pResult = new AnalyseResult;
	for (BYTE i = 0; i < 5; i++)
	{
		pResult->cbCard[i] = cbCard[i];
	}
	pResult->nSortValue = 0;
	pResult->wType = wType;
	pResult->pNext = nullptr;
	pResult->nSortValue = nSortValue;
	PushResult(pHead, pResult);
}

void CGameLogic::PushResult(AnalyseResult* pHead, BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4, BYTE cbCard5, WORD wType)
{
	BYTE cbTemp[5];

	cbTemp[0] = cbCard1;
	cbTemp[1] = cbCard2;
	cbTemp[2] = cbCard3;
	cbTemp[3] = cbCard4;
	cbTemp[4] = cbCard5;

	int nValue = GetCardValue(cbCard1)*4000 + GetCardValue(cbCard2)*255 + GetCardValue(cbCard3) * 15 + 20 - GetCardValue(cbCard5);

	//int nValue = cbCard1 * 40 + cbCard2 * 2 + cbCard3 + cbCard4 + cbCard5;
	PushResult(pHead, cbTemp, wType, nValue);
}

WORD CGameLogic::SaveOtherType(AnalyseResult* pHead, BYTE cbCard[], BYTE cbCount, bool bHaveBoss)
{
	WORD wType = HT_SINGLE;

	BYTE cbTemp[13];
	memset(cbTemp, 0, sizeof(cbTemp));

	for (BYTE i = 0; i < cbCount; i++)
	{
		cbTemp[i] = cbCard[i];
	}

	SortCardByType(cbTemp, cbCount);
	
	BYTE cbBossCount = 0;
	BYTE cbTwoCount = 0;
	BYTE cbThreeCount = 0;

	BYTE cbBossCard[2];
	BYTE cbCard2[3];
	BYTE cbCard3[3];

	BYTE cbHaveOne = 0;
	BYTE cbHaveTwo = 0;
	BYTE cbHaveThree = 0;
	BYTE cbHaveFour = 0;
	BYTE cbHaveFive = 0;

	BYTE cbOneCard[13];
	BYTE cbTwoCard[6][2];
	BYTE cbThreeCard[4][3];
	BYTE cbFourCard[3][4];
	BYTE cbFiveCard[2][5];

	for (BYTE i = 0; i < cbCount; i++)
	{
		if (cbTemp[i] >= 0x4E)
		{
			cbBossCard[cbBossCount++] = cbTemp[i];
		}
		
		if (!bHaveBoss) continue;
		if (2 == GetCardValue(cbTemp[i]))
			cbCard2[cbTwoCount++] = cbTemp[i];
		if (3 == GetCardValue(cbTemp[i]))
			cbCard3[cbThreeCount++] = cbTemp[i];
	}

	for (BYTE i = 0; i < cbCount; i++)
	{
		if (cbTemp[i] >= 0x4E) continue;
		if (cbTemp[i] == 0) continue;

		BYTE cbMaxCount = GetLastIndex(cbTemp, cbCount, cbTemp[i]) - i + 1;
	
		if (cbMaxCount == 2)
		{
			cbTwoCard[cbHaveTwo][0] = cbTemp[i];
			cbTwoCard[cbHaveTwo][1] = cbTemp[i+1];
			cbHaveTwo++;
			i++;
		}
		else if (cbMaxCount == 3)
		{
			/*if (bHaveBoss && (GetCardValue(cbTemp[i]) == 2 || GetCardValue(cbTemp[i]) == 3))
			{
				i += 2;
				continue;
			}*/
			cbThreeCard[cbHaveThree][0] = cbTemp[i];
			cbThreeCard[cbHaveThree][1] = cbTemp[i+1];
			cbThreeCard[cbHaveThree][2] = cbTemp[i+2];
			cbHaveThree++;
			i += 2;
		}
		else if (cbMaxCount == 4)
		{
			cbFourCard[cbHaveFour][0] = cbTemp[i];
			cbFourCard[cbHaveFour][1] = cbTemp[i+1];
			cbFourCard[cbHaveFour][2] = cbTemp[i+2];
			cbFourCard[cbHaveFour][3] = cbTemp[i+3];
			cbHaveFour++;
			i += 3;
		}
		else if (cbMaxCount == 5)
		{
			cbFiveCard[cbHaveFive][0] = cbTemp[i];
			cbFiveCard[cbHaveFive][1] = cbTemp[i + 1];
			cbFiveCard[cbHaveFive][2] = cbTemp[i + 2];
			cbFiveCard[cbHaveFive][3] = cbTemp[i + 3];
			cbFiveCard[cbHaveFive][4] = cbTemp[i + 4];
			cbHaveFive++;
			i += 4;
		}
		else
		{
			cbOneCard[cbHaveOne++] = cbTemp[i];
		}
	}

	/*if (cbTwoCount == 3 && cbBossCount == 2)
	{
		wType |= HT_SIX_BOOM;
		PushResult(pHead, cbCard2[0], cbCard2[1], cbCard2[2], cbBossCard[0], cbBossCard[1], HT_SIX_BOOM);
	}
	if (cbThreeCount == 3 && cbBossCount == 2)
	{
		wType |= HT_SIX_BOOM;
		PushResult(pHead, cbCard3[0], cbCard3[1], cbCard3[2], cbBossCard[0], cbBossCard[1], HT_SIX_BOOM);
	}

	if (cbTwoCount == 3 && cbBossCount)
	{
		wType |= HT_FIVE_BOOM;
		if(cbHaveOne) PushResult(pHead, cbCard2[0], cbCard2[1], cbCard2[2], cbBossCard[0], cbOneCard[cbHaveOne-1], HT_FIVE_BOOM);
		else PushResult(pHead, cbCard2[0], cbCard2[1], cbCard2[2], cbBossCard[0], 0, HT_FIVE_BOOM);
	}
	if (cbThreeCount == 3 && cbBossCount)
	{
		wType |= HT_FIVE_BOOM;
		if (cbHaveOne) PushResult(pHead, cbCard3[0], cbCard3[1], cbCard3[2], cbBossCard[0], cbOneCard[cbHaveOne - 1], HT_FIVE_BOOM);
		else PushResult(pHead, cbCard3[0], cbCard3[1], cbCard3[2], cbBossCard[0], 0, HT_FIVE_BOOM);
	}
	if (cbHaveFour && cbBossCount)
	{
		wType |= HT_FIVE_BOOM;
		for (BYTE i = 0; i < cbHaveFour; i++)
			PushResult(pHead, cbFourCard[i][0], cbFourCard[i][1], cbFourCard[i][2], cbFourCard[i][3], cbBossCard[0], HT_FIVE_BOOM);
	}
	if (cbHaveThree && cbBossCount == 2)
	{
		wType |= HT_FIVE_BOOM;
		for (BYTE i = 0; i < cbHaveThree; i++)
			PushResult(pHead, cbThreeCard[i][0], cbThreeCard[i][1], cbThreeCard[i][2], cbBossCard[0], cbBossCard[1], HT_FIVE_BOOM);
	}
*/
	if (cbTwoCount == 3)
	{
		wType |= HT_TWO_DOUBLE;
		if (cbHaveOne >= 2)
		{
			PushResult(pHead, cbCard2[0], cbCard2[1], cbCard2[2], cbOneCard[cbHaveOne - 2], cbOneCard[cbHaveOne - 1], HT_TWO_DOUBLE);
		}
		else
		{
			PushResult(pHead, cbCard2[0], cbCard2[1], cbCard2[2], 0, 0, HT_TWO_DOUBLE);
		}
	}
	if (cbThreeCount == 3)
	{
		wType |= HT_THREE;
		if (cbHaveOne >= 2)
		{
			PushResult(pHead, cbCard3[0], cbCard3[1], cbCard3[2], cbOneCard[cbHaveOne - 2], cbOneCard[cbHaveOne - 1], HT_THREE);
		}
		else
		{
			PushResult(pHead, cbCard3[0], cbCard3[1], cbCard3[2], 0, 0, HT_THREE);
		}
	}
	/*if (cbHaveFive)
	{
		wType |= HT_FIVE_BOOM;
		for (BYTE i = 0; i < cbHaveFive; i++)
			PushResult(pHead, cbFiveCard[i][0], cbFiveCard[i][1], cbFiveCard[i][2], cbFiveCard[i][3], cbFiveCard[i][4], HT_FIVE_BOOM);
	}*/
	if (cbHaveFour)
	{
		wType |= HT_FOUR_BOOM;
		BYTE cbData = 0;
		if (cbHaveOne >= 1) cbData = cbOneCard[cbHaveOne - 1];
		else if (cbHaveTwo >= 1) cbData = cbTwoCard[cbHaveTwo - 1][0];
		else if (cbHaveThree >= 1) cbData = cbThreeCard[cbHaveThree - 1][0];
		for (BYTE i = 0; i < cbHaveFour; i++)
			PushResult(pHead, cbFourCard[i][0], cbFourCard[i][1], cbFourCard[i][2], cbFourCard[i][3], cbData, HT_FOUR_BOOM);
	}
	if (cbHaveThree && cbBossCount >= 1)
	{
		wType |= HT_FOUR_BOOM;
		BYTE cbData = 0;
		if (cbHaveOne >= 1) cbData = cbOneCard[cbHaveOne - 1];
		for (BYTE i = 0; i < cbHaveThree; i++)
			PushResult(pHead, cbThreeCard[i][0], cbThreeCard[i][1], cbThreeCard[i][2], cbBossCard[0], cbData, HT_FOUR_BOOM);
	}
	if (cbHaveTwo && cbBossCount == 2)
	{
		wType |= HT_FOUR_BOOM;
		BYTE cbData = 0;
		if (cbHaveOne >= 1) cbData = cbOneCard[cbHaveOne - 1];
		for (BYTE i = 0; i < cbHaveTwo; i++)
			PushResult(pHead, cbTwoCard[i][0], cbTwoCard[i][1], cbBossCard[0], cbBossCard[1], cbData, HT_FOUR_BOOM);
	}

	if (cbHaveThree && cbHaveTwo)
	{
		wType |= HT_THREE_DEOUBLE;
		for (BYTE j = cbHaveTwo; j >0; j--)
		{
			for (BYTE i = 0; i < cbHaveThree; i++)
				PushResult(pHead, cbThreeCard[i][0], cbThreeCard[i][1], cbThreeCard[i][2], cbTwoCard[j-1][0], cbTwoCard[j-1][1], HT_THREE_DEOUBLE);
		}
	}
	else if (cbHaveThree >= 2)
	{
		wType |= HT_THREE_DEOUBLE;
		for (BYTE i = 0; i < cbHaveThree; i++)
		{
			for (BYTE j = cbHaveThree; j > 0; j--)
			{
				if(j == i) continue;
				PushResult(pHead, cbThreeCard[i][0], cbThreeCard[i][1], cbThreeCard[i][2], cbThreeCard[j - 1][0], cbThreeCard[j - 1][1], HT_THREE_DEOUBLE);
			}
		}
	}

	if (cbHaveTwo>=2 && cbBossCount)
	{
		wType |= HT_THREE_DEOUBLE;
		for (BYTE i = 0; i < cbHaveTwo; i++)
			PushResult(pHead, cbTwoCard[i][0], cbTwoCard[i][1], cbBossCard[0], cbTwoCard[cbHaveTwo - 1][0], cbTwoCard[cbHaveTwo - 1][1], HT_THREE_DEOUBLE);
	}

	if (cbHaveThree)
	{
		wType |= HT_THREE;
		BYTE cbData1 = 0;
		BYTE cbData2 = 0;
		if (cbHaveOne >= 1)
		{
			cbData2 = cbOneCard[cbHaveOne - 1];
		}
		else if (cbHaveTwo >= 1) cbData2 = cbTwoCard[cbHaveTwo - 1][0];
		else if (cbHaveThree >= 1) cbData2 = cbThreeCard[cbHaveThree - 1][0];

		if (cbHaveOne >= 2)
		{
			cbData1 = cbOneCard[cbHaveOne - 2];
		}
		else if (cbHaveTwo >= 1) cbData1 = cbTwoCard[cbHaveTwo - 1][0];
		else if (cbHaveThree >= 1) cbData1 = cbThreeCard[cbHaveThree - 1][0];

		for (BYTE i = 0; i < cbHaveThree; i++)
			PushResult(pHead, cbThreeCard[i][0], cbThreeCard[i][1], cbThreeCard[i][2], cbData1, cbData2, HT_THREE);
	}
	if (cbHaveTwo && cbBossCount)
	{
		wType |= HT_THREE;
		BYTE cbData1 = 0;
		BYTE cbData2 = 0;
		if (cbHaveOne >= 2)
		{
			cbData1 = cbOneCard[cbHaveOne - 2];
			cbData2 = cbOneCard[cbHaveOne - 1];
		}
		for (BYTE i = 0; i < cbHaveTwo; i++)
			PushResult(pHead, cbTwoCard[i][0], cbTwoCard[i][1], cbBossCard[0], cbData1, cbData2, HT_THREE);
	}
	if (cbBossCount == 2)
	{
		wType |= HT_THREE;
		BYTE cbData1 = 0;
		BYTE cbData2 = 0;
		if (cbHaveOne >= 3)
		{
			cbData1 = cbOneCard[cbHaveOne - 2];
			cbData2 = cbOneCard[cbHaveOne - 1];

			for (BYTE i = 0; i < cbHaveOne - 2; i++)
				PushResult(pHead, cbOneCard[i], cbBossCard[0], cbBossCard[1], cbData1, cbData2, HT_THREE);
		}
		if (cbHaveOne >= 1)
		{
			for (BYTE i = cbHaveOne -2; i < cbHaveOne; i++)
				PushResult(pHead, cbOneCard[i], cbBossCard[0], cbBossCard[1], 0, 0, HT_THREE);
		}
		else
		{
			for (BYTE i = cbBossCount; i < cbCount; i++)
				PushResult(pHead, cbTemp[i], cbBossCard[0], cbBossCard[1], 0, 0, HT_THREE);
		}
	}

	if (cbHaveTwo >= 2)
	{
		wType |= HT_TWO_DOUBLE;
		BYTE cbData = 0;
		if (cbHaveOne) cbData = cbOneCard[cbHaveOne - 1];
		for (BYTE i = 0; i < cbHaveTwo - 1; i++)
		{
			if (cbData == 0 || GetCardValue(cbData) == GetCardValue(cbTwoCard[i][0]))
			{
				for (size_t j = 0; j < cbHaveTwo - 1; j++)
				{
					if (j != i)
					{
						cbData = cbTwoCard[j][0];
					}
				}
			}

			PushResult(pHead, cbTwoCard[i][0], cbTwoCard[i][1], cbTwoCard[cbHaveTwo - 1][0], cbTwoCard[cbHaveTwo - 1][1], cbData, HT_TWO_DOUBLE);
		}
	}

	if (cbHaveTwo)
	{
		wType |= HT_ONE_DOUBLE;
		BYTE cbData1 = 0;
		BYTE cbData2 = 0;
		BYTE cbData3 = 0;
		if (cbHaveOne>=3)
		{
			cbData1 = cbOneCard[cbHaveOne - 3];
			cbData2 = cbOneCard[cbHaveOne - 2];
			cbData3 = cbOneCard[cbHaveOne - 1];
		}
		for (BYTE i = 0; i < cbHaveTwo; i++)
			PushResult(pHead, cbTwoCard[i][0], cbTwoCard[i][1], cbData1, cbData2, cbData3, HT_ONE_DOUBLE);
	}
	if (cbHaveOne && cbBossCount)
	{
		wType |= HT_ONE_DOUBLE;
		BYTE cbData1 = 0;
		BYTE cbData2 = 0;
		BYTE cbData3 = 0;
		if (cbHaveOne >= 4)
		{
			cbData1 = cbOneCard[cbHaveOne - 3];
			cbData2 = cbOneCard[cbHaveOne - 2];
			cbData3 = cbOneCard[cbHaveOne - 1];
			for (BYTE i = 0; i < cbHaveOne - 3; i++)
				PushResult(pHead, cbOneCard[i], cbBossCard[0], cbData1, cbData2, cbData3, HT_ONE_DOUBLE);
		}
		else
		{
			for (BYTE i = 0; i < cbHaveOne; i++)
				PushResult(pHead, cbOneCard[i], cbBossCard[0], cbData1, cbData2, cbData3, HT_ONE_DOUBLE);
		}
	}

	return wType;
}

AnalyseResult* CGameLogic::GetNextData(AnalyseResult* pHead, AnalyseResult* pCurrent, WORD wType)
{
	int nMaxValue = 0;
	int nSecondValue = 0;

	AnalyseResult* pBest = nullptr;
	AnalyseResult* p = pHead;

	if (pCurrent) nMaxValue = pCurrent->nSortValue;

	while (p)
	{
		if (p!=pCurrent && p->wType == wType)
		{
			if (pCurrent == nullptr)
			{
				if (p->nSortValue > nMaxValue)
				{
					nMaxValue = p->nSortValue;
					pBest = p;
				}
			}
			else if (p->nSortValue > nSecondValue && (p->nSortValue < nMaxValue))
			{
				nSecondValue = p->nSortValue;
				pBest = p;
			}
		}
		p = p->pNext;
	}
	if (pBest != nullptr) return pBest;

	p = pHead;
	while (p)
	{
		if (p->wType == wType)
		{
			if (p->nSortValue >= nMaxValue)
			{
				nMaxValue = p->nSortValue;
				pBest = p;
			}
		}
		p = p->pNext;
	}

	return pBest;
}


int CGameLogic::IsSameColorLine(BYTE cbCard[], BYTE cbCount)
{
	int nValue = 0;
	BYTE cbBossCount = 0;
	BYTE cbFirst = 0xFF;
	BYTE cbSecond = 0xFF;
	BYTE cbLast = GetCardValue(cbCard[cbCount - 1]);
	for (BYTE i = 0; i < cbCount - 1; i++)
	{
		if (cbCard[i] >= 0x4E)
		{
			cbBossCount++;
			continue;
		}
		if (cbFirst == 0xFF) cbFirst = GetCardValue(cbCard[i]);
		else if (cbSecond == 0xFF) cbSecond = GetCardValue(cbCard[i]);
		if (GetCardColor(cbCard[i]) != GetCardColor(cbCard[i + 1]))
			return nValue;
		if (GetCardValue(cbCard[i]) == GetCardValue(cbCard[i + 1]))
			return nValue;
	}

	for (BYTE i = 0; i < 5; i++)
	{
		nValue += (GetCardValue(cbCard[i]) * 4 + GetCardColor(cbCard[i])) * 100;
	}

	if (cbFirst == 14 && cbSecond < 6) return 14;
	if (cbFirst - cbLast + 1 <= cbCount) return (cbLast + 4) >= 14 ? 15 : (cbLast + 4);

	return 0;
}

//////////////////////////////////////////////////////////////////////////