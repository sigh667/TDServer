/*----------------- sceneMonster.h
*
* Copyright (C): 2011  
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/9/15 11:45:18
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "sceneUnit.h"
#include "gameLogic/sceneManage/sceneMonsterMgr.h"
/*************************************************************/
struct _stMonsterModel;
struct _stMonsterRegion;
class CSceneMonster	: public CSceneUnit
{
public:
	typedef CSceneUnit Parent;
private:
	const _stMonsterModel*	m_pMonsterModel;
	uint64					m_uBuildTick;		// ����ʱ��
	uint64					m_uDeadTick;		// ����ʱ�� �㵹��ʱ�����
	uint32					m_uDeadTime;		// ����ʱ��
	uint64					m_uSuperArmor;		// ����ֵ
	uint64					m_uSuperArmorTick;	// ����ָ�ʱ��
	SET_U32					m_setBirthAddBuff;	//�����ӹ���buff
	SET_U32					m_setFightAddBuff;	//����ս���ӹ���buff
	uint64					m_uLastFightTick;	// �ϴ�ս��ʱ��
	uint64					m_uLastSearchTime;	// �ϴβ�ѯʱ��
	uint64					m_uLastLiangyiTime;	// �ϴβ�ѯʱ��
	uint32					m_uParam1;			//	����1  ���ǹֱ�ʾ���� ���������Ҫ�������Ƹ��Ի�
	uint32					m_uParam2;			//	����2  ���ǹֱ�ʾ�Ƿ񼤻�  ʯ���ֱ�ʾ�Ƿ񼤻�
	CScenePlayer*			m_pActivePlayer;		// ���ڼ�������

	uint32					m_uWorldBossActivityId;	//����boss�Ļ���
	bool					m_bActivityFreezAI;		//�����
	_stMonsterRegion*		m_pMonsterRegion;		// ˢ��������Ϣ

public:
	CSceneMonster();
	virtual~CSceneMonster();
	uint8			getMonsterType	();
	uint32			getMonsterID	();
	inline	uint64	getSuperArmor	()				{	return m_uSuperArmor;	}
	inline	uint32	getDeadTime		()				{	return m_uDeadTime;		}
	inline	void	setParam1		(uint32 uParam)	{	m_uParam1 = uParam;		}
	inline	void	setParam2		(uint32 uParam)	{	m_uParam2 = uParam;		}
	inline	uint32	getParam1		()				{	return m_uParam1;		}
	inline	uint32	getParam2		()				{	return m_uParam2;		}
	inline	CScenePlayer*	getActivePlayer		()	{	return m_pActivePlayer;	}
	inline	uint64	getBuildTick		()			{	return m_uBuildTick;	}
	inline  _stMonsterRegion* getMonsterRegion	()	{   return m_pMonsterRegion;}

public:
	virtual CSceneMonster*	asMonster		()	{	return this;	}
	virtual const std::string	getName()const	{	return "����";	}
	virtual	bool			packetGhost		(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo);

public:
	virtual bool			initialize		();
	/*--->[ ���� ]*/
	virtual bool			build			(const _stMonsterModel*pModel,_stMonsterRegion * pMonsterRegion,float32 fPointX,float32 fPointY,float32 fDir,uint32 uRouteID=0,uint16 uLevel=0);
public:
	virtual void			processLogic	(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);

	virtual void			enterScene		(CGameSceneCell*pCell);
	virtual void			leaveScene		();

	virtual bool			canRunAI		();

	virtual	bool			canUseSkill		(CSceneUnit*pTarget);
	virtual	bool			canByUseSkill	(CSceneUnit*pAtker);

	virtual	bool			canUpdateHp		(int32 iHP);

	virtual uint32			getFlyTotalTick	();
	virtual uint32			getBakFlayTick	();
	virtual float32			getModelRadius	();

	virtual bool			isVisible		(CSceneUnit*pObject);	// Ŀ����Լ��Ƿ�ɼ� 

	virtual void			callbackDead	(CSceneUnit* pSource);
	virtual void			callbackUpdateHP(int32 iHP);
	virtual	void			callbackFight	();

	virtual void			callbackHurt	(CSceneUnit* pUnit,int32 iHurt);

	virtual void			speak			(uint32 uSpeakID);
public:
	void					exitScene		();
	float32					getCallDist		();
	bool					isQuestLimit	(CScenePlayer* pPlayer);
	void					questComplete	();
	void					setActivePlayer	(CScenePlayer* pPlayer);
public:
	inline const _stMonsterModel*	getMonsterModel	()const	{	return m_pMonsterModel;		}
	void					updateAttrByRatio(uint8 uType,uint32 uAttrRatio, std::set<int32>& setNeedChange);
protected:
	void					playerExp		(CScenePlayer* pPlayer, uint32 iValue);
	void					superArmorTick	();
	void					processBirthTick(uint64 uTick);
	void					processFightTick(uint64 uTick);

public:
	inline void				setWorldBossId(uint32 uWorldBossId){m_uWorldBossActivityId = uWorldBossId;};
	inline uint32			getWorldBossId(){return m_uWorldBossActivityId;};

	inline void				setActivityFreezeAI(bool bFreeze){ m_bActivityFreezAI = bFreeze;}
	inline bool				getActivityFreezeAI(){ return m_bActivityFreezAI ;}
};
