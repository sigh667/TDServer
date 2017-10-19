/*----------------- wordFilterData.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/2/6 18:19:31
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/stringFunctions.h"
#include "stl/stl_vector.h"
#include "cfg_data/fileData/fileData.h"
#include "cfg_data/wordFilter/wordFilterModel.h"
#include "common/darts.h"
/*************************************************************/
class CSensitiveWords
{
private:
	Darts::DoubleArray m_clDarts;

public:
	virtual ~CSensitiveWords()	{	clear();	};

	void build(std::vector<const char *> keys)
	{
		m_clDarts.build(keys.size(), (keys.size() > 0) ? &keys[0] : NULL);
	}

	void clear()
	{
		m_clDarts.clear();
	}

	bool match(const std::string & input)
	{
		// ����жϺ���Ҫ�����û��build()ֱ�ӽ����жϻ��쳣
		if (m_clDarts.size() <= 0)
			return false;
		std::wstring ws = convertToWString(input.c_str());
		static const std::size_t MAX_NUM_RESULTS = 1;
		int results[MAX_NUM_RESULTS] = {0};
		const char *ptr = input.c_str();
		for (size_t i = 0; i < ws.length(); i++)
		{
			if (m_clDarts.commonPrefixSearch(ptr, results, MAX_NUM_RESULTS) > 0)
			{
				return true;
			}
			else
			{
				// ��ΪDarts�ڲ�ʵ���õ�char�����ַ�������ƥ�䣬��������ı����е���� ��Ϊ1�������Ƕ��char��ʶ���������ֵ�����char�ܿ��ܱ����С�
				// �����ת�ɿ��ֽ��ַ���wstring�������ÿ����ռ����Ч�ֽڣ�ASCII�ַ�->1�ֽڣ�����->2�ֽڡ�
				// �����ַ���input����ƫ�ƽ��бȶԣ����Ƿ���ǰ׺ƥ��
				ptr += (ws.at(i) <= 0x7F) ? 1 : 2;
			}
		}
		return false;
	}
};

//-------------------------------------------------------------
class CWordFilterData	: public CFileData
{
protected:
	enum
	{
		Version		= 20170322
	};

public:
	typedef stl_vector<std::wstring>						VEC_FILTERMODEL;

private:
	VEC_FILTERMODEL	m_vecModel;

public:
	explicit		CWordFilterData();
	virtual			~CWordFilterData();
	virtual pc_str	getFileName	()const				{	return "wordfilter.dat";	}
	virtual pc_str	getXmlName	()const				{	return "cs_wordfilter.xml";	}

public:
	bool haveBadWord		(const char *pStr);
	void addBadWord			(const char* pWord,bool bUtf8);

protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CWordFilterData	g_clWordFilterData;
