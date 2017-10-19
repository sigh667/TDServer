/*----------------- hashMapPool.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2011/8/20 20:50:56
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
template<typename T>
struct _Val_Init
{
	static void initialize(T *pVal);
};

template<typename T>
void	_Val_Init<T>::initialize(T *pVal)
{
	if (NULL != pVal)
	{
		pVal->initialize();
	}
}

// ------------------------------------------------------------------
template<typename T>
struct _Val_PB_Init
{
	static void initialize(T *pVal);
};

template<typename T>
void	_Val_PB_Init<T>::initialize(T *pVal)
{
	if (NULL != pVal)
	{
		pVal->Clear();
	}
}

// ------------------------------------------------------------------
template<typename T>
struct _Val_STL_Init
{
	static void initialize(T *pVal);
};

template<typename T>
void	_Val_STL_Init<T>::initialize(T *pVal)
{
	if (NULL != pVal)
	{
		pVal->clear();
	}
}