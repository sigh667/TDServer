/*----------------- dbSqlite3.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/8/15 22:41:23
*--------------------------------------------------------------
*1��sqlite3�Ա����ݽ��ж�д��Ϊ����д�����ݱ���ж�д
*2���ַ������ݱ���ʹ�õ�����'����
*3����Ҫд�����������ʱʹ��
*	a��BLOB�ֶ�
*	b��CSqlite3::getEncoded��ȡ�����Ʊ�������ַ���д��(���ݱ���ʹ�õ�����'����)
*	c������ʹ���������뷽����CSqlite3Statement::bind_blob()[CSqlite3Statement::bind_null()]����(�������)
*		cmd("insert into table values (?,?,?,?);")
*	d����ȡ����ʹ��CSqlite3Query::getBlob()��ȡ���ݼ�CSqlite3Query::getBytes()��ȡ���ݳ���
*4��������������ʱ��ʹ�����������Դ�����Ч��:CSqlite3Transaction:CSqlite3Statement
*5�����ݲ�ѯ�������ַ���
*	a��CSqlite3Query(�����Ի�ò�ѯ����,��ѯ���Ĭ�ϵ�һ��,ֻ��������ѯnextRow():����ʹ��for(;!CSqlite3Query::isEof();CSqlite3Query::nextRow())
*	b��CSqlite3Table(���Ի�ò�ѯ����getRow(),��ѯָ����SetRow())
*6���ֶ�����
*	TEXT	�ı�
*	NUMERIC	ʵ��(8�ֽ�__int64)
*	INTEGER	����
*	FLOAT	����
*	BLOB	������
******************
*	ORDER BY <> ASC		����(С��ǰ���ں�)
*	ORDER BY <> DESC	����(����ǰС�ں�)
*------------------------------------------------------------*/
#pragma once
#include <iostream>
#include <stdio.h>
#include "include/sqlite3.h"
#define sqlite_throw
/*************************************************************/
/**************************************************************
* ˵�� : �쳣�׳���Ϣ
**************************************************************/
class CSqlite3Exception
{
public:
	int	nCode;
	char	szMsg[512];
public:
	CSqlite3Exception(sqlite3* pDB,int nCode = 0,const char* pMsg = NULL);
	~CSqlite3Exception(){}
};
/**************************************************************
* ˵�� : SQL�������
**************************************************************/
class CSqlite3Query
{
protected:
	bool			m_bEof;
	int				m_nColumnCount;
	sqlite3_stmt*	m_pStatement;

public:
	CSqlite3Query(sqlite3_stmt* pStmt = NULL);
	~CSqlite3Query();

public:
	inline bool isEof		()	{	return m_bEof;	}

public:
	CSqlite3Query& operator=(CSqlite3Query& query);

public:
	/*----->{ ����SQL }*/ 
	int			finalize		();
	/*----->{ ���ݵ���һ�� }*/ 
	bool		nextRow			();

public:
	/*----->{ ���ݽ������ }*/ 
	int			getColumn		();

protected:
	bool		isColumn		(int nCol);

public:/*�б�Ŵ�����������ʼ*/ 
	/*----->{ ��N�е�ֵ���������� }*/ 
	int			getType			(int nCol);	
	/*----->{ ���ظ����� CREATE TABLE ���������������,���������ڵ����������ǿ��ַ�����ʱ�� }*/ 
	const char*	getDecltype		(int nCol);
	/*----->{ �ֶ�����ID }*/ 
	int			getField		(const char* pField);
	/*----->{ ��N�е��ֶ��� }*/ 
	const char*	getName			(int nCol);

public:/*�б�Ŵ�����������ʼ*/ 	
	/*----->{ ����˫�ֽ����� }*/ 
	double		getDouble		(int nCol);
	/*----->{ �Ա���������������ʽ����һ������ֵ }*/ 
	int			getInt			(int nCol);
	/*----->{ ����64λ������ }*/ 
	long long	getInt64		(int nCol);
	/*----->{ BLOBs�е��ֽ�������TEXT�ַ������ֽ��� }*/ 
	int			getBytes		(int nCol);
	/*----->{ ����BLOB���� }*/ 
	const void*	getBlob			(int nCol);
	/*----->{ ����TEXT���� }*/ 
	const unsigned char*getString(int nCol);
	/*----->{ ��ȡ���������� }*/ 
	bool		getBlob			(int nCol,void* pBuf,int nBufLen);

public:/*�б�Ŵ�����������ʼ*/ 
	/*----->{ ��N�е�UTF-16�����ֶ��� }*/ 
	const void*	getName16	(int nCol);
	/*----->{ �������ظ����� CREATE TABLE ���������������,���������ڵ����������ǿ��ַ�����ʱ�� UTF-16���� }*/ 
	const void*	getDecltype16(int nCol);
	/*----->{ ����UTF-16�����BLOBs�е��ֽ�������TEXT�ַ������ֽ��� }*/ 
	int			getBytes16	(int nCol);
	/*----->{ ����UTF-16����TEXT���� }*/ 
	const void*	getText16	(int nCol);
};
/**************************************************************
* ˵�� : ����(��������ʱ������Ч���磺[insert into table values (?, ?);])
**************************************************************/
class CSqlite3Statement
{
protected:
	sqlite3*		m_pDBCon;		/*����*/ 
	sqlite3_stmt*	m_pStatement;

public:
	CSqlite3Statement(sqlite3*pDBCon,sqlite3_stmt*pStmt);
	~CSqlite3Statement();

public:
	CSqlite3Statement& operator=(CSqlite3Statement& statement);

public:
	/*----->{ ����SQL }*/ 
	void	finalize	();
	/*----->{ ���� }*/ 
	void	reset		();
	/*----->{ ִ�����(����Ӱ��������) }*/ 
	int		exec		();
public:/*�󶨲���(��1��ʼ)*/ 
	int		bind_null	(int nParam);
	int		bind_double	(int nParam,double dValue);
	int		bind_int	(int nParam,int nValue);
	int		bind_int64	(int nParam,long long int nValue);
	int		bind_blob	(int nParam,const void* pValue,int nLen);
	int		bind_text	(int nParam,const char* pValue);
	int		bind_text16	(int nParam,const void* pValue);
};
/**************************************************************
* ˵�� : 
**************************************************************/
class CSqlite3Table
{
protected:
	int		m_nCols;
	int		m_nRows;
	int		m_nCurrentRow;
	char**	m_ppszResults;
public:
	CSqlite3Table(char** ppszResults, int nRows, int nCols);
	virtual ~CSqlite3Table();

public:
	CSqlite3Table& operator=(CSqlite3Table& table);

public:
	void		finalize	();

protected:
	int			getIndex	(int nCol);
	int			getField	(const char* pField);

public:
	/*----->{ ���ݽ������ }*/ 
	int			getColumn	();
	/*----->{ ���ݽ������ }*/ 
	int			getRow		();
	/*----->{ ���õ�ǰ�� }*/ 
	bool		setRow		(int nRow);
	/*----->{ ��N�е��ֶ��� }*/ 
	const char*	getName		(int nCol);

public:
	bool		isNull		(int nCol);
	int			getInt		(int nCol,int nNullValue = 0);
	double		getDouble	(int nCol,double dNullValue = 0.0);
	const char* getValue	(int nCol);
	const char* getString	(int nCol,const char* pNullValue = "");

public:
	bool		isNull		(const char* pField);
	int			getInt		(const char* pField,int nNullValue = 0);
	double		getDouble	(const char* pField,double dNullValue = 0.0);
	const char* getValue	(const char* pField);
	const char* getString	(const char* pField,const char* pNullValue = "");
};
/**************************************************************
* ˵�� : ����(����insert/updateʱʹ�ÿ����Ч��)
**************************************************************/
class CSqlite3Transaction
{
private:
	bool		m_bEnd;
	sqlite3*	m_pDBCon;	/*����*/ 

public:
	CSqlite3Transaction(sqlite3* pDBCon);
	~CSqlite3Transaction();

public:
	CSqlite3Transaction& operator=(CSqlite3Transaction& transaction);

public:
	/*----->{ ��ʼ���� }*/ 
	bool	begin	();
	/*----->{ �ύ���� }*/ 
	bool	commit	();
	/*----->{ �ع����� }*/ 
	bool	rollback();
};
/**************************************************************
* ˵�� : 
**************************************************************/
class CSqlite3
{
protected:
	//char*			m_pSQL;				/*SQL���*/ 
	char			m_szSQL[10*1024];	/*SQL���*/ 
	char			m_szDBName[256];	/*�������ݿ���*/ 
	sqlite3*		m_pConnection;		/*����*/ 

protected:
	int				m_nBinaryLen;
	int				m_nBufferLen;
	int				m_nEncodedLen;
	bool			m_bEncoded;
	unsigned char*	m_pBuf;

public:
	CSqlite3();
	virtual~CSqlite3();

public:
	/*----->{ �Ƿ��Ѵ����ݿ� }*/ 
	inline bool	isOpen	()						{	return (m_pConnection != NULL);	}

public:
	/*----->{ �����ݿ�(�ڴ����ݿ�":memory:") }*/ 
	bool	open		(const char* pDBName);
	/*----->{ �����ݿ�(�ڴ����ݿ�":memory:") }*/ 
	bool	open16		(const void* pDBName);
	/*----->{ �ر����ݿ� }*/ 
	void	close		();
	/*----->{ ���ݿ⸴��(����) }*/ 
	bool	copy		(CSqlite3&clSqlite);

public:
	/*----->{ ���ó�ʱʱ��(����) }*/ 
	void	setBusyTimeout	(int nMillisecs);

public:
	/*----->{ ���Ƿ���� }*/ 
	bool	tableExists	(const char* pTableName);

protected:
	void	clearSQL	();

public:
	/*----->{ ��ʽ��SQL���(��������%Q��ʽ��) }*/ 
	void	cmd			(const char* pCmd,...);

public:
	/*----->{ ִ�����(����Ӱ��������-1Ϊʧ��) }*/ 
	int					exec			();
	/*----->{ ��ѯ(limit [0,]1;��ѯ�ڼ���ʼ����������) }*/ 
	CSqlite3Query		query			();
	/*----->{ ��ñ� }*/ 
	CSqlite3Table		getTable		();
	/*----->{ ���һ������ }*/ 
	CSqlite3Statement	compileStatement();
	/*----->{ ����һ���������� }*/ 
	CSqlite3Transaction	getTransaction	();

public:
	/*----->{ ��������ID }*/ 
	long long			lastInsertRowID	();

	/*�����ƶ�д*/
protected:
	void				clear			();
	/*----->{ �����Ʊ��� }*/ 
	unsigned int		encode_binary	(const unsigned char *pIn,unsigned int uIn, unsigned char *pOut,unsigned int uOut);
	/*----->{ �����ƽ��� }*/ 
	unsigned int		decode_binary	(const unsigned char *pIn,unsigned int uIn,unsigned char *pOut,unsigned int uOut);

public:
	/*----->{ ��ñ���(����д�����ݿ�) }*/ 
	const unsigned char*getEncoded		(const unsigned char* pBuf,int nLen);
	/*----->{ ���ж���������(�����ݿ�ȡ��) }*/ 
	const unsigned char*getBinary		(const unsigned char* pBuf,int nLen);
	/*----->{ ��ñ��볤�� }*/ 
	int					getBinaryLength	();
};
