/*----------------- pathFinder.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2016/2/2 11:14:45
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/stlDefine.h"
#include "math/point2.h"
#include "DetourNavMeshQuery.h"
#include <list>
#include <vector>
#include "protocol/sceneStruct.pb.h"
/*************************************************************/
struct _stSceneArea;
struct _stSceneGround;
//-------------------------------------------------------------
//------------------------------ 
class dtNavMesh;
struct _stFindPathMark
{
	uint8 uIncludeFlag; // _emSceneAreaType
	uint8 uExcludeFlag; // _emSceneAreaType

	_stFindPathMark()
	{
		uIncludeFlag = -1;

		uExcludeFlag = -1;
		_SET_BIT32(uExcludeFlag,SceneAreaType_Groud,false);
	}
	inline	void	setIncludeFlag		(uint8 uAreaType)
	{
		_SET_BIT32(uIncludeFlag,uAreaType,true);
	}

	inline	void	setExcludeFlag		(uint8 uAreaType)
	{
		_SET_BIT32(uExcludeFlag,uAreaType,true);
	}
};
class CPathFinder
{
	dtNavMesh*				m_navMesh;
	dtNavMeshQuery*			m_navQuery;
public:
	typedef stl_vector<_stSceneArea> VEC_SCENE_AREA;

	_stSceneGround*			m_pGround;
	VEC_SCENE_AREA			m_vecArea;		// ��Ϊ���Ⱥ�������� ������ vector��
private:
	static const int		MAX_POLYS		= 256;
	static const int		MAX_PATH_NUM	= 256;
	float					m_polyPickExt[3];
	dtQueryFilter			m_filter;
	float					m_spos[3];
	float					m_epos[3];
	float					m_nearestPt[3];
	dtPolyRef				m_startRef;
	dtPolyRef				m_endRef;
	dtPolyRef				m_polys[MAX_POLYS];
	int						m_npolys;
	float					m_straightPath[MAX_PATH_NUM*3];
	int						m_nstraightPath;

	float					m_hitPos[3];
	float					m_hitNormal[3];
	bool					m_hitResult;
public:
	CPathFinder();
	~CPathFinder();
public:
	bool	initialize		();

	// ���õر���Ϣ
	void	setGround		(_stSceneGround* pGround);

	// ������������
	bool	addArea			(const _stSceneArea* pArea,bool bRebuild);

	// ɾ����������
	bool	delArea			(uint32	uAreaID,bool bRebuild);

	bool	hasArea			(uint32	uAreaID);

	// ��������ϵͳ
	bool	build			();
public:
	// Ѱ·
	bool	findPath		(const _stPoint2F& start,const _stPoint2F& end,std::vector<_stPoint2F>& vecPath,_stFindPathMark* pMark = NULL);
	bool	findPath		(const _stPoint2F& start,const _stPoint2F& end,std::list<_stPoint2F>& listPath,_stFindPathMark* pMark = NULL);
	bool	findPathNextPoint(const _stPoint2F& start,const _stPoint2F& end,_stPoint2F& stTargetPoint,_stFindPathMark* pMark = NULL);


	// ���λ��
	bool	findRandomPoint	(_stPoint2F&stOut,float32 fCenterX,float32 fCenterY,float32 fRadius,uint32 uTryTime,_stFindPathMark* pMark = NULL);
	
	// �ҵ���ͨ������Զֱ�ߵ�
	bool	getTargetPoint	(const _stPoint2F& start,float32 fdir,float32 fdis,_stPoint2F& target,_stFindPathMark* pMark = NULL);

	// �ҵ���ͨ������Զֱ�ߵ�
	bool	getTargetPoint	(const _stPoint2F& start,const _stPoint2F& end,_stPoint2F& target,_stFindPathMark* pMark = NULL);

	// ����֮���Ƿ� ֱ�߿�ͨ
	bool	isPassable		(const _stPoint2F& start,const _stPoint2F& end,_stFindPathMark* pMark = NULL);

	bool	isMoveable		(const _stPoint2F& start,_stFindPathMark* pMark = NULL);

	//�漴λ��
	bool	findRandomPoint	(_stPoint2F&stOut,_stFindPathMark* pMark = NULL);
private:
	bool	findPath		(const _stPoint2F& start,const _stPoint2F& end,_stFindPathMark* pMark);

	void	setFilter		(_stFindPathMark* pMark);

	bool	setStart		(float32 fX,float32 fY);
	bool	setEnd			(float32 fX,float32 fY);
};
