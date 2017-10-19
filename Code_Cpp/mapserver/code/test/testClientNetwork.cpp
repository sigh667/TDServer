#include "test/testClientNetwork.h"
#include "test/testPeer.h"
#ifdef WIN32
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif


bool CTestClientNetwork::addConnector(const char* ip, short port)
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
			CTestPeerCompactSender * p = new CTestPeerCompactSender((int)event_get_fd(&(p_connect_event->ev_read)));
			if (p)
			{
				p->setBE(p_connect_event);
				bufferevent_setcb(p_connect_event, socket_read_cb, NULL, socket_event_cb, p);
				bufferevent_enable(p_connect_event, EV_READ | EV_PERSIST);
				addNewPeer(p);
				return true;
			}
		}
	}

	return false;
}

