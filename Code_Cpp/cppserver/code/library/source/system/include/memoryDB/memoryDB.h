/*------------- memoryDB.h
* Copyright (C): 2011 Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/8/31 14:26:59
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "dbSqlite3/include/dbSqlite3.h"
/*************************************************************/
class CMemoryDB
{
private:
	static	CSqlite3	m_dbSqlite;

public:
	/*----->{ 创始化内存数据库 }*/ 
	static	bool	InitMemoryDB	();
	/*----->{ 关闭内存数据库 }*/ 
	static	void	CloseMemoryDB	();

public:
	static	CSqlite3&GetSqlite3		();
	/*----->{  }*/ 
	static	CSqlite3Transaction	GetTransaction();
};
