/*----------------- questionData.h
*
* Copyright (C): 2017  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2017/4/26 11:39:19
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "cfg_data/fileData/fileData.h"
#include "questionModel.h"
//-------------------------------------------------------------
//------------------------------ 
class CQuestionData	: public CFileData
{
protected:
	enum
	{
		Version		= 20170426
	};

public:
	typedef stl_map<uint32, _stQuestionModel>	MAP_Question_MODEL;
public:
	MAP_Question_MODEL			m_mapQuestionModel;
	_stQuestionConifigModel		m_stQuestionConifigModel;
public:
	CQuestionData();
	virtual pc_str	getFileName	()const				{	return "question.dat";						}
	virtual pc_str	getXmlName	()const				{	return "cs_question.xml";					}

public:
	const _stQuestionModel*			findQuestionModel			(uint32 uID) const	{	return m_mapQuestionModel.find_(uID);			}
	const _stQuestionConifigModel*	findQuestionConifigModel	() const			{	return &m_stQuestionConifigModel;				}

protected:
	virtual bool	onLoad			(TiXmlElement&xmlRoot);

	virtual bool	load_question	(TiXmlElement&xmlRoot);
	virtual bool	load_conifg		(TiXmlElement&xmlRoot);
protected:
	virtual bool	onLoad			(CFileStream&clFile);
	virtual bool	onSave			(CFileStream&clFile);
public:
	virtual	void		onCompletioned	();
};
//-------------------------------------------------------------
extern 	CQuestionData	g_clQuestionData;
