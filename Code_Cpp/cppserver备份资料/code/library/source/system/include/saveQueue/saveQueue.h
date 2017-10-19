/*----------------- saveQueue.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/2/14 15:19:29
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "memoryPool/hashMapPool.h"
#include "timer/timers.h"
#include "stream/bufferStream.h"
#include "saveQueue/dbEventStruct.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 对象计数器
template <typename Key>
class map_count
{
public:
	typedef	stl_map<Key,uint32>	MAP_COUNT;

protected:
	MAP_COUNT		m_count;

public:
	virtual~map_count(){}

public:
	inline uint32	find_count	(Key _object)
	{
		typename MAP_COUNT::iterator pos = m_count.find(_object);
		return ((pos != m_count.end()) ? (pos->second): 0);
	}
	inline void		add_count	(Key _object)
	{
		typename MAP_COUNT::_Pairib _pai = m_count.insert_(_object,0);
		if(_pai.first != m_count.end())
			((_pai.first)->second)++;
	}
	inline bool		del_count	(Key _object)
	{
		typename MAP_COUNT::iterator pos = m_count.find(_object);
		if(pos == m_count.end())
			return true;

		uint32&_count = pos->second;
		if(_count > 1)
		{
			_count--;
			return false;
		}
		m_count.erase(_object);
		return true;
	}
};

//-------------------------------------------------------------
//------------------------------ 保存队列
class CSaveQueue	: public CTimerObject
{
protected:
	struct _stDBQueue : _stDBBlock
	{
		time_t	tSendTime;

		_stDBQueue()				{	initialize();					}
		_stDBQueue&	getDBQueue	()	{	return *this;					}
		inline void	initialize	()	{	memset(this,0,sizeof(*this));	}
	};
protected:
	typedef CHashMapPool<uint64,_stDBQueue>	MAP_BLOCK;
	typedef stl_set<uint8>					COUNT_EVENT;

protected:
	uint32				m_uSaveOrder;
	MAP_BLOCK			m_mapBlock;
	COUNT_EVENT			m_countEvent;	//计数事件
	map_count<uint32>	m_mapCount;		//计数器

public:
	explicit CSaveQueue();
	virtual~CSaveQueue(){}

public:
	inline uint32	getQueueCount	()					{	return m_mapBlock.size();									}
	inline void		save_begin		(uint32 uObjectID)	{	return m_mapCount.add_count(uObjectID);						}
	inline bool		save_end		(uint32 uObjectID)	{	return m_mapCount.del_count(uObjectID);						}
	inline bool		is_save_done	(uint32 uObjectID)	{	return (m_mapCount.find_count(uObjectID) == 0);				}
	inline bool		is_count_event	(uint8 uEvent)		{	return (m_countEvent.find(uEvent) != m_countEvent.end());	}
	inline void		set_count_event	(uint8 uEvent)		{	m_countEvent.insert(uEvent);								}

protected:
	bool		insertQueue		(uint8 uEventFlag,uint8 uEvent,uint32 uObjectID,const void* pData,uint32 uSize,uint8 uSubEvent = 0,uint8 uEventPri = EventPri_Mid,bool bNow = false,uint32 uWait = 0);

public:
	bool		insertRead		(uint8 uEvent,uint32 uObjectID,CBufferStream&clStream,uint8 uSubEvent = 0,uint8 uEventPri = EventPri_Mid,bool bNow = false,uint32 uWait = 0);
	bool		insertRead		(uint8 uEvent,uint32 uObjectID,const void* pData,uint32 uSize,uint8 uSubEvent = 0,uint8 uEventPri = EventPri_Mid,bool bNow = false,uint32 uWait = 0);
	bool		insertWrite		(uint8 uEvent,uint32 uObjectID,CBufferStream&clStream,uint8 uSubEvent = 0,uint8 uEventPri = EventPri_Mid,bool bNow = false,uint32 uWait = 0);
	bool		insertWrite		(uint8 uEvent,uint32 uObjectID,const void* pData,uint32 uSize,uint8 uSubEvent = 0,uint8 uEventPri = EventPri_Mid,bool bNow = false,uint32 uWait = 0);

public:
	_stDBBlock*	findQueue		(uint8 uEvent,uint32 uObjectID,uint8 uSubEvent = 0);
	void		removeQueue		(_stDBEvent* pEvent);

protected:
	virtual bool	canProcess		() = 0;
	virtual void	onProcessQueue	(_stDBQueue& stBlock,time_t tTime) = 0;
	virtual void	callbackSaveDone(uint32 uObjectID){};

public:
	virtual void	timerProcess();
};
