#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//用户事件
#define	DBR_GR_LOGON_USERID			100									//I D 登录
#define	DBR_GR_LOGON_MOBILE			101									//手机登录
#define	DBR_GR_LOGON_ACCOUNTS		102									//帐号登录
#define DBR_GR_LOGON_MATCH			103									//比赛登录
#define DBR_GR_ROOM					104									//进入房间

//系统事件
#define DBR_GR_WRITE_GAME_SCORE		200									//游戏积分
#define DBR_GR_LEAVE_GAME_SERVER	201									//离开房间
#define DBR_GR_GAME_SCORE_RECORD	202									//积分记录
#define DBR_GR_MANAGE_USER_RIGHT	203									//权限管理
#define DBR_GR_LOAD_SYSTEM_MESSAGE	204									//系统消息
#define DBR_GR_LOAD_SENSITIVE_WORDS	205									//敏感词	
#define DBR_GR_UNLOCK_ANDROID_USER	206									//解锁机器人

//配置事件
#define DBR_GR_LOAD_PARAMETER		300									//加载参数
#define DBR_GR_LOAD_GAME_COLUMN		301									//加载列表
#define DBR_GR_LOAD_ANDROID_PARAMETER 302								//加载参数
#define DBR_GR_LOAD_ANDROID_USER	303									//加载机器
#define DBR_GR_LOAD_GAME_PROPERTY	304									//加载道具
#define DBR_GR_LOAD_GROWLEVEL_CONFIG 305								//等级配置

//银行事件
#define DBR_GR_USER_ENABLE_INSURE	400									//开通银行
#define DBR_GR_USER_SAVE_SCORE		401									//存入游戏币
#define DBR_GR_USER_TAKE_SCORE		402									//提取游戏币
#define DBR_GR_USER_TRANSFER_SCORE	403									//转帐游戏币
#define DBR_GR_QUERY_INSURE_INFO	404									//查询银行
#define DBR_GR_QUERY_TRANSFER_USER_INFO	    405							//查询用户

//游戏事件
#define DBR_GR_PROPERTY_REQUEST		500									//购买礼物
#define DBR_GR_GAME_FRAME_REQUEST	502									//游戏请求
#define DBR_GR_WRITE_USER_GAME_DATA	503									//用户游戏数据

//比赛事件
#define DBR_GR_MATCH_SIGNUP			600									//比赛费用
#define DBR_GR_MATCH_UNSIGNUP		601									//退出比赛
#define DBR_GR_MATCH_START			602									//比赛开始
#define DBR_GR_MATCH_OVER			603									//比赛结束	
#define DBR_GR_MATCH_REWARD			604									//比赛奖励
#define DBR_GR_MATCH_ENTER_FOR		605									//报名进入
#define DBR_GR_MATCH_ELIMINATE      606									//比赛淘汰

//任务事件
#define DBR_GR_TASK_TAKE			700									//领取任务
#define DBR_GR_TASK_REWARD			701									//领取奖励
#define DBR_GR_TASK_LOAD_LIST		702									//加载任务
#define DBR_GR_TASK_QUERY_INFO		703									//查询任务

//兑换事件
#define DBR_GR_LOAD_MEMBER_PARAMETER 800								//会员参数
#define DBR_GR_PURCHASE_MEMBER		801									//购买会员
#define DBR_GR_EXCHANGE_SCORE_INGOT	802									//兑换游戏币
#define DBR_GR_EXCHANGE_SCORE_BEANS	803									//兑换游戏币

//等级命令
#define DBR_GR_GROWLEVEL_QUERY_IFNO	 820								//查询等级
//////////////////////////////////////////////////////////////////////////////////
//输出信息

//逻辑事件
#define DBO_GR_LOGON_SUCCESS		100									//登录成功
#define DBO_GR_LOGON_FAILURE		101									//登录失败

