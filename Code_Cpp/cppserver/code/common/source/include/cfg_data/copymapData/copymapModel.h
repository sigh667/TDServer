/*----------------- copymapModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/5/13 10:43:03
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/stlDefine.h"
#include "timer/dateTime.h"
#include "basic/systemFunction.h"
#include "stream/streamBase.h"
#include "math/point2.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//------------------------------------------------------
//------------------------------ 副本标记
enum _emCopymapMark
{
	CopymapMark_LeaderOpen			= 0,	//是否队长开启
	CopymapMark_UseTeamLevel		= 1,	//是否使用队伍平均等级(副本刷怪用)
};
//------------------------------------------------------
//------------------------------ 退出类型
enum _emCopymapLeaveType
{
	CopymapLeaveType_OldPositon		= 1,	//传送至进副本前坐标
	CopymapLeaveType_LeaveScene		= 2,	//传送到指定坐标
};
//-------------------------------------------------------------
//------------------------------ 
struct _stUnitGroupBase
{
	uint32		uGroupID;			//组ID
	uint8		uUnitType;			//对象类型 _em_GameUnitType
	uint32		uUnitID;			//对象ID
	uint16		uUnitCount;			//对象个数
	float32		fRandDir;			//随机半径
	uint32		uRouteID;			//路径点

	_stUnitGroupBase&	getBase	()			{	return *this;						}
	inline void		initBase	()			{	memset(this,0,sizeof(*this));		}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stCopymapWaveBase
{
	uint16		uCopymapID;			//副本ID跟场景ID相同
	uint16		uWaveID;			//副本波次ID
	uint32		uWaveStepTime;		//波次时间
	uint32		uGroupOrNum;		//组或不重复数量

	_stCopymapWaveBase&	getBase	()			{	return *this;						}
	inline void		initBase	()			{	memset(this,0,sizeof(*this));		}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stCopymapBase
{
	uint16		uCopymapID;			//副本ID跟场景ID相同
	uint32		uMainType;			//大类_emCopymapMainType
	uint32		uType;				//玩法类型_emCopymapType
	uint8		uDifficulty;		//副本难度 _emCopymapDifficulty
	uint32		uMark;				//(_emCopymapMark位)
	uint32		uReadyTime;			//副本准备时长
	uint32		uSurvivalTime;		//副本持续时间(秒)
	uint32		uExtraTime;			//额外补时(秒)
	uint32		uSpecialExtraTime;	//特殊补时(秒) 灵犀一动时间
	uint8		uEnterType;			//副本进入类型(_emCopymapEnterType)
	uint8		uEnterCopyType;		// 进入副本处理(_emCopymapEnterCopyType)
	uint32		uEnterCount;		//副本日次数
	uint32		uDayPrizeCount;		//副本日奖励次数
	uint32		uCopymapTimeType;	// 扣除次数方式 (_emCopymapCopymapTimeType)
	uint32		uOneBossID;			// 第一个Boss怪物ID 
	bool		bCantRelive;		//  可否复活
	uint8		uResetType;			//重置类型(_emCopymapResetType)
	uint32		uPlayerCount;		//人数上限
	uint32		uAssistPlayerCount;	//可邀请助战人数上限
	uint8		uLeaveType;			//退出类型 _emCopymapLeaveType
	uint16		uLeaveSceneID;		//退出的场景ID
	uint16		uLeaveRegionID;		//退出的场景ID
	uint32		uTaskID;			//任务ID
	uint8		uLeaveTeam2ndConfirm;//离开队伍是否需要2次确认,0不需要,1需要
	uint8		uLeaveTeamAllowed;	//是否允许离开队伍,0可以,1不可以
	uint16		uNextCopymapID;		//后续副本ID
	uint32		uNextCopymapIDRate;	//后续副本ID概率


	_stCopymapBase&	getBase	()							{	return *this;											}
	inline void		initBase	()						{	memset(this,0,sizeof(*this));							}
	inline bool		check_mark		(uint8 _mark)const	{	return _CHECK_BIT(uMark,_BIT32(_mark));					}
	inline bool		check_enter		(uint8 _enter)const	{	return _CHECK_BIT(uEnterType,_BIT32(_enter));			}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stUnitGroupModel	: public _stUnitGroupBase
{
	stl_map<uint32,_stPoint2F> mapBirthPostion;		//key出生类型

	_stUnitGroupModel&getModel	(){	return *this;					}
	inline void		initModel()
	{
		initBase();
		mapBirthPostion.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stUnitGroupBase::read(stream)&&
			stream.read(mapBirthPostion);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stUnitGroupBase::write(stream)&&
			stream.write(mapBirthPostion);
	}
};
typedef stl_vector<const _stUnitGroupModel*> VEC_WAVE_UNITGROUP;
//-------------------------------------------------------------
//------------------------------ 
struct _stCopymapWaveModel	: public _stCopymapWaveBase
{
	VEC_U32		vecGroupAnd;				// 组与
	VEC_U32		vecGroupOr;					// 组或者

	_stCopymapWaveModel&getModel	(){	return *this;					}
	inline void		initModel()
	{
		initBase();
		vecGroupAnd.clear();
		vecGroupOr.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stCopymapWaveBase::read(stream)&&
			stream.read(vecGroupAnd)&&
			stream.read(vecGroupOr);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stCopymapWaveBase::write(stream)&&
			stream.write(vecGroupAnd)&&
			stream.write(vecGroupOr);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stCopymapModel	: public _stCopymapBase
{
	stl_vector<_stDateTime>					vecOpenTime;				// 开启时间
	DMAP_UINT8_32_32						dmapPreCondition;			// 前提条件
	DMAP_NEED_ITEM							mapNeedItem;				// 扣除的道具
	MAP_U16_I32								mapGiveIdentitySkill;		// 临时身份技能
	stl_map<uint16,_stCopymapWaveModel*>	mapWaveModel;				// 怪物波数
	MAP_U32_U32								mapPrefectCondition;		// 完美条件
	VEC_U32									vecGiveSkill;				// 临时技能
	MAP_U16_U8								mapEnterRegion;				//进入副本的区域 KEY:区域ID VALUE:阵营

	_stCopymapModel&getModel	(){	return *this;					}
	inline void		initModel()
	{
		initBase();
		dmapPreCondition.clear();
		mapNeedItem.clear();
		vecOpenTime.clear();
		mapGiveIdentitySkill.clear();
		mapPrefectCondition.clear();
		mapWaveModel.clear();
		vecGiveSkill.clear();
		mapEnterRegion.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stCopymapBase::read(stream)&&
			stream.read(vecOpenTime)&&
			stream.read(mapNeedItem)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapGiveIdentitySkill)&&
			stream.read(vecGiveSkill)&&
			stream.read(mapEnterRegion)&&
			stream.read(mapPrefectCondition);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stCopymapBase::write(stream)&&
			stream.write(vecOpenTime)&&
			stream.write(mapNeedItem)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapGiveIdentitySkill)&&
			stream.write(vecGiveSkill)&&
			stream.write(mapEnterRegion)&&
			stream.write(mapPrefectCondition);
	}
	inline bool		isInOpenTime	(uint64 uTime) const 
	{
		if ( vecOpenTime.empty() )
			return true;

		uint32	uSize 	= vecOpenTime.size();
		for ( uint32 i = 0; i < uSize; ++i )
		{
			const  _stDateTime& stDataTime = vecOpenTime[i];
			if (stDataTime.toDataTime(uTime))
				return true;
		}
		return false;
	}
	inline const _stCopymapWaveModel*	findWave	(uint16 uWaveID) const
	{
		stl_map<uint16,_stCopymapWaveModel*>::const_iterator _pos = mapWaveModel.find(uWaveID);
		return _pos != mapWaveModel.end() ? _pos->second : NULL;
	}
	inline uint16 getBirthRegion	(uint8 uCampID) const
	{
		CONST_ITER_MAP_U16_U8 _pos = mapEnterRegion.begin();
		for (; _pos != mapEnterRegion.end(); ++_pos)
		{
			if (_pos->second == uCampID)
				return _pos->first;
		}
		return 0;
	}
};
