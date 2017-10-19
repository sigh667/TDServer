/*----------------- playerCopymap.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/5/10 14:57:18
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "protocol/PlayerStruct.pb.h"
#include "memoryPool/mapPool.h"
/*************************************************************/
struct _stPlayerCopymap
{
	uint16		uCopymapID;
	uint32		uEnterCount;

	_stPlayerCopymap&getInfo()			{	return *this;					}
	inline void	initialize()			{	memset(this,0,sizeof(*this));	}

	void		loadFromProtobuf		(const PBCopymapInfo&pbInfo);
	void		saveToProtobuf			(PBCopymapInfo&pbInfo);
};
//-------------------------------------------------------------
//------------------------------ 
struct _stCopymapModel;
class PBW2GCopymapEnter;
class PBPlayerCopymap;
class CPlayerCopymap : public CGamePlayerFun
{
	typedef CMapPool<uint16,_stPlayerCopymap>	MAP_CopymapInfo;

	uint32			m_uAssistCount;
	MAP_CopymapInfo	m_mapCopymapInfo;
public:
	CPlayerCopymap(CScenePlayer& clPlayer);
	virtual ~CPlayerCopymap();
	virtual	void	initialize				();
	virtual	bool	loadFromProtobuf		(const PBPlayerData& pbPlayer);
	virtual	bool	saveToProtobuf			(PBPlayerData& pbPlayer);
	virtual	_emGameSystemType	getFunType	()const	{	return GameSystemType_Copymap;	}

	virtual	void	onLogin					();
	//--- 每日清理
	virtual void	cleanupDaily			();
	//--- 每周清理
	virtual void	cleanupWeekly			();
	virtual	void	newOnline				();
	//--- 同步前端
	virtual void	onClientGo				();
public:
	void	leaveCopymap					();

	_stPlayerCopymap*	getInfo				(uint16 uCopymapID);
	inline void			updateAssistCount	(uint32 uCount)	{	m_uAssistCount += 1;	}
	static bool sort_time					(CSceneMonster* left, CSceneMonster* right);
	bool	canPrize						(uint32 uCopymapID);

protected:
	bool	save							(PBPlayerCopymap& pbData);

	void	synWorldServerAll				();
	void	synWorldServerOne				(_stPlayerCopymap& stData);
	bool	canCreate						(const _stCopymapModel* pCopymapModel,uint32& uCash,uint8& uCode);
	bool	canTeamForceEnter				(const _stCopymapModel* pCopymapModel,CSceneMonster* pNpc,uint32& uCash,uint8& uCode);
private:
	bool	handle_Copymap_Create			(uint8 uEventID, CNetStream& clStream);
	bool	handle_Copymap_Exit				(uint8 uEventID, CNetStream& clStream);
	bool	handle_Copymap_Invite			(uint8 uEventID, CNetStream& clStream);
	bool	handle_Copymap_AcceptInvite		(uint8 uEventID, CNetStream& clStream);
	bool	handle_Copymap_TeamMemberInfo	(uint8 uEventID, CNetStream& clStream);
	bool	handle_Copymap_EnterNpc			(uint8 uEventID, CNetStream& clStream);
	bool	handle_Copymap_FindAttackMonster(uint8 uEventID, CNetStream& clStream);
	bool	handle_Copymap_UnAcitve			(uint8 uEventID, CNetStream& clStream);
	
public:
	bool	copymap_create					(uint16 uCopymapID,uint32 uNpcID,uint8& uCode);
	bool	copymap_enternpc				(uint16 uCopymapID,uint32 uNpcUnitID,uint8& uCode);
	bool	copymap_exit					(uint16 uCopymapID,uint8& uCode);
	bool	copymap_invite					(uint16 uCopymapID,uint32 uTargetID,uint8& uCode);
	bool	copymap_acceptinvite			(uint16 uCopymapID,uint32 uTargetID,bool bOk,uint8& uCode);
	bool	copymap_teammemberinfo			(uint16 uCopymapID,uint8& uCode);
	bool	copymap_clear					(uint16 uCopymapID);
	bool	findAttackMonster				();
public:
	void	on_copymap_enter				(PBW2GCopymapEnter& ack,uint8 uCode);
	void	on_copymap_complete				(const _stCopymapModel* pModel,_emCopymapCompleteType emType,_emCopymapEnterReason emReason,uint32 uExtraNum);
};
