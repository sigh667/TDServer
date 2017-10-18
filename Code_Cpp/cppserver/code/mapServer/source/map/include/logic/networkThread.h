#ifndef __NETWORK_THREAD_H__
#define __NETWORK_THREAD_H__



#include <map>
#include <event2/event.h>    
#include <event2/listener.h>    
#include <event2/bufferevent.h>    
#include <event2/bufferevent_struct.h>

#include "base/objectPool.h"
#include "base/threadMgr.h"
#include "logic/player.h"
#include "networkMsg.h"
#include "base/byteArray.h"
#include "base/eventBase.h"


void socket_read_cb(bufferevent *bev, void *arg);
void socket_event_cb(bufferevent *bev, short events, void *arg);
void listener_cb(evconnlistener *listener, evutil_socket_t fd,
	struct sockaddr *sock, int socklen, void *arg);

enum EPeerType
{//链接通以后需要有第一个协议互相通知身份
	E_PEER_TYPE_UNKNOWN,
	E_PEER_TYPE_GATE,
	E_PEER_TYPE_WORLD,
};
class CNetworkThread : public CThreadLE
{
	typedef CThreadLE BASECLASS;
public:
	CNetworkThread(int id):BASECLASS(id){};
	~CNetworkThread() {};

public:
	virtual void addNewPeer(CLEPeer*p)
	{
		if (p)
		{
			m_mpPeers[p->socket()] = p;
		}
	}
	CLEPeer* getFirstPeer()
	{
		if (m_mpPeers.size() > 0)
		{
			return m_mpPeers.begin()->second;
		}
		return nullptr;
	}
	CLEPeer* getPeer(int socket)
	{
		auto iter = m_mpPeers.find(socket);
		if (iter != m_mpPeers.end())
		{
			return iter->second;
		}
		return nullptr;
	}


public:
	void onRun(unsigned long long nMsNow) override
	{
		//std::cout << "current peer " << m_mpPeers.size() << std::endl;
	};

	virtual bool initThread()
	{
		setNeedTimer(true);
		if (BASECLASS::initThread())
		{
			//注册
			return true;
		}
		return false;
	}
	virtual bool addListener(const char* ip, short port);
	virtual bool addConnector(const char* ip, short port);

protected:
	std::list<evconnlistener *> m_listListener; 
	std::map<int, CLEPeer*> m_mpPeers;
};



#endif