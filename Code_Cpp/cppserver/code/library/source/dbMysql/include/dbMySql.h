/*----------------- dbMySql.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/8/16 21:25:06
*--------------------------------------------------------------
*MySQL���ӷ�װ,֧��MySQL 1.4.1��5.1.x��汾
*------------------------------------------------------------*/
/*TODO:	MYSQL�ֶα�־ֵ
|	�ֶα�־ֵ				|	��־����								|
|---------------------------|----------------------------|
|	NOT_NULL_FLAG			|	�ֶβ���ΪNULL						|
|---------------------------|----------------------------|
|	PRI_KEY_FLAG			|	�ֶ�����������ɲ���				|
|---------------------------|----------------------------|
|	UNIQUE_KEY_FLAG		|	�ֶ���Ψһ������ɲ���			|
|---------------------------|----------------------------|
|	MULTIPLE_KEY_FLAG		|	�ֶ��Ƿ�Ψһ������ɲ���		|
|---------------------------|----------------------------|
|	UNSIGNED_FLAG			|	�ֶξ���UNSIGNED����				|
|---------------------------|----------------------------|
|	ZEROFILL_FLAG			|	�ֶξ���ZEROFILL����				|
|---------------------------|----------------------------|
|	BINARY_FLAG				|	�ֶξ���BINARY����				|
|---------------------------|----------------------------|
|	AUTO_INCREMENT_FLAG	|	�ֶξ���AUTO_INCREMENT����		|
|---------------------------|----------------------------|
|	ENUM_FLAG				|	�ֶ���ENUM���������ӣ�			|
|---------------------------|----------------------------|
|	SET_FLAG					|	�ֶ���SET���������ӣ�			|
|---------------------------|----------------------------|
|	BLOB_FLAG				|	�ֶ���BLOB��TEXT���������ӣ�	|
|---------------------------|----------------------------|
|	TIMESTAMP_FLAG			|	�ֶ���TIMESTAMP���������ӣ�	|
--------------------------------------------------------*/
/*TODO:	MYSQL�ֶ�����ֵ
|	����ֵ					|	��������				|	C����							|	��ע
|-----------------------|-----------------------------------------------|-----------------------------------|
|	MYSQL_TYPE_TINY		|	TINYINT				|	char							|	(1λ)										|
|-----------------------|------------------------------|----------------------------------------------------|
|	MYSQL_TYPE_SHORT		|	SMALLINT				|	short							|	(2λ)										|
|-----------------------|------------------------------|----------------------------------------------------|
|	MYSQL_TYPE_LONG		|	INTEGER				|	long/int						|	(4λ)										|
|-----------------------|------------------------------|----------------------------------------------------|
|	MYSQL_TYPE_INT24		|	MEDIUMINT			|	*int							|	(24λ)									|
|-----------------------|------------------------------|----------------------------------------------------|
|	MYSQL_TYPE_LONGLONG	|	BIGINT				|	long long int/__int64	|	(8λ)										|
|-----------------------|------------------------------|----------------------------------------------------|
|	MYSQL_TYPE_DECIMAL	|	DECIMAL��NUMERIC	|	*float						|												|
|-----------------------|------------------------------|----------------|-----------------------------------|
|	MYSQL_TYPE_NEWDECIMAL|	DECIMAL��NUMERIC	|	*float						|	������ѧ									|
|-----------------------|------------------------------|----------------|-----------------------------------|
|	MYSQL_TYPE_FLOAT		|	FLOAT					|	float							|	(4λ)										|
|-----------------------|------------------------------|----------------|-----------------------------------|
|	MYSQL_TYPE_DOUBLE		|	DOUBLE��REAL		|	double						|	(8λ)										|
|-----------------------|------------------------------|----------------|-----------------------------------|
|	MYSQL_TYPE_BIT			|	BIT					|	unsigned char				|	(@��ΧΪ��1��64)						|
|-----------------------|------------------------------|----------------|-----------------------------------|
|	MYSQL_TYPE_TIMESTAMP	|	TIMESTAMP			|	MYSQL_TIME					|												|
|-----------------------|--------------------|--------------------------|-----------------------------------|
|	MYSQL_TYPE_DATE		|	DATE					|	MYSQL_TIME					|												|
|-----------------------|--------------------|--------------------------|-----------------------------------|
|	MYSQL_TYPE_TIME		|	TIME					|	MYSQL_TIME					|												|
|-----------------------|------------------------------|----------------|-----------------------------------|
|	MYSQL_TYPE_DATETIME	|	DATETIME				|	MYSQL_TIME					|												|
|-----------------------|--------------------|--------------------------|-----------------------------------|
|	MYSQL_TYPE_YEAR		|	YEAR					|									|												|
|-----------------------|--------------------|--------------------------|-----------------------------------|
|	MYSQL_TYPE_STRING		|	CHAR					|									|												|
|-----------------------|--------------------|--------------------------|-----------------------------------|
|	MYSQL_TYPE_VAR_STRING|	VARCHAR				|									|												|
|-----------------------|--------------------|--------------------------|-----------------------------------|
|	MYSQL_TYPE_BLOB		|	BLOB��TEXT			|									|	��ʹ��max_length��ȷ����󳤶ȣ� |
|-----------------------|--------------------|--------------------------|-----------------------------------|
|	MYSQL_TYPE_SET			|	SET					|									|	(������ֵ����ռ�ڼ�λ[�ɶ�λ])	|
|-----------------------|--------------------|--------------------------|-----------------------------------|
|	MYSQL_TYPE_ENUM		|	ENUM					|									|												|
|-----------------------|--------------------|--------------------------|-----------------------------------|
|	MYSQL_TYPE_GEOMETRY	|	Spatial				|									|												|
|-----------------------|-----------------------------------------------|-----------------------------------|
|	MYSQL_TYPE_NULL		|	NULL-type			|									|												|
|-----------------------|--------------------|--------------------------|-----------------------------------|
|	MYSQL_TYPE_CHAR		|							|									|	�������ӣ���MYSQL_TYPE_TINYȡ��	|
*----------------------------------------------------------------------------------------------------------*/
#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <iostream>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif//WIN32
#include <memory>
#include <stdio.h>
#include "basic/basicTypes.h"
#include "basic/basicFunctions.h"
#include "basic/memoryFunctions.h"
#include "basic/stringFunctions.h"
#define HAVE_INT64
#define HAVE_UINT64
#include <mysql.h>
#include <errmsg.h>

