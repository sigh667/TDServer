/*------------- hatredMgr.h
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.01
* Date         : 2016/02/26
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "basic/memoryFunctions.h"
#include "stl/std_map.h"
#include "math/point2.h"
#include "def_struct/gameDefine.h"
#include "memoryPool/mapPool.h"
/*************************************************************/
class CScenePlayer;
class CSceneUnit;
struct _stSkillModel;
struct _stSkillEffectModel;
class CHatredMgr
{
	friend		class	CSceneUnitAI;
public:
	struct _stHatred
	{
		uint32	uUnitID;	//����ID
		int32	nLevel;		//��޵ȼ�
		int32	nHatred;	//���ֵ

		inline void		initialize	()	{	dMemset(this, 0, sizeof(_stHatred));	}
	};
private:
	typedef CMapPool<uint32, _stHatred>		MAP_HATRED;
	typedef stl_set<uint32>					SET_HOST;
private:
	CSceneUnit*		m_pOwner;		//ӵ����
	//����б�ͱ�����б���һһ��Ӧ�ģ�
	//�����ｫ��Ҽ������б��ʱ�����ҲҪ����������뱻����б�
	//ɾ����޹�ϵ����ӳ�޹�ϵ�����³��ֵ��Ӧ����˫�����
	//��������ߣ���ת��ͼ����������ձ�����б�ɾ�����г�޹�ϵ
	//������������Ĺ������ߣ�ɾ��ĳһ����޹�ϵ
	//����յ����ƣ�������ҲҪ���µ��������ߵı�����б����ĳ���б��
	//���������������״̬�£����������䷶Χ����Ҽ����޹�ϵ����Ҳ��������������
	//�����յ���һ���npc��������˺������Է��������б�
	//������������ճ���б�ɾ�����г�޹�ϵ
	MAP_HATRED		m_mapHatred;			//����б�    ����ӵ�г���б�
	SET_HOST		m_setBeHatred;			//������б�  ��Ҷ�Ӧ�б�����б�
	_stHatred*		m_pFirstHatred;			//��߳��
public:
	void			initialize				(CSceneUnit* pUnit);

	void			clear					();
	void			clearHatred				();
	void			clearBeHatred			();

	uint32			getFirstHatredID		();
	uint32			getOrderHatredID		(uint32 uOrder);
	uint32			getRandomHatredID		();

	void			updateHatred			(CSceneUnit* pUnit, int32 nHatred, bool bCall = true);
	void			addHatredLevel			(CSceneUnit* pUnit, int32 nAddHatredLevel);
	_stHatred*		newHatred				(CSceneUnit* pUnit, int32 nHatred);
	void			deleteHatred			(CSceneUnit* pUnit);
	bool			hasHatred				();
	bool			hasBeHatred				();
	bool			isHatred				(uint32 uUnitID);

	bool			canHatred				(CSceneUnit* pUnit);
	bool			getHatredPlayer			(stl_set<CScenePlayer*>& setPlayer);
public:
	// �����ص�
	void			onDead					();
	// �뿪�����ص�
	void			onLeaveScene			();
	// �������˺��ص�
	void			onByDamage_Skill		(CSceneUnit* pSource, int32 nHP,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	// ��buff�˺��ص�
	void			onByDamage_Buff			(CSceneUnit* pSource, int32 nHP);
	// ���������ƻص�
	void			onByCure_Skill			(CSceneUnit* pSource, int32 nHP,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	// ��buff���ƻص�
	void			onByCure_Buff			(CSceneUnit* pSource, int32 nHP);
	// �ɼ�״̬�ı�ص�
	void			onRefreshVisible		();
	// ���뾯�䷶Χ�ص�
	void			onEnterFightDis			(CSceneUnit* pSource);
private:
	void			updateFirstHatred		();
public:
	CHatredMgr();
	~CHatredMgr();
};
