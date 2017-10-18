/*----------------- configStruct.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 刘雷
* Version      : V1.0
* Date         : 2011/8/22 21:26:55
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicFunctions.h"
#include "basic/stringFunctions.h"
#include "network/netStruct.h"
#include "def_struct/playerDefine.h"
#include "def_struct/stlDefine.h"
#include "configDefine.h"
#include "protocol/CASProtocol.pb.h"
#include <string>
#include <set>
/*************************************************************/
#pragma pack(push,1)
//------------------------------------------------------
//------------------------------ 数据库配置
struct _stDBConfig
{
	char	szDBServerName[32];
	char	szDataBaseName[32];
	char	szDBUserID[32];
	char	szDBPassword[32];
	uint16	uPort;

	_stDBConfig()				{	initDBConfig();					}
	void	initDBConfig	()	{	memset(this,0,sizeof(*this));	}

	char*	getServerName	()	{	return szDBServerName;			}
	uint16	getServerPort	()	{	return uPort;					}
	char*	getDBName		()	{	return szDataBaseName;			}
	char*	getUserName		()	{	return szDBUserID;				}
	char*	getPassword		()	{	return szDBPassword;			}

	void	setServerName	(const char*pStr)
	{
		if(pStr && *pStr) 
			dStrcpy(szDBServerName,sizeof(szDBServerName),pStr);
		else
			szDBServerName[0] = '\0';
	}
	void	setServerPort	(uint16 uPort)
	{
		this->uPort = uPort;
	}
	void	setDBName		(const char*pStr)
	{
		if(pStr && *pStr)
			dStrcpy(szDataBaseName,sizeof(szDataBaseName),pStr);
		else
			szDataBaseName[0] = '\0';
	}
	void	setUserName		(const char*pStr)
	{
		if(pStr && *pStr)
			dStrcpy(szDBUserID,sizeof(szDBUserID),pStr);
		else
			szDBUserID[0] = '\0';
	}
	void	setPassword		(const char*pStr)
	{
		if(pStr && *pStr)
			dStrcpy(szDBPassword,sizeof(szDBPassword),pStr);
		else
			szDBPassword[0] = '\0';
	}
};
//-------------------------------------------------------------
//------------------------------ 在线配置(主要用于世界服务器配置)
struct _stOnlineConfig 
{
	uint32	uOnlineMax;		//最大在线数量(默认4000)
	uint32	uOnlineTimeout;	//在线超时时间(默认2*60秒)

	_stOnlineConfig&	getOnlineConfig	()	{	return *this;	}
	void				initOnlineConfig()
	{
		uOnlineMax		= 4000;
		uOnlineTimeout	= 2 * 60;
	}
};
//-------------------------------------------------------------
//------------------------------ 游戏配置
struct _stGameConfig 
{
	uint8	arrSwitch[_BIT8ArraySize(32)];//使用位来进行判定

	_stGameConfig()											{	initGameConfig();							}
	_stGameConfig&	getGameConfig()							{	return *this;								}
	inline void		initGameConfig()
	{
		memset(this,0,sizeof(*this));
		for (uint16 i = 0;i < GameSwitch_Max;++i)
			setSwitch(i,true);
	}
	inline bool		checkSwitch	(uint16 _switch)			{	return _checkBIT8Array(arrSwitch,_switch,sizeof(arrSwitch));	}
	inline void		setSwitch	(uint16 _switch,bool _set)	{	_setBIT8Array(arrSwitch,_switch,_set,sizeof(arrSwitch));		}
};
//-------------------------------------------------------------
//------------------------------ 协议打印配置
struct _stDebugMsgConfig 
{
	uint8	arrSwitch[_BIT8ArraySize(_emPC2S_Protocol_ARRAYSIZE)];//使用位来进行判定
	SET_U64	setFilters;

	_stDebugMsgConfig()										{	init();											}
	_stDebugMsgConfig&	get()								{	return *this;									}
	inline void		init()
	{
		memset(arrSwitch,0,sizeof(arrSwitch));
		for (uint16 i = 0;i < _emPC2S_Protocol_ARRAYSIZE;++i)
			setSwitch(i, false);
		setFilters.clear();
	}
	inline bool		checkSwitch			(uint16 _switch)					{	return _checkBIT8Array(arrSwitch,_switch,sizeof(arrSwitch));	}
	inline void		setSwitch			(uint16 _switch, bool _set)			{	_setBIT8Array(arrSwitch,_switch,_set,sizeof(arrSwitch));		}
	inline void		addFilter			(uint32 mainCode, uint32 subCode)	{	setFilters.insert(MAKE_UINT64(mainCode, subCode));				}
	inline void		delFilter			(uint32 mainCode, uint32 subCode)	{	setFilters.erase(MAKE_UINT64(mainCode, subCode));				}
	inline bool		checkFilter			(uint32 mainCode, uint32 subCode)	{	return setFilters.is_exist(MAKE_UINT64(mainCode, subCode));		}
};
//-------------------------------------------------------------
//------------------------------GM登录数据结构
struct _stGMLogin
{
	uint64 uEndTimer;
	uint16 uWorldID;
	uint32 uAccountID;
	char   cAccountName[Account_Name_Max];

	_stGMLogin& getGMLogin()				{	return * this;					}
	inline void initGMLogin()				{	memset(this,0,sizeof(*this));	}
};
//------------------------------------------------------
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------ dip配置
struct _stKeyConfig
{
	uint32		uTimeout;
	std::string strKey;

	_stKeyConfig()			{	initKeyConfig("",120);		}
	inline void	initKeyConfig(pc_str _key,uint32 _timeout)
	{
		strKey	= _key ? _key : "";
		uTimeout= _timeout;
	}
};