/*************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*************************************************************/
#ifndef _MySQL_throw
#define _MySQL_throw
#endif //_MySQL_throw
/**************************************************************
* ˵�� : MySQL�쳣����
**************************************************************/
class CMySQLException
{
private:
protected:
public:
	unsigned int	uCode;
	char			szMsg[512];
	char			szError[512];
	char			szSqlState[512];

public:
	CMySQLException(MYSQL* mysql,unsigned int uCode = 0,const char* pMsg = NULL);
	CMySQLException(MYSQL_STMT* stmt,unsigned int uCode = 0,const char* pMsg = NULL);
	virtual~CMySQLException(){}
};

class CMySQLResult;
class CMySQLStatement;
/*************************************************************
* ˵�� : MySQL���ݿ�����
*************************************************************/
const	int		max_mysql_command_size	= 1024 * 16;
class CDBMySql
{
private:
protected:
	MYSQL*			m_pMySQL;				/*���ݿ����ӵľ��*/ 
	char			m_szCharsetName[256];	/*�����ַ�*/ 
	my_bool			m_bConnected;			/*�Ƿ�������*/ 
	my_bool			m_bReConnect;			/*�Ͽ��Ƿ��Զ�����(Ĭ��������)*/ 
	unsigned int	m_uConnectTimeout;		/*���ӳ�ʱ(Ĭ��3��)*/ 

protected:
	ulong			m_ulCmdLength;
	char			m_szCommand[max_mysql_command_size];

public:
	friend class CMySQLMultiFree;
	friend class CMySQLTransaction;

public:
	inline bool		isOpen				()		{	return m_bConnected != 0;	}

public:
	CDBMySql();
	virtual~CDBMySql();

public:
	/*--->[ ��ʼ�����ݿ��������� ]*/ 
	virtual	void	initialize			(my_bool bReConnect = true,unsigned int uConnectTimeout = 3,const char*pCharsetName = NULL);

public:
	/*--->[ ���������ݿ� ]
	(host=NULL��host="localhost"���ӽ�����Ϊ�뱾������������)
	(user=NULL��user=""���û�������Ϊ��ǰ�û�)
	(db=NULL�����ӻὫĬ�ϵ����ݿ���Ϊ��ֵ)
	(port!=0����ֵ������TCP/IP���ӵĶ˿ں�)
	(unix_socket!=NULL�����ַ���������Ӧʹ�õ��׽��ֻ������ܵ�)
	*/ 
	virtual	bool	open				(const char*host,const char*user,const char*passwd,const char*db =NULL,unsigned int port = 0
										,const char*unixSocket = NULL,unsigned long clientFlag = 0);
	/*--->[ �ر����ݿ�����(�ͷž��) ]*/ 
	virtual	void	close				();
	/*--->[ �Ƿ�����SQL��Ͽ����Զ����� ]
	(�����ڼ��������н������ʹ��)
	*/ 
	virtual	bool	ping				();
	//--- ɱ����ǰ���ݿ�����
	virtual	bool	disconnect			();

