/*----------------- wordFilterData.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
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
		// 这个判断很重要，如果没有build()直接进行判断会异常
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
				// 因为Darts内部实现用的char保存字符串来做匹配，会存在中文被误判的情况 因为1个中文是多个char标识，跨中文字的连续char很可能被误判。
				// 这里简单转成宽字节字符串wstring，计算出每个字占的有效字节，ASCII字符->1字节，其他->2字节。
				// 输入字符串input按字偏移进行比对，看是否有前缀匹配
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
