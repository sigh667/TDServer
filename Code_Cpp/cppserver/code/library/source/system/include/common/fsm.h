/*----------------- fsm.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ¡÷æ‘
* Version      : V1.0
* Date         : 2016/11/21 20:01:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "log/log.h"
#include <functional>
/*************************************************************/
struct IState
{
	std::function<void (void)>	OnEnter;
	std::function<void (void)>	OnUpdate;
};

// -------------------------------------------------------------------

class FSM 
{ 
private: 
	int			m_iCurID;
	stl_map<int, IState>	m_map;

public: 
	explicit FSM()
		: m_iCurID(0)
	{
	}
	static void _default()	{}

	virtual ~FSM()
	{
		m_map.clear();
	}

	virtual void initState()	= 0;

	virtual void addState(int id,
		std::function<void (void)> onEnter	= _default,
		std::function<void (void)> onUpdate	= _default)
	{
		CHECK(!m_map.is_exist(id));
		IState s = {onEnter, onUpdate};
		m_map[id] = s;
	}

	virtual void update()const 
	{
		const IState *s = m_map.find_(m_iCurID);
		if (s) 
		{ 
			s->OnUpdate(); 
		} 
	}

	virtual void setState(int id) 
	{
		if (m_iCurID == id)
			return;

		const IState *old_state = m_map.find_(m_iCurID);
		const IState *new_state = m_map.find_(id);
		CHECK(NULL != new_state);
		m_iCurID = id;
		if (NULL != new_state->OnEnter)
		{
			new_state->OnEnter(); 
		}
	}

	virtual int stateID() const 
	{ 
		return m_iCurID;  
	}
}; 
