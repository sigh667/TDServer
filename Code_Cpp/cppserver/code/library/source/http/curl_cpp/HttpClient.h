/*----------------- HttpClient.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2016/8/26 20:29:03
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include <vector>
#include <list>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpProcessor.h"
#include "threads/condEvent.h"
#include "memoryPool/memoryPool.h"
#include "threads/csLocker.h"

class HttpThread;
class HttpClient
{
	template<class _Ty>
	class mutex_list	: private std::list<_Ty>
	{
		typedef std::list<_Ty>	Parent;

		uint32			m_uSize;	// Ϊ���̰߳�ȫ�ķ��� �б��Ƿ�Ϊ�� 
		CMutex_			m_clMutex;
	public:
		mutex_list()
		{
			m_uSize = 0;
		}
		bool	isEmpty	()
		{
			return m_uSize <= 0;
		}

		void	pusk	(_Ty data)
		{
			if ( !data )
				return ;

			CCritLocker	clLock(m_clMutex);
			Parent::push_back(data);
			++m_uSize;
		}

		_Ty		pop		()
		{
			CCritLocker	clLock(m_clMutex);
			_Ty ret = _Ty();
			if (!isEmpty())
			{
				ret = Parent::front();
				Parent::pop_front();
				--m_uSize;
			}
			return ret;
		}
	};

protected:
	friend				class						HttpProcessor;
	typedef				std::vector<HttpProcessor*>	VECTOR_THREAD;
	typedef				mutex_list<HttpRequest*>	LIST_REQUEST;
	typedef				mutex_list<HttpResponse*>	LIST_RESPONSE;

	typedef				CMemoryPool<HttpResponse>	RESPONSE_POOL;
protected:
	RESPONSE_POOL		m_poolResponse;

	CCondEvent			m_clExitEvent;			//�˳�ѭ���¼�
	VECTOR_THREAD		m_vecWorkThread;		//�����̳߳�

	LIST_REQUEST		m_listRequest;

	LIST_RESPONSE		m_listResponse;

	uint32				m_timeoutForConnect;
	uint32				m_timeoutForRead;
protected:
	volatile bool		m_bStarted;				//�Ƿ�������
	volatile bool		m_bShutdown;			//�Ƿ�ر�

public:
	inline bool			isStarted				()				{	return m_bStarted;					}

	inline void			setTimeoutForConnect	(uint32 value)	{	m_timeoutForConnect = value;		}
	inline uint32		getTimeoutForConnect	()				{	return m_timeoutForConnect;			}

	inline void			setTimeoutForRead		(uint32 value)	{	m_timeoutForRead = value;		}
	inline uint32		getTimeoutForRead		()				{	return m_timeoutForRead;			}
public:
	HttpClient();
	virtual~HttpClient();
public:
	//-----��������
	virtual bool		start					();
	//-----�رշ���
	virtual void		shutdown				();
public:
	//Ͷ������
	virtual bool		postHttpRequest			(HttpRequest* pRequest);
	//����Ӧ��
	virtual void		processHttpResponse		(HttpResponse*pResponse)			= 0;
	//�߼��ƽ�
	virtual	void		process					();
protected:
	//Ͷ��Ӧ��
	virtual void		postHttpResponse		(HttpResponse* pResponse);

	virtual	HttpProcessor*	newProcessor		();
	virtual	void			deleteProcessor		(HttpProcessor* pProcessor);
private:
	bool				setup_workers			();
	void				shutdown_workers		();

	void				clear_request_response	();
private:
	static THREAD_RETURN	_working_thread_	(void* pParam);
};
