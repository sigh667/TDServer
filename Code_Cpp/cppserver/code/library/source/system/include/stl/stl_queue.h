/*----------------- stl_queue.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/7/11 22:34:20
*--------------------------------------------------------------
*�������,�Ƚ��ȳ�
-����ʱ�����deque��,����ʱ����deque
*------------------------------------------------------------*/
#pragma once
#include <queue>
#include "basic/basicTypes.h"
/*************************************************************/
// #pragma pack(push,1)
//------------------------------------------------------
template<class _Ty>
class stl_queue	: public std::queue<_Ty>
{
public:
	typedef std::queue<_Ty>	Parent;

public:
};
//------------------------------------------------------
// #pragma pack(pop)
