/*------------- gameSceneCell.h
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/3/23 11:57:33
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "gameLogic/sceneManage/sceneTrapMgr.h"
#include "gameLogic/sceneManage/sceneMonsterMgr.h"
#include "gameLogic/sceneManage/sceneItemMgr.h"
//#include "gameLogic/sceneManage/sceneDartCarMgr.h"
#include "gameSceneBlock.h"
#include "def_functions/pathFinder.h"
#include "def_struct/dartcarStruct.h"
/*************************************************************/
class CGameScene;
class CCopymapBase;
class CGameSceneBlock;
struct _stSceneMapModel;
struct _stCopymapModel;
class CSceneMonster;
/*************************************************************/

class CGameSceneCell	: public CSceneMonsterMgr
						, public CSceneTrapMgr
						, public CSceneItemMgr
						//, public CSceneDartCarMgr

{
protected:
	enum
	{
		SceneBlock_Size	= 4,	//地形块尺寸
	};

protected:
	typedef	stl_map<uint32,uint64>			MAP_ANIMATION_AREA; // area id ==〉 end ani tick
	typedef stl_vector<uint32>				VECTOR_UNIT_ID;

protected:
	uint32					m_uSceneSizeWide;		//地图宽
	uint32					m_uSceneSizeHigh;		//地图高
	uint32					m_uSceneGridWide;		//地图格宽
	uint32					m_uSceneGridHigh;		//地图格高
protected:
	uint32					m_uBlockWideNum;		//地形块横向数量(m_uSceneSizeHigh / m_uTileWide)
	uint32					m_uBlockHighNum;		//地形块竖向数量(m_uSceneSizeHigh / m_uTileHigh)
	uint32					m_uBlockTotal;			//地形块总数量(m_uBlockWideNum * m_uBlockHighNum)
	CGameSceneBlock*		m_pBlock;

protected:
	uint32					m_uCellID;
	uint64					m_uDynID;
	CGameScene*				m_pSceneMap;
	CCopymapBase*			m_pGameCopymap;
	_stSceneMapModel*		m_pSceneModel;
	CPathFinder				m_clPathFinder;
	MAP_ANIMATION_AREA		m_mapAddAniArea;
	MAP_ANIMATION_AREA		m_mapDelAniArea;

protected:
	HASH_PLAYER				m_hashPlayer;
	MAP_GAME_UNIT			m_mapGameObject;
private:
	bool					m_bClosing;
protected:
	bool					m_bFactionDrinkWine;
	uint32					m_uFactionDrinkWineNpc;
protected:
	VECTOR_UNIT_ID			m_vecLevelObject[_em_GameUnitType_ARRAYSIZE];
public:
	inline uint32		playerCount	()
	{
		return m_hashPlayer.size();
	}
	inline HASH_PLAYER&			 getAllPlayer	()	{	return m_hashPlayer;			}
	inline HASH_PLAYER::iterator player_begin	()	{	return m_hashPlayer.begin();	}
	inline HASH_PLAYER::iterator player_end		()	{	return m_hashPlayer.end();		}
	inline CScenePlayer*findPlayer	(uint32 uPlayerID)
	{
		HASH_PLAYER::iterator pos = m_hashPlayer.find(uPlayerID);
		return ((pos != m_hashPlayer.end()) ? pos->second : NULL);
	}

	inline MAP_GAME_UNIT&		getAllObject	()	{	return m_mapGameObject;			}
	inline MAP_GAME_UNIT::iterator object_begin	()	{	return m_mapGameObject.begin();	}
	inline MAP_GAME_UNIT::iterator object_end	()	{	return m_mapGameObject.end();	}
	inline CSceneUnit*	findObject	(uint32 uObjectID)
	{
		MAP_GAME_UNIT::iterator pos = m_mapGameObject.find(uObjectID);
		return ((pos != m_mapGameObject.end()) ? pos->second : NULL);
	}
public:
	inline uint32			getCellID			()const						{	return m_uCellID;			}
	inline uint64			getDynID			()const						{	return m_uDynID;			}
	inline CGameScene*		getSceneMap			()							{	return m_pSceneMap;			}
	inline CCopymapBase*	getGameCopymap		()							{	return m_pGameCopymap;		}
	inline const _stSceneMapModel*getSceneModel	()							{	return m_pSceneModel;		}
	_stCopymapModel*		getCopymapModel		();

public:
	uint32					getBlockIndexWide	(float32 fx)const;
	uint32					getBlockIndexHigh	(float32 fy)const;
	uint32					getBlockIndex		(int32 uWide,int32 uHigh)const;
public:
	CGameSceneCell();
	~CGameSceneCell();

public:
	uint16	getSceneID			();
	bool	isCopymap			();	
	void	restart				();
	void	close				();
	void	shutdown			();
	bool	isClose				();
public:
	void	initialize			(uint64 uDynID = 0, uint32 uCellID = 0, CGameScene*pSceneMap = NULL, const PBCreateCellParam *pbParam=NULL );
public:
	bool	playerEnterScene	(CScenePlayer*pPlayer);
	void	playerLeaveScene	(CScenePlayer*pPlayer);

	void	player_syn			(CScenePlayer*pPlayer);

	void	broadcastMsg		(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	void	onClientGo			(CScenePlayer*pPlayer);
protected:
	void	processFactionHomeLogic		(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
	void	processFactionDrinkWine		(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
public:
	bool	inFactionDrinkWineTime		();
	bool	isFactionHome				();

	void	process				(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);

public:
	bool	enterScene			(CSceneUnit*pObject);
	void	leaveScene			(CSceneUnit*pObject);

protected:
	void	removeObject		(uint8 uObjectType,uint32 uUnitID);

protected:
	/*--->[ 初始化 ]*/
	void	initBlock			(uint32 uWide,uint32 uHigh);
	/*--->[ 清理 ]*/
	void	clearBlock			();

public:
	bool	getSpawnPoint		(_stPoint2F&stPoint,uint16 uRegionID);
	bool	randGetPoint		(_stPoint2F&stOut,float32 fCenterX,float32 fCenterY,float32 fRadius);
	bool	randGetPoint		(stl_vector<_stPoint2F>& vecOut,_stPoint2F fCenter,float32 fRandDis,float32 fRadius,uint32 uOutNum);
	bool	findPath			(const _stPoint2F& start,const _stPoint2F& end,std::vector<_stPoint2F>& vecPath);
	bool	findPath			(const _stPoint2F& start,const _stPoint2F& end,std::list<_stPoint2F>& listPath);
	bool	findPath			(const _stPoint2F& start,const _stPoint2F& end,_stPoint2F& stNextPoint);
	bool	canMove				(float32 fx,float32 fy);
	// 找到可通过的最远直线点
	bool	getTargetPoint		(const _stPoint2F& start,float32 fdir,float32 fdis,_stPoint2F& target,_stFindPathMark* pMark = NULL);
	// 找到可通过的最远直线点
	bool	getTargetPoint		(const _stPoint2F& start,const _stPoint2F& end,_stPoint2F& target,_stFindPathMark* pMark = NULL);
	// 两点之间是否 直线可通
	bool	isPassable			(const _stPoint2F& start,const _stPoint2F& end,_stFindPathMark* pMark = NULL);
public:
	//--- 受击对像范围内某坐标对像列表
	void			gatherObject(const _stPoint2F&stPoint,float32 fRadius,uint32 uTypeMark,MAP_GAME_UNIT& _map);
	/*--->[ 对像是否存在 ]*/
	CSceneUnit*		findObject	(uint32 uObjectID,float32 fx,float32 fy,float32 r);
	/*--->[ 擦除对像 ]*/
	void			eraseObject	(CSceneUnit*pObject,float32 fx,float32 fy);
	//--- 获得场景块
	CGameSceneBlock*getBlock	(float32 fx,float32 fy);
	/*--->[ 插入对像 ]*/
	bool			insertObject(CSceneUnit*pObject,float32 x,float32 y);

private:
	void			synPlayerCount2World();

public:
	virtual	void	callback_killmonster(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	virtual void	callback_depotGoods(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	virtual	void	arrestmonster(CSceneUnit* pTarget,CScenePlayer* pPlayer);
	virtual	void	thugmonster(CSceneUnit* pTarget,CScenePlayer* pPlayer);
	virtual	void	npcdialog(uint32 uUnitID,CScenePlayer* pPlayer);
	virtual	void	callback_playerdead(CScenePlayer* pPlayer,CSceneUnit* pSource);
	virtual	void	callback_playerlife(CScenePlayer* pPlayer,CScenePlayer* pSource);
	virtual	void	callback_flyout(CScenePlayer* pPlayer);
	virtual	void	callback_player_gethurt(CScenePlayer* pPlayer, uint32 uHurt, CScenePlayer* pPlayerSource);
public:
	inline CPathFinder&	getPathFinder()								{	return m_clPathFinder;			}
	// 增加特殊区域
	void			addArea			(uint32	uAreaID);
	// 删除特殊区域
	void			delArea			(uint32	uAreaID);

	bool			testRegion		(uint8 uAttrType)const;

	uint16			getDefaultRegion();
};
