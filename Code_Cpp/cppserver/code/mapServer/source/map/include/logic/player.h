#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "base/timeAxis.h"

//player 需要管理自己的内部逻辑，比如技能cd，
class CPlayer:public CTimerAxis
{
public :
	CPlayer() {};
	~CPlayer() {};

};

#endif