//配置事件
#define DBO_GR_GAME_PARAMETER		200									//配置信息
#define DBO_GR_GAME_COLUMN_INFO		201									//列表信息
#define DBO_GR_GAME_ANDROID_PARAMETER 202								//参数信息
#define DBO_GR_GAME_ANDROID_INFO	203									//机器信息
#define DBO_GR_GAME_UNLOCK_ANDROID	204									//解锁机器
#define DBO_GR_GAME_PROPERTY_INFO	205									//道具信息
#define DBO_GR_GAME_MEMBER_PAREMETER 206								//会员参数

//银行命令
#define DBO_GR_USER_INSURE_INFO		300									//银行资料
#define DBO_GR_USER_INSURE_SUCCESS	301									//银行成功
#define DBO_GR_USER_INSURE_FAILURE	302									//银行失败
#define DBO_GR_USER_INSURE_USER_INFO   303								//用户资料
#define DBO_GR_USER_INSURE_ENABLE_RESULT 304							//开通结果

//游戏事件
#define DBO_GR_PROPERTY_SUCCESS		400									//道具成功
#define DBO_GR_PROPERTY_FAILURE		401									//道具失败
#define DBO_GR_GAME_FRAME_RESULT	402									//游戏结果

//比赛事件
#define DBO_GR_MATCH_EVENT_START	500									//比赛标识
#define DBO_GR_MATCH_SIGNUP_RESULT	500									//报名结果
#define DBO_GR_MATCH_UNSIGNUP_RESULT 501								//退赛结果
#define DBO_GR_MATCH_RANK_LIST		502									//比赛排行
#define DBO_GR_MATCH_REWARD_RESULT  503									//奖励结果
#define DBO_GR_MATCH_EVENT_END		599									//比赛标识

//系统事件
#define DBO_GR_SYSTEM_MESSAGE_RESULT	600								//消息结果
#define DBO_GR_SYSTEM_MESSAGE_FINISH	601								//消息完成	
#define DBO_GR_SENSITIVE_WORDS		    602								//敏感词

//任务事件
#define DBO_GR_TASK_LIST			700									//任务列表
#define DBO_GR_TASK_LIST_END		701									//任务结束
#define DBO_GR_TASK_INFO			702									//任务信息
#define DBO_GR_TASK_RESULT			703									//任务结果

//兑换事件
#define DBO_GR_MEMBER_PARAMETER     800									//会员参数
#define DBO_GR_PURCHASE_RESULT		801									//购买结果
#define DBO_GR_EXCHANGE_RESULT		802									//兑换结果

//等级命令
#define DBO_GR_GROWLEVEL_CONFIG		820									//等级配置
#define DBO_GR_GROWLEVEL_PARAMETER	821									//等级参数
#define DBO_GR_GROWLEVEL_UPGRADE	822									//等级升级
#define DBO_GR_UPDATE_CONTROLINFO	900									//更新控制

//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct DBR_GR_LogonUserID
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//附加信息
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛编号
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//ID 登录
struct DBR_GR_LogonMobile
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//附加信息
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛编号
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//帐号登录
struct DBR_GR_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//游戏积分
struct DBR_GR_WriteGameScore
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址

	//用户信息
	DWORD							dwDBQuestID;						//请求标识
	DWORD							dwInoutIndex;						//进出索引

	//附件信息
	bool							bTaskForward;						//任务跟进

	//成绩变量
	tagVariationInfo				VariationInfo;						//提取信息

	//比赛信息
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次
};

//离开房间
struct DBR_GR_LeaveGameServer
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwInoutIndex;						//记录索引
	DWORD							dwLeaveReason;						//离开原因
	DWORD							dwOnLineTimeCount;					//在线时长

	//成绩变量
	tagVariationInfo				RecordInfo;							//记录信息
	tagVariationInfo				VariationInfo;						//提取信息

	//系统信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};
