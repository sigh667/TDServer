/*----------------- gameSceneCell.cpp
*
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/13 15:24:33
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameScene/gameSceneCell.h"
#include "gameScene/gameScene.h"
#include "basic/systemFunction.h"
#include "log/log.h"
#include "cfg_data/loadData/loadData.h"
#include "cfg_data/monsterData/monsterData.h"
#include "gameScene/copymapBase.h"
#include "protocol/WAGMsg.pb.h"
#include "protocol/WAGProtocol.pb.h"
#include "main/gameServer.h"
#include "protocol/sceneStruct.pb.h"
#include "gameScene/copymapMgr.h"
//#include "gameLogic/playerDart/playerDart.h"
//#include "gameLogic/playerClimbTower/playerClimbTower.h"
#include "cfg_data/constantData/constantData.h"
#include "cfg_data/factionData/factionData.h"
//#include "gameLogic/playerFaction/playerFaction.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CGameSceneCell::CGameSceneCell()
{
	m_uSceneSizeWide	= 0;
	m_uSceneSizeHigh	= 0;
	m_uSceneGridWide	= 0;
	m_uSceneGridHigh	= 0;
	m_uBlockWideNum		= 0;
	m_uBlockHighNum		= 0;
	m_uBlockTotal		= 0;
	m_pBlock			= NULL;
	m_pGameCopymap		= NULL;
	initialize(0,0,NULL, NULL);
}

//-------------------------------------------------------------
//------------------------------ 
CGameSceneCell::~CGameSceneCell()
{
	shutdown();
}

//-------------------------------------------------------------
//------------------------------ 
uint16		CGameSceneCell::getSceneID	()
{
	return (m_pSceneMap ? m_pSceneMap->getSceneID() : 0);
}

//-------------------------------------------------------------
//------------------------------ 
bool		CGameSceneCell::isCopymap	()
{
	return  m_pSceneModel && m_pSceneModel->pCopymapModel;
}

//-------------------------------------------------------------
//------------------------------ 
_stCopymapModel*	CGameSceneCell::getCopymapModel	()
{
	return  m_pSceneModel ? m_pSceneModel->pCopymapModel : NULL;
}

//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::restart		()
{
	m_bClosing	= false;
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::close				()
{
	m_bClosing	= true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::isClose				()
{
	if ( !m_bClosing )
		return false;

	return m_hashPlayer.empty();
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::shutdown()
{
	CSceneUnit*pUnit;
	MAP_GAME_UNIT::iterator _pos = m_mapGameObject.begin();
	MAP_GAME_UNIT::iterator _end = m_mapGameObject.end();
	for(;_pos != _end;++_pos)
	{
		pUnit = _pos->second;
		if(pUnit)
			pUnit->unregisterObject();
	}
	m_mapGameObject.clear();

	safe_delete_array(m_pBlock);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::initialize(uint64 uDynID,uint32 uCellID,CGameScene*pSceneMap, const PBCreateCellParam *pbParam)
{
	m_uCellID			= uCellID;
	m_uDynID			= uDynID;
	m_pSceneMap			= pSceneMap;
	m_pSceneModel		= NULL;
	m_pGameCopymap		= NULL;
	m_bClosing			= false;
	m_clPathFinder.initialize();
	m_bFactionDrinkWine	= false;
	m_uFactionDrinkWineNpc = 0;
	
	m_mapGameObject.clear();
	m_mapAddAniArea.clear();
	m_mapDelAniArea.clear();
	if(pSceneMap && pbParam)
	{
		m_pSceneModel		= pSceneMap->getSceneModel();
		//初始化场景块
		initBlock(pSceneMap->getSceneSizeWide(),pSceneMap->getSceneSizeHigh());
		m_uSceneGridWide	= pSceneMap->getSceneGridWide();
		m_uSceneGridHigh	= pSceneMap->getSceneGridHigh();

		m_clPathFinder.setGround(&pSceneMap->getGround());

		MAP_SCENE_AREA::iterator _pos = pSceneMap->m_mapArea.begin();
		MAP_SCENE_AREA::iterator _end = pSceneMap->m_mapArea.end();
		for ( ; _pos != _end ; ++_pos )
		{
			if ( !_pos->second.isDynamic )
			{
				m_clPathFinder.addArea(&_pos->second,false);
			}
		}
		m_clPathFinder.build();

		CSceneMonsterMgr::initialize(this,pSceneMap->m_vecMonsterRegion);
		CSceneTrapMgr::initialize(this);
		CSceneItemMgr::initialize(this);
		//CSceneDartCarMgr::initialize(this);


		if (m_pSceneModel->isCopymap())
		{
			m_pGameCopymap	= g_pCopymapMgr->alloc(m_pSceneModel->pCopymapModel);
			if (m_pGameCopymap)
			{
				m_pGameCopymap->restart(this,*pbParam);
			}
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::player_syn			(CScenePlayer*pPlayer)
{
	if ( !pPlayer || !m_pSceneMap )
		return ;

	{
		PBS2CSceneAreaList msg;
		auto _pos = m_clPathFinder.m_vecArea.begin();
		auto _end = m_clPathFinder.m_vecArea.end();
		for ( ; _pos != _end ; ++_pos )
		{
			if (m_mapDelAniArea.find(_pos->uAreaID) == m_mapDelAniArea.end() )
				msg.add_areaid(_pos->uAreaID);
		}

		auto _pos1 = m_mapAddAniArea.begin();
		auto _end1 = m_mapAddAniArea.end();
		for ( ; _pos1 != _end1 ; ++_pos1 )
		{
			msg.add_areaid(_pos1->first);
		}

		if (msg.areaid_size() > 0)
		{
			pPlayer->sendMsg(P_S2C_Protocol_Common,S2C_Common_Area_Syn,&msg);
		}
	}
	
	//如果有押镖任务,当前场景又没有镖车,极少可能同步问题,那么重新向worldserver 请求镖车的位置,然后跳转
	//if (pPlayer->getPlayerDart())
	//{
	//	pPlayer->getPlayerDart()->onLoadOverTryTraceDartcar();
	//}
	////如果在个人爬塔玩法中,需要继续跳转下一个挑战副本
	//if (pPlayer->getPlayerClimbTower())
	//{
	//	pPlayer->getPlayerClimbTower()->checkInClimbCpymap();
	//}

	if ( m_pGameCopymap )
		m_pGameCopymap->playerOnline(pPlayer);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::onClientGo			(CScenePlayer*pPlayer)
{
	if ( m_pGameCopymap )
		m_pGameCopymap->onClientGo(pPlayer);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::synPlayerCount2World()
{
	PBG2WScenePlayerCount info;
	info.set_scenedynid(getDynID());
	info.set_sceneid(getSceneID());
	info.set_playercount(playerCount());
	g_pServer->sendMsgToWorld(g_pServer->getLogicWorldID(),P_G2W_Protocol_Scene, G2W_Scene_PlayerCount,&info);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::playerEnterScene(CScenePlayer*pPlayer)
{
	if(!pPlayer || !m_pSceneMap || !m_pSceneModel)
		return false;

	{///离开原场景
		CGameSceneCell*pCell = pPlayer->getSceneCell();
		if(pCell)
			pCell->playerLeaveScene(pPlayer);
	}

	static _stPoint2F stPoint;
	stPoint.x = pPlayer->getPlayerBase().fPosX;
	stPoint.y = pPlayer->getPlayerBase().fPosY;
	if(!pPlayer->testPlayerMark(PlayerMark_FixPoint))
	{
		//进入场景时选择出生点 判断是否用进入前的坐标
		if (!m_pSceneModel->testSceneMark(SceneMapMarkType_UseEnterPos))
		{
			if (getSpawnPoint(stPoint,pPlayer->getPlayerBase().uSpawnID))
				pPlayer->setPlayerMark(PlayerMark_FixPoint,true);
		}
		else
		{
			pPlayer->setPlayerMark(PlayerMark_FixPoint,true);
		}
	}
	//设置随机点
	if(!canMove(stPoint.x,stPoint.y) )
	{
		randGetPoint(stPoint,stPoint.x,stPoint.y,CGhostObject::ViewRange / 3);		
	}
	if(!canMove(stPoint.x,stPoint.y))
	{
		m_clPathFinder.findRandomPoint(stPoint);
	}
	pPlayer->addState(StateType_WaitReady);
	pPlayer->setPosition(stPoint,SetPositionReason_Build,false);
	pPlayer->setDirection(pPlayer->getPlayerBase().fDir);
	if (!enterScene(pPlayer))
	{
		CLog::error("角色:%d进入副本层[%d->%d(%d,%d)]失败!",pPlayer->getID(),getSceneID(),getCellID(),stPoint.x,stPoint.y);
		return false;
	}
	pPlayer->setGhost(_BIT32(Ghost_Move),false,true);
	m_hashPlayer.insert_(pPlayer->getPlayerID(),pPlayer);

	synPlayerCount2World();
	
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::playerLeaveScene(CScenePlayer*pPlayer)
{
	if(!pPlayer)
		return;

	CLog::warn("角色:%d离开场景线[%d->%d]",pPlayer->getPlayerID(),getSceneID(),getCellID());

	leaveScene(pPlayer);
	m_hashPlayer.erase(pPlayer->getPlayerID());

	synPlayerCount2World();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::canMove(float32 fx,float32 fy)
{
	_stPoint2F pos;
	pos.x = fx;
	pos.y = fy;
	return m_clPathFinder.isMoveable(pos);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::enterScene(CSceneUnit*pObject)
{
	FUNCTION_BEGIN_CHECK();
	if(!pObject)
		return false;

	//镖车保留unitid 
	//if (pObject->asDartCar() != NULL)
	//{
	//	pObject->registerObject(pObject->getUnitID());
	//}
	//else
	{
		pObject->registerObject();
	}
	pObject->enterScene(this);
	if(!pObject->onSetPoint(false))
	{
		removeObject(pObject->getUnitType(),pObject->getUnitID());
		pObject->leaveScene();
		return false;
	}

	m_mapGameObject.insert_(pObject->getUnitID(),pObject);

	//角色进入副本
	CScenePlayer* pPlayer= pObject->asPlayer();
	if(pPlayer)
	{
		if(m_pGameCopymap)
			m_pGameCopymap->playerEnter(pPlayer);
	}
	//镖车进入场景
	//if (pObject->asDartCar() != NULL)
	//{
	//	onEnterScene(pObject->asDartCar());
	//}
	//怪物进入副本
	CSceneMonster* pMonster= pObject->asMonster();
	if(pMonster && m_pGameCopymap)
	{
		m_pGameCopymap->callback_createmonster(pMonster);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::leaveScene(CSceneUnit*pObject)
{
	FUNCTION_BEGIN_CHECK();
	if(!pObject)
		return;

	if (m_pGameCopymap && pObject->asPlayer())
		m_pGameCopymap->playerLeave(pObject->asPlayer());

	if (m_pGameCopymap && pObject->asTrap())
		m_pGameCopymap->callback_trapLeave(pObject->asTrap());


	removeObject(pObject->getUnitType(),pObject->getUnitID());

	pObject->leaveScene();

	eraseObject(pObject,pObject->getPosition().x,pObject->getPosition().y);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::removeObject(uint8 uObjectType,uint32 uUnitID)
{
	if(uObjectType < _em_GameUnitType_ARRAYSIZE)
		m_vecLevelObject[uObjectType		].push_back(uUnitID);
	else
		m_vecLevelObject[GameObject_None	].push_back(uUnitID);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::process		(uint64 uTick, uint64 uTime, uint32 uSecond, bool bNewDay)
{
	//1 移除所有离开场景对像
	uint32 uUnitID	= 0;
	uint32 uSize	= 0;
	for (int i = 0;i < _em_GameUnitType_ARRAYSIZE;++i)
	{
		VECTOR_UNIT_ID&vec = m_vecLevelObject[i];
		uSize = vec.size();
		for (uint32 j = 0;j < uSize;++j)
		{
			uUnitID = vec[j];

			if(i == GameObject_Trap)
				CSceneTrapMgr::removeObject(uUnitID);
			if(i == GameObject_Item)
				CSceneItemMgr::removeObject(uUnitID);
			//if(i == GameObject_DartCar)
			//	CSceneDartCarMgr::removeObject(uUnitID);

			m_mapGameObject.erase(uUnitID);
		}

		if(i == GameObject_Monster)
			CSceneMonsterMgr::remove(uTime,vec);

		vec.clear();
	}
		
	//2 游戏对像推进
	if(g_clLoadData.is_reload_mark() || !m_hashPlayer.empty() )
	{
		CSceneUnit*pUnit;
		MAP_GAME_UNIT::iterator _pos = m_mapGameObject.begin();
		MAP_GAME_UNIT::iterator _end = m_mapGameObject.end();
		for(;_pos != _end;++_pos)
		{
			pUnit = _pos->second;
			if(!pUnit)
				continue;

			if(g_clLoadData.is_reload_mark())
				pUnit->refreshAttribute();

			if(pUnit->getSceneCell())
			{
				pUnit->process(uTick,uTime,uSecond,bNewDay);
			}
		}
	}

	//3 其他逻辑推进
	CSceneMonsterMgr::processObject(uTick, uTime, uSecond);
	CSceneTrapMgr::processObject(uTick, uTime);

	if (m_pGameCopymap)
	{
		m_pGameCopymap->process(uTick, uTime, uSecond);
	}

	{// 特殊区域 动画完成 推进
		{
			auto _pos = m_mapAddAniArea.begin();
			while (  _pos != m_mapAddAniArea.end() )
			{
				auto _itr = _pos++;
				if (_itr->second > uTick )
					continue;

				const _stSceneArea* pArea = m_pSceneMap->findArea(_itr->first);
				if (pArea)
					m_clPathFinder.addArea(pArea,true);	
				m_mapAddAniArea.erase(_itr);
			}
		}

		{
			auto _pos = m_mapDelAniArea.begin();
			while (  _pos != m_mapDelAniArea.end() )
			{
				auto _itr = _pos++;
				if (_itr->second > uTick )
					continue;
				m_clPathFinder.delArea(_itr->first,true);	
				m_mapDelAniArea.erase(_itr);
			}
		}
	}

	if (getSceneID() == g_clConstantData.Faction_HomeSceneID )
	{
		processFactionHomeLogic(uTick,uTime,uSecond,bNewDay);
	}
}
//-------------------------------------------------------------
//------------------------------ 初始化
void	CGameSceneCell::initBlock	(uint32 uWide,uint32 uHigh)
{
	FUNCTION_BEGIN_CHECK();
	safe_delete_array(m_pBlock);

	m_uSceneSizeWide	= uWide;
	m_uSceneSizeHigh	= uHigh;
	m_uBlockWideNum= uWide / SceneBlock_Size;
	m_uBlockHighNum= uHigh / SceneBlock_Size;

	if(uWide % SceneBlock_Size)
		++m_uBlockWideNum;

	if(uHigh % SceneBlock_Size)
		++m_uBlockHighNum;

	m_uBlockTotal	= m_uBlockWideNum * m_uBlockHighNum;

	m_pBlock	= new CGameSceneBlock[m_uBlockTotal];
	for(uint32 x = 0;x < m_uBlockWideNum;++x)
	{
		for(uint32 y = 0;y < m_uBlockHighNum;++y)																																								
		{
			m_pBlock[getBlockIndex(x,y)].init(uint16(x),uint16(y));
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 清理
void	CGameSceneCell::clearBlock	()
{
	FUNCTION_BEGIN_CHECK();
	if (!m_pBlock)
		return;

	for(uint32 i = 0;i < m_uBlockTotal;++i)
	{
		m_pBlock[i].clear();
	}
}
//-------------------------------------------------------------
//------------------------------ 受击对像范围内某坐标对像列表
void	CGameSceneCell::gatherObject(const _stPoint2F&stPoint,float32 fRadius,uint32 uTypeMark,MAP_GAME_UNIT& _map)
{
	FUNCTION_BEGIN_CHECK();
	float32 minX = stPoint.x - fRadius;
	float32 minY = stPoint.y - fRadius;
	float32 maxX = stPoint.x + fRadius;
	float32 maxY = stPoint.y + fRadius;

	uint32 minIndexX=getBlockIndexWide(minX);
	uint32 minIndexY=getBlockIndexHigh(minY);
	uint32 maxIndexX=getBlockIndexWide(maxX);
	uint32 maxIndexY=getBlockIndexHigh(maxY);

	for(uint32 _x = minIndexX;_x <= maxIndexX;++_x)
	{
		for(uint32 _y = minIndexY;_y <= maxIndexY;++_y)
		{
			m_pBlock[getBlockIndex(_x,_y)].gather(stPoint,fRadius,uTypeMark,_map);
		}
	}
}
/*------------------------------------------------------------------------------
**对像是否存在
*/
CSceneUnit*CGameSceneCell::findObject	(uint32 uObjectID,float32 fx,float32 fy,float32 r)
{
	FUNCTION_BEGIN_CHECK();
	CSceneUnit*pUnit = findObject(uObjectID);
	if(!pUnit)
		return NULL;

	_stPoint2F stPoint = pUnit->getPosition();
	if((fx - r) > stPoint.x || stPoint.x > (fx + r))
		return NULL;

	if((fy - r) > stPoint.y || stPoint.y > (fy + r))
		return NULL;

	return pUnit;
}

