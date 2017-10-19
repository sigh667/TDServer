/*------------- systemFunction.h
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/2/15 10:59:39
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "stl/stl_vector.h"
#include "stl/std_map.h"
#include "stl/stl_list.h"
#include "basic/basicTypes.h"
#include "stackChecker/stackChecker.h"
/*************************************************************/
//-------------------------------------------------------------
//--- ���ģ���ļ�·����
extern void		getModuleFileName(char*pFilename,uint32 uSize);

//------------------------------------------------------
//--- ���ù���Ŀ¼
extern void		setWorkDirectory(bool bModuleFile = true);

//------------------------------------------------------
//--- ����һ�������
extern int32	randRange(int32 nMax,int32 nMin  = 0);
extern uint32	randRange(uint32 uMax,uint32 uMin  = 0);
//-------------------------------------------------------------
//--- ����������
extern int32	getRandIndex(stl_vector<uint32>&_arr);
//--- ���������ͬ������
template <typename T>
extern bool getMapRandValue(stl_map<T,uint32>& mapData,T& value)
{
	stl_vector<uint32> vecArr;
	typename stl_map<T,uint32>::iterator _pos = mapData.begin();
	for (; _pos != mapData.end(); ++_pos)
		vecArr.push_back(_pos->second);


	if (vecArr.size() > 0)
	{
		int32 nIndex = getRandIndex(vecArr);
		_pos = mapData.begin();
		advance(_pos,nIndex);
		if (_pos != mapData.end())
			value = _pos->first;
	}
	return true;
}
//-------------------------------------------------------------
//--- �Ƿ�����ɹ�
extern bool		randRatio	(uint32 uMin,uint32 uMax,uint32 uCount,uint32 uRatio,uint32 uRatioMax = 10000);
//--- �Ƿ�����ɹ�
extern bool		randRatio	(uint32 uRatio,uint32 uRatioMax = 10000);
//-------------------------------------------------------------
//------------------------------ ���������ͬ������
template <typename T>
void	getRandVecDiff(const stl_vector<T> &vecInput, stl_vector<T>& vecOutput,uint32 uRandNum)
{
	stl_list<uint32> listData;
	for (uint32 i = 0; i < vecInput.size(); ++i)
		listData.push_back(i);

	if (uRandNum >= vecInput.size())
		uRandNum = vecInput.size();

	for (uint32 i = 0; i < uRandNum; ++i)
	{
		if (listData.size() < 1)
			break;

		uint32 uIndex = randRange((uint32)(listData.size() - 1));
		stl_list<uint32>::iterator pos = listData.begin();
		std::advance(pos,uIndex);
		if (pos == listData.end() || *pos >= vecInput.size())
			continue;

		vecOutput.push_back(vecInput[*pos]);
		listData.erase(pos);
	}
}
