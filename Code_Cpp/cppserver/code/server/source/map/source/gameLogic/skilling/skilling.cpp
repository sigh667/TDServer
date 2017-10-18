/*----------------- skilling.cpp
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/1/26 9:57:51
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/skilling/skilling.h"
#include "basic/timeFunction.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/GameDefine.pb.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameScene/gameSceneCell.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
//#include "gameLogic/playerColletion/playerColletion.h"
#include "cfg_data/monsterData/monsterModel.h"
#include "cfg_data/monsterData/monsterData.h"
#include "cfg_data/constantData/constantData.h"
#include "cfg_data/skillData/skillData.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CSkilling::CSkilling()
{
	m_uLastCombatTick	= 0;	//��һ��������ʱ��
	m_uNextCombatSkill	= 0;	//��һ����������ID
	m_mapChildSkill.clear();
	m_pSkillModel		= NULL;
	m_uTarUnitID = 0;
	m_stOldPosition.zero();
	initialize(NULL,NULL);
}

//-------------------------------------------------------------
//------------------------------ 
void	CSkilling::initialize(CSceneUnit* pOwner, const _stSkillModel*	pSkillModel)
{
	m_pOwner			= pOwner;
	m_pSkillModel		= pSkillModel;
	m_pSkillAnimModel	= NULL;
	m_uCurStage			 = 0;
	m_uStartAnimTick	 = 0;
	m_uExpendCount		= 0;
	m_fChargeRate		= 0.0f;
	m_setChooseTarget.clear();
	m_setGatherTarget.clear();
	m_clSkillAnim.initialize(pOwner,m_uStartAnimTick,m_pSkillModel,m_pSkillAnimModel);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkilling::canUse		(CSceneUnit* pOwner, const _stSkillModel* pSkillModel,uint8& uCode)
{
	if (!pSkillModel || !pOwner)
		return false;

	if (!pOwner->canUseSkill(NULL))
	{
		uCode = R_ResultSkill_ActForbid;
		return false;
	}

	//�Ƿ���CD
	if (!pOwner->check_mark(SceneUnitMark_IgnoreSkillCD) || pSkillModel->uType == SkillType_NormalAtk)
	{
		//�Ƿ���ȴ��
		if (pSkillModel->uCoolTicket && pOwner->getCooling().isCooling(CoolingType_Skill,pSkillModel->uSkillID,SKILL_GOOLING_GROUP_ID))
		{
			uCode = R_ResultSkill_Cooling;
			return false;
		}
	}

	//�������
	if (!m_pOwner->check_mark(SceneUnitMark_IgnoreSkillExpend) && !pSkillModel->mapAddExpend.empty())
	{
		int32 nReduce = pOwner->getGameBuffMgr().getReduceSkillExpendRate();
		MAP_U16_I64 mapAddExpend;
		CONST_ITER_MAP_U16_I64 _pos = pSkillModel->mapAddExpend.begin();
		for (; _pos != pSkillModel->mapAddExpend.end(); ++_pos)
		{
			int64 iValue = _pos->second;
			iValue -= (int64)(iValue * (nReduce / 10000.0f));
			if (iValue)
				mapAddExpend.insert_(_pos->first,iValue);
		}
		if (!mapAddExpend.empty() && !pOwner->canExpend(mapAddExpend))
		{
			uCode = R_ResultSkill_Consume;
			return false;
		}
	}

	//���������ж�
	if (SkillCompositeType_Combat == pSkillModel->stCompositeData.uType)
	{
		if (pSkillModel->stCompositeData.uAtkPart > 1)
		{
			if ( pSkillModel->stCompositeData.uAtkPartCoolTicket && ( getSystemTick64() > m_uLastCombatTick + pSkillModel->stCompositeData.uAtkPartCoolTicket ) )
			{
				uCode = R_ResultSkill_Part;
				return false;
			}

			if (!m_uNextCombatSkill || m_uNextCombatSkill != pSkillModel->uSkillID)
			{
				uCode = R_ResultSkill_Part;
				return false;
			}
		}
	}
	else if (SkillCompositeType_Child == pSkillModel->stCompositeData.uType)
	{
		_stCompositeInfo* pInfo = getChildSkill(pSkillModel->uCompositeBaseSkillID);
		if (pInfo)
		{
			if (pInfo->m_uNextSkill != pSkillModel->uSkillID)
			{
				uCode = R_ResultSkill_Part;
				return false;
			}

			if (getSystemTick64() > pInfo->m_uExpireTick)
			{
				uCode = R_ResultSkill_Part;
				return false;
			}
		}
		else
		{
			if (pSkillModel->stCompositeData.uAtkPart > 1)
			{
				uCode = R_ResultSkill_Part;
				return false;
			}
		}
	}

	//��ɱ����
	if (SkillType_Thug == pSkillModel->uType && !useRewardSkill(uCode))
			return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ ��ȡ����ʱ��
uint32 CSkilling::getSkillTimeByType(const _stSkillModel* pSkillModel,_emSkillTimeType emType)
{
	if (!m_pSkillModel || !m_pOwner)
		return 0;

	uint32 uTick = 0;
	switch (emType)
	{
	case SkillTimeType_CD:
		uTick = pSkillModel->uCoolTicket;
		break;
	case SkillTimeType_Charge:
		uTick = pSkillModel->uChargeTime;
		break;
	case SkillTimeType_Continue:
		uTick = pSkillModel->uChargeTime;
		break;
	default:
		break;
	}

	//����buff
	uTick = m_pOwner->getGameBuffMgr().getSkillTimeByTime(emType,pSkillModel->uSkillID,uTick);
	return uTick;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkilling::useConsume		(bool bSinging,uint8& uCode)
{
	if (!canOperate())
		return false;

	//����
	if (!m_pOwner->check_mark(SceneUnitMark_IgnoreSkillExpend) && !m_pSkillModel->mapAddExpend.empty())
	{
		int32 nReduce = m_pOwner->getGameBuffMgr().getReduceSkillExpendRate();
		MAP_U16_I64 mapAddExpend;
		CONST_ITER_MAP_U16_I64 _pos = m_pSkillModel->mapAddExpend.begin();
		for (; _pos != m_pSkillModel->mapAddExpend.end(); ++_pos)
		{
			int64 iValue = _pos->second;
			if (iValue < 0)
				iValue -= (int64)(iValue * (nReduce / 10000.0f));

			if (iValue)
				mapAddExpend.insert_(_pos->first,iValue);

		}
		m_pOwner->addExpend(mapAddExpend, DoingType_Skill);
		m_uExpendCount++;
	}

	//�������Ӷ����˺�
	if (m_pSkillModel->stExpendAddDamage.uExpendType)
	{
		if (m_pOwner->addExpend(m_pSkillModel->stExpendAddDamage.uExpendType,m_pSkillModel->stExpendAddDamage.iValue, DoingType_Skill))
		{
			m_pOwner->update_mark(SceneUnitMark_AddExtraDamage,m_pSkillModel->uSkillID);
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ �����ƽ�����
bool	CSkilling::process		(uint64 uTick)
{
	processEndAnim(uTick);
	processExpend(uTick);
	m_clSkillAnim.process(uTick);

	{//���Ӽ��ܵȴ�����ʱ�����
		MAP_COMPOSITEINFO::iterator _pos =  m_mapChildSkill.begin();
		for (; _pos != m_mapChildSkill.end();)
		{
			_stCompositeInfo& stInfo = _pos->second;
			if (!stInfo.m_uExpireTick || stInfo.m_uExpireTick > getSystemTick64())
			{
				 ++_pos;
				continue;
			}
			CGameSkill* pGameSkill = m_pOwner->getGameSkillMgr().findSkill(stInfo.m_uBaseSkill);
			if (!pGameSkill || !pGameSkill->getSkillModel())
			{
				++_pos;
				continue;
			}

			m_mapChildSkill.erase(_pos++);
			uint32 uCoolTime = getSkillTimeByType(pGameSkill->getSkillModel(),SkillTimeType_CD);
			m_pOwner->getCooling().addCooling(CoolingType_Skill,pGameSkill->getSkillModel()->uSkillID,uCoolTime,SKILL_GOOLING_GROUP_ID, pGameSkill->getSkillModel()->uGroupCoolTicket);
		}
	}
	
	//������ϼ��ܵ�CD
	if (m_pSkillModel && m_pSkillModel->stCompositeData.uNextSkillID > 0)
	{
		if (SkillCompositeType_Combat == m_pSkillModel->stCompositeData.uType && m_uLastCombatTick && m_pSkillModel->stCompositeData.uAtkPartCoolTicket &&
			getSystemTick64() >= m_uLastCombatTick + m_pSkillModel->stCompositeData.uAtkPartCoolTicket)
		{
			m_uLastCombatTick = 0;
			m_uNextCombatSkill = 0;
			CGameSkill* pBaseGameSkill = m_pOwner->getGameSkillMgr().findSkill(m_pSkillModel->uCompositeBaseSkillID);
			if (pBaseGameSkill && pBaseGameSkill->getSkillModel())
			{
				uint32 uCoolTime = getSkillTimeByType(pBaseGameSkill->getSkillModel(),SkillTimeType_CD);
				m_pOwner->getCooling().addCooling(CoolingType_Skill,m_pSkillModel->uCompositeBaseSkillID,uCoolTime,SKILL_GOOLING_GROUP_ID, pBaseGameSkill->getSkillModel()->uGroupCoolTicket);
			}
		}
	}

	//����������Ŀ�꼼�� ��Ҫ�ж�
	if (m_uTarUnitID && m_pOwner->haveState(StateType_Skiling) && m_pSkillModel && m_pSkillModel->check_mark(SkillMark_Continue) && m_pSkillModel->check_mark(SkillMark_SingLockTarget) )
	{
		CSceneUnit* pTarget = m_pOwner->findCellObject(m_uTarUnitID);
		if (!pTarget || (pTarget && pTarget->isDead()))
		{
			uint8 uCode = 0;
			breakSkillAsk(m_pSkillModel->uSkillID,true,uCode);
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ ���ĳ���
void	CSkilling::processExpend		(uint64 uTick)
{
	if (!canOperate() || !m_uStartAnimTick || !m_uExpendCount)
		return;

	const _stSkillSpareParam* pSpareParam = m_pSkillModel->findParam(SkillSpareParamType_ExpendInterval);
	if (!pSpareParam || pSpareParam->vecParams.empty())
		return;

	//���Ĳ��� ֱ��ֹͣ����
	if (uTick >= m_uStartAnimTick + pSpareParam->vecParams[0] * m_uExpendCount)
	{
		if (!m_pOwner->canExpend(m_pSkillModel->mapAddExpend))
		{
			uint8 uCode = 0;
			breakSkillAsk(m_pSkillModel->uSkillID,true,uCode);
			return;
		}

		m_pOwner->addExpend(m_pSkillModel->mapAddExpend, DoingType_Skill);
	}

}

//-------------------------------------------------------------
//------------------------------ �����ƽ�����
bool	CSkilling::processEndAnim		(uint64 uTick)
{
	if (!canOperate() || !m_pSkillAnimModel)
		return false;

	//��������
	if (m_pSkillModel->check_mark(SkillMark_Continue) && !m_pSkillModel->uContinueTime)
		return true;

	if (!m_clSkillAnim.isBreak() && (!m_uStartAnimTick || m_uEndAnimTick > uTick))
		return true;

	m_uStartAnimTick = 0;
	m_uEndAnimTick = 0;
	// 	if (m_pOwner->asPlayer())
	// 		CLog::error("ʩ������ ��ǰʱ��:%lld,����ID:%d",uTick,m_pSkillModel->uSkillID);

	//����������Ҫ�㲥ֹͣ
	if (m_pSkillModel->check_mark(SkillMark_Continue))
	{
		CGameSkillMgr::send_SkillBreak_Ack(m_pSkillModel->uSkillID,1,0,true,m_pOwner);
	}

	onDoneSkill();
	return true;
}

//-------------------------------------------------------------
//------------------------------ ������� �Զ�ʹ�ü���
void	CSkilling::onDoneSinging(uint32 uSingingTick)
{
	if (!canOperate())
		return;

	uint8 uCode = R_ResultSkill_Fail;

	//������ϵ��
	if (m_pSkillModel->check_mark(SkillMark_Charge))
	{
		uint32 uChargeTick = getSkillTimeByType(m_pSkillModel,SkillTimeType_Charge);
		if (uChargeTick)
		{
			float32 fTmpRate = (float32) MAX(uSingingTick,(uint32)1000) / uChargeTick;
			m_fChargeRate = MIN(fTmpRate,1.0f);
		}
	}

	//ʹ�ü���
	if (!readyUseSkill(getSingTriggerID(),uCode))
	{
		CGameSkillMgr::send_UseSkill_Ack(getSkillID(),1,m_uCurStage,getSingTriggerID(),getTargetPoint(),uCode,false,m_pOwner);
	}

	// �Ƿ�ɼ�����
	if(m_pSkillModel->uType == SkillType_MonsterCollletion)
	{
		if (!useColletionSkill(uCode))
		{
			CGameSkillMgr::send_UseSkill_Ack(getSkillID(),1,m_uCurStage,getSingTriggerID(),getTargetPoint(),uCode,false,m_pOwner);
		}
		return;
	}
	else if (m_pSkillModel->uType == SkillType_Arrest)
	{
		if (!arrestSkill(uCode))
		{
			CGameSkillMgr::send_UseSkill_Ack(getSkillID(),1,m_uCurStage,getSingTriggerID(),getTargetPoint(),uCode,false,m_pOwner);
		}
		return;
	}
	else if (m_pSkillModel->uType == SkillType_Active)
	{
		if (!activeSkill(uCode))
		{
			CGameSkillMgr::send_UseSkill_Ack(getSkillID(),1,m_uCurStage,getSingTriggerID(),getTargetPoint(),uCode,false,m_pOwner);
		}
		return;
	}
}

//-------------------------------------------------------------
//------------------------------ 
void	CSkilling::onDoneSkill(bool bBreak/* = false*/)
{
	if (!m_pOwner || !m_pSkillModel)
		return;
	m_pOwner->callbackUseSkillEnd(m_pSkillModel);
	m_pOwner->delState(StateType_Skiling);

	//������� ��ʼ����
	if (!bBreak && SkillType_Arrest == m_pSkillModel->uType)
	{
		uint8 uCode = 0;
		arrestSkill(uCode);
	}

	//������ɿ�ʼCD (��������)
	if (!m_pOwner->check_mark(SceneUnitMark_IgnoreSkillCD) && m_pSkillModel->check_mark(SkillMark_Continue))
	{
		uint32 uCoolTime = getSkillTimeByType(m_pSkillModel,SkillTimeType_CD);
		m_pOwner->getCooling().addCooling(CoolingType_Skill,m_pSkillModel->uSkillID,uCoolTime,SKILL_GOOLING_GROUP_ID,m_pSkillModel->uGroupCoolTicket);
	}

	m_pOwner->delete_mark(SceneUnitMark_AddExtraDamage);

	//ɾ��״̬
	const _stSkillSpareParam* pSkillSpareParam = m_pSkillModel->findParam(SkillSpareParamType_AddState);
	if (pSkillSpareParam && pSkillSpareParam->vecParams.size() > 0)
		m_pOwner->delState((_emGameStateType)(uint32)pSkillSpareParam->vecParams[0]);

	m_uStartAnimTick = 0;
	m_clSkillAnim.end();
	m_uLastCombatTick	= 0;	//��һ��������ʱ��
	m_uNextCombatSkill	= 0;	//��һ����������ID
	m_fChargeRate = 0.0f;

	//�������ɾ��buff
	const _stSkillSpareParam* pSpareParam = m_pSkillModel->findParam(SkillSpareParamType_doneDelBuff);
	if (pSpareParam && pSpareParam->vecParams.size() > 0)
	{
		m_pOwner->getGameBuffMgr().delBuff((uint32)pSpareParam->vecParams[0], DoingType_Skill);
	}

	//�������ܷ������Լ��ͷ�
	if (SkillCompositeType_Continue == m_pSkillModel->stCompositeData.uType && !bBreak)
	{
		_stPoint2F stTargetPoint(getTargetPoint());
		if (m_pSkillModel->stCompositeData.uNextSkillID > 0)
		{
			//�����ǰ������Ԥ������,��ô��һ������Ҫ���¶��巽��  �����ǳ�ײ����
			_stSkillModel* pNextModel = g_clSkillData.findSkill(m_pSkillModel->stCompositeData.uNextSkillID, m_pSkillModel->uLevel);
			if (m_pSkillModel->nRangeWarnID && m_pSkillModel->nRangeWarnType && pNextModel && pNextModel->findParam(SkillSpareParamType_Dash))
			{

				//����ʵʱ����
				CGameSceneCell* pCell = m_pOwner->getSceneCell();
				if (pCell)
				{
					CSceneUnit* pUnit = pCell->findObject(m_pOwner->getSkilling().getCurentTargetUnitId());
					if (pUnit != NULL)
					{
						stTargetPoint = pUnit->getPosition();
					}
				}
				//���³���	
				float32 fDir	= get2PiRadian(m_pOwner->getPosition(), stTargetPoint);
				m_pOwner->setDirection(fDir);

				//����ϰ���
				_stPoint2F _stRealTargetPosition;
				if (pCell && !pCell->getTargetPoint(m_pOwner->getPosition(), stTargetPoint, _stRealTargetPosition))
				{
					stTargetPoint = _stRealTargetPosition;
					//��Ҫ��ai���棬����ײǽ��
					m_pOwner->callbackUseSkillFailedByBlock(m_pSkillModel->stCompositeData.uNextSkillID);
				}
			}
			m_pOwner->getGameSkillMgr().useSkill(m_pSkillModel->stCompositeData.uNextSkillID,m_pSkillModel->uLevel,m_pOwner->getDirection(),getCurentTargetUnitId(),stTargetPoint);
			return ;
		}
	}

	//����
	if (SkillCompositeType_Combat == m_pSkillModel->stCompositeData.uType) 
	{
		m_uNextCombatSkill = m_pSkillModel->stCompositeData.uNextSkillID;
		m_uLastCombatTick = m_uNextCombatSkill ? getSystemTick64():0;
	}

	//���Ӽ��� ���һ��ɾ��
	if (SkillCompositeType_Child == m_pSkillModel->stCompositeData.uType)
	{
		if (!m_pSkillModel->stCompositeData.uNextSkillID)
		{
			m_mapChildSkill.erase(m_pSkillModel->uCompositeBaseSkillID);
		}
		else
		{
			_stCompositeInfo* pInfo = getChildSkill(m_pSkillModel->uCompositeBaseSkillID);
			if (pInfo )
			{
				pInfo->m_uNextSkill = m_pSkillModel->stCompositeData.uNextSkillID;
				pInfo->m_uExpireTick = getSystemTick64() + m_pSkillModel->stCompositeData.uAtkPartCoolTicket;
				pInfo->m_uBaseSkill = m_pSkillModel->uCompositeBaseSkillID;
			}
			else
			{
				_stCompositeInfo& stInfo = m_mapChildSkill[m_pSkillModel->uCompositeBaseSkillID];
				stInfo.m_uNextSkill = m_pSkillModel->stCompositeData.uNextSkillID;
				stInfo.m_uExpireTick = getSystemTick64() + m_pSkillModel->stCompositeData.uAtkPartCoolTicket;
				stInfo.m_uBaseSkill = m_pSkillModel->uCompositeBaseSkillID;
			}
		}
	}

	///����ɢ������ �Զ�ƫ��
	do 
	{
		if (m_pOwner->asMonster() && !m_setGatherTarget.empty())
		{
			ITER_SET_U32 _pos =  m_setGatherTarget.begin();
			if (*_pos == m_pOwner->getUnitID())
				break;

			CSceneUnit* pOffsetTarget = m_pOwner->findCellObject(*_pos);
			if (!pOffsetTarget)
				break;

			MAP_GAME_UNIT	mapObject;
			float32 fAngle = get2PiRadian(pOffsetTarget->getPosition(),m_pOwner->getPosition());
			float32 fDistance = getDistance(pOffsetTarget->getPosition(),m_pOwner->getPosition());
			float32 fModelRadius = m_pOwner->getSkillModelRadius() > 0.1f ? m_pOwner->getSkillModelRadius() : 0.1f;
			float32 fTargetModelRadius = pOffsetTarget->getSkillModelRadius() > 0.1f ? pOffsetTarget->getSkillModelRadius() : 0.1f;

			//����̫�� �����Զ�����������������
			if (pOffsetTarget->asPlayer(),fDistance < fModelRadius + fTargetModelRadius)
			{
				float32 fNewDistance = fModelRadius + fTargetModelRadius + 0.1f;
				_stPoint2F stNewPoint = pOffsetTarget->getPosition();
				stNewPoint.x += fNewDistance * ::sin(fAngle);
				stNewPoint.y += fNewDistance * ::cos(fAngle);
				//����ʵʱ����
				CGameSceneCell* pCell = m_pOwner->getSceneCell();
				if (pCell && m_pOwner->canMove() && pCell->canMove(stNewPoint.x,stNewPoint.y))
				{
					m_pOwner->setPosition(stNewPoint,SetPositionReason_Move,true);
				}
			}
			else
			{
				m_pOwner->gatherSceneObject(m_pOwner->getPosition(),fModelRadius,mapObject,_BIT32(GameObject_Monster));

				float32 fOffsetRadian = fModelRadius * 3/fDistance;
				MAP_GAME_UNIT	mapTarObject;
				m_pOwner->gatherSceneObject(pOffsetTarget->getPosition(),fDistance,mapTarObject,_BIT32(GameObject_Monster));
				if (mapObject.size() > ceil(mapTarObject.size() * fOffsetRadian / M_2PI) && fDistance > FLOAT_EPSINON)
				{
					_stPoint2F stNewPoint = pOffsetTarget->getPosition();
					float32	fOffsetNewRadian= get2PiRadian(fOffsetRadian + fAngle);
					stNewPoint.x += fDistance * ::sin(fOffsetNewRadian);
					stNewPoint.y += fDistance * ::cos(fOffsetNewRadian);

					//����ʵʱ����
					CGameSceneCell* pCell = m_pOwner->getSceneCell();
					if (pCell && m_pOwner->canMove() && pCell->canMove(stNewPoint.x,stNewPoint.y))
						m_pOwner->setPosition(stNewPoint,SetPositionReason_Move,true);
				}
			}
		}
	} while (false);


	if (!bBreak)
		m_pOwner->getGameSkillMgr().processNextCmd();
}

