#pragma once
#include <string>
#include <vector>

class CBaseConfig
{
public:
	CBaseConfig();
	~CBaseConfig();

	static std::string SetConfigPathName(std::string cfgname);
	static bool		  GetKeyValueUInt(std::string strBigKey,std::string strLitKey,unsigned int& nValue);
	static bool		  GetKeyValueStr(std::string strBigKey,std::string strLitKey,std::string& strValue);
	static bool		  GetCardConf(unsigned char roundIdx,unsigned char userCnt,std::vector<BYTE> &cardConf, unsigned int &zhuang);
	static bool		  GetRateUser(std::vector<unsigned int> &vUserID,unsigned int &nRate);
private:
	static std::string m_strPath;
};