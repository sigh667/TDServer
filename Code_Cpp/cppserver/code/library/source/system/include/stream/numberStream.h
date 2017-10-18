/*----------------- numberStream.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/8/20 17:37:54
*--------------------------------------------------------------
*��ˮ�Ų���
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "basic/timeFunction.h"
#include "stackChecker/stackChecker.h"
#include <time.h>
/*************************************************************/
class CNumberStream
{
protected:
	/*************************************************************/
#pragma pack(push,1)
	union _U_NUMBER
	{
		struct
		{
			uint16	_order;	//���
			uint32	_time;	//ʱ��
			uint16	_flag;	//���ID[����С��32767��15λ]
		}_STREAM;

		uint64		_stream;/*��ˮ��*/ 
	};
#pragma pack(pop)
	/*************************************************************/
protected:
	_U_NUMBER	m_uNumber;
	uint32		m_uMaxTime;			/*ʱ��*/ 

public:
	CNumberStream(uint16 _flag = 0)
	{
		FUN_STACK_CHECK();

		m_uMaxTime			= 0;
		m_uNumber._stream	= 0;
		setFlag(_flag);
	}
	virtual~CNumberStream(){}

public:
	inline void		setFlag			(uint16 _flag)
	{
		FUN_STACK_CHECK();

		m_uNumber._STREAM._flag	= _flag;
	}
public:
	inline uint64	lastStreamNumber	()
	{
		FUN_STACK_CHECK();

		return m_uNumber._stream;
	}
	/*----->{ �����¼���ˮ�� }*/ 
	inline uint64	buildStreamNumber		(uint16 _flag = 0)
	{
		FUN_STACK_CHECK();

		/*���ñ�־*/ 
		if(_flag)
			setFlag(_flag);

		m_uNumber._STREAM._time = (uint32)getTime();
		/*����ʱ��ı��־*/ 
		if(m_uMaxTime > m_uNumber._STREAM._time)
		{
			m_uNumber._STREAM._flag|= 0x8000;
		}
		else
		{
			m_uMaxTime = m_uNumber._STREAM._time;
			m_uNumber._STREAM._flag&= 0x7FFF;
		}

		/*���*/ 
		m_uNumber._STREAM._order++;

		return m_uNumber._stream;
	}
};
