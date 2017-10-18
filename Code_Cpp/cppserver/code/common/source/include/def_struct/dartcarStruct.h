/*----------------- teamStruct.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
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
	E_DART_STATUS_INIT,						//��û�п�ʼ��ʼ��.  
	E_DART_STATUS_ADD_SUPPLY_BEGINE,		//�����������,���Ӳ����򿪽���.
	E_DART_STATUS_ADD_SUPPLY,				//�����������,�������Ӳ���.
	E_DART_STATUS_STARTED,					//��ʼ�����,���ڿ�ʼ.
	E_DART_STATUS_UNDERGOING,				//���ڽ�����.
	E_DART_STATUS_PAUSE,					//�ڼ��վ��ͣ.�ȴ���
	E_DART_STATUS_PAUSE_CHECK_CONFIRM,		//�ڼ��վ,�յ�ȷ��
	E_DART_STATUS_ROBBING_START,			//��ʼ����.
	E_DART_STATUS_ROBBING_PREPARE,			//���ڵ���ʱ.
	E_DART_STATUS_ROBBING,					//�ڽ��ڽ���.
	E_DART_STATUS_ROBBING_FINISHED_SUCCESS,	//�ӽ��ڽ��淵��.�ɹ�
	E_DART_STATUS_ROBBING_FINISHED_FAILED,	//�ӽ��ڽ��淵��.ʧ��
	E_DART_STATUS_FINISHED_SUCCESS,			//Ѻ�ڳɹ�����.������..
	E_DART_STATUS_FINISHED_FAILED,			//Ѻ��ʧ�ܽ���,����ο��..
	E_DART_STATUS_FLY_WAIT,					//�ȴ�worldserver ����..
	E_DART_STATUS_FLY_ACK,					//worldserver ���ȷ���..
};
//-------------------------------------------------------------
//------------------------------ buff��Ϣ
struct _stDartBuffInfo
{
	uint32								m_uDartBuffID;									//�ڳ�buffid
	uint64								m_uRemoveTime;									//����ʱ��
};
//-------------------------------------------------------------
//------------------------------ ��Ա��Ϣ
struct _stDartMemberInfo
{
	uint32								m_uPlayerID;									//���playerid
	uint32								m_uAddSupply;									//��������,������ʹ��
	PBPlayerAvatar						m_pbAvatar;										//��������,��һ�������ʱ���Ҫ��
	uint32								m_uIdentityPersonalPrize;						//����淨�ĸ��˽���
	void   zero()
	{
		m_uPlayerID = 0;
		m_uAddSupply = 0;
		m_pbAvatar.Clear();
		m_uIdentityPersonalPrize = 0;
	}

};
//-------------------------------------------------------------
//------------------------------ Ѻ��ʵʱ��Ϣ
struct _stDartInfoRT
{
	uint64								m_uGlobalDartID;								//Ψһid buildItemSN(uint32 uPlayerID, uint32& uSequence)  ������Ʒ����ˮ������
	uint32								m_uCurrentDartID;								//��ǰ��Ѻ��id
	uint32								m_uDartLeaderPlayerID;							//Ѻ�ڼ�ʻԱ
	uint64								m_uLastCheckTime;								//��ʼʱ���,ÿ�ο�ʼ����������,
	uint32								m_uLeftSecond;									//ʣ������
	uint32								m_uTotalSecond;									//����Ѻ�����������
	uint32								m_uStatus;										//_emDartStatus  .
	uint32								m_uPrizeTimes;									//��������1�� 2�� 3��
	uint32								m_uRobTriggerTime;								//�������ڵ�ʱ��,�������Ժ���0,��ֹ�ٴδ���
	stl_vector<_stDartMemberInfo>		m_vcDartMembersPlayerID;						//��ǰѺ�ڵĳ�Ա
	uint32								m_uPathIndex;									//�ڼ���·��
	stl_vector<_stDartBuffInfo>			m_vcGetDartBuffID;								//�Ѿ���õ�buff
	uint32								m_uEnterSceneTime;								//���볡���Ĵ���,�����ÿ��update��ʱ��,worldserverӦ���յ�1 2 3
	uint32								m_uSceneID;										//��ǰ����ID
	uint32								m_uSceneCellID;									//��ǰ��ID
	uint64								m_uSceneDynID;									//��ǰ����ΨһID
	float32								m_fCurrentX;									//��ǰ����,������ڸ�����Ҫ��
	float32								m_fCurrentY;									//��ǰ����,������ڸ�����Ҫ��
	float32								m_fDir;											//��ǰ����,������ڸ�����Ҫ��
	_stDartInfoRT& operator = (const _stDartInfoRT & copy)
	{
		if (this != &copy)
		{
			m_uGlobalDartID			= copy.m_uGlobalDartID;									//Ψһid buildItemSN(uint32 uPlayerID, uint32& uSequence)  ������Ʒ����ˮ������
			m_uCurrentDartID		= copy.m_uCurrentDartID;								//��ǰ��Ѻ��id
			m_uDartLeaderPlayerID	= copy.m_uDartLeaderPlayerID;							//Ѻ�ڼ�ʻԱ
			m_uLastCheckTime		= copy.m_uLastCheckTime;								//��ʼʱ���,ÿ�ο�ʼ����������,
			m_uLeftSecond			= copy.m_uLeftSecond;									//ʣ������
			m_uTotalSecond			= copy.m_uTotalSecond;									//����Ѻ�����������
			m_uStatus				= copy.m_uStatus;										//_emDartStatus  .
			m_uPrizeTimes			= copy.m_uPrizeTimes;									//��������1�� 2�� 3��
			m_uRobTriggerTime		= copy.m_uRobTriggerTime;								//�������ڵ�ʱ��,�������Ժ���0,��ֹ�ٴδ���
			m_vcDartMembersPlayerID = copy.m_vcDartMembersPlayerID;							//��ǰѺ�ڵĳ�Ա
			m_uPathIndex			= copy.m_uPathIndex;									//·������Ϣ,�����±�.
			m_vcGetDartBuffID		= copy.m_vcGetDartBuffID;								//�Ѿ���õ�buff
			m_uEnterSceneTime		= copy.m_uEnterSceneTime;								//���볡���Ĵ���,�����ÿ��update��ʱ��,worldserverӦ���յ�1 2 3
			m_uSceneID				= copy.m_uSceneID;										//��ǰ����ID
			m_uSceneCellID			= copy.m_uSceneCellID;									//��ǰ��ID
			m_uSceneDynID			= copy.m_uSceneDynID;									//��ǰ����ΨһID
			m_fCurrentX				= copy.m_fCurrentX;										//��ǰ����,������ڸ�����Ҫ��
			m_fCurrentY				= copy.m_fCurrentY;										//��ǰ����,������ڸ�����Ҫ��
			m_fDir					= copy.m_fDir;											//��ǰ����,������ڸ�����Ҫ��
		}
		return *this;
	}

	void	initialize()
	{
		m_uGlobalDartID = 0;								//Ψһid buildItemSN(uint32 uPlayerID, uint32& uSequence)  ������Ʒ����ˮ������
		m_uCurrentDartID = 0;								//��ǰ��Ѻ��id
		m_uDartLeaderPlayerID = 0;							//Ѻ�ڼ�ʻԱ
		m_uLastCheckTime = 0;								//��ʼʱ���,ÿ�ο�ʼ����������,
		m_uLeftSecond = 0;									//ʣ������
		m_uTotalSecond = 0;									//����Ѻ�����������
		m_uStatus = E_DART_STATUS_INIT;						//_emDartStatus  .
		m_uPrizeTimes = 0;									//��������1�� 2�� 3��
		m_uRobTriggerTime = 0;								//�������ڵ�ʱ��,�������Ժ���0,��ֹ�ٴδ���
		m_vcGetDartBuffID.clear();							//�Ѿ���õ�buff
		m_uPathIndex = 0;									//·������Ϣ
		m_vcDartMembersPlayerID.clear();					//��ǰѺ�ڵĳ�Ա
		m_uEnterSceneTime = 0;								//���볡���Ĵ���,�����ÿ��update��ʱ��,worldserverӦ���յ�1 2 3
		m_uSceneID	= 0;									//��ǰ����ID
		m_uSceneCellID = 0;									//��ǰ��ID
		m_uSceneDynID = 0;									//��ǰ����ΨһID
		m_fCurrentX = 0;									//��ǰ����,������ڸ�����Ҫ��
		m_fCurrentY = 0;									//��ǰ����,������ڸ�����Ҫ��
		m_fDir = 0;											//��ǰ����,������ڸ�����Ҫ��

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
			//�滻,
			if (pDartBuff->uLayerType == DartBuffLayerType_Replace)
			{
				m_vcGetDartBuffID[uExisted].m_uRemoveTime = pDartBuff->uEffectiveCD + uTime;
			}
			//����
			else if (pDartBuff->uLayerType == DartBuffLayerType_Add)
			{
				m_vcGetDartBuffID[uExisted].m_uRemoveTime += pDartBuff->uEffectiveCD ;
			}
		}
		else
		{
			//���
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
//------------------------------ ������������,��Ҫ���������ļ�,��ʱ������
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