/*------------- gameHurtMgr.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/04/21
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "basic/memoryFunctions.h"
#include "stl/std_map.h"
/*************************************************************/
struct _stHurt
{
	uint32	uObjectID;	//playerID
	uint64	uHurt;		//伤害值
	uint32	uHurtTime;	//最后一次伤害时间

	_stHurt()			{	dMemset(this, 0, sizeof(_stHurt));	}
};

struct _stHurtSort
{
	bool  operator() (const _stHurt* left, const _stHurt* right) const
	{
		if (!left || !right)
			return false;

		if ( right->uHurt != left->uHurt )
		{
			return right->uHurt < left->uHurt;
		}
		return left < right;
	}
};

typedef stl_map<uint32, _stHurt>		MAP_HURT;
typedef stl_map<uint32, uint64>			MAP_U32_U64;
class CSceneUnit;
class CScenePlayer;
class CGameHurtMgr
{
private:
	CSceneUnit*		m_pOwner;		//拥有者
	CScenePlayer*	m_pFirstPlayer;	//第一刀的玩家
	MAP_HURT		m_mapHurt;		//伤害列表
	uint64			m_uHurtSum;		//总伤害

public:
	inline void		bindOwner		(CSceneUnit* pOwner)		{		m_pOwner = pOwner;				}
	inline MAP_HURT&getHurtMap		()							{		return m_mapHurt;				}
	inline uint64	getHurtSum		()							{		return m_uHurtSum;				}
	CScenePlayer*	getHurtFirstPlayer()						{		return m_pFirstPlayer;			}

	void			initialize		(CSceneUnit* pUnit);
	void			clear			();

	void			clearHurt		();
	void			updateHurt		(CScenePlayer* pPlayer, int32 nHP);
	float32			getHurtRatio	(uint64 uHurt);
	CScenePlayer*	getHurtMaxPlayer();

public:
	CGameHurtMgr();
	~CGameHurtMgr();
};
