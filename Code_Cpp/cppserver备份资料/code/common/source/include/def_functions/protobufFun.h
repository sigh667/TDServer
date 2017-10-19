/*------------- protobufFun.h
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.01
* Date         : 2016/03/15
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "basic/basicTypes.h"
#include "google/protobuf/message.h"

/*************************************************************/
typedef google::protobuf::Message	ProtobufMsg;
typedef google::protobuf::int32		int32;

extern	ProtobufMsg*		createMessage	(const std::string& type_name);
extern	const std::string*	findEnumValueStr(const std::string& enum_name, const int32 iValue);
extern	const int			findEnumValue	(const std::string& enum_name, std::string& value_name);
extern	const std::string	findSubProtocol_C2S(uint8 uMainProtocol, uint8 uProtocol);
extern	const std::string	findSubProtocol_S2C(uint8 uMainProtocol, uint8 uProtocol);
extern	const std::string	findResultCode(uint8 uMainProtocol, uint8 uCode);