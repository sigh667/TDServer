/*----------------- HttpProcessor.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����Դ
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
//------------------------------����http ���� �̶߳���
class HttpProcessor	: public CThread
{
	static const int RESPONSE_BUFFER_SIZE = 256;
	friend		class			HttpClient;
private:
	HttpClient&	m_clClient;		//��������
	char		m_responseMessage[RESPONSE_BUFFER_SIZE];
public:
	HttpProcessor(HttpClient& clClient);
	virtual ~HttpProcessor();
public:
	inline	HttpClient&			getClient		()							{ return m_clClient;	}
public:
	virtual HttpResponse*		processRequest	(HttpRequest* pRequest);
protected:
	// �߳��ƽ����� 
	virtual void				process			();
};