	/*--->[ �Ƿ���������䴦�� ]*/ 
	bool			multiSentence		(bool bMore);

public:
	/*--->[ ���õ�ǰ�����ַ��� ]
	(charsetName = "utf8",�ȵ�)
	(������Ϊgbkʱ�������������ַ���Ϊgbk,utf8ͬ�� ͨ��:���ݿ���Ҫ����ʱ�����ַ���Ϊgbk/utf8)
	*/ 
	bool			setConnectCharacter	(const char*charsetName);
	/*--->[ ���õ�ǰ�ַ��� ]*/
	bool			setCharacter		(const char*charsetName);

	/*--->[ ��ǰ����Ĭ���ַ��� ]*/ 
	const char*		getConnectCharacter	();
	/*--->[ ��ǰ����Ĭ���ַ�����Ϣ ]*/ 
	bool			getConnectCharacter	(MY_CHARSET_INFO& sInfo);

public:
	bool			getServerInfo		();
	bool			getClientInfo		();
	/*--->[ ��ʾ��ǰ�������̵߳��б� ]*/ 
	CMySQLResult	showProcessesList	();
	/*--->[ ��õ�ǰ���ӵ��߳�ID ]*/ 
	unsigned long	getConnectThreadID	();
	/*--->[ ������һ��SQL������ɵĸ澯�� ]*/ 
	unsigned int	getLastWarningCount	();
	/*--->[ ���ִ�в�ѯ���ַ�����Ϣ ]*/ 
	const char*		getExecuteInfo		();

public:
	/*--->[ ���������ϵ��û������ݿ� ]*/ 
	bool			changeConnectUser	(const char*user,const char*passwd, const char*db);
	/*--->[ ���ݿ�ѡ�� ]*/ 
	bool			dbSelect			(const char*db);
	/*--->[ ���ݿⴴ�� ]
	(db = ���ݿ���)
	(charsetName = �ַ�����)
	(collationName = ����)
	(exist = true ������ʱ�Ŵ���)
	*/ 
	bool			dbCreate			(const char*db,const char*charsetName = NULL,const char*collationName = NULL,bool exist = true);
	/*--->[ ���ݿ�ɾ�� ]*/ 
	bool			dbDrop				(const char*db,bool exist = true);
	/*--->[ ���ݿ��޸� ]
	(db = ���ݿ���)
	(charsetName = �ַ�����)
	(collationName = ����)
	*/ 
	bool			dbAlter				(const char*db,const char*charsetName,const char*collationName);

public:
	/*----->{ ���Ƿ���� }*/ 
	bool	tableExists					(const char* pTableName);
	/*--->[ ���ݱ��� ]
	(tblName = ����)
	(temp = �Ƿ���ʱ��)
	(exist = true ������ʱ�Ŵ���)
	(definition = ����)
	*/ 
	bool			tableCreate			(const char*tblName,bool temp,bool exist,const char*definition,...);
	/*--->[ ���ݱ�ɾ�� ]*/ 
	bool			tableDrop			(bool temp,bool exist,const char*tblName,...);
	/*--->[ ���ݱ��޸� ]*/ /*ADD | ALTER | CHANGE | MODIFY | DROP*/ 
	bool			tableAlter			(const char*tblName,const char*command,...);
	/*--->[ ���ݱ������� ]*/ 
	bool			tableRename			(const char*tblName,const char*newName);
	/*--->[ ���ݱ������� ]*/ 
	bool			tableRename			(const char*tblName,...);

public:
	/*--->[ �������ݿ��� ]
	(wild = ��������ʽ����,�ɰ���ͨ�����%����_��)
	*/ 
	CMySQLResult	showDatabase		(const char*wild = NULL);
	/*--->[ ������ǰ���ݿ��ڱ��� ]
	(wild = ��������ʽ����,�ɰ���ͨ�����%����_��)
	*/ 
	CMySQLResult	showTables			(const char*wild = NULL);
	/*--->[ ������ǰ���ݿ��б��ֶ��� ]
	(wild = ��������ʽ����,�ɰ���ͨ�����%����_��)
	*/ 
	CMySQLResult	showFields			(const char*table,const char*wild = NULL);

public:
	/*--->[ ��ʽ�����ݿ�ָ�� ]
	(��ӦΪ�������ս�ֺţ���;������\g��)
	*/ 
	ulong			cmd					(const char*pCmd,...);
	ulong			cmd_				(const char*pCmd);
	/*--->[ ִ��(execute) ]*/ 
	bool			execute				(bool real = true);
	/*--->[ ���AUTO_INCREMENT�����ɵ�ID ]*/ 
	my_ulonglong	getInsertIncrement	();
	/*--->[ ���Ӱ��/�ı����� ]*/ 
	my_ulonglong	getRowAffected		();
	/*--->[ ���ִ�н������(�޽����ʱ����) ]*/ 
	unsigned int	getResultFieldCount	();
	/*--->[ ���ִ�н�� ]*/ 
	CMySQLResult	getResult			();

public:
	/*--->[ ��һ������� ]
	(��ʹ��do{}while(resultMore());)
	*/ 
	bool			resultMore			();

public:
	/*--->[ ���Ԥ���� ]*/ 
	CMySQLStatement	createStatement		();
};
/*************************************************************
* ˵�� : ���ѯ�ͷ�
*	���ʹ�ö�����ѯ��洢���̲�ѯʱ�������
*************************************************************/
class CMySQLMultiFree
{
private:
protected:
	MYSQL*				m_pMySQL;			/*���ݿ����ӵľ��*/ 
public:
	CMySQLMultiFree();
	CMySQLMultiFree(MYSQL*pMySQL);
	CMySQLMultiFree(CDBMySql*pMySQL);
	virtual~CMySQLMultiFree();
};

