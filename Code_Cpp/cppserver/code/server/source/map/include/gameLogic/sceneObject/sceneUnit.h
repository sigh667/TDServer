/*------------- sceneUnit.h
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/3/23 13:07:42
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "def_struct/gameStruct.h"
#include "basic/basicFunctions.h"
#include "basic/systemFunction.h"
#include "basic/timeFunction.h"
#include "math/mathFunctions.h"
#include "math/point2.h"
#include "gameLogic/ghostObject/ghostObject.h"
#include "protocol/GameDefine.pb.h"
#include "def_struct/gameDefine.h"
#include "protocol/sceneStruct.pb.h"
#include "gameLogic/gameSkill/gameSkillMgr.h"
#include "gameLogic/gameBuff/gameBuffMgr.h"
#include "gameLogic/singing/singing.h"
#include "gameLogic/skilling/skilling.h"
#include "gameLogic/cooling/cooling.h"
#include "packet/clientPacket.h"
#include "stl/std_deque.h"
#include "def_struct/playerStuct.h"
#include "gameLogic/hatred/hatredMgr.h"
#include "gameLogic/gameHurt/gameHurtMgr.h"
#include <string>
/*************************************************************/
class CGameSceneHave;
class CGameSceneBlock;
class CGameSceneCell;
class CGameScene;
class CScenePlayer;
class CSceneMonster;
class CSceneItem;
class CScenePet;
class CSceneTrap;
class CSceneDartCar;
class CCopymapBase;
class PBGhostInfo;
class PBGhostMove;
class CSceneUnitAI;
struct _stGridRegionInfo;
struct _stRegionEvent;
struct _stGameRegion;
struct _stBattleAttrModel;
struct _stJobModel;
struct _stTeamInfo;
enum _emStateBuffAttr;
typedef stl_deque<CScenePlayer*>	QUEUE_PLAYER;
typedef stl_list<_stSkillAttack>	LIST_ATTACK;
typedef	stl_vector<PBEnergy>		VEC_ENERGY;

typedef stl_map<_emGameSystemType, _stGameAttribute>	MAP_ATTR;
typedef stl_map<uint8, _stEnergyRecovery>				MAP_ENERGYRECOVERY;
//-------------------------------------------------------------
//------------------------------ 
enum _emSetPositionReason
{
	SetPositionReason_Move,
	SetPositionReason_Skill,
	SetPositionReason_Build,
	SetPositionReason_AI,
	SetPositionReason_Gm,
	SetPositionReason_ChangePos,
	SetPositionReason_Qinggong,
};
//-------------------------------------------------------------
//------------------------------ 游戏对像
class CSceneUnit	: public CGhostObject
{
private:
	typedef				CGhostObject				super;
protected:
	//------------------------------ 位置相关
	_stPoint2F			m_stPosition;				//当前坐标点
	float32				m_fDirection;				//当前朝向
	//------------------------------ 移动相关
	_emMoveType			m_eMoveType;				//移动方式_emMoveType
	uint64				m_uBeginMoveTick;			//开始移动时间
	_stPoint2F			m_stBeginMovePosition;		//开始移动点
	float32				m_fMoveDirection;			//移动朝向
	float32				m_fMoveSpeed;				//移动速度
	uint32				m_uQinggongID;				//轻功寻路id
	uint64				m_uQinggongStartTick;		//执行轻功跳跃开始的时间


protected:
	//------------------------------ 游戏属性
	uint8				m_uJob;						//职业
	uint8				m_uCampID;					//当前阵营ID
	int32				m_iCurrHP;					//生命
	int32				m_iMaxHP;
	int32				m_uWalkSpeedAttr;
	int32				m_uRunSpeedAttr;
	uint32				m_uFightingEndTime;			//脱离战斗时间
	uint32				m_uFightingBeginTime;		//进入战斗时间
	MAP_ENERGY			m_mapEnergy;				//能量
	MAP_ENERGYRECOVERY	m_mapEnergyRecovery;		//能量恢复
	const _stJobModel*	m_pJobModel;				//职业
	bool				m_bAttributeRefreshBuff;	//Buff属性重算
	_stGameAttribute	m_stGameAttributeBase;		//基础属性
	_stGameAttribute	m_stGameAttributeBuff;		//Buff属性
	_stGameAttribute	m_stGameAttribute;			//基础属性-结果
	MAP_ATTR			m_mapSystemAttribute;		//游戏属性-系统
	uint32				m_uRouteID;					//寻路id
protected:
	//------------------------------ 游戏场景区域
	CGameSceneHave*		m_pSceneHave;
	CGameSceneBlock*	m_pSceneBlock;		//所在地图块
	CGameSceneCell*		m_pSceneCell;		//所在地图
	_stGridRegionInfo*	m_pRegion;			//所在游戏区域

protected:
	//------------------------------ 战斗相关
	bool				m_uStateFlag[_emGameStateType_ARRAYSIZE];	//行为状态
	bool				m_bUpdateValue[_UV_Max];	//更新计算数据
	CGameSkillMgr		m_clGameSkillMgr;		//技能管理
	CGameBuffMgr		m_clGameBuffMgr;		//Buff管理
	CSinging			m_clSinging;			//技能吟唱
	CSkilling			m_clSkilling;			//技能施法
	CCooling			m_clCooling;			//冷却管理
	uint32				m_uStateBuffAttrMark;	//状态buff的属性掩码

