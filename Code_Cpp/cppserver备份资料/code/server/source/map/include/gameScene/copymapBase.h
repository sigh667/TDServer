/*----------------- copymapBase.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/5/11 18:18:00
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_set.h"
#include "stl/std_map.h"
#include "cfg_data/copymapData/copymapData.h"
#include "google/protobuf/message.h"
#include "def_struct/playerStuct.h"
#include "protocol/WAGMsg.pb.h"
#include "gameLogic/sceneObject/sceneItem.h"
/*************************************************************/
class CScenePlayer;
class CSceneUnit;
class CSceneTrap;
class CSceneMonster;
class CGameSceneCell;
class PBW2GCopymapPlayerInfo;
class PBPvpBannerPlayerInfo;
class CCopymapBase
{
public:
	struct _stCopymapPlayer
	{
		_stPlayerDisplay		stDisplay;				// �����Ϣ����Ҵ���ȡ������ݣ�
		_emCopymapEnterReason	emEnterReason;			// ���븱��ԭ��
		_em_CopymapPlayerState	emCopymapPlayerState;	//״̬
		bool					bDead;					//�Ƿ����� Ĭ�ϴ��
		CScenePlayer*			pPlayer;				//���ָ��
		bool					bAlreadyOnline;			//�Ƿ��һ�ν���
		uint64					uHurtValue;				//�˺���ֵ
		uint32					uKillCount;				//��ɱ��Ҹ���
		uint32					uContinueKillCount;		//������ɱ��Ҹ���	�㳬��
		uint32					uLimitTimeKillCount;	//��ʱ��ɱ��Ҹ��� �㱩��
		uint32					uLastKillTime;			//�ϴλ�ɱ���ʱ��
		uint32					uDeadCount;				//��������
		uint32					uAssistKillCount;		//��������
		uint32					uCapturenum;			//�������
		uint32					uContri;				//����ֵ
		uint32					uContriTime;			//���ӹ���ֵ��ʱ�� ����mvp��
		uint32					uGodlikeCount;			//�������
		uint32					uMegakillCount;			//���ߴ���
		uint32					uMvpCount;				//mvp����
		bool					bNoPrize;				//�Ƿ��޽���
		uint8					uOldCampID;				//֮ǰ����ӪID
		uint8					uOldPkType;				//֮ǰ��pkģʽ

		_emCopymapCompleteType	emCopymapCompleteType;	//�������

		void	initialize()
		{
			dMemset(this,0,sizeof(_stCopymapPlayer));
			emCopymapCompleteType = CopymapCompleteType_None;
		}
		void	saveToProtobuf		(PBPvpBannerPlayerInfo* pbData);
	};
	typedef	stl_map<uint32,_stCopymapPlayer>	MAP_PLAYER;
protected:
	MAP_PLAYER				m_mapCopymapPlayer;		//���������Ϣ
	uint8					m_eCopymapState;		//����״̬ _emCopymapState
	uint32					m_uStartTime;			//��������ʱ�� UNIXʱ��
	uint32					m_uRefreshWaveTime;		//��������ʱ�� UNIXʱ��
	uint16					m_uCurWaveID;			//��ǰ����
	uint32					m_uCloseTime;			//�����ر�ʱ�� UNIXʱ��
	uint32					m_uCompleteTime;		//���������ʱ���룩
	_emCopymapCompleteType	m_emCopymapCompleteType;//����������� _emCopymapCompleteType
	uint32					m_uExtraNum;			//������� ������������,
	uint32					m_uCreateNpc;			//�ĸ�npc���������ģ����ڹ��ﴴ����׼��
	uint16					m_uCreateLevel;			//�����Ļ�׼�ȼ� ����Χ������
	uint32					m_uReadyStartTime;		//׼����ʼ��ʱ�䣨ʱ���ڲ�֪ͨ��
	PBPlayerDisplay			m_pbCreator;			//�������������
	uint32					m_uCash;				//�ͽ�����
	bool					m_bCashCopyMap;			//�Ƿ����ͽ𸱱� 
	stl_set<CSceneItem*>	m_setDropUnit;			// ������Ʒ

protected:
	CGameSceneCell*			m_pSceneCell;
	const _stCopymapModel*	m_pCopymapModel;
public:
	inline const _stCopymapModel*	getCopymapModel		()const		{	return m_pCopymapModel;												}
	inline uint16					getCopymapID		()const		{	return (m_pCopymapModel ? m_pCopymapModel->uCopymapID : 0);			}
	inline uint32					getCopymapType		()const		{	return (m_pCopymapModel ? m_pCopymapModel->uType:0);				}
	inline uint8					getCopymapMainType	()const		{	return (m_pCopymapModel ? m_pCopymapModel->uMainType:0);			}
	inline bool						canRelive			()const		{	return (m_pCopymapModel ? m_pCopymapModel->bCantRelive: true);		}
	inline bool						canOperate			()const		{	return (m_pCopymapModel && m_pSceneCell);							}
	bool							isLastWave			();
	inline uint32					getCreateNpc		()const		{	return m_uCreateNpc;												}
	inline _emCopymapState			getCopymapState		()const		{	return (_emCopymapState)m_eCopymapState;							}
	inline _stCopymapPlayer*		getCopymapPlayer	(uint32 uPlayerID)	{	return m_mapCopymapPlayer.find_(uPlayerID);					}
	inline uint32					getCashPlayerID		()			{	return m_bCashCopyMap ? m_pbCreator.playerid() : 0;						}
public:
	CCopymapBase();
	virtual~CCopymapBase();
public:
	virtual void			initialize			();
	virtual void			restart				(CGameSceneCell*pScene,const PBCreateCellParam&pbParam);
	virtual	void			close				();
	virtual	bool			isPrefect			();
	virtual	uint32			getExtraNum			() { return 0; }
	//--- �������
	virtual	void			on_copymap_complete	(uint32 uTime,_emCopymapCompleteReson emReason);
	// ���ùر�ʱ��
	virtual void			set_closetime		(uint32 uTime);
	// ��ʼˢ�²���
	virtual void			begin_wave			();