//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::useSkillAsk(CSceneUnit*	pOwner, const _stSkillModel* pSkillModel, uint32 uTarUnitID, _stPoint2F&stTargetPoint, uint8& uCode)
{
	if (!pOwner || !pSkillModel || !canUse(pOwner,pSkillModel,uCode) || !m_pOwner->getSceneCell())
		return false;

	//��Ŀ�겻�ż���
	if (pSkillModel->check_mark(SkillMark_NoTarNoSkill))
	{
		if (!uTarUnitID)
			return false;
	}

	uint32 uOldTarUnitID = uTarUnitID;
	float32 fNewDir = pOwner->getDirection();
	CSceneUnit* pTarget = m_pOwner->findCellObject(uTarUnitID);
	if (pTarget)
		fNewDir = get2PiRadian(pOwner->getPosition(),pTarget->getPosition());

	//����Ŀ��
	switch (pSkillModel->uDirStrategy)
	{
	case SkillDirStrategy_NoDir:			//�޷���ԭ��ʩ��
	case SkillDirStrategy_ButtonDir:		//���ܰ�ťָ������
	case SkillDirStrategy_PoleDir: 			//ҡ��ָ������
		{
			stTargetPoint = pOwner->getPosition();
			uTarUnitID = 0;
		}
		break;
	case SkillDirStrategy_TarDir:			//��Ŀ��ȷ������
	case SkillDirStrategy_ApoointTargetDir:
		{
			stTargetPoint = pOwner->getPosition();
			pOwner->setDirection(fNewDir);
			if (!pSkillModel->check_mark(SkillMark_Charge) && !pSkillModel->check_mark(SkillMark_Sing))
			{
				uTarUnitID = 0;
			}
		}
		break;
	case SkillDirStrategy_ReleasePos:		//���ܰ�ťָ�����λ��
		{
			stTargetPoint = pOwner->getPosition();
			uTarUnitID = 0;
		}
		break;
	case SkillDirStrategy_ButtonPos: 		//���ܰ�ťָ��λ��
	case SkillDirStrategy_PoleReleasePos:	//ҡ��ָ�����λ�ã�������
	case SkillDirStrategy_DevelopPos:		//��������ָ��λ��
		{
			uTarUnitID = 0;
		}
		break;
	case SkillDirStrategy_TargetPos:		//��Ŀ��ȷ��λ�ã����û��Ŀ��ȡ��̾��룩
	case SkillDirStrategy_ApoointTargetPos:
		{
			//������
			_stPoint2F stMaxTargetPoint(stTargetPoint);
			if (pOwner->getSceneCell())
				pOwner->getSceneCell()->getTargetPoint(m_pOwner->getPosition(), m_pOwner->getDirection(), (float32)pSkillModel->uReleaseRange, stMaxTargetPoint);

			stTargetPoint = pTarget ? pTarget->getPosition() : pOwner->getPosition();
 			if (pTarget)
 			{
 				stTargetPoint = pTarget->getPosition();
				if (pOwner->getSceneCell())
					pOwner->getSceneCell()->getTargetPoint(m_pOwner->getPosition(),pTarget->getPosition(),stTargetPoint);
 			}
 			else
			{
				stTargetPoint = stMaxTargetPoint;
			}
		}
		break;
	case SkillDirStrategy_Target:
		{
			//Ŀ�겻���� ����
			if (!pTarget)
				return false;

			stTargetPoint = pTarget ? pTarget->getPosition() : pOwner->getPosition();
			stTargetPoint = pTarget->getPosition();
			float32 fNewDir = ::get2PiRadian(m_pOwner->getPosition(),pTarget->getPosition());
			m_pOwner->setDirection(fNewDir);
		}
		break;
	default:
		break;
	}

	//��ײ����Ŀ���
	if (pSkillModel->findParam(SkillSpareParamType_Dash))
	{
		stTargetPoint = pOwner->getPosition();
		//������
		if (pOwner->getSceneCell())
			pOwner->getSceneCell()->getTargetPoint(m_pOwner->getPosition(), m_pOwner->getDirection(), (float32)pSkillModel->uReleaseRange, stTargetPoint);
	}

	//ʹ���Ӽ���λ�� ��Ҫ����Ŀ���
	if (pSkillModel->findParam(SkillSpareParamType_InheritTargetPos))
	{
		if (1 == pSkillModel->stCompositeData.uAtkPart)
		{
			m_stOldPosition = pOwner->getPosition();
		}
		else
		{
			stTargetPoint = m_stOldPosition;
			m_stOldPosition.zero();
		}
	}

	//��������Ŀ���
	if (pSkillModel->check_mark(SkillMark_SingLockTarget))
	{
		uTarUnitID = uOldTarUnitID;
	}

	return useSkillInnerAsk(pOwner,pSkillModel,uTarUnitID,stTargetPoint,uCode);
}
//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::useSkillInnerAsk(CSceneUnit* pOwner, const _stSkillModel* pSkillModel, uint32 uTarUnitID, _stPoint2F&stTargetPoint, uint8& uCode)
{
	if (pSkillModel->check_mark(SkillMark_ChargeEnableMove)
		|| pSkillModel->check_mark(SkillMark_SkillEnableMove)
		|| pSkillModel->findParam(SkillSpareParamType_Dash)
		|| pOwner->asMonster() != NULL || pOwner->asTrap() != NULL)
	{
	}
	else
	{
		//�����ƶ��ͷŵļ���,��̨Ҫ����ͣס.
		pOwner->endMove();
	}

	//�������޶�������ֱ���ͷ�
	bool bSinging = pSkillModel->check_mark(SkillMark_Charge) || pSkillModel->check_mark(SkillMark_Sing);
	if (!bSinging && SkillBehavior_NoAnim == pSkillModel->uBehavior)
	{
		setTargetPoint(stTargetPoint);
		bool bRet = CSkilling::useNoAnimSkill(m_pOwner,pSkillModel,uTarUnitID,stTargetPoint);
		if (bRet)
		{
			if (pSkillModel->uCoolTicket)
				m_pOwner->getCooling().addCooling(CoolingType_Skill,pSkillModel->uSkillID,pSkillModel->uCoolTicket);
		}
		return bRet;
	}

	initialize(pOwner,pSkillModel);

	//����
	if (pOwner->asPlayer())
	{
		//uint8 uCode = 0;
		pOwner->asPlayer()->mount(false);
	}

	//������������ ����������
	if (bSinging && pOwner->getSinging().getSingState() == SingState_Null)
		return singing(uTarUnitID,stTargetPoint,uCode);

	//�ж����������Ƿ��Ѿ����
	if (bSinging && (pOwner->getSinging().getSingState() != SingState_End))
	{
		uCode = R_ResultSkill_Singing;
		return false;
	}

	//����
	if (!useConsume(false,uCode))
		return false;

	setTargetPoint(stTargetPoint);
	return readyUseSkill(uTarUnitID,uCode);
}

