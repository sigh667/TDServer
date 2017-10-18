/*----------------- dbSqlite3.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/15 22:41:23
*--------------------------------------------------------------
*1、sqlite3对表数据进行读写分为语句读写及根据表进行读写
*2、字符串数据必须使用单引号'括起
*3、需要写入二进制数据时使用
*	a、BLOB字段
*	b、CSqlite3::getEncoded获取二进制编码进入字符串写入(数据必须使用单引号'括起)
*	c、可以使用批量插入方法绑定CSqlite3Statement::bind_blob()[CSqlite3Statement::bind_null()]插入(无需编码)
*		cmd("insert into table values (?,?,?,?);")
*	d、读取可以使用CSqlite3Query::getBlob()读取数据及CSqlite3Query::getBytes()读取数据长度
*4、批量插入数据时请使用事务锁可以大大提高效率:CSqlite3Transaction:CSqlite3Statement
*5、数据查询可有两种方法
*	a、CSqlite3Query(不可以获得查询行数,查询结果默认第一行,只能逐条查询nextRow():建议使用for(;!CSqlite3Query::isEof();CSqlite3Query::nextRow())
*	b、CSqlite3Table(可以获得查询行数getRow(),查询指定行SetRow())
*6、字段类型
*	TEXT	文本
*	NUMERIC	实数(8字节__int64)
*	INTEGER	整型
*	FLOAT	浮点
*	BLOB	二进制
******************
*	ORDER BY <> ASC		升序(小在前大在后)
*	ORDER BY <> DESC	降序(大在前小在后)
*------------------------------------------------------------*/
#pragma once
#include <iostream>
#include <stdio.h>
#include "include/sqlite3.h"
#define sqlite_throw
/*************************************************************/
/**************************************************************
* 说明 : 异常抛出信息
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
* 说明 : SQL结果声明
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
	/*----->{ 销毁SQL }*/ 
	int			finalize		();
	/*----->{ 数据到下一行 }*/ 
	bool		nextRow			();

public:
	/*----->{ 数据结果列数 }*/ 
	int			getColumn		();

protected:
	bool		isColumn		(int nCol);

public:/*列编号从左到右以零起始*/ 
	/*----->{ 第N列的值的数据类型 }*/ 
	int			getType			(int nCol);	
	/*----->{ 返回该列在 CREATE TABLE 语句中声明的类型,它可以用在当返回类型是空字符串的时候 }*/ 
	const char*	getDecltype		(int nCol);
	/*----->{ 字段名列ID }*/ 
	int			getField		(const char* pField);
	/*----->{ 第N列的字段名 }*/ 
	const char*	getName			(int nCol);

public:/*列编号从左到右以零起始*/ 	
	/*----->{ 返回双字节数据 }*/ 
	double		getDouble		(int nCol);
	/*----->{ 以本地主机的整数格式返回一个整数值 }*/ 
	int			getInt			(int nCol);
	/*----->{ 返回64位的整数 }*/ 
	long long	getInt64		(int nCol);
	/*----->{ BLOBs列的字节数或者TEXT字符串的字节数 }*/ 
	int			getBytes		(int nCol);
	/*----->{ 返回BLOB数据 }*/ 
	const void*	getBlob			(int nCol);
	/*----->{ 返回TEXT数据 }*/ 
	const unsigned char*getString(int nCol);
	/*----->{ 获取二进制数据 }*/ 
	bool		getBlob			(int nCol,void* pBuf,int nBufLen);

