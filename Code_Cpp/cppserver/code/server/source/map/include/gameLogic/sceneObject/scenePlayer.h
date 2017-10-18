/*----------------- scenePlayer.h
*
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/15 11:04:57
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "sceneUnit.h"
#include "protocol/PlayerStruct.pb.h"
#include "protocol/AccountStruct.pb.h"
#include "network/netStruct.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/GameDefine.pb.h"
#include "def_struct/playerStuct.h"
#include "gameLogic/gameItem/gameItemManager.h"
//#include "gameLogic/playerStore/playerStore.h"
/*************************************************************/
/*************************************************************/
class CClientPacket;
class CGlobalPacket;
class CWorldPacket;
class CNetStream;
class CGamePlayerFun;
//class CPlayerCounter;
class CPlayerCommon;
class CPlayerSystemSwitch;
//class CPlayerColletion;
//class CPlayerPrize;
class CPlayerQuestMgr;
//class CPlayerWulue;
//class CPlayerIdentity;
//class CPlayerTeam;
class CPlayerCopymap;
//class CPlayerWuxue;
//class CPlayerDart;
//class CPlayerIdentity;
//class CPlayerPvpWar;
//class CPlayerTitle;
class CPlayerEquipColumn;
//class CPlayerClimbTower;
//class CPlayerGreatWallTrain;
//class CPlayerActivity;
class CPlayerRelive;
//class CPlayerWenTao;
class PBMoneyConsume;
//class CPlayerWorldBoss;
//class CPlayerCorps;
//class CPlayerWeal;
//class CPlayerSinglePvp;
class CPlayerMail;
struct _stPlayerModel;
struct _stPromotionModel;
struct _stTeamInfo;
class PBC2GCommon_PlaneLeave;
//class CPlayerManor;