//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::breakSkillAsk(uint32 uSkillID,bool bForce,uint8& uCode)
{
	if (!canOperate() /*|| m_pSkillModel->uSkillID != uSkillID*/)
		return false;

	//�����Ƿ��ܱ����
	if (!bForce && m_pSkillModel->check_mark(SkillMark_NoBeBreak))
		return false;

	//������
	if (SingState_Ing == m_pOwner->getSinging().getSingState())
	{
		m_pOwner->getSinging().breakSinging(true);
	}
	else
	{
		if (!m_pOwner->haveState(StateType_Skiling))
			return false;

		//��Ϲ㲥
		CGameSkillMgr::send_SkillBreak_Ack(m_pSkillModel->uSkillID,m_uCurStage,R_ResultSkill_Succeed,true,m_pOwner);

		onDoneSkill(true);
	
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::useChargeAsk(uint32 uSkillID,uint8& uCode)
{
	if (!canOperate() || m_pSkillModel->uSkillID != uSkillID || !m_pSkillModel->check_mark(SkillMark_Charge))
		return false;

	//�Ƿ���������
	if (SingState_Ing != m_pOwner->getSinging().getSingState())
		return false;

	//�������
	m_pOwner->getSinging().doneSinging();
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::canSinging(uint32 uTarUnitID,uint8& uCode)
{
	if (!m_pSkillModel)
		return false;

	//�жϼ���
	if (SkillType_Active == m_pSkillModel->uType || SkillType_MonsterCollletion == m_pSkillModel->uType)
	{
		CSceneUnit* pTarget = m_pOwner->findCellObject(uTarUnitID);
		if (!pTarget)
			return false;

		CSceneMonster* pMonster = pTarget->asMonster();
		if (!pMonster)
			return false;

		//�����ڼ��� �����ü���
		if (pMonster->getActivePlayer())
		{
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::singing(uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode)
{
	if (!canOperate())
		return false;

	if (!canSinging(uTarUnitID,uCode))
	{
		return false;
	}
	//�������
	if (!useConsume(true,uCode))
		return true;

	//��ʼ����
	uint32 uTime = 0;
	if (m_pSkillModel->check_mark(SkillMark_Charge))
	{
		uTime = getSkillTimeByType(m_pSkillModel,SkillTimeType_Charge);
	}
	else
	{
		uTime = getSkillTimeByType(m_pSkillModel,SkillTimeType_Continue);
	}
	this->initSingObject(m_pSkillModel->uSkillID,m_pSkillModel->uLevel,uTarUnitID,uTime,&stTargetPoint);
	m_pOwner->getSinging().beginSinging(this);
	return true;
}

//-------------------------------------------------------------
//------------------------------ ʱ�俪ʼ
void CSkilling::beginTime()
{
	if (!canOperate() || !m_pSkillAnimModel || m_pSkillModel == NULL || m_pOwner == NULL)
		return;

	m_uStartAnimTick = getSystemTick64();
	//��Ҫ�����������
	uint32 uMaxTime = MAX(m_pSkillModel->uContinueTime,m_pSkillAnimModel->uTotalTick);
	m_uEndAnimTick	= m_uStartAnimTick + uMaxTime;
}

//-------------------------------------------------------------
//------------------------------ ׼��ʩ��
bool CSkilling::readyUseSkill(uint32 uTarUnitID,uint8& uCode)
{
	if (!canOperate())
		return false;

	m_uTarUnitID = uTarUnitID;
	_stPoint2F stSkillStart(m_pOwner->getPosition());

	//���⼼������Ŀ�� ���ķ���
	if (m_pSkillModel->check_mark(SkillMark_SingLockTarget))
	{
		CSceneUnit* pTargetUnit = m_pOwner->findCellObject(uTarUnitID);
		if (pTargetUnit)
		{
			float32 fNewDir = ::get2PiRadian(m_pOwner->getPosition(),pTargetUnit->getPosition());
			m_pOwner->setDirection(fNewDir);
		}
	}

	//�ص�
	m_pOwner->callbackUseSkill(m_pSkillModel,uTarUnitID);

	//���÷����λ��
	if (m_pSkillModel->findParam(SkillSpareParamType_InheritTargetPos))
		m_pOwner->setPosition(getTargetPoint(),SetPositionReason_Skill,false);

	//�޶�������Ч��ֱ���˺�
	if (!m_pSkillModel->vecEffectModel.empty() && !m_pSkillModel->pAnimModel)
		return useNoAnimSkill(m_pOwner,m_pSkillModel,uTarUnitID,getTargetPoint());

	//����ֻ����һ��
	CHECKF(NULL == m_pSkillAnimModel);

	//������Ϊ
	m_pOwner->addState(StateType_Skiling);
	m_pSkillAnimModel = m_pSkillModel->pAnimModel;
	m_uCurStage++;

	//��״̬
	const _stSkillSpareParam* pSkillSpareParam = m_pSkillModel->findParam(SkillSpareParamType_AddState);
	if (pSkillSpareParam && pSkillSpareParam->vecParams.size() > 0)
		m_pOwner->addState((_emGameStateType)(uint32)pSkillSpareParam->vecParams[0]);

	//��ʼʱ��
	beginTime();

	m_clSkillAnim.initialize(m_pOwner,m_uStartAnimTick,m_pSkillModel,m_pSkillAnimModel);
	m_clSkillAnim.start();
	//�㲥
	CGameSkillMgr::send_UseSkill_Ack(m_pSkillModel->uSkillID,m_pSkillModel->uLevel,m_uCurStage,uTarUnitID,getTargetPoint(),R_ResultSkill_Succeed,true,m_pOwner,&stSkillStart);
	
	//������ɿ�ʼCD (�ǳ�������) ��ϼ�����������ϼ��ܵĵ�һ������CD
	if ((!m_pOwner->check_mark(SceneUnitMark_IgnoreSkillCD) && !m_pSkillModel->check_mark(SkillMark_Continue) && !m_pSkillModel->stCompositeData.uType)
		|| (m_pSkillModel->stCompositeData.uType && !m_pSkillModel->stCompositeData.uNextSkillID))
	{
		uint32 uCoolTime = 0;
		uint32 uTmpSkillID = m_pSkillModel->uSkillID;
		uint32 uGroupCoolTicket = m_pSkillModel->uGroupCoolTicket;
		if (m_pSkillModel->stCompositeData.uType)
		{
			uTmpSkillID = m_pSkillModel->uCompositeBaseSkillID;
			const _stSkillModel* pCompoSkillModel =g_clSkillData.findSkill(m_pSkillModel->uCompositeBaseSkillID,m_pSkillModel->uLevel);
			if (pCompoSkillModel)
			{
				uCoolTime = getSkillTimeByType(pCompoSkillModel,SkillTimeType_CD);
				uGroupCoolTicket = pCompoSkillModel->uGroupCoolTicket;
			}
		}
		else
		{
			uCoolTime = getSkillTimeByType(m_pSkillModel,SkillTimeType_CD);
		}
		m_pOwner->getCooling().addCooling(CoolingType_Skill,uTmpSkillID,uCoolTime,SKILL_GOOLING_GROUP_ID,uGroupCoolTicket);
	}
	
// 	if (m_pOwner->asPlayer())
// 		CLog::error("��ʼʩ�� ��ǰʱ��:%lld,����ID:%d",m_uStartAnimTick,m_pSkillModel->uSkillID);
	m_pOwner->energyUseSkill(m_pSkillModel);
	return true;
}

//-------------------------------------------------------------
//------------------------------ �޶�������
bool	CSkilling::useNoAnimSkill(CSceneUnit* pOwner,const _stSkillModel* pSkillModel,uint32 uTarUnitID,_stPoint2F&stTargetPoint)
{
	if (!pOwner || !pSkillModel || SkillBehavior_HaveAnim == pSkillModel->uBehavior || pSkillModel->vecEffectModel.empty())
		return false;

	//�㲥
	uint8 uLevel = pOwner->getGameSkillMgr().getSkillLevel(pSkillModel->uSkillID);
	CGameSkillMgr::send_UseSkill_Ack(pSkillModel->uSkillID,uLevel,1,uTarUnitID,stTargetPoint,R_ResultSkill_Succeed,true,pOwner);
	CSceneUnit* pTarget = pOwner->findCellObject(uTarUnitID);
	for (uint32 i = 0; i < pSkillModel->vecEffectModel.size(); ++i)
	{
		const _stSkillEffectModel* pEffect = pSkillModel->vecEffectModel[i];
		if (!pEffect)
			return false;

		//�˺�
		if (!CSkillAnim::beginDamage(pOwner,pTarget,stTargetPoint,pSkillModel,pEffect))
			return false;
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ �ɼ�����
bool CSkilling::useColletionSkill(uint8& uCode)
{
	uCode = R_ResultSkill_Fail;
	CScenePlayer* pPlayer = m_pOwner->asPlayer();
	if (!pPlayer) 
		return false;

	CSceneUnit* pUnit = m_pOwner->findCellObject(getSingTriggerID());
	if (!pUnit) 
		return false;

	//CPlayerColletion *pPlayerColleton = pPlayer->getPlayerColletion();
	//if (pPlayerColleton)
	//{
	//	if(!pPlayerColleton->colletionMonster(pUnit, uCode))
	//	{
	//		return false;
	//	}
	//}

	return true;
}
//-------------------------------------------------------------
//------------------------------ ץ������
bool CSkilling::arrestSkill(uint8& uCode)
{
	uCode = R_ResultSkill_Fail;

	CGameSceneCell* pGameCell = m_pOwner->getSceneCell();
	if (!pGameCell)
		 return false;

	CSceneUnit* pTargeUnit = m_pOwner->findCellObject(getCurentTargetUnitId());
	if (!pTargeUnit || !pTargeUnit->asMonster() || pTargeUnit->asMonster()->getMonsterType() != MonsterType_Capture)
	{
		uCode = R_ResultSkill_ArrestNoPlayer;
		return false;
	}

	pGameCell->arrestmonster(pTargeUnit,m_pOwner->asPlayer());
	return true;
}
//-------------------------------------------------------------
//------------------------------ �����
bool CSkilling::activeSkill(uint8& uCode)
{
	uCode = R_ResultSkill_Fail;

	CGameSceneCell* pGameCell = m_pOwner->getSceneCell();
	if (!pGameCell)
		return false;

	CSceneUnit* pTargeUnit = m_pOwner->findCellObject(getSingTriggerID());
	CSceneMonster* pMonster = pTargeUnit ? pTargeUnit->asMonster() :NULL;
	if (!pMonster)
	{
		uCode = R_ResultSkill_NoActive;
		return false;
	}
	uint8 uType = pMonster->getMonsterType();
	if (uType != MonsterType_Active)
	{
		uCode = R_ResultSkill_NoActive;
		return false;
	}

	pMonster->setActivePlayer(NULL);
	return true;
}
//-------------------------------------------------------------
//------------------------------ �̿Ͱ�ɱ����
bool CSkilling::useRewardSkill(uint8& uCode)
{
	uCode = R_ResultSkill_Fail;

	CGameSceneCell* pGameCell = m_pOwner->getSceneCell();
	if (!pGameCell)
		return false;

	CSceneUnit* pTargeUnit = m_pOwner->findCellObject(getSingTriggerID());
	CSceneMonster* pMonster = pTargeUnit ? pTargeUnit->asMonster() :NULL;
	if (!pMonster)
	{
		uCode = R_ResultSkill_NoThug;
		return false;
	}
	if (pMonster->getMonsterType() != MonsterType_RewardThugElite)
	{
		uCode = R_ResultSkill_NoThug;
		return false;
	}

	//��ɱ
	pGameCell->thugmonster(pTargeUnit,m_pOwner->asPlayer());

	return true;
}
//-------------------------------------------------------------
//------------------------------ ɱ�˻ص�
void CSkilling::callbackKillTarget		(CSceneUnit* pTarget)
{
	if (!m_pSkillModel || !pTarget)
		return;

	//����������Ŀ�꼼�� ��Ҫ�ж�
	if (m_pSkillModel->check_mark(SkillMark_Continue) && m_pSkillModel->check_mark(SkillMark_SingLockTarget) )
	{
		uint8 uCode = 0;
		breakSkillAsk(m_pSkillModel->uSkillID,true,uCode);
	}
}
