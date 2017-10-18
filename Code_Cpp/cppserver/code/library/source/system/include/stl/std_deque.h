/*----------------- std_deque.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/7/11 22:37:14
*--------------------------------------------------------------
*˫�˶������������÷ֿ�����Խ�����洢����,���и�Ч��ɾ����βԪ�صĺ���
*-�ɲ��������������ķ�ʽ���б�������
*---
- push_back()
- push_front()
- iterator insert(iterator pos, const T& x)
- pop_front()	ɾ��deque�ĵ�һ��Ԫ��
- pop_back()	ɾ��deque�����һ��Ԫ��
- swap(deque&)	deque�Ľ�����
*************
vector deque ��ʹ�õ�����������ɾ������ʱ������ʹ���ѱ����end()
*------------------------------------------------------------*/
#pragma once
#include <deque>
#include "basic/basicTypes.h"
/*************************************************************/
// #pragma pack(push,1)
//------------------------------------------------------
template<class _Ty>
class stl_deque	: public std::deque<_Ty>
{
public:
	typedef std::deque<_Ty>	Parent;

public:
};
//------------------------------------------------------
// #pragma pack(pop)
