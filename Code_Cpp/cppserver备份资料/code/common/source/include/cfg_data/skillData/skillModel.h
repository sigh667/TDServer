/*----------------- skillModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/21 13:39:53
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/memoryFunctions.h"
#include "math/range.h"
#include "def_struct/stlDefine.h"
#include "math/point2.h"
#include "basic/basicTypes.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 增加对象
struct _stSkillAddObject
{
	uint8		uObjectType;		//对象类型 _em_GameUnitType
	uint32		uOjbectID;			//对象ID
	uint8		uTargetPositonType;	//目标位置类型  _emEffectTargetType
	float32		fDistance;			//相对位置 可以为负
	float32		fHigh;				//高度
	float32		fRandDis;			//随机半径米
	uint8		uObjectNum;			//陷阱个数；怪物个数;
	uint16		uParam1;			//陷阱类型_emTrapAngleType 怪物不用
	uint16		uParam2;			//陷阱子弹发射角度 左右总和;   怪物不用
	uint8		uExpendType;		//额外消耗值
	int32		iValue;				//消耗数量
	uint8		uAddNum;			//增加个数

	_stSkillAddObject()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 增加对象
struct _stSkillAddBuff
{
	uint16	uExpendType;//消耗成功
	int32	iValue;
	uint32	uBuffID;

	_stSkillAddBuff()	{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ 技能效果类型
struct _stSkillEffectType
{
	uint8	uEffectType;		//_emSkillEffectType
	uint8	uEffectTargetType;	//_emEffectTargetType
	bool	bSkillHit;			//依赖技能命中
	bool	bAddOneCount;		//是否只加一次

	_stSkillEffectType()	{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ 技能范围
struct _stSkillRangeData
{
	uint8				uRangeType;			//范围类型 _emRangeType
	float32				fLength;			//长 圆表示半径 环表示外半径
	float32				fWidth;				//宽 圆表示角度 环表示内半径
	float32				fOffset;			//偏移量
	float32				fOffsetAngle;		//偏移朝向的角度

	_stSkillRangeData()	{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 组合技能
struct _stSkillCompositeData
{
	uint8		uType;							//组合类型 _emSkillCompositeType
	uint8		uAtkPart;						//段位
	uint32		uNextSkillID;					//下一个技能ID
	uint32		uAtkPartCoolTicket;				//段位冷却时长(毫秒)

	_stSkillCompositeData()	{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 增加对象
struct _stSkillBeAttackMove
{
	uint8				uDirectionType;		//受击位移类型 _emDirectionType
	float32				fBeAttackMove;		//受击位移距离
	uint32				uBuffID;			//buffID 眩晕用

	_stSkillBeAttackMove()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 消耗增加额外伤害
struct _stSkillExpendAddDamage
{
	uint8	uExpendType;		//_emExpendType
	int32	iValue;
	uint32	uRate;				//万分比
	uint32	uDamage;			//伤害值

	_stSkillExpendAddDamage()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 增加位移
struct _stSkillEffectAddMove
{
	uint8				uAddMoveType;		//受击位移类型 _emEffectAddMoveType
	int32				iDistance;			//位移距离，负数表示后退

	_stSkillEffectAddMove()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 技能效果基础数据
struct _stSkillEffectBase
{
	uint32				uEffectID;			//效果ID
	uint8				uLevel;				//效果等级
	uint8				uMaxAffectNum;		//最大的目标个数
	uint8				uTargetSelectType;	//目标选取方式  _emTargetSelectType
	int32				nHatredValue;		//附加仇恨
	uint32				uExtraDamage;		//附加伤害值
	uint32				uExtraDamageRadio;	//附加伤害系数
	uint32				uAverageDamageRadio;//均摊伤害系数 除以目标个数 影响暴击倍率
	uint32				uParamDamageRadio;	//外环伤害系数
	uint32				uOutRingAddBuff;	//外环增加buff
	uint32				uExtraCure;			//附加治疗值
	uint32				uExtraCureRadio;	//附加治疗系数
	_stSkillAddObject	stAddObject;		//增加对象
	_stSkillBeAttackMove stBeAttackMove;	//受击位移
	uint16				uHitUseSkill;		//命中目标处使用技能
	_stSkillExpendAddDamage stExpendAddDamage; //消耗增加额外伤害
	uint32				uResetBuffID;		//重置buff时间的ID
	_stSkillEffectAddMove	stAddMove;		//增加位移
	_stSkillEffectBase&	getBase()	{	return *this;					}
	inline void			initBase()	{	dMemset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ 技能动作基础数据
struct _stSkillAnimBase
{
	uint32	uTotalTick;		//动作总时间

	_stSkillAnimBase&	getBase()	{	return *this;					}
	inline void		initBase()		{	dMemset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ 技能基础数据
struct _stSkillBase
{
	uint32		uSkillID;						//技能ID
	uint8		uType;							//分类 _emSkillType
	uint8		uMaxLevel;						//技能最大等级
	uint8		uLevel;							//技能等级
	uint8		uBehavior;						//使用类型(_emSkillBehavior)
	uint64		uMark;							//技能掩码(_emSkillMark)
	uint8		uDirStrategy;					//技能方向选择 _emSkillDirStrategy
	uint32		uChargeTime;					//技能吟唱时长(毫秒)
	uint32		uContinueTime;					//技能持续时长(毫秒)
	uint32		uCoolTicket;					//冷却时长(毫秒)
	uint32		uGroupCoolTicket;				//公共CD冷却时长(毫秒)
	uint32		uTagetCategory;					//目标种类 _emTargetCategory
	uint32		uReleaseRange;					//技能射程
	uint16		uHaloID;						//光环ID
	_stSkillCompositeData stCompositeData;		//组合数据
	uint8		uUseType;						//使用类型_emSkillUseType
	int32		nUseParam;						//使用参数
	int32		nRangeWarnType;					//技能警告类型
	int32		nRangeWarnID;					//技能警告id
	uint32		uCompositeBaseSkillID;			//组合基础技能技能ID
	uint32		uInterruptValue;				//中断值
	_stSkillExpendAddDamage stExpendAddDamage; //消耗增加额外伤害

	_stSkillBase&	getBase()	{	return *this;					}
	inline void		initBase()	{	dMemset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 技能效果基础模版
struct _stSkillEffectModel	: _stSkillEffectBase
{
	typedef stl_map<uint8,_stSkillRangeData> MapSkillRangeData;
	typedef double_map<uint8,uint32,_stSkillAddBuff> DMAP_SKILLADDBUFF;
	DMAP_SKILLADDBUFF						dmapAddBuff;		//增加buff key1:_emEffectTargetType,key2:buffid
	MAP_U16_I64								mapAddExpend;		//增加目标消耗
	MAP_U16_I64								mapAddSelfExpend;	//增加自身消耗
	stl_map<uint16,_stAddBattleAttri>		mapAddSelfAttr;		//增加自身属性
	stl_vector<_stSkillEffectType>			vecEffectType;		//效果类型
	MapSkillRangeData						mapRangeData;		//范围数据 多个范围
	
	_stSkillEffectModel()		{	initModel();					}
	inline void		initModel()
	{
		initBase();
		dmapAddBuff.clear();
		mapAddExpend.clear();
		mapAddSelfExpend.clear();
		mapAddSelfAttr.clear();
		vecEffectType.clear();
		mapRangeData.clear();

	}
	inline _stAddBattleAttri* findAttr(uint16 uAttriType)
	{
		return mapAddSelfAttr.find_(uAttriType);
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stSkillEffectBase::read(stream)&&
			stream.read(dmapAddBuff)&&
			stream.read(mapAddExpend)&&
			stream.read(mapAddSelfExpend)&&
			stream.read(mapAddSelfAttr)&&
			stream.read(vecEffectType)&&
			stream.read(mapRangeData);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stSkillEffectBase::write(stream)&&
			stream.write(dmapAddBuff)&&
			stream.write(mapAddExpend)&&
			stream.write(mapAddSelfExpend)&&
			stream.write(mapAddSelfAttr)&&
			stream.write(vecEffectType)&&
			stream.write(mapRangeData);
	}
};

//-------------------------------------------------------------
//------------------------------ 
struct _stAttEffect
{
	uint32						uTick;
	float32						fDir;
	float32						fDistance;
	uint8						uUseRange;
	const _stSkillEffectModel*	pEffectModel;

	_stAttEffect()
	{
		init();
	}

	void init()
	{
		uTick = 0;
		pEffectModel = NULL;
		uUseRange = 1;
		fDir		= 0.f;
		fDistance	= 0.f;
	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(uTick)&&
			stream.read(fDir)&&
			stream.read(fDistance)&&
			stream.read(uUseRange);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(uTick)&&
			stream.write(fDir)&&
			stream.write(fDistance)&&
			stream.write(uUseRange);
	}
};

//-------------------------------------------------------------
//------------------------------ 技能动作基础模版
struct _stSkillAnimModel	: _stSkillAnimBase
{
	stl_vector<_stAttEffect>		vecAttEffect;//攻击位移帧
	inline void		initModel()
	{
		initBase();
		vecAttEffect.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stSkillAnimBase::read(stream)&&
			stream.read(vecAttEffect);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stSkillAnimBase::write(stream)&&
			stream.write(vecAttEffect);
	}
};

//-------------------------------------------------------------
//------------------------------
struct _stSkillSpareParam
{
	stl_vector<float32> vecParams;
	_stSkillSpareParam() {   vecParams.clear(); }

	inline bool		read			(CStreamBase& stream)
	{
		return 	stream.read(vecParams);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return 	stream.write(vecParams);
	}
};

//-------------------------------------------------------------
//------------------------------ 技能基础模版
struct _stSkillModel	: _stSkillBase
{
	_stSkillAnimModel*						pAnimModel;		//动作模板
	MAP_U16_I64								mapAddExpend;	//消耗属性
	VEC_U32									vecEffect;		//效果组
	stl_vector<const _stSkillEffectModel*>	vecEffectModel;	//动作组模板
	stl_map<uint8,_stSkillSpareParam>		mapSpareParam;	//自由参数
	SET_U32									setOugiSkill;	//奥义技能
	SET_U32									setNeedJob;		//需要的职业

	inline bool		check_mark	(uint8 _mark)const	{	return _CHECK_BIT(uMark,_BIT64(_mark));	}
	_stSkillModel() {   initModel(); }
	inline const _stSkillSpareParam* findParam(uint8 uType)	const	{	return mapSpareParam.find_(uType);	}
	inline void		initModel()
	{
		initBase();
		mapAddExpend.clear();
		pAnimModel = NULL;
		vecEffect.clear();
		vecEffectModel.clear();
		mapSpareParam.clear();
		setOugiSkill.clear();
		setNeedJob.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stSkillBase::read(stream)&&
			stream.read(mapAddExpend)&&
			stream.read(vecEffect)&&
			stream.read(mapSpareParam)&&
			stream.read(setNeedJob)&&
			stream.read(setOugiSkill);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stSkillBase::write(stream)&&
			stream.write(mapAddExpend)&&
			stream.write(vecEffect)&&
			stream.write(mapSpareParam)&&
			stream.write(setNeedJob)&&
			stream.write(setOugiSkill);
	}
};