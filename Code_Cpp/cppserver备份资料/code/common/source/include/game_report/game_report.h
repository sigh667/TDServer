/*----------------- game_report.h
*
* Copyright (C): 2013  Mokylin��Mokyqi
* Author       : ����Դ
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
	int8	m_separate;			//�����
	char	m_head[1024];		//ͷ������
	char	m_buffer[1024*4];	//�ϱ�������

protected:
	char	m_talbeSuffix[256];	//����׺

protected://����ϱ�������Ϣͷ
	uint32	m_last_head_id;
	uint64	m_last_head_time;

public:
	game_report();
	virtual~game_report(){}

public:
	//--- ���ü����
	void			set_separate		(char _char);
	//--- �����ϱ����׺
	void			set_talbeSuffix		(pc_str _suffix);

protected:
	//--- ����
	virtual	void	sendTo				(const void*pBuffer,uint32 uSize) = 0;
	//--- �ϱ�
	virtual	bool	reportTo			(uint8 uType);
//
//public:
//	//--- ������¼ͷ
//	void			report_head			(_stPlayer& stPlayer,uint64 uTime);
//
//public:
//	/*----->{ ������Ϣ }*/
//	virtual	bool	reportBase			(_stGamePlayer& stPlayer,char* szAccountName,uint64 uTime);
//
//	/*----->{ ���� }*/
//	virtual	bool	reportMount			(_stGamePlayer& stPlayer,uint8 uBranchID,_stPlayerMountBase&stPlayerMount);
//
//	/*----->{ ��� }*/
//	virtual	bool	reportRidingWeapon	(_stRidingWeapon& stRidingWeapon);
//
//	/*----->{ ���� }*/
//	virtual	bool	reportSouls			(_stPlayerQuest& stPlayer);
//
//	/*----->{ ���� }*/
//	virtual	bool	reportBow			(_stPlayerBow& stBow);
//
//	/*----->{ ���� }*/
//	virtual	bool	reportGeneral		(_stPlayerGeneral& stGeneral);
//
//	/*----->{ ��ʯ }*/
//	virtual	bool	reportPrecious		(_stGamePlayer& stPlayer);
//
//	/*----->{ ǩ������ }*/
//	virtual	bool	reportCheckInCount	(_stGamePlayer& stPlayer);
//
//	/*----->{ װ�� }*/
//	virtual	bool	reportEquip			(_stGamePlayer& stPlayer);
//
//	/*----->{ ���Ŷݼ� }*/
//	virtual	bool	reportFighters		(_stPlayerFighters& stFighters);
//
//	/*----->{ ���� }*/
//	virtual	bool	reportKeepsake		(_stPlayerKeepsake& stKeepsake);
//
//	/*----->{ ���� }*/
//	virtual	bool	reportDrugTripod	(_stPlayerDrugTripod& stDrugTripod);
//
//	/*----->{ ����װ�� }*/
//	virtual	bool	reportMountEquip	(_stGamePlayer& stPlayer);
//
//	/*----->{ ���� }*/
//	virtual	bool	reportCopyMap		(_stPlayerCopymap& stCopymap);
//
//	/*----->{ ��ɫǱ�� }*/
//	virtual	bool	reportPotential		(_stPlayerPotential& stPotential);
//
//	/*----->{ ��ɫ���� }*/
//	virtual	bool	reportSkill			(_stGamePlayer& stPlayer);
//
//	/*----->{ �佫��� }*/
//	virtual	bool	reportGeneralWeapon	(_stPlayerGeneralWeapon& stWeapon);
//
//	/*----->{ ����ʹ�� }*/
//	virtual	bool	reportItemRecord	(_stItemFlag&stItem);
//
//	/*----->{ ��� }*/
//	virtual	bool	reportBanner		(_stPlayerBanner& stBanner);
//	
//	/*----->{ ��� }*/
//	virtual	bool	reportSpirit		(_stPlayerSpirit& stSpirit);
//
//public:
//	//--- ��¼�ǳ���Ϣ
//	virtual	bool	reportCity			(_stCity&stModel,uint64 uTime);
//	//--- ��¼���Ƿɽ���Ϣ
//	virtual	bool	reportMagicWeapon	(_stMagicWeapon&stModel,uint64 uTime);
//
//public:
//	//--- ��¼�ǳ���Ϣ
//	virtual	bool	reportGiftActivities(_stGamePlayer& stPlayer,char* szAccountName,uint16 uActivitiesID,uint16 uGiftCount,uint64 uTime);
};
