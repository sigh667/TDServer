/*----------------- gmModel.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ¡÷æ‘
* Version      : V1.0
* Date         : 2016/9/19 13:33:17
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/stlDefine.h"
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------

struct _stGMModelBase
{
	std::string		sModule;
	std::string		sDesc;
	std::string		sCmd;
	std::string		sP1type;
	std::string		sP1desc;
	std::string		sP2type;
	std::string		sP2desc;
	std::string		sP3type;
	std::string		sP3desc;
	std::string		sP4type;
	std::string		sP4desc;
	std::string		sRemark;
	_stGMModelBase&	get()						{	return *this;											}
	inline void		init()			
	{	
		sModule.clear();
		sDesc.clear();
		sCmd.clear();
		sP1type.clear();
		sP1desc.clear();
		sP2type.clear();
		sP2desc.clear();
		sP3type.clear();
		sP3desc.clear();
		sP4type.clear();
		sP4desc.clear();
		sRemark.clear();
	}

	inline bool	read(CStreamBase& stream)
	{
		return stream.read(sModule)&&
			stream.read(sDesc)&&
			stream.read(sCmd)&&
			stream.read(sP1type)&&
			stream.read(sP1desc)&&
			stream.read(sP2type)&&
			stream.read(sP2desc)&&
			stream.read(sP3type)&&
			stream.read(sP3desc)&&
			stream.read(sP4type)&&
			stream.read(sP4desc)&&
			stream.read(sRemark);
	}
	inline bool	write(CStreamBase& stream)
	{
		return stream.write(sModule)&&
			stream.write(sDesc)&&
			stream.write(sCmd)&&
			stream.write(sP1type)&&
			stream.write(sP1desc)&&
			stream.write(sP2type)&&
			stream.write(sP2desc)&&
			stream.write(sP3type)&&
			stream.write(sP3desc)&&
			stream.write(sP4type)&&
			stream.write(sP4desc)&&
			stream.write(sRemark);
	}
};

struct _stGMModel	: _stGMModelBase
{
	_stGMModel& get()	{	return *this;			}
	inline void init()	{	_stGMModelBase::init();	}
	inline bool read(CStreamBase& stream)
	{
		return _stGMModelBase::read(stream);
	}
	inline bool write(CStreamBase& stream)
	{
		return _stGMModelBase::write(stream);
	}
};
