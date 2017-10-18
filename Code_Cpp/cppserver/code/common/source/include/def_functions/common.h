/*----------------- common.h
*
* Copyright (C): 2013  Mokylin°§Mokyqi
* Author       : ¡÷æ‘
* Version      : V1.0
* Date         : 2016/10/31 17:51:04
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
struct	_stNetHost;
struct	_stNetAddress;
struct	_stDBConfig;
struct	_stNetTimeout;
class	CLoadData;
class	PBCfg_Net;
class	PBCfg_NetTimeout;
class	PBCfg_MysqlDB;
class	PBCfg_Res;
struct	lua_State;
namespace google{
	namespace protobuf {
		class Message;
	}
}
extern void		pb2_stNetHost	(_stNetHost &st,	const PBCfg_Net &pb);
extern void		pb2_stNetAddress(_stNetAddress &st, const PBCfg_Net &pb);
extern void		pb2_stDBConfig	(_stDBConfig &st,	const PBCfg_MysqlDB &pb);
extern void		pb2_stNetTimeout(_stNetTimeout &st,	const PBCfg_Net &pb);
extern void		pb2_stNetTimeout(_stNetTimeout &st,	const PBCfg_NetTimeout &pb);