/*************************************************************
* ˵�� : MySQL����
*************************************************************/
class CMySQLTransaction
{
private:
protected:
	MYSQL*	m_pMySQL;
	bool	m_bEndTrans;

public:
	CMySQLTransaction(MYSQL* mysql);
	CMySQLTransaction(CDBMySql*pMySQL);
	virtual~CMySQLTransaction();

public:
	/*--->[ ��ʼ���� ]*/
	bool	begin	();
	/*--->[ �ύ���� ]*/
	bool	commit	();
	/*--->[ �ع����� ]*/
	bool	rollback();
};

/*************************************************************
* ˵�� : ���ݿ��ѯ�����
*************************************************************/
class CMySQLResult
{
protected:
	MYSQL*				m_pMySQL;			/*���ݿ����ӵľ��(��Ҫ�����쳣����)*/ 
	MYSQL_RES*			m_pResult;			/*�����еĲ�ѯ�����*/ 
	MYSQL_ROW			m_pRow;				/*����1������*/ 
	my_ulonglong		m_ulRowCount;		/*������*/ 
	unsigned int		m_uFiledCount;		/*������*/ 
	unsigned long*		m_pulFiledsLength;	/*�����ݳ���*/ 

public:
	CMySQLResult(MYSQL* mysql = NULL,MYSQL_RES*pResult = NULL);
	virtual~CMySQLResult();

public:
	inline MYSQL_RES*		getMYSQL_RES	()	{	return m_pResult;			}
	inline MYSQL_ROW		getMYSQL_ROW	()	{	return m_pRow;				}

public:
	/*--->[ ��ý�������� ]*/ 
	inline my_ulonglong		getRowCount		()	{	return m_ulRowCount;		}
	/*--->[ ��ý�������� ]*/ 
	inline unsigned int		getFiledCount	()	{	return m_uFiledCount;		}
	/*--->[ ��ý���������ݳ��� ]*/ 
	inline unsigned long*	getFiledsLength	()	{	return m_pulFiledsLength;	}

public:
	CMySQLResult&			operator=		(CMySQLResult& clMySQLResult);

public:
	/*--->[ ��ʼ������� ]*/ 
	void				initialize			(MYSQL_RES*pResult);
	/*--->[ �ͷŽ����ʹ�õ��ڴ� ]*/ 
	void				freeResult			();

public:
	/*--->[ �����ϴ��ֶι���λ�� ]
	(�����ϴ���ʹ���ֶι���λ��)
	*/
	MYSQL_FIELD_OFFSET	fieldTell			();
	/*--->[ ���й������ָ������ ]
	(��ʼΪ0��FieldCount)
	*/
	MYSQL_FIELD_OFFSET	fieldSeek			(MYSQL_FIELD_OFFSET offset);
	/*--->[ �������ڽ�����������е���Ϣ(��һ���ֶ�) ]*/
	MYSQL_FIELD*		fieldFetch			();
	/*--->[ �������ĳ�е���Ϣ ]
	(fieldnr = 0��FieldCount-1)
	*/
	MYSQL_FIELD*		fieldFetchDirect	(unsigned int fieldnr);
	/*--->[ ���������ֶνṹ������ ]
	(ȡ�����[0��FieldCount-1].name)
	*/
	MYSQL_FIELD*		fieldFetchs			();
	/*--->[ ��������ݳ��� ]*/ 
	unsigned long		getFiledDataLength	(unsigned int uField);
	/*--->[ ������� ]*/ 
	const char*			getFiledName		(unsigned int uField)	{	MYSQL_FIELD* pField = fieldFetchDirect(uField);	return ((pField != NULL) ? pField->name : NULL);		}
	/*--->[ ����������� ]*/ 
	unsigned int		getFiledNameLength	(unsigned int uField)	{	MYSQL_FIELD* pField = fieldFetchDirect(uField);	return ((pField != NULL) ? pField->name_length : 0);	}
	/*--->[ ����п�� ]*/ 
	unsigned long		getFiledLength		(unsigned int uField)	{	MYSQL_FIELD* pField = fieldFetchDirect(uField);	return ((pField != NULL) ? pField->length : 0);			}
	/*--->[ ���������� ]*/ 
	unsigned long		getFiledMaxLength	(unsigned int uField)	{	MYSQL_FIELD* pField = fieldFetchDirect(uField);	return ((pField != NULL) ? pField->max_length : 0);		}
	/*--->[ �����λ��־(�ļ�ͷ) ]
	(IS_NOT_NULL(flags)	|	������ֶζ���ΪNOT NULL��Ϊ���桱)
	(IS_PRI_KEY(flags)	|	������ֶ���������Ϊ���桱)
	(IS_BLOB(flags)		|	������ֶ���BLOB��TEXT��Ϊ���桱���������ӣ��ò���field->typeȡ����֮)
	*/ 
	unsigned int		getFiledFlags		(unsigned int uField)	{	MYSQL_FIELD* pField = fieldFetchDirect(uField);	return ((pField != NULL) ? pField->flags : 0);					}
	/*--->[ ���������(�ļ�ͷ) ]*/ 
	enum_field_types	getFiledType		(unsigned int uField)	{	MYSQL_FIELD* pField = fieldFetchDirect(uField);	return ((pField != NULL) ? pField->type : MYSQL_TYPE_DECIMAL);	}
	//--- 
	const char*			getFiledDefaultValue(unsigned int uField)	{	MYSQL_FIELD* pField = fieldFetchDirect(uField);	return ((pField != NULL) ? pField->def : NULL);					}

public:
	/*--->[ ���������� ]
	(while(rowMore()){})
	*/ 
	bool				rowMore				();
	/*--->[ �Ƿ��ѵ������β ]*/ 
	bool				rowEof				();

