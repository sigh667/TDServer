/*----------------- operator_result.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
**���Ӱ�����
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
**Ӧ����������
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
	**���ӹ������
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
		//--- ���ӻص�
		virtual void	onConnect		(tcp_context* pContext);
		//--- �Ͽ�
		virtual void	onDisconnect	(tcp_context* pContext);
		/*-----[ ��ʱ֪ͨ ]*/ 
		virtual void	onTimeoutRead	(tcp_context* pContext);
		/*----->{ ���ͳ�ʱ }*/ 
		virtual void	onTimeoutWrite	(tcp_context* pContext);
		//--- �����
		virtual bool	onProcessPackage(tcp_context* pContext,CNetBuffer* pBuffer){return true;}
	};
};
