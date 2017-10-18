/*----------------- playerStuct.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/1/6 14:21:09
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicFunctions.h"
#include <string>
#include "stlDefine.h"
#include "stl/stl_queue.h"
#include "playerDefine.h"
#include "protocol/GameDefine.pb.h"
/*************************************************************/
class PBPlayerBase;
class PBPlayerDisplay;
class PBItem;
class PBPlayerShapeData;
class PBBingfu;
class PBEquipLucky;
class PBCoolingInfo;
class PBItemUseCount;
class PBPlayerAvatar;
class PBPlayerClimbTowerItem;
class PBPlayerClimbTower;
class PBPlayerPvpData;
class PBTreasureValueInfo;
class PBPvpMember;
class PBPlayerAvatar;
class PBPlayerLoginWealData;
class PBSinglePvpMember;
class PBSinglePvpRecord;
class PBPlayerSinglePvp;
typedef stl_map<uint8, int32>		MAP_ENERGY;
//-------------------------------------------------------------
//------------------------------
struct _stPlayerStream
{
	uint32 uSaveOrder;
	uint16 uSize;
	char szStream[_IO_BUFFER_ - 1024];

	_stPlayerStream()						{	init();							}
	void			init()					{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stPlayerDisplay
{
	char					szPlayerName[Player_Name_Max];//��ɫ��
	uint32					uPlayerID;			// ��ɫID
	uint32					uLevel;				// ��ɫ�ȼ�
	uint32					uWorldID;			// ����ID
	uint32					uLogicWorldID;		// �߼�����ID
	uint32					uJob;				// ְҵ
	uint32					uGender;			// �Ա�
	uint32					uCampID;			// ��Ӫ
	uint32					uFactionID;			// ���ID
	char					szFactionName[Faction_Name_Max];// �����
	char					szAlianceName[Faction_Name_Max];// ������
	_emCorps				emCorps;			// ����ö��
	uint32					uIdentity;			// �����ID
	uint32					uTitleID;			// �ƺ�ID
	char					szFalctionBanner[Faction_Name_Max];	// ������

	_stPlayerDisplay		&getDisplay()		{	return *this;					}
	const _stPlayerDisplay	&getDisplay() const	{	return *this;					}
	inline void	initDisplay	()					{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf		(const PBPlayerDisplay&pbDisplay);
	bool	saveToProtobuf			(PBPlayerDisplay&pbDisplay) const;
};
//-------------------------------------------------------------
//------------------------------ ��ɫ��ʾ��Ϣ
struct _stPlayerShape
{
	uint32	uShape[_emPlayerShapeType_ARRAYSIZE];

	_stPlayerShape()				{	initPlayerShape();				}
	_stPlayerShape&getPlayerShape()	{	return *this;					}
	inline void	initPlayerShape()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ ��ɫ����ʾ��Ϣ
struct _stPlayerAvatar
{
	_stPlayerDisplay	stDisplay;			//����
	_stPlayerShape		stShape;			//���
	SET_U32				setFashion;			//ʱװ

	_stPlayerAvatar()					{	init();							}
	_stPlayerAvatar&getPlayerAvatar()	{	return *this;					}
	inline void	init()	
	{
		stDisplay.initDisplay();
		stShape.initPlayerShape();
		setFashion.clear();
	}

	bool	loadFromProtobuf	(const PBPlayerAvatar&pbInfo);
	bool	saveToProtobuf		(PBPlayerAvatar&pbInfo) const;
};

//-------------------------------------------------------------
//------------------------------ ��ɫ�������
struct _stPlayerBase : _stPlayerDisplay
{
	uint32					uAccountID;			// �˺�ID
	uint32					uPlayerMark;		// ��ɫ��ʾ(λ_em_PlayerMark)
	uint32					uSceneID;			// ���ڳ���ID
	uint32					uSceneCellID;		// ���ڳ�����ID
	uint64					uSceneDynID;		// ���ڳ�����̬ID
	uint8					uSpawnID;			// ������
	float32					fPosX;				// λ��x
	float32					fPosY;				// λ��y
	float32					fDir;				// ����
	uint32					uLastLoginTime;		// ���һ�ε�¼ʱ��
	uint32					uLastLogoutTime;	// ���һ�εǳ�ʱ��
	uint32					uCreateTime;		// ������ɫʱ��
	uint8					uPKType;			// PKģʽ
	uint32					uPKValue;			// PKֵ
	uint32					uKillTime;			// ���� ��ʼʱ��
	uint64					uExp;				// ��ǰ����
	uint64					uWisdom;			// ���
	int32					nCurHP;				// ��ǰ����
	uint32					uItemSequence;		// ���߹������
	_stPlayerShape			stPlayerShape;		// ��ɫ����
	uint32					uFightPower;		// ս����
	uint32					uMaxFightPower;		// ��ʷ���ս����
	uint32					uEquipLucky;		// װ������ֵ
	uint32					uMountID;			// ���ID 0�����
	uint32					uOldSceneID;		// �ϵ� ���ڳ���ID
	uint32					uOldSceneCellID;	// �ϵ� ���ڳ�����ID
	uint64					uOldSceneDynID;		// �ϵ� ���ڳ�����̬ID
	float32					fOldPosX;			// �ϵ� λ��x
	float32					fOldPosY;			// �ϵ� λ��y
	float32					fOldDir;			// �ϵ� ����
	uint64					uSufferValue;		// ����ֵ
	uint32					uCorpsJob;			// ����ְλ
	uint32					uDailyCleanTime;	// ������ʱ��
	uint32					uWeeklyCleanTime;	// ������ʱ��
	uint32					uMonthlyCleanTime;	// ������ʱ��


	_stPlayerBase&getPlayerBase	()						{	return *this;					}
	inline void	initPlayerBase	()						{	memset(this,0,sizeof(*this));	}

	inline void	setPlayerMark	(uint8 uMark,bool bSet)	{	_SET_BIT32(this->uPlayerMark,uMark,bSet);				}
	inline bool testPlayerMark	(uint8 uMark)const		{	return _CHECK_BIT(this->uPlayerMark,_BIT32(uMark));		}
};
//-------------------------------------------------------------
//------------------------------ 
// ��ɫ�������
struct _stPlayer : _stPlayerBase
{
	stl_map<uint8,uint64>	mapMoney;			// ��Ǯ
	MAP_ENERGY				mapEnergy;			// ����
	std::string				strOnlyIndex;		// Ψһ���� ����
	std::string				strOnlyDayIndex;	// Ψһ������
	std::string				strOnlyWeekIndex;	// Ψһ������
	std::string				strOnlyMonthIndex;	// Ψһ���� ��
	MAP_U8_U32				mapCopymapDayCount;	// �����ս������
	SET_U32					setFashion;			// ʱװ
	MAP_U32_U32				mapTeamTaskDayCount;// ��������ս������ key team_task�淨ID
	MAP_U32_U32				mapForbitSpeak;		// ����

	_stPlayer&getPlayer			()					{	return *this;					}
	inline void	initPlayer		()
	{	
		initPlayerBase();
		mapMoney.clear();
		mapEnergy.clear();
		strOnlyIndex.clear();
		strOnlyDayIndex.clear();
		strOnlyWeekIndex.clear();
		strOnlyMonthIndex.clear();
		mapCopymapDayCount.clear();
		setFashion.clear();
		mapTeamTaskDayCount.clear();
		mapForbitSpeak.clear();
	}

	bool	loadFromProtobuf	(const PBPlayerBase&pbPlayerBase);
	bool	saveToProtobuf		(PBPlayerBase&pbPlayerBase);

	bool	saveToAvatar		(PBPlayerAvatar&pbAvatar);
};
//------------------------------------------------------
//------------------------------ ����/װ������
struct _stItemBase
{
	uint32	uItemID;				//��ƷID 
	uint16	uItemNum;				//��Ʒ����
	uint64	uItemSN;				//��Ʒ��ˮ�� 
	uint32	uFlag;					//��Ʒ��ʾ(_em_ItemFlag)
	uint32	uLeftTime;				//ʣ��ʱ����
	uint32	uEndtime;				//����ʱ�� Ϊ0 ֻȡleftick
	uint32	uEnhanceExp;			//ǿ������
	uint32	uForgeExp;				//��ĥ����
	uint32	uBingfuExp;				//��������

	_stItemBase	&getBase	()	{	return *this;					}
	void		initBase	()	{	memset(this,0,sizeof(*this));	}
};
//------------------------------------------------------
//------------------------------ ����/װ������
struct _stItem : _stItemBase
{
	stl_vector<uint32> vecSpecialID;	//��Ʒ����

	_stItem					()			{	initialize();		}
	void		initialize	()	
	{	
		initBase();
		vecSpecialID.clear();
	}
	_stItem	&getItem			()						{	return *this;					}
	bool	testFlag			(uint8 flag)const		{	return _CHECK_BIT(this->uFlag,_BIT32(flag));}
	void	setFlag				(uint8 flag,bool bSet)	{	_SET_BIT32(this->uFlag,flag,bSet);			}
	bool	loadFromProtobuf	(const PBItem& pbData);
	bool	saveToProtobuf		(PBItem& pbData,bool bOfflineNoTime = false);

};
//------------------------------------------------------
//------------------------------ ����/װ������
struct _stItemUseCount
{
	uint32	uItemID;
	uint32	uDayUseCount;
	uint32	uWeekUseCount;
	uint32	uMonthUseCount;

	_stItemUseCount				()			{	init();		}
	void	init				()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBItemUseCount& pbData);
	bool	saveToProtobuf		(PBItemUseCount& pbData);
};
//------------------------------------------------------
//------------------------------��ȴ����
struct _stCoolingInfo
{
	uint16			eType;		//���� _emCoolingType
	uint32			uID;		//id
	uint64			uBeginTick;	//��ʼʱ��	����������ʱ��(����)
	uint64			uEndTick;	//����ʱ��	����������ʱ��(����)
	uint32			uBeginTime; //��ʼ��UNIXʱ�� (��)  ����ͻָ���ȴ��Ϣʹ��

	_stCoolingInfo	()						{	init();								}
	void			init				()	{	memset(this,0,sizeof(*this));		}

	bool			loadFromProtobuf	(const PBCoolingInfo& pbData);
	bool			saveToProtobuf		(PBCoolingInfo* pbData);
};
//------------------------------------------------------
//------------------------------���������淨����,������
struct _stClimbTowerItem
{
	uint32 			uItemID 	;	// ����id
	uint32			uCount		;	// ����
	_stClimbTowerItem	()						{	init();								}
	void			init				()		{	memset(this,0,sizeof(*this));		}
	bool			loadFromProtobuf	(const PBPlayerClimbTowerItem& pbData);
	bool			saveToProtobuf		(PBPlayerClimbTowerItem* pbData);

};
//------------------------------------------------------
//------------------------------
struct _stClimbTower
{

	uint32 							uDayCurrentCopymapID 		;		// ÿ������,�����Ѿ���ɵ�����������,��һ������id,��Ҫ���ݸ���idȥ�Ҷ�Ӧ������,ÿ����0,Ĭ��0
	std::vector<_stClimbTowerItem> 	vcDayItemUnsettled			;		// ÿ������,�����Ѿ���õĵ��߽���,��û�����.���ѡ����������,���ѡ����ȡ,�������ս����,��ȡ����
	uint32 							uDayPlayCount				;		// ÿ������,������ս�Ĵ���,���ܳ����޶�
	uint32 							uWeekCurrentClass			;		// ÿ������,���ܵ��ѶȽ׶�,Ĭ��0
	uint32 							uWeekMaxCopyID		 		;		// ÿ������,���ܵ���߽���,Ĭ��0,�����ʼ����Ž���,���������½����

	uint32							uNextJumpCopymapID			;		// ������������תĿ���¼

	uint32							uLuckDrawRightNow			;		// ������һ�������������ŵĽ���
	_stClimbTower					()		{	init();								}
	void			init			()		
	{	
		uDayCurrentCopymapID = 0;
		uDayPlayCount		 = 0;
		uWeekCurrentClass	 = ClimbTowerClass_Easy;//Ĭ���Ǽ��Ѷ�,��ʼ�Ѷ�
		uWeekMaxCopyID		 =  0;
		vcDayItemUnsettled.clear();
		uNextJumpCopymapID   = 0;
		uLuckDrawRightNow	 = 0;
	}
;
	bool			loadFromProtobuf	(const PBPlayerClimbTower& pbData);
	bool			saveToProtobuf		(PBPlayerClimbTower* pbData);

};
//------------------------------------------------------
//------------------------------ ����/װ������
struct _stPvpWar
{
	uint32	uID					;	// �淨ID
	uint32	uEntercount			;	// ���ս������
	uint32	uWinnum				;	// ʤ���ܳ���
	uint32	uFailnum			;	// ʧ���ܳ���
	uint32	uCapturenum			;	// ϴ�����
	uint32	uKillnum			;	// ��ɱ����
	uint32	uAssistnum			;	// ��������
	uint32	uDeadnum			;	// ��������
	uint32	uMegakill 			;	// ���ߴ���
	uint32	uGodlike 			;	// �������
	uint32	uMvpnum 			;	// MVP����
	uint32	uExitReadyCount		;	// �����˳�׼������
	uint32	uNextEnterTime		;	// �´��ܽ���ʱ�� 0��ʾ������

	_stPvpWar					()			{	init();		}
	void	init				()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBPlayerPvpData& pbData);
	bool	saveToProtobuf		(PBPlayerPvpData* pbData);
};
//------------------------------------------------------
//------------------------------ �ر�ͼ��Ϣ
struct _stTreasureValueInfo
{
	uint32	uPrizeID	;	// ����ID treasure_map.xls
	uint32	uDayValue	;	// ������ֵ
	uint32	uDayCount	;	// �մ���
							   
	_stTreasureValueInfo		()			{	init();		}
	void	init				()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBTreasureValueInfo& pbData);
	bool	saveToProtobuf		(PBTreasureValueInfo* pbData);
};
//-------------------------------------------------------------
//------------------------------
struct _stPvpMember
{
	_stPlayerDisplay	stDisplayer;	//�����Ϣ
	uint64	uMaxFightPower;				//��ʷ���ս�� * 1000
	uint64	uTeamID;					//����ID
	bool	bReady;						//�Ƿ�׼��

