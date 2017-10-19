/*----------------- sceneModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/1/23 13:01:02
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "timer/dateTime.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 场景格子信息
struct _stSceneGrid
{
	stl_vector<uint8>	vecRegionID;	// 所属区域列表

	_stSceneGrid&		getSceneGrid	()	{	return *this;						}
	void				initSceneGrid()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 区域事件
struct _stRegionEvent
{
	uint16				uRegionID;		// 区域ID
	uint8				uTriggerType;	//触发类型 _emRegionTriggerType
	uint8 				uEventType;		//事件类型 _emRegionEventType
	stl_vector<int32>	vecParamlist;	//参数列表

	void				initRegionEvent	()	
	{	
		uRegionID		= 0;
		uTriggerType	= 0;
		uEventType		= 0;
		vecParamlist.clear();
	}
	_stRegionEvent&		getRegionEvent	()	
	{	
		return *this;					
	}
};
//-------------------------------------------------------------
//------------------------------ 事件区域数据
typedef stl_vector<_stRegionEvent>	VECTOR_REGION_EVENT;
struct _stGameRegion
{
	uint16				uRegionID;	// 区域ID
	uint64				uMark;		// _emRegionMark 区域属性
	VECTOR_REGION_EVENT	vecEvent;	// 事件列表
	float32				fCenterX;	// 中心点坐标x
	float32				fCenterY;	// 中心点坐标y

	_stGameRegion&	getGameRegion	()						{	return *this;					}
	void			initGameRegion	()
	{
		uRegionID = 0; 
		uMark = 0;
		vecEvent.clear();	
	}
	inline bool		check_mark		(uint8 _mark)	const	{	return _CHECK_BIT(uMark,_BIT64(_mark));	}
	inline void		set_mark		(uint8 _mark,bool _set)	{	_SET_BIT64(uMark,_mark,_set);			}
};
typedef stl_map<uint16,_stGameRegion> MAP_REGION;
//-------------------------------------------------------------
//------------------------------ 刷怪类型和数量
struct _stMonsterCount
{
	uint32		uMonsterID;		// 怪物类型
	uint32 		uCount;			// 数量
	uint32 		uRouteID;		// 路径id
	uint32		uAreaID;		// 所在的封闭区
	float32		fPosX;			// 玩家被拉后所在的位置中心坐标x- 
	float32		fPosY;			// 玩家被拉后所在的位置中心坐标y- 

};
//-------------------------------------------------------------
//------------------------------ 刷怪区域信息
typedef stl_vector<_stMonsterCount>	VECTOR_MONSTER_COUNT;
struct _stMonsterRegion
{
	uint8					uRefreshType;	// 刷新类型 _emMonsterRefreshType
	uint32					uIntervalSecond;// 间隔时间(秒)
	float32					fPosX;			// 中心点坐标x
	float32 				fPosY;			// 中心点坐标y
	float32 				fRadius;		// 半径
	float32 				fDir;			// 朝向（弧度 负数代表是 随机朝向）
	VECTOR_MONSTER_COUNT	vecMonster;		// 怪物列表
	stl_vector<_stDateTime>	vecFixedTime;	// 固定时间

	_stMonsterRegion()
	{
		initMonsterRegion();
	}

	_stMonsterRegion&	getMonsterRegion()						{	return *this;					}
	void			initMonsterRegion	()
	{
		uRefreshType = 0; 
		uIntervalSecond = 0; 
		fPosX = 0.f;
		fPosY = 0.f;
		fRadius = 0.f;
		fDir = 0.f;
		vecMonster.clear();
		vecFixedTime.clear();
	}
};
typedef stl_vector<_stMonsterRegion>		VECTOR_MONSTER_REGION;
//-------------------------------------------------------------
//------------------------------ 地表信息
struct _stSceneGround
{
	std::vector<float>		vecVerts;				//顶点列表
	std::vector<int>		vecTris;				//三角形表

	_stSceneGround&	getSceneGround	()						{	return *this;					}
	void			initSceneGround	()
	{
		vecVerts.clear();
		vecTris.clear();
	}
};
//-------------------------------------------------------------
//------------------------------ 特殊区域数据（柱状体）
struct _stSceneArea
{
	uint32				uAreaID;		// 区域ID
	uint32				uType;			// _emSceneAreaType 区域类型
	bool				isDynamic;		// 是否动态生成
	std::vector<float>	vecVerts;		// 顶点列表
	float32				fHeight;		// 柱状体高度
	uint32				uAddDelayTime;	// 开启动画时间（毫秒）
	uint32				uDelDelayTime;	// 关闭动画时间（毫秒）
	_stSceneArea&	getSceneArea	()						{	return *this;					}
	void			initSceneArea	()
	{
		uAreaID = 0; 
		uType = 0;
		isDynamic = false;
		fHeight = 0.f;
		vecVerts.clear();
		uAddDelayTime = 0;
		uDelDelayTime = 0;
	}
};
typedef stl_map<uint32,_stSceneArea> MAP_SCENE_AREA;
//-------------------------------------------------------------
//------------------------------ 路径
struct _stSceneRout	
{
	uint32					uRoutID;	// 路径ID
	uint32					uType;		// _emRouteType 路径类型
	std::vector<_stPoint2F>	vecPoint;	// 点列表
	_stSceneRout&	getSceneRout	()						{	return *this;					}
	void			initSceneRout	()
	{
		uRoutID = 0; 
		uType = 0;
		vecPoint.clear();
	}
};
typedef stl_map<uint32,_stSceneRout> MAP_SCENE_ROUT;
//-------------------------------------------------------------
//------------------------------ 轻功路径
struct  _stSceneQinggongPoint
{
	float32								fX;					// x
	float32 							fY;					// y
	uint32								uType;  			// 从这个点出发的轻功动作类型 _emQinggongActionType
	uint32								uSpeed;  			// 从这个点出发的速度
	uint32								uActionTimeMS;		// 单位毫秒从这个点出发到下一个点要用掉的时间,根据速度来计算,加载场景的时候计算
};
struct _stSceneQinggongPath
{
	uint32								uQinggongPathID;	//轻功路径id
	std::vector<_stSceneQinggongPoint>	vecPoint;			//轻功路径点集合
};
typedef stl_map<uint32,_stSceneQinggongPath> MAP_SCENE_QINGGOING_ROUTE;

