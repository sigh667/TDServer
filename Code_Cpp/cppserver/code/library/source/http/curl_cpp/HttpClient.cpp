/*----------------- HttpClient.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/8/27 10:07:50
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "HttpClient.h"
#include "basic/timeFunction.h"
#include "stackChecker/stackChecker.h"
//-------------------------------------------------------------
//------------------------------ 
HttpClient::HttpClient()
{
	m_bStarted			= false;
	m_bShutdown			= true;

	m_timeoutForConnect	= 30;
	m_timeoutForRead	= 30;
}
//-------------------------------------------------------------
//------------------------------ 
HttpClient::~HttpClient()
{
	shutdown();
}
//-------------------------------------------------------------
//------------------------------ 启动服务
bool	HttpClient::start		()
{
	//首先调用关闭 然后重新启动
	shutdown();

	m_bShutdown		= false;
	if(!setup_workers())
	{
		shutdown();
		return false;
	}
	m_bStarted		= true;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 关闭服务
void	HttpClient::shutdown	()
{
	if ( !m_bStarted )
		return ;

	m_bShutdown		= true;

	shutdown_workers();
	clear_request_response();
	m_bStarted		= false;
}
//-------------------------------------------------------------
//------------------------------
void	HttpClient::clear_request_response	()
{
	while (HttpResponse * pResponse = m_listResponse.pop() )
	{
		processHttpResponse(pResponse);
		delete pResponse;
	}

	while (HttpRequest * pRequest = m_listRequest.pop() )
	{
		HttpResponse tmp(pRequest);
		processHttpResponse(&tmp);
	}
}
//-------------------------------------------------------------
//------------------------------ 启动工作线程
bool	HttpClient::setup_workers	()
{
	//创建退出事件
	if(!m_clExitEvent.create_event(true,false))
		return false;

	m_clExitEvent.reset_event();

	uint32	uThreadCount= 1;
#ifdef WIN32
	SYSTEM_INFO	sysInfo;
	::GetSystemInfo( &sysInfo );
	uThreadCount = (uint32)sysInfo.dwNumberOfProcessors;
#else//WIN32
	uThreadCount = (uint32)sysconf(_SC_NPROCESSORS_CONF);
#endif//WIN32

	HttpProcessor*_thread	= NULL;
	for (uint32 i = 0;i < uThreadCount;i++)
	{
		_thread = newProcessor();
		if(!_thread)
			return false;

		//创建线程
		if(!_thread->create(HttpClient::_working_thread_,_thread))
		{
			deleteProcessor(_thread);
			return false;
		}
		m_vecWorkThread.push_back(_thread);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	HttpClient::shutdown_workers()
{
	m_clExitEvent.set_event();	//设置线程退出事件

	bool		bRunning= true;
	HttpProcessor*	_thread	= NULL;
	while(bRunning)
	{
		bRunning = false;
		for (uint32 i = 0;i < m_vecWorkThread.size();i++)
		{
			_thread = m_vecWorkThread[i];
			if(!_thread)
				continue;

			/*未停止运行*/ 
			if(_thread->is_runing())
			{
				bRunning = true;
				dSleep(10);
				continue;
			}

			//等待线程退出
			_thread->shutdown();

			dSleep(10);

			//释放内存
			deleteProcessor(_thread);
			m_vecWorkThread[i] = NULL;
		}
	}

	m_vecWorkThread.clear();

	m_clExitEvent.close_event();
}
//-------------------------------------------------------------
//------------------------------ 发送请求
bool	HttpClient::postHttpRequest			(HttpRequest* pRequest)
{
	if ( !pRequest || m_bShutdown )
		return false;

	m_listRequest.pusk(pRequest);
	return true;
}
//-------------------------------------------------------------
//------------------------------
void	HttpClient::postHttpResponse		(HttpResponse* pResponse)
{
	if ( !pResponse )
		return ;

	m_listResponse.pusk(pResponse);
}
//-------------------------------------------------------------
//------------------------------ 逻辑推进
void	HttpClient::process					()
{
	while (HttpResponse * pResponse = m_listResponse.pop() )
	{
		processHttpResponse(pResponse);
		delete pResponse;
	}
}
//-------------------------------------------------------------
//------------------------------ 
HttpProcessor*	HttpClient::newProcessor		()
{
	return new HttpProcessor(*this);
}
//-------------------------------------------------------------
//------------------------------ 
void	HttpClient::deleteProcessor		(HttpProcessor* pProcessor)
{
	if ( !pProcessor )
		return ;

	delete pProcessor;
}
//-------------------------------------------------------------
//------------------------------ 
THREAD_RETURN	HttpClient::_working_thread_	(void* pParam)
{
	INIT_STACK_CHECKER();
	HttpProcessor *_thread = (HttpProcessor*)pParam;
	if(!_thread)
		return 0;

	_thread->process();
	return 0;
}