//更新控制
struct DBO_GR_UpdateControlInfo
{
	DWORD							dwUserID;							//用户标识
	int								nWinRate;							//获胜概率
	BYTE							cbNullity;							//是否冻结
};
//游戏记录
struct DBR_GR_GameScoreRecord
{
	//桌子信息
	WORD							wTableID;							//桌子号码
	WORD							wUserCount;							//用户数目
	WORD							wAndroidCount;						//机器数目

	//损耗税收
	SCORE							lWasteCount;						//损耗数目
	SCORE							lRevenueCount;						//税收数目

	//统计信息
	DWORD							dwUserMemal;						//奖牌数目
	DWORD							dwPlayTimeCount;					//游戏时间

	//时间信息
	SYSTEMTIME						SystemTimeStart;					//开始时间
	SYSTEMTIME						SystemTimeConclude;					//结束时间

	//积分记录
	WORD							wRecordCount;						//记录数目
	tagGameScoreRecord				GameScoreRecord[MAX_CHAIR];			//游戏记录
};

//开通银行
struct DBR_GR_UserEnableInsure
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D	
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szLogonPass[LEN_PASSWORD];			//登录密码
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//存入游戏币
struct DBR_GR_UserSaveScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出游戏币
struct DBR_GR_UserTakeScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转帐游戏币
struct DBR_GR_UserTransferScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址	
	SCORE							lTransferScore;						//转帐游戏币
	TCHAR							szAccounts[LEN_NICKNAME];			//目标用户
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//转帐备注
};

//加载机器
struct DBR_GR_LoadAndroidUser
{
	DWORD							dwBatchID;							//批次标识
	DWORD							dwAndroidCount;						//机器数目
};

//查询银行
struct DBR_GR_QueryInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
};

//查询用户
struct DBR_GR_QueryTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
	DWORD							dwUserID;							//用户 I D
};

//道具请求
struct DBR_GR_PropertyRequest
{
	//购买信息
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//购买对象
	DWORD							dwTargetUserID;						//使用对象

	//消费模式
	BYTE                            cbRequestArea;			            //使用范围 
	BYTE							cbConsumeScore;						//积分消费
	SCORE							lFrozenedScore;						//冻结积分

	//用户权限
	DWORD                           dwUserRight;						//会员权限

	//系统信息
	WORD							wTableID;							//桌子号码
	DWORD							dwInoutIndex;						//进入标识
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};


//查询任务
struct DBR_GR_TaskQueryInfo
{
	//用户信息
	DWORD							dwUserID;							//用户标识	
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
};

//领取任务
struct DBR_GR_TaskTake
{
	//用户信息
	WORD							wTaskID;							//任务标识
	DWORD							dwUserID;							//用户标识	
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//领取奖励
struct DBR_GR_TaskReward
{
	//用户信息
	WORD							wTaskID;							//任务标识
	DWORD							dwUserID;							//用户标识	
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询等级
struct DBR_GR_GrowLevelQueryInfo
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//购买会员
struct DBR_GR_PurchaseMember
{
	DWORD							dwUserID;							//用户 I D
	BYTE							cbMemberOrder;						//会员标识
	WORD							wPurchaseTime;						//购买时间

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//兑换游戏币
struct DBR_GR_ExchangeScoreByIngot
{
	DWORD							dwUserID;							//用户标识
	SCORE							lExchangeIngot;						//兑换元宝

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//兑换游戏币
struct DBR_GR_ExchangeScoreByBeans
{
	DWORD							dwUserID;							//用户标识
	double							dExchangeBeans;						//兑换游戏豆

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//兑换道具
struct DBR_GR_ExchangeScoreByProp
{
	DWORD							dwUserID;							//用户标识
	WORD							wItemIndex;							//兑换道具

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询签到
struct DBR_GR_CheckInQueryInfo
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
};

//执行签到
struct DBR_GR_CheckInDone
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//用户游戏数据
struct DBR_GR_WriteUserGameData
{
	//用户信息
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szUserGameData[LEN_GAME_DATA];		//游戏数据
};

//////////////////////////////////////////////////////////////////////////////////

//登录成功
struct DBO_GR_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团 I D
	DWORD							dwCustomID;							//自定头像
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lIngot;								//用户元宝
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行	
	DOUBLE							dBeans;								//用户游戏豆

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwExperience;						//用户经验
	LONG							lLoveLiness;						//用户魅力

