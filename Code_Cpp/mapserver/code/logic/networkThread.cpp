#include "logic/networkThread.h"

#ifdef WIN32
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

void socket_read_cb(bufferevent *bev, void *arg)
{
	if (!arg)
	{
		return;
	}
	CLEPeer *peer = (CLEPeer*)arg;
	if (peer)
	{
		peer->onRecv();
	}
}


void socket_event_cb(bufferevent *bev, short events, void *arg)
{
	if (!arg)
	{
		return;
	}

//#define BEV_EVENT_READING	0x01	/**< error encountered while reading */
//#define BEV_EVENT_WRITING	0x02	/**< error encountered while writing */
//#define BEV_EVENT_EOF		0x10	/**< eof file reached */
//#define BEV_EVENT_ERROR		0x20	/**< unrecoverable error encountered */
//#define BEV_EVENT_TIMEOUT	0x40	/**< user-specified timeout reached */
//#define BEV_EVENT_CONNECTED	0x80	/**< connect operation finished. */


	CLEPeer *peer = (CLEPeer*)arg;
	if (peer)
	{
		if (events == BEV_EVENT_CONNECTED)
		{
			peer->onConnected();
		}
		else
		{
			std::cout<<"close a client "<< peer->socket()<<std::endl;

			peer->onClose();
		}
	}
}

void listener_cb(evconnlistener *listener, evutil_socket_t fd,
	struct sockaddr *sock, int socklen, void *arg)
{
	if (!arg)
	{
		return;
	}
	printf("accept a client %d\n", fd);

	CNetworkThread *nt = (CNetworkThread*)arg;

	//为这个客户端分配一个bufferevent    
	bufferevent *bev = bufferevent_socket_new(nt->getEventBase(), fd,
		BEV_OPT_CLOSE_ON_FREE);
	if (bev)
	{
		CTestEchoPeer * p = new CTestEchoPeer(fd);
		if (p)
		{
			p->setBE(bev);
			nt->addNewPeer(p);
			bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, p);
			bufferevent_enable(bev, EV_READ | EV_PERSIST);
			p->onConnected();
		}
	}
}


bool CNetworkThread::addListener(const char* ip, short port)
{
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(ip);
	evconnlistener* p = evconnlistener_new_bind(getEventBase(), listener_cb, this,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
		10, (struct sockaddr*)&sin,
		sizeof(struct sockaddr_in));
	if (!p)
	{
		return false;
	}
	m_listListener.push_back(p);
	return true;
}


bool CNetworkThread::addConnector(const char* ip, short port)
{
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(ip);

	bufferevent *p_connect_event = bufferevent_socket_new(getEventBase()
		, -1, BEV_OPT_CLOSE_ON_FREE);
	if (p_connect_event)
	{
		int r = bufferevent_socket_connect(p_connect_event
			, (struct sockaddr *)&sin, sizeof(sin));
		if (r == 0)
		{
			CLEPeer * p = new CLEPeer((int)event_get_fd(&(p_connect_event->ev_read)));
			if (p)
			{
				bufferevent_setcb(p_connect_event, socket_read_cb, NULL, socket_event_cb, p);
				bufferevent_enable(p_connect_event, EV_READ | EV_PERSIST);
				addNewPeer(p);
				return true;
			}
		}
	}

	return false;
}