public:/*列编号从左到右以零起始*/ 
	/*----->{ 第N列的UTF-16编码字段名 }*/ 
	const void*	getName16	(int nCol);
	/*----->{ 用来返回该列在 CREATE TABLE 语句中声明的类型,它可以用在当返回类型是空字符串的时候 UTF-16编码 }*/ 
	const void*	getDecltype16(int nCol);
	/*----->{ 返回UTF-16编码的BLOBs列的字节数或者TEXT字符串的字节数 }*/ 
	int			getBytes16	(int nCol);
	/*----->{ 返回UTF-16编码TEXT数据 }*/ 
	const void*	getText16	(int nCol);
};
/**************************************************************
* 说明 : 声明(批量操作时可增加效率如：[insert into table values (?, ?);])
**************************************************************/
class CSqlite3Statement
{
protected:
	sqlite3*		m_pDBCon;		/*连接*/ 
	sqlite3_stmt*	m_pStatement;

public:
	CSqlite3Statement(sqlite3*pDBCon,sqlite3_stmt*pStmt);
	~CSqlite3Statement();

public:
	CSqlite3Statement& operator=(CSqlite3Statement& statement);

public:
	/*----->{ 销毁SQL }*/ 
	void	finalize	();
	/*----->{ 重置 }*/ 
	void	reset		();
	/*----->{ 执行语句(返回影响行数字) }*/ 
	int		exec		();
public:/*绑定参数(从1开始)*/ 
	int		bind_null	(int nParam);
	int		bind_double	(int nParam,double dValue);
	int		bind_int	(int nParam,int nValue);
	int		bind_int64	(int nParam,long long int nValue);
	int		bind_blob	(int nParam,const void* pValue,int nLen);
	int		bind_text	(int nParam,const char* pValue);
	int		bind_text16	(int nParam,const void* pValue);
};
/**************************************************************
* 说明 : 
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
	/*----->{ 数据结果列数 }*/ 
	int			getColumn	();
	/*----->{ 数据结果行数 }*/ 
	int			getRow		();
	/*----->{ 设置当前行 }*/ 
	bool		setRow		(int nRow);
	/*----->{ 第N列的字段名 }*/ 
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
* 说明 : 事务(批量insert/update时使用可提高效率)
**************************************************************/
class CSqlite3Transaction
{
private:
	bool		m_bEnd;
	sqlite3*	m_pDBCon;	/*连接*/ 

public:
	CSqlite3Transaction(sqlite3* pDBCon);
	~CSqlite3Transaction();

public:
	CSqlite3Transaction& operator=(CSqlite3Transaction& transaction);

public:
	/*----->{ 开始事务 }*/ 
	bool	begin	();
	/*----->{ 提交事务 }*/ 
	bool	commit	();
	/*----->{ 回滚事务 }*/ 
	bool	rollback();
};
/**************************************************************
* 说明 : 
**************************************************************/
class CSqlite3
{
protected:
	//char*			m_pSQL;				/*SQL语句*/ 
	char			m_szSQL[10*1024];	/*SQL语句*/ 
	char			m_szDBName[256];	/*链接数据库名*/ 
	sqlite3*		m_pConnection;		/*连接*/ 

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
	/*----->{ 是否已打开数据库 }*/ 
	inline bool	isOpen	()						{	return (m_pConnection != NULL);	}

public:
	/*----->{ 打开数据库(内存数据库":memory:") }*/ 
	bool	open		(const char* pDBName);
	/*----->{ 打开数据库(内存数据库":memory:") }*/ 
	bool	open16		(const void* pDBName);
	/*----->{ 关闭数据库 }*/ 
	void	close		();
	/*----->{ 数据库复制(覆盖) }*/ 
	bool	copy		(CSqlite3&clSqlite);

public:
	/*----->{ 设置超时时间(毫秒) }*/ 
	void	setBusyTimeout	(int nMillisecs);

public:
	/*----->{ 表是否存在 }*/ 
	bool	tableExists	(const char* pTableName);

protected:
	void	clearSQL	();

public:
	/*----->{ 格式化SQL语句(二进制用%Q格式化) }*/ 
	void	cmd			(const char* pCmd,...);

public:
	/*----->{ 执行语句(返回影响行数字-1为失败) }*/ 
	int					exec			();
	/*----->{ 查询(limit [0,]1;查询第几开始多少条数据) }*/ 
	CSqlite3Query		query			();
	/*----->{ 获得表 }*/ 
	CSqlite3Table		getTable		();
	/*----->{ 获得一个声明 }*/ 
	CSqlite3Statement	compileStatement();
	/*----->{ 产生一个新事务处理 }*/ 
	CSqlite3Transaction	getTransaction	();

public:
	/*----->{ 最后插入行ID }*/ 
	long long			lastInsertRowID	();

	/*二进制读写*/
protected:
	void				clear			();
	/*----->{ 二进制编码 }*/ 
	unsigned int		encode_binary	(const unsigned char *pIn,unsigned int uIn, unsigned char *pOut,unsigned int uOut);
	/*----->{ 二进制解码 }*/ 
	unsigned int		decode_binary	(const unsigned char *pIn,unsigned int uIn,unsigned char *pOut,unsigned int uOut);

public:
	/*----->{ 获得编码(用于写入数据库) }*/ 
	const unsigned char*getEncoded		(const unsigned char* pBuf,int nLen);
	/*----->{ 获行二进制数据(从数据库取出) }*/ 
	const unsigned char*getBinary		(const unsigned char* pBuf,int nLen);
	/*----->{ 获得编码长度 }*/ 
	int					getBinaryLength	();
};