	//用户权限
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限

	//特殊属性
	DWORD							dwJudging;							//玩家阴德:DCY by 2017.3.11

	//索引变量
	DWORD							dwInoutIndex;						//记录索引
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识

	//任务变量
	WORD							wTaskID;							//任务标识
	BYTE							cbTaskStatus;						//任务状态
	DWORD							dwTaskProgress;						//任务进度	
	SYSTEMTIME						TaskTakeTime;						//领取时间

	//手机定义
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//辅助信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szDescribeString[128];				//错误消息
	TCHAR							szUserGameData[LEN_GAME_DATA];		//游戏数据

	//任务信息
	WORD							wTaskCount;							//任务数目
	tagUserTaskInfo					UserTaskInfo[TASK_MAX_COUNT];		//任务信息
};

//登录失败
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};

//配置信息
struct DBO_GR_GameParameter
{
	//汇率信息
	DWORD							dwMedalRate;						//奖牌汇率
	DWORD							dwRevenueRate;						//银行税收
	DWORD							dwExchangeRate;						//兑换比率
	DWORD							dwPresentExchangeRate;				//魅力游戏币兑换率
	DWORD							dwRateGold;							//游戏豆游戏币兑换率

	//经验奖励
	DWORD							dwWinExperience;					//赢局经验

	//版本信息
	DWORD							dwClientVersion;					//客户版本
	DWORD							dwServerVersion;					//服务版本

	SCORE							lEducateGrantScore;					//练习赠送
};

//列表信息
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//结果代码
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//列表信息
};

//机器信息
struct DBO_GR_GameAndroidParameter
{
	LONG							lResultCode;						//结果代码
	WORD							wParameterCount;					//参数数目
	tagAndroidParameter				AndroidParameter[MAX_BATCH];		//机器信息
};

//机器信息
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//结果代码
	DWORD							dwBatchID;							//批次标识
	WORD							wAndroidCount;						//用户数目
	tagAndroidAccountsInfo			AndroidAccountsInfo[MAX_ANDROID];	//机器帐号
};

//道具信息
struct DBO_GR_GamePropertyInfo
{
	LONG							lResultCode;						//结果代码
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//道具信息
};

//银行资料
struct DBO_GR_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE							cbEnjoinTransfer;					//转帐开关
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wRevenueTransferMember;				//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//银行游戏币
	SCORE							lTransferPrerequisite;				//转帐条件
};

//银行成功
struct DBO_GR_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lSourceScore;						//原来游戏币
	SCORE							lSourceInsure;						//原来游戏币
	SCORE							lInsureRevenue;						//银行税收
	SCORE							lFrozenedScore;						//冻结积分
	SCORE							lVariationScore;					//游戏币变化
	SCORE							lVariationInsure;					//银行变化
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct DBO_GR_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //游戏动作
	LONG							lResultCode;						//操作代码
	SCORE							lFrozenedScore;						//冻结积分
	TCHAR							szDescribeString[128];				//描述消息
};

//用户资料
struct DBO_GR_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwGameID;							//用户 I D
	TCHAR							szAccounts[LEN_ACCOUNTS];			//帐号昵称
};

//开通结果
struct DBO_GR_UserInsureEnableResult
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE							cbInsureEnabled;					//使能标识
	TCHAR							szDescribeString[128];				//描述消息
};

//道具成功
struct DBO_GR_S_PropertySuccess
{
	//道具信息
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//购买对象
	DWORD							dwTargetUserID;						//使用对象