	/*--->[ ȡ���ڼ������� ]
	(offset = 0��RowCount-1)
	*/
	bool				dataSeek			(my_ulonglong offset);
	/*--->[ ��ǰ�й�� ]*/ 
	MYSQL_ROWS*			rowTell				();
	/*--->[ �ƶ��й�� ]*/ 
	MYSQL_ROWS*			rowSeek				(MYSQL_ROWS*offset);

public:
	/*--->[ ��ȡ��ǰ���ֶ�����/�ַ������� ]*/ 
	const char*			getData				(unsigned int uField,const char*pDefault = NULL);
	/*--->[ ��ȡ��ǰ���ֶ�����/�ַ������� ]*/ 
	const char*			getString			(unsigned int uField,const char*pDefault = "")
	{	const char* pValue = getData(uField);	return (pValue ? pValue : pDefault);	}
	/*--->[ ��ò����ֶ����� ]*/ 
	inline bool			getBool				(unsigned int uField,bool bDefault = false)
	{	const char* pValue = getData(uField);	return (pValue ? static_cast<bool>(dAtoi(pValue) > 0) : bDefault);	}
	/*--->[ ��ø����ֶ����� ]*/ 
	inline float		getFloat			(unsigned int uField,float fDefault = 0.0f)
	{	const char* pValue = getData(uField);	return (pValue ? static_cast<float>(dAtof(pValue)) : fDefault);		}
	/*--->[ ���˫�����ֶ����� ]*/ 
	inline double		getDouble			(unsigned int uField,double dDefault = 0)
	{	const char* pValue = getData(uField);	return (pValue ? static_cast<double>(dAtof(pValue)) : dDefault);	}
	/*--->[ ���8λ�����ֶ����� ]*/ 
	inline int8			getInt8				(unsigned int uField,int8 iDefault = 0)
	{	const char* pValue = getData(uField);	return (pValue ? static_cast<int8>(dAtol(pValue)) : iDefault);		}
	/*--->[ ���8λ�޷��������ֶ����� ]*/ 
	inline uint8		getUInt8			(unsigned int uField,uint8 uDefault = 0)
	{	const char* pValue = getData(uField);	return (pValue ? static_cast<uint8>(dAtol(pValue)) : uDefault);		}
	/*--->[ ���16λ�������ֶ����� ]*/ 
	inline int16		getInt16			(unsigned int uField,int16 iDefault = 0)
	{	const char* pValue = getData(uField);	return (pValue ? static_cast<int16>(dAtol(pValue)) : iDefault);		}

