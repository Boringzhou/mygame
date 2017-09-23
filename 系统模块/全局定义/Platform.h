#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//定义文件
#include "Macro.h"
#include "Define.h"

//结构文件
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//模板文件
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//////////////////////////////////////////////////////////////////////////////////

//最后一位不提示更新 中间位不强制更新  第一位强制更新
//程序版本
#define MainVer 8
#define SubVer 0
#define BuildVer 1

#define VERSION_FRAME				PROCESS_VERSION(MainVer,SubVer,BuildVer)				//框架版本
#define VERSION_PLAZA				PROCESS_VERSION(MainVer,SubVer,BuildVer)				//大厅版本
#define VERSION_MOBILE_ANDROID		PROCESS_VERSION(MainVer,SubVer,BuildVer)				//手机版本
#define VERSION_MOBILE_IOS			PROCESS_VERSION(MainVer,SubVer,BuildVer)				//手机版本

//版本定义
#define VERSION_EFFICACY			1									//效验版本
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,1)				//框架版本

//////////////////////////////////////////////////////////////////////////////////
//发布版本

const TCHAR szProduct[]=TEXT("一点点电玩城");							//产品名字
const TCHAR szProductKey[]=TEXT("YDDGamePlatform");					//产品主键

//////////////////////////////////////////////////////////////////////////////////

//数据库名
const TCHAR szPlatformDB[]=TEXT("THPlatformDB");						//平台数据库
const TCHAR szAccountsDB[]=TEXT("THAccountsDB");						//用户数据库
const TCHAR szTreasureDB[]=TEXT("THTreasureDB");						//财富数据库
const TCHAR szGameMatchDB[]=TEXT("THGameMatchDB");						//比赛数据库
const TCHAR szExerciseDB[]=TEXT("THExerciseDB");						//练习数据库

//////////////////////////////////////////////////////////////////////////////////

//授权信息
const TCHAR szCompilation[]=TEXT("217F8DFB-FE8A-4C3B-A023-C35489A1C0F5");

//////////////////////////////////////////////////////////////////////////////////

#endif