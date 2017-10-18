/*----------------- singleton.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2012/9/1 11:56:34
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
template <typename T>
class Singleton
{
protected:
	static T*	__single_instance;

protected:
	Singleton(){}
	virtual~Singleton(){}

public:
	static T*	get_single_object	()	{	return __single_instance;	}
	static T&	get_single_instance	()	{	return *__single_instance;	}
	static T*	new_single_instance()
	{
		if(!__single_instance)
			__single_instance = new T;

		return __single_instance;
	}
	static void	del_single_instance()
	{
		if(__single_instance)
			delete __single_instance;

		__single_instance = NULL;
	}
};
template <typename T>T*	Singleton<T>::__single_instance = NULL;

#define _single_public(_T)			public Singleton<_T>
#define _single_friend(_T)			friend class Singleton<_T>;
#define _single_object(_T)			_T::get_single_object	()
#define _single_instance(_T)		_T::get_single_instance	()
#define _single_instance_new(_T)	_T::new_single_instance	()
//#define _single_instance_del(_T)	_T::del_single_instance	()
#define _single_instance_del(ptr)	do { if (ptr)	(ptr)->del_single_instance();	(ptr) = NULL;	} while (false)
