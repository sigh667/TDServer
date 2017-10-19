/*------------- jobData.h
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.01
* Date         : 2016/02/23
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "cfg_data/fileData/fileData.h"
#include "jobModel.h"
//-------------------------------------------------------------
//------------------------------ 
class CJobData	: public CFileData
{
protected:
	enum
	{
		Version		= 20160223
	};

public:
	typedef stl_map<uint8, _stJobModel>			MAP_MODEL;

private:
	MAP_MODEL	m_mapModel;

public:
	CJobData();
	virtual pc_str	getFileName	()const				{	return "job.dat";						}
	virtual pc_str	getXmlName	()const				{	return "cs_job.xml";					}

public:
	const _stJobModel*		findJobModel(uint8 uJob) const	{	return m_mapModel.find_(uJob);	}

protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);
	virtual bool	load_job	(TiXmlElement&xmlRoot);
	virtual bool	load_anim	();

protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern 	CJobData	g_clJobData;
