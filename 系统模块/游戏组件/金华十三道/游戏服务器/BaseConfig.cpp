#include "stdafx.h"
#include "BaseConfig.h"

std::string CBaseConfig::m_strPath = "";
CBaseConfig::CBaseConfig()
{
}

CBaseConfig::~CBaseConfig()
{
}

std::string CBaseConfig::SetConfigPathName( std::string cfgname )
{
	char  szPath[MAX_PATH] = {0};
	::GetModuleFileNameA(NULL, szPath, MAX_PATH);
	std::string szDirector = szPath;
	szDirector = szDirector.substr(0, szDirector.rfind("\\"));
	szDirector += "\\";
	szDirector += cfgname;
	m_strPath = szDirector;
	return szDirector;
}

bool CBaseConfig::GetKeyValueUInt(std::string strBigKey,std::string strLitKey,unsigned int& nValue )
{
	if(m_strPath.empty())
		return false;
	nValue = ::GetPrivateProfileIntA(strBigKey.c_str(),strLitKey.c_str(), 0, m_strPath.c_str());
	return true;
}

bool CBaseConfig::GetKeyValueStr( std::string strBigKey,std::string strLitKey,std::string& strValue )
{
	char szBuffer[1024*4] = {0};
	if(m_strPath.empty())
		return false;
	::GetPrivateProfileStringA(strBigKey.c_str(), strLitKey.c_str(), "", szBuffer, 1024*4, m_strPath.c_str());
	strValue = szBuffer;
	return true;
}

bool CBaseConfig::GetCardConf(unsigned char roundIdx,unsigned char userCnt,std::vector<BYTE> &cardConf, unsigned int &zhuang)
{
	char szRound[32] = {0};
	char szUser[32] = {0};
	if(userCnt > 5)
		userCnt = 5;
	sprintf_s(szRound,sizeof(szRound)-1,"round%d",roundIdx);
	GetKeyValueUInt(szRound,"zhuang",zhuang);

	//º”‘ÿÕÊº“≈∆
	for(unsigned char i = 1;i <= userCnt;i++)
	{
		memset(szUser,0,32);
		std::string strCard;
		sprintf_s(szUser,sizeof(szUser)-1,"user%d",i);
		if(GetKeyValueStr(szRound,szUser,strCard))
		{
			int pos = 0;
			int startpos = 0;
			int idx = 0;
			while ((startpos = strCard.find("0x", pos)) != std::string::npos)
			{
				if(idx > 12)
					break;
				int endpos = strCard.find(",", startpos);
				if (endpos == std::string::npos)
				{
					endpos = strCard.find('\n', startpos);
				}
				std::string substr = strCard.substr(startpos, endpos - startpos);

				int value = 0;
				int ret  = sscanf(substr.c_str(), "%x", &value);
				if (ret == 1)
				{
					cardConf.push_back(value);
				}
				idx++;
				pos = endpos;
			}
			while(idx++ <= 12)
				cardConf.push_back(0);
		}
	}

	if(cardConf.empty())
		return false;
	return true;
}

bool CBaseConfig::GetRateUser(std::vector<unsigned int> &vUserID,unsigned int &nRate)
{
	char szUser[32] = {0};
	std::string bigKey = "param";
	GetKeyValueUInt(bigKey,"rate",nRate);
	for(unsigned char i = 1;i <= 100;i++)
	{
		memset(szUser,0,32);
		unsigned int nUserId = 0;
		sprintf_s(szUser,sizeof(szUser)-1,"user%d",i);
		if(GetKeyValueUInt(bigKey,szUser,nUserId))
		{
			if (nUserId == 0)
			{
				break;
			}
			vUserID.push_back(nUserId);
		}
		else
		{
			break;
		}
	}
	if(vUserID.empty())
		return false;
	return true;
}