/*----------------- sceneModel.h
*
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/3/5 15:22:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "basic/memoryFunctions.h"
#include "timer/dateTime.h"
#include "basic/basicFunctions.h"
#include "def_struct/stlDefine.h"
/*************************************************************/
struct _stCopymapModel;
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
enum _emScenMapeMark
{
	SceneMapMark_canReliveOrigin	,//是否允许原地复活

	SceneMapMark_Max
};
//-------------------------------------------------------------
//------------------------------ 
enum _emBelongType
{
	BelongType_Normal	,//普通本国
	BelongType_BigWorld	,//跨服
	BelongType_Neutral	,//中立

	BelongType_Max
};
//-------------------------------------------------------------
//------------------------------ 
struct _stSceneMapBase
{
	uint16	uSceneID;
	uint8	uBelongType;	//归属关系(_emBelongType)
	uint8	uDefaultLineCount;
	uint16	uMaxLineCount;
	uint16	uDefaultPlayerCount;
	uint16	uPollPlayerCount;
	int32	nReliveLocalLimit;	// 原地复活和复活点复活 允许总数
	uint16	uBirthSceneID;		// 出生点地图ID
	uint16	uBirthRegionID;		// 出生点区域ID
	uint16	uReliveOriginCD;	// 原地复活CD
	uint16	uRelivePointCD;		// 复活点复活CD
	uint16	uReliveCityCD;		// 回城复活CD
	uint32	uReliveOriginItemID;// 原地复活需要消耗的道具
	uint32	uMark;				// 掩码 _emSceneMapMarkType
	_stSceneMapBase&getSceneMapBase	()					{	return *this;									}
	inline void		initBase()							{	dMemset(this,0,sizeof(*this));					}
	inline bool		testSceneMark	(uint8 _mark)const	{	return _CHECK_BIT(this->uMark,_BIT32(_mark));	}
	inline void		set_mark	(uint8 _mark,bool bSet)	{	_SET_BIT32(uMark,_mark,bSet);					}

	READ_WRITE_STRUCT
};
#pragma pack(pop)
//-------------------------------------------------------------
struct _stSceneMapModel	: _stSceneMapBase
{
	typedef stl_map<uint16,uint8> MAP_CONVERT;
	std::string							strResoureName;
	std::string							strName;
	MAP_CONVERT							mapConvert;
	std::string							strEnterScript;
	_stCopymapModel*					pCopymapModel;
	MAP_U8_U16							mapRelivePoint;

	_stSceneMapModel()							{	initModel();									}
	_stSceneMapModel&getModel	()				{	return *this;									}
	inline bool		isCopymap			()const	{	return pCopymapModel != NULL;					}
	inline void		initModel	()
	{	
		initBase();
		mapConvert.clear();		
		strEnterScript.clear();
		strResoureName.clear();
		pCopymapModel = NULL;
		strName.clear();
		mapRelivePoint.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return	_stSceneMapBase::read(stream)&&
				stream.read(strResoureName)&&
				stream.read(strEnterScript)&&
				stream.read(mapRelivePoint)&&
				stream.read(strName)&&
				stream.read(mapConvert);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return	_stSceneMapBase::write(stream)&&
			stream.write(strResoureName)&&
			stream.write(strEnterScript)&&
			stream.write(mapRelivePoint)&&
			stream.write(strName)&&
			stream.write(mapConvert);
	}
};