//-------------------------------------------------------------
//------------------------------ 擦除对像
void	CGameSceneCell::eraseObject	(CSceneUnit*pObject,float32 fx,float32 fy)
{
	FUNCTION_BEGIN_CHECK();
	if(!pObject)
		return;

	CGameSceneBlock*pBlock = getBlock(fx,fy);
	if(pBlock)
		pBlock->erase(pObject);
}

//-------------------------------------------------------------
//------------------------------ 获得场景块
CGameSceneBlock*CGameSceneCell::getBlock(float32 fx,float32 fy)
{
	FUNCTION_BEGIN_CHECK();
	uint32 uIndex = getBlockIndex(getBlockIndexWide(fx),getBlockIndexHigh(fy));
	if(uIndex >= m_uBlockTotal)
		return NULL;

	return &(m_pBlock[uIndex]);
}
//-------------------------------------------------------------
//------------------------------ 插入对像
bool CGameSceneCell::insertObject(CSceneUnit*pObject,float32 fx,float32 fy)
{
	FUNCTION_BEGIN_CHECK();
	if(!pObject)
		return false;

	CGameSceneBlock*pBlock	= getBlock(fx,fy);
	if ( !pBlock)
		return false;

	if(pObject->getSceneBlock() != pBlock)
	{
		pObject->updateSceneBlock(pBlock);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
uint32	CGameSceneCell::getBlockIndexWide	(float32 fx)const
{
	int32 x = getGridPostion(fx,m_uSceneGridWide);
	if(x < 0)
		x = 0;
	uint32 uIndex = x / SceneBlock_Size;
	return ((uIndex < m_uBlockWideNum) ? uIndex : (m_uBlockWideNum - 1));
}
//-------------------------------------------------------------
//------------------------------
uint32	CGameSceneCell::getBlockIndexHigh	(float32 fy)const
{
	int32 y = getGridPostion(fy,m_uSceneGridHigh);

	if(y < 0)
		y = 0;
	uint32 uIndex = y / SceneBlock_Size;
	return ((uIndex < m_uBlockHighNum) ? uIndex : (m_uBlockHighNum - 1));
}
//-------------------------------------------------------------
//------------------------------
uint32	CGameSceneCell::getBlockIndex		(int32 uWide,int32 uHigh)const
{
	uint32 uIndex = uHigh * m_uBlockWideNum + uWide;
	return (uIndex < m_uBlockTotal ? uIndex : 0);							
}

//-------------------------------------------------------------
//------------------------------ 
bool CGameSceneCell::findPath(const _stPoint2F& start,const _stPoint2F& end,std::vector<_stPoint2F>& vecPath)
{
	if (!m_pSceneMap)
		return false;

	return m_clPathFinder.findPath(start, end, vecPath);
}

//-------------------------------------------------------------
//------------------------------ 
bool CGameSceneCell::findPath(const _stPoint2F& start,const _stPoint2F& end,std::list<_stPoint2F>& listPath)
{
	if (!m_pSceneMap)
		return false;

	return m_clPathFinder.findPath(start, end, listPath);
}

bool CGameSceneCell::findPath(const _stPoint2F& start,const _stPoint2F& end,_stPoint2F& stNextPoint)
{
	if (!m_pSceneMap)
		return false;

	return m_clPathFinder.findPathNextPoint(start, end, stNextPoint);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::randGetPoint		(_stPoint2F&stOut,float32 fCenterX,float32 fCenterY,float32 fRadius)
{
	float fDir = getRandDir();
	uint32 uCount = MAX((uint32)fRadius/10,(uint32)10);
	for (uint32 i = 0; i <= uCount; ++i)
	{
		_stPoint2F stStart;
		stStart.x	= fCenterX;
		stStart.y	= fCenterY;
		stOut		= stStart;
		float fDistance = (float) (randRange(int32(fRadius*100), int32(0)) / 100.0);
		if (getTargetPoint(stStart,fDir,fDistance,stOut))
			return true;
	}
	return false;
}
//-------------------------------------------------------------
//------------------------------ 矩形中随机N个小矩形
bool	CGameSceneCell::randGetPoint(stl_vector<_stPoint2F>& vecOut,_stPoint2F fCenter,float32 fRandDis,float32 fRadius,uint32 uOutNum)
{
	if (isFloatEqual(fRadius,0.0f))
		return false;

	vecOut.clear();

	float32 fBeginX = MAX(fCenter.x - fRandDis,0.0f);
	float32 fBeginY = MAX(fCenter.y - fRandDis,0.0f);
	float32 fEndX = MAX(fCenter.x + fRandDis,0.0f);
	float32 fEndY =  MAX(fCenter.y + fRandDis,0.0f);

	stl_vector<_stPoint2F> vecAll;
	uint32 uNumX = uint32((fEndX - fBeginX )/ (fRadius * 2));
	uint32 uNumY = uint32((fEndY - fBeginY )/ (fRadius * 2));
	for (uint32 i = 0; i < uNumX; ++i)
	{
		for (uint32 j = 0; j < uNumY; ++j)
		{
			fCenter.x = fBeginX + i * fRadius * 2 + fRadius;
			fCenter.y = fBeginY +j * fRadius * 2+ fRadius;
			if (canMove(fCenter.x,fCenter.y))
				vecAll.push_back(fCenter);
		}
	}

	//获取随机个数
	::getRandVecDiff(vecAll,vecOut,uOutNum);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::getTargetPoint		(const _stPoint2F& start,float32 fdir,float32 fdis,_stPoint2F& target,_stFindPathMark* pMark)
{
	return m_clPathFinder.getTargetPoint(start,fdir,fdis,target,pMark);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::getTargetPoint(const _stPoint2F& start,const _stPoint2F& end,_stPoint2F& target,_stFindPathMark* pMark)
{
	return m_clPathFinder.getTargetPoint(start,end,target,pMark);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::isPassable(const _stPoint2F& start,const _stPoint2F& end,_stFindPathMark* pMark)
{
	return m_clPathFinder.isPassable(start,end,pMark);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::getSpawnPoint(_stPoint2F&stPoint,uint16 uRegionID)
{
	if ( !m_pSceneMap )
		return false;

	stPoint.zero();
	CGameScene::MAP_RGN_GRID_LIST::iterator _pos = m_pSceneMap->m_mapRgnGridList.find(uRegionID);
	if ( _pos == m_pSceneMap->m_mapRgnGridList.end() )
		_pos = m_pSceneMap->m_mapRgnGridList.begin();

	if (_pos==m_pSceneMap->m_mapRgnGridList.end())
		return false;

	CGameScene::VEC_GRID_LIST& vecGridList = _pos->second;
	if ( vecGridList.empty() )
		return false;

	const uint16 uTestCount = 10;
	for ( uint16 i = 0 ; i < uTestCount ; ++i )
	{
		uint32 nRandIndex = randRange( (uint32)vecGridList.size() - 1,(uint32)0 );
		if ( nRandIndex >= (uint32)vecGridList.size() )
			nRandIndex = 0;

		stPoint = vecGridList[nRandIndex];

		if ( canMove(stPoint.x,stPoint.y) )
			break;
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::callback_killmonster(CSceneMonster* pMonster,CScenePlayer* pPlayer)
{
	if ( m_pGameCopymap )
	{
		m_pGameCopymap->callback_killmonster(pMonster,pPlayer);
	}
}

//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::callback_depotGoods(CSceneMonster* pMonster,CScenePlayer* pPlayer)
{
	if ( m_pGameCopymap )
	{
		m_pGameCopymap->callback_depotGoods(pMonster,pPlayer);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::arrestmonster(CSceneUnit* pTarget,CScenePlayer* pPlayer)
{
	if (!pTarget || !pPlayer)
		return;

	leaveScene(pTarget);
	if (m_pGameCopymap && pTarget->asMonster())
		m_pGameCopymap->callback_arrestmonster(pTarget->asMonster(),pPlayer);
}
//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::thugmonster(CSceneUnit* pTarget,CScenePlayer* pPlayer)
{
	if (!pTarget || !pPlayer)
		return;

	if (m_pGameCopymap && pTarget->asMonster())
		m_pGameCopymap->callback_thugmonster(pTarget->asMonster(),pPlayer);
}

//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::npcdialog(uint32 uUnitID,CScenePlayer* pPlayer)
{
	if (!pPlayer)
		return;

	CSceneUnit* pUnit = findObject(uUnitID);
	if (!pUnit || !pUnit->asMonster())
		return;

	const _stMonsterModel* pModel = pUnit->asMonster()->getMonsterModel();
	if (!pModel || !pModel->check_mark(MonsterMark_DialogNpc))
		return;

	if (m_pGameCopymap)
		m_pGameCopymap->callback_npcdialog_complete(pPlayer,pUnit->asMonster());
}
//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::callback_playerdead(CScenePlayer* pPlayer,CSceneUnit* pSource)
{
	if ( !pPlayer )
		return ;

	if (m_pGameCopymap)
	{
		m_pGameCopymap->callback_playerdead(pPlayer,pSource);

		if (pSource->asPlayer())
		{
			m_pGameCopymap->callback_killplayer(pSource->asPlayer(),pPlayer);
		}
	}
}
//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::callback_playerlife(CScenePlayer* pPlayer,CScenePlayer* pSource)
{
	if ( !pPlayer )
		return ;

	if (m_pGameCopymap)
		m_pGameCopymap->callback_playerlife(pPlayer,pSource);
}
//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::callback_flyout(CScenePlayer* pPlayer)
{
	if ( !pPlayer )
		return ;

	if (m_pGameCopymap)
		m_pGameCopymap->callback_flyout(pPlayer);
}

//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::callback_player_gethurt(CScenePlayer* pPlayer, uint32 uHurt, CScenePlayer* pPlayerSource)
{
	if ( !pPlayer )
		return ;
	if (m_pGameCopymap)
		m_pGameCopymap->callback_player_gethurt(pPlayer, uHurt, pPlayerSource);

}
//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::addArea	(uint32	uAreaID)
{
	if (!m_pSceneMap)
		return ;

	const _stSceneArea* pArea = m_pSceneMap->findArea(uAreaID);
	if (!pArea)
		return ;

	m_mapAddAniArea[uAreaID]= getSystemTick64() + pArea->uAddDelayTime;

	PBS2CSceneAreaList msg;
	msg.add_areaid(uAreaID);
	broadcastMsg(P_S2C_Protocol_Common,S2C_Common_Area_Add,&msg);
}
//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::delArea	(uint32	uAreaID)
{
	const _stSceneArea* pArea = m_pSceneMap->findArea(uAreaID);
	if (!pArea)
		return ;

	if (!m_clPathFinder.hasArea(uAreaID))
		return ;

	m_mapDelAniArea[uAreaID]= getSystemTick64() + pArea->uDelDelayTime;

	PBS2CSceneAreaList msg;
	msg.add_areaid(uAreaID);
	broadcastMsg(P_S2C_Protocol_Common,S2C_Common_Area_Del,&msg);
}
//-------------------------------------------------------------
//------------------------------
bool	CGameSceneCell::testRegion		(uint8 uAttrType)const
{
	if ( !m_pSceneMap )
		return false;

	return m_pSceneMap->testRegion(uAttrType);
}

//-------------------------------------------------------------
//------------------------------ 
uint16	CGameSceneCell::getDefaultRegion()
{
	if ( !m_pSceneMap )
		return (uint16)(-1);

	return m_pSceneMap->getDefaultRegion();
}

//-------------------------------------------------------------
//------------------------------
void	CGameSceneCell::broadcastMsg		(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	auto _pos = m_hashPlayer.begin();
	auto _end = m_hashPlayer.end();
	for ( ; _pos != _end ; ++_pos )
	{
		CScenePlayer* pPlayer = _pos->second;
		if (!pPlayer)
			continue;

		pPlayer->sendMsg(uMainProtocol,uProtocol,msg,uEvent);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::processFactionHomeLogic		(uint64 uTick, uint64 uTime, uint32 uSecond, bool bNewDay)
{
	processFactionDrinkWine(uTick,uTime,uSecond,bNewDay);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneCell::processFactionDrinkWine		(uint64 uTick, uint64 uTime, uint32 uSecond, bool bNewDay)
{
	const _stFactionDrinkWineConstantModel& stConstModel = g_clFactionData.getDrinkWineConstant();
	bool bInTime = stConstModel.isInOpenTime(uTime);
	if ( bInTime && !m_bFactionDrinkWine )
	{// 刷出npc
		const _stMonsterModel*pModel = g_clMonsterData.find(stConstModel.uNpcID);
		if (pModel)
		{
			CSceneMonster* pMonster = createMonster(pModel,stConstModel.fPosX,stConstModel.fPosY,stConstModel.fAngleDir< 0.0f ? getRandDir() : getAngleToRadian(stConstModel.fAngleDir), 0, true);
			if (pMonster)
				m_uFactionDrinkWineNpc = pMonster->getUnitID();
		}
	}
	else if ( !bInTime && m_bFactionDrinkWine )
	{// 删除npc
		CSceneUnit* pUnit = findObject(m_uFactionDrinkWineNpc);
		if ( pUnit && pUnit->asMonster() && pUnit->asMonster()->getMonsterID() == stConstModel.uNpcID ) 
			leaveScene(pUnit);
	}

	m_bFactionDrinkWine = bInTime;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::inFactionDrinkWineTime			()
{
	return m_bFactionDrinkWine;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneCell::isFactionHome			()
{
	return getSceneID() == g_clConstantData.Faction_HomeSceneID;
}
