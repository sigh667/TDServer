/*----------------- sceneData.h
*
* Copyright (C): 2011  
* Author       : ����Դ
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
//------------------------------ ���ӵ�����������Ϣ
typedef stl_vector<const _stGameRegion*>	VECTOR_GAME_REGION;
struct _stGridRegionInfo
{
	VECTOR_GAME_REGION	vecRegion;	// �����б�
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
// ��ͼ������Ϣ(ÿ�����Ӷ�Ӧһ���¼������б�)
protected:
	uint16					m_uSceneWide;			//��ͼ��
	uint16					m_uSceneHigh;			//��ͼ��
	uint16					m_uGridWide;			//���ӿ�
	uint16					m_uGridHigh;			//���Ӹ�
	uint16					m_uGridWideSize;		//���Ӻ�������(m_uSceneWide / m_uGridWide)
	uint16					m_uGridHighSize;		//������������(m_uSceneHigh / m_uGridHigh)
//������Ϣ�б�
protected:
	typedef stl_vector<_stGridRegionInfo*>	VEC_GRID_RGN;
	typedef stl_vector<_stPoint2F>			VEC_GRID_LIST;
	typedef stl_map<uint16,VEC_GRID_LIST>	MAP_RGN_GRID_LIST;
	_stGridRegionInfo**		m_pGridRegion;			//�������ӵ�������Ϣ
	_stGridRegionInfo		m_stDefaultGridRegion;	//����Ĭ�ϸ��ӵ�������Ϣ
	VEC_GRID_RGN			m_vecGridRegion;		//������ȫ��ͬ��������Ϣ�ĸ����б�
	MAP_RGN_GRID_LIST		m_mapRgnGridList;		//�����а����ĸ�����Ϣ
//�¼������б�
protected:
	MAP_REGION				m_mapRegion;			//�¼�������Ϣ
	_stGameRegion			m_stDefaultRegion;		//Ĭ��������Ϣ
//�ر���Ϣ
protected:
	_stSceneGround			m_stGround;				//�ر�
//ˢ��������Ϣ
public:
	VECTOR_MONSTER_REGION	m_vecMonsterRegion;		// ����ˢ����Ϣ
//����������Ϣ
public:
	MAP_SCENE_AREA			m_mapArea;				// ����������Ϣ
//·����Ϣ
public:
	MAP_SCENE_ROUT			m_mapRout;				// ·����Ϣ
//�Ṧ��Ϣ
public:
	MAP_SCENE_QINGGOING_ROUTE m_mapQinggongRoute;	// �Ṧ��Ϣ
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
	// �����Ѿ�����ͬ������Ϣ�ĸ���
	_stGridRegionInfo*	findSameGridRegion	(stl_vector<uint16>& vecRegionID);
	// ����һ���µĸ��ӵ�������Ϣ
	bool				genGridRegion		(uint16 uGridX,uint16 uGridY,uint32 uIndex,stl_vector<uint16>& vecRegionID);
	// ������и��ӵ�������Ϣ
	void				clearGridRegion		();
protected:
	virtual bool	onLoad				(CFileStream&clFile);
	virtual bool	loadFile			(pc_str pFileName);
};