	/*--->[ ���16λ�޷��Ŷ������ֶ����� ]*/ 
	inline uint16		getUInt16			(unsigned int uField,uint16 uDefault = 0)
	{	const char* pValue = getData(uField);	return (pValue ? static_cast<uint16>(dAtol(pValue)) : uDefault);	}
	/*--->[ ���32λ�����ֶ����� ]*/ 
	inline int32		getInt32			(unsigned int uField,int32 iDefault = 0)
	{	const char* pValue = getData(uField);	return (pValue ? static_cast<int32>(dAtoi(pValue)) : iDefault);		}

	/*--->[ ���32λ�޷��������ֶ����� ]*/ 
	inline uint32		getUInt32			(unsigned int uField,uint32 uDefault = 0)
	{	const char* pValue = getData(uField);	return (pValue ? static_cast<uint32>(dAtoull(pValue)) : uDefault);	}

	/*--->[ ���64λ�����ֶ����� ]*/ 
	inline int64		getInt64			(unsigned int uField,int64 iDefault = 0)
	{	const char* pValue = getData(uField);	return (pValue ? dAtoll(pValue) : iDefault);	}

	/*--->[ ���64λ�޷��������ֶ����� ]*/ 
	inline uint64	getUInt64(unsigned int uField,uint64 uDefault = 0)
	{	const char* pValue = getData(uField);	return (pValue ? dAtoull(pValue) : uDefault);	}
};

/*************************************************************
* ˵�� : ���Ԥ����
* �÷� :*�ڽ������Ԥ����ʱ��Ҫ����������ѯ����close()��
*		1������cmd()Ԥ������䷵�ؿɰ󶨲�������
		2������Bind...()�󶨸��������ݻ���
		3�����ð󶨲�����������ֵ
		4������execute()ִ����䣬�����ٴ�ִ�н�����ظ�3��4��
		5������getRowAffected()�鿴Ӱ������
*************************************************************/
class CMySQLStatement
{
private:
protected:
	MYSQL_STMT*		m_pStatement;	/*Ԥ���������*/ 

protected:
	MYSQL_BIND*		m_pParamBind;	/*��󶨲�������*/ 
	unsigned long*	m_pParamLength;	/*��������������*/ 
	unsigned long	m_ulParamCount;	/*��󶨲�������*/ 

protected:
	my_ulonglong	m_ulRowCount;	/*���������*/ 
	MYSQL_BIND*		m_pResultBind;	/*��󶨽��������*/ 
	unsigned long	m_ulResultCount;/*��󶨽��������*/ 

public:
	CMySQLStatement(MYSQL_STMT*pStatement = NULL);
	virtual~CMySQLStatement();

public:
	/*--->[ ���Ԥ������ ]*/ 
	inline MYSQL_STMT*		getMYSQL_STMT	()	{	return m_pStatement;	}
	/*--->[ ��ð󶨲������� ]*/ 
	inline unsigned long	getParamCount	()	{	return m_ulParamCount;	}
	/*--->[ ��ð󶨽�������� ]*/ 
	inline unsigned long	getResultCount	()	{	return m_ulResultCount;	}

public:
	CMySQLStatement&		operator=		(CMySQLStatement& clMySQLStatement);

protected:
	/*--->[ �ͷŰ󶨻��� ]*/ 
	void			freeBind			();

public:
	/*--->[ �ر�Ԥ������� ]*/ 
	bool			close				();
	/*--->[ �ͷ���ִ��Ԥ����������ɵĽ�����йص��ڴ� ]*/ 
	bool			freeResult			();

public:
	/*--->[ ��ʽ�����ݿ�ָ�׼�� ]
	(��ӦΪ�������ս�ֺţ���;������\g��)
	*/ 
	bool			cmd					(const char*pCmd,...);
	/*--->[ ִ��(execute) ]
	(�ǲ�ѯ��ִ��֮ǰ�����в���,�������bindParams())
	(��д���в�������+sendLongData()���)
	(ÿ����һ��ִ��һ�ν��)
	*/ 
	bool			execute				();
	/*--->[ Ԥ������临λΪ���׼�����״̬ ]
	(�������ͳ����ݵ�������)
	*/ 
	bool			reset				();
	/*--->[ ���Ӱ��/�ı����� ]*/ 
	my_ulonglong	getRowAffected		();
	/*--->[ ���AUTO_INCREMENT�����ɵ�ID ]*/ 
	my_ulonglong	getInsertIncrement	();

public:
	/*--->[ �󶨲��� ]
	(������а󶨲�������ý������ΰ�)
	*/ 
	bool			bindParams			();
	/*--->[ ���ͳ����ݵ������� ]
	(�����Ȱ󶨲���bindParams(),�ɶ�TEXT��BLOB���������ֶν��ж�����ݷ������)
	(parameter_number = �������,��0��ʼ)
	(data,length = ���ݻ����������γ���)
	*/ 
	bool			sendLongData		(unsigned int parameter_number,const char*data,unsigned long length);

public:	
	/*--->[ �󶨽���� ]
	(����ִ�к����)
	(������а󶨽������ý������ΰ�)
	*/ 
	bool			bindResults			();
	/*--->[ �Ƿ���ڽ�� ]
	(while(rowMore()){})
	*/ 
	bool			rowMore				();
	/*--->[ ȡ���ڼ������� ]
	(offset = 0��RowCount-1)
	*/
	bool			dataSeek			(my_ulonglong offset);
	/*--->[ �ӵ�ǰ������л�ȡ1�� ]
	(column = �б��,��0��ʼ)
	(offset = ����ֵ�ڵ�ƫ���������Ӹô���ʼ�������ݡ�)
	*/ 
	bool			getColume			 (MYSQL_BIND *bind, unsigned int column, unsigned long offset = 0);

public:/*�󶨴��������ִ��ǰ*/ 
	/*--->[ �󶨲��� ]*/ 
	bool			bindParam			(unsigned int uField,enum_field_types buffer_type,void*buffer,unsigned long length,my_bool*is_null=NULL);