	void					broadCastMsg		(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	void					broadCastBuffer		(uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);

	void					updatePlayerInfo	(PBW2GCopymapPlayerInfo&msg);
	_emCopymapEnterReason	getEnterType		(uint32 uPlayerID) const;
	uint32					getDeadNum			();
	bool					isAllDead			();
	bool					isAllLife			();
	bool					isAllExit			();
	bool					isHavePlayerExit	();
	bool					refreshUnit			(const _stUnitGroupModel* pGroupModel);
	bool					synWaveInfo			();
	bool					transToBirthRegion	();
	CScenePlayer*			getTeamLeader		();
	void					callback_callAllPlayer(CSceneMonster* pMonster);

	void					callback_AICloseCopymap(uint32 uTime, uint32 uChatid);
	void					clearMonsterArea(CSceneMonster* pMonster);

	void					AddSceneItem(CSceneItem *pSceneItem);

public:
	//--- ��ɫ���볡��
	virtual void			playerEnter			(CScenePlayer*pPlayer);
	//--- ��ɫ����
	virtual void			playerReconnect		(CScenePlayer*pPlayer);
	//--- ��ɫ�뿪����
	virtual void			playerLeave			(CScenePlayer*pPlayer);
	//--- ��ɫ���ߴ���(��ʼ�곡��ͬ��)
	virtual void			playerOnline		(CScenePlayer*pPlayer);
	virtual	void			onFirstOnline		(CScenePlayer*pPlayer);
	void					onClientGo			(CScenePlayer*pPlayer);
public:
	//--- �����ƽ�
	virtual void			process				(uint64 uTick,uint64 uTime,uint32 uSecond);
protected:
	// �����ƽ�
	virtual void			process_ing			(uint64 uTick,uint32 uTime,uint32 uSecond);
	// ��ɺ��ƽ�
	virtual void			process_done		(uint64 uTick,uint32 uTime,uint32 uSecond);

	void					synState2World		();

protected:
	//--- ��⸱�����
	virtual	void			check_complete		(uint32 uTime);
	//--- ��⸱���ر�
	virtual void			check_closeing		(uint32 uTime);
	//--- ��⸱��ˢ�²���
	virtual void			check_refreshWave	(uint32 uTime);

public:
	virtual	void			callback_killmonster	(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	virtual	void			callback_killplayer		(CScenePlayer* pSource,CScenePlayer* pTarget);
	virtual void			callback_depotGoods		(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	virtual	void			callback_arrestmonster	(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	virtual	void			callback_thugmonster	(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	virtual	bool			unactivemonster			(CSceneMonster* pMonster,CScenePlayer* pPlayer,uint8& uCode);
	virtual	void			callback_playerlife		(CScenePlayer* pPlayer,CScenePlayer* pSource);
	virtual	void			callback_playerdead		(CScenePlayer* pPlayer,CSceneUnit* pSourceUnit);
	virtual	void			callback_npcdialog_complete	(CScenePlayer* pPlayer,CSceneMonster* pNpc);
	virtual	void			callback_createmonster	(CSceneMonster* pMonster);
	virtual	void			callback_newtrap		(CSceneTrap* pTrap) {}
	virtual	void			callback_monsterhurt	(CSceneMonster* pMonster,CSceneUnit* pSourceUnit,int32 iHurt);
	virtual	void			callback_flyout			(CScenePlayer* pPlayer);
	virtual void			callback_trapLeave		(CSceneTrap*pTrap);
	virtual	void			callback_player_gethurt	(CScenePlayer* pPlayer, uint32 uHurt, CScenePlayer* pPlayerSource);//���������³����˺�

private:
	void					giveSkill			(CScenePlayer* pPlayer,bool bAdd,_emDoingType emDoing);
	void					startCopymap		();	

	// �ͽ��ŷ�Ǯ
	void					giveCach			();
	void					sceneItemSendMail		();	

};
