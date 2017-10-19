/*----------------- mem_malloc.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2012/7/10 10:28:00
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "memoryPool/nedmalloc.h"
//#include "memoryPool/malloc.c.h"
/*************************************************************/
class mem_malloc
{
protected:
	nedalloc::nedpool*	m_nedpool;

public:
	mem_malloc()
	{
		m_nedpool = NULL;
	}
	virtual~mem_malloc()
	{
		destroy();
	}

protected:
	inline void	create		()
	{
		if(!m_nedpool)
			m_nedpool = nedalloc::nedcreatepool(0,0);
	}
	inline void	destroy		()
	{
		if(m_nedpool)
			nedalloc::neddestroypool(m_nedpool);
		/*nedalloc::nedpmallinfo*/
		m_nedpool = NULL;
	}

public:
	inline void*_malloc	(size_t size)
	{
		create();
		return nedalloc::nedpmalloc(m_nedpool,size);
	}
	inline void	_free	(void*p)
	{
		nedalloc::nedpfree(m_nedpool,p);
	}
};
/*************************************************************/
//#define _USE_NEDMALLOC

#define mem_malloc_declaration()\
public:\
	static mem_malloc	__malloc;\
	inline void*operator new		(size_t size)	{	return __malloc._malloc(size);	}\
	inline void*operator new[]		(size_t size)	{	return __malloc._malloc(size);	}\
	inline void	operator delete		(void*p)		{	__malloc._free(p);				}\
	inline void	operator delete[]	(void*p)		{	__malloc._free(p);				}

#define mem_malloc_instance(_object)\
	mem_malloc	_object::__malloc;

//#define _USE_OBJECT_POOL
#define object_declaration(_object)\
	mem_malloc_declaration();

#define object_instance(_object)\
	mem_malloc_instance(_object);