class CScenePlayer	:	public CSceneUnit
{
public:
	typedef CSceneUnit Parent;
	typedef stl_map<CScenePlayer*,uint64> MAP_ENERMYLIST;
	friend	class CGameLogin;
	friend	class CGamePlayerMgr;
private:
	uint32						m_uSaveOrder[_emPlayerType_ARRAYSIZE];
private:
	PBAccount					m_pbAccount;
	_stPlayer					m_stPlayerBase;
	bool						m_bBuild;				//是否构建
	uint64						m_uBuildTime;			//登录构建时间
	uint64						m_uSaveNextTime;		//下次数据保存时间
	uint64						m_uSynGhostTime;		// ghost时间
	uint8						m_uLoginState;			//当前装态
	uint8						m_uCopymapState;		//副本状态
	bool						m_bUpdateParantSvr;		//更新上层服务器的缓存数据
	bool						m_bTransfer;			//是否在传送中
	const _stPlayerModel*		m_pPlayerLevelModel;	//玩家的等级模板
	const _stPromotionModel*	m_pPromotionModel;		//玩家的晋升模板
	uint64						m_uOfflineTime;			//玩家下线时间 五分钟后才真正剔出
	uint64						m_uAutoReliveTime;		//自动复活时间
	uint32						m_uLastActionTime;		//上次活跃时间 移动/主动放技能

private:
	CClientPacket*			m_pPacket;

private:
	MAP_ENERMYLIST	m_mapEnermyList;		//敌人列表
	MAP_ENERMYLIST	m_mapSelfEnermyList;	//自己所在的敌人列表

private:
	CPlayerCommon*			m_pPlayerCommon;		//通用
	//CPlayerCounter*			m_pPlayerCounter;		//计数器
	//CPlayerTeam*			m_pPlayerTeam;			//队伍
	//CPlayerPrize*			m_pPlayerPrize;			//奖励系统
	CGameItemMgr			m_GameItemMgr;			//道具管理
	CPlayerQuestMgr*		m_pPlayerQuestMgr;		//任务管理器
	//CPlayerStore*			m_pPlayerStore;			//商店管理
	//CPlayerWulue*			m_pPlayerWulue;			//武略管理器
	CPlayerSystemSwitch*	m_pPlayerSystemSwitch;	//系统功能开关
	//CPlayerColletion*		m_pPlayerColleton;		//采集功能
	//CPlayerIdentity*		m_pPlayerIdentity;		//身份系统
	//CPlayerWuxue*			m_pPlayerWuxue;			//武学系统
	CPlayerCopymap*			m_pPlayerCopymap;		//副本系统
	//CPlayerDart*			m_pPlayerDart;			//押镖系统
	//CPlayerTitle*			m_pPlayerTitle;			//称号系统
	CPlayerEquipColumn*		m_pPlayerEquipColumn;	//装备栏强化打磨
	//CPlayerClimbTower*		m_pPlayerClimbTower;	//个人爬塔玩法,兵器谱
	//CPlayerPvpWar*			m_pPlayerPvpWar;		//边塞秋演兵(战场5v5)
	//CPlayerCorps*			m_pPlayerCorps;			//军团
	//CPlayerGreatWallTrain*	m_pPlayerGreatWallTrain;//长城试炼
	//CPlayerWorldBoss*		m_pPlayerWorldBoss;		//世界野外boss
	//CPlayerActivity*		m_pPlayerActivity;		//活动
	//CPlayerWenTao*			m_pPlayerWentao;		//文韬
	CPlayerRelive*			m_pPlayerRelive;		//复活
	//CPlayerWeal*			m_pPlayerWeal;			//福利
	//CPlayerManor*			m_pPlayerManor;			//领地
	//CPlayerSinglePvp*		m_pPlayerSinglePvp;		//1v1论剑
	CPlayerMail*			m_pPlayerMail;			//邮件

private:
	CGamePlayerFun*			m_arrNetHandler[_emPC2S_Protocol_ARRAYSIZE];
	CGamePlayerFun*			m_arrPlayerFun[_emGameSystemType_ARRAYSIZE];
public:
	CScenePlayer();
	~CScenePlayer();
private:
	void	registerSystem	(CGamePlayerFun* pPlayerFun,int32 uMainProctocal = -1 );
public:
	bool	initialize		();
	void	refreshAttribute();
	void	newOnline		();
	bool	logout			();//真正离线
	bool	logoutLeave		();
	void	logoutScene		();
	void	sendPlayer		();
public:
	virtual CScenePlayer*	asPlayer		()	{	return this;							}
	virtual uint32			getPlayerID		()	{	return m_stPlayerBase.uPlayerID;		}
	virtual void	processLogic			(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
	virtual	void	processUpdateValue		(uint64 uTime);
	virtual bool	triggerEvent			(const _stRegionEvent*pEvent);
	virtual	void	onEnterRegion			(const _stGameRegion* pGameRegion);
	virtual	void	onLeaveRegion			(const _stGameRegion* pGameRegion);
	virtual void	process_move			(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
	uint16	testRegionReturnId				(uint8 uAttrType)const;

public:
	// 给客户端发送消息
	bool					isClientConneted	();
	bool					sendMsg				(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	bool					sendBuffer			(uint8 uMainProtocol, uint8 uProtocol,void* pBuffer,uint32 uSize, uint8 uEvent = 0);
	bool					sendErrorMsg		(uint8 uMainProtocol, uint8 uProtocol, uint8 uEventFlag,const char* pParam1,const char* pParam2);

	// 给全局服务器发送消息
	bool					isGlobalConneted	();
	bool					sendMsgToGlobal		(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	bool					sendBufferToGlobal	(uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);

	// 给世界服务器发送消息
	bool					isWorldConneted		();
	bool					sendMsgToWorld		(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	bool					sendBufferToWorld	(uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);
	bool					sendPlayerMsgToWorld(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg = NULL,  uint8 uEvent = 0);
	bool					sendMsgToBigWorld	(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);

public:
	inline uint32			getPlayerID()const				{	return m_stPlayerBase.uPlayerID;					}
	virtual const std::string getName()	const				{	return m_stPlayerBase.szPlayerName;					}
	const std::string getAnsiName()	const;
	inline uint64			getLoginSN()const				{	return m_pbAccount.accountflag().loginsn();			}
	inline void				setClientPacket			(CClientPacket*pPacket);
	inline uint64			getBuildTime			()const	{	return m_uBuildTime;								}
	inline	uint16			getLogicWorldID			()const	{	return m_stPlayerBase.uLogicWorldID;				}
	inline _stPlayerBase&	getPlayerBase			()		{	return m_stPlayerBase;								}
	inline _stPlayer&		getPlayerST				()		{	return m_stPlayerBase;								}
	inline bool				testPlayerMark			(uint8 uMark)const;
	inline void				setPlayerMark			(uint8 uMark,bool bSet);
	void					SetGLoginState			(uint8 uState);
	inline uint32			GetPKValue				()		{	return m_stPlayerBase.uPKValue;						}
	inline uint8			GetPKType				()		{	return m_stPlayerBase.uPKType;						}
	inline uint32&			getItemSequence			()		{	return m_stPlayerBase.uItemSequence;				}
	inline void				setCopymapState			(uint8 uCopymapState)	{	m_uCopymapState = uCopymapState;	}
	inline uint8			getCopymapState			()const					{	return m_uCopymapState;				}
	inline void				setTransfer				(bool bSet)				{	m_bTransfer = bSet;					}
	inline bool				isTransfer				()						{	return m_bTransfer;					}
	inline const _stPlayerModel*	getLevelModel	()const					{	return m_pPlayerLevelModel;			}
	inline const _stPromotionModel*	getPromotionModel()const				{	return m_pPromotionModel;			}
	uint8					getPromotionStep		()const;
	inline uint64			getOfflineTime			()						{	return m_uOfflineTime;				}
	inline void				setOfflineTime			(uint64 uOfflineTime)	{	m_uOfflineTime = uOfflineTime;		}
	virtual uint32			getLevel				()const					{	return m_stPlayerBase.uLevel;		}
	virtual uint64			getExp					()const					{	return m_stPlayerBase.uExp;			}
	virtual uint32			getMaxEnergy			(uint16 uType)const;
	uint32					getVipLevel				()const					{	return 0;							}
	uint32					getEquipLucky			()						{	return m_stPlayerBase.uEquipLucky;	}
	bool					addEquipLucky			(int32 nAddValue,_emDoingType emDoing );
	inline	uint32			getMountID				()						{	return m_stPlayerBase.uMountID;		}
	inline	void			setMountID				(uint32 uMountID)		{	m_stPlayerBase.uMountID = uMountID;	}
	inline	uint64			getWisdom				()						{	return m_stPlayerBase.uWisdom;		}
	bool					addWisdom				(int64 nAddValue,_emDoingType emDoing );
	inline	uint64			getSufferValue			()						{	return m_stPlayerBase.uSufferValue;	}
	bool					addSufferValue			(int64 nAddValue,_emDoingType emDoing );
	inline	uint32			getFightPower			()						{	return m_stPlayerBase.uFightPower;	}
	inline	uint32			getMaxFightPower		()						{	return m_stPlayerBase.uMaxFightPower;}
	uint32					getDayCopymapCount		(uint8 uCopymapMainType);	
	void					addDayCopymapCount		(uint8 uCopymapMainType,uint32 uCount = 1);
	uint32					getDayTeamTaskCount		(uint32 uID);	
	void					addDayTeamTaskCount		(uint32 uID,uint32 uCount = 1,bool bSend = true);
	void					setUpdateParantSvr		()						{	m_bUpdateParantSvr = true;			}
	uint32					getTitleID				()const					{	return m_stPlayerBase.uTitleID;		}
	void					setTitleID				(uint32 uTitleID);
	uint64					getContribution			();
	bool					addContribution			(int32 nAddValue,_emDoingType emDoing );
	inline uint32			getLastActionTime		()						{	return m_uLastActionTime;			}
	uint32					getMakeVigour			();	
	bool					addMakeVigour			(int32 nAddValue,_emDoingType emDoing);
	inline uint32			getCorpsJob				()						{	return m_stPlayerBase.uCorpsJob;	}
	void					setCorpsJob				(uint32 uJob);
	virtual void			setCampID				(uint8 uCampID);

public:
	inline CPlayerQuestMgr*	getPlayerQuestMgr		()		{	return 	m_pPlayerQuestMgr;							}
	//inline CPlayerCounter*	getPlayerCounter		()		{	return 	m_pPlayerCounter;							}
	//inline CPlayerTeam*		getPlayerTeam			()		{	return 	m_pPlayerTeam;								}
	inline CPlayerCopymap*	getPlayerCopymap		()		{	return 	m_pPlayerCopymap;							}
	//inline CPlayerPrize*	getPlayerPrize			()		{	return 	m_pPlayerPrize;								}
	inline CGameItemMgr&	getGameItemMgr			()		{	return 	m_GameItemMgr;								}
	//inline CPlayerStore*	getPlayerStore			()		{	return 	m_pPlayerStore;								}
	//inline CPlayerWulue*	getPlayerWulue			()		{	return 	m_pPlayerWulue;								}
	inline CPlayerSystemSwitch*getPlayerSystemSwitch()		{	return 	m_pPlayerSystemSwitch;						}
	//inline CPlayerColletion*	getPlayerColletion	()		{	return 	m_pPlayerColleton;							}
	//inline CPlayerWuxue*	getPlayerWuxue			()		{	return 	m_pPlayerWuxue;								}
	inline CPlayerCommon*	getPlayerCommon			()		{	return 	m_pPlayerCommon;							}
	//inline CPlayerDart*		getPlayerDart			()		{	return 	m_pPlayerDart;								}
	//inline CPlayerIdentity*	getPlayerIdentity		()		{	return 	m_pPlayerIdentity;							}
	//inline CPlayerTitle*	getPlayerTitle			()		{	return 	m_pPlayerTitle;								}
	//inline CPlayerClimbTower*		getPlayerClimbTower()		{	return 	m_pPlayerClimbTower;					}
	//inline CPlayerGreatWallTrain*	getPlayerGreatWallTrain()	{	return 	m_pPlayerGreatWallTrain;				}
	//inline CPlayerPvpWar*		getPlayerPvpWar		()		{	return 	m_pPlayerPvpWar;							}
	//inline CPlayerCorps*		getPlayerCorps		()		{	return 	m_pPlayerCorps;								}
	//inline CPlayerWorldBoss*	getPlayerWorldBoss	()		{	return 	m_pPlayerWorldBoss;							}
	//inline CPlayerActivity*		getPlayerActivity	()		{	return 	m_pPlayerActivity;							}
	inline CPlayerRelive*		getPlayerRelive		()		{	return 	m_pPlayerRelive;							}
	//inline CPlayerWeal*			getPlayerWeal		()		{	return 	m_pPlayerWeal;								}
	//inline CPlayerSinglePvp*	getPlayerSinglePvp	()		{	return 	m_pPlayerSinglePvp;							}
	//inline CPlayerManor*		getPlayerManor		()		{	return 	m_pPlayerManor;								}
	inline CPlayerMail*			getPlayerMail		()		{	return 	m_pPlayerMail;								}
	inline CPlayerEquipColumn*	getPlayerEquipColumn()		{	return 	m_pPlayerEquipColumn;						}


	bool					checkOnlyIndex			(uint16 uIndex);
	void					setOnlyIndex			(uint16 uIndex,bool bSet);
	void					clearOnlyEvent  		(_em_CleanupOnlyIndex emOnlyIndex);
	void					onBuildComplete			();
	void					onLogin					();
	void					onPassLogicDayClearup	(uint64 uProcessTime, uint64 uTime);
	void					gmCleanDailyTime		();
	void					dailyCleanup			(uint64 uTime,int32 iOfflineDay);
	void					weekCleanup				(uint64 uTime,int32 iOfflineWeek);
	void					monthCleanup			(uint64 uTime,int32 iOfflineMonth);
	bool					canPreCondition			(const DMAP_UINT8_32_32& dmapPreCondition,int64 nValue = 1);
	bool					addPreCondition			(const DMAP_UINT8_32_32& dmapPreCondition,uint8 uPreType,int64 nAddValue = 0);
	bool					addPreCondition			(uint8 uPreType,uint32 uID,int32 nValue,int64 nAddValue = 0);
	bool					addExp					(int32 nAddValue,_emDoingType emDoing);
	bool					promote					(uint8& uCode);
	bool					setPKType				(uint8 uType,uint8& uCode);
	bool					addLevel				(int32 nAddValue,_emDoingType emDoing );
	uint64					getMoney				(uint8 uMoneyType);
	bool					canAddMoney				(uint8 uMoneyType,int64 nMoney);
	bool					addMoney				(uint8 uMoneyType,int64 nMoney, _emDoingType emDoing, PBMoneyConsume *pbConsume = NULL);
	bool					addMoney				(const PBMoneyConsume &consume, _emDoingType emDoing );
	virtual uint64			getExpend				(uint16 uExpendType);
	virtual bool			isExpendFull			(uint16 uExpendType);
	virtual bool			canExpend				(uint16 uExpendType,uint64 uValue);
	virtual bool			canExpend				(const MAP_U16_I64& mapExpend);
	virtual bool			canExpend				(const MAP_NEED_EXPEND& mapExpend);  //兼容性
	virtual bool			canDelExpend			(const MAP_NEED_EXPEND& mapExpend,uint16 uErroType = 0);
	bool					clearAllExpend			();
	bool					delExpend				(const MAP_NEED_EXPEND& mapExpend,_emDoingType emDoing,CSceneUnit* pSource = NULL);
	bool					addExpend				(const MAP_NEED_EXPEND& mapExpend,_emDoingType emDoing ,CSceneUnit* pSource = NULL);
	virtual bool			addExpend				(uint16 uExpendType,int64 iValue,_emDoingType emDoing ,CSceneUnit* pSource = NULL);
	virtual bool			addExpend				(const MAP_U16_I64& mapExpend,_emDoingType emDoing ,CSceneUnit* pSource = NULL);
	void					updatePKValue			(uint32 uValue,bool bAdd);
	void					refreshKillTime			();
	void					checkUpdatePKValue		(CScenePlayer* pBeKillPlayer);
	bool					canAddPKValue			(CScenePlayer* pBeKillPlayer);
	bool					addPrize				(uint32 uPrizeID);
	bool					plane_enter				(uint8& uCode);
	bool					plane_leave				(PBC2GCommon_PlaneLeave& msg,uint8& uCode);
	bool					updateMoney				(uint8 uMoneyType,int64 nMoney,_emDoingType emDoing );
	bool					canUpdateMoney			(uint8 uMoneyType,int64 nMoney);
	bool					haveIdentity			(uint16	uID);
	bool					mount					(bool bOnMount);
	bool					toplist_update			(uint32 uType,uint64 uValue,uint64 uSubValue);
public:
	virtual void			unregisterObject		();
	virtual void			enterScene				(CGameSceneCell*pCell);
	virtual void			leaveScene				();
	virtual bool			isVisible				(CSceneUnit*pObject);	// 目标对自己是否可见 
public:
	void					savePlayer				(uint64 uTime,bool bNow = true);
	bool					buildPlayer				(const PBPlayerData&pbPlayer,bool bLogin);
	CGamePlayerFun*			getPlayerFun			(_emGameSystemType uType);
public:
	void					loadDataFromProtobuf	(const PBPlayerData&pbPlayer,bool bLogin);
	void					saveDataToProtobuf		(PBPlayerData&pbPlayer);
	void					saveMemoryToBase		();

public:
	bool					functionNetPacket		(uint8 uMainProtocol,uint8 uProtocol,uint8 uEventFlag,CNetStream& clStream);
	bool					checkSystemSwitchOpen		(uint8 uMainProtocol, uint8 uProtocol);
public:
	bool					playerFlyScene_ByCellID	(uint16 SceneID,uint32 uCellID,uint16 uRegionID);
	bool					playerFlyScene_ByDynID	(uint16 SceneID,uint64 uDynID,uint16 uRegionID);
	bool					playerFlyScene_ByCellID	(uint16 SceneID,uint32 uCellID,float32 fPosX,float32 fPosY);
	bool					playerFlyScene_ByDynID	(uint16 SceneID,uint64 uDynID,float32 fPosX,float32 fPosY);
	bool					playerFlyScene_Imp		(uint16 SceneID,uint64 uDynID,uint32 uCellID,bool bFixPos,uint16 uRegionID,float32 fPosX,float32 fPosY);
	void					playerRealFlyScene		(uint16 SceneID,uint64 uDynID,uint32 uCellID,bool bFixPos,uint16 uRegionID,float32 fPosX,float32 fPosY,_stSvrInfo& stTargetGameSvrInfo,_stNetHost& stTargetHost);
	bool					randPos					(uint16 uSpawnID);
	bool					randScene				();

public:
	bool					copymap_create			(uint16 uCopymapID,uint32 uParam);
	CSceneMonster*			createMonster			(uint32 uMonsterID,float32 fRadius = 1.0f);
public:
	virtual	bool			packetGhost				(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo);
private:
	void					synGhost				(uint64 uTime);
	bool					synExpendDataToClient	(uint16 uType, uint64 uValue);

	uint16					getRelivePoint	();
public:
	void					packetView				(PBPlayerView* pbView,bool bFindPlayer = false);
	void					updateAvater			();
public:
	virtual uint32			getFlyTotalTick			();
	virtual uint32			getBakFlayTick			();
	virtual float32			getModelRadius			();
	virtual uint8			getRelation				(CSceneUnit* pUnit);
	virtual CSceneUnit*		getOwner				();
	virtual _stTeamInfo*	getTeam					();
	virtual uint32			getFactionID			();
	virtual uint32			getUnionID				();
	virtual uint32			getCorpsID				();
protected:
	virtual void			onEnterState			(_emGameStateType uStateType);
	virtual void			onLeaveState			(_emGameStateType uStateType,bool bInterupt);
public:
	bool					processEnermyList		(uint64 uTick);
	void					AddEnermy				(CScenePlayer* pPlayer);
	void					AddSelfEnermy			(CScenePlayer* pPlayer);
	void					DelEnermy				(CScenePlayer* pPlayer,bool bDel = false);
	void					DelSelfEnermy			(CScenePlayer* pPlayer);
	void					DelSelfAllEnermy		();
	void					refreshVisibleMonster	();
public:
	 bool					sendMail				(_emMailType emType,const DMAP_NEED_ITEM* pAddItem,_emDoingType emDoing,::google::protobuf::Message* msg = NULL);

public:
	void					handleOfflineDataLoad	(uint32 uOrderNum,char* pData, uint16 uSize);
	// 确认不在线后 可调用
	static bool				offlineData				(uint16 uLogicWorldID, uint32 uPlayerID, uint16 uSysType, uint32 uSignType, ::google::protobuf::Message* msg);
	static bool				offlineData				(uint16 uLogicWorldID, uint32 uPlayerID, uint16 uSysType, uint32 uSignType, void* msg, uint16 uSize);
	// 在线或者不在线 都统一处理 可调用
	static bool				sendOpeData				(uint16 uLogicWorldID, uint32 uPlayerID, uint16 uSysType, uint32 uSignType, ::google::protobuf::Message* msg);
	static bool				sendOpeData				(uint16 uLogicWorldID, uint32 uPlayerID, uint16 uSysType, uint32 uSignType, void* msg, uint16 uSize);
public:
	void					processUpdateGlobal		();
	void					processAutoRelive		(uint64 uTime);
public:
	//收到前端场景加载完成 回调
	void					callbackClientGo		();
	void					callback_flyout			();

	void			callbackCompleteTreasureQuest	(uint32 uQuestID,uint32 uItemID);
	void			treasurePrize					(uint32 uTreasureID,uint32 uPrizeID,float32 fPosx,float32 fPosy);
	void			addForbitSpeak					(uint32 uChannel,uint32 uAddSecond);
	bool			calculateSubFightPower				(uint8 &uCode);
public:
	virtual void	collectTempSkill		(MAP_U16_U8& mapSkill);
	virtual void	callbackAttack			(CSceneUnit* pUnit,int32 iDamage,bool bCritical);
	virtual void	callbackByAttack		(CSceneUnit* pUnit,int32 iDamage);
	virtual void	callbackCritical		();
	virtual void	callbackSkillHit		(const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual void	callbackBeSkillEffect	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual void	callbackDead			(CSceneUnit* pSource);
	virtual void	callbackKillTarget		(CSceneUnit* pTarget);
	virtual void	callbackLife			();
	virtual	void	callbackFightPower		();
	virtual	void	callbackFight			();
	virtual	void	callbackEnterScene		(CGameSceneCell* pSceneCell);
	virtual void	callbackPostionChange	();//位置移动,
	virtual void	callbackUseSkill		(const _stSkillModel*pSkillModel,uint32 uTarUnitID);


};
#include "scenePlayer_inline.h"
