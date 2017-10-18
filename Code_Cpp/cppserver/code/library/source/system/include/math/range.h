/*------------- range.h
* Copyright (C): 2014 Mokylin¡¤Mokyqi
* Author       : ÁõÀ×
* Version      : V1.01
* Date         : 2014/22/5 10:27
*
*/ 
/***************************************************************
* ·¶Î§
***************************************************************/
#pragma once
#include "basic/basicTypes.h"
#include <math.h>
/*************************************************************/
#pragma pack(push,1)
//##############################################################################
//-------------------------------------------------------------
//------------------------------ ·¶Î§Çø¼ä
template <typename T>
struct _stRange
{
public:
	T	_min;
	T	_max;

public:
	_stRange&	getRange()							{	return *this;			}

public:
	inline void	zero	()							{	_min = _max = T(0.0f);	}
	inline void	setRange(const T&_min,const T&_max)
	{
		this->_min = _min;
		this->_max = _max;
	}
	//--- (±ÕÇø¼ä)
	inline bool	isRange	(const T&_value) const
	{
		if(_min > _value)
			return false;

		if(_max != T(0.0f) && _max < _value)
			return false;

		return true;
	}
};
//-------------------------------------------------------------
typedef _stRange<uint8>		_stRangeU8;
typedef _stRange<uint16>	_stRangeU16;
typedef _stRange<uint32>	_stRangeU32;
//##############################################################################
#pragma pack(pop)
