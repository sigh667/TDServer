/*----------------- gameSkillMgr.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/20 17:31:28
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "memoryPool/mapPool.h"
#include "gameLogic/gameSkill/gameSkill.h"
#include "protocol/GameDefine.pb.h"
#include "memoryPool/queue_pool.h"
/*************************************************************/
class CSceneUnit;
class PBSystemSkill;
struct _stItemModel;
class CGameSkillMgr	
{
	friend	class	CPlayerSkill;
	friend	class	CGameBuffMgr;
protected:
	enum _emSkillCmdType
	{
		SkillCmdType_Use,			// 使用技能
		SkillCmdType_Charge,		// 释放蓄力技能
		SkillCmdType_Break,			// 打断技能
	};
	struct _stSkillCmd
	{
		_emSkillCmdType			eCmdType;
		const _stSkillModel*	pSkillModel;
		float32					fDirection;
		uint32					uTarUnitID;
		_stPoint2F				stSelfPoint;
		_stPoint2F				stTargetPoint;

		void	initialize()
		{
			pSkillModel	= NULL;
			fDirection	= 0.f;
			uTarUnitID	= 0;
			stSelfPoint.zero();
			stTargetPoint.zero();
		}
	}; 
public:
	typedef CMapPool<uint32,CGameSkill>	POOL_SKILL;
	typedef	stl_vector<CGameSkill*>		VEC_SKILL;
	typedef queue_pool<_stSkillCmd>		QUEUE_CMD;
private:
	CSceneUnit*			m_pOwner;						//所有者
	POOL_SKILL			m_mapSkill;						//技能列表
	uint64				m_uProcessTick;
	VEC_SKILL			m_vecSkill[_emSkillUseType_ARRAYSIZE];//按技能使用方式来分类
	uint32				m_uContinueNotHit;				//连续未命中
private:
	QUEUE_CMD			m_queueCmd;						// 命令队列  为了能连贯的使用连续技能儿设计的 原因如下：
														// 1.我们不能使用 前端不等待后端消息就先播放技能动作的方式 因为我们很多技能都带技能位移（比如 熊军普通攻击），如果使用前端先播放动作的方式 那前后端的位置就无法保证一致了。
														// 2.我们不能使用 前端等待后端消息后才采取下一步动作的方式 因为我们有很多连续技能（比如 熊军普通攻击就是三段连续攻击） 若采取等待后端通知的上一个技能施放消息后才 
														//	 发送使用下一个技能的消息的话 中间就有网络延迟+处理延迟导致的间隔时间  这样的话 连续技能就会变得不连贯。
														// 3.这个命令队列就是 为了处理上述两种情况设计的
public:
	CGameSkillMgr();
	~CGameSkillMgr();

public:
	inline CSceneUnit*	getOwner			()						{	return m_pOwner;								}
	inline CGameSkill*	findSkill			(uint32 uSkillID)		{	return m_mapSkill.find(uSkillID);				}
	inline	void		bind				(CSceneUnit* pUnit)		{	m_pOwner = pUnit;								}
	inline	bool		canOperate			()						{	return m_pOwner != NULL;						}
	
public:
	uint8	getSkillLevel		(uint32 uSkillID);
	void	gatherHaloVec		(VEC_U16& haloVec);
public:
	void	clear				();
	void	initialize			(CSceneUnit* pOwner);
	bool	load				(const PBSystemSkill& pbSkill);
	bool	save				(PBSystemSkill* pbSkill);

public:
	bool	canUse				(uint32 uSkillID,uint8& uCode);
	bool	canAdd				(uint32 uSkillID,uint8& uCode);
	bool	canUpgrade			(uint32 uSkillID,uint8 uLevel,uint8& uCode);
	void	updateTempSkill		();
public:
	//--- 请求使用技能
	bool	useSkillAsk		(uint32 uSkillID,float32 fDirection,uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode);
		//---  请求添加技能
	bool	addSkillAsk		(uint32 uSkillID,uint8 uLevel,uint8& uCode);
	//---  请求升级技能
	bool	upgradeSkillAsk	(uint32 uSkillID,uint8 uLevel,uint8& uCode);
public:
	//--- 使用技能 指令预存
	bool	addSkillUseCmd	(uint32 uSkillID,float32 fDirection,uint32 uTarUnitID,_stPoint2F&stSelfPoint,_stPoint2F&stTargetPoint,uint8& uCode);
	//--- 释放蓄力技能 指令预存
	bool	addSkillChargeCmd(uint32 uSkillID,uint8& uCode);
	//--- 打断技能 指令预存
	bool	addSkillBreakCmd(uint32 uSkillID,uint8& uCode);
	//--- 执行预存指令
	void	processNextCmd	();

	void	clearSkillCmd	();
public:
	//--- 添加技能
	bool	addSkill		(uint32 uSkillID,uint8 uLevel,bool bTmp,bool bSend,_emDoingType );
	//--- 删除技能
	bool	delSkill		(uint32 uSkillID,bool bSend,_emDoingType emDoing);
	//--- 升级技能
	bool	upgradeSkill	(uint32 uSkillID,uint8 uLevel,bool bSend,_emDoingType );
	//--- 内部使用技能
	bool	useSkill		(uint32 uSkillID,uint8 uLevel,float32 fDirection,uint32 uTarUnitID,_stPoint2F&stTargetPoint);
	//--- 清除所有的技能CD
	void	clearSkillCD	(uint32 uSkillID);
public:
	static bool	send_UseSkill_Ack		(uint32 uSkillID,uint8 uLevel,uint8 uStage,uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8 uCode,bool bBroad,CSceneUnit* pOwner,_stPoint2F* pStartPoint = NULL);
	static bool	send_SkillBreak_Ack		(uint32 uSkillID,uint8 uStage,uint8 uCode,bool bBroad,CSceneUnit* pOwner);
	static bool	send_AddSkill_Ack		(uint32 uSkillID,uint8 uLevel,uint8 uCode,CSceneUnit* pOwner);
	static bool	send_DelSkill_Ack		(uint32 uSkillID,uint8 uCode,CSceneUnit* pOwner);
	static bool	send_UpgradeSkill_Ack	(uint32 uSkillID,uint8 uLevel,uint8 uCode,CSceneUnit* pOwner);
	static bool	send_AttackNotice_Ack	(CSceneUnit* pSource,CSceneUnit* pTarget,uint8 uNoticeType,bool bBroad = false);
	static bool	send_AttackDamage_Ack	(CSceneUnit* pSource,uint32 uSkillID,uint8 uCritical,int32 iDamage,bool bBroad,CSceneUnit* pOwner);
	static bool	send_SkillUseCharge_Ack	(uint32 uSkillID,uint8 uCode,bool bBroad,CSceneUnit* pOwner);
	
public:
	bool	process						(uint64 uTick);
protected:
	bool	trigerSkill					(const _stSkillModel* pSkillModel,uint32 uTarUnitID,uint32 uTrigerParam = 0);
	bool	tryTrigerSkill				(uint8 uSkillUseType,uint32 uTarUnitID,uint32 uTrigerParam = 0);

	void	sortSkill					();
public:
	void	callbackUseItem				(const _stItemModel* pItemModel);
	void	callbackUseSkill			(uint32 uTarUnitID);
	void	callbackBeSkillHit			(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	void	callbackBeKillHarm			(CSceneUnit* pUnit,int32& iDamage);
	void	callbackUpdateHP			();
	void	callbackSkillNotHit			();
	void	callbackSkillHit			();
};
