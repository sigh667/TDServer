/*----------------- tickdown.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ¡÷æ‘
* Version      : V1.0
* Date         : 2016/11/21 20:01:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/stl_list.h"
#include "log/log.h"
#include "basic/timeFunction.h"
#include "basic/systemFunction.h"
#include <functional>
/*************************************************************/
class CTickDown
{
protected:
	struct _stTickDown
	{
		int32							iInterval;
		int64							iTick;
		std::function<void (uint64)>	pHandler;
	};
	stl_list<_stTickDown>				m_list;
	int64								m_lastTick;

public:
	explicit CTickDown()
	{
		initialize();
	}

	virtual ~CTickDown()
	{
		m_list.clear();
	}

	virtual void initialize()
	{
		m_lastTick = (int64)getTickCount64();
		m_list.clear();
	}

public:
	void	tick(int32 iInterval, const std::function<void (uint64)> &pTickHandler, bool bRandomTick = false)
	{
		CHECK(iInterval > 0 && nullptr != pTickHandler)
		_stTickDown st;
		st.iInterval= iInterval;
		st.iTick	= bRandomTick ? randRange(iInterval) : iInterval;
		st.pHandler	= pTickHandler;
		m_list.push_back(st);
	}

	void	process(int64 iTick)
	{
		if (iTick <= 0 || iTick <= m_lastTick)
			return;

		for (auto &t : m_list)
		{
			int64 iPass = iTick - m_lastTick;
			if (t.iTick > iPass)
			{
				t.iTick -= iPass;
			}
			else
			{
				while (true)
				{
					iPass -= t.iTick;
					if (iPass >= 0)
					{
						t.pHandler((uint64)iTick);
						t.iTick = t.iInterval;
					}
					else
					{
						t.iTick = -iPass;
						break;
					}
				}
			}
		}
		m_lastTick = iTick;
	}
};