	CSceneUnitAI*		m_pUnitAI;			//ai
	CHatredMgr			m_clHatredMgr;		//仇恨管理器
	CGameHurtMgr		m_clhurtMgr;		//伤害管理器

private:
	LIST_ATTACK			m_listAttack;
	uint32				m_uOwnerID;			//主人的场景对象ID
	uint32				m_uOwnerPlayerID;	//主人的玩家ID
	std::string			m_strOwnerPlayerName;		//主人的名称
	uint64				m_uLastEnergyTick;	//上次更新能量的时间

//////临时变量
private:
	MAP_U16_U32			m_mapMark;			//标记 key:_emSceneUnitMark

public:
	bool				check_mark			(_emSceneUnitMark emMark,uint32 uValue = 0);
	void				update_mark			(_emSceneUnitMark emMark,uint32 uValue = 0);
	void				delete_mark			(_emSceneUnitMark emMark,uint32 uValue = 0);
	uint32				find_mark			(_emSceneUnitMark emMark);

public:
	inline int32		getHP				()const;
	inline int32		getHPMax			()const;
	inline void			setHP				(int32 iHP);
	bool				updateHP			(int32 iHP,CSceneUnit* pSource,_emDoingType emDoing);
public:
	bool				isDead				();
	void				enterFight			(_emEnterFightReason eReason);	// 进入战斗
	void				leaveFight			();								// 脱离战斗
	bool				isFighting			(); 
	uint32				getFightTime		();
public:
	inline void			initAttribute		();
	inline int32		getAttribute		(uint8 uType)const;
	inline void			setUpdateValue		(uint8 uType);
public:
	inline const _stPoint2F&getPosition		()const;
	bool				setPosition			(const float32 x,const float32 y,_emSetPositionReason eReason,bool bSyn);
	bool				setPosition			(const _stPoint2F&stPoint,_emSetPositionReason eReason,bool bSyn);
	inline float32		getDirection		()const;
	inline void			setDirection		(float32 uDir);
	inline void			setDirectionAndSync	(float32 uDir);
	inline _emMoveType	getMoveType			();
	inline void			setMoveType			(_emMoveType);
	inline bool			isMoving			()			{	return m_eMoveType != MoveType_Idle;	}
	inline uint32		getEnergy			(uint8 uType);
	inline uint8		getEnergyByExpend	(uint16 uType);
	inline float32		getDirection		()	{return m_fDirection ;}
	inline uint32		getRouteID			()	{return m_uRouteID;}
	inline void			setRouteID			(uint32 uRouteId) {m_uRouteID = uRouteId;}
	inline void			setQinggongID		(uint32 uQinggongID)	{m_uQinggongID = uQinggongID;}
	inline void			setQinggongStartTick(uint64 uTick)	{m_uQinggongStartTick = uTick;}

public:
	inline CGameSceneCell*		getSceneCell	()		{	return m_pSceneCell;	}
	inline CGameSceneBlock*		getSceneBlock	()		{	return m_pSceneBlock;	}
	inline _stGridRegionInfo*	getRegion		()		{	return m_pRegion;		}

public:
	inline CGameSkillMgr&	getGameSkillMgr		();
	inline CGameBuffMgr&	getGameBuffMgr		();
	inline CSinging&		getSinging			();
	inline CSkilling&		getSkilling			();
	inline CCooling&		getCooling			();
	inline CHatredMgr&		getHatredMgr		();
	inline CGameHurtMgr&	getHurtMgr			();
public:
	inline _stGameAttribute&getGameAttribute();
public:
	explicit CSceneUnit();
	virtual~CSceneUnit();

public:
	static uint32		registerObjectID	();
	_stGameAttribute*	registerAttribute	(_emGameSystemType systemType);
	_stGameAttribute*	getAttribute		(_emGameSystemType systemType);
	int32				getAttributeSingle	(_emGameSystemType systemType, uint8 uAttrType);
	bool				addAttribute		(_emGameSystemType systemType,uint16 uAttrType,int32 iValue,int32 iRadio,_emDoingType emDoing = DoingType_Others);
	virtual uint64		getExpend			(uint16 uExpendType);
	virtual bool		isExpendFull		(uint16 uExpendType);
	virtual bool		canExpend			(uint16 uExpendType,uint64 iValue);
	virtual bool		canExpend			(const MAP_U16_I64& mapExpend);
	virtual bool		addExpend			(uint16 uExpendType,int64 iValue,_emDoingType emDoing ,CSceneUnit* pSource = NULL);
	virtual bool		addExpend			(const MAP_U16_I64& mapExpend,_emDoingType emDoing ,CSceneUnit* pSource = NULL);
	bool				updateEnergy		(uint8 uType, int32 nValue,_emDoingType emDoing );
	void				registerEnergy		(uint8 uType, int32 nCurValue);
	uint32				getMaxEnergy		(uint8 uType);

public:
	uint16				getSceneID			();
	uint32				getSceneCellID		();
	uint64				getSceneDynID		();
	//--- 是否在视野内
	bool				isInViewRange		(CSceneUnit* pUnit,float32 iRadius);
public:
	//--- 注册对像
	virtual void		registerObject		(uint32 uUnitID = 0);
	//--- 注销对像
	virtual void		unregisterObject	();
	virtual bool		initialize			();
	virtual void		refreshAttribute	();
	virtual bool		onSetPoint			(bool bMove);
	virtual _stPoint2F	getBrithPoint		();
	virtual bool		insertGhostObject	(CSceneUnit*pObject,bool bRecursive = true);
	virtual void		gatherHaloVec		(VEC_U16& haloVec);
	virtual _stTeamInfo*getTeam				();
	virtual uint64		getTeamID			();
	virtual uint32		getFactionID		();
	virtual uint32		getUnionID			();
	virtual uint32		getCorpsID			();
public:
	virtual uint32		getID				()const	{	return 0;			}
	virtual uint8		getJob				()const	{	return m_uJob;		}
	virtual const _stJobModel*getJobModel	()const	{	return m_pJobModel;	}

