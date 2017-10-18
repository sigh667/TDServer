/*----------------- factionStruct.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : �־�
* Version      : V1.0
* Date         : 2016/10/12 19:32:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameDefine.h"
#include "def_struct/stlDefine.h"
#include "def_struct/playerStuct.h"
#include "protocol/GameStruct.pb.h"
#include "memoryPool/mapPool.h"
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class PBFactionBase;
struct _stFactionBase
{
	uint32		uFactionid;			// ����ID
	uint32		uLeader;			// ����ID
	uint32		uWorldID;			// ����ID
	uint32		uLevel;				// ���ɵȼ�
	uint32		uUnion;				// ����ID
	std::string	sName;				// ������
	std::string	sBanner;			// ���
	_emCorps	emCorps;			// ����

	explicit _stFactionBase()	{	init();	}
	void	init()		
	{	
		uFactionid		= 0;
		uLeader			= 0;
		uWorldID		= 0;
		uLevel			= 0;
		uUnion			= 0;
		sName.clear();
		sBanner.clear();
		emCorps			= Corps_Unknown;
	}

	void	loadFromProtobuf(const PBFactionBase&pbInfo);
	void	saveToProtobuf	(PBFactionBase&pbInfo)const;

};

//-------------------------------------------------------------
//------------------------------	 // �����г�Ա��Ϣ
class PBFactionMember;
class PBFactionMemberBase;
struct _stFactionMember
{
	uint32				uPlayerID;			// ��ɫID
	uint32				uContribution;		// ����
	uint32				uExperience;		// ����
	uint32				uJointime;			// ����ʱ��
	_emFactionJob		emJob;				// ְλ
	stl_set<uint32>		setTitle;			// �ƺ�
	uint32				uLastOfflineTime;	//�ϴ�����ʱ��

	// �����ֶδ�game��global������ͬ������
	_stPlayerDisplay	stDisplay;
	uint32				uFightPower;
	bool				bOnline;

	uint32				uFobitCount;		// �����˴�
	uint32				uFobitTime;			// ��һ�ν��Ե�ʱ��
	// �����ֶ��߼�������
	bool				bNeedUpdate;		// �Ƿ���Ҫ���µ�����������

	explicit _stFactionMember()
	{
		init();
	}

	void init()
	{
		uPlayerID		= 0;
		uContribution	= 0;
		uExperience		= 0;
		uJointime		= 0;
		emJob			= FactionJob_Unknown;
		uLastOfflineTime= 0;

		stDisplay.initDisplay();
		uFightPower		= 0;
		bOnline			= false;

		bNeedUpdate		= false;
	}

	inline void setUpdate(bool bUpdate = true)	{	bNeedUpdate = bUpdate;	}
	inline bool isNeedUpdate()	const			{	return bNeedUpdate;		}

	void	saveToPBFactionMember(PBFactionMember& info) const;
	void	saveToPBFactionMemberBase(PBFactionMemberBase& info) const;
};

//-------------------------------------------------------------
//------------------------------	 // ���������б�����������Ϣ
class PBFactionApplyData;
struct _stFactionApply
{
	uint32				uPlayerID;			// ��ɫID
	uint32				uApplytime;			// ����ʱ��
	_stPlayerDisplay	stDisplay;
	uint32				uFightPower;

	explicit _stFactionApply()
	{
		init();
	}

	void init()
	{
		uPlayerID		= 0;
		uApplytime		= 0;
		stDisplay.initDisplay();
		uFightPower		= 0;
	}

	void	saveToPBFactionApplyData(PBFactionApplyData& info) const;
};
//-------------------------------------------------------------
//------------------------------��Ἴ��
class PBFactionSkill;
struct _stFactionSkill
{
	uint32				uSkillID;			// ����ID
	uint32				uLevel;				// �ȼ�
	uint32				eState;				// ����״̬
	uint32				uDevelopLeftTime;	// �о�ʣ��ʱ��(��) �����������������
	uint32				uDevelopLastTime;	// ���һ�μ����о�ʣ��ʱ���ʱ��� ���ֶ���Ϊ�˹ط����ܼ�¼��Ƶ� ǰ�˲���Ҫ���ĸ��ֶ�

	explicit _stFactionSkill()
	{
		init();
	}

	void init()
	{
		uSkillID			= 0;
		uLevel				= 0;
		eState				= 0;
		uDevelopLeftTime	= 0;
		uDevelopLastTime	= 0;
	}

	void	saveToPBFactionSkill(PBFactionSkill& info) const;
};
//-------------------------------------------------------------
//------------------------------��ѧ�б�
class PBFactionStudy;
struct _stFactionStudy
{
	uint32				uFactionID;			// ���ID
	uint32				uBeginTime;			// ��ʼʱ��

	explicit _stFactionStudy()
	{
		init();
	}

	void init()
	{
		uFactionID		= 0;
		uBeginTime		= 0;
	}

	void	saveToPBFactionStudy(PBFactionStudy& info) const;
};
//-------------------------------------------------------------
//------------------------------��ѧ����
class PBFactionStudyConfig;
struct _stFactionStudyConfig
{
	bool				bOpenStudy;			// �Ƿ�������ѧ
	uint32				uStudyMoneyType;	// ��ѧ��Ҫ�Ļ������� _emFactionMoneyType
	uint32				uStudyMoneyValue;	// ��ѧ��Ҫ�Ļ���ֵ
	uint32				uTime;				// ����ʱ��

	explicit _stFactionStudyConfig()
	{
		init();
	}

	void init()
	{
		bOpenStudy		= false;
		uStudyMoneyType	= 0;
		uStudyMoneyValue= 0;
		uTime			= 0;
	}

	void	loadFromPBFactionStudyConfig(const PBFactionStudyConfig& info);
	void	saveToPBFactionStudyConfig(PBFactionStudyConfig& info) const;
};
//-------------------------------------------------------------
//------------------------------���ɵ�����Ϣ
class PBFactionImpeach;
struct _stFactionImpeach
{
	uint32				uImpeachID;			// ���������ID
	uint32				uImpeachTime;		// ����ʧЧʱ�� 0δ����
	uint32				uMaxMemberNum;		// �����Ч���� ����ȷ��
	SET_U32				setAgreeMember;		// ͬ��ĳ�Ա
	SET_U32				setRefuseMember;	// ��ͬ��ĳ�Ա

	explicit _stFactionImpeach()
	{
		init();
	}

	void init()
	{
		uImpeachID = 0;
		uImpeachTime = 0;
		uMaxMemberNum = 0;
		setAgreeMember.clear();
		setRefuseMember.clear();
	}

	void	saveToPBFactionImpeach(PBFactionImpeach& info) const;
};

//-------------------------------------------------------------
//------------------------------

struct _stTopListInfo		//���а� ��ֵ
{
	uint32		uType;		//�������� _emTopListType
	uint64		uValue;		//����ֵ
	uint64		uSubValue;	//������ֵ
	uint32		uTime;		//�޸�ʱ��

	_stTopListInfo&getInfo()			{	return *this;					}
	inline void	initialize()			{	memset(this,0,sizeof(*this));	}

	inline bool	isValid			()const	{	return uValue||uSubValue;		}

	void		loadFromProtobuf(const PBTopListInfo &pbInfo);
	void		saveToProtobuf	 (PBTopListInfo &pbInfo)const;
};

//-------------------------------------------------------------
//------------------------------ 
struct _stTopListSortType	//���а� ����
{
	uint32		uType;		//�������� _emTopListType
	uint32		uJob;		//ְҵ
	//uint32	uState;		//����

	_stTopListSortType&getSortType	()		{	return *this;					}
	inline void	initSortType		()		{	memset(this,0,sizeof(*this));	}

	inline bool	operator <		(const _stTopListSortType& rhs)const
	{
		if ( uType != rhs.uType )
			return uType < rhs.uType;
		else 
		{
			return uJob < rhs.uJob ;
		}
		return false;
	}
	inline bool	operator ==		(const _stTopListSortType& rhs)const
	{
		if ( uType != rhs.uType )
			return false;

		if (uJob != rhs.uJob)
			return false;

		return true;
	}

	void		loadFromProtobuf(const PBTopListSortType &pbInfo);
	void		saveToProtobuf	 (PBTopListSortType &pbInfo)const;
};
//-------------------------------------------------------------
//------------------------------ 
struct _stTopListOrder	: _stTopListSortType	//���а� ����
{
	uint32		uOrder;		//����

	_stTopListOrder&getInfo()			{	return *this;					}
	inline void	initInfo	()			{	memset(this,0,sizeof(*this));	}

	inline bool	isValid			()const	{	return 0!=uOrder;				}

	void		loadFromProtobuf(const PBTopListOrder &pbInfo);
	void		saveToProtobuf	 ( PBTopListOrder &pbInfo)const;
};

typedef stl_map<uint8,_stTopListInfo>		MAP_TOPLIST_INFO;	// <���ͣ�������Ϣ>
typedef stl_set<_stTopListOrder>			SET_TOPLIST_ORDER;	// ����ֵ

//-------------------------------------------------------------
//------------------------------	 // ������Ϣ
class PBFactionDBData;
class PBFactionBase;
struct _stFaction : public _stFactionBase
{
	typedef	stl_map<uint32, _stFactionMember>	MAP_PLAYERS;	// player id -> member
	typedef stl_map<uint32, _stFactionApply>	MAP_APPLYS;		// player id -> _stFactionApply
	typedef stl_map<uint32, _stFactionSkill>	MAP_SKILLSS;	// skill id -> _stFactionSkill
	typedef stl_map<uint32, _stFactionStudy>	MAP_STUDYS;		// faction id -> _stFactionStudy
	std::string				sDeclaration;		// ����
	uint32					uCreateTime;		// ����ʱ��
	uint64					uStone;				// ��ʯ
	uint64					uSilver;			// �ʽ�
	uint64					uEnergy;			// �ж���
	uint64					uScore;				// ����ֵ
	uint64					uBoom;				// ���ٶ�
	uint64					uExperience;		// ����
	uint32					uOrder;				// ����
	bool					bAutoApply;			// �Ƿ��Զ��������
	MAP_PLAYERS				mapPlayers;			// ��ų�Ա�б�
	MAP_APPLYS				mapApplys;			// ��������б�
	uint32					uRequireLevel;		// ���ȼ�Ҫ��
	MAP_U32_U32				mapRefuses;			// (�ܾ�/ȡ��)��������¼	player id -> ʱ���
	MAP_SKILLSS				mapSkills;			// �����б�
	MAP_STUDYS				mapStudys;			// ��ѧ�б�
	_stFactionStudyConfig	stCurStudyConifg;	// ��ǰ����ѧѧ����
	_stFactionStudyConfig	stFrontStudyConifg;	// Ԥ�����ѧѧ����
	stl_map<uint32,uint64>	m_mapJobPrivilege;	// ְλȨ�޹���
	_stFactionImpeach		m_stImpeach;		// ������Ϣ

	MAP_TOPLIST_INFO		m_mapTopListInfo;					// ��ֵ
	SET_TOPLIST_ORDER		m_setTopListOrder;					// ����

	explicit _stFaction()	{	init();		}
	void	init()
	{
		_stFactionBase::init();
		sDeclaration.clear();
		uCreateTime = 0;
		uStone = 0;
		uSilver = 0;
		uEnergy = 0;
		uScore = 0;
		uBoom = 0;
		uExperience = 0;
		uOrder = 0;
		mapPlayers.clear();
		mapApplys.clear();
		uRequireLevel = 0;
		mapRefuses.clear();
		mapSkills.clear();
		mapStudys.clear();
		stCurStudyConifg.init();
		stFrontStudyConifg.init();
		m_mapJobPrivilege.clear();
		m_stImpeach.init();
		bAutoApply = false;
	}

	void		loadFromProtobuf(const PBFactionDBData &pbInfo);
	void		saveToProtobuf(PBFactionDBData &pbInfo)	const;
	void		saveToPBFactionBase(PBFactionBase &pbInfo)	const;
	uint32		size() const;
};

//-----------------------------------------------------------------------------------------------------
// �������������������
enum _emFactionOfflineType
{
	FactionOfflineType_Remove	= 1,	// ���Ƴ�����	PBFactionOfflineRemove
};
//-------------------------------------------------------------
//------------------------------
class PBCorpsMemberInfo;
struct _stCorpsMember
{
	uint32			uCorpsJob;		//����ְλ _emCorpsJob
	_stPlayerAvatar stAvatar;		//��Ա��Ϣ
	uint32			uFobitCount;	//��������
	uint32			uNextSettime;	//�´θ�����Աʱ��
	explicit _stCorpsMember()	{	init();		}
	void	init()
	{
		uCorpsJob = 0;
		stAvatar.init();
		uFobitCount = 0;
		uNextSettime = 0;
	}
	void		loadFromProtobuf(const PBCorpsMemberInfo &pbInfo);
	void		saveToProtobuf(PBCorpsMemberInfo* pbInfo)	const;
};	
//-------------------------------------------------------------
//------------------------------����
class PBCorpsInfo;
struct _stCorps
{
	typedef stl_map<uint32,_stCorpsMember> MAP_CORPSMEMBER; //key;ְλ
	uint32			uCorpID;			//����ID	_emCorps
	std::string		strDeclaration;		//����
	MAP_CORPSMEMBER	mapMember;			//ְλ��Ϣ

	explicit _stCorps()	{	init();		}
	void	init()
	{
		uCorpID = 0;
		strDeclaration.clear();
		mapMember.clear();
	}

	void		loadFromProtobuf(const PBCorpsInfo &pbInfo);
	void		saveToProtobuf(PBCorpsInfo* pbInfo)	const;
};

//-------------------------------------------------------------
//-----------------------------����������������
enum _emCorpsOfflineType
{
	CorpsOfflineType_ChangeJob	= 1,	// �ı�ְλ	PBCorpsOfflineChangeJob
};