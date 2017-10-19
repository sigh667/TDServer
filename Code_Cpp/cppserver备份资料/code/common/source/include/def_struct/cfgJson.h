/*----------------- cfgJson.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 林驹
* Version      : V1.0
* Date         : 2016/10/28 19:32:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include <string>
#include "singleton/singleton.h"
/*************************************************************/
/*************************************************************/
namespace google{
	namespace protobuf{
		class Message;
	}
}
class CfgJsonBase
{
public:
	explicit		CfgJsonBase();
	virtual			~CfgJsonBase();

public:
	virtual bool	load(std::string &path);

protected:
	// 处理默认值
	virtual bool	loadDefault()	= 0;

protected:
	::google::protobuf::Message	*m_pPBMsg;
};
//--------------------------------------------------------------
class PBCfg_World;
class PBCfg_Game;
class PBCfg_Login;
class PBCfg_Register;
class PBCfg_DB;
class PBCfg_Global;
class PBCfg_Bigworld;
class PBCfg_Dip;
class PBCfg_Rooter;
class PBCfg_RobotServer;
class PBCfg_RobotClient;
class PBCfg_Record;
class CfgJsonRoot : public CfgJsonBase
					, _single_public(CfgJsonRoot)
{
private:
	_single_friend(CfgJsonRoot);
protected:
	explicit	CfgJsonRoot();
	virtual		~CfgJsonRoot();

public:
	const	PBCfg_World&		world()			;
	const	PBCfg_Game&			game()			;
	const	PBCfg_Login&		login()			;
	const	PBCfg_Register&		register_()		;
	const	PBCfg_DB&			db()			;
	const	PBCfg_Global&		global()		;
	const	PBCfg_Bigworld&		bigworld()		;
	const	PBCfg_Dip&			dip()			;
	const	PBCfg_Rooter&		rooter()		;
	const	PBCfg_RobotServer&	robotserver()	;
	const	PBCfg_RobotClient&	robotclient()	;

protected:
	virtual bool	loadDefault();
};
//--------------------------------------------------------------
class PBCfg_Xls2Data;
class CfgJsonXls2Data : public CfgJsonBase
	, _single_public(CfgJsonXls2Data)
{
private:
	_single_friend(CfgJsonXls2Data);
protected:
	explicit	CfgJsonXls2Data();
	virtual		~CfgJsonXls2Data();

protected:
	virtual bool	loadDefault()		{	return true;	};

public:
	const	PBCfg_Xls2Data&		xls2data();
};

//--------------------------------------------------------------
class PBCfg_Record;
class CfgJsonRecordServer : public CfgJsonBase
	, _single_public(CfgJsonRecordServer)
{
private:
	_single_friend(CfgJsonRecordServer);
protected:
	explicit	CfgJsonRecordServer();
	virtual		~CfgJsonRecordServer();

protected:
	virtual bool	loadDefault()		{	return true;	};

public:
	const	PBCfg_Record&		record();
};

//--------------------------------------------------------------
extern	CfgJsonRoot*			g_pCfgJsonRoot;
extern	CfgJsonXls2Data*		g_pCfgJsonXls2Data;
extern	CfgJsonRecordServer*	g_pCfgJsonRecordServer;