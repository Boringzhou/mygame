#ifndef TABLE_FRAME_HEAD_FILE
#define TABLE_FRAME_HEAD_FILE

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//数组定义
typedef IServerUserItem * CTableUserItemArray[MAX_CHAIR];				//游戏数组
typedef CWHArray<IServerUserItem *> CLookonUserItemArray;				//旁观数组
typedef CWHArray<tagGameScoreRecord *> CGameScoreRecordArray;			//记录数组
interface IMatchTableFrameNotify;
//////////////////////////////////////////////////////////////////////////////////

//桌子框架
class CTableFrame : public ITableFrame, public ICompilationSink
{
	//游戏属性
protected:
	WORD							m_wTableID;							//桌子号码
	WORD							m_wChairCount;						//椅子数目
	BYTE							m_cbStartMode;						//开始模式
	WORD							m_wUserCount;						//用户数目
	bool							m_bRefuseSameIP;					//拒绝同IP
	bool							m_bDistanceIn300;					//拒绝300米内一同游戏
	double							m_dLongitude[MAX_CHAIR];			//经度
	double							m_dLatitude[MAX_CHAIR];				//纬度

	//状态变量
protected:
	bool							m_bGameStarted;						//游戏标志
	bool							m_bDrawStarted;						//游戏标志
	bool							m_bTableStarted;					//游戏标志
	bool							m_bTableInitFinish;					//初始标识
	bool							m_bFangkaGameStarted;				//房卡开始标识

	//状态变量
protected:
	bool							m_bAllowLookon[MAX_CHAIR];			//旁观标志
	SCORE							m_lFrozenedScore[MAX_CHAIR];		//服务费用
	TCHAR							m_szHeadUrl[MAX_CHAIR][LEN_URL];	//头像链接

	//游戏变量
protected:
	SCORE							m_lCellScore;						//单元积分
	BYTE							m_cbGameStatus;						//游戏状态
	bool							m_bAgree[MAX_CHAIR];				//同意解散
	bool							m_bResponseDismiss[MAX_CHAIR];		//响应解散
	SCORE							m_lZuanshi;							//钻石
	BYTE							m_cbPayType;						//支付方式
	bool							m_bWaitForContinue;					//等待续费
	WORD							m_wFangzhu;							//房主
	bool							m_bContinue[MAX_CHAIR];				//续费
	DWORD							m_dwFangka;							//房卡
	bool							m_bKouFangka;						//是否先扣房卡
	bool							m_bKing;							//是否有大小王

	//时间变量
protected:
	DWORD							m_dwDrawStartTime;					//开始时间
	SYSTEMTIME						m_SystemTimeStart;					//开始时间
	WORD                            m_wDrawCount;                       //游戏局数

	//动态属性
protected:
	DWORD							m_dwTableOwnerID;					//桌主用户
	TCHAR							m_szEnterPassword[LEN_PASSWORD];	//进入密码
	DWORD							m_dwTablePassword;					//进入密码

	//断线变量
protected:
	WORD							m_wOffLineCount[MAX_CHAIR];			//断线次数
	DWORD							m_dwOffLineTime[MAX_CHAIR];			//断线时间

	//用户数组
protected:
	CTableUserItemArray				m_TableUserItemArray;				//游戏用户
	CLookonUserItemArray			m_LookonUserItemArray;				//旁观用户

	//组件接口
public:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	ITableFrameSink	*				m_pITableFrameSink;					//桌子接口
	IMainServiceFrame *				m_pIMainServiceFrame;				//服务接口
	IAndroidUserManager *			m_pIAndroidUserManager;				//机器接口

	//扩展接口
protected:
	ITableUserAction *				m_pITableUserAction;				//动作接口
	ITableUserRequest *				m_pITableUserRequest;				//请求接口	

	//数据接口
protected:
	IDataBaseEngine *				m_pIKernelDataBaseEngine;			//内核数据
	IDataBaseEngine *				m_pIRecordDataBaseEngine;			//记录数据

	//配置信息
protected:
	tagGameParameter *				m_pGameParameter;					//配置参数
	tagGameMatchOption *			m_pGameMatchOption;					//比赛配置	
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//服务属性
	tagGameServiceOption *			m_pGameServiceOption;				//服务配置
	tagFangkaGameOption				m_nFangkaGameOption;				//房卡配置

	//游戏记录
protected:
	CGameScoreRecordArray			m_GameScoreRecordActive;			//游戏记录
	static CGameScoreRecordArray	m_GameScoreRecordBuffer;			//游戏记录

	//比赛接口
protected:
	ITableFrameHook	*				m_pITableFrameHook;					//比赛接口
	ITableUserAction *				m_pIMatchTableAction;				//动作接口

