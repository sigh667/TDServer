/*------------- mathFunctions.h
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
//############################## �Ƕ�/����
//#############################################################
//--- �ж����������Ƿ����
extern bool	isFloatEqual(float32 fFirst,float32 fSecond);
//--- ���PI�ڵĻ���
extern float32	getPiRadian(float32 fRadian);
//--- ���2PI�ڵĻ���
extern float32	get2PiRadian(float32 fRadian);
//--- ���PI�ڵĻ���
extern float32	getPiRadian	(float32 x,float32 y);
//--- ���2PI�ڵĻ���
extern float32	get2PiRadian(float32 x,float32 y);

//--- ��ýǶ�
extern float32	get2PiAngle	(float32 fAngle);
//--- ��ýǶ�
extern float32	get2PiAngle	(float32 m,float32 n);

//--- ����תΪ�Ƕ�
extern float32	getRadianToAngle	(float32 fRadian);
//--- �Ƕ�תΪ����
extern float32	getAngleToRadian	(float32 fAngle);

//--- �жϻ����Ƿ��ڻ��ȷ�Χ��
extern bool		inRadianRange	(float32 f2PiRadian,float32 fRange,float32 fRadian);
//--- �жϽǶ��Ƿ��ڽǶȷ�Χ��
extern bool		inAngleRange	(float32 f2PiAngle,float32 fRange,float32 fAngle);

//--- ��������ĽǶ� start��end�ĽǶ�
extern float32	get2PiAngle	(const _stPoint2F& start,const _stPoint2F& end);
//--- ��������Ļ���
extern float32	get2PiRadian(const _stPoint2F& start,const _stPoint2F& end);
//#############################################################
//############################## ��
//#############################################################
//------------------------------ 
//--- ����������
extern float32	getDistance(const _stPoint2F& a,const _stPoint2F& b);

//--- ��õ㵽�ߵľ���(a��b���߶ε������˵㣬 c�Ǽ���)
extern float32	getPointToLineDistance(const _stPoint2F& a,const _stPoint2F& b,const _stPoint2F& p);

//--- �Ƿ��ڷ�Χ��
extern bool		inRange(const _stPoint2F& a,const _stPoint2F& b,float32 iRange);
//--- ����Ŀ���(����1:��ʼ��;����2:���򣨻��ȣ�;����3:����;����4:Ŀ���)
extern void		getTargetPoint(const _stPoint2F& start,float32 fdir,float32 fdis,_stPoint2F& target);
//--- ����������
extern float32	getRandDir();
//--- ����������
extern void		getRandPosition(_stPoint2F&stOut,float32 fCenterX,float32 fCenterY,float32 fRadius);

