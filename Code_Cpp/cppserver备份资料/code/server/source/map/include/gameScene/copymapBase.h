/*----------------- copymapBase.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/5/11 18:18:00
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_set.h"
#include "stl/std_map.h"
#include "cfg_data/copymapData/copymapData.h"
#include "google/protobuf/message.h"
#include "def_struct/playerStuct.h"
#include "protocol/WAGMsg.pb.h"
#include "gameLogic/sceneObject/sceneItem.h"
/*************************************************************/
class CScenePlayer;
class CSceneUnit;
class CSceneTrap;
class CSceneMonster;
class CGameSceneCell;
class PBW2GCopymapPlayerInfo;
class PBPvpBannerPlayerInfo;
class CCopymapBase
{
public:
	struct _stCopymapPlayer
	{
		_stPlayerDisplay		stDisplay;				// 玩家信息（玩家存在取玩家数据）
		_emCopymapEnterReason	emEnterReason;			// 进入副本原因
		_em_CopymapPlayerState	emCopymapPlayerState;	//状态
		bool					bDead;					//是否死亡 默认存活
		CScenePlayer*			pPlayer;				//玩家指针
		bool					bAlreadyOnline;			//是否第一次进入
		uint64					uHurtValue;				//伤害数值
		uint32					uKillCount;				//击杀玩家个数
		uint32					uContinueKillCount;		//连续击杀玩家个数	算超神
		uint32					uLimitTimeKillCount;	//限时击杀玩家个数 算暴走
		uint32					uLastKillTime;			//上次击杀玩家时间
		uint32					uDeadCount;				//死亡次数
		uint32					uAssistKillCount;		//助攻次数
		uint32					uCapturenum;			//拔旗次数
		uint32					uContri;				//贡献值
		uint32					uContriTime;			//增加贡献值的时间 计算mvp用
		uint32					uGodlikeCount;			//超神次数
		uint32					uMegakillCount;			//暴走次数
		uint32					uMvpCount;				//mvp次数
		bool					bNoPrize;				//是否无奖励
		uint8					uOldCampID;				//之前的阵营ID
		uint8					uOldPkType;				//之前的pk模式

		_emCopymapCompleteType	emCopymapCompleteType;	//完成类型

		void	initialize()
		{
			dMemset(this,0,sizeof(_stCopymapPlayer));
			emCopymapCompleteType = CopymapCompleteType_None;
		}
		void	saveToProtobuf		(PBPvpBannerPlayerInfo* pbData);
	};
	typedef	stl_map<uint32,_stCopymapPlayer>	MAP_PLAYER;
protected:
	MAP_PLAYER				m_mapCopymapPlayer;		//副本玩家信息
	uint8					m_eCopymapState;		//副本状态 _emCopymapState
	uint32					m_uStartTime;			//副本启动时间 UNIX时间
	uint32					m_uRefreshWaveTime;		//副本启动时间 UNIX时间
	uint16					m_uCurWaveID;			//当前波数
	uint32					m_uCloseTime;			//副本关闭时间 UNIX时间
	uint32					m_uCompleteTime;		//副本完成用时（秒）
	_emCopymapCompleteType	m_emCopymapCompleteType;//副本完成类型 _emCopymapCompleteType
	uint32					m_uExtraNum;			//额外参数 缉拿囚车个数,
	uint32					m_uCreateNpc;			//哪个npc附近创建的，用于怪物创建基准点
	uint16					m_uCreateLevel;			//创建的基准等级 用于围场狩猎
	uint32					m_uReadyStartTime;		//准备开始的时间（时间内才通知）
	PBPlayerDisplay			m_pbCreator;			//创建副本的玩家
	uint32					m_uCash;				//赏金数量
	bool					m_bCashCopyMap;			//是否是赏金副本 
	stl_set<CSceneItem*>	m_setDropUnit;			// 掉落物品

protected:
	CGameSceneCell*			m_pSceneCell;
	const _stCopymapModel*	m_pCopymapModel;
public:
	inline const _stCopymapModel*	getCopymapModel		()const		{	return m_pCopymapModel;												}
	inline uint16					getCopymapID		()const		{	return (m_pCopymapModel ? m_pCopymapModel->uCopymapID : 0);			}
	inline uint32					getCopymapType		()const		{	return (m_pCopymapModel ? m_pCopymapModel->uType:0);				}
	inline uint8					getCopymapMainType	()const		{	return (m_pCopymapModel ? m_pCopymapModel->uMainType:0);			}
	inline bool						canRelive			()const		{	return (m_pCopymapModel ? m_pCopymapModel->bCantRelive: true);		}
	inline bool						canOperate			()const		{	return (m_pCopymapModel && m_pSceneCell);							}
	bool							isLastWave			();
	inline uint32					getCreateNpc		()const		{	return m_uCreateNpc;												}
	inline _emCopymapState			getCopymapState		()const		{	return (_emCopymapState)m_eCopymapState;							}
	inline _stCopymapPlayer*		getCopymapPlayer	(uint32 uPlayerID)	{	return m_mapCopymapPlayer.find_(uPlayerID);					}
	inline uint32					getCashPlayerID		()			{	return m_bCashCopyMap ? m_pbCreator.playerid() : 0;						}
public:
	CCopymapBase();
	virtual~CCopymapBase();
public:
	virtual void			initialize			();
	virtual void			restart				(CGameSceneCell*pScene,const PBCreateCellParam&pbParam);
	virtual	void			close				();
	virtual	bool			isPrefect			();
	virtual	uint32			getExtraNum			() { return 0; }
	//--- 副本完成
	virtual	void			on_copymap_complete	(uint32 uTime,_emCopymapCompleteReson emReason);
	// 设置关闭时间
	virtual void			set_closetime		(uint32 uTime);
	// 开始刷新波次
	virtual void			begin_wave			();

