/*----------------- fileData.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/9/22 11:15:21
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "network/include/socket.h"
#include "stream/fileStream.h"
#include "stl/std_hash_map.h"
#include "dbMysql/include/dbMySql.h"
#include "cfg_data/tinyxml/tinyxml.h"
/*************************************************************/
class CFileData
{
private:
	enum
	{
		_FileFlagMax	= 16
	};
protected:
	uint32	m_uVersion;					//�ļ��汾��
	uint32	m_uRevision;				//�ļ������汾��
	pc_str	m_pExportTextPath;			//�����ļ�·��
	pc_str	m_pExportSqlPath;			//����Sql�ļ�·��
	pc_str	m_pReadBinaryPath;			//��ȡ��excel·��

public:
	CFileData(uint32 uVersion = 0);
	virtual~CFileData(){}
	pc_str	getBinaryPath	()const					{  return m_pReadBinaryPath;				};
	void	setBinaryPath	(pc_str pReadBinaryPath){  m_pReadBinaryPath = pReadBinaryPath;		};
public:
	virtual pc_str	getFileName	()const		= 0 ;
	virtual pc_str	getXmlName	()const		= 0 ;
	std::string	getXlsName	();
public:
	virtual void	initFile	();

public:
	virtual bool	loadFile				(pc_str pFileName);
	virtual bool	saveFile				(pc_str pFileName);
protected:
	virtual bool	onLoad					(CFileStream&clFile);
	virtual bool	onSave					(CFileStream&clFile);
public:
	virtual bool	loadXmlFile				(pc_str pFileName);
public:
	virtual bool	onLoad					(TiXmlElement&xmlRoot);
public:
	virtual bool	exportText				(CDBMySql&dbMysql,pc_str pFilePath);
	virtual bool	exportSql				(CDBMySql&dbMysql,pc_str pFilePath);
	virtual bool	loadDB					(CDBMySql&dbMysql);
	virtual bool	onLoad					(CDBMySql&dbMysql);
	virtual bool	onExport				(CDBMySql&dbMysql);
	virtual bool	onExportSql				(CDBMySql&dbMysql);
	virtual bool	table2Text				(CDBMySql&dbMysql,pc_str pTableName);
public:
	virtual void	onCompletioned();
};
