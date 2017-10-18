/*------------- streamBase.h
* Copyright (C): 2011 Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/8/19 9:54:00
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include <iostream>
#include <memory.h>
#include "stl/stl_vector.h"
#include "stl/std_map.h"
#include "stl/std_set.h"
#include "basic/basicTypes.h"
#include "stackChecker/stackChecker.h"
/*************************************************************/
class CStreamBase
{
public:
	virtual~CStreamBase(){}

public:
	virtual uint32	getStreamSize	()						= 0;
	virtual uint32	getPosition		()						= 0;
	virtual bool	setPosition		(uint32 newPosition)	= 0;

protected:
	virtual bool	_read			(uint32 uBytes,void* outBuffer)		= 0;
	virtual bool	_write			(uint32 uBytes,const void*inBuffer)	= 0;
public:
	virtual uint32	fprintf			(const char* pszFormat,...);

public:
	inline uint32	getSpareSize	()									{	return getStreamSize() - getPosition();					}

public:
	inline bool		read			(uint32 uBytes,void*outBuffer)		{	return _read(uBytes,outBuffer);							}
	inline bool		write			(uint32 uBytes,const void*inBuffer)	{	return _write(uBytes,inBuffer);							}

	inline bool		read			(int8& outData)						{	return _read(sizeof(int8),&outData);					}
	inline bool		write			(int8 inData)						{	return _write(sizeof(int8),&inData);					}

	inline bool		read			(char& outData)						{	return _read(sizeof(char),&outData);					}
	inline bool		write			(char inData)						{	return _write(sizeof(char),&inData);					}

	inline bool		read			(uint8& outData)					{	return _read(sizeof(uint8),&outData);					}
	inline bool		write			(uint8 inData)						{	return _write(sizeof(uint8),&inData);					}

	inline bool		read			(int16& outData)					{	return _read(sizeof(int16),&outData);					}
	inline bool		write			(int16 inData)						{	return _write(sizeof(int16),&inData);					}

	inline bool		read			(uint16& outData)					{	return _read(sizeof(uint16),&outData);					}
	inline bool		write			(uint16 inData)						{	return _write(sizeof(uint16),&inData);					}

	inline bool		read			(int32& outData)					{	return _read(sizeof(int32),&outData);					}
	inline bool		write			(int32 inData)						{	return _write(sizeof(int32),&inData);					}

	inline bool		read			(uint32& outData)					{	return _read(sizeof(uint32),&outData);					}
	inline bool		write			(uint32 inData)						{	return _write(sizeof(uint32),&inData);					}

	inline bool		read			(int64& outData)					{	return _read(sizeof(int64),&outData);					}
	inline bool		write			(int64 inData)						{	return _write(sizeof(int64),&inData);					}

	inline bool		read			(uint64& outData)					{	return _read(sizeof(uint64),&outData);					}
	inline bool		write			(uint64 inData)						{	return _write(sizeof(uint64),&inData);					}

	inline bool		read			(float32& outData)					{	return _read(sizeof(float32),&outData);					}
	inline bool		write			(float32 inData)					{	return _write(sizeof(float32),&inData);					}

	inline bool		read			(float64& outData)					{	return _read(sizeof(float64),&outData);					}
	inline bool		write			(float64 inData)					{	return _write(sizeof(float64),&inData);					}

	inline bool		read			(std::string& outData)				
	{
		outData.clear();
		uint32 uCount = 0;
		if(!read(uCount))
			return false;

		for (uint32 i = 0;i < uCount ; ++i)
		{
			char temp;
			if (!read(temp))
				return false;
			outData.push_back(temp);
		}
		return true;
	}
	inline bool		write			(std::string& inData)
	{
		return write(const_cast<const std::string&>(inData));				
	}
	inline bool		write			(const std::string& inData)
	{
		uint32 uCount = inData.length();
		if(!write(uint32(uCount)))
			return false;

		for (uint32 i = 0;i < uCount ; ++i)
		{
			if (!write(inData[i]))
				return false;
		}

		return true;				
	}

	template <typename T>
	inline bool		read			(T& outData)
	{
		return outData.read(*this);
	}
	template <typename T>
	inline bool		write			(T& inData)
	{
		return inData.write(*this);
	}

