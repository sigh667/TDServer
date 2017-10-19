/*----------------- teamStruct.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : tanghaibo
* Version      : V1.0
* Date         : 2016/12/2o 19:32:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameDefine.h"
#include "cfg_data/dartData/dartData.h"
#include "def_struct/stlDefine.h"
#include "def_struct/playerStuct.h"
#include "protocol/GameDefine.pb.h"
#include "protocol/WAGMsg.pb.h"
#include "protocol/CASMsg.pb.h"
/*************************************************************/
/*************************************************************/
enum _emDartStatus
{
	E_DART_STATUS_INIT,						//还没有开始初始化.  
	E_DART_STATUS_ADD_SUPPLY_BEGINE,		//如果是天下镖,增加补给打开界面.
	E_DART_STATUS_ADD_SUPPLY,				//如果是天下镖,可以增加补给.
	E_DART_STATUS_STARTED,					//初始化完成,正在开始.
	E_DART_STATUS_UNDERGOING,				//正在进行中.
	E_DART_STATUS_PAUSE,					//在检查站暂停.等待中
	E_DART_STATUS_PAUSE_CHECK_CONFIRM,		//在检查站,收到确认
	E_DART_STATUS_ROBBING_START,			//开始劫镖.
	E_DART_STATUS_ROBBING_PREPARE,			//劫镖倒计时.
	E_DART_STATUS_ROBBING,					//在劫镖界面.
	E_DART_STATUS_ROBBING_FINISHED_SUCCESS,	//从劫镖界面返回.成功
	E_DART_STATUS_ROBBING_FINISHED_FAILED,	//从劫镖界面返回.失败
	E_DART_STATUS_FINISHED_SUCCESS,			//押镖成功结束.发奖励..
	E_DART_STATUS_FINISHED_FAILED,			//押镖失败结束,发安慰奖..
	E_DART_STATUS_FLY_WAIT,					//等待worldserver 调度..
	E_DART_STATUS_FLY_ACK,					//worldserver 调度返回..
};
//-------------------------------------------------------------
//------------------------------ buff信息
struct _stDartBuffInfo
{
	uint32								m_uDartBuffID;									//镖车buffid
	uint64								m_uRemoveTime;									//过期时间
};
//-------------------------------------------------------------
//------------------------------ 乘员信息
struct _stDartMemberInfo
{
	uint32								m_uPlayerID;									//玩家playerid
	uint32								m_uAddSupply;									//补货数量,天下镖使用
	PBPlayerAvatar						m_pbAvatar;										//外形形象,第一次组包的时候就要填
	uint32								m_uIdentityPersonalPrize;						//身份玩法的个人奖励
	void   zero()
	{
		m_uPlayerID = 0;
		m_uAddSupply = 0;
		m_pbAvatar.Clear();
		m_uIdentityPersonalPrize = 0;
	}

};
//-------------------------------------------------------------
//------------------------------ 押镖实时信息
struct _stDartInfoRT
{
	uint64								m_uGlobalDartID;								//唯一id buildItemSN(uint32 uPlayerID, uint32& uSequence)  借用物品的流水号来做
	uint32								m_uCurrentDartID;								//当前的押镖id
	uint32								m_uDartLeaderPlayerID;							//押镖驾驶员
	uint64								m_uLastCheckTime;								//开始时间戳,每次开始都重新设置,
	uint32								m_uLeftSecond;									//剩余秒数
	uint32								m_uTotalSecond;									//完整押镖允许的秒数
	uint32								m_uStatus;										//_emDartStatus  .
	uint32								m_uPrizeTimes;									//奖励倍数1倍 2倍 3倍
	uint32								m_uRobTriggerTime;								//触发劫镖的时间,触发过以后置0,防止再次触发
	stl_vector<_stDartMemberInfo>		m_vcDartMembersPlayerID;						//当前押镖的乘员
	uint32								m_uPathIndex;									//第几段路径
	stl_vector<_stDartBuffInfo>			m_vcGetDartBuffID;								//已经获得的buff
	uint32								m_uEnterSceneTime;								//进入场景的次数,如果是每次update的时候,worldserver应该收到1 2 3
	uint32								m_uSceneID;										//当前场景ID
	uint32								m_uSceneCellID;									//当前线ID
	uint64								m_uSceneDynID;									//当前场景唯一ID
	float32								m_fCurrentX;									//当前坐标,进入劫镖副本需要用
	float32								m_fCurrentY;									//当前坐标,进入劫镖副本需要用
	float32								m_fDir;											//当前朝向,进入劫镖副本需要用
	_stDartInfoRT& operator = (const _stDartInfoRT & copy)
	{
		if (this != &copy)
		{
			m_uGlobalDartID			= copy.m_uGlobalDartID;									//唯一id buildItemSN(uint32 uPlayerID, uint32& uSequence)  借用物品的流水号来做
			m_uCurrentDartID		= copy.m_uCurrentDartID;								//当前的押镖id
			m_uDartLeaderPlayerID	= copy.m_uDartLeaderPlayerID;							//押镖驾驶员
			m_uLastCheckTime		= copy.m_uLastCheckTime;								//开始时间戳,每次开始都重新设置,
			m_uLeftSecond			= copy.m_uLeftSecond;									//剩余秒数
			m_uTotalSecond			= copy.m_uTotalSecond;									//完整押镖允许的秒数
			m_uStatus				= copy.m_uStatus;										//_emDartStatus  .
			m_uPrizeTimes			= copy.m_uPrizeTimes;									//奖励倍数1倍 2倍 3倍
			m_uRobTriggerTime		= copy.m_uRobTriggerTime;								//触发劫镖的时间,触发过以后置0,防止再次触发
			m_vcDartMembersPlayerID = copy.m_vcDartMembersPlayerID;							//当前押镖的乘员
			m_uPathIndex			= copy.m_uPathIndex;									//路径点信息,数组下标.
			m_vcGetDartBuffID		= copy.m_vcGetDartBuffID;								//已经获得的buff
			m_uEnterSceneTime		= copy.m_uEnterSceneTime;								//进入场景的次数,如果是每次update的时候,worldserver应该收到1 2 3
			m_uSceneID				= copy.m_uSceneID;										//当前场景ID
			m_uSceneCellID			= copy.m_uSceneCellID;									//当前线ID
			m_uSceneDynID			= copy.m_uSceneDynID;									//当前场景唯一ID
			m_fCurrentX				= copy.m_fCurrentX;										//当前坐标,进入劫镖副本需要用
			m_fCurrentY				= copy.m_fCurrentY;										//当前坐标,进入劫镖副本需要用
			m_fDir					= copy.m_fDir;											//当前朝向,进入劫镖副本需要用
		}
		return *this;
	}

