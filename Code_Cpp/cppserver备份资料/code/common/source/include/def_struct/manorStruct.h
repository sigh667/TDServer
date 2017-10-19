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
#include "cfg_data/manorData/manorData.h"
#include "def_struct/stlDefine.h"
#include "def_struct/playerStuct.h"
#include "def_struct/factionStruct.h"
#include "protocol/GameDefine.pb.h"
#include "protocol/WAGMsg.pb.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/WADBMsg.pb.h"
#include <map>
#include <string>
/*************************************************************/
/*************************************************************/

struct _stManorChallengerDB
{
	uint32		uFactionid;			// ����ID
	uint32		uApplyTime;			// ����ʱ��
	bool		bIsChallenger;		// �Ƿ���ս��,true��ʾ��ս�� ,��ɫ
	uint32		uManorid;			// ���ID����ǲ������ݿ�� �����ڴ��ʱ���ʼ��

	void		init()
	{
		uFactionid = 0;
		uApplyTime = 0;
		bIsChallenger = false;
	}

	void	loadFromProtobuf(const PBFactionChallengerDB&pbInfo);
	void	saveToProtobuf	(PBFactionChallengerDB&pbInfo);
};

struct _stManorInfoDB
{
	uint32										uManorid		;	// ������manor���
	uint32										uStatus			;	// ���״̬ _emManorOccupyStatus
	uint32										uOwnerfactionid	;	// ��ǰռ�а���
	typedef std::map<uint32 , _stManorChallengerDB> MAPID2CHALLENGER;
	MAPID2CHALLENGER							mpChallengers	;	// ��������,�������鶼��������ʾ,��������˫������־
	uint64										uWeekPrizeTime	;	// ÿ�η��Ž�����ʱ���,��Ϊ��һ�ܵ���ʼʱ��
	uint64										uDayPrizeTime	;	// ÿ�η��Ž�����ʱ���,��Ϊ��һ�����ʼʱ��
	void		init()
	{
		uManorid		= 0;
		uStatus			= 0;
		uOwnerfactionid = 0;
		mpChallengers.clear();
		uWeekPrizeTime  = 0;
		uDayPrizeTime	= 0;
	}
	void	loadFromProtobuf					(const PBManorInfoDB&pbInfo);
	void	saveToProtobuf						(PBManorInfoDB&pbInfo);
	_stManorChallengerDB* getChallengerByFactionID(uint32 uFactionID);
	void	addChallenger						(uint32 uFactionID);
	void	removeChallenger					(uint32 uFactionID);
};

struct _stManorChallenger
{
	_stFactionBase		stFactionBase;		// ��ػ�����Ϣ
	uint32				uApplyTime;			// ����ʱ��
	bool				bIsChallenger;		// �Ƿ���ս��,true��ʾ��ս�� ,��ɫ
	void	loadFromProtobuf(const PBFactionChallenger&pbInfo);
	void	saveToProtobuf	(PBFactionChallenger&pbInfo);
	void    init()
	{
		stFactionBase.init();
		uApplyTime = 0;
		bIsChallenger = false;
	}
};

struct _stManorCpymapInfo
{
	uint32				uSceneID;			// ����id
	uint64				uDynID;				// ��̬id
	uint32				uCellID;			// ��id
	uint32				regionid;			// �����ս��,��Ҫ���id

	void		init()
	{
		uSceneID		= 0;			// ����id
		uDynID			= 0;			// ��̬id
		uCellID			= 0;			// ��id
		regionid		= 0;			// �����ս��,��Ҫ���id
	}
	void	loadFromProtobuf(const PBManorCpymapInfo&pbInfo);
	void	saveToProtobuf	(PBManorCpymapInfo&pbInfo);
};