	//函数定义
public:
	//构造函数
	CTableFrame();
	//析构函数
	virtual ~CTableFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//属性接口
public:
	//桌子号码
	virtual WORD GetTableID() { return m_wTableID; }
	//游戏人数
	virtual WORD GetChairCount() { return m_wChairCount; }
	//空位置数目
	virtual WORD GetNullChairCount(){return m_wChairCount - m_wUserCount;}

	//配置参数
public:
	//自定配置
	virtual VOID * GetCustomRule() { return m_pGameServiceOption->cbCustomRule; };
	//比赛配置
	virtual tagGameMatchOption * GetGameMatchOption() { return m_pGameMatchOption; };
	//服务属性
	virtual tagGameServiceAttrib * GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//服务配置
	virtual tagGameServiceOption * GetGameServiceOption() { return m_pGameServiceOption; }
	//房卡配置
	virtual tagFangkaGameOption * GetFangkaGameOption(){ return &m_nFangkaGameOption; }

	//配置接口
public:
	//开始模式
	virtual BYTE GetStartMode() { return m_cbStartMode; }
	//开始模式
	virtual VOID SetStartMode(BYTE cbStartMode) { m_cbStartMode=cbStartMode; }

	//单元积分
public:
	//单元积分
	virtual SCORE GetCellScore() { return m_lCellScore; }
	//单元积分
	virtual VOID SetCellScore(SCORE lCellScore);
	//扣除分数
	virtual void DedultScore(IServerUserItem * pIServerUserItem,SCORE lScore);
	//扣除房卡
	void DedultInsure(IServerUserItem * pIServerUserItem,DWORD dwInsure);
	//状态接口
public:
	//获取状态
	virtual BYTE GetGameStatus() { return m_cbGameStatus; }
	//设置状态
	virtual VOID SetGameStatus(BYTE cbGameStatus) { m_cbGameStatus=cbGameStatus; }
	//获取密码
	virtual DWORD GetPassword(){ return m_dwTablePassword; }
	//设置密码
	virtual void SetPassword(DWORD dwPassword){ m_dwTablePassword=dwPassword; }
	//获取IP限制
	virtual bool GetRefuseSameIP(){ return m_bRefuseSameIP; }
	//设置IP限制
	virtual void SetRefuseSameIP(bool bRefuseSameIP){ m_bRefuseSameIP=bRefuseSameIP; }
	//获取距离限制
	virtual bool GetDistanceIn300(){ return m_bDistanceIn300; }
	//设置距离限制
	virtual void SetDistanceIn300(bool bDistanceIn300){ m_bDistanceIn300=bDistanceIn300; }
	//获取经度
	virtual double GetLongitude(WORD wChairID){ return m_dLongitude[wChairID]; }
	//设置经度
	virtual void SetLongitude(WORD wChairID,double dLongitude){ m_dLongitude[wChairID]=dLongitude; }
	//获取纬度
	virtual double GetLatitude(WORD wChairID){ return m_dLatitude[wChairID]; }
	//设置纬度
	virtual void SetLatitude(WORD wChairID,double dLatitude){ m_dLatitude[wChairID]=dLatitude; }
	//设置钻石
	virtual void SetZuanshi(SCORE lZuanshi) { m_lZuanshi = lZuanshi;}
	virtual void SetInsure(DWORD lFangka){m_dwFangka = lFangka;}
	virtual BYTE GetZuanshi(){ if (m_cbPayType == 0)
	{
		return m_lZuanshi;
	} 
	else
	{
		return m_lZuanshi/m_wChairCount;
	}}
	//支付方式
	virtual void SetPayType(BYTE cbType){m_cbPayType = cbType;}
	//设置房主
	virtual void SetFangzhu(WORD wUser){m_wFangzhu = wUser;}
	virtual WORD GetFangzhu(){return m_wFangzhu;}
	//设置房主ID
	virtual void SetCreatorID(DWORD dwCreatorID){m_dwTableOwnerID = dwCreatorID;}
	virtual DWORD GetCreatorID(){return m_dwTableOwnerID;}

	//设置续费
	virtual void SetContinue(WORD wUser){m_bContinue[wUser]=true;}
	void SetKouFangka(bool bKou){m_bKouFangka = bKou;}
	void SetKing(bool bKing){m_bKing = bKing;}
	//信息接口
public:
	//游戏状态
	virtual bool IsGameStarted() { return m_bGameStarted; }
	//游戏状态
	virtual bool IsDrawStarted() { return m_bDrawStarted; }
	//游戏状态
	virtual bool IsTableStarted() { return m_bTableStarted; }
	//锁定状态
	virtual bool IsTableLocked() { return (m_szEnterPassword[0]!=0); }

