/*----------------- game_report.h
*
* Copyright (C): 2013  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2013/11/7 20:26:34
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
class game_report
{
protected:
	int8	m_separate;			//间隔符
	char	m_head[1024];		//头缓冲区
	char	m_buffer[1024*4];	//上报缓冲区

protected:
	char	m_talbeSuffix[256];	//表后后缀

protected://最后上报构建信息头
	uint32	m_last_head_id;
	uint64	m_last_head_time;

public:
	game_report();
	virtual~game_report(){}

public:
	//--- 设置间隔符
	void			set_separate		(char _char);
	//--- 设置上报表后缀
	void			set_talbeSuffix		(pc_str _suffix);

protected:
	//--- 发送
	virtual	void	sendTo				(const void*pBuffer,uint32 uSize) = 0;
	//--- 上报
	virtual	bool	reportTo			(uint8 uType);
//
//public:
//	//--- 构建记录头
//	void			report_head			(_stPlayer& stPlayer,uint64 uTime);
//
//public:
//	/*----->{ 基本信息 }*/
//	virtual	bool	reportBase			(_stGamePlayer& stPlayer,char* szAccountName,uint64 uTime);
//
//	/*----->{ 坐骑 }*/
//	virtual	bool	reportMount			(_stGamePlayer& stPlayer,uint8 uBranchID,_stPlayerMountBase&stPlayerMount);
//
//	/*----->{ 骑兵 }*/
//	virtual	bool	reportRidingWeapon	(_stRidingWeapon& stRidingWeapon);
//
//	/*----->{ 四灵 }*/
//	virtual	bool	reportSouls			(_stPlayerQuest& stPlayer);
//
//	/*----->{ 弓箭 }*/
//	virtual	bool	reportBow			(_stPlayerBow& stBow);
//
//	/*----->{ 美人 }*/
//	virtual	bool	reportGeneral		(_stPlayerGeneral& stGeneral);
//
//	/*----->{ 宝石 }*/
//	virtual	bool	reportPrecious		(_stGamePlayer& stPlayer);
//
//	/*----->{ 签到次数 }*/
//	virtual	bool	reportCheckInCount	(_stGamePlayer& stPlayer);
//
//	/*----->{ 装备 }*/
//	virtual	bool	reportEquip			(_stGamePlayer& stPlayer);
//
//	/*----->{ 八门遁甲 }*/
//	virtual	bool	reportFighters		(_stPlayerFighters& stFighters);
//
//	/*----->{ 玉佩 }*/
//	virtual	bool	reportKeepsake		(_stPlayerKeepsake& stKeepsake);
//
//	/*----->{ 四灵 }*/
//	virtual	bool	reportDrugTripod	(_stPlayerDrugTripod& stDrugTripod);
//
//	/*----->{ 坐骑装备 }*/
//	virtual	bool	reportMountEquip	(_stGamePlayer& stPlayer);
//
//	/*----->{ 副本 }*/
//	virtual	bool	reportCopyMap		(_stPlayerCopymap& stCopymap);
//
//	/*----->{ 角色潜能 }*/
//	virtual	bool	reportPotential		(_stPlayerPotential& stPotential);
//
//	/*----->{ 角色技能 }*/
//	virtual	bool	reportSkill			(_stGamePlayer& stPlayer);
//
//	/*----->{ 武将神兵 }*/
//	virtual	bool	reportGeneralWeapon	(_stPlayerGeneralWeapon& stWeapon);
//
//	/*----->{ 道具使用 }*/
//	virtual	bool	reportItemRecord	(_stItemFlag&stItem);
//
//	/*----->{ 旌旗 }*/
//	virtual	bool	reportBanner		(_stPlayerBanner& stBanner);
//	
//	/*----->{ 武魂 }*/
//	virtual	bool	reportSpirit		(_stPlayerSpirit& stSpirit);
//
//public:
//	//--- 记录城池信息
//	virtual	bool	reportCity			(_stCity&stModel,uint64 uTime);
//	//--- 记录龙城飞将信息
//	virtual	bool	reportMagicWeapon	(_stMagicWeapon&stModel,uint64 uTime);
//
//public:
//	//--- 记录城池信息
//	virtual	bool	reportGiftActivities(_stGamePlayer& stPlayer,char* szAccountName,uint16 uActivitiesID,uint16 uGiftCount,uint64 uTime);
};