	/*--->[ ��λ�ֶ����� ]*/ 
	inline bool		bindParam_Bit		(unsigned int uField,uint8*buffer ,my_bool*is_null=NULL)
	{	return bindParam(uField,MYSQL_TYPE_BIT,buffer,0,is_null);			}
	/*--->[ ������ʱ���ֶ����� ]*/ 
	inline bool		bindParam_DateTime	(unsigned int uField,MYSQL_TIME*buffer,my_bool*is_null=NULL)
	{	return bindParam(uField,MYSQL_TYPE_DATETIME,buffer,0,is_null);		}
	/*--->[ ���ַ����ֶ����� ]*/ 
	inline bool		bindParam_String	(unsigned int uField,char*buffer,unsigned long length,my_bool*is_null=NULL)
	{	return bindParam(uField,MYSQL_TYPE_STRING,buffer,length,is_null);	}
	/*--->[ �󶨶������ֶ����� ]*/ 
	inline bool		bindParam_Blob		(unsigned int uField,void*buffer,unsigned long length,my_bool*is_null=NULL)
	{	return bindParam(uField,MYSQL_TYPE_BLOB,buffer,length,is_null);		}

	/*--->[ �����Ͳ��� ]*/ 
	bool			bindParam_Int		(unsigned int uField,enum_field_types buffer_type,my_bool is_unsigned,void*buffer,my_bool*is_null=NULL);
	/*--->[ �󶨲����ֶ����� ]*/ 
	inline bool		bindParam_Bool		(unsigned int uField,bool*buffer,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_TINY,true,buffer,is_null);			}
	/*--->[ �󶨸����ֶ����� ]*/ 
	inline bool		bindParam_Float		(unsigned int uField,float*buffer,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_FLOAT,false,buffer,is_null);			}
	/*--->[ ��˫�����ֶ����� ]*/ 
	inline bool		bindParam_Double	(unsigned int uField,double*buffer ,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_DOUBLE,false,buffer,is_null);		}
	/*--->[ ��8λ�з��������ֶ����� ]*/ 
	inline bool		bindParam_Int8		(unsigned int uField,int8*buffer ,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_TINY,false,buffer,is_null);			}
	/*--->[ ��8λ�޷��������ֶ����� ]*/ 
	inline bool		bindParam_UInt8		(unsigned int uField,uint8*buffer ,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_TINY,true,buffer,is_null);			}
	/*--->[ ��16λ�з��������ֶ����� ]*/ 
	inline bool		bindParam_Int16		(unsigned int uField,int16*buffer,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_SHORT,false,buffer,is_null);			}
	/*--->[ ��16λ�޷��������ֶ����� ]*/ 
	inline bool		bindParam_UInt16	(unsigned int uField,uint16*buffer,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_SHORT,true,buffer,is_null);			}
	/*--->[ ��32λ�з��������ֶ����� ]*/ 
	inline bool		bindParam_Int32		(unsigned int uField,int32*buffer,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_LONG,false,buffer,is_null);			}
	/*--->[ ��32λ�޷��������ֶ����� ]*/ 
	inline bool		bindParam_UInt32	(unsigned int uField,uint32*buffer,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_LONG,true,buffer,is_null);			}
	/*--->[ ��64λ�з��������ֶ����� ]*/ 
	inline bool		bindParam_Int64		(unsigned int uField,int64*buffer,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_LONGLONG,false,buffer,is_null);		}
	/*--->[ ��64λ�޷��������ֶ����� ]*/ 
	inline bool		bindParam_UInt64	(unsigned int uField,uint64*buffer,my_bool*is_null=NULL)
	{	return bindParam_Int(uField,MYSQL_TYPE_LONGLONG,true,buffer,is_null);		}

