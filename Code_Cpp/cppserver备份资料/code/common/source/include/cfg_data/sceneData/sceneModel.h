/*----------------- sceneModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����Դ
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
//------------------------------ ����������Ϣ
struct _stSceneGrid
{
	stl_vector<uint8>	vecRegionID;	// ���������б�

	_stSceneGrid&		getSceneGrid	()	{	return *this;						}
	void				initSceneGrid()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ �����¼�
struct _stRegionEvent
{
	uint16				uRegionID;		// ����ID
	uint8				uTriggerType;	//�������� _emRegionTriggerType
	uint8 				uEventType;		//�¼����� _emRegionEventType
	stl_vector<int32>	vecParamlist;	//�����б�

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
//------------------------------ �¼���������
typedef stl_vector<_stRegionEvent>	VECTOR_REGION_EVENT;
struct _stGameRegion
{
	uint16				uRegionID;	// ����ID
	uint64				uMark;		// _emRegionMark ��������
	VECTOR_REGION_EVENT	vecEvent;	// �¼��б�
	float32				fCenterX;	// ���ĵ�����x
	float32				fCenterY;	// ���ĵ�����y

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
//------------------------------ ˢ�����ͺ�����
struct _stMonsterCount
{
	uint32		uMonsterID;		// ��������
	uint32 		uCount;			// ����
	uint32 		uRouteID;		// ·��id
	uint32		uAreaID;		// ���ڵķ����
	float32		fPosX;			// ��ұ��������ڵ�λ����������x- 
	float32		fPosY;			// ��ұ��������ڵ�λ����������y- 

};
//-------------------------------------------------------------
//------------------------------ ˢ��������Ϣ
typedef stl_vector<_stMonsterCount>	VECTOR_MONSTER_COUNT;
struct _stMonsterRegion
{
	uint8					uRefreshType;	// ˢ������ _emMonsterRefreshType
	uint32					uIntervalSecond;// ���ʱ��(��)
	float32					fPosX;			// ���ĵ�����x
	float32 				fPosY;			// ���ĵ�����y
	float32 				fRadius;		// �뾶
	float32 				fDir;			// ���򣨻��� ���������� �������
	VECTOR_MONSTER_COUNT	vecMonster;		// �����б�
	stl_vector<_stDateTime>	vecFixedTime;	// �̶�ʱ��

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
//------------------------------ �ر���Ϣ
struct _stSceneGround
{
	std::vector<float>		vecVerts;				//�����б�
	std::vector<int>		vecTris;				//�����α�

	_stSceneGround&	getSceneGround	()						{	return *this;					}
	void			initSceneGround	()
	{
		vecVerts.clear();
		vecTris.clear();
	}
};
//-------------------------------------------------------------
//------------------------------ �����������ݣ���״�壩
struct _stSceneArea
{
	uint32				uAreaID;		// ����ID
	uint32				uType;			// _emSceneAreaType ��������
	bool				isDynamic;		// �Ƿ�̬����
	std::vector<float>	vecVerts;		// �����б�
	float32				fHeight;		// ��״��߶�
	uint32				uAddDelayTime;	// ��������ʱ�䣨���룩
	uint32				uDelDelayTime;	// �رն���ʱ�䣨���룩
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
//------------------------------ ·��
struct _stSceneRout	
{
	uint32					uRoutID;	// ·��ID
	uint32					uType;		// _emRouteType ·������
	std::vector<_stPoint2F>	vecPoint;	// ���б�
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
//------------------------------ �Ṧ·��
struct  _stSceneQinggongPoint
{
	float32								fX;					// x
	float32 							fY;					// y
	uint32								uType;  			// �������������Ṧ�������� _emQinggongActionType
	uint32								uSpeed;  			// �������������ٶ�
	uint32								uActionTimeMS;		// ��λ�����������������һ����Ҫ�õ���ʱ��,�����ٶ�������,���س�����ʱ�����
};
struct _stSceneQinggongPath
{
	uint32								uQinggongPathID;	//�Ṧ·��id
	std::vector<_stSceneQinggongPoint>	vecPoint;			//�Ṧ·���㼯��
};
typedef stl_map<uint32,_stSceneQinggongPath> MAP_SCENE_QINGGOING_ROUTE;

