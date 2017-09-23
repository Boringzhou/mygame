#ifndef GLOBAL_USER_MANAGER_HEAD_FILE
#define GLOBAL_USER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CGlobalUserItem;
class CGlobalPlazaItem;
class CGlobalServerItem;
class CGlobalRoomItem;

//数组定义
typedef CWHArray<CGlobalUserItem *> CGlobalUserItemArray;
typedef CWHArray<CGlobalPlazaItem *> CGlobalPlazaItemArray;
typedef CWHArray<CGlobalServerItem *> CGlobalServerItemArray;
typedef CWHArray<CGlobalRoomItem*> CGlobalRoomItemArray;

//索引定义
typedef CMap<DWORD,DWORD,CGlobalUserItem *,CGlobalUserItem *> CMapUserID;
typedef CMap<DWORD,DWORD,CGlobalUserItem *,CGlobalUserItem *> CMapGameID;
typedef CMap<WORD,WORD,CGlobalPlazaItem *,CGlobalPlazaItem *> CMapPlazaID;
typedef CMap<WORD,WORD,CGlobalServerItem *,CGlobalServerItem *> CMapServerID;
typedef CMap<DWORD,DWORD,CGlobalRoomItemArray*,CGlobalRoomItemArray*> CMapUserRoom;

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//用户信息
struct tagGlobalUserInfo
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//辅助信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级
};


//////////////////////////////////////////////////////////////////////////////////

//用户子项
class CGlobalUserItem
{
	//友元定义
	friend class CGlobalInfoManager;

	//用户属性
public:
	DWORD							m_dwUserID;							//用户标识
	DWORD							m_dwGameID;							//游戏标识
	TCHAR							m_szNickName[LEN_NICKNAME];			//用户昵称

	//辅助信息
public:
	BYTE							m_cbGender;							//用户性别
	BYTE							m_cbMemberOrder;					//会员等级
	BYTE							m_cbMasterOrder;					//管理等级

	//房间信息
public:
	CGlobalServerItemArray			m_GlobalServerItemArray;			//房间数组

	//链表属性
protected:
	CGlobalUserItem *				m_pNextUserItemPtr;					//对象指针

	//函数定义
protected:
	//构造函数
	CGlobalUserItem();
	//析构函数
	virtual ~CGlobalUserItem();

	//功能函数
public:
	//用户标识
	DWORD GetUserID() { return m_dwUserID; }
	//游戏标识
	DWORD GetGameID() { return m_dwGameID; }
	//用户昵称
	LPCTSTR GetNickName() { return m_szNickName; }

	//用户信息
public:
	//用户性别
	BYTE GetGender() { return m_cbGender; }
	//会员等级
	BYTE GetMemberOrder() { return m_cbMemberOrder; }
	//会员等级
	BYTE GetMasterOrder() { return m_cbMasterOrder; }

	//其他信息
public:
	//枚举房间
	CGlobalServerItem * EnumServerItem(WORD wIndex);
};

//////////////////////////////////////////////////////////////////////////////////

//广场子项
class CGlobalPlazaItem
{
	//友元定义
	friend class CGlobalInfoManager;

	//变量定义
public:
	WORD							m_wIndex;							//绑定索引
	tagGamePlaza					m_GamePlaza;						//游戏广场

	//索引变量
public:
	CMapUserID						m_MapUserID;						//用户索引

	//链表属性
protected:
	CGlobalPlazaItem *				m_pNextPlazaItemPtr;				//对象指针

	//函数定义
protected:
	//构造函数
	CGlobalPlazaItem();
	//析构函数
	virtual ~CGlobalPlazaItem();
};

//////////////////////////////////////////////////////////////////////////////////

// 房间子项
class CGlobalRoomItem
{
	//友元定义
	friend class CGlobalInfoManager;
public:
	//变量定义
	DWORD						m_dwRoomID;							//房间ID
	WORD						m_wServerID;						//服务ID
	DWORD						m_dwCreatorID;						//房间创建者ID
	BYTE						m_cbRoomState;						//房间状态
	BYTE						m_cbJuShu;							//局数模式
	BYTE						m_cbPlayer;							//玩家人数
	BYTE						m_cbSitPlayerCnt;					//已坐下玩家数量

protected:
	CGlobalRoomItem				*m_pNextRoomItemPtr;				//对象指针
protected:
	//构造函数
	CGlobalRoomItem();
	//析构函数
	virtual ~CGlobalRoomItem();
private:
	void Reset();
};



//游戏服务子项
class CGlobalServerItem
{
	//友元定义
	friend class CGlobalInfoManager;

	//变量定义
public:
	WORD							m_wIndex;							//绑定索引
	tagGameServer					m_GameServer;						//游戏房间
	tagGameMatch					m_GameMatch;						//游戏比赛

	//索引变量
public:
	CMapUserID						m_MapUserID;						//用户索引

	//链表属性
protected:
	CGlobalServerItem *				m_pNextServerItemPtr;				//对象指针

	//函数定义
protected:
	//构造函数
	CGlobalServerItem();
	//析构函数
	virtual ~CGlobalServerItem();