	_stPvpMember				()			{	init();		}
	void		init			()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBPvpMember& pbData);
	bool	saveToProtobuf		(PBPvpMember* pbData);
};
//-------------------------------------------------------------
//------------------------------
struct _stWealLogin
{
	uint32 	uID 		;	// cs_weal.xls login_weal��ID
	uint32 	uPrizeday	;	// �Ѿ��콱������
	uint32 	uPrizetime	;	// �ϴ��콱ʱ��

	_stWealLogin				()			{	init();		}
	void		init			()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBPlayerLoginWealData& pbData);
	bool	saveToProtobuf		(PBPlayerLoginWealData* pbData);
};
//------------------------------------------------------
//------------------------------ 1v1��¼
struct _stSinglePvpRecord
{
	_stPlayerDisplay	stDisplayer		;	//�����Ϣ
	uint32				uSinglePvpScore	;	// 1v1����
	uint32				uResult			;	// ��� _emCopymapCompleteType

	_stSinglePvpRecord			()			{	init();		}
	void	init				()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBSinglePvpRecord& pbData);
	bool	saveToProtobuf		(PBSinglePvpRecord* pbData);
};
//------------------------------------------------------
//------------------------------ 1v1����
struct _stSinglePvpBase
{
	uint32	uDayEnterCount		;	// ���ս������
	uint32	uWinCount			;	// ʤ������
	uint32	uFailCount			;	// ʧ�ܳ���
	uint32	uEqualCount			;	// ƽ�ֳ���
	uint32	uWeekCount			;	// �ܳ���
	uint32	uSinglePvpScore		;	// 1v1��λ����
	uint32	uSinglePvpMaxScore	;	// 1v1��λ��߻���

	_stSinglePvpBase			()			{	initBase();		}
	void	initBase			()			{	memset(this,0,sizeof(*this));	}
};
//------------------------------------------------------
//------------------------------ 1v1����
struct _stSinglePvp : _stSinglePvpBase
{
	SET_U32							setFirstPrizeID;	//�Ѿ���ȡ���״�ID
	SET_U32							setDayPrize;		//�Ѿ���ȡ��ÿ�ս���
	stl_list<_stSinglePvpRecord>	listPvpRecord;		//��¼

	_stSinglePvp			()			{	init();		}
	void	init				()			
	{	
		initBase();
		setFirstPrizeID.clear();
		setDayPrize.clear();
		listPvpRecord.clear();
	}
	bool	loadFromProtobuf	(const PBPlayerSinglePvp& pbData);
	bool	saveToProtobuf		(PBPlayerSinglePvp* pbData);
};
//-------------------------------------------------------------
//------------------------------ 1v1��Ա��Ϣ
struct _stSinglePvpMember
{
	_stPlayerDisplay	stDisplayer;	//�����Ϣ
	uint32	uSinglepvpscore	;			// 1v1����
	uint32	uMaxfightpower	;			// ��ʷ���ս��
	uint32	uWincount		;			// ʤ������
	uint32	uFailcount		;			// ʧ�ܳ���
	uint32	uEqualcount		;			// ƽ�ֳ���

	_stSinglePvpMember			()			{	init();		}
	void		init			()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBSinglePvpMember& pbData);
	bool	saveToProtobuf		(PBSinglePvpMember* pbData);
};