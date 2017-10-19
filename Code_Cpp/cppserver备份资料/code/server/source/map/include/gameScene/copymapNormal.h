/*----------------- copymapNormal.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/5/16 18:21:47
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameScene/copymapBase.h"
#include "stl/std_map.h"
#include "cfg_data/questData/questModel.h"
/*************************************************************/
class CSceneMonster;
class CScenePlayer;
class CGameSceneCell;
struct _stQuestModel;
struct _stQuestTarget;
class CCopymapNormal : public CCopymapBase
{
	typedef CCopymapBase Parent;
private:
	stl_map<uint32,uint32>				m_mapQuestCounter;	//����Ŀ����ɼ��� Ŀ������==>����
	const _stQuestModel*				m_pQuestModel;		//����ģ��
public:
	virtual void	initialize			();
	virtual void	restart				(CGameSceneCell*pScene,const PBCreateCellParam&pbParam);
	//--- ��ɫ���ߴ���(��ʼ�곡��ͬ��)
	virtual void			playerOnline		(CScenePlayer*pPlayer);
public:
	void					changeCopymapTask	(uint32 uQuestID);
protected:
	// ��������Ŀ�����
	bool					trigerQuestEvent	(uint8 uType,uint32 uSubType,uint32	uCount = 1);
protected:
	virtual	void			callback_killmonster(CSceneMonster* pMonster,CScenePlayer* pPlayer); 
	virtual	void			callback_depotGoods	(CSceneMonster* pMonster,CScenePlayer* pPlayer);
	//--- ��⸱�����
	virtual	void			check_complete		(uint32 uTime);
	virtual void			onQuestFinished		();

	void					triggerTargetAction	(const stl_vector<_stQuestTargetAction>& vecAction);
};
