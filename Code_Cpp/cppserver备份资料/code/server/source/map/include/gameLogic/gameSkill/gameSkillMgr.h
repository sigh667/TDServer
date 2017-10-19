/*----------------- gameSkillMgr.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/1/20 17:31:28
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "memoryPool/mapPool.h"
#include "gameLogic/gameSkill/gameSkill.h"
#include "protocol/GameDefine.pb.h"
#include "memoryPool/queue_pool.h"
/*************************************************************/
class CSceneUnit;
class PBSystemSkill;
struct _stItemModel;
class CGameSkillMgr	
{
	friend	class	CPlayerSkill;
	friend	class	CGameBuffMgr;
protected:
	enum _emSkillCmdType
	{
		SkillCmdType_Use,			// ʹ�ü���
		SkillCmdType_Charge,		// �ͷ���������
		SkillCmdType_Break,			// ��ϼ���
	};
	struct _stSkillCmd
	{
		_emSkillCmdType			eCmdType;
		const _stSkillModel*	pSkillModel;
		float32					fDirection;
		uint32					uTarUnitID;
		_stPoint2F				stSelfPoint;
		_stPoint2F				stTargetPoint;

		void	initialize()
		{
			pSkillModel	= NULL;
			fDirection	= 0.f;
			uTarUnitID	= 0;
			stSelfPoint.zero();
			stTargetPoint.zero();
		}
	}; 
public:
	typedef CMapPool<uint32,CGameSkill>	POOL_SKILL;
	typedef	stl_vector<CGameSkill*>		VEC_SKILL;
	typedef queue_pool<_stSkillCmd>		QUEUE_CMD;
private:
	CSceneUnit*			m_pOwner;						//������
	POOL_SKILL			m_mapSkill;						//�����б�
	uint64				m_uProcessTick;
	VEC_SKILL			m_vecSkill[_emSkillUseType_ARRAYSIZE];//������ʹ�÷�ʽ������
	uint32				m_uContinueNotHit;				//����δ����
private:
	QUEUE_CMD			m_queueCmd;						// �������  Ϊ���������ʹ���������ܶ���Ƶ� ԭ�����£�
														// 1.���ǲ���ʹ�� ǰ�˲��ȴ������Ϣ���Ȳ��ż��ܶ����ķ�ʽ ��Ϊ���Ǻܶ༼�ܶ�������λ�ƣ����� �ܾ���ͨ�����������ʹ��ǰ���Ȳ��Ŷ����ķ�ʽ ��ǰ��˵�λ�þ��޷���֤һ���ˡ�
														// 2.���ǲ���ʹ�� ǰ�˵ȴ������Ϣ��Ų�ȡ��һ�������ķ�ʽ ��Ϊ�����кܶ��������ܣ����� �ܾ���ͨ���������������������� ����ȡ�ȴ����֪ͨ����һ������ʩ����Ϣ��� 
														//	 ����ʹ����һ�����ܵ���Ϣ�Ļ� �м���������ӳ�+�����ӳٵ��µļ��ʱ��  �����Ļ� �������ܾͻ��ò����ᡣ
														// 3.���������о��� Ϊ�˴����������������Ƶ�
public:
	CGameSkillMgr();
	~CGameSkillMgr();

public:
	inline CSceneUnit*	getOwner			()						{	return m_pOwner;								}
	inline CGameSkill*	findSkill			(uint32 uSkillID)		{	return m_mapSkill.find(uSkillID);				}
	inline	void		bind				(CSceneUnit* pUnit)		{	m_pOwner = pUnit;								}
	inline	bool		canOperate			()						{	return m_pOwner != NULL;						}
	
public:
	uint8	getSkillLevel		(uint32 uSkillID);
	void	gatherHaloVec		(VEC_U16& haloVec);
public:
	void	clear				();
	void	initialize			(CSceneUnit* pOwner);
	bool	load				(const PBSystemSkill& pbSkill);
	bool	save				(PBSystemSkill* pbSkill);

public:
	bool	canUse				(uint32 uSkillID,uint8& uCode);
	bool	canAdd				(uint32 uSkillID,uint8& uCode);
	bool	canUpgrade			(uint32 uSkillID,uint8 uLevel,uint8& uCode);
	void	updateTempSkill		();
public:
	//--- ����ʹ�ü���
	bool	useSkillAsk		(uint32 uSkillID,float32 fDirection,uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode);
		//---  ������Ӽ���
	bool	addSkillAsk		(uint32 uSkillID,uint8 uLevel,uint8& uCode);
	//---  ������������
	bool	upgradeSkillAsk	(uint32 uSkillID,uint8 uLevel,uint8& uCode);
public:
	//--- ʹ�ü��� ָ��Ԥ��
	bool	addSkillUseCmd	(uint32 uSkillID,float32 fDirection,uint32 uTarUnitID,_stPoint2F&stSelfPoint,_stPoint2F&stTargetPoint,uint8& uCode);
	//--- �ͷ��������� ָ��Ԥ��
	bool	addSkillChargeCmd(uint32 uSkillID,uint8& uCode);
	//--- ��ϼ��� ָ��Ԥ��
	bool	addSkillBreakCmd(uint32 uSkillID,uint8& uCode);
	//--- ִ��Ԥ��ָ��
	void	processNextCmd	();