	//------------------------------ 
	template <typename T>
	inline bool		read			(stl_vector<T>&vec)
	{
		uint32 uCount = 0;
		if(!read(uCount))
			return false;

		for (uint32 i = 0;i < uCount ; ++i)
		{
			T temp;
			if (!read(temp))
				return false;
			vec.push_back(temp);
		}
		return true;
	}
	//------------------------------ 
	template <typename T>
	inline bool		write			(stl_vector<T>&vec)
	{
		uint32 uCount = vec.size();
		if(!write(uint32(uCount)))
			return false;

		for (uint32 i = 0;i < uCount ; ++i)
		{
			if (!write(vec[i]))
				return false;
		}

		return true;
	}

	//------------------------------ 读取map容器
	template <typename K , typename V >
	inline bool		read		(stl_map<K,V>&stlMap)
	{
		uint32 uCount = 0;
		if(!read(uCount))
			return false;

		for (uint32 i = 0;i < uCount ; ++i)
		{
			K _key;
			V _value;
			if( !read( _key ) || !read( _value ) )
				return false;

			typename stl_map<K,V>::_Pairib _pai = stlMap.insert_( _key , _value );
			if(_pai.first != stlMap.end())
				(_pai.first)->second = _value;
		}

		return true;
	}
	//------------------------------ 
	template <typename K , typename V >
	inline bool		write	(stl_map<K,V>&stlMap)
	{
		if(!write(uint32(stlMap.size())))
			return false;

		typename stl_map<K,V>::iterator _pos = stlMap.begin();
		typename stl_map<K,V>::iterator _end = stlMap.end();
		for ( ; _pos != _end ; ++_pos )
		{
			if( !write( _pos->first ) || !write( _pos->second ) )
				return false;
		}

		return true;
	}
	//------------------------------ 
	template <typename K1 , typename K2 , typename V >
	inline bool		read		(double_map<K1, K2, V>&stlDMap)
	{
		uint32 uCount = 0;
		if(!read(uCount))
			return false;

		for (uint32 i = 0;i < uCount ; ++i)
		{
			K1 _key;
			if( !read( _key ) )
				return false;
			
			typename double_map<K1, K2, V>::_main_map::_Pairib _pai = stlDMap.main_map().insert_( _key , typename double_map<K1, K2, V>::_sub_map() );
			if(_pai.first == stlDMap.end() ||  !read( (_pai.first)->second ) )
				return false;
		}

		return true;
	}
	//------------------------------ 
	template <typename K1 , typename K2 , typename V >
	inline bool		write	(double_map<K1, K2, V>&stlDMap)
	{
		if(!write(uint32(stlDMap.size())))
			return false;

		typename double_map<K1, K2, V>::iterator _pos = stlDMap.begin();
		typename double_map<K1, K2, V>::iterator _end = stlDMap.end();
		for ( ; _pos != _end ; ++_pos )
		{
			if( !write( _pos->first ) || !write( _pos->second ) )
				return false;
		}

		return true;
	}
	//------------------------------ 读取时会做清理操作
	template <typename K>
	inline bool		read		(stl_set<K>&stlSet)
	{
		uint32 uCount = 0;
		if(!read(uCount))
			return false;

		for (uint32 i = 0; i < uCount ; ++i )
		{
			K _key;
			if( !read( _key ))
				return false;

			stlSet.insert(_key);
		}

		return true;
	}
	//------------------------------ 
	template <typename K>
	inline bool		write	(stl_set<K>&stlSet)
	{
		if(!write(uint32(stlSet.size())))
			return false;

		typename stl_set<K>::iterator _pos = stlSet.begin();
		typename stl_set<K>::iterator _end = stlSet.end();
		for ( ; _pos != _end ; ++_pos )
		{
			if( !write(K(*_pos)) )
				return false;
		}

		return true;
	}
public:
	virtual bool	isEof			()							{	return (getSpareSize() == 0);			}
	virtual bool	isEmpty			()							{	return (getStreamSize() == 0);			}

public:
	virtual bool	skipPosition	(int32 _Bytes);
	virtual bool	readString		(char* stringBuf,uint32 bufferSize);
	virtual bool	writeString		(const char *stringBuf,int32 maxLen = -1);
	virtual bool	readLine		(char* buffer,uint32 bufferSize);
	virtual bool	writeLine		(const char *buffer);
	virtual bool	writeString		(const std::string&_string);
	virtual bool	readString		(std::string&_string);
};
