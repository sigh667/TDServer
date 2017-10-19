/*----------------- gameBuffMgr.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/26 14:48:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "memoryPool/memoryPool.h"
#include "memoryPool/memoryPool.h"
#include "gameLogic/gameBuff/gameBuff.h"
#include "protocol/GameStruct.pb.h"
#include "def_struct/stlDefine.h"
/*************************************************************/
class CSceneUnit;
class CScenePlayer;
class CClientPacketMgr;
class PBSystemBuff; 
class CGameBuffMgr
{
private:
	typedef double_map<uint32,uint32,CGameBuff*>::_sub_map	SUB_DMAP_BUFF;
	typedef CMemoryPool<CGameBuff>							POOL_BUFF;
	typedef double_map<uint32,uint32,CGameBuff*>			DMAP_BUFF;		//key1:buffid,key2:unitid 
	typedef	stl_map<uint32,uint32>							MAP_BUFF_ONETIME;

private:
	CSceneUnit*			m_pOwner;			//所有者
	POOL_BUFF			m_poolBuff;			//buff列表
	DMAP_BUFF			m_dmapBuff;			//buff列表
	uint64				m_uProcessTick;
	MAP_BUFF_ONETIME	m_mapOneTimeBuff;	//增加一次的buff
public:
	CGameBuffMgr();
	~CGameBuffMgr();

public:
	inline	bool			canOperate	()									{   return m_pOwner != NULL;					}
	CGameBuff*				find		(uint32 uBuffID,uint32 uSourceID);	
	CGameBuff*				findFirst	(uint32 uBuffID);	
	CGameBuff*				alloce		(const _stBuffModel* pModel,uint32 uSourceID);
	bool					getGroupBuff(uint32 uGroupID,stl_vector<CGameBuff*>& vecGameBuff);

public:
	uint32					getExtraDamage			(uint32 uSourceID,uint32 uSkillID,int32 iDamage);
	uint32					getSkillTimeByTime		(uint8 uSkillTimeType,uint32 uSkillID,uint32 uCoolTick);
	int32					getReduceSkillExpendRate();
	float32					getAddCureRate			();
	float32					getAddHarmRate			(uint32 uSkillID = 0);
	float32					getAddCriticalRate		(CSceneUnit* pTarget);
	uint32					getBuffTime				(const _stBuffModel* pBuffModel);
	int32					getAtkBuffAddTime		(const _stBuffModel* pBuffModel);
	float32					getSuckHPRate			(bool bCritical);
	float32					getMonsterExtraDamage	();
	void					getAddTargetHarm		(int32& nAddValue,float32& fAddRate);

	uint32					getStateBuffAttr		();
	bool					refreshAttr				();

protected:
	bool					checkCriticalConditon	(const _stBuffAddCritical& stConditon);
public:
	void	clear			();
	void	initialize		(CSceneUnit* pOwner);
	void	onClientGo		();
	bool	load			(const PBSystemBuff& pbSkill);
	bool	save			(PBSystemBuff* pbSkill);

public:
	bool	addBuffAsk		(PBBuff* pBuff,uint8& uCode);
	bool	delBuffAsk		(uint32 uBuffID,uint8& uCode);
	void	onDelete		(CGameBuff* pBuff);

public:
	bool	canAddBuff		(const _stBuffModel* pModel);
	bool	DelOldBuff		(const _stBuffModel* pModel);
	bool	addBuff			(uint32 uBuffID,uint8 uLevel,uint32 uSourceID,uint32 uSkillID,_emDoingType emDoing );
	bool	addBuff			(uint32 uBuffID,uint8 uLevel,uint64 uLeftTick,uint32 uSourceID,uint32 uSkillID,bool bSend , _emDoingType emDoing);
	bool	delBuff			(uint32 uBuffID,_emDoingType emDoing);
	bool	delBuff			(CGameBuff* pBuff,bool bSend , _emDoingType emDoing);
	bool	clearBuff		(_emEffectType emEffectType,_emDoingType emDoing);
	bool	resetBuffTime	(CSceneUnit* pSource,uint32 uBuffID);

public:
	void	updatePassiveSkillBuff();
public:
	bool	process			(uint64 uTick,uint64 uTime,uint32 uSecond);

public:
	static bool	send_AddBuffAck		(uint32 uBuffID,uint8 uLevel,uint64 uLeftTick,uint32 uUnitID,uint32 uSkillID,uint8 uCode,bool bBroad,CSceneUnit* pOwner);
	static bool	send_AddMoveBuffAck	(uint32 uBuffID,uint8 uLevel,uint64 uLeftTick,uint32 uUnitID,uint32 uSkillID,uint8 uCode,bool bBroad,CSceneUnit* pOwner);
	static bool	send_AddBuffAck		(stl_vector<PBBuff>& vecPBBuff,uint8 uCode,bool bBroad,CSceneUnit* pOwner);
	static bool	send_DelBuffAck		(uint32 uBuffID,uint32 uSourceID,uint8 uCode,bool bBroad,CSceneUnit* pOwner);
	static bool	send_BuffHpAck		(int32 iAddHp,CSceneUnit* pOwner,CSceneUnit* pSource,_emDoingType emDoing);
	static bool	send_UpdateBuffTickAck	(uint32 uBuffID,uint32 uSouceID,uint64 uLeftTick,CSceneUnit* pOwner);

public:
	void	callbackCritical	();
	void	callbackSkillHit	();
	void	callbackUseSkill	();
	void	callbackUseSkillEnd	();
	void	callbackDead		();
	void	callbackChangeExpend(uint16 uExpendType,int32 iValue);
	void	callbackBeSkillHit	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	void	callback_flyout		();

};