	//消费模式
	BYTE                            cbRequestArea;						//请求范围
	BYTE							cbConsumeScore;						//积分消费
	SCORE							lFrozenedScore;						//冻结积分

	//会员权限
	DWORD                           dwUserRight;						//会员权限

	//结果信息
	SCORE							lConsumeGold;						//消费游戏币
	LONG							lSendLoveLiness;					//赠送魅力
	LONG							lRecvLoveLiness;					//接受魅力

	//会员信息
	BYTE							cbMemberOrder;						//会员等级
};

//签到奖励
struct DBO_GR_CheckInReward
{
	SCORE							lRewardGold[LEN_WEEK];				//奖励金额
};

//签到信息
struct DBO_GR_CheckInInfo
{
	WORD							wSeriesDate;						//连续日期
	DWORD							dwRewardMulti;						//奖励倍数
	bool							bTodayChecked;						//签到标识
};

//签到结果
struct DBO_GR_CheckInResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lScore;								//当前分数
	TCHAR							szNotifyContent[128];				//提示内容
};

//任务列表
struct DBO_GR_TaskListInfo
{
	WORD							wTaskCount;							//任务数目
};

//任务信息
struct DBO_GR_TaskInfo
{
	WORD							wTaskCount;							//任务数量
	tagTaskStatus					TaskStatus[TASK_MAX_COUNT];			//任务状态
};

//签到结果
struct DBO_GR_TaskResult
{
	//结果信息
	bool							bSuccessed;							//成功标识
	WORD							wCommandID;							//命令标识
	WORD							wCurrTaskID;						//当前任务

	//财富信息
	SCORE							lCurrScore;							//当前金币
	SCORE							lCurrIngot;							//当前元宝
	
	//提示信息
	TCHAR							szNotifyContent[128];				//提示内容
};

//等级配置
struct DBO_GR_GrowLevelConfig
{
	WORD							wLevelCount;						//等级数目
	tagGrowLevelConfig				GrowLevelConfig[60];				//等级配置
};

//等级参数
struct DBO_GR_GrowLevelParameter
{
	WORD							wCurrLevelID;						//当前等级	
	DWORD							dwExperience;						//当前经验
	DWORD							dwUpgradeExperience;				//下级经验
	SCORE							lUpgradeRewardGold;					//升级奖励
	SCORE							lUpgradeRewardIngot;				//升级奖励
};

//等级升级
struct DBO_GR_GrowLevelUpgrade
{
	SCORE							lCurrScore;							//当前游戏币
	SCORE							lCurrIngot;							//当前元宝
	TCHAR							szNotifyContent[128];				//升级提示
};

//会员参数
struct DBO_GR_MemberParameter
{
	WORD							wMemberCount;						//会员数目
	tagMemberParameter				MemberParameter[64];				//会员参数
};

//购买结果
struct DBO_GR_PurchaseResult
{
	bool							bSuccessed;							//成功标识
	BYTE							cbMemberOrder;						//会员系列
	DWORD							dwUserRight;						//用户权限
	SCORE							lCurrScore;							//当前游戏币
	DOUBLE							dCurrBeans;							//当前游戏豆
	TCHAR							szNotifyContent[128];				//提示内容
};
 
//兑换结果
struct DBO_GR_ExchangeResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lCurrScore;							//当前游戏币
	SCORE							lCurrInsure;						//当前银行
	SCORE							lCurrIngot;							//当前元宝
	double							dCurrBeans;							//当前游戏豆
	TCHAR							szNotifyContent[128];				//提示内容
};

//道具失败
struct DBO_GR_PropertyFailure
{
	BYTE                            cbRequestArea;						//请求范围
	LONG							lResultCode;						//操作代码
	SCORE							lFrozenedScore;						//冻结积分
	TCHAR							szDescribeString[128];				//描述消息
};