	void	initialize()
	{
		m_uGlobalDartID = 0;								//唯一id buildItemSN(uint32 uPlayerID, uint32& uSequence)  借用物品的流水号来做
		m_uCurrentDartID = 0;								//当前的押镖id
		m_uDartLeaderPlayerID = 0;							//押镖驾驶员
		m_uLastCheckTime = 0;								//开始时间戳,每次开始都重新设置,
		m_uLeftSecond = 0;									//剩余秒数
		m_uTotalSecond = 0;									//完整押镖允许的秒数
		m_uStatus = E_DART_STATUS_INIT;						//_emDartStatus  .
		m_uPrizeTimes = 0;									//奖励倍数1倍 2倍 3倍
		m_uRobTriggerTime = 0;								//触发劫镖的时间,触发过以后置0,防止再次触发
		m_vcGetDartBuffID.clear();							//已经获得的buff
		m_uPathIndex = 0;									//路径点信息
		m_vcDartMembersPlayerID.clear();					//当前押镖的乘员
		m_uEnterSceneTime = 0;								//进入场景的次数,如果是每次update的时候,worldserver应该收到1 2 3
		m_uSceneID	= 0;									//当前场景ID
		m_uSceneCellID = 0;									//当前线ID
		m_uSceneDynID = 0;									//当前场景唯一ID
		m_fCurrentX = 0;									//当前坐标,进入劫镖副本需要用
		m_fCurrentY = 0;									//当前坐标,进入劫镖副本需要用
		m_fDir = 0;											//当前朝向,进入劫镖副本需要用

	};
	bool	getNextStartCheckPoint( _stDartCheckPoint & _t, const _stDartModel* pDartModel)
	{
		if (pDartModel!= NULL && pDartModel->vecPath.size()  > m_uPathIndex + 1)
		{
			_t = pDartModel->vecPath[m_uPathIndex + 1];
			return true;
		}
		return false;
	};
	bool	getStartCheckPoint(_stDartCheckPoint & _t, const _stDartModel* pDartModel)
	{
		if (pDartModel!= NULL && pDartModel->vecPath.size()  > m_uPathIndex)
		{
			_t = pDartModel->vecPath[m_uPathIndex ];
			return true;
		}
		return false;
	};
	bool	getEndCheckPoint(_stDartCheckPoint & _t, const _stDartModel* pDartModel)
	{
		return getStartCheckPoint(_t, pDartModel);
	};
	bool	addDartBuff(uint32 uDartBuffID, uint64 uTime)
	{
		const _stDartBuffModel* pDartBuff = g_clDartData.findDartBuffData(uDartBuffID);
		if (pDartBuff == NULL )
		{
			return false;
		}
		int uExisted = checkHasDartBuffID(uDartBuffID);
		if (uExisted >= 0)
		{
			//替换,
			if (pDartBuff->uLayerType == DartBuffLayerType_Replace)
			{
				m_vcGetDartBuffID[uExisted].m_uRemoveTime = pDartBuff->uEffectiveCD + uTime;
			}
			//叠加
			else if (pDartBuff->uLayerType == DartBuffLayerType_Add)
			{
				m_vcGetDartBuffID[uExisted].m_uRemoveTime += pDartBuff->uEffectiveCD ;
			}
		}
		else
		{
			//添加
			_stDartBuffInfo _st = {0};
			_st.m_uDartBuffID = uDartBuffID;
			_st.m_uRemoveTime = pDartBuff->uEffectiveCD + uTime;

			m_vcGetDartBuffID.push_back(_st);
		}
		return true;
	};