public:/*�󶨴����������ִ�к�*/ 
	/*--->[ �󶨽�� ]*/ 
	bool			bindResult			(unsigned int uField,enum_field_types buffer_type,void*buffer,unsigned long buffer_length,my_bool*is_null=NULL);
	/*--->[ �󶨽��λ�ֶ����� ]*/ 
	inline bool		bindResult_Bit		(unsigned int uField,uint8*buffer ,my_bool*is_null=NULL)
	{	return bindResult(uField,MYSQL_TYPE_BIT,buffer,0,is_null);					}
	/*--->[ �󶨽������ʱ���ֶ����� ]*/ 
	inline bool		bindResult_DateTime	(unsigned int uField,MYSQL_TIME*buffer ,my_bool*is_null=NULL)
	{	return bindResult(uField,MYSQL_TYPE_DATETIME,buffer,0,is_null);				}
	/*--->[ �󶨽���ַ����ֶ����� ]*/ 
	inline bool		bindResult_String	(unsigned int uField,char*buffer,unsigned long buffer_length,my_bool*is_null=NULL)
	{	return bindResult(uField,MYSQL_TYPE_STRING,buffer,buffer_length,is_null);	}
	/*--->[ �󶨽���������ֶ����� ]*/ 
	inline bool		bindResult_Blob		(unsigned int uField,void*buffer,unsigned long buffer_length,my_bool*is_null=NULL)
	{	return bindResult(uField,MYSQL_TYPE_BLOB,buffer,buffer_length,is_null);		}

	/*--->[ �󶨽������ ]*/ 
	bool			bindResult_Int		(unsigned int uField,enum_field_types buffer_type,my_bool is_unsigned,void*buffer,my_bool*is_null=NULL);
	/*--->[ �󶨽�������ֶ����� ]*/ 
	inline bool		bindResult_Bool		(unsigned int uField,bool*buffer,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_TINY,true,buffer,is_null);			}
	/*--->[ �󶨽�������ֶ����� ]*/ 
	inline bool		bindResult_Float	(unsigned int uField,float*buffer,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_FLOAT,false,buffer,is_null);		}
	/*--->[ �󶨽��˫�����ֶ����� ]*/ 
	inline bool		bindResult_Double	(unsigned int uField,double*buffer ,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_DOUBLE,false,buffer,is_null);		}
	/*--->[ �󶨽��8λ�з��������ֶ����� ]*/ 
	inline bool		bindResult_Int8		(unsigned int uField,int8*buffer ,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_TINY,false,buffer,is_null);			}
	/*--->[ �󶨽��8λ�޷��������ֶ����� ]*/ 
	inline bool		bindResult_UInt8	(unsigned int uField,uint8*buffer ,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_TINY,true,buffer,is_null);			}
	/*--->[ �󶨽��16λ�з��������ֶ����� ]*/ 
	inline bool		bindResult_Int16	(unsigned int uField,int16*buffer,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_SHORT,false,buffer,is_null);		}
	/*--->[ �󶨽��16λ�޷��������ֶ����� ]*/ 
	inline bool		bindResult_UInt16	(unsigned int uField,uint16*buffer,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_SHORT,true,buffer,is_null);			}
	/*--->[ �󶨽��32λ�з��������ֶ����� ]*/ 
	inline bool		bindResult_Int32	(unsigned int uField,int32*buffer,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_LONG,false,buffer,is_null);			}
	/*--->[ �󶨽��32λ�޷��������ֶ����� ]*/ 
	inline bool		bindResult_UInt32	(unsigned int uField,uint32*buffer,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_LONG,true,buffer,is_null);			}
	/*--->[ �󶨽��64λ�з��������ֶ����� ]*/ 
	inline bool		bindResult_Int64	(unsigned int uField,int64*buffer,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_LONGLONG,false,buffer,is_null);		}
	/*--->[ �󶨽��64λ�޷��������ֶ����� ]*/ 
	inline bool		bindResult_UInt64	(unsigned int uField,uint64*buffer,my_bool*is_null=NULL)
	{	return bindResult_Int(uField,MYSQL_TYPE_LONGLONG,true,buffer,is_null);		}
};
//****************************************************************
#ifdef __cplusplus
}
#endif