	//控制接口
public:
	//开始游戏
	virtual bool StartGame();
	//解散游戏
	virtual bool DismissGame();
	//继续游戏
	virtual bool ContinueGame(WORD wChairID);
	//结束游戏
	virtual bool ConcludeGame(BYTE cbGameStatus);
	//结束桌子
	virtual bool ConcludeTable();
	//开始房卡模式
	virtual void SetFangkaStart(bool bStart);
	//是否有任何人续费
	virtual bool AnyOneContinue();
	//用户接口
public:
	//寻找用户
	virtual IServerUserItem * SearchUserItem(DWORD dwUserID);
	//游戏用户
	virtual IServerUserItem * GetTableUserItem(WORD wChairID);
	//旁观用户
	virtual IServerUserItem * EnumLookonUserItem(WORD wEnumIndex);

	//写分接口
public:
	//写入积分
	virtual bool WriteUserScore(WORD wChairID, tagScoreInfo & ScoreInfo, DWORD dwGameMemal=INVALID_DWORD, DWORD dwPlayGameTime=INVALID_DWORD);
	//写入积分
	virtual bool WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount);

	//计算接口
public:
	//计算税收
	virtual SCORE CalculateRevenue(WORD wChairID, SCORE lScore);
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);

	//时间接口
public:
	//设置时间
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter);
	//删除时间
	virtual bool KillGameTimer(DWORD dwTimerID);
	//是否有计时器
	virtual bool HasGameTimer(DWORD dwTimerID);
	//获取计时器剩余时间
	virtual DWORD GetLeaveTime(DWORD dwTimerID);
	//游戏用户
public:
	//发送数据
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID);
	//发送数据
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize,WORD wMainCmdID=MDM_GF_GAME);

	//旁观用户
public:
	//发送数据
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID);
	//发送数据
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//所有用户
public:
	//发送数据
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID);
	//发送数据
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//系统消息
public:
	//发送消息
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType);
	//游戏消息
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);
	//房间消息
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);

	//动作处理
public:
	//起立动作
	virtual bool PerformStandUpAction(IServerUserItem * pIServerUserItem, bool bInitiative=false);
	//旁观动作
	virtual bool PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	//坐下动作
	virtual bool PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem, LPCTSTR lpszPassword=NULL);

	//功能接口
public:
	//发送场景
	virtual bool SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//比赛接口
public:
	//设置接口
	virtual bool SetTableFrameHook(IUnknownEx * pIUnknownEx);
	//获取接口
	virtual IUnknownEx * GetTableFrameHook(){ return m_pITableFrameHook;}
	//伪造配置
	virtual bool ImitateGameOption(IServerUserItem * pIServerUserItem);

	//获取信息
public:
	//获取信息
	virtual LPCTSTR GetCompilation(){return szCompilation;}

	//功能函数
public:
	//游戏局数
	WORD GetDrawCount();
	//获取空位
	WORD GetNullChairID();
	//随机空位
	WORD GetRandNullChairID();
	//用户数目
	WORD GetSitUserCount();
	//旁观数目
	WORD GetLookonUserCount();
	//断线数目
	WORD GetOffLineUserCount();
	//同意解散
	bool OnEventDismissRoom(WORD wChairID,bool bAgree);
	//发送解散房间信息
	void SendDismissInfo(WORD wChairID);
	//功能函数
public:
	//用户状况
	WORD GetTableUserInfo(tagTableUserInfo & TableUserInfo);
	//配置桌子
	bool InitializationFrame(WORD wTableID, tagTableFrameParameter & TableFrameParameter);

	//用户事件
public:
	//断线事件
	bool OnEventUserOffLine(IServerUserItem * pIServerUserItem);
	//积分事件
	bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//系统事件
public:
	//时间事件
	bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//游戏事件
	bool OnEventSocketGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架事件
	bool OnEventSocketFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//辅助函数
public:
	//桌子状态
	bool SendTableStatus();
	//请求失败
	bool SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode);

	//效验函数
public:
	//开始效验
	bool EfficacyStartGame(WORD wReadyChairID);
	//地址效验
	bool EfficacyIPAddress(IServerUserItem * pIServerUserItem);
	//积分效验
	bool EfficacyScoreRule(IServerUserItem * pIServerUserItem);
	//积分效验
	bool EfficacyEnterTableScoreRule(WORD wChairID, IServerUserItem * pIServerUserItem);

public:
	//检查分配
	bool CheckDistribute();
	//游戏记录
	void RecordGameScore(bool bDrawStarted, DWORD dwStartGameTime=INVALID_DWORD);


	//准备超时时间
	void StartReadyOutTime(BYTE wChairID);
};

//////////////////////////////////////////////////////////////////////////////////

#endif