////比赛名次
//struct DBO_GR_MatchRank
//{
//	BYTE							cbRank;								//比赛名次
//	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
//	LONG							lMatchScore;						//用户得分
//};

//报名结果
struct DBO_GR_MatchSingupResult
{
	bool							bResultCode;						//结果代码
	DWORD							dwReason;							//取消原因
	DWORD							dwUserID;							//用户标识
	DWORD							dwMatchNO;							//比赛场次
	SCORE							lCurrGold;							//当前金币
	SCORE							lCurrIngot;							//当前元宝
	TCHAR							szDescribeString[128];				//描述消息
};

//比赛排行
struct DBO_GR_MatchRankList
{
	WORD							wUserCount;							//用户数目
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次
	tagMatchRankInfo				MatchRankInfo[128];					//奖励信息
};

//权限管理
struct DBR_GR_ManageUserRight
{
	DWORD							dwUserID;							//目标用户
	DWORD							dwAddRight;							//添加权限
	DWORD							dwRemoveRight;						//删除权限
	bool							bGameRight;							//游戏权限
};

//权限管理
struct DBR_GR_ManageMatchRight
{	
	DWORD							dwUserID;							//目标用户
	DWORD							dwAddRight;							//添加权限
	DWORD							dwRemoveRight;						//删除权限	
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次
};

//比赛报名
struct DBR_GR_MatchSignup
{
	//报名费用
	DWORD							dwUserID;							//用户ID
	SCORE							lMatchFee;							//报名费用
	BYTE							cbMatchType;						//比赛类型

	//系统信息
	DWORD							dwInoutIndex;						//进入标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次	
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取消报名
struct DBR_GR_MatchUnSignup
{
	//报名费用
	DWORD							dwReason;							//取消原因
	DWORD							dwUserID;							//用户ID
	SCORE							lMatchFee;							//报名费用	

	//系统信息
	DWORD							dwInoutIndex;						//进入标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwMatchID;							//比赛ID
	DWORD							dwMatchNo;							//比赛场次
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//比赛开始
struct DBR_GR_MatchStart
{
	WORD							wServerID;							//房间标识
	BYTE							cbMatchType;						//比赛类型
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次	
};

//比赛结束
struct DBR_GR_MatchOver
{
	WORD							wServerID;							//房间标识
	BYTE							cbMatchType;						//比赛类型
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次	
	SYSTEMTIME						MatchStartTime;						//开始时间
	SYSTEMTIME						MatchEndTime;						//结束时间
};

//比赛淘汰
struct DBR_GR_MatchEliminate
{
	DWORD							dwUserID;							//用户标识
	WORD							wServerID;							//房间标识
	BYTE							cbMatchType;						//比赛类型
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次	
};


//比赛奖励
struct DBR_GR_MatchReward
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lRewardGold;						//奖励金币
	DWORD							dwRewardIngot;						//奖励元宝
    DWORD							dwRewardExperience;					//用户经验	
	DWORD							dwClientAddr;						//连接地址
};

//奖励结果
struct DBO_GR_MatchRewardResult
{
	bool							bResultCode;						//结果代码
	DWORD							dwUserID;							//用户标识
	SCORE							lCurrGold;							//当前金币
	SCORE							lCurrIngot;							//当前元宝
	DWORD							dwCurrExperience;					//当前经验
};

//系统消息
struct DBR_GR_SystemMessage
{
	DWORD                           dwMessageID;                        //消息ID
	BYTE							cbMessageType;						//消息类型
	BYTE                            cbAllRoom;                          //全体房间
	DWORD							dwTimeRate;						    //时间频率
	__time64_t						tStartTime;							//开始时间
	__time64_t                      tConcludeTime;                      //结束时间
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

//解锁机器人
struct DBR_GR_UnlockAndroidUser
{
	WORD							wServerID;							//房间ID
	WORD							wBatchID;							//批次ID
};
//////////////////////////////////////////////////////////////////////////////////

#endif