/*------------- basicFunctions.h
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/1/29 17:46:12
*
*/ 
/***************************************************************
* ������������
***************************************************************/
#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include "basic/basicTypes.h"
#include "stl/std_hash_map.h"
#include "stl/std_map.h"
/*************************************************************/
extern int32	dAtoi			(const char*pString);
extern uint32	dAtoui			(const char*pString);
extern long		dAtol			(const char*pString);
extern double	dAtof			(const char*pString);
extern int64	dAtoll			(const char*pString);
extern uint64	dAtoull			(const char*pString);
extern void		dFormatInt		(int64 _value, std::string &str);
extern void		dFormatUint		(int64 _value, std::string &str);
//------------------------------------------------------
//------------------------------- Value Function...
//--- ���uint32�����а�������λ
extern uint8	getNumberBit	(uint32 uNumber);

//--- ����ۼ���  positive
extern uint32	getAddValue		(uint32 uFrom,uint32 uValue,uint32 nMax);

//--- ������
extern uint32	addValue		(uint32 uValue,int nValue,uint32 nMax = uint32(-1));

//--- ����intֵ
extern bool		setInt			(int32&iOld,int32 iNew,int32 iMax = 0x7FFFFFFF,int32 iMin = 0/*0x80000000*/);
//--- ����intֵ
extern bool		updateInt		(int32&iValue,int32 iUpdate,int32 iMax = 0x7FFFFFFF,int32 iMin = 0);
//--- �ܷ����int32ֵ
extern bool		canUpdateUint32	(uint32 uValue,int32 iUpdate,uint32 uMax = 0xFFFFFFFF);
//--- ����int32ֵ
extern bool		updateUint32	(uint32&uValue,int32 iUpdate,uint32 uMax = 0xFFFFFFFF);
extern bool		canUpdateUint32	(uint32 uValue,uint32 uUpdate,bool bAdd,uint32 uMax = 0xFFFFFFFF);
extern bool		updateUint32	(uint32&uValue,uint32&uUpdate,bool bAdd,uint32 uMax = 0xFFFFFFFF);
extern bool		updateUint32_	(uint32&uValue,uint32 uUpdate,bool bAdd,uint32 uMax = 0xFFFFFFFF);
extern uint32	dUpdateUint32	(uint32 uValue,uint32 uUpdate,bool bAdd,uint32 uMax = 0xFFFFFFFF);
//--- ��ȡ����ֵ
extern uint32	getUint32Proportion	(uint32 uValue,float32 fRatio,bool bRoundedUp = true);

/*--- ����
@_molecular		=����
@_denominator	=��ĸ
@_floor			=����ȡ��
*/
extern uint32	dDivisionUint32	(uint64 _molecular,uint32 _denominator,bool _floor = true);
extern uint64	dDivisionUint64	(uint64 _molecular,uint64 _denominator,bool _floor = true);

//-------------------------------------------------------------
//------------------------------ ָ������(��ָ���ں�)
template<class T>
inline void	_qsort_pointer(T *_list, const uint32&_count)
{
	if(!_list || !_count)
		return;

	for (uint32 i = 0;i < _count;++i)
	{
		if(_list[i])
			continue;

		bool _empty = true;
		for (uint32 j = i + 1;j < _count;++j)
		{
			if(!_list[j])
				continue;

			_empty	= false;
			_list[i]= _list[j];
			_list[j]= NULL;
			break;
		}
		if(_empty)
			break;
	}
}

//-------------------------------------------------------------
//------------------------------ ����ָ��ֵ
template<class _T>
inline void	setValue(_T*_pointer,_T _value)
{
	if(_pointer)
		*_pointer = _value;
}

//-------------------------------------------------------------
//------------------------------ 
inline void	_setBIT8Array	(uint8 _arr[],uint16 _bit,bool _set,uint16 uSize)
{
	uint16 _index = uint16(_bit >> 3);
	if(_index >= 0 && _index < uSize)
	{
		if(_set)
			_arr[_index] |= uint8(1 << (_bit & 0x7));
		else
			_arr[_index] &= uint8(~(1 << (_bit & 0x7)));
	}
}

//-------------------------------------------------------------
//------------------------------ 
inline bool	_checkBIT8Array	(const uint8 _arr[],uint16 _bit,uint16 uSize)
{
	uint16 _index = uint16(_bit >> 3);
	if(_index >= 0 && _index < uSize)
	{
		return ((_arr[_index] & uint8(1 << (_bit & 0x7))) != 0);
	}
	return false;
}

#define _BIT8ArraySize(_bit)	((_bit) / 8 + 1)