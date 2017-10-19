#ifndef __NETWORK_MSG_H__
#define __NETWORK_MSG_H__

#include <iostream>
#include "base/socketPeer.h"
class CGatePeer : public CLEPeer
{
	bool afterRecv() override
	{
		//尝试解包，只要满足长度就组包，并投递给相应的场景线程
		return true;
	}
	void onConnected() override
	{
	};

};

class CTestEchoPeer : public CLEPeer
{

public:
	using CLEPeer::CLEPeer;
	virtual bool afterRecv();
	void onConnected() override
	{
	};

};

#endif

