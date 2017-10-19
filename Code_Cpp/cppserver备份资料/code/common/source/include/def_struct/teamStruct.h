/*----------------- teamStruct.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2016/8/5 19:32:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameDefine.h"
#include "def_struct/stlDefine.h"
#include "def_struct/playerStuct.h"
#include "protocol/GameDefine.pb.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
class PBTeamTarget;
class PBTeamTask;
class CWorldPlayer;
struct _stTeamTarget
{
	uint32	uTargetid			; //���Ŀ��,�����0��ʾû��Ŀ��,����Ŀ����Ҫ����
	uint32	uDifficulty			; //��Щ���Ŀ���ﺬ���Ѷ���Ϣ
	uint32	uLevelmin			; //��͵ȼ�����,��ʼӦ�ôӻ������,���ڶӳ����Ե���
	uint32	uLevelmax			; //��ߵȼ�����,��ʼӦ�ôӻ������,���ڶӳ����Ե���
	uint32	uActiveid			; //�id,����������ؼ��,�ȼ�,����ʱ���
	bool	bIsAutoAccept		; //�Ƿ��Զ���������
	bool	bIsAutoMatch		; //�Ƿ��Զ�ƥ��
	uint32	uCash				; //�ͽ�����( Ԫ��)
	_stTeamTarget()
	{
		init();
	}
	void init()
	{
		uTargetid = 0			; 
		uDifficulty	= 0			;
		uLevelmin = 0			;
		uLevelmax = 0			;
		uActiveid = 0			;
		uCash	  = 0			;	
		bIsAutoMatch = false	;
		bIsAutoAccept = false	;
	}
	void		loadFromProtobuf	(const PBTeamTarget&pbInfo);
	void		saveToProtobuf		(PBTeamTarget&pbInfo);
	bool		canMatchTarget		(_stPlayerDisplay& stPlayerDisplay);
};

struct _stTeamInfoBase
{
	uint64			uTeamID;				//����ID
	uint32			uLeaderID;				//�ӳ�ID 
	uint32			uAllotType;				//�������� _emTeamAllotType

	_stTeamInfoBase()		{	initBase();							}
	void	initBase()		
	{	
		uTeamID			= 0;
		uLeaderID		= 0;
		uAllotType		= AllotType_Dice;
	}
};
//-------------------------------------------------------------
//------------------------------ 
class PBTeamMember;
class PBTeamMemberSimple;
enum _emTeamVote
{
	TeamVote_None,	
	TeamVote_Agree,		// ͬ��
	TeamVote_Refuse		// �ܾ�
};
//-------------------------------------------------------------
//------------------------------
struct _stTeamMember				// �����Ա
{
	_stPlayerAvatar		stAvatar;				//���2017-06-22 �ͻ���Ҫ������ɫ
	uint32				uFightPower;
	bool				bOnline;
	uint32				uTime;
	uint32				uSceneID;
	uint32				uCellID;
	_emTeamVote			eKickVote;
	_emTeamVote			eSetAllotVote;
	uint32				uMark;					//λ _emTeamMark
	uint32				uCallupTime;			//�ټ�ʱ��
	uint32				uTeamConfirmState;		//ȷ��״̬	_emTeamConfirmState

	_stTeamMember()		{	init();							}
	void	init()		{
		stAvatar.init();
		uFightPower			= 0;
		bOnline				= false;
		eKickVote			= TeamVote_None;
		eSetAllotVote		= TeamVote_None;
		uTime				= 0;
		uSceneID			= 0;
		uCellID				= 0;
		uMark				= 0;
		uCallupTime			= 0;
	}

	void		loadFromProtobuf	(const PBTeamMember&pbInfo);
	void		saveToProtobuf		(PBTeamMember&pbInfo);
	void		saveToProtobuf		(PBTeamMemberSimple&pbInfo);
	inline bool		check_mark		(uint8 _mark)	const			{	return _CHECK_BIT(uMark,_BIT32(_mark));	}
	inline void		set_mark		(uint8 _mark,bool bSet)			{	_SET_BIT32(uMark,_mark,bSet);			}

};
//-------------------------------------------------------------
//------------------------------���ø�������
struct _stCopymapArrestData
{
	uint32		uCopymapID;				//����ID
	uint8		uCopymapCompleteType;	//����������� _emCopymapCompleteType
	uint32		uExtraParam;			//������� ��������
	_stCopymapArrestData()			{	memset(this,0,sizeof(*this));							}
};
//-------------------------------------------------------------
//------------------------------����հ�λ��ְҵ����
class PBTeamMemberJobRequire;
struct _stTeamMemberJobRequire
{
	typedef std::set<uint32> SETJOBS;
	uint32					uPostion;		//λ����Ϣ[1,2,3,4,5]
	SETJOBS					setJobs;		//ְҵ��Ϣ
	_stTeamMemberJobRequire()
	{
		uPostion = 0;
	}
	void		loadFromProtobuf		(const PBTeamMemberJobRequire&pbInfo);
	void		saveToProtobuf			(PBTeamMemberJobRequire&pbInfo);
	uint32		findMatchCount			(uint32 uJob);
};
//-------------------------------------------------------------
//------------------------------��������Ϣ
class PBTeamApplicant;
struct _stApplicant
{
	uint32 uPlayerid				;//player id
	std::string strName				;//����
	uint32 uLevel					;//�ȼ�
	uint32 uJob						;//ְҵ
	uint32 uFightpower				;//ս����
	uint32 uRelationship			;//����ϵ
	uint64 uLastApplyTick			;//��һ�������ʱ���
	std::string strFactionName		;//�������
	uint32 uFactionID				;//���id
	_stApplicant()	{init();	}
	void	init ()
	{
		strName = "";
		uPlayerid		= 0;		//player id
		uLevel			= 0;		//�ȼ�
		uJob			= 0;		//ְҵ
		uFightpower		= 0;		//ս����
		uRelationship	= 0;		//����ϵ
		uLastApplyTick  = 0;		//��һ�������ʱ���
		strFactionName	= "";		//�������
		uFactionID		= 0;		//���id
	}
	void		loadFromProtobuf		(const PBTeamApplicant&pbInfo);
	void		saveToProtobuf			(PBTeamApplicant&pbInfo);

};
//-------------------------------------------------------------
//------------------------------
struct _stTeamTask
{
	uint32	uID;		//�淨ID
	uint32	uNpcID;		//��ǰ������ID
	uint32	uCopymapID;	//����ID
	uint8	uState;		//����ID	_emTeamTaskState

	_stTeamTask	()	{	init();								}
	void init	()	{	memset(this,0,sizeof(*this));		}
	void		loadFromProtobuf	(const PBTeamTask&pbInfo);
	void		saveToProtobuf		(PBTeamTask&pbInfo);
};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------	 // ������Ϣ
class PBTeamInfo;
class PBTeamInfoSimple;
class PBGetApplicantListAck;
class PBSetAutoMatchJobLimitAck;
struct _stTeamInfo : public _stTeamInfoBase
{
	stl_map<uint32,_stTeamMember>							mapMember;			//��Ա�б�
	_stTeamTarget											m_stTeamTarget;		//���Ŀ��
	_stTeamTask												m_stTeamTask;		//�������
	typedef std::vector<_stTeamMemberJobRequire>			VECJOBREQUIRE;
	VECJOBREQUIRE											m_vcJobRequire;		//����λ��ְҵ������Ϣ
	typedef 	stl_map<uint32, _stApplicant> MAP_PLAYERID_APPLICANT;
	MAP_PLAYERID_APPLICANT									m_mpApplicants;		//�����˼�����Ϣ
	_stTeamInfo()		{	init();							}
	void	init()
	{
		initBase();
		mapMember.clear();
		m_stTeamTarget.init();
		m_vcJobRequire.clear();
		m_mpApplicants.clear();
		m_stTeamTask.init();
	}

	void		loadFromProtobuf(const PBTeamInfo&pbInfo);
	void		saveToProtobuf	 (PBTeamInfo&pbInfo);
	void		saveToProtobuf	 (PBTeamInfoSimple&pbInfo);
	
	bool set_jobRequire(_stTeamMemberJobRequire& req);
	bool build_applicantList(PBGetApplicantListAck& ack);
	bool get_allApplicant(std::vector<uint32> & vcAllPlayerId);
	bool add_applicant(_stApplicant& applicant);
	bool clear_applicantList();
	bool check_applicant(uint32 uPlayerId);
	bool remove_applicant(uint32 uPlayerId);
	bool canMatchTarget(_stPlayerDisplay& stPlayerDisplay);
	bool adjustJobRequire(uint32 uNewMemberJob);
	bool build_jobRequires(PBSetAutoMatchJobLimitAck& ack);
};