	void					broadCastMsg		(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	void					broadCastBuffer		(uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);

	void					updatePlayerInfo	(PBW2GCopymapPlayerInfo&msg);
	_emCopymapEnterReason	getEnterType		(uint32 uPlayerID) const;
	uint32					getDeadNum			();
	bool					isAllDead			();
	bool					isAllLife			();
	bool					isAllExit			();
	bool					isHavePlayerExit	();
	bool					refreshUnit			(const _stUnitGroupModel* pGroupModel);
	bool					synWaveInfo			();
	bool					transToBirthRegion	();
	CScenePlayer*			getTeamLeader		();
	void					callback_callAllPlayer(CSceneMonster* pMonster);

	void					callback_AICloseCopymap(uint32 uTime, uint32 uChatid);
	void					clearMonsterArea(CSceneMonster* pMonster);

	void					AddSceneItem(CSceneItem *pSceneItem);

public:
	//--- 角色进入场景
	virtual void			playerEnter			(CScenePlayer*pPlayer);
	//--- 角色重连
	virtual void			playerReconnect		(CScenePlayer*pPlayer);
	//--- 角色离开场景
	virtual void			playerLeave			(CScenePlayer*pPlayer);
	//--- 角色上线处理(初始完场景同步)
	virtual void			playerOnline		(CScenePlayer*pPlayer);
	virtual	void			onFirstOnline		(CScenePlayer*pPlayer);
	void					onClientGo			(CScenePlayer*pPlayer);
public:
	//--- 副本推进
	virtual void			process				(uint64 uTick,uint64 uTime,uint32 uSecond);
protected:
	// 进行推进
	virtual void			process_ing			(uint64 uTick,uint32 uTime,uint32 uSecond);
	// 完成后推进
	virtual void			process_done		(uint64 uTick,uint32 uTime,uint32 uSecond);

	void					synState2World		();

protected:
	//--- 检测副本完成
	virtual	void			check_complete		(uint32 uTime);
	//--- 检测副本关闭
	virtual void			check_closeing		(uint32 uTime);
	//--- 检测副本刷新波数
	virtual void			check_refreshWave	(uint32 uTime);

public:
	virtual	void			callback_killmonster	(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	virtual	void			callback_killplayer		(CScenePlayer* pSource,CScenePlayer* pTarget);
	virtual void			callback_depotGoods		(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	virtual	void			callback_arrestmonster	(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	virtual	void			callback_thugmonster	(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	virtual	bool			unactivemonster			(CSceneMonster* pMonster,CScenePlayer* pPlayer,uint8& uCode);
	virtual	void			callback_playerlife		(CScenePlayer* pPlayer,CScenePlayer* pSource);
	virtual	void			callback_playerdead		(CScenePlayer* pPlayer,CSceneUnit* pSourceUnit);
	virtual	void			callback_npcdialog_complete	(CScenePlayer* pPlayer,CSceneMonster* pNpc);
	virtual	void			callback_createmonster	(CSceneMonster* pMonster);
	virtual	void			callback_newtrap		(CSceneTrap* pTrap) {}
	virtual	void			callback_monsterhurt	(CSceneMonster* pMonster,CSceneUnit* pSourceUnit,int32 iHurt);
	virtual	void			callback_flyout			(CScenePlayer* pPlayer);
	virtual void			callback_trapLeave		(CSceneTrap*pTrap);
	virtual	void			callback_player_gethurt	(CScenePlayer* pPlayer, uint32 uHurt, CScenePlayer* pPlayerSource);//被攻击导致承受伤害

private:
	void					giveSkill			(CScenePlayer* pPlayer,bool bAdd,_emDoingType emDoing);
	void					startCopymap		();	

	// 赏金团分钱
	void					giveCach			();
	void					sceneItemSendMail		();	

};