	//功能函数
public:
	//绑定索引
	WORD GetIndex() { return m_wIndex; }
	//获取类型
	WORD GetKindID() { return m_GameServer.wKindID; }
	//获取房间
	WORD GetServerID() { return m_GameServer.wServerID; }
	//用户数目
	DWORD GetUserItemCount() { return (DWORD)m_MapUserID.GetCount(); }
	//比赛房间
	bool IsMatchServer() { return (m_GameMatch.wServerID==m_GameServer.wServerID) && m_GameMatch.dwMatchID!=0; }

	//查找函数
public:
	//寻找用户
	CGlobalUserItem * SearchUserItem(DWORD dwUserID);
};

//////////////////////////////////////////////////////////////////////////////////

//全局信息
class CGlobalInfoManager
{
	//索引变量
protected:
	CMapUserID						m_MapUserID;						//用户标识
	CMapPlazaID						m_MapPlazaID;						//广场标识
	CMapServerID					m_MapServerID;						//服务标识
	CMapUserRoom					m_MapUserRoom;						//房间标识

	CCriticalSection				m_CriticalSection;					//房间列表锁
	//辅助索引
protected:
	CMapGameID						m_MapGameID;						//标识索引
	CMapStringToPtr					m_MapNickName;						//昵称索引

	//存储变量
protected:
	CGlobalUserItem *				m_pGlobalUserItem;					//用户存储
	CGlobalPlazaItem *				m_pGlobalPlazaItem;					//广场存储
	CGlobalServerItem *				m_pGlobalServerItem;				//服务存储
	CGlobalRoomItem *				m_pGlobalRoomItem;					//房间存储

	//函数定义
public:
	//构造函数
	CGlobalInfoManager();
	//析构函数
	virtual ~CGlobalInfoManager();

	//管理函数
public:
	//重置数据
	VOID ResetData();
	//用户数目
	DWORD GetUserItemCount() { return (DWORD)m_MapUserID.GetCount(); }
	//大厅数目
	DWORD GetPlazaItemCount() { return (DWORD)m_MapPlazaID.GetCount(); }
	//游戏服务数目
	DWORD GetServerItemCount() { return (DWORD)m_MapServerID.GetCount(); }
	//房间数目
	DWORD GetRoomItemCount(DWORD dwUserID);

	//用户管理
public:
	//删除用户
	bool DeleteUserItem(DWORD dwUserID, WORD wServerID);
	//激活用户
	bool ActiveUserItem(tagGlobalUserInfo & GlobalUserInfo, WORD wServerID);

	//广场管理
public:
	//删除广场
	bool DeletePlazaItem(WORD wPlazaID);
	//激活广场
	bool ActivePlazaItem(WORD wBindIndex, tagGamePlaza & GamePlaza);

	//房间管理
public:
	//删除游戏服务
	bool DeleteServerItem(WORD wServerID);
	//激活游戏服务
	bool ActiveServerItem(WORD wBindIndex, tagGameServer & GameServer);

public:
	//删除房间
	bool DeleteRoomItem(DWORD dwUserID,DWORD dwRoomID);
	//激活房间
	bool ActiveRoomItem(DWORD dwUserID,tagGlobalRoomInfo &roomInfo);
	//修改房间信息
	bool UpdateRoomItem(DWORD dwUserID,tagGlobalRoomInfo &roomInfo);
	//服务查找
public:
	//寻找广场
	CGlobalPlazaItem * SearchPlazaItem(WORD wPlazaID);
	//寻找房间
	CGlobalServerItem * SearchServerItem(WORD wServerID);

	//用户查找
public:
	//寻找用户
	CGlobalUserItem * SearchUserItemByUserID(DWORD dwUserID);
	//寻找用户
	CGlobalUserItem * SearchUserItemByGameID(DWORD dwGameID);
	//寻找用户
	CGlobalUserItem * SearchUserItemByNickName(LPCTSTR pszNickName);

	//枚举函数
public:
	//枚举用户
	CGlobalUserItem * EnumUserItem(POSITION & Position);
	//枚举广场
	CGlobalPlazaItem * EnumPlazaItem(POSITION & Position);
	//枚举游戏服务
	CGlobalServerItem * EnumServerItem(POSITION & Position);
	//枚举房间
	CGlobalRoomItem *EnumRoomItem(DWORD dwUserID,WORD wRoomIdx);
	//创建函数
private:
	//创建用户
	CGlobalUserItem * CreateGlobalUserItem();
	//创建广场
	CGlobalPlazaItem * CreateGlobalPlazaItem();
	//创建游戏服务
	CGlobalServerItem * CreateGlobalServerItem();
	//创建房间
	CGlobalRoomItem * CreateGlobalRoomItem();

	//释放函数
private:
	//释放用户
	bool FreeGlobalUserItem(CGlobalUserItem * pGlobalUserItem);
	//释放广场
	bool FreeGlobalPlazaItem(CGlobalPlazaItem * pGlobalPlazaItem);
	//释放游戏服务
	bool FreeGlobalServerItem(CGlobalServerItem * pGlobalServerItem);
	//释放房间
	bool FreeGlobalRoomItem(CGlobalRoomItem * pGlobalRoomItem);
};

//////////////////////////////////////////////////////////////////////////////////

#endif