/*----------------- copymapModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/5/13 10:43:03
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/stlDefine.h"
#include "timer/dateTime.h"
#include "basic/systemFunction.h"
#include "stream/streamBase.h"
#include "math/point2.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//------------------------------------------------------
//------------------------------ �������
enum _emCopymapMark
{
	CopymapMark_LeaderOpen			= 0,	//�Ƿ�ӳ�����
	CopymapMark_UseTeamLevel		= 1,	//�Ƿ�ʹ�ö���ƽ���ȼ�(����ˢ����)
};
//------------------------------------------------------
//------------------------------ �˳�����
enum _emCopymapLeaveType
{
	CopymapLeaveType_OldPositon		= 1,	//������������ǰ����
	CopymapLeaveType_LeaveScene		= 2,	//���͵�ָ������
};
//-------------------------------------------------------------
//------------------------------ 
struct _stUnitGroupBase
{
	uint32		uGroupID;			//��ID
	uint8		uUnitType;			//�������� _em_GameUnitType
	uint32		uUnitID;			//����ID
	uint16		uUnitCount;			//�������
	float32		fRandDir;			//����뾶
	uint32		uRouteID;			//·����

	_stUnitGroupBase&	getBase	()			{	return *this;						}
	inline void		initBase	()			{	memset(this,0,sizeof(*this));		}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stCopymapWaveBase
{
	uint16		uCopymapID;			//����ID������ID��ͬ
	uint16		uWaveID;			//��������ID
	uint32		uWaveStepTime;		//����ʱ��
	uint32		uGroupOrNum;		//����ظ�����

	_stCopymapWaveBase&	getBase	()			{	return *this;						}
	inline void		initBase	()			{	memset(this,0,sizeof(*this));		}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stCopymapBase
{
	uint16		uCopymapID;			//����ID������ID��ͬ
	uint32		uMainType;			//����_emCopymapMainType
	uint32		uType;				//�淨����_emCopymapType
	uint8		uDifficulty;		//�����Ѷ� _emCopymapDifficulty
	uint32		uMark;				//(_emCopymapMarkλ)
	uint32		uReadyTime;			//����׼��ʱ��
	uint32		uSurvivalTime;		//��������ʱ��(��)
	uint32		uExtraTime;			//���ⲹʱ(��)
	uint32		uSpecialExtraTime;	//���ⲹʱ(��) ��Ϭһ��ʱ��
	uint8		uEnterType;			//������������(_emCopymapEnterType)
	uint8		uEnterCopyType;		// ���븱������(_emCopymapEnterCopyType)
	uint32		uEnterCount;		//�����մ���
	uint32		uDayPrizeCount;		//�����ս�������
	uint32		uCopymapTimeType;	// �۳�������ʽ (_emCopymapCopymapTimeType)
	uint32		uOneBossID;			// ��һ��Boss����ID 
	bool		bCantRelive;		//  �ɷ񸴻�
	uint8		uResetType;			//��������(_emCopymapResetType)
	uint32		uPlayerCount;		//��������
	uint32		uAssistPlayerCount;	//��������ս��������
	uint8		uLeaveType;			//�˳����� _emCopymapLeaveType
	uint16		uLeaveSceneID;		//�˳��ĳ���ID
	uint16		uLeaveRegionID;		//�˳��ĳ���ID
	uint32		uTaskID;			//����ID
	uint8		uLeaveTeam2ndConfirm;//�뿪�����Ƿ���Ҫ2��ȷ��,0����Ҫ,1��Ҫ
	uint8		uLeaveTeamAllowed;	//�Ƿ������뿪����,0����,1������
	uint16		uNextCopymapID;		//��������ID
	uint32		uNextCopymapIDRate;	//��������ID����


	_stCopymapBase&	getBase	()							{	return *this;											}
	inline void		initBase	()						{	memset(this,0,sizeof(*this));							}
	inline bool		check_mark		(uint8 _mark)const	{	return _CHECK_BIT(uMark,_BIT32(_mark));					}
	inline bool		check_enter		(uint8 _enter)const	{	return _CHECK_BIT(uEnterType,_BIT32(_enter));			}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stUnitGroupModel	: public _stUnitGroupBase
{
	stl_map<uint32,_stPoint2F> mapBirthPostion;		//key��������

	_stUnitGroupModel&getModel	(){	return *this;					}
	inline void		initModel()
	{
		initBase();
		mapBirthPostion.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stUnitGroupBase::read(stream)&&
			stream.read(mapBirthPostion);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stUnitGroupBase::write(stream)&&
			stream.write(mapBirthPostion);
	}
};
typedef stl_vector<const _stUnitGroupModel*> VEC_WAVE_UNITGROUP;
//-------------------------------------------------------------
//------------------------------ 
struct _stCopymapWaveModel	: public _stCopymapWaveBase
{
	VEC_U32		vecGroupAnd;				// ����
	VEC_U32		vecGroupOr;					// �����

	_stCopymapWaveModel&getModel	(){	return *this;					}
	inline void		initModel()
	{
		initBase();
		vecGroupAnd.clear();
		vecGroupOr.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stCopymapWaveBase::read(stream)&&
			stream.read(vecGroupAnd)&&
			stream.read(vecGroupOr);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stCopymapWaveBase::write(stream)&&
			stream.write(vecGroupAnd)&&
			stream.write(vecGroupOr);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stCopymapModel	: public _stCopymapBase
{
	stl_vector<_stDateTime>					vecOpenTime;				// ����ʱ��
	DMAP_UINT8_32_32						dmapPreCondition;			// ǰ������
	DMAP_NEED_ITEM							mapNeedItem;				// �۳��ĵ���
	MAP_U16_I32								mapGiveIdentitySkill;		// ��ʱ��ݼ���
	stl_map<uint16,_stCopymapWaveModel*>	mapWaveModel;				// ���ﲨ��
	MAP_U32_U32								mapPrefectCondition;		// ��������
	VEC_U32									vecGiveSkill;				// ��ʱ����
	MAP_U16_U8								mapEnterRegion;				//���븱�������� KEY:����ID VALUE:��Ӫ

	_stCopymapModel&getModel	(){	return *this;					}
	inline void		initModel()
	{
		initBase();
		dmapPreCondition.clear();
		mapNeedItem.clear();
		vecOpenTime.clear();
		mapGiveIdentitySkill.clear();
		mapPrefectCondition.clear();
		mapWaveModel.clear();
		vecGiveSkill.clear();
		mapEnterRegion.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stCopymapBase::read(stream)&&
			stream.read(vecOpenTime)&&
			stream.read(mapNeedItem)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapGiveIdentitySkill)&&
			stream.read(vecGiveSkill)&&
			stream.read(mapEnterRegion)&&
			stream.read(mapPrefectCondition);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stCopymapBase::write(stream)&&
			stream.write(vecOpenTime)&&
			stream.write(mapNeedItem)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapGiveIdentitySkill)&&
			stream.write(vecGiveSkill)&&
			stream.write(mapEnterRegion)&&
			stream.write(mapPrefectCondition);
	}
	inline bool		isInOpenTime	(uint64 uTime) const 
	{
		if ( vecOpenTime.empty() )
			return true;

		uint32	uSize 	= vecOpenTime.size();
		for ( uint32 i = 0; i < uSize; ++i )
		{
			const  _stDateTime& stDataTime = vecOpenTime[i];
			if (stDataTime.toDataTime(uTime))
				return true;
		}
		return false;
	}
	inline const _stCopymapWaveModel*	findWave	(uint16 uWaveID) const
	{
		stl_map<uint16,_stCopymapWaveModel*>::const_iterator _pos = mapWaveModel.find(uWaveID);
		return _pos != mapWaveModel.end() ? _pos->second : NULL;
	}
	inline uint16 getBirthRegion	(uint8 uCampID) const
	{
		CONST_ITER_MAP_U16_U8 _pos = mapEnterRegion.begin();
		for (; _pos != mapEnterRegion.end(); ++_pos)
		{
			if (_pos->second == uCampID)
				return _pos->first;
		}
		return 0;
	}
};
