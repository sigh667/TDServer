#ifndef __NETWORK_MSG_H__
#define __NETWORK_MSG_H__

#include <iostream>
#include "base/socketPeer.h"
class CGatePeer : public CLEPeer
{
	virtual bool afterRecv()
	{
		//尝试解包，只要满足长度就组包，并投递给相应的场景线程
		return true;
	}

};

class CWorldPeer : public CLEPeer
{
	virtual bool afterRecv()
	{
		//尝试解包，只要满足长度就组包，并投递给相应的场景线程
		return true;
	}

};
class CTestEchoPeer : public CLEPeer
{

public:
	using CLEPeer::CLEPeer;
	virtual bool afterRecv();
};

#endif

