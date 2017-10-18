/*----------------- aiModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2016/12/21 19:14:05
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "stream/streamBase.h"
#include <string>
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------
struct _stAIModelBase
{
	uint32				uID;				//���
	uint32				uAICallFrequency;	//aiִ��Ƶ��(֡��)
	uint8				uSelectTargetType;	//Ŀ��ѡ��ʽ
	uint8				uIdleType;			//�������� _emAIIdleType
	float32				fPatrolRadius;		//Ѳ�߰뾶 _emAISelectTargetType
	uint32				uPatrolTimeMin;		//Ѳ�߼����Сֵ(��)
	uint32				uPatrolTimeMax;		//Ѳ�߼�����ֵ(��)
	float32				fFightRadius;		//����뾶
	float32				fPursueDis;			//׷������
	float32				fHatredRadius;		//��ް뾶
	uint32				uDefaultSkillLevel;	//Ĭ�ϼ��ܵȼ�
	uint32				uDefaultBuffLevel;	//Ĭ��Buff�ȼ�

	inline void				initAIModelBase()	{	memset(this,0,sizeof(*this));			}
	inline _stAIModelBase&	getAIModelBase()	{	return *this;							}
	READ_WRITE_STRUCT
};

/*************************************************************/
#pragma pack(pop)
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stAIModel	: _stAIModelBase
{
	std::string		strBTName;		//��Ϊ������

	_stAIModel&		getAIModel()							{	return *this;							}
	inline void		initAIModel()
	{
		initAIModelBase();
		strBTName.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stAIModelBase::read(stream)&&
			stream.read(strBTName);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stAIModelBase::write(stream)&&
			stream.write(strBTName);
	}
};

//-------------------------------------------------------------
