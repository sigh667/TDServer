/*----------------- buffModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/2/1 20:14:11
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/memoryFunctions.h"
#include "math/range.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ buff持续增加消耗属性
struct _stBuffContiueAddExpend
{
	uint32		uEffectTime;		//生效时间
	uint32		uStepTime;			//间隔时间
	uint16		uExpendType;		//_emExpendType
	uint8		uTargetCategory;	//_emEffectTargetType
	uint16		uAttrType;			//_emBattleAttribute
	int32		iRadio;				//概率 万分比
	int32		iValue;				//增加当前值
	uint16		uDelExpendType;		//_emExpendType
	int32		iDelValue;			//消耗值
};
//-------------------------------------------------------------
//------------------------------ buff持续增加Buff
struct _stBuffContiueAddBuff
{
	uint32		uEffectTime;		//生效时间
	uint32		uStepTime;			//间隔时间
	uint32		uBuffID;
};

//-------------------------------------------------------------
//------------------------------ buff增加位移
struct _stBuffAddMove
{
	uint8		uBuffMoveType;		//位移类型 _emBuffMoveType
	uint8		uDirectionType;		//方向类型 _emDirectionType
	float32		fSpeed;				//速度
	uint8		uTargetType;		//位移目标类型 _emBuffMoveTargetType
};
//-------------------------------------------------------------
//------------------------------ buff持续间隔释放技能
struct _stBuffContiueUseSkill
{
	uint32		uEffectTime;		//生效时间
	uint32		uStepTime;			//间隔时间
	uint32		uSkillID;			//技能ID
};
//-------------------------------------------------------------
//------------------------------ buff增加额外伤害
struct _stBuffSkillExtraDamage
{
	uint32		uSkillID;			//技能ID
	uint32		uValue;				//值
	uint32		uRadio;				//万分比
};
//-------------------------------------------------------------
//------------------------------ buff减少技能最大冷却时间
struct _stBuffSkillCoolTime
{
	uint32		uSkillID;			//技能ID
	uint8		uSkillTimeType;		//技能时间类型 _emSkillTimeType
	uint32		uRadio;				//万分比

	_stBuffSkillCoolTime()	{	memset(this,0,sizeof(*this)); }

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ buff减少技能最大冷却时间
struct _stBuffDeleteTriggerEvent
{
	uint8		uType;				//技能ID
	uint8		uSkillTimeType;		//技能时间类型 _emSkillTimeType
	uint32		uRadio;				//万分比
};
//-------------------------------------------------------------
//------------------------------ buff增加治疗效果 参数
struct _stBuffAddCureEffet 
{
	float32		fHPRadio;		//触发条件 生命百分比
	bool		bGreater;		//触发条件 是否大于等于
	float32		fEffetRadio;	//增加的效果
};
//-------------------------------------------------------------
//------------------------------ buff增加伤害效果 参数
struct _stBuffAddHarmEffet 
{
	float32		fHPRadio;		//触发条件 生命百分比
	bool		bGreater;		//触发条件 是否大于等于
	float32		fEffetRadio;	//增加的效果
	uint32		uBeSkillID;		//技能ID 为0 所有技能都生效
};
//-------------------------------------------------------------
//------------------------------ buff增加增加暴击率 参数
struct _stBuffAddCritical 
{
	uint8		uConditionType; //条件类型 _emBuffCriticalConditionType
	int32		nParam;			//参数
	float32		fEffetRadio;	//增加的效果
};
//-------------------------------------------------------------
//------------------------------ 增加持续BUFF持续时间 参数
struct _stBuffAddBuffTime
{
	uint16		uBuffGroupID;	//buff的组ID
	float32		fEffetRadio;	//增加的效果
};
//-------------------------------------------------------------
//------------------------------ 吸血
struct _stBuffSuckHP
{
	bool		bNeedCritical;	//是否需要爆击
	float32		fEffetRadio;	//吸血百分比
};
//-------------------------------------------------------------
//------------------------------ 增加持续BUFF持续时间 参数
struct _stBuffAddSkillBuffTime
{
	uint16		uBuffGroupID;	//buff的组ID
	int32		nAddBuffMS;		//增加的毫秒
};
//-------------------------------------------------------------
//------------------------------ 增加持续BUFF持续时间 参数
struct _stBuffAddTargetHarm
{
	float32		fHPRadioMax;	//触发条件 生命百分比上限
	float32		fHPRadioMin;	//触发条件 生命百分比下线
	float32		fHPSep;			//每掉生命百分比
	int32		nAddHarmValue;	//增加伤害绝对值
	float32		fAddHarmRadio;	//增加伤害百分比
};
//-------------------------------------------------------------
//------------------------------ 一种消耗值增加影响另外一种消耗值增加
struct _stBuffExpendAddExpend
{
	uint16 uExpendType1;
	int32 iValue1;
	uint16 uExpendType2;
	int32 iValue2;
};
//-------------------------------------------------------------
//------------------------------ 被技能命中反击使用技能
struct _stBuffBeSkillHitUseSkill
{
	uint32 uBeSkillID;
	uint32 uUseSkillID;
};
//-------------------------------------------------------------
//------------------------------ 被技能命中删除buff
struct _stBuffBeSkillHitDelBuff
{
	uint32 uBeSkillID;
};
//-------------------------------------------------------------
//------------------------------ 
struct _stBuffModelBase
{
	uint32					uBuffID;
	uint8					uLevel;	
	uint32					uGroupID;				//组ID 参照表cs_buff_group_mutual
	uint8					uDifRepeatType;			//不同来源叠加类型 _emBuffDifRepeatType
	uint8					uMaxLevel;				//最大等级
	uint8					uBuffType;				//类型 _emBuffType
	uint16					uStateType;				//状态
	uint32					uStateBuffAttrMark;		//状态类buff的属性掩码 _emStateBuffAttr(为了加快访问速度 优化到这里了 )
	uint8					uEffectType;			//效果类型 _emEffectType
	uint8					uRepeatType;			//叠加类型 _emBuffSameRepeatType
	uint32					uContinueTime;			//持续时间
	uint32					uMaxContinueTime;		//最大持续时间	
	uint8					uMaxLayer;				//最大层数 无层数默认1
	bool					bIsClearLayer;			//时间到是否清除
	uint32					uMark;					//掩码 _emBuffMark
	uint8					uBuffCat;				//类型 _emBuffCat
	union 
	{// uBuffType 不一样而不一样	
		_stBuffContiueAddExpend		stContiueAddExpend;		//持续增加消耗属性 BuffType_ContiueAddExpend
		_stBuffContiueAddBuff		stContiueAddBuff;		//持续增加Buff BuffType_ContiueAddBuff
		_stBuffAddMove				stAddMove;				//buff增加位移 BuffType_Move
		_stBuffContiueUseSkill		stContiueUseSkill;		//持续释放技能 BuffType_ContiueUseSkill
		_stAddExpend				stAttackAddExpend;		//攻击加消耗属性 BuffType_AttackAddExpend
		_stBuffSkillExtraDamage		stSkillExtraDamage;		//技能攻击额外伤害 BuffType_SkillExtraDamage
		int32						nReduceSkillExpend;		//减少技能消耗万分比 BuffType_SkillExpendReduce
		_stBuffAddCureEffet			stBuffAddCureEffet;		//修正携带者被治疗效果 BuffType_AddCureEffet
		_stBuffAddHarmEffet			stBuffAddHarmEffet;		//修正携带者被伤害效果 BuffType_AddHarmEffet
		_stBuffAddCritical			stBuffAddCritical;		//修正携带者暴击率 BuffType_AddCriticalEffet
		_stBuffAddBuffTime			stBuffAddBuffTime;		//修正携带者BUFF持续时间 BuffType_AddBuffTime
		_stBuffSuckHP				stBuffSuckHP;			//携带者攻击吸血 BuffType_SuckHP
		float32						fBuffAddHarmMonster;	//修正携带者对怪物的伤害 BuffType_AddHarmMonster
		_stBuffAddSkillBuffTime		stBuffAddSkillBuffTime; //使用眩晕技能时增加buff持续时间 BuffType_AddSkillBuffTime
		_stBuffAddTargetHarm		stBuffAddTargetHarm;	//修正携带者的伤害(血量越低伤害越高) BuffType_AddTargetHarm
		_stBuffExpendAddExpend		stExpendAddExpend;		//一种消耗值增加影响另外一种消耗值增加 BuffType_ExpendAddExpend
		_stBuffBeSkillHitUseSkill	stBeSkillHitUseSkill;	//被技能命中来源在buff主上使用技能 BuffType_BeSkillHitUseSkill
		_stBuffBeSkillHitDelBuff	stBeSkillHitDelBuff;	//被技能命中删除buff BuffType_BeSkillHitDelBuff
		int32						nHatredLevel;			//改变仇恨等级 BuffType_HatredLevel
		_stBuffBeSkillHitUseSkill	stBeSkillHitTarUseSkill;//被技能命中来源在目标上使用技能 BuffType_BeSkillHitUseSkill

	};

	_stBuffModelBase&	getBase		()				{	return *this;							}
	inline void			initBase	()				{	dMemset(this,0,sizeof(*this));			}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 状态类buff的属性
struct _stBuffStateAttrModel
{
	uint32					uStateID;
	uint32					uStateAttrMark;		//状态类buff的属性掩码

	_stBuffStateAttrModel&	getModel	()				{	return *this;							}
	inline void				initModel	()				{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stBuffModel : _stBuffModelBase
{
	typedef stl_vector<_stAddBattleAttri> VecAddBattleAttri;
	typedef stl_map<uint32,_stBuffSkillCoolTime>	MAP_SKILL_COOLTIME;

	MAP_U8_U32			mapBuff;				//层数加buff
	MAP_U8_U32			mapSkillID;				//层数使用技能ID
	MAP_U8_U32			mapDeleteTriggerEvent;	//删除buff触发事件 KEY:_emBuffDeleteTriggerEventType
	VecAddBattleAttri	vecAddBattleAttr;		//增加战斗属性
	MAP_SKILL_COOLTIME	mapSkillCoolTime;		//减少技能冷却时间 BuffType_ChangeSkillTime

	inline bool		check_mark	(uint8 _mark)	const	{	return _CHECK_BIT(uMark,_BIT32(_mark));	}
	_stBuffModel&	getModel	()						{	return *this;							}
	inline void		initModel	()				
	{
		initBase();
		mapBuff.clear();
		mapDeleteTriggerEvent.clear();
		mapSkillID.clear();
		vecAddBattleAttr.clear();
		mapSkillCoolTime.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBuffModelBase::read(stream)&&
			stream.read(mapBuff)&&
			stream.read(mapSkillID)&&
			stream.read(vecAddBattleAttr)&&
			stream.read(mapDeleteTriggerEvent)&&
			stream.read(mapSkillCoolTime);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBuffModelBase::write(stream)&&
			stream.write(mapBuff)&&
			stream.write(mapSkillID)&&
			stream.write(vecAddBattleAttr)&&
			stream.write(mapDeleteTriggerEvent)&&
			stream.write(mapSkillCoolTime);
	}
};