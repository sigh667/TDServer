/*----------------- rect.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/22 14:33:44
*--------------------------------------------------------------
*矩形/区域
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "math/point2.h"
#include <math.h>
/*************************************************************/
#pragma pack(push,1)
//##############################################################################
//-------------------------------------------------------------
//------------------------------ 区域
template <typename T>
struct _stRect
{
public:
	T	pointX;
	T	pointY;
	T	extentX;
	T	extentY;

public:
	_stRect&getRect()								{	return *this;								}

public:
	inline void	zero		()						{	pointX = pointY = 0;extentX = extentY = 0;	}
	inline void	setPoint	(const T&xy)			{	pointX = pointY = xy;						}
	inline void	setPoint	(const T&_x,const T&_y)	{	pointX = _x;pointY = _y;					}
	inline void	setExtent	(const T&xy)			{	extentX = extentY = xy;						}
	inline void	setExtent	(const T&_x,const T&_y)	{	extentX = _x;extentY = _y;					}
	inline bool	included	(const _stPoint2I&_p)
	{
		if(_p.x < pointX || _p.x > extentX)
			return false;

		if(_p.y < pointY || _p.y > extentY)
			return false;

		return true;
	}
	inline bool	included	(const _stPoint2I16&_p)
	{
		if(_p.x < pointX || _p.x > extentX)
			return false;

		if(_p.y < pointY || _p.y > extentY)
			return false;

		return true;
	}
	inline bool	included	(const _stPoint2I8&_p)
	{
		if(_p.x < pointX || _p.x > extentX)
			return false;

		if(_p.y < pointY || _p.y > extentY)
			return false;

		return true;
	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};
//-------------------------------------------------------------
typedef _stRect<int8>	_stRectI8;
typedef _stRect<int16>	_stRectI16;
//##############################################################################
#pragma pack(pop)