	void	clearSkillCmd	();
public:
	//--- ��Ӽ���
	bool	addSkill		(uint32 uSkillID,uint8 uLevel,bool bTmp,bool bSend,_emDoingType );
	//--- ɾ������
	bool	delSkill		(uint32 uSkillID,bool bSend,_emDoingType emDoing);
	//--- ��������
	bool	upgradeSkill	(uint32 uSkillID,uint8 uLevel,bool bSend,_emDoingType );
	//--- �ڲ�ʹ�ü���
	bool	useSkill		(uint32 uSkillID,uint8 uLevel,float32 fDirection,uint32 uTarUnitID,_stPoint2F&stTargetPoint);
	//--- ������еļ���CD
	void	clearSkillCD	(uint32 uSkillID);
public:
	static bool	send_UseSkill_Ack		(uint32 uSkillID,uint8 uLevel,uint8 uStage,uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8 uCode,bool bBroad,CSceneUnit* pOwner,_stPoint2F* pStartPoint = NULL);
	static bool	send_SkillBreak_Ack		(uint32 uSkillID,uint8 uStage,uint8 uCode,bool bBroad,CSceneUnit* pOwner);
	static bool	send_AddSkill_Ack		(uint32 uSkillID,uint8 uLevel,uint8 uCode,CSceneUnit* pOwner);
	static bool	send_DelSkill_Ack		(uint32 uSkillID,uint8 uCode,CSceneUnit* pOwner);
	static bool	send_UpgradeSkill_Ack	(uint32 uSkillID,uint8 uLevel,uint8 uCode,CSceneUnit* pOwner);
	static bool	send_AttackNotice_Ack	(CSceneUnit* pSource,CSceneUnit* pTarget,uint8 uNoticeType,bool bBroad = false);
	static bool	send_AttackDamage_Ack	(CSceneUnit* pSource,uint32 uSkillID,uint8 uCritical,int32 iDamage,bool bBroad,CSceneUnit* pOwner);
	static bool	send_SkillUseCharge_Ack	(uint32 uSkillID,uint8 uCode,bool bBroad,CSceneUnit* pOwner);
	
public:
	bool	process						(uint64 uTick);
protected:
	bool	trigerSkill					(const _stSkillModel* pSkillModel,uint32 uTarUnitID,uint32 uTrigerParam = 0);
	bool	tryTrigerSkill				(uint8 uSkillUseType,uint32 uTarUnitID,uint32 uTrigerParam = 0);

	void	sortSkill					();
public:
	void	callbackUseItem				(const _stItemModel* pItemModel);
	void	callbackUseSkill			(uint32 uTarUnitID);
	void	callbackBeSkillHit			(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	void	callbackBeKillHarm			(CSceneUnit* pUnit,int32& iDamage);
	void	callbackUpdateHP			();
	void	callbackSkillNotHit			();
	void	callbackSkillHit			();
};
