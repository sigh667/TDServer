/*------------- mathFunctions.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/5/3 17:28:24
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "basic/basicTypes.h"
#include "point2.h"
#include <math.h>
/*************************************************************/
//#############################################################
//############################## 角度/弧度
//#############################################################
//--- 判断两个浮点是否相等
extern bool	isFloatEqual(float32 fFirst,float32 fSecond);
//--- 获得PI内的弧度
extern float32	getPiRadian(float32 fRadian);
//--- 获得2PI内的弧度
extern float32	get2PiRadian(float32 fRadian);
//--- 获得PI内的弧度
extern float32	getPiRadian	(float32 x,float32 y);
//--- 获得2PI内的弧度
extern float32	get2PiRadian(float32 x,float32 y);

//--- 获得角度
extern float32	get2PiAngle	(float32 fAngle);
//--- 获得角度
extern float32	get2PiAngle	(float32 m,float32 n);

//--- 弧度转为角度
extern float32	getRadianToAngle	(float32 fRadian);
//--- 角度转为弧度
extern float32	getAngleToRadian	(float32 fAngle);

//--- 判断弧度是否在弧度范围内
extern bool		inRadianRange	(float32 f2PiRadian,float32 fRange,float32 fRadian);
//--- 判断角度是否在角度范围内
extern bool		inAngleRange	(float32 f2PiAngle,float32 fRange,float32 fAngle);

//--- 获得两点间的角度 start到end的角度
extern float32	get2PiAngle	(const _stPoint2F& start,const _stPoint2F& end);
//--- 获得两点间的弧度
extern float32	get2PiRadian(const _stPoint2F& start,const _stPoint2F& end);
//#############################################################
//############################## 点
//#############################################################
//------------------------------ 
//--- 获得两点距离
extern float32	getDistance(const _stPoint2F& a,const _stPoint2F& b);

//--- 获得点到线的距离(a和b是线段的两个端点， c是检测点)
extern float32	getPointToLineDistance(const _stPoint2F& a,const _stPoint2F& b,const _stPoint2F& p);

//--- 是否在范围内
extern bool		inRange(const _stPoint2F& a,const _stPoint2F& b,float32 iRange);
//--- 计算目标点(参数1:起始点;参数2:方向（弧度）;参数3:距离;参数4:目标点)
extern void		getTargetPoint(const _stPoint2F& start,float32 fdir,float32 fdis,_stPoint2F& target);
//--- 获得随机方向
extern float32	getRandDir();
//--- 获得随机坐标
extern void		getRandPosition(_stPoint2F&stOut,float32 fCenterX,float32 fCenterY,float32 fRadius);