	virtual uint32		getLevel			()const	{	return 0;			}
	virtual uint32		getMaxEnergy		(uint16 uType)const	{	return MAX_ENERGE_NUM;	}
	virtual uint8		getCampID			()const	{	return m_uCampID;			}
	virtual void		setCampID			(uint8 uCampID);
	virtual const std::string	getName		()const	{	return "好烦啊，没有名字";	}

	//--- 获得移动速度
	virtual float32		getMoveSpeed		(uint8 uMoveType)const;
	virtual int32		getMoveSpeedAttr	(uint8 uMoveType)const;
	virtual void		collectTempSkill	(MAP_U16_U8& mapSkill);
protected:
	//--- 属性计算相关
	static int32		getConverValue		(int32 iAttribute, uint8 uType, float32 fValue);
	void				attributeConvert	(_stGameAttribute& stAttrBase,_stGameAttribute& stAttr);
	void				calculateAttributeBase 	();
	void				calculateAttributeBuff 	();
	void				calculateAttribute		();


public:
	virtual uint32				getOwnerID			()	{	return m_uOwnerID;			}
	virtual uint32				getOwnerPlayerID	()	{	return m_uOwnerPlayerID;	}
	virtual const std::string&	getOwnerPlayerName	()	{	return m_strOwnerPlayerName;}

