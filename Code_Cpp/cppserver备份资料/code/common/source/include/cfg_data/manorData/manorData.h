/*----------------- climbTowerData.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : tanghaibo
* Version      : V1.0
* Date         : 2017/06/23 14:47:14
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "manorModel.h"
/*************************************************************/
class CManorData	: public CFileData
{
public:
	enum
	{
		Version	= 20170623
	};

public:

	typedef stl_map<uint32, _stmanorModel> MAP_ID2MANOR;
	MAP_ID2MANOR m_mpID2Manor;

public:

public:
	virtual pc_str			getFileName	()const				{	return "manor.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_manor.xml";			}

public:
	CManorData();

public:
	const MAP_ID2MANOR & getAllManorModel(){return m_mpID2Manor;}
	bool				 canApply		 (uint32 uManorID);//�ж��Ƿ��ڱ����ʱ�䷶Χ��
	bool				 canFight		 (uint32 uManorID);//�ж��Ƿ������ս���Χ��
	bool				 canApply		 (_stmanorModel & model);//�ж��Ƿ��ڱ����ʱ�䷶Χ��
	bool				 canFight		 (_stmanorModel & model);//�ж��Ƿ������ս���Χ��
	uint32				 calcValidFightSeconds (_stmanorModel & model);//����õ��������
	uint32				 calcValidFightSeconds (uint32 uManorID);//����õ��������
	bool				 canCloseSettle	 (_stmanorModel & model);//�ж��Ƿ����ֹͣͶƱ,����
	_stmanorModel*		 findModel		 (uint32 uManorID){return m_mpID2Manor.find_(uManorID);}

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);
private:

};
//-------------------------------------------------------------
extern CManorData	g_clManorData;
