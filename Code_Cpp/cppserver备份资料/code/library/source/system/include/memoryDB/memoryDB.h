/*------------- memoryDB.h
* Copyright (C): 2011 Mokylin��Mokyqi
* Author       : ����Դ
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
	/*----->{ ��ʼ���ڴ����ݿ� }*/ 
	static	bool	InitMemoryDB	();
	/*----->{ �ر��ڴ����ݿ� }*/ 
	static	void	CloseMemoryDB	();

public:
	static	CSqlite3&GetSqlite3		();
	/*----->{  }*/ 
	static	CSqlite3Transaction	GetTransaction();
};