	virtual CSceneUnit*		getOwner		();
	virtual CScenePlayer*	getOwnerPlayer	();
	virtual	void			setOwner		(CSceneUnit* pOwner);
	virtual CScenePlayer*	asPlayer		()	{	return NULL;	}
	virtual CSceneMonster*	asMonster		()	{	return NULL;	}
	virtual CSceneItem*		asSceneItem		()	{	return NULL;	}
	virtual CScenePet*		asPet			()	{	return NULL;	}
	virtual CSceneTrap*		asTrap			()	{	return NULL;	}
	virtual CSceneDartCar*	asDartCar		()	{	return NULL;	}
	virtual uint32			getPlayerID		()	{	return 0;		}
	virtual uint32			GetPKValue		()	{	return 0;		}

public:
	virtual void		updateSceneBlock	(CGameSceneBlock*pBlock);
	virtual void		updateSceneHave		(CGameSceneHave*pHave);
	void				updateViewRange		();
	CGameScene*			getSceneMap			();
	CCopymapBase*		getGameCopymap		();
	virtual void		updateGhostList		(MAP_GAME_UNIT& _map);
	virtual bool		isVisible			(CSceneUnit*pObject);	// 目标对自己是否可见 
	virtual bool		isHide				();						// 自己是否是隐身的 
	virtual	void		refreshVisible		(bool bHide = true);
public:
	CScenePlayer*		findCellPlayer		(uint32 uID);
	CSceneUnit*			findCellObject		(uint32 uID);
	CSceneUnit*			findRangeObject		(uint32 uID);
	void				gatherSceneObject	(const _stPoint2F&stPoint,float32 uRadius,MAP_GAME_UNIT& _map,uint32 uTypeMark,bool bClear = true);
	void				gatherSceneObject	(const _stPoint2F&stPoint,float32 uRadius,VECTOR_RANGE_UNIT& vec,uint32 uTypeMark);
	bool				getUnitTargetPoint	(float32 fdir,float32 fdis,_stPoint2F& target);
	bool				getUnitTargetPoint	(const _stPoint2F& target,_stPoint2F& outTarget);
	void				setNewPosition		(CSceneUnit* pSource,float32 fDis,uint8 uDirType);

public:
	void				gatherObjectPoint	(const _stPoint2F&stPoint,MAP_GAME_UNIT&mapObject,VECTOR_GAME_UNIT& vec);
	void				gatherObjectLine	(const _stPoint2F&stPoint,float32 fDirection,float32 uLineLength,float32 uLineWidth,MAP_GAME_UNIT&mapObject,MAP_GAME_UNIT& mapUseObject,bool bOnlyMove = false);
	void				gatherObjectArc		(const _stPoint2F&stPoint,float32 fDir,float32 uArcRadius,float32 uArcAngle,MAP_GAME_UNIT&mapObject,MAP_GAME_UNIT& mapUseObject,bool bOnlyMove = false);
	void				gatherObjectRound	(const _stPoint2F&stPoint,float32 uRoundRadius,MAP_GAME_UNIT&mapObject,MAP_GAME_UNIT& mapUseObject,bool bOnlyMove = false);

public:
	virtual void		gatherBroadcast		(QUEUE_PLAYER&vectorPacket);
	virtual void		sendBroadcast		(uint8 uMainProtocol,uint8 uSubProtocol,const google::protobuf::Message* pMsg,uint8 uEventID,bool bIgnore = false);

public:
	virtual	bool		canUpdateHp			(int32 iHP);
	virtual	bool		canUseSkill			(CSceneUnit*pTarget);
	virtual	bool		canByUseSkill		(CSceneUnit*pAtker);
	//获取关系
	virtual uint8		getRelation			(CSceneUnit* pUnit);
	virtual bool		byDamage			(CSceneUnit*pUnit,int32 iDamage,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,uint32 uCritical,uint32 uDelayTime = 0);
	bool				byAttack			(CSceneUnit* pOwner,const _stPoint2I&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,uint32 uDelayTick);
	void				processAttack		(uint64 uTick);
	int32				getSkillAttribute	(uint16 uAttrType,const _stSkillEffectModel*pSkillEffectModel);
	virtual bool		isAttackHit			(CSceneUnit* pTarget,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual uint32		getCritcal			(CSceneUnit* pTarget,const _stSkillEffectModel*pSkillEffectModel, const _stSkillModel* pSkill);
	void				energyUseSkill		(const _stSkillModel* pSkill);
	void				energyByAttack		();
	void				energyByHitEnermy	(const _stSkillEffectModel* pSkillEffectModel);
	virtual uint32		getFlyTotalTick		() {		return 0;	};
	virtual uint32		getBakFlayTick		() {		return 0;	};
	virtual float32		getModelRadius		() {		return 0;	};
	virtual void		checkRefreshKillTime(CSceneUnit* pBeAtkUnit);
	float32				getSkillModelRadius	() {		return getModelRadius() * 0.9f;	};

	void				refreshStateBuffAttr();
	bool				checkStateBuffAttr	(_emStateBuffAttr eAttr);
	void				refreshBuffAttr();

	void				breakUseSkill		();
public:
	virtual void	enterScene			(CGameSceneCell*pCell);
	virtual void	leaveScene			();
	virtual void	sceneHave			(bool _increase);
	virtual void	process				(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
	virtual void	process_move		(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
	virtual void	processAI			(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
	virtual void	processFight		(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
	virtual void	processLogic		(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
	virtual void	processUpdateValue	(uint64 uTime);

	void			processEnergy		(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);

	virtual bool	canRunAI			();
	//--- 更新区域
	virtual void	updateRange			(bool bMove);
	virtual bool	packetGhost			(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo);

	virtual void	speak				(uint32 uSpeakID);

	bool			isInCopymap			();

	CSceneUnitAI*	getAI				(){ return m_pUnitAI;}
public:
	bool			canMove				();
	bool			canDirection		();

	bool			beginMove			(uint8 uMoveType,float32 fPosX,float32 fPosY,float32 fDir);
	bool			endMove				(float32 fPosX,float32 fPosY,float32 fDir);
	bool			beginMove			(uint8 uMoveType,float32 fDir);
	bool			endMove				();
	bool			changeDiretion		(float32 fPosX,float32 fPosY,float32 fDir);

	bool			packetMove			(PBGhostMove& move);

	void			setMoveState		(uint8 uMoveType,float32 fPosX,float32 fPosY,float32 fDir);

	void			breakMove			();
private:
	void			setState			(uint16 uStateType,bool bFlag);

	bool			checkMove			(float32 fPosX,float32 fPosY,float32 fDir);
public:
	//--- 状态-----------------------
	virtual bool	haveState			(_emGameStateType uStateType);
	virtual bool	delState			(_emGameStateType uStateType,bool bInterupt = false );
	virtual bool	addState			(_emGameStateType uStateType);

protected:
	virtual void	onEnterState		(_emGameStateType uStateType);
	virtual void	onLeaveState		(_emGameStateType uStateType,bool bInterupt);
public:
	virtual	bool	testRegion				(uint8 uAttrType)const;
	virtual void	triggerRegionEvent		(_emRegionTriggerType uTriggerType,uint32 uNotTrigger = 0);
	virtual	bool	isInRegion				(uint16 uRegionID);
protected:
	virtual void	triggerGridEvent		(_stGridRegionInfo* pGridRegion,_emRegionTriggerType uTriggerType,uint32 uNotTrigger);
	virtual void	triggerGameEvent		(const _stGameRegion* pGameRegion,_emRegionTriggerType uTriggerType,uint32 uNotTrigger,uint32& uTriggeredEventMark);
	virtual bool	canTriggerEvent			(const _stRegionEvent*pEvent);
	virtual bool	triggerEvent			(const _stRegionEvent*pEvent);

	virtual	void	onEnterRegion			(const _stGameRegion* pGameRegion);
	virtual	void	onLeaveRegion			(const _stGameRegion* pGameRegion);

	/*************************************************************/
	/********************回调函数*************************************/
public:
	virtual void	callbackHurt			(CSceneUnit* pUnit,int32 iHurt);
	virtual void	callbackBeKillHarm		(CSceneUnit* pUnit,int32& iDamage);
	virtual void	callbackAttack			(CSceneUnit* pUnit,int32 iDamage,bool bCritical);
	virtual void	callbackByAttack		(CSceneUnit* pUnit,int32 iDamage);
	virtual void	callbackCritical		();
	virtual void	callbackUseSkill		(const _stSkillModel*pSkillModel,uint32 uTarUnitID);
	virtual void	callbackUseSkillEnd		(const _stSkillModel*pSkillModel);
	virtual void	callbackSkillHit		(const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual void	callbackSkillNotHit		(const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual void	callbackBeSkillEffect	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual void	callbackSkillHitTarget	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual void	callbackDead			(CSceneUnit* pSource);
	virtual void	callbackKillTarget		(CSceneUnit* pTarget);
	virtual void	callbackLife			();
	virtual void	callbackUpdateHP		(int32 iHP);
	virtual	void	callbackFight			();
	virtual	void	callbackFightPower		() {}
	virtual	void	callbackUseSkillFailedByBlock		(uint32 uSkillID) ;  //因为阻挡点的问题，导致技能使用失败
	virtual void	callbackChangeExpend	(uint16 uExpendType,int32 iValue);
	virtual void	callbackPostionChange	(){};//位置移动,
};
/*************************************************************/
#include "sceneUnit_inline.h"