struct _stManorInfo
{
	uint32						manorid		;	// ������manor���
	uint32						status		;	// ���״̬ _emManorOccupyStatus
	_stFactionBase				owner		;	// ��ǰռ�а���
	typedef std::vector<_stManorChallenger> VECTOR_MANORCHALLENGER;
	VECTOR_MANORCHALLENGER		vcChallengers	;	// ��������,�������鶼��������ʾ,��������˫������־
	typedef std::map<uint32, _stManorCpymapInfo> MAP_REGIONID_2_BATTLEFIELD;
	MAP_REGIONID_2_BATTLEFIELD	mpBattlefield;					//ս�����,���ݷ��غ���ս�ֱ�����
	_stManorCpymapInfo			defenderLobby;					//���ط�����
	_stManorCpymapInfo			challengerLobby;				//��ս������
	typedef std::set<uint32>	SET_ELITE_PLAYERS;				
	SET_ELITE_PLAYERS			setElitePlayerIDs;				//��Ӣ��ҵ�id����
	void	init()
	{
		manorid = 0;
		status = 0;
		owner.init();
		vcChallengers.clear();
		mpBattlefield.clear();
		defenderLobby.init();
		challengerLobby.init();
		setElitePlayerIDs.clear();
	}
	void	loadFromProtobuf(const PBManorInfo&pbInfo);
	void	saveToProtobuf	(PBManorInfo&pbInfo);
};

struct _stManorBattleFieldPlayerScore
{
	bool						ischallenger	 ;	//	true��ʾ����ս�� ��ɫ,������ɫ
	uint32						job				 ;	//	ְҵ
	std::string					name			 ;	//	����
	uint32						score			 ;	//	����
	uint32						kill			 ;	//	��ɱ����
	uint32						dead			 ;	//	����ɱ����
	uint32						playerid		 ;	//	playerid
	uint64						takeHurt		 ;	//	�����˺�
	uint32						assistkill		 ;	//	��������
	void	init()
	{
		job				 = 0;
		name			 = "";	//	����
		score			 =	0;	//	����
		kill			 =	0;	//	��ɱ����
		dead			 =	0;	//	����ɱ����
		ischallenger	 =	false;	//	true��ʾ����ս�� ��ɫ,������ɫ
		playerid		 =	0;	//	playerid
		takeHurt		 =	0;	//	�����˺�
		assistkill		 =	0;	//	��������

		//�ڴ���ʹ��
		mpUseful.clear();
		mpHarmful.clear();
		safeZoneStayTime = 0;//���밲ȫ���ȴ�����ʼʱ��
		lostEliteTime = 0;
		continusKill = 0;
	}
	void	loadFromProtobuf(const PBManorBattlefieldPlayerScore&pbInfo);
	void	saveToProtobuf	(PBManorBattlefieldPlayerScore&pbInfo);

	bool operator < (const _stManorBattleFieldPlayerScore &m)const {
		return score < m.score;
	}

	//���������ݴ���,�ڴ���ʹ��
	typedef  std::map<uint32, uint32> MAP_PLAYERID_2_PLAYERID_BUFF_RECORD;
	MAP_PLAYERID_2_PLAYERID_BUFF_RECORD mpUseful;
	MAP_PLAYERID_2_PLAYERID_BUFF_RECORD mpHarmful;
	void	addBuffRecordUseful(uint32 uPlayerID);
	void	addBuffRecordHarmful(uint32 uPlayerID);
	MAP_PLAYERID_2_PLAYERID_BUFF_RECORD& getUsefulBuffRecord(){return mpUseful;}
	MAP_PLAYERID_2_PLAYERID_BUFF_RECORD& getHarmfulBuffRecord(){return mpHarmful;}
	uint32	safeZoneStayTime;//���밲ȫ���ȴ�����ʼʱ��
	uint32	lostEliteTime;//��ʧ��Ӣ����ʼʱ��
	uint32	continusKill;//����ɱ�˼�¼
};

struct _stManorBattleFieldReport
{
	uint32											factionid		;		//���ID
	uint32											count			;		//��ǰս������
	typedef std::map<uint32,_stManorBattleFieldPlayerScore>  MAP_PLAYERID_2_SCORE;
	MAP_PLAYERID_2_SCORE							mpPlayerscores	;		//ÿ���˵ĵ÷� 
	uint32											score			;		//�ð��ĵ÷�

	void	init()
	{
		factionid	= 0;
		count		= 0;
		mpPlayerscores.clear();
		score		= 0;
	}
	void	loadFromProtobuf(const PBManorBattleFieldReport&pbInfo);//ע����������,
	void	saveToProtobuf	(PBManorBattleFieldReport&pbInfo);

};
