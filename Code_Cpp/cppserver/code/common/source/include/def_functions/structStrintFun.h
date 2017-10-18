/*----------------- structStrintFun.h
*
* Copyright (C): 2013  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2013/12/12 17:50:32
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "math/range.h"
#include "timer/dateTime.h"
#include "stl/stl_vector.h"
#include "def_struct/stlDefine.h"

struct _stGameAttribute;
struct _stItemBuild;

//--- 时间列表
//[*/年/年-年][*/月/月-月][[*/日-日]/[w周,w周]][时间-时间];[*/年/年-年][*/月/月-月][[*/日-日]/[w周,w周]][时间-时间]
extern	bool	readFromString	(std::string&strString,stl_vector<_stDateTime>&stModel);

//--- 区域范围读取
extern bool		readFromString	(std::string&strString,_stRangeU8 &stModel,char c = ';');
extern bool		readFromString	(std::string&strString,_stRangeU16&stModel,char c = ';');
extern bool		readFromString	(std::string&strString,_stRangeU32&stModel,char c = ';');

extern bool		readFromString	(std::string&strString,_stGameAttribute&stAttr,char c = ';');

extern bool		readFromString	(std::string&strString,_stItemBuild&stItemBuild,char c = ';');
extern bool		readFromString	(std::string&strString,DMAP_UINT8_32_32& dmapPreCondition,char c = ';');
extern bool		readFromString	(std::string&strString,DMAP_NEED_ITEM& dmapNeedItem,char c = ';');
template<class T1,class T2> 
extern bool		readFromString	(std::string&strString,stl_map<T1,T2>& mapData,char c = ';')
{
	while (!strString.empty())
	{
		T1 uType	= (T1)readInt64FromString(strString,'_');
		T2 uValue	= (T2)readInt64FromString(strString,c);
		mapData.insert_(uType,uValue);
	}
	return true;
}