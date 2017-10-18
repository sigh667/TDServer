/*----------------- operator_result.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/6/7 16:51:13
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "network/include/network.h"
#include "stl/std_map.h"
/*************************************************************/
class COperatorsResult;
/*-------------------------------------------------------------
**链接包管理
*/
class COperatorsResultMgr
{
public:
	COperatorsResultMgr(){}
	virtual~COperatorsResultMgr(){}

public:
	virtual void	freeResultPacket(COperatorsResult*_result) = 0;
};
/*-------------------------------------------------------------
**应答包处理基类
*/
class COperatorsResult
{
protected:
	uint16				m_world_id;
	uint32				m_account_id;
	uint32				m_player_id;
	uint64				m_context_id;
protected:
	std::string			m_send;
	COperatorsResultMgr*m_result_mgr;

public:
	inline void		set_world_id	(uint16 _id)							{	m_world_id	= _id;		}
	inline void		set_account_id	(uint32 _id)							{	m_account_id= _id;		}
	inline void		set_player_id	(uint32 _id)							{	m_player_id = _id;		}
	inline void		set_context_id	(uint64 _id)							{	m_context_id= _id;		}
	inline uint16	get_world_id	()const									{	return m_world_id;		}
	inline uint32	get_account_id	()const									{	return m_account_id;	}
	inline uint32	get_player_id	()const									{	return m_player_id;		}
	inline uint64	get_context_id	()const									{	return m_context_id;	}

public:
	inline std::string&			get_send		()							{	return m_send;			}
	inline uint32				get_send_length	()const						{	return m_send.length();	}
	inline COperatorsResultMgr*	get_result_mgr	()							{	return m_result_mgr;	}

public:
	COperatorsResult(){initResult(NULL);}
	virtual~COperatorsResult(){}

public:
	virtual void	initResult	(COperatorsResultMgr*_mgr);

public:
	virtual void	onResult	(pc_str _content,uint32 _length)=0;
};
//#############################################################
//##############################
//#############################################################
namespace operators
{
	void	set_logic_lock(CMutex_*_mutex);

	/*-------------------------------------------------------------
	**链接管理基类
	*/
	class COperatorConnect : public tcp_network
	{
	public:
		typedef stl_map<tcp_context*,COperatorsResult*>MAP_RESULT;

	protected:
		MAP_RESULT	m_mapResult;

	public:
		virtual~COperatorConnect(){}

	public:
		//--- 
		bool			connect			(COperatorsResult*_result,uint16 uPort,pc_str pAddress);
	public:
		//--- 链接回调
		virtual void	onConnect		(tcp_context* pContext);
		//--- 断开
		virtual void	onDisconnect	(tcp_context* pContext);
		/*-----[ 超时通知 ]*/ 
		virtual void	onTimeoutRead	(tcp_context* pContext);
		/*----->{ 发送超时 }*/ 
		virtual void	onTimeoutWrite	(tcp_context* pContext);
		//--- 处理包
		virtual bool	onProcessPackage(tcp_context* pContext,CNetBuffer* pBuffer){return true;}
	};
};
