/*----------------- skillAnim.cpp
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/1/27 19:58:49
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/skilling/skillAnim.h"
#include "basic/timeFunction.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/GameDefine.pb.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "cfg_data/trapData/trapData.h"
#include "gameScene/gameSceneCell.h"
#include "cfg_data/monsterData/monsterData.h"
#include "cfg_data/constantData/constantData.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CSkillAnim::CSkillAnim()
{
	initialize(NULL,0,NULL,NULL);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSkillAnim::initialize(CSceneUnit* pOwner,uint64 uStartTick,const _stSkillModel* pSkillModel,const _stSkillAnimModel* pSkillAnimModel)
{
	m_pOwner		= pOwner;
	m_uStartAnimTick= uStartTick;
	m_uEndAnimTick	= 0;
	if (pSkillModel)
		m_uEndAnimTick = uStartTick + pSkillModel->uContinueTime;
	m_stAttEffect.init();
	m_uCurEffect	= 0;
	m_pSkillModel = pSkillModel;
	m_pSkillAnimModel = pSkillAnimModel;
	m_bBreak = false;
}

//-------------------------------------------------------------
//------------------------------ 
void	CSkillAnim::start()
{
	if (!canOperate()  )
		return;

	if ( m_pSkillAnimModel->vecAttEffect.empty() )
	{
		end();
		return ;
	}

	m_uCurEffect++;
	if (m_uCurEffect <= m_pSkillAnimModel->vecAttEffect.size())
	{
		m_stAttEffect = m_pSkillAnimModel->vecAttEffect[m_uCurEffect - 1];
	}
	else
	{
		//����������Ҫ���²��Ŷ���
		if (m_pSkillModel->check_mark(SkillMark_Continue) && (!m_pSkillModel->uContinueTime || m_uEndAnimTick >= getSystemTick64()))
		{
			m_uCurEffect = 1;//ֱ��ִ�е�һ�׶�
			m_stAttEffect = m_pSkillAnimModel->vecAttEffect[0];
			m_uStartAnimTick += m_pSkillAnimModel->uTotalTick;
			//�㲥
			uint8 uLevel = m_pOwner->getGameSkillMgr().getSkillLevel(m_pSkillModel->uSkillID);
			CGameSkillMgr::send_UseSkill_Ack(m_pSkillModel->uSkillID,uLevel,1,0,m_pOwner->getSkilling().getTargetPoint(),R_ResultSkill_Succeed,true,m_pOwner);
		}
		else
		{
			end();
		}
	}
}

//-------------------------------------------------------------
//------------------------------ ����
void	CSkillAnim::end()
{
	m_uCurEffect = 0;
	m_uStartAnimTick = 0;
	m_stAttEffect.init();
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::process		(uint64 uTick)
{
	processAttack(uTick);
	return true;
}

//-------------------------------------------------------------
//------------------------------ ����֡�ƽ�
bool	CSkillAnim::processAttack	(uint64 uTick)
{
	if (!canOperate() || !m_stAttEffect.pEffectModel || !m_pOwner)
		return false;

	if (!m_uStartAnimTick || m_uStartAnimTick > uTick || m_uStartAnimTick +  m_stAttEffect.uTick > uTick)
		return true;

	//if (m_pOwner->asPlayer())
		//CLog::error("����֡:%d,��ʼʱ��:%lld,��ǰʱ��:%lld,����ID:%d,Ч��ID:%d",m_uCurEffect,m_uStartAnimTick,uTick,m_pSkillModel->uSkillID,m_stAttEffect.pEffectModel->uEffectID);

	//��Ҫȥ�����µ�����
	_stPoint2F target = m_pOwner->getSkilling().getTargetPoint();
	//λ��
	{
		//���õ�λ��
		if (!isFloatEqual(m_stAttEffect.fDistance,0.0f))
			m_pOwner->getUnitTargetPoint(m_stAttEffect.fDir + m_pOwner->getDirection(),m_stAttEffect.fDistance,target);

		//��ײ��λ��
		const _stSkillSpareParam* pParam = m_pSkillModel->findParam(SkillSpareParamType_Dash);
		if (pParam && pParam->vecParams.size() > 0 && m_pOwner->getPosition() != m_pOwner->getSkilling().getTargetPoint())
		{
			uint32 uLastTick = m_uCurEffect > 1 ? m_pSkillAnimModel->vecAttEffect[m_uCurEffect -2].uTick : 0;
			float32 fTmpDis = (m_stAttEffect.uTick - uLastTick) * pParam->vecParams[0] / 1000.0f;
			float32 fMaxDis = getDistance(m_pOwner->getPosition(),m_pOwner->getSkilling().getTargetPoint());
			if (fTmpDis < fMaxDis)
			{
				m_pOwner->getUnitTargetPoint(m_pOwner->getDirection(),fTmpDis,target);
			}
			else
			{
				target = m_pOwner->getSkilling().getTargetPoint();

				//����Զ���뼼��ֹͣ
				m_bBreak = true;
			}
		}
	}

	//�ƶ��в�����λ�� �м��ܱ��ƶ����ͷ�
	if(!m_pOwner->isMoving() && m_pSkillModel->uDirStrategy !=SkillDirStrategy_Target)
	{
		m_pOwner->setPosition(target,SetPositionReason_Skill,false);
	}
	//if (m_pOwner->asPlayer())
		//CLog::error("processAttack position(%f,%f),dir(%f)",m_pOwner->getPosition().x,m_pOwner->getPosition().y,m_pOwner->getDirection());

	float32 fMoveDis = getDistance(target,m_pOwner->getPosition());
	if (!m_stAttEffect.uUseRange)
		fMoveDis = 0;

	//ʩ��λ�� Ĭ��ȡ�ͻ��˷���Ŀ��λ�� ��ײĬ��ȡ����λ��
	_stPoint2F skillTarget = m_pOwner->getPosition();

	//�˺�
	CSceneUnit* pTarget = m_pOwner->findCellObject(m_pOwner->getSkilling().getCurentTargetUnitId());
	if (pTarget)
		skillTarget = pTarget->getPosition();

	beginDamage(m_pOwner,pTarget,skillTarget,m_pSkillModel,m_stAttEffect.pEffectModel,fMoveDis);

	//������һ֡
	start();
	return true;
}

//-------------------------------------------------------------
//------------------------------ �ܹ�ѡȡĿ��
bool	CSkillAnim::canChooseTo	(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stSkillModel* pModel)
{
	if (!pOwner || !pTarget || !pModel)
		return false;

	if (!pOwner->canUseSkill(pTarget))
		return false;

	if (!pTarget->canByUseSkill(pOwner) || pTarget->isDead())
		return false;

	if(pModel->uTagetCategory != TargetCategory_All)
	{
		uint8 uRelation = pOwner->getRelation(pTarget);
		if (TargetCategory_FriendAndSelf == pModel->uTagetCategory)
		{
			if (uRelation != TargetCategory_Self && uRelation != TargetCategory_Friend )
			{
				return false;
			}
		}
		else if (uRelation != pModel->uTagetCategory)
		{
			return false;
		}
	}

	CSceneUnit* pTargetOwner = pTarget->getOwner();
	if ( pTargetOwner )
	{
		//-todo
		CScenePlayer* pTargetPlayer = pTargetOwner->asPlayer();
		if (pTargetPlayer&&pTargetPlayer->testRegion(RegionMark_PlayerCntBeHarm))
			return true;
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::gatherObject(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,MAP_GAME_UNIT&mapTarget,float32 fMoveDis)
{
	if (!pOwner || !pSkillEffectModel || !pSkillModel || !pSkillEffectModel->uMaxAffectNum)
		return false;

	CGameScene*pScene = pOwner->getSceneMap();
	if(!pScene)
		return false;

	//���б�����
	static VECTOR_RANGE_UNIT vecRangeUnit;
	vecRangeUnit.clear();

	if(!CSkillAnim::gatherObject(pOwner,stTargetPoint,pSkillModel,pSkillEffectModel,vecRangeUnit,fMoveDis))
		return false;

	static VECTOR_RANGE_UNIT vecEffectUnit;
	vecEffectUnit.clear();
	for (uint32 i = 0;i < vecRangeUnit.size();++i)
	{
		_stRangeUnit&stUnit = vecRangeUnit[i];
		//�ͻ��˸���Ŀ��һ��Ҫ��
		if(stUnit.pUnit && canChooseTo(pOwner,stUnit.pUnit,pSkillModel))
		{
			if (stUnit.pUnit == pTarget)
			{
				mapTarget.insert_(pTarget->getUnitID(),pTarget);
			}
			else
			{
				vecEffectUnit.push_back(stUnit);
			}
		}
	}

	//ѡ��Ŀ��
	switch (pSkillEffectModel->uTargetSelectType)
	{
	case TargetSelectType_Near:
		{
			for (uint32 i = 0; i < vecEffectUnit.size(); ++i)
			{
				if (vecEffectUnit[i].pUnit && mapTarget.size() < pSkillEffectModel->uMaxAffectNum)
					mapTarget.insert_(vecEffectUnit[i].pUnit->getUnitID(),vecEffectUnit[i].pUnit);
			}
		}
		break;
	case TargetSelectType_Rand:
		{
			VECTOR_RANGE_UNIT vecOut;
			getRandVecDiff(vecEffectUnit,vecOut,pSkillEffectModel->uMaxAffectNum);
			for (uint32 i = 0; i < vecOut.size(); ++i)
			{
				if (vecOut[i].pUnit && mapTarget.size() < pSkillEffectModel->uMaxAffectNum)
					mapTarget.insert_(vecOut[i].pUnit->getUnitID(),vecOut[i].pUnit);
			}
		}
		break;
	case TargetSelectType_Long:
		{
			for (int32 i = (int32)vecEffectUnit.size() - 1; i >= 0 ; --i)
			{
				if (vecEffectUnit[i].pUnit && mapTarget.size() < pSkillEffectModel->uMaxAffectNum)
					mapTarget.insert_(vecEffectUnit[i].pUnit->getUnitID(),vecEffectUnit[i].pUnit);
			}
		}
		break;
	default:
		break;
	}


	//����Ŀ��
	if (pSkillModel->check_mark(SkillMark_TargetOnce))
	{
		MAP_GAME_UNIT::iterator _pos = mapTarget.begin();
		for (; _pos != mapTarget.end();)
		{
			if (pOwner->getSkilling().haveChoose(_pos->first))
			{
				mapTarget.erase(_pos++);
			}
			else
			{
				 pOwner->getSkilling().addChooseTarget(_pos->first);
				 ++_pos;
			}
		}
	}

	//����ѡĿ��
	MAP_GAME_UNIT::iterator _pos = mapTarget.begin();
	for (; _pos != mapTarget.end();_pos++)
	{
		pOwner->getSkilling().addGatherTarget(_pos->first);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::gatherObject	(CSceneUnit* pOwner,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,VECTOR_RANGE_UNIT&vecRangeUnit,float32 fMoveDis)
{
	if(!pSkillModel || !pSkillEffectModel)
		return false;

	CGameSceneCell* pScene = pOwner->getSceneCell();
	if(!pScene)
		return false;

	//���ҹ�����Χ�ڵ����ж���
	static MAP_GAME_UNIT	mapObject;
	static MAP_GAME_UNIT	mapUseObject;
	static VECTOR_GAME_UNIT vecObject;
	mapObject.clear();
	vecObject.clear();
	mapUseObject.clear();

	float fRadius = 0;
	_stSkillEffectModel::MapSkillRangeData::const_iterator _pos = pSkillEffectModel->mapRangeData.begin();
	for (; _pos != pSkillEffectModel->mapRangeData.end(); ++_pos)
	{
		const _stSkillRangeData& stData = _pos->second;
		fRadius = MAX(fRadius,stData.fLength + stData.fOffset);
		if (stData.uRangeType == RangeType_Rect)
			fRadius = MAX(fRadius,stData.fWidth);
	}
	fRadius = MAX(fRadius,fMoveDis) + pScene->getMaxModelRadius();
	//�Ѽ�����Բ��ΧĿ��
	pOwner->gatherSceneObject(stTargetPoint,fRadius,mapObject,g_uBattleObject);

	//�Ѽ�����ģ��Բ�ķ�Χ
	pOwner->gatherObjectRound(stTargetPoint,pOwner->getModelRadius(),mapObject,mapUseObject);

	//�Ѽ�λ�ƾ��εķ�Χ �����෴
	if (fMoveDis)
		pOwner->gatherObjectLine(stTargetPoint,pOwner->getDirection() + M_PI_F,fMoveDis,pOwner->getModelRadius(),mapObject,mapUseObject);

	_pos = pSkillEffectModel->mapRangeData.begin();
	for (; _pos != pSkillEffectModel->mapRangeData.end(); ++_pos)
	{
		const _stSkillRangeData& stData = _pos->second;
		float32 fOffsetDir = pOwner->getDirection() + getAngleToRadian(stData.fOffsetAngle);
		//����ƫ��Ŀ�� ���ױ���
		_stPoint2F stPoint;
		getTargetPoint(stTargetPoint,fOffsetDir,stData.fOffset,stPoint);
		//�� ����
		if(stData.uRangeType == RangeType_Rect)
			pOwner->gatherObjectLine(stPoint,fOffsetDir,stData.fLength,stData.fWidth,mapObject,mapUseObject);
		//Բ ����
		if(stData.uRangeType == RangeType_Round)
			pOwner->gatherObjectArc(stPoint,fOffsetDir,stData.fLength,stData.fWidth,mapObject,mapUseObject);
		//Բ ����
		if(stData.uRangeType == RangeType_Ring)
			pOwner->gatherObjectRound(stPoint,stData.fLength,mapObject,mapUseObject);
	}

	MAP_GAME_UNIT::iterator iterUseObject = mapUseObject.begin();
	for (; iterUseObject != mapUseObject.end(); ++iterUseObject)
	{
		CSceneUnit* pUnit = iterUseObject->second;
		if(!pUnit)
			continue;
		
		//�⻷
		const _stSkillRangeData* pRangeData = pSkillEffectModel->mapRangeData.find_(RangeType_Ring);
		float32 fDis = getDistance(stTargetPoint,pUnit->getPosition()) - pUnit->getSkillModelRadius();
		if (fDis < 0.0f)
			fDis = fDis * (-1);
		if (pRangeData && pRangeData->fWidth && fDis >= pRangeData->fWidth)
			pUnit->update_mark(SceneUnitMark_OutSideDamage);

		//�������λ���ж�
		if (!pUnit->haveState(StateType_Plane))
		{
			vecRangeUnit.push_back(_stRangeUnit(pUnit,fDis));
		}
	}

	//����
	if(!vecRangeUnit.empty())
		qsort(&vecRangeUnit[0],vecRangeUnit.size(),sizeof(_stRangeUnit),CGhostObject::_rangeUnit_distance);

	return true;
}
//-------------------------------------------------------------
//------------------------------ ��ʱ����
bool	CSkillAnim::delayDamage	(CSceneUnit* pOwner,CSceneUnit* pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if (!pOwner || !pTarget || !pSkillModel || !pSkillEffectModel)
		return false;

	MAP_GAME_UNIT mapTarget;
	mapTarget.insert_(pTarget->getUnitID(),pTarget);
	return realDamage(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);
}
//-------------------------------------------------------------
//------------------------------ ��ʼ����
bool	CSkillAnim::beginDamage(CSceneUnit* pOwner,CSceneUnit* pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,float32 fMoveDis/* = 0*/)
{
	if (!pOwner || !pSkillEffectModel || !pSkillModel)
		return false;

	MAP_GAME_UNIT mapTarget;
	if (!CSkillAnim::gatherObject(pOwner,pTarget,stTargetPoint,pSkillModel,pSkillEffectModel,mapTarget,fMoveDis))
		return false;

	//��Ŀ���жϼ���
	if (pSkillModel->check_mark(SkillMark_NoTargeStop) && mapTarget.empty())
	{
		pOwner->breakUseSkill();
		return true;
	}
/* 	if (pOwner->asPlayer())
 		CLog::error("beginDamage ��ǰλ��(%f,%f),Ŀ��λ��(%f,%f),Ŀ�����(%d),Ч��ID(%d)",pOwner->getPosition().x,pOwner->getPosition().y,\
 					stTargetPoint.x,stTargetPoint.y,mapTarget.size(),pSkillEffectModel->uEffectID);*/

	//����ʱ����
	const _stSkillSpareParam* pSpareParam = pSkillModel->findParam(SkillSpareParamType_DelaySkillEffect);
	if (pSpareParam && pSpareParam->vecParams.size() > 0)
	{
		float32 fSpeed = pSpareParam->vecParams[0];
		if (isFloatEqual(fSpeed,0.0f))
			return false;

		//��Ŀ�� Ҫ���Լ���buff
		if (mapTarget.empty())
			return CSkillAnim::skillAllEffect(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);

		uint16 uTargetPos = 0;//Ŀ���˳��
		MAP_GAME_UNIT::iterator _pos = mapTarget.begin();
		MAP_GAME_UNIT::iterator _end = mapTarget.end();
		for (; _pos != _end; ++_pos)
		{
			CSceneUnit* pTarget = _pos->second;
			if (!pTarget)
				continue;

			uTargetPos++;
			float32 fDis = getDistance(pOwner->getPosition(),pTarget->getPosition());
			uint32 uTick = (uint32)(fDis * 1000 / fSpeed);
			pTarget->byAttack(pOwner,stTargetPoint,pSkillModel,pSkillEffectModel,uTick);
		}	
		//��ײ����λ��
		if (pSkillModel->findParam(SkillSpareParamType_Dash))
		{
			//��Ҫʵʱλ��
			_stPoint2F stP = pOwner->getSkilling().getTargetPoint();
			pOwner->setPosition(stP,SetPositionReason_Skill, false);
		}

		return true;
	}

	//�����˺�
	return realDamage(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);
}
//-------------------------------------------------------------
//------------------------------ �����˺�
bool	CSkillAnim::realDamage	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapTarget,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if (!pOwner || !pSkillEffectModel || !pSkillModel)
		return false;

	
	uint8 uHitNum = 0;//������
	uint16 uTargetPos = 0;//Ŀ���˳��
	MAP_GAME_UNIT::iterator _pos =  mapTarget.begin();
	for (; _pos != mapTarget.end();)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
		{
			++_pos;
			continue;
		}
		uTargetPos++;
		pUnit->delete_mark(SceneUnitMark_SkillMiss);
		if (pOwner->isAttackHit(pUnit,pSkillModel,pSkillEffectModel))
		{
			uHitNum++;
			 ++_pos;
			 pUnit->callbackBeSkillEffect(pOwner,pSkillModel,pSkillEffectModel);		
			 pOwner->callbackSkillHitTarget(pUnit,pSkillModel,pSkillEffectModel);		

		}
		else
		{//����֪ͨ�ͻ���
			pUnit->update_mark(SceneUnitMark_SkillMiss);
			CGameSkillMgr::send_AttackNotice_Ack(pOwner,pUnit,SkillNoticeType_Miss,false);
			mapTarget.erase(_pos++);
		}
	}

	//���ез��ص�
	if (uHitNum > 0)
		pOwner->callbackSkillHit(pSkillModel,pSkillEffectModel);
	else
		pOwner->callbackSkillNotHit(pSkillModel,pSkillEffectModel);

	//�˺�Ч��
	CSkillAnim::skillAllEffect(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);

	//�����˺����
	skillDamageOver(pOwner,mapTarget,pSkillModel,pSkillEffectModel);
	return true;
}
//-------------------------------------------------------------
//------------------------------ ����Ч��
bool CSkillAnim::skillAllEffect(CSceneUnit* pOwner,MAP_GAME_UNIT&mapTarget,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel)
		return false;

	for (uint8 i = 0; i <pSkillEffectModel->vecEffectType.size(); ++i)
	{
		const _stSkillEffectType& stSkillEffectType = pSkillEffectModel->vecEffectType[i];
		if (SkillEffectType_Damage == stSkillEffectType.uEffectType)//���˺�
		{
			skillAttack(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);
		}
		else if (SkillEffectType_Cure == stSkillEffectType.uEffectType)//����
		{
			skillCure(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);
		}
		else if (SkillEffectType_AddBuff == stSkillEffectType.uEffectType)//��buff
		{
			skillAddBuff(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_AddExpend == stSkillEffectType.uEffectType)//��������ֵ
		{
			skillAddExpend(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_AddObject == stSkillEffectType.uEffectType)//����
		{
			skillAddSceneObject(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);
		}
		else if (SkillEffectType_ClearUsefulBuff == stSkillEffectType.uEffectType)//�������buff
		{
			skillClearBuff(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_ClearHarmfulBuff == stSkillEffectType.uEffectType)//�������buff
		{
			skillClearBuff(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_ClearSkillCD == stSkillEffectType.uEffectType)//���CD
		{
			pOwner->getGameSkillMgr().clearSkillCD(pSkillModel->uSkillID);
		}
		else if (SkillEffectType_Move == stSkillEffectType.uEffectType)//����λ��
		{
			skillAddMove(pOwner,stTargetPoint,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_AddSkill == stSkillEffectType.uEffectType)//���Ӽ���
		{
			skillUseSkill(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_OutRingAddBuff == stSkillEffectType.uEffectType)//���Ӽ���
		{
			skillOutRingAddBuff(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_ResetBuffTime == stSkillEffectType.uEffectType)//���Ӽ���
		{
			skillResetBuffTime(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ ����buff
bool	CSkillAnim::skillAddBuff(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel || SkillEffectType_AddBuff != stSkillEffectType.uEffectType)
		return false;

	const _stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map* pSumap = pSkillEffectModel->dmapAddBuff.find_(stSkillEffectType.uEffectTargetType);
	if (!pSumap)
		return false;

	//���Լ��Ӳ��������е�����
	if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType && !stSkillEffectType.bSkillHit)
	{
		_stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map::const_iterator _pos = pSumap->begin();
		for (; _pos != pSumap->end(); ++_pos)
		{
			const _stSkillAddBuff& stAddBuff = _pos->second;
			if (!stAddBuff.uExpendType || pOwner->addExpend(stAddBuff.uExpendType,stAddBuff.iValue, DoingType_Skill))
				pOwner->getGameBuffMgr().addBuff(stAddBuff.uBuffID,pSkillModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
		}
		
		return true;
	}

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	uint8 uHitNum = 0;
	for (; _pos != mapObject.end(); ++_pos)
	{
		//CLog::warn("----has target skillAddBuff target  is ");

		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;

		if (stSkillEffectType.bSkillHit && pUnit->check_mark(SceneUnitMark_SkillMiss))
			continue;

		uHitNum++;
		//Ŀ��ֱ�Ӽ�
		if (EffectTargetType_Other == stSkillEffectType.uEffectTargetType)
		{
			_stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map::const_iterator _pos = pSumap->begin();
			for (; _pos != pSumap->end(); ++_pos)
			{
				const _stSkillAddBuff& stAddBuff = _pos->second;
				if (!stAddBuff.uExpendType || pOwner->addExpend(stAddBuff.uExpendType,stAddBuff.iValue, DoingType_Skill))
				{
					pUnit->getGameBuffMgr().addBuff(stAddBuff.uBuffID,pSkillModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
					//CLog::warn("skillAddBuff target %d add buff %d ", pUnit->getUnitID(), stAddBuff.uBuffID);

				}
			}
		}
		else if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType)
		{
			//���� ֻ��һ�ε�����
			if (stSkillEffectType.bAddOneCount && uHitNum == 1)
			{
				_stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map::const_iterator _pos = pSumap->begin();
				for (; _pos != pSumap->end(); ++_pos)
				{
					const _stSkillAddBuff& stAddBuff = _pos->second;
					if (!stAddBuff.uExpendType || pOwner->addExpend(stAddBuff.uExpendType,stAddBuff.iValue, DoingType_Skill))
						pOwner->getGameBuffMgr().addBuff(stAddBuff.uBuffID,pSkillModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
				}
				return	true;
			}

			_stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map::const_iterator _pos = pSumap->begin();
			for (; _pos != pSumap->end(); ++_pos)
			{
				const _stSkillAddBuff& stAddBuff = _pos->second;
				if (!stAddBuff.uExpendType || pOwner->addExpend(stAddBuff.uExpendType,stAddBuff.iValue, DoingType_Skill))
					pOwner->getGameBuffMgr().addBuff(stAddBuff.uBuffID,pSkillModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
			}
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::skillAddExpend	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| mapObject.empty()|| SkillEffectType_AddExpend != stSkillEffectType.uEffectType)
		return false;

	//���Լ��Ӳ��������е�����
	if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType && !stSkillEffectType.bSkillHit)
		return pOwner->addExpend(pSkillEffectModel->mapAddSelfExpend, DoingType_Skill);

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	uint8 uHitNum = 0;
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;

		if (stSkillEffectType.bSkillHit && pUnit->check_mark(SceneUnitMark_SkillMiss))
			continue;

		uHitNum++;
		//Ŀ��ֱ�Ӽ�
		if (EffectTargetType_Other == stSkillEffectType.uEffectTargetType)
		{
			pUnit->addExpend(pSkillEffectModel->mapAddExpend, DoingType_Skill);
		}
		else if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType)
		{
			//���� ֻ��һ�ε�����
			if (stSkillEffectType.bAddOneCount && uHitNum == 1)
				return	pOwner->addExpend(pSkillEffectModel->mapAddSelfExpend, DoingType_Skill);
			
			pOwner->addExpend(pSkillEffectModel->mapAddSelfExpend, DoingType_Skill);
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ ���buff
bool	CSkillAnim::skillClearBuff	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| mapObject.empty())
		return false;

	_emEffectType emEffectType;
	if (SkillEffectType_ClearHarmfulBuff == stSkillEffectType.uEffectType)
	{
		emEffectType = EffectType_Harmful;
	}
	else
	{
		emEffectType = EffectType_Useful;
	}
	//Ŀ�����Լ���ֻ���������buff
	if (SkillEffectType_ClearHarmfulBuff == stSkillEffectType.uEffectType && EffectTargetType_Self == stSkillEffectType.uEffectTargetType)
		return pOwner->getGameBuffMgr().clearBuff(emEffectType, DoingType_Skill);

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;

		if (stSkillEffectType.bSkillHit && pUnit->check_mark(SceneUnitMark_SkillMiss))
			continue;

		//Ŀ��ֱ�Ӽ�
		if (EffectTargetType_Other == stSkillEffectType.uEffectTargetType)
			pUnit->getGameBuffMgr().clearBuff(emEffectType, DoingType_Skill);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ ����λ��
bool	CSkillAnim::skillAddMove	(CSceneUnit* pOwner,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel || SkillEffectType_Move != stSkillEffectType.uEffectType)
		return false;

	_stPoint2F stNewTarget = pOwner->getPosition();
	if (EffectAddMoveType_UseRelease == pSkillEffectModel->stAddMove.uAddMoveType)
	{
		pOwner->getUnitTargetPoint(pOwner->getDirection(),(float32)pSkillModel->uReleaseRange,stNewTarget);
	}
	else if (EffectAddMoveType_UseDistance == pSkillEffectModel->stAddMove.uAddMoveType)
	{
		pOwner->getUnitTargetPoint(pOwner->getDirection(),(float32)pSkillEffectModel->stAddMove.iDistance,stNewTarget);
		//ǿ���޸�Ŀ��λ��
		pOwner->getSkilling().setTargetPoint(stNewTarget);
	}

	pOwner->setPosition(stNewTarget,SetPositionReason_Skill,false);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::skillUseSkill(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| mapObject.empty()|| SkillEffectType_AddSkill != stSkillEffectType.uEffectType)
		return false;

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;

		if (stSkillEffectType.bSkillHit && pUnit->check_mark(SceneUnitMark_SkillMiss))
			continue;

		if (EffectTargetType_Other == stSkillEffectType.uEffectTargetType)
		{
			_stPoint2F stTargetPoint(pUnit->getPosition());
			pOwner->getGameSkillMgr().useSkill(pSkillEffectModel->uHitUseSkill,pSkillEffectModel->uLevel,pOwner->getDirection(),
				pUnit->getUnitID(),stTargetPoint);
		}
		else if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType)
		{
			_stPoint2F stTargetPoint(pOwner->getPosition());
			pOwner->getGameSkillMgr().useSkill(pSkillEffectModel->uHitUseSkill,pSkillEffectModel->uLevel,pOwner->getDirection(),
				pOwner->getUnitID(),stTargetPoint);
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::skillOutRingAddBuff(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| !pSkillEffectModel->uOutRingAddBuff || mapObject.empty()|| SkillEffectType_OutRingAddBuff != stSkillEffectType.uEffectType)
		return false;

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit || !pUnit->check_mark(SceneUnitMark_OutSideDamage))
			continue;

		if (stSkillEffectType.bSkillHit && pUnit->check_mark(SceneUnitMark_SkillMiss))
			continue;

		if (EffectTargetType_Other == stSkillEffectType.uEffectTargetType)
		{
			pUnit->getGameBuffMgr().addBuff(pSkillEffectModel->uOutRingAddBuff,pSkillEffectModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
		}
		else if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType)
		{
			pOwner->getGameBuffMgr().addBuff(pSkillEffectModel->uOutRingAddBuff,pSkillEffectModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ ����buffʱ��
bool	CSkillAnim::skillResetBuffTime(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel || mapObject.empty()|| SkillEffectType_ResetBuffTime != stSkillEffectType.uEffectType)
		return false;

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit )
			continue;
		pUnit->getGameBuffMgr().resetBuffTime(pOwner,pSkillEffectModel->uResetBuffID);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ �����������
bool	CSkillAnim::skillDamageOver(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel || mapObject.empty())
		return false;

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;
		pUnit->delete_mark(SceneUnitMark_OutSideDamage);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::skillCure	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| mapObject.empty())
		return false;

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;
		uint64 uAddHp = (uint64)pUnit->getHPMax() * pSkillEffectModel->uExtraCureRadio / 10000 + pSkillEffectModel->uExtraCure;

		//���Ƽ���
		uAddHp += (uint64)(uAddHp * pOwner->getGameBuffMgr().getAddCureRate());

		pUnit->updateHP((int32)uAddHp,pOwner, DoingType_Skill);

		pUnit->getHatredMgr().onByCure_Skill(pOwner,(int32)uAddHp,pSkillModel,pSkillEffectModel);
		CGameSkillMgr::send_AttackDamage_Ack(pOwner,pSkillModel->uSkillID,10,(int32)uAddHp,true,pUnit);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::skillAttack	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| mapObject.empty())
		return false;

	//�㲥�ܻ�����
	PBS2CSkillEffectBroad pbBroad;
	pbBroad.set_sourceid(pOwner->getUnitID());
	pbBroad.set_skillid(pSkillModel->uSkillID);

	//�����˺�
	MAP_GAME_UNIT::iterator _pos = mapObject.begin();
	MAP_GAME_UNIT::iterator _end = mapObject.end();
	uint8 uAttackNum = 1;
	for (;_pos != _end;++_pos)
	{
		CSceneUnit*pTarget = _pos->second;
		if (!pTarget)
			continue;

		if (pTarget->checkStateBuffAttr(StateBuffAttr_ForbidDamage))
			continue;

		attackDamage(pOwner,pTarget,stTargetPoint,pSkillModel,pSkillEffectModel,uAttackNum++,mapObject.size());

		//������ܱ�����
		//if (pTarget->asMonster())
		{
			PBS2CSkillEffect* pbEffect = pbBroad.add_skilleffect();
			if (pbEffect)
			{
				PBPoint* pbPos = pbEffect->mutable_newposion();
				if ( pbPos )
				{
					pbPos->set_posx(pTarget->getPosition().x);
					pbPos->set_posy(pTarget->getPosition().y);
				}
				pbEffect->set_targetid(pTarget->getUnitID());
			}
		}
	}
	
	if (pbBroad.skilleffect_size() >= 1 )
	{
		//���㲥���Լ�
		pOwner->sendBroadcast(P_S2C_Protocol_Skill,S2C_Skill_EffectBroad,&pbBroad,R_ResultSkill_Succeed,true);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ �����˺�
bool	CSkillAnim::attackDamage(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,uint8 uAttackNum,uint8 uTargetNum)
{
	if(!pOwner || !pTarget || !pSkillModel || !pSkillEffectModel || !uTargetNum)
		return false;
	
	//�����˺��⻷�˺�ֵ��һ�� ������buffֻ��һ��(������)
	uint32 uExtraDamageRadio = pSkillEffectModel->uExtraDamageRadio ;
	if (pTarget->check_mark(SceneUnitMark_OutSideDamage))
		uExtraDamageRadio = pSkillEffectModel->uParamDamageRadio;

	//MAX(((MAX((a����-b����)*(a���ܷŴ��+a�˺�����-b�˺�����)+a�����˺�,a����*0.1))*MAX(a������*bʧ����-b��������*aʧ����,1.1)+a�ػ�-b��)*(1-b����),1)
	float32 fDamageRadio = MAX(0.0f,(uExtraDamageRadio+ pOwner->getAttribute(BattleAttribute_DamageImprove) - pTarget->getAttribute(BattleAttribute_DamageRemit)) / FLOAT_MAX_PROBABILITY);
	int32 iDamage = MAX(0,(int32)((pOwner->getAttribute(BattleAttribute_Attack) - pTarget->getAttribute(BattleAttribute_Defense)) * fDamageRadio));
	iDamage = MAX(int32(iDamage + pSkillEffectModel->uExtraDamage),int32(pOwner->getAttribute(BattleAttribute_Attack) * 0.1));
	
	//��������
	if (pSkillModel->check_mark(SkillMark_Charge))
	{
		float32 fChargeRate = pOwner->getSkilling().getChargeRate();
		iDamage = int32(iDamage * fChargeRate);
	}

	//���㱩���� Ĭ������ʮ��
	uint32 uCritical = pOwner->getCritcal(pTarget,pSkillEffectModel,pSkillModel);
	uint32 uEndCritical = uint32((uint64)uCritical * pSkillEffectModel->uAverageDamageRadio / 10000);

	//����ƽ���˺�
	if(pSkillModel->check_mark(SkillMark_Average))
		uEndCritical = uEndCritical / uTargetNum;
	//�����������������˺�/�����˺�+�ػ�ֵ-�Է���ֵ
	iDamage = iDamage * uEndCritical / 10 + pOwner->getAttribute(BattleAttribute_Thump) -pTarget->getAttribute(BattleAttribute_Parry);
	iDamage = int32(iDamage * (1.0f- pTarget->getAttribute(BattleAttribute_Exempt) / FLOAT_MAX_PROBABILITY));
	iDamage = MAX(1,iDamage);

	//�����ܻ� �ް�����ܱ�����
	CSceneMonster* pTargetMonster = pTarget->asMonster();
	if (pTargetMonster && !pTargetMonster->getSuperArmor() && pTargetMonster->getMonsterModel() && pTargetMonster->getMonsterModel()->uBeAttackMoveTick)
	{
		//������Ӳֱ
		uint32 uBeAttackTick = pTargetMonster->getMonsterModel()->uBeAttackMoveTick;
		uint32 uBuffID = g_clConstantData.Skill_BeAttackAddBuff;
		pTarget->getGameBuffMgr().addBuff(uBuffID,pSkillEffectModel->uLevel,uBeAttackTick,pOwner->getUnitID(),pSkillModel->uSkillID,false, DoingType_Skill);

		//�����ܻ�λ��
		if (pSkillEffectModel->stBeAttackMove.uDirectionType)
		{
			pTarget->setNewPosition(pOwner,pSkillEffectModel->stBeAttackMove.fBeAttackMove,pSkillEffectModel->stBeAttackMove.uDirectionType);
		}
	}

	int32 iTotalDamage = iDamage;

	//�˺���������
	const _stSkillSpareParam* pSpareParam = pSkillModel->findParam(SkillSpareParamType_DamageUpByNum);
	if (pSpareParam && uAttackNum > 1 && pSpareParam->vecParams.size() > 0)
	{
		uint32 uValue = uint32((uAttackNum - 1) * pSpareParam->vecParams[0]);
		iTotalDamage += int32(iDamage * (uValue / FLOAT_MAX_PROBABILITY));
	}

	//buff�����˺�
	iTotalDamage += pTarget->getGameBuffMgr().getExtraDamage(pOwner->getUnitID(),pSkillModel->uCompositeBaseSkillID,iDamage);

	CSceneMonster* pMonster = pTarget->asMonster();
	if ( pMonster )
	{
		iTotalDamage += (int32)(iTotalDamage*pOwner->getGameBuffMgr().getMonsterExtraDamage());
	}

	//���ļ��˺��ٷֱ�
	if (pSkillEffectModel->stExpendAddDamage.uExpendType
		&& pOwner->addExpend(pSkillEffectModel->stExpendAddDamage.uExpendType,pSkillEffectModel->stExpendAddDamage.iValue, DoingType_Skill))
	{
		iTotalDamage += int32(iDamage * (pSkillEffectModel->stExpendAddDamage.uRate / 10000.0f)) + pSkillEffectModel->stExpendAddDamage.uDamage;
	}

	//����
	if (pOwner->check_mark(SceneUnitMark_AddExtraDamage,pSkillModel->uSkillID))
	{
		iTotalDamage += int32(iDamage * (pSkillModel->stExpendAddDamage.uRate / 10000.0f)) + pSkillModel->stExpendAddDamage.uDamage;
	}

	// �������������˺���buff(Ѫ��Խ���˺�Խ��)
	int32	nAddHarmValue	= 0;
	float32	fAddHarmRate	= 0.f;
	pOwner->getGameBuffMgr().getAddTargetHarm(nAddHarmValue,fAddHarmRate);
	iTotalDamage		= (int32)((1.0f + fAddHarmRate)*iTotalDamage);
	iTotalDamage		+= nAddHarmValue;

	//�������˺�����
	iTotalDamage += (int32)(iTotalDamage * pTarget->getGameBuffMgr().getAddHarmRate(pSkillModel->uSkillID));

	//���¸���5%
	iTotalDamage = (int32)(iTotalDamage*((float32)randRange(g_clConstantData.Game_FinalDamageRand_High, g_clConstantData.Game_FinalDamageRand_Low)/100));

	//�������˺�
	pTarget->byDamage(pOwner,iTotalDamage,pSkillModel,pSkillEffectModel,uCritical);	
	return true;
}

//-------------------------------------------------------------
//------------------------------ ���Ӽ���
bool	CSkillAnim::skillAddSceneObject(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel* pSkillEffectModel)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel)
		return false;

	uint32 uTargetUnitID = 0;
	if (!mapObject.empty() && mapObject.begin()->second)
		uTargetUnitID = mapObject.begin()->second->getUnitID();

	CGameSceneCell* pSceneCell = pOwner->getSceneCell();
	if (!pSceneCell)
		return false;

	_stPoint2F stTmpTarget = stTargetPoint;
	if ( EffectTargetType_Self == pSkillEffectModel->stAddObject.uTargetPositonType)
		stTmpTarget = pOwner->getPosition();

	//����ƫ��
	_stPoint2F stRealTarget = stTmpTarget;
	getTargetPoint(stTmpTarget,pOwner->getDirection(),pSkillEffectModel->stAddObject.fDistance,stRealTarget);
	_stPoint2F stRandTarget = stRealTarget;
	CGameSceneCell* pCell = pOwner->getSceneCell();
	if (!pCell)
		return false;

	if (pSkillEffectModel->stAddObject.fRandDis > FLOAT_EPSINON)
	{
		pCell->randGetPoint(stRandTarget,stRealTarget.x,stRealTarget.y,pSkillEffectModel->stAddObject.fRandDis);
	}

	if (GameObject_Trap == pSkillEffectModel->stAddObject.uObjectType)
	{
		const _stTrapModel*pTrapModel = g_clTrapData.findTrap(pSkillEffectModel->stAddObject.uOjbectID,pSkillEffectModel->uLevel);
		if (!pTrapModel)
			return false;

		//�޸��� �����Ǿ�̬���� ȡ����Ŀ���
		uint8 uObjectNum = pSkillEffectModel->stAddObject.uObjectNum;
		if (pSkillEffectModel->stAddObject.uExpendType &&
			pOwner->addExpend(pSkillEffectModel->stAddObject.uExpendType,pSkillEffectModel->stAddObject.iValue, DoingType_Skill))
		{
			uObjectNum += pSkillEffectModel->stAddObject.uAddNum;
		}

		if (TrapAngleType_OneDir == pSkillEffectModel->stAddObject.uParam1)
		{
			for (uint8 i = 0; i < uObjectNum; ++i)
			{
				pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection(),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
			}

		}
		else if (TrapAngleType_Equal == pSkillEffectModel->stAddObject.uParam1)
		{
			float32 fBeginDir = pOwner->getDirection() - getAngleToRadian((float32)pSkillEffectModel->stAddObject.uParam2 / 2);
			float32 fEndDir = pOwner->getDirection() + getAngleToRadian((float32)pSkillEffectModel->stAddObject.uParam2 / 2);
			if (1 == uObjectNum)
			{
				pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection(),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
			}
			else if (2 == uObjectNum)
			{
				//��ʼ���յ㶼Ҫ��(360�ȵ�ʱ����ʼ���ص� ȡ����90��)
				if (pSkillEffectModel->stAddObject.uParam2 == 360)
				{
					pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection() - getAngleToRadian(90),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
					pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection() + getAngleToRadian(90),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
				}
				else
				{
					pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,fBeginDir,pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
					pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection() + fEndDir,pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
				}
			}
			else
			{
				//��ʼ���յ㶼Ҫ�� (360�ȵ�ʱ����ʼ���ص� ���һ�ȷ�)
				float32 fInterval = getAngleToRadian((float32)pSkillEffectModel->stAddObject.uParam2 / (uObjectNum - 1)); 
				if (pSkillEffectModel->stAddObject.uParam2 == 360)
					fInterval = getAngleToRadian((float32)pSkillEffectModel->stAddObject.uParam2 / uObjectNum);

				for (uint8 i = 0; i < uObjectNum; ++i)
				{
					pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,fBeginDir,pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
					fBeginDir += fInterval;
				}
			}
		}
		else if (TrapAngleType_Rand == pSkillEffectModel->stAddObject.uParam1)
		{
			for (uint8 i = 0; i < uObjectNum; ++i)
			{
				uint16 uRandAngle = randRange(pSkillEffectModel->stAddObject.uParam2) - pSkillEffectModel->stAddObject.uParam2 / 2;
				float32 fDir = pOwner->getDirection() + getAngleToRadian((float32)uRandAngle);
				pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,fDir,pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
			}
		}
		else if (TrapAngleType_Static == pSkillEffectModel->stAddObject.uParam1)
		{
			for (uint8 i = 0; i < uObjectNum; ++i)
			{
				pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection(),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
				if (pSkillEffectModel->stAddObject.fRandDis > FLOAT_EPSINON)
					pCell->randGetPoint(stRandTarget,stRealTarget.x,stRealTarget.y,pSkillEffectModel->stAddObject.fRandDis);
			}
		}
		else if (TrapAngleType_RandNoRepeat == pSkillEffectModel->stAddObject.uParam1)
		{
			stl_vector<_stPoint2F> vecOut;
			pSceneCell->randGetPoint(vecOut,stRandTarget,pSkillEffectModel->stAddObject.fRandDis,pTrapModel->fRadius,uObjectNum);
			for (uint32 i = 0; i < vecOut.size(); ++i)
			{
				pSceneCell->newTrap(pOwner,pTrapModel,vecOut[i].x,vecOut[i].y,pOwner->getDirection(),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
			}
		}
	}
	else if (GameObject_Monster == pSkillEffectModel->stAddObject.uObjectType)
	{
		const _stMonsterModel*pMonsterModel = g_clMonsterData.find(pSkillEffectModel->stAddObject.uOjbectID);
		if (!pMonsterModel)
			return false;

		_stPoint2F stOut;
		for ( uint32 i = 0 ; i < pSkillEffectModel->stAddObject.uObjectNum ;++i )
		{
			CSceneMonster*  pSceneMonster = pCell->createMonster(pMonsterModel,stRandTarget.x,stRandTarget.y,getRandDir(),0,true);
			if (!pSceneMonster)
				continue;
			pSceneMonster->setOwner(pOwner);
			pCell->randGetPoint(stRandTarget,stRealTarget.x,stRealTarget.y,pSkillEffectModel->stAddObject.fRandDis);

		}
	}

	return true;
}