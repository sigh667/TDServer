/*----------------- HttpProcessor.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/8/27 11:26:21
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "threads/thread.h"
#include "threads/condEvent.h"
/*************************************************************/
class HttpRequest;
class HttpResponse;
class HttpClient;
//-------------------------------------------------------------
//------------------------------处理http 请求 线程对像
class HttpProcessor	: public CThread
{
	static const int RESPONSE_BUFFER_SIZE = 256;
	friend		class			HttpClient;
private:
	HttpClient&	m_clClient;		//归属对像
	char		m_responseMessage[RESPONSE_BUFFER_SIZE];
public:
	HttpProcessor(HttpClient& clClient);
	virtual ~HttpProcessor();
public:
	inline	HttpClient&			getClient		()							{ return m_clClient;	}
public:
	virtual HttpResponse*		processRequest	(HttpRequest* pRequest);
protected:
	// 线程推进函数 
	virtual void				process			();
};