	int32 checkHasDartBuffID(uint32 uBuffID)
	{
		for (uint32 i=0; i<m_vcGetDartBuffID.size() ; ++i)
		{
			if ( m_vcGetDartBuffID[i].m_uDartBuffID == uBuffID)
			{
				return i;
			}
		}
		return -1;
	};

	int32 checkHasDartBuff(_emDartBuffType _type)
	{
		for (uint32 i=0; i<m_vcGetDartBuffID.size() ; ++i)
		{
			const _stDartBuffModel* pDartBuff = g_clDartData.findDartBuffData(m_vcGetDartBuffID[i].m_uDartBuffID);
			if (pDartBuff != NULL && pDartBuff->uType == _type)
			{
				return i;
			}
		}
		return -1;
	};
	const _stDartBuffModel*	checkHasDartBuffType(_emDartBuffType _type)
	{
		for (uint32 i=0; i<m_vcGetDartBuffID.size() ; ++i)
		{
			if (m_vcGetDartBuffID[i].m_uDartBuffID != 0)
			{
				const _stDartBuffModel* pDartBuff = g_clDartData.findDartBuffData(m_vcGetDartBuffID[i].m_uDartBuffID);
				if (pDartBuff != NULL && pDartBuff->uType == _type)
				{
					return pDartBuff;
				}
			}
		}
		return NULL;
	};

	bool	processRemoveDartBuff(uint64 uTime)
	{
		bool _r = false;
		stl_vector<_stDartBuffInfo>::iterator iter = m_vcGetDartBuffID.begin();
		for ( ; iter != m_vcGetDartBuffID.end(); )
		{
			if ((*iter).m_uRemoveTime < uTime)
			{
				_r = true;
				iter = m_vcGetDartBuffID.erase(iter);
			}
			else
			{
				iter ++;
			}
		}
		return _r;
	}
	bool	getDartBuffParamByType(_emDartBuffType _type, int32 &_uParamAssemble)
	{
		bool _r = false;
		for (uint32 i=0; i<m_vcGetDartBuffID.size() ; ++i)
		{
			if (m_vcGetDartBuffID[i].m_uDartBuffID != 0)
			{
				const _stDartBuffModel* pDartBuff = g_clDartData.findDartBuffData(m_vcGetDartBuffID[i].m_uDartBuffID);
				if (pDartBuff != NULL && pDartBuff->uType == _type)
				{
					_uParamAssemble += pDartBuff->uPara1;
					_r = true;
				}
			}
		}
		return _r;
	};
	void	proceedCheckPoint()	
	{
		m_uPathIndex ++;
	};

	bool	doDartCDAndCheck				(uint64 uTime)
	{
		if (m_uLastCheckTime == 0)
		{
			m_uLastCheckTime = uTime; 
		}
		uint32 _elapseSeconds = (uint32)(uTime - m_uLastCheckTime);
		m_uLastCheckTime = uTime; 
		if (_elapseSeconds > m_uLeftSecond)
		{
			m_uLeftSecond = 0;
		}
		else
		{
			m_uLeftSecond -= _elapseSeconds; 
		}
		return m_uLeftSecond <= 0;
	}
	bool	buildSupplyInfo				(PBS2CDartAddSupplyNtf & pbDASN, bool bIsClose)
	{
		pbDASN.set_leaderplayerid(m_uDartLeaderPlayerID);
		pbDASN.set_isclose(bIsClose);
		pbDASN.set_isleader(false);
		stl_vector<_stDartMemberInfo>::iterator _pos = m_vcDartMembersPlayerID.begin();
		stl_vector<_stDartMemberInfo>::iterator _end = m_vcDartMembersPlayerID.end();
		for (; _pos != _end; _pos++)
		{
			_stDartMemberInfo & st = *_pos;
			PBDartAddSupplyInfo* pbDASI = pbDASN.add_addinfo();
			if (pbDASI)
			{
				pbDASI->set_count(st.m_uAddSupply);
				pbDASI->set_playerid(st.m_uPlayerID);
				pbDASI->set_name(st.m_pbAvatar.display().playername());
			}
		}
		return true;
	}

	void	loadFromProtobuf(const PBDartCarInfoRT&pbInfo);
	void	saveToProtobuf	(PBDartCarInfoRT&pbInfo)const;


};


//-------------------------------------------------------------
//------------------------------ 创建副本参数,需要独立出来文件,暂时放这里
struct _stCopyMapCreateParam
{
	uint32				m_uNpcID;
	uint32				m_uLevel;
	_stDartInfoRT		m_stDartInfoRT;
	void initialize()
	{
		m_uNpcID = 0;
		m_uLevel = 0;
		m_stDartInfoRT.initialize();
	}
	void		loadFromProtobuf(const PBCreateCellParam&pbInfo);
	void		saveToProtobuf	(PBCreateCellParam&pbInfo)const;
};