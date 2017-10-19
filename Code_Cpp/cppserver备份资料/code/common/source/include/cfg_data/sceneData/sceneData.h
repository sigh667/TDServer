/*----------------- sceneData.h
*
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/16 11:15:34
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "def_struct/gameDefine.h"
#include "sceneModel.h"
#include <vector>
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 格子的所属区域信息
typedef stl_vector<const _stGameRegion*>	VECTOR_GAME_REGION;
struct _stGridRegionInfo
{
	VECTOR_GAME_REGION	vecRegion;	// 区域列表
	_stGridRegionInfo&	getGridRegionInfo	()				{	return *this;					}
	void				initGridRegionInfo	()
	{
		vecRegion.clear();	
	}

	bool	testMark	(uint8 uType)const;
	uint16	testMarkReturnID	(uint8 uType)const;
};
//-------------------------------------------------------------
//------------------------------ 
class CSceneData	: public CFileData
{
protected:
	enum
	{
		Version		=		2011092201
	};
// 地图格子信息(每个格子对应一个事件区域列表)
protected:
	uint16					m_uSceneWide;			//地图宽
	uint16					m_uSceneHigh;			//地图高
	uint16					m_uGridWide;			//格子宽
	uint16					m_uGridHigh;			//格子高
	uint16					m_uGridWideSize;		//格子横向数量(m_uSceneWide / m_uGridWide)
	uint16					m_uGridHighSize;		//格子竖向数量(m_uSceneHigh / m_uGridHigh)
//格子信息列表
protected:
	typedef stl_vector<_stGridRegionInfo*>	VEC_GRID_RGN;
	typedef stl_vector<_stPoint2F>			VEC_GRID_LIST;
	typedef stl_map<uint16,VEC_GRID_LIST>	MAP_RGN_GRID_LIST;
	_stGridRegionInfo**		m_pGridRegion;			//场景格子的区域信息
	_stGridRegionInfo		m_stDefaultGridRegion;	//场景默认格子的区域信息
	VEC_GRID_RGN			m_vecGridRegion;		//有着完全不同的区域信息的格子列表
	MAP_RGN_GRID_LIST		m_mapRgnGridList;		//区域中包含的格子信息
//事件区域列表
protected:
	MAP_REGION				m_mapRegion;			//事件区域信息
	_stGameRegion			m_stDefaultRegion;		//默认区域信息
//地表信息
protected:
	_stSceneGround			m_stGround;				//地表
//刷怪区域信息
public:
	VECTOR_MONSTER_REGION	m_vecMonsterRegion;		// 怪物刷新信息
//特殊区域信息
public:
	MAP_SCENE_AREA			m_mapArea;				// 特殊区域信息
//路径信息
public:
	MAP_SCENE_ROUT			m_mapRout;				// 路径信息
//轻功信息
public:
	MAP_SCENE_QINGGOING_ROUTE m_mapQinggongRoute;	// 轻功信息
public:
	CSceneData();
	virtual~CSceneData();
protected:
	virtual pc_str	getFileName			()const		{	return "";				}
	virtual pc_str	getXmlName			()const		{	return "";				}
protected:
	void			free				();

public:
	inline uint16	getSceneWide		()const		{	return m_uSceneWide;	}
	inline uint16	getSceneHigh		()const		{	return m_uSceneHigh;	}
	inline uint16	getSceneGridWide	()const		{	return m_uGridWide;		}
	inline uint16	getSceneGridHigh	()const		{	return m_uGridHigh;		}
	inline uint16	getSceneSizeWide	()const		{	return m_uGridWideSize;	}
	inline uint16	getSceneSizeHigh	()const		{	return m_uGridHighSize;	}
	inline _stSceneGround&	getGround	()			{	return m_stGround;		}
public:
	const _stGameRegion*	findGameRegion		(uint16 uRegionID)	const;
	const _stSceneArea*		findArea			(uint32 uAreaID)	const;
	const _stSceneRout*		findRout			(uint32 uRoutID)	const;
	const _stPoint2F*		findRoutePoint		(uint32 uRoutID,uint32& uIndex)const;
	bool					findQinggongPoint	(uint32 uQinggongPathID, uint32 uTimeUsed, uint32& uNextIndex, uint32& uMs2NextPoint, float32& fNowX, float32& fNowY);
	_stGridRegionInfo*		findGridRegion		(float32 x,float32 y);
	_stGridRegionInfo*		findGridRegion		(const _stPoint2F&stPoint);
	bool					testRegion			(uint8 uAttrType)const;
	uint16					getDefaultRegion	();
private:
	// 查找已经有相同局域信息的格子
	_stGridRegionInfo*	findSameGridRegion	(stl_vector<uint16>& vecRegionID);
	// 增加一个新的格子的区域信息
	bool				genGridRegion		(uint16 uGridX,uint16 uGridY,uint32 uIndex,stl_vector<uint16>& vecRegionID);
	// 清除所有格子的区域信息
	void				clearGridRegion		();
protected:
	virtual bool	onLoad				(CFileStream&clFile);
	virtual bool	loadFile			(pc_str pFileName);
};
