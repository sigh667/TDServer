/*----------------- corpsModel.h
*
* Copyright (C): 2017  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2017/6/9 16:50:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "stream/netStream.h"
#include "def_struct/stlDefine.h"
#include "basic/systemFunction.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stCorpsJobBase
{
	uint32			uCorpsJob;			//����ְλ _emCorpsJob
	uint32			uAddTitle;			//���ӳƺ�ID
	uint32			uAddMountID;		//��������ID
	uint32			uSetCoolTime;		//���õ�ǰְλ��ȴʱ��
	uint32			uForbitSpeakTime;	//����ʱ����
	uint32			uForbitSpeakHourNum;//ÿСʱ���Ե�����

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stCorpsJobModel : _stCorpsJobBase
{
	SET_U32		setJobSet;				// ����Ȩ�޵�ְλ
	SET_U32		setForbitSpeakJob;		// �ܽ��Ե�ְλ

	void initModel()	
	{ 
		initBase();
		setJobSet.clear();
		setForbitSpeakJob.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stCorpsJobBase::read(stream)&&
			stream.read(setJobSet)&&
			stream.read(setForbitSpeakJob);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stCorpsJobBase::write(stream)&&
			stream.write(setJobSet)&&
			stream.write(setForbitSpeakJob);
	}
};
