/*----------------- sceneUnitAI.cpp
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/1/26 11:33:05
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/sceneObject/sceneUnitAI.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameScene/gameSceneCell.h"
#include "cfg_data/skillData/skillData.h"
#include "gameLogic/sceneObject/sceneMonster.h"
#include "cfg_data/monsterData/monsterModel.h"
#include "log/log.h"
#include "cfg_data/constantData/constantData.h"
#include "gameScene/gameScene.h"
#include "cfg_data/copymapData/copymapModel.h"
#include "cfg_data/aiData/aiData.h"
#include "cfg_data/monsterData/monsterData.h"
#include "gameScene/copymapNormal.h"
#define  MOVE_TIMES		6
//-------------------------------------------------------------
//------------------------------ 
BEGIN_PROPERTIES_DESCRIPTION(CSceneUnitAI)
{
	{
#ifndef _WIN32
		REGISTER_METHOD(change2IdleState);
#else
		CMethodBase& meta =	REGISTER_METHOD(change2IdleState);
		meta.DESC(L"ת��������״̬");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(change2BattleState);
#else
		CMethodBase& meta =	REGISTER_METHOD(change2BattleState);
		meta.DESC(L"ת����ս��״̬");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(change2ReturnState);
#else
		CMethodBase& meta =	REGISTER_METHOD(change2ReturnState);
		meta.DESC(L"ת��������״̬");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isIdleState);
#else
		CMethodBase& meta =	REGISTER_METHOD(isIdleState);
		meta.DESC(L"�ж��Ƿ�������״̬");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isBattleState);
#else
		CMethodBase& meta =	REGISTER_METHOD(isBattleState);
		meta.DESC(L"�ж��Ƿ���ս��״̬");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isReturnState);
#else
		CMethodBase& meta =	REGISTER_METHOD(isReturnState);
		meta.DESC(L"�ж��Ƿ��ڻ���״̬");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(doIdle);
#else
		CMethodBase& meta =	REGISTER_METHOD(doIdle);
		meta.DESC(L"ִ�в߻����õ�������Ϊ");
#endif // _WIN32
	}

	{
#ifndef _WIN32
		REGISTER_METHOD(doPatroll);
#else
		CMethodBase& meta =	REGISTER_METHOD(doPatroll);
		meta.DESC(L"ִ��Ѳ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(doPath);
#else
		CMethodBase& meta =	REGISTER_METHOD(doPath);
		meta.DESC(L"ִ��·�����ƶ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(doTargetPos);
#else
		CMethodBase& meta =	REGISTER_METHOD(doTargetPos);
		meta.DESC(L"ִ��Ŀ����ƶ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(doNothing);
#else
		CMethodBase& meta =	REGISTER_METHOD(doNothing);
		meta.DESC(L"ʲô������");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(doAttack);
#else
		CMethodBase& meta =	REGISTER_METHOD(doAttack);
		meta.DESC(L"ִ�й���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(doChase);
#else
		CMethodBase& meta =	REGISTER_METHOD(doChase);
		meta.DESC(L"ִ��׷��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(doReturn);
#else
		CMethodBase& meta =	REGISTER_METHOD(doReturn);
		meta.DESC(L"ִ�л���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(getFreeVar);
#else
		CMethodBase& meta =	REGISTER_METHOD(getFreeVar);
		meta.DESC(L"������ɱ���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setFreeVar);
#else
		CMethodBase& meta =	REGISTER_METHOD(setFreeVar);
		meta.DESC(L"�������ɱ���");
		meta.PARAM_DISPLAY_INFO(L"iValue", L"�µ�ֵ");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(getProperty);
#else
		CMethodBase& meta =	REGISTER_METHOD(getProperty);
		meta.DESC(L"��ȡ�Զ�������(Ĭ��ֵΪ0)");
		meta.PARAM_DISPLAY_INFO(L"iKey", L"��������");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setProperty);
#else
		CMethodBase& meta =	REGISTER_METHOD(setProperty);
		meta.DESC(L"�����Զ�������");
		meta.PARAM_DISPLAY_INFO(L"iKey", L"��������");
		meta.PARAM_DISPLAY_INFO(L"iValue", L"�µ�ֵ");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(clearProperty);
#else
		CMethodBase& meta =	REGISTER_METHOD(clearProperty);
		meta.DESC(L"����Զ��������б�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(hasTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(hasTarget);
		meta.DESC(L"����Ƿ��й�������");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(clearTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(clearTarget);
		meta.DESC(L"���Ŀ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(selectTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(selectTarget);
		meta.DESC(L"������ѡȡĿ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(selectHatredTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(selectHatredTarget);
		meta.DESC(L"�ӳ���б���ѡȡָ�����ֵ�����Ŀ��");
		meta.PARAM_DISPLAY_INFO(L"uHatredOrder", L"�ڼ������Ŀ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setSelectTargetType);
#else
		CMethodBase& meta =	REGISTER_METHOD(setSelectTargetType);
		meta.DESC(L"����Ŀ��ѡȡ����");
		meta.PARAM_DISPLAY_INFO(L"uHatredOrder", L"��cs_ai_config.xls���SelectTargetType�ֶ�һ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setHatredSelectOrder);
#else
		CMethodBase& meta =	REGISTER_METHOD(setHatredSelectOrder);
		meta.DESC(L"���ó�޵���Ĺֵ�ѡ����ֵ������");
		meta.PARAM_DISPLAY_INFO(L"uHatredOrder", L"�ڼ������Ŀ��");
#endif 
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(selectFirstHatredTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(selectFirstHatredTarget);
		meta.DESC(L"�ӳ���б���ѡȡ�����ߵ�Ŀ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(selectRandTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(selectRandTarget);
		meta.DESC(L"�ӳ���б������ѡ��Ŀ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(selectMinHPTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(selectMinHPTarget);
		meta.DESC(L"�ӳ���б���ѡȡѪ�����ٵ�Ŀ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(scanHatredObj);
#else
		CMethodBase& meta =	REGISTER_METHOD(scanHatredObj);
		meta.DESC(L"��龯�䷶Χ�ڵĵ���,���Ҽ������б�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(getTargetDistance);
#else
		CMethodBase& meta =	REGISTER_METHOD(getTargetDistance);
		meta.DESC(L"����뵱ǰĿ��ľ���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isTargetInReleaseRange);
#else
		CMethodBase& meta =	REGISTER_METHOD(isTargetInReleaseRange);
		meta.DESC(L"Ŀ���Ƿ���ѡ�м��ܵ���̷�Χ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isTargetInSkillRange);
#else
		CMethodBase& meta =	REGISTER_METHOD(isTargetInSkillRange);
		meta.DESC(L"Ŀ���Ƿ���ָ�����ܵ���̷�Χ��");
		meta.PARAM_DISPLAY_INFO(L"uSkillID", L"Ҫʹ�õļ���ID");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(hasSkill);
#else
		CMethodBase& meta =	REGISTER_METHOD(hasSkill);
		meta.DESC(L"�Ƿ���ѡ�м���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isSkillCooling);
#else
		CMethodBase& meta =	REGISTER_METHOD(isSkillCooling);
		meta.DESC(L"�ü����Ƿ�����ȴ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(selectSkill);
#else
		CMethodBase& meta =	REGISTER_METHOD(selectSkill);
		meta.DESC(L"������ʹ�����ȼ�ѡȡ��ǰ��ʹ�õļ���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setSelectSkill);
#else
		CMethodBase& meta =	REGISTER_METHOD(setSelectSkill);
		meta.DESC(L"ָ����ǰҪʹ�õļ���");
		meta.PARAM_DISPLAY_INFO(L"uSkillID", L"Ҫʹ�õļ���ID");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(useSkill);
#else
		CMethodBase& meta =	REGISTER_METHOD(useSkill);
		meta.DESC(L"ʹ��ָ������,��������ѡȡĿ��");
		meta.PARAM_DISPLAY_INFO(L"uSkillID", L"Ҫʹ�õļ���ID");
		meta.PARAM_DISPLAY_INFO(L"uLevel", L"Ҫʹ�õļ��ܵȼ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(useSelectedSkill);
#else
		CMethodBase& meta =	REGISTER_METHOD(useSelectedSkill);
		meta.DESC(L"ʹ�õ�ǰѡ�м���,��ǰû��ѡ�м��ܵĻ����Զ�ѡȡһ�����ü���,��������ѡȡĿ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(addSkill);
#else
		CMethodBase& meta =	REGISTER_METHOD(addSkill);
		meta.DESC(L"���ӳ��漼��");
		meta.PARAM_DISPLAY_INFO(L"uSkillID", L"Ҫ���ӵļ���ID");
		meta.PARAM_DISPLAY_INFO(L"uPriority", L"ʹ�����ȼ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(delSkill);
#else
		CMethodBase& meta =	REGISTER_METHOD(delSkill);
		meta.DESC(L"ɾ�����漼��");
		meta.PARAM_DISPLAY_INFO(L"uSkillID", L"Ҫɾ���ļ���ID");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(resetSkill);
#else
		CMethodBase& meta =	REGISTER_METHOD(resetSkill);
		meta.DESC(L"���ó��漼��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(startSpecialCD);
#else
		CMethodBase& meta =	REGISTER_METHOD(startSpecialCD);
		meta.DESC(L"����ָ����CD��ʼ��ȴ");
		meta.PARAM_DISPLAY_INFO(L"uIndex", L"CD����");
		meta.PARAM_DISPLAY_INFO(L"uSecondCD", L"��ȴʱ�����");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(checkSpecialCD);
#else
		CMethodBase& meta =	REGISTER_METHOD(checkSpecialCD);
		meta.DESC(L"�鿴ָ����CD��û����ȴ���");
		meta.PARAM_DISPLAY_INFO(L"uIndex", L"CD����");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(checkAndStartSpecialCD);
#else
		CMethodBase& meta =	REGISTER_METHOD(checkAndStartSpecialCD);
		meta.DESC(L"�鿴ָ����CD��û����ȴ��ɣ�������ȴ�з��ؼ٣��Ѿ������ȴ�����沢������ȴ");
		meta.PARAM_DISPLAY_INFO(L"uIndex", L"CD����");
		meta.PARAM_DISPLAY_INFO(L"uSecondCD", L"��ȴʱ�����");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(clearBuff);
#else
		CMethodBase& meta =	REGISTER_METHOD(clearBuff);
		meta.DESC(L"���Buff");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(clearHatred);
#else
		CMethodBase& meta =	REGISTER_METHOD(clearHatred);
		meta.DESC(L"��ճ���б�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(clearTargetHatred);
#else
		CMethodBase& meta =	REGISTER_METHOD(clearTargetHatred);
		meta.DESC(L"�����ǰĿ��ĳ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(clearHurt);
#else
		CMethodBase& meta =	REGISTER_METHOD(clearHurt);
		meta.DESC(L"����˺��б�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(addBuff);
#else
		CMethodBase& meta =	REGISTER_METHOD(addBuff);
		meta.DESC(L"����buff");
		meta.PARAM_DISPLAY_INFO(L"uBuffID", L"buff id");
		meta.PARAM_DISPLAY_INFO(L"uLevel", L"buff�ȼ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(delBuff);
#else
		CMethodBase& meta =	REGISTER_METHOD(delBuff);
		meta.DESC(L"ɾ��buff");
		meta.PARAM_DISPLAY_INFO(L"uBuffID", L"buff id");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(hasHatredObj);
#else
		CMethodBase& meta =	REGISTER_METHOD(hasHatredObj);
		meta.DESC(L"�Ƿ��г�޶���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(canMove);
#else
		CMethodBase& meta =	REGISTER_METHOD(canMove);
		meta.DESC(L"�Ƿ���ƶ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(canUseSkill);
#else
		CMethodBase& meta =	REGISTER_METHOD(canUseSkill);
		meta.DESC(L"�Ƿ��ʹ�ü���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isFighting);
#else
		CMethodBase& meta =	REGISTER_METHOD(isFighting);
		meta.DESC(L"�Ƿ���ս����");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isMoving);
#else
		CMethodBase& meta =	REGISTER_METHOD(isMoving);
		meta.DESC(L"�Ƿ��ƶ���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isUsingSkill);
#else
		CMethodBase& meta =	REGISTER_METHOD(isUsingSkill);
		meta.DESC(L"�Ƿ�ʹ�ü�����");
#endif // _WIN32
	}
	{
#ifndef _WIN32
	REGISTER_METHOD(getHPPercent);
#else
	CMethodBase& meta =	REGISTER_METHOD(getHPPercent);
	meta.DESC(L"��ȡ��ǰѪ���ٷֱ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(getCamp);
#else
		CMethodBase& meta =	REGISTER_METHOD(getCamp);
		meta.DESC(L"�����Ӫ");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(changeCamp);
#else
		CMethodBase& meta =	REGISTER_METHOD(changeCamp);
		meta.DESC(L"�ı���Ӫ");
		meta.PARAM_DISPLAY_INFO(L"uNewCampID", L"����ӪID");
#endif // _WIN32
	}
//////////////////////////////////////////////////////////////////////////
	{
#ifndef _WIN32
		REGISTER_METHOD(killMonsterById);
#else
		CMethodBase& meta =	REGISTER_METHOD(killMonsterById);
		meta.DESC(L"ɱ�����ָ��id�Ĺ��");
		meta.PARAM_DISPLAY_INFO(L"uMonsterId", L"Ҫɱ���Ĺ���ID");
		meta.PARAM_DISPLAY_INFO(L"uCount", L"���ɱ��������");
		meta.PARAM_DISPLAY_INFO(L"fRadius", L"�����뾶");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(addSceneArea);
#else
		CMethodBase& meta =	REGISTER_METHOD(addSceneArea);
		meta.DESC(L"���������е���������");
		meta.PARAM_DISPLAY_INFO(L"uAreaID", L"��������ID");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(delSceneArea);
#else
		CMethodBase& meta =	REGISTER_METHOD(delSceneArea);
		meta.DESC(L"�رճ����е���������");
		meta.PARAM_DISPLAY_INFO(L"uAreaID", L"��������ID");
#endif
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(checkSkillFailedAndWait);
#else
		CMethodBase& meta =	REGISTER_METHOD(checkSkillFailedAndWait);
		meta.DESC(L"����Ƿ��м�����Ϊ�赲�������û���ͷųɹ��������������");
		meta.PARAM_DISPLAY_INFO(L"uSkillID", L"����ID");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(speak);
#else
		CMethodBase& meta =	REGISTER_METHOD(speak);
		meta.DESC(L"˵��");
		meta.PARAM_DISPLAY_INFO(L"uSpeakID", L"˵��ID");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setAIPoint);
#else
		CMethodBase& meta =	REGISTER_METHOD(setAIPoint);
		meta.DESC(L"����ǰ��������Ϊս�����ĵ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(stopAI);
#else
		CMethodBase& meta =	REGISTER_METHOD(stopAI);
		meta.DESC(L"ֹͣAI");
		meta.PARAM_DISPLAY_INFO(L"bStop", L"trueֹͣfalse����");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setBirthPoint);
#else
		CMethodBase& meta =	REGISTER_METHOD(setBirthPoint);
		meta.DESC(L"����ǰ��������Ϊ������");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setAI);
#else
		CMethodBase& meta =	REGISTER_METHOD(setAI);
		meta.DESC(L"����AI");
		meta.PARAM_DISPLAY_INFO(L"uID", L"��AI������ID");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setPathID);
#else
		CMethodBase& meta =	REGISTER_METHOD(setPathID);
		meta.DESC(L"����·����ID");
		meta.PARAM_DISPLAY_INFO(L"uPatrollId", L"��·��ID");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setTargetPos);
#else
		CMethodBase& meta =	REGISTER_METHOD(setTargetPos);
		meta.DESC(L"����Ŀ�������");
		meta.PARAM_DISPLAY_INFO(L"fPosX", L"Ŀ������x");
		meta.PARAM_DISPLAY_INFO(L"fPosY", L"Ŀ������y");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isPathComplete);
#else
		CMethodBase& meta =	REGISTER_METHOD(isPathComplete);
		meta.DESC(L"·���ƶ��Ƿ������(·��������0�Ĳ�������)");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isTargetPosComplete);
#else
		CMethodBase& meta =	REGISTER_METHOD(isTargetPosComplete);
		meta.DESC(L"Ŀ����ƶ��Ƿ������");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(getPlayerCountRange);
#else
		CMethodBase& meta =	REGISTER_METHOD(getPlayerCountRange);
		meta.DESC(L"��ȡ��Χ���������");
		meta.PARAM_DISPLAY_INFO(L"fRadius", L"�����뾶");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(getDisFromAIPoint);
#else
		CMethodBase& meta =	REGISTER_METHOD(getDisFromAIPoint);
		meta.DESC(L"�����ս�����ĵ�ľ���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(getFightTime);
#else
		CMethodBase& meta =	REGISTER_METHOD(getFightTime);
		meta.DESC(L"����ս��ʱ��(��)");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(questComplete);
#else
		CMethodBase& meta =	REGISTER_METHOD(questComplete);
		meta.DESC(L"���ɱ������");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(removeMonsterById);
#else
		CMethodBase& meta =	REGISTER_METHOD(removeMonsterById);
		meta.DESC(L"ɾ�����ָ��id�Ĺ���");
		meta.PARAM_DISPLAY_INFO(L"uMonsterId", L"Ҫɾ���Ĺ���ID");
		meta.PARAM_DISPLAY_INFO(L"uCount", L"���ɾ��������");
		meta.PARAM_DISPLAY_INFO(L"fRadius", L"�����뾶");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(killSelf);
#else
		CMethodBase& meta =	REGISTER_METHOD(killSelf);
		meta.DESC(L"��ɱ");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(removeSelf);
#else
		CMethodBase& meta =	REGISTER_METHOD(removeSelf);
		meta.DESC(L"�Լ���ʧ");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(setTime);
#else
		CMethodBase& meta =	REGISTER_METHOD(setTime);
		meta.DESC(L"���ö�ʱ��");
		meta.PARAM_DISPLAY_INFO(L"uIndex", L"��ʱ��ID");
		meta.PARAM_DISPLAY_INFO(L"uMS", L"ʱ��(����)");
		meta.PARAM_DISPLAY_INFO(L"bRuning", L"�Ƿ񷵻�ִ����");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(resetTime);
#else
		CMethodBase& meta =	REGISTER_METHOD(resetTime);
		meta.DESC(L"������ʱ��");
		meta.PARAM_DISPLAY_INFO(L"uIndex", L"��ʱ��ID");
#endif // _WIN32
	}

	{
#ifndef _WIN32
		REGISTER_METHOD(addMonster_fixPos);
#else
		CMethodBase& meta =	REGISTER_METHOD(addMonster_fixPos);
		meta.DESC(L"�̶�λ��ˢ��");
		meta.PARAM_DISPLAY_INFO(L"uMonsterId", L"����ID");
		meta.PARAM_DISPLAY_INFO(L"uCount", L"����");
		meta.PARAM_DISPLAY_INFO(L"fPosX", L"λ��x");
		meta.PARAM_DISPLAY_INFO(L"fPosY", L"λ��y");
		meta.PARAM_DISPLAY_INFO(L"fAngleDir", L"�Ƕ�");
#endif // _WIN32
	}

	{
#ifndef _WIN32
		REGISTER_METHOD(addMonster_relatedPos);
#else
		CMethodBase& meta =	REGISTER_METHOD(addMonster_relatedPos);
		meta.DESC(L"���λ��ˢ��");
		meta.PARAM_DISPLAY_INFO(L"uMonsterId", L"����ID");
		meta.PARAM_DISPLAY_INFO(L"uCount", L"����");
		meta.PARAM_DISPLAY_INFO(L"fRelatedX", L"���λ��x");
		meta.PARAM_DISPLAY_INFO(L"fRelatedY", L"���λ��y");
		meta.PARAM_DISPLAY_INFO(L"fAngleDir", L"�Ƕ�");
#endif // _WIN32
	}

	{
#ifndef _WIN32
		REGISTER_METHOD(addMonster_randomPos);
#else
		CMethodBase& meta =	REGISTER_METHOD(addMonster_randomPos);
		meta.DESC(L"���λ��ˢ��");
		meta.PARAM_DISPLAY_INFO(L"uMonsterId", L"����ID");
		meta.PARAM_DISPLAY_INFO(L"uCount", L"����");
		meta.PARAM_DISPLAY_INFO(L"fMinRadius", L"�����С�뾶");
		meta.PARAM_DISPLAY_INFO(L"fMaxRadius", L"�����С��");
		meta.PARAM_DISPLAY_INFO(L"fAngleDir", L"�Ƕ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(changeDirection);
#else
		CMethodBase& meta =	REGISTER_METHOD(changeDirection);
		meta.DESC(L"�ı䳯��");
		meta.PARAM_DISPLAY_INFO(L"uAngle", L"�³���Ƕ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(sendMessageAround);
#else
		CMethodBase& meta =	REGISTER_METHOD(sendMessageAround);
		meta.DESC(L"����Χ������Ϣ");
		meta.PARAM_DISPLAY_INFO(L"uIndex", L"��ϢID");
		meta.PARAM_DISPLAY_INFO(L"uMonsterID", L"������Ϣ�Ĺ���ID(0�������й���)");
		meta.PARAM_DISPLAY_INFO(L"fRadius", L"������Ϣ�뾶");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(sendMessage);
#else
		CMethodBase& meta =	REGISTER_METHOD(sendMessage);
		meta.DESC(L"������Ϣ");
		meta.PARAM_DISPLAY_INFO(L"uIndex", L"��ϢID");
		meta.PARAM_DISPLAY_INFO(L"uMonsterID", L"������Ϣ�Ĺ���ID(0�������й���)");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(readMessage);
#else
		CMethodBase& meta =	REGISTER_METHOD(readMessage);
		meta.DESC(L"��ȡ��Ϣ");
		meta.PARAM_DISPLAY_INFO(L"uIndex", L"��ϢID");
		meta.PARAM_DISPLAY_INFO(L"bDelete", L"��ȡ�ɹ����Ƿ�ɾ����Ϣ");
#endif // _WIN32
	}

	{
#ifndef _WIN32
		REGISTER_METHOD(targetAddBuff);
#else
		CMethodBase& meta =	REGISTER_METHOD(targetAddBuff);
		meta.DESC(L"Ŀ������buff");
		meta.PARAM_DISPLAY_INFO(L"uBuffID", L"buff id");
		meta.PARAM_DISPLAY_INFO(L"uLevel", L"buff�ȼ�");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(targetDelBuff);
#else
		CMethodBase& meta =	REGISTER_METHOD(targetDelBuff);
		meta.DESC(L"Ŀ��ɾ��buff");
		meta.PARAM_DISPLAY_INFO(L"uBuffID", L"buff id");
#endif // _WIN32
	}
		{
#ifndef _WIN32
		REGISTER_METHOD(selectBuffTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(selectBuffTarget);
		meta.DESC(L"ѡ��Я���ض�buff��Ŀ��");
		meta.PARAM_DISPLAY_INFO(L"uBuffID", L"buff id");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(askForHelpAround);
#else
		CMethodBase& meta =	REGISTER_METHOD(askForHelpAround);
		meta.DESC(L"��ָ����Χ��Ԯ");
		meta.PARAM_DISPLAY_INFO(L"uMonsterID", L"��Ԯ�Ĺ�������(0�����й���)");
		meta.PARAM_DISPLAY_INFO(L"fRadius", L"��Ԯ�뾶");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(askForHelp);
#else
		CMethodBase& meta =	REGISTER_METHOD(askForHelp);
		meta.DESC(L"������ͼ��Ԯ");
		meta.PARAM_DISPLAY_INFO(L"uMonsterID", L"��Ԯ�Ĺ�������(0�����й���)");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(playerScanTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(playerScanTarget);
		meta.DESC(L"�Զ�ս���У�����Ŀ�꣬������Ŀ����ݡ�����֣�BOSS�֣���Ӣ�֣���ͨ�֡����� ���������Զ�ս��Ŀ���б���");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(playerSelectTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(playerSelectTarget);
		meta.DESC(L"���Զ�ս��Ŀ���б���ѡ��һ����Ч��Ŀ�꣨��������֣�BOSS�֣���Ӣ�֣���ͨ�ֵĹ���ѡȡ��");
#endif // _WIN32
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(isBeAttack);
#else
		CMethodBase& meta =	REGISTER_METHOD(isBeAttack);
		meta.DESC(L"�ж����ʱ������û�б�����");
		meta.PARAM_DISPLAY_INFO(L"uIntervalMS", L"������ٺ���");
#endif 
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(moveBack);
#else
		CMethodBase& meta =	REGISTER_METHOD(moveBack);
		meta.DESC(L"��Ŀ��ķ������ƶ����õľ��루�ƶ���ָ������󷵻سɹ���");
		meta.PARAM_DISPLAY_INFO(L"fDis", L"ָ������");
#endif 
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(moveToTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(moveToTarget);
		meta.DESC(L"�ƶ������뵱ǰĿ��ָ�������ڵ�λ��");
		meta.PARAM_DISPLAY_INFO(L"fDis", L"ָ������");
#endif 
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(selectNearstTargetAround);
#else
		CMethodBase& meta =	REGISTER_METHOD(selectNearstTargetAround);
		meta.DESC(L"���ӳ�޻��Ʋ��Ҳ��������Ŀ��");
		meta.PARAM_DISPLAY_INFO(L"fRadius", L"ָ����Χ");
#endif 
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(selectNearstTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(selectNearstTarget);
		meta.DESC(L"�ӳ���б���ѡȡ������С��Ŀ��");
#endif 
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(selectMonsterTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(selectMonsterTarget);
		meta.DESC(L"���ӳ�޲���ָ��������ΪĿ��");
		meta.PARAM_DISPLAY_INFO(L"uMonsterID", L"����ID");
#endif 
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(faceTarget);
#else
		CMethodBase& meta =	REGISTER_METHOD(faceTarget);
		meta.DESC(L"��������������Ŀ��");
#endif 
	}
	{
#ifndef _WIN32
		REGISTER_METHOD(closeCopymap);
#else
		CMethodBase& meta =	REGISTER_METHOD(closeCopymap);
		meta.DESC(L"�رո���");
		meta.PARAM_DISPLAY_INFO(L"uTime", L"��ʱ�رո���ʱ��");
		meta.PARAM_DISPLAY_INFO(L"uChatID", L"chat��ID");
#endif 
	}
}
END_PROPERTIES_DESCRIPTION()
//-------------------------------------------------------------
//------------------------------ 
CSceneUnitAI::CSceneUnitAI()
{
	initialize();
}
//-------------------------------------------------------------
//------------------------------ 
CSceneUnitAI::~CSceneUnitAI()
{
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneUnitAI::initialize()
{
	m_pAIModel		= NULL;
	m_pOwner		= NULL;
	m_uTargetID		= 0;
	m_pSkillModel	= NULL;
	m_vecOrdinalSkill.clear();
	m_mapProperty.clear();
	m_setMessage.clear();
	m_stMoveDest.zero();
	m_stMoveBeginPos.zero();
	m_stAIPoint.zero();
	m_stBirthPoint.zero();
	m_mapSpecialCD.clear();
	m_mapSpecialTimer.clear();
	m_fFirstDirection = 0.0f;
	m_uPathID = 0;
	m_uPathTarIndex = 0;
	m_stTargetPos.zero();
	m_uFreeVar = 0;
	m_uRunAIFrameCount = 0;
	m_uMoveType = MoveType_Walk;
	m_eAIStateType	= AIState_Idle;
	m_uNextPatrolTime	= 0;
	m_uFailedSkillIdByBlock = 0;
	m_uSelectTargetType	= 0;
	m_uHatredSelectOrder= 0;
	m_uLastBeAttackTick	= 0;
	m_uAICallFrequency	= 0;
	m_bStop				= 0;
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneUnitAI::process(uint64 uTick,uint64 uTime,uint32 uSecond)
{
	if (!canOperate() || !m_pOwner->canRunAI())
		return;
	
	//�ƶ��ƽ�
	processMove();

	if (m_bStop)
		return ;

	++m_uRunAIFrameCount;
	// ��·Ҫ��ȷ����
	if ( m_uRunAIFrameCount >= m_uAICallFrequency )
	{
		m_uRunAIFrameCount = 0 ;

		//AI�ƽ�
		btexec();

		//�ƽ���޵�λ ������
		processOutHatred();
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnitAI::isFighting()
{
	return m_pOwner && m_pOwner->isFighting();
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnitAI::isMoving	()							//��ѯ�Ƿ��ƶ�״̬
{
	return m_pOwner && m_pOwner->isMoving();
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneUnitAI::isUsingSkill			()
{
	return m_pOwner && (m_pOwner->haveState(StateType_Singing) || m_pOwner->haveState(StateType_Skiling) );
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnitAI::isBeAttack				(uint32 uIntervalMS)
{
	if (!m_uLastBeAttackTick)
		return false;

	if (uIntervalMS > getSystemTick64() - m_uLastBeAttackTick)
		return false;

	m_uLastBeAttackTick = 0;		// �߻���������� Ҫ����
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnitAI::setMoveDest(uint8 MoveType,float fPX, float fPY)
{
	if (!canOperate())
		return false;

	endMove();
	if (!canMove() || isUsingSkill() )
		return false;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return false;

	if (!pCell->canMove(fPX, fPY))
		return false;

	m_stMoveDest.x = fPX;
	m_stMoveDest.y = fPY;
	_stPoint2F	stDirectDest;
	if ( !pCell->findPath(m_pOwner->getPosition(), m_stMoveDest, stDirectDest))
		return false;

	return beginMove(MoveType,stDirectDest);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnitAI::processOutHatred	()
{
	if (!canOperate() )
		return ;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return ;

	CHatredMgr::MAP_HATRED::iterator _pos;
	m_pOwner->getHatredMgr().m_mapHatred.getHead(_pos);
	while (!m_pOwner->getHatredMgr().m_mapHatred.isTail(_pos))
	{
		CHatredMgr::_stHatred* pHatred = m_pOwner->getHatredMgr().m_mapHatred.getNext(_pos);
		if (!pHatred)
			continue;

		CSceneUnit* pUnit = pCell->findObject(pHatred->uUnitID);
		if (!pUnit)
			continue;

		if (getDistance(pUnit->getPosition(), m_pOwner->getPosition()) > m_pAIModel->fHatredRadius || !m_pOwner->getHatredMgr().canHatred(pUnit) )
			m_pOwner->getHatredMgr().deleteHatred(pUnit);
	}

	if (m_uTargetID)
	{
		CSceneUnit* pUnit = pCell->findObject(m_uTargetID);
		if (!pUnit)
		{
			m_uTargetID = 0;
		}
		else if( getDistance(pUnit->getPosition(), m_stAIPoint) > m_pAIModel->fPursueDis && getDistance(m_pOwner->getPosition(), m_stAIPoint) > m_pAIModel->fPursueDis )
		{
			m_pOwner->getHatredMgr().deleteHatred(pUnit);
			m_uTargetID = 0;
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneUnitAI::processMove()
{
	if (!canOperate() || !m_pOwner->isMoving() )
		return ;

	// �Ѿ��߳��ľ���
	float32 fAlreadyMoveDis = getDistance(m_stMoveBeginPos,m_pOwner->getPosition());

	// ��ƫ��
	if (abs(m_pOwner->getDirection() - m_fCurrentMoveDir ) >= 0.01f ||
		(fAlreadyMoveDis >= 1.0f && abs( get2PiRadian(m_stMoveBeginPos, m_pOwner->getPosition()) - m_fCurrentMoveDir ) >= 0.01f) )
	{
		endMove();
		return ;
	}

	//û���յ�
	if ( fAlreadyMoveDis < m_fCurrentMoveDis ) 
		return ;

	endMove();
	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return ;

	if (m_stMoveDest.isZero() || getDistance(m_pOwner->getPosition(),m_stMoveDest) <= 0.5f )
		return ;

	_stPoint2F stDirectDest;
	if ( !pCell->findPath(m_pOwner->getPosition(), m_stMoveDest, stDirectDest))
		return ;

	beginMove(m_uMoveType,stDirectDest);
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::setAI(uint32 uID)
{
	const _stAIModel*	 pAIModel = g_clAIData.find(uID);
	if (!pAIModel || pAIModel->strBTName.empty() )
		return behaviac::BT_FAILURE;

	m_pAIModel = pAIModel;
	m_uAICallFrequency		= MAX(m_uAICallFrequency,pAIModel->uAICallFrequency);
	m_uSelectTargetType = m_pAIModel->uSelectTargetType;
	m_uHatredSelectOrder= 0;
	bool bRet = btload(pAIModel->strBTName.c_str());
	if (bRet)
	{
		btsetcurrent(pAIModel->strBTName.c_str());
	}
	return bRet?behaviac::BT_SUCCESS:behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::stopAI(bool bStop)
{
	if (bStop)
	{
		endMove();
	}
	m_bStop = bStop;
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
CSceneUnitAI* CSceneUnitAI::createSceneUnitAI(CSceneUnit* pOwner,uint32 uAIID)
{
 	if (!pOwner || !uAIID)
 		return NULL;

	CSceneUnitAI* pUnitAI = behaviac::Agent::Create<CSceneUnitAI>();
	if (!pUnitAI)
		return NULL;

	pUnitAI->initialize();
	pUnitAI->build(pOwner,uAIID);

	return pUnitAI;
}

//-------------------------------------------------------------
//------------------------------ 
void CSceneUnitAI::destroySceneUnitAI(CSceneUnitAI*& pUnitAI)
{
	if (pUnitAI)
	{
		behaviac::Agent::Destroy(pUnitAI);
		pUnitAI = NULL;
	}
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::useSkill		(uint32 uSkillID, uint8 uLevel)			//�򵥵�ʹ�ü���,��������ѡȡĿ��
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	//����ʹ����
	if (isUsingSkill())
	{
		//CLog::warn("----return--running---------AI----%s",__FUNCTION__);
		return behaviac::BT_RUNNING;
	}

	endMove();
	
	uint8 uSkillLevel = (uLevel==0 ? m_pAIModel->uDefaultSkillLevel : uLevel);
	
	_stSkillModel* pSkillModel = g_clSkillData.findSkill(uSkillID,uSkillLevel);
	if (!pSkillModel)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	_stPoint2F stPoint;
	float32 fDir;
	CSceneUnit* pUnit = m_uTargetID ? pCell->findObject(m_uTargetID) : NULL;
	if (pUnit)	//Ŀ�겻����
	{
		stPoint = pUnit->getPosition();
		fDir	= get2PiRadian(m_pOwner->getPosition(), stPoint);
	}
	else
	{
		m_uTargetID = 0;
		stPoint		= m_pOwner->getPosition();
		fDir		= m_pOwner->getDirection();
	}
	if (m_pOwner->getGameSkillMgr().useSkill(uSkillID,pSkillModel->uLevel,fDir,m_uTargetID,stPoint))
	{
		return behaviac::BT_SUCCESS;
	}

	return behaviac::BT_SUCCESS;

}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::useSelectedSkill()
{
	if ( !m_pSkillModel )
		selectSkill();

	if ( m_pSkillModel )
		return useSkill(m_pSkillModel->uSkillID, 1);

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnitAI::getTargetPosByUnitID	(uint32 uUnitID ,_stPoint2F& stPoint)
{
	if (!canOperate())
		return false;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return false;

	CSceneUnit* pUnit = pCell->findObject(uUnitID);
	if (!pUnit)
		return false;

	stPoint = pUnit->getPosition();
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnitAI::getCurrentTargetPos	(_stPoint2F& stPoint)
{
	if (!m_uTargetID)
		return false;

	return getTargetPosByUnitID(m_uTargetID,stPoint);
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnitAI::hasHatredObj()
{
	if (!canOperate())
		return false;

	return m_pOwner->getHatredMgr().hasHatred();
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::setBirthPoint()
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	m_stBirthPoint = m_pOwner->getPosition();
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::setPathID	(uint32 uPathID) 
{
	m_uPathID		= uPathID;
	m_uPathTarIndex = 0;
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::setTargetPos	(float32 fPosX,float32 fPosY) 
{
	m_stTargetPos.x		= fPosX;
	m_stTargetPos.y		= fPosY;
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------
behaviac::EBTStatus		CSceneUnitAI::questComplete			()
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CSceneMonster* pMonster = m_pOwner->asMonster();
	if (!pMonster)
		return behaviac::BT_FAILURE;

	pMonster->questComplete();
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
float32 CSceneUnitAI::getDisFromAIPoint()
{
	if (!canOperate() )
		return 0.0f;

	return getDistance(m_pOwner->getPosition(), m_stAIPoint);
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnitAI::endMove()
{
	if (!canOperate() || !m_pOwner->isMoving())
		return false;

	m_pOwner->endMove();
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneUnitAI::build(CSceneUnit* pOwner,uint32 uAIID)
{
	if ( !pOwner )
		return ;

	m_vecOrdinalSkill.clear();
	m_pOwner = pOwner;
	CSceneMonster* pMonster = m_pOwner->asMonster();
	if (pMonster)
	{
		const _stMonsterModel* pModel = pMonster->getMonsterModel();		
		if (pModel)
			m_vecOrdinalSkill	= pModel->vecSkills;
		m_uPathID				= pMonster->getRouteID();
		m_uAICallFrequency		= (pModel->uType == MonsterType_Monster && pModel->uSubType == Monster_Normal) ? 5 :0;
	}
	m_fFirstDirection = pOwner->getDirection();
	setAI(uAIID);
	setAIPoint();
	setBirthPoint();
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::clearBuff()
{
	if (m_pOwner)
		m_pOwner->getGameBuffMgr().clear();

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::clearHatred()
{
	if (m_pOwner)
		m_pOwner->getHatredMgr().clear();

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::clearTargetHatred()
{
	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;

	CSceneUnit* pTarget = pCell->findObject(m_uTargetID);
	if (!pTarget)
		return behaviac::BT_SUCCESS;

	if (m_pOwner)
		m_pOwner->getHatredMgr().deleteHatred( pTarget);

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::clearHurt()
{
	if (m_pOwner)
		m_pOwner->getHurtMgr().clear();

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::addBuff(uint32 uBuffID, uint8 uLevel)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	uint8 uBuffLevel = (uLevel==0 ? m_pAIModel->uDefaultBuffLevel: uLevel); 

	if (m_pOwner)
		m_pOwner->getGameBuffMgr().addBuff(uBuffID, uBuffLevel, m_pOwner->getUnitID(),0, DoingType_AI);

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::addSkill(uint32 uSkillID,uint32 uPriority)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	auto _pos = m_vecOrdinalSkill.begin();
	auto _end = m_vecOrdinalSkill.end();
	for ( ; _pos != _end ; ++_pos )
	{
		// �Ѿ�����
		if (_pos->uSkillID == uSkillID )
			return behaviac::BT_SUCCESS;
	}

	_stAIOrdinalSkill stSkill;
	stSkill.uSkillID	= uSkillID;
	stSkill.uPriority	= uPriority;
	m_vecOrdinalSkill.push_back(stSkill);
	std::sort(m_vecOrdinalSkill.begin(),m_vecOrdinalSkill.end());
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::delSkill(uint32 uSkillID)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	auto _pos = m_vecOrdinalSkill.begin();
	auto _end = m_vecOrdinalSkill.end();
	for ( ; _pos != _end ; ++_pos )
	{
		// �ҵ���
		if (_pos->uSkillID == uSkillID )
		{
			m_vecOrdinalSkill.erase(_pos);
			break;
		}
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::resetSkill()
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	m_vecOrdinalSkill.clear();
	CSceneMonster* pMonster = m_pOwner->asMonster();
	if (pMonster)
	{
		const _stMonsterModel* pModel = pMonster->getMonsterModel();		
		if (pModel)
			m_vecOrdinalSkill	= pModel->vecSkills;
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::delBuff(uint32 uBuffID)
{
	if (m_pOwner)
		m_pOwner->getGameBuffMgr().delBuff(uBuffID, DoingType_AI);

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
float32					CSceneUnitAI::getMoveSpeed		()const
{
	if ( !m_pOwner )
		return 1.0f;

	return m_pOwner->getMoveSpeed( m_uMoveType );
}
//-------------------------------------------------------------
//------------------------------ 
bool					CSceneUnitAI::beginMove			(uint8 MoveType,const _stPoint2F& stDirectDest)
{
	if ( !canOperate() )
		return false;
	
	m_stMoveBeginPos	= m_pOwner->getPosition();
	m_fCurrentMoveDir	= get2PiRadian(m_stMoveBeginPos, stDirectDest);
	m_fCurrentMoveDis	= getDistance(m_stMoveBeginPos,stDirectDest);
	return m_pOwner->beginMove( MoveType,m_fCurrentMoveDir);
}

//-------------------------------------------------------------
//------------------------------ 
uint32 CSceneUnitAI::getHPPercent()
{
	if (!canOperate() || !m_pOwner->getHPMax() )
		return 0;

	return (uint32)(100*m_pOwner->getHP() / m_pOwner->getHPMax());
}

//-------------------------------------------------------------
//------------------------------ 
uint32 CSceneUnitAI::getPlayerCountRange(float32 fRadius)
{
	if (!canOperate())
		return 0;

	MAP_GAME_UNIT player_map;
	m_pOwner->gatherSceneObject(m_pOwner->getPosition(), fRadius, player_map, _BIT32(GameObject_Player));
	return player_map.size();
}
//-------------------------------------------------------------
//------------------------------ 
int32 CSceneUnitAI::getProperty(int32 iKey)
{
	MAP_I32_I32::iterator it = m_mapProperty.find(iKey);
	if (it != m_mapProperty.end())
		return it->second;

	return 0;
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneUnitAI::setProperty(int32 iKey, int32 iValue)
{
	m_mapProperty[iKey] = iValue;
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneUnitAI::clearProperty()
{
	m_mapProperty.clear();
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::setSelectTargetType		(uint32 uType)
{
	m_uSelectTargetType = uType;
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::setHatredSelectOrder	(uint32 uOrder)						//���ó�޵���Ĺֵ�ѡ��������
{
	m_uHatredSelectOrder = uOrder;
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::selectTarget()						//������ѡȡĿ��
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	switch (m_uSelectTargetType)
	{
	case AISelectTargetType_Hatred:
		{
			return selectHatredTarget(m_uHatredSelectOrder);
		}
		break;
	case AISelectTargetType_Rand:
		{
			if (hasTarget())
				return behaviac::BT_SUCCESS;

			return selectRandTarget();
		}
		break;
	case AISelectTargetType_MinHP:
		{
			return selectMinHPTarget();
		}
		break;
	case AISelectTargetType_Nearst:
		{
			return selectNearstTarget();
		}
		break;
	default:
		break;
	}
	return behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::selectHatredTarget		(uint32 uHatredOrder)
{
	if ( !m_pOwner )
		return behaviac::BT_FAILURE;

	m_uTargetID = 0;
	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;


	m_uTargetID = m_pOwner->getHatredMgr().getOrderHatredID(uHatredOrder);
	return m_uTargetID ? behaviac::BT_SUCCESS : behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::selectFirstHatredTarget		()							//�ӳ���б���ѡȡ�����ߵ�Ŀ��
{
	return selectHatredTarget(0);
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::selectRandTarget		()							//�ӳ���б������ѡ��Ŀ��
{
	if ( !m_pOwner )
		return behaviac::BT_FAILURE;

	m_uTargetID = 0;
	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	m_uTargetID = m_pOwner->getHatredMgr().getRandomHatredID();
	return m_uTargetID ? behaviac::BT_SUCCESS : behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::selectMinHPTarget		()							//�ӳ���б���ѡȡѪ�����ٵ�Ŀ��
{
	if ( !m_pOwner )
		return behaviac::BT_FAILURE;

	m_uTargetID = 0;
	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CSceneUnit* pPreUnit= NULL;
	CHatredMgr::MAP_HATRED::iterator _pos;
	m_pOwner->getHatredMgr().m_mapHatred.getHead(_pos);
	while (!m_pOwner->getHatredMgr().m_mapHatred.isTail(_pos))
	{
		CHatredMgr::_stHatred* pHatred = _pos->second;
		if (!pHatred)
			continue;

		CSceneUnit* pUnit = pCell->findObject(pHatred->uUnitID);
		if (!pUnit)
			continue;
		if (!pPreUnit || pPreUnit->getHP() > pUnit->getHP() )
		{
			pPreUnit = pUnit;
		}
	}

	if ( !pPreUnit )
		return behaviac::BT_FAILURE;

	m_uTargetID = pPreUnit->getUnitID();
	return m_uTargetID ? behaviac::BT_SUCCESS : behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::selectNearstTarget		()
{
	if ( !m_pOwner )
		return behaviac::BT_FAILURE;

	m_uTargetID = 0;
	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CSceneUnit* pPreUnit= NULL;
	float32	fPreDistance = 0.0f;
	CHatredMgr::MAP_HATRED::iterator _pos;
	m_pOwner->getHatredMgr().m_mapHatred.getHead(_pos);
	while (!m_pOwner->getHatredMgr().m_mapHatred.isTail(_pos))
	{
		CHatredMgr::_stHatred* pHatred = _pos->second;
		if (!pHatred)
			continue;

		CSceneUnit* pUnit = pCell->findObject(pHatred->uUnitID);
		if (!pUnit)
			continue;
		if (!pPreUnit || fPreDistance > getDistance(m_pOwner->getPosition(),pUnit->getPosition()) )
		{
			pPreUnit		= pUnit;
			fPreDistance	= getDistance(m_pOwner->getPosition(),pUnit->getPosition());
		}
	}

	if ( !pPreUnit )
		return behaviac::BT_FAILURE;

	m_uTargetID = pPreUnit->getUnitID();
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::selectNearstTargetAround(float32 fRadius)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CSceneUnit* pPreUnit= NULL;
	float32	fPreDistance = 0.0f;
	MAP_GAME_UNIT _map;
	m_pOwner->getSceneCell()->gatherObject(m_pOwner->getPosition(), fRadius, _BIT32(GameObject_Monster)|_BIT32(GameObject_Player) , _map);
	auto _iter	= _map.begin();
	auto _end	= _map.end();
	for (;_iter != _end; ++_iter)
	{
		CSceneUnit* pUnit = _iter->second;
		if (!pUnit || m_pOwner == pUnit )
			continue;

		if (!m_pOwner->getHatredMgr().canHatred(pUnit))
			continue;

		if (!pPreUnit || fPreDistance > getDistance(m_pOwner->getPosition(),pUnit->getPosition()) )
		{
			pPreUnit		= pUnit;
			fPreDistance	= getDistance(m_pOwner->getPosition(),pUnit->getPosition());
		}
	}

	if ( !pPreUnit )
		return behaviac::BT_FAILURE;
	m_uTargetID = pPreUnit->getUnitID();
	if (!m_pOwner->getHatredMgr().isHatred(m_uTargetID))
	{
		m_pOwner->getHatredMgr().updateHatred(pPreUnit,1);
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::selectMonsterTarget				(uint32 uMonsterID)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;;

	CSceneUnit* pPreUnit= NULL;
	float32	fPreDistance = 0.0f;
	auto _pos = pCell->object_begin();
	auto _end = pCell->object_end();
	for ( ; _pos != _end ; ++_pos )
	{
		CSceneUnit*	pUnit = _pos->second;
		if (!pUnit)
			continue;

		CSceneMonster* pMonster = pUnit->asMonster();
		if (!pMonster || pMonster->isDead() )
			continue;

		if (uMonsterID && uMonsterID != pMonster->getMonsterID())
			continue;

		if (!pPreUnit || fPreDistance > getDistance(m_pOwner->getPosition(),pMonster->getPosition()) )
		{
			pPreUnit		= pMonster;
			fPreDistance	= getDistance(m_pOwner->getPosition(),pMonster->getPosition());
		}
	}
	if ( !pPreUnit )
		return behaviac::BT_FAILURE;
	m_uTargetID = pPreUnit->getUnitID();
	if (!m_pOwner->getHatredMgr().isHatred(m_uTargetID))
	{
		m_pOwner->getHatredMgr().updateHatred(pPreUnit,1);
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnitAI::isTargetInRange			(const _stSkillModel*	pSkillModel)
{
	if (!canOperate() || !pSkillModel || !m_uTargetID )   //�����ϲ��ᷢ������Ϊ֮ǰ�����˼��
		return false;
	//ʩ�����벻��
	if (!pSkillModel->uReleaseRange)
		return true;
	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return false;
	CSceneUnit* pUnit = pCell->findObject(m_uTargetID);
	if (!pUnit)
	{
		m_uTargetID = 0;
		return false;
	}
	float32 fReleaseRange = (float32)pSkillModel->uReleaseRange;
	if ( pUnit->isMoving() )
	{
		fReleaseRange -= 0.5;
	}
	if ( getDistance(pUnit->getPosition(), m_pOwner->getPosition()) > fReleaseRange )
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnitAI::isTargetInReleaseRange			()							//Ŀ���Ƿ���ѡ�м��ܵ���̷�Χ��
{
	return isTargetInRange(m_pSkillModel);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnitAI::isTargetInSkillRange	(uint32 uSkillID)					//Ŀ���Ƿ���ָ�����ܵ���̷�Χ��
{
	return isTargetInRange(g_clSkillData.findSkill(uSkillID,1));
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::doAttack				()							//ִ�й���
{
	if (m_pOwner&&m_pOwner->isDead())
		return behaviac::BT_SUCCESS;
	return useSelectedSkill();
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::doPatroll				()							//ִ��Ѳ�� ��������֮��ġ�����Ҫ��ϸ����
{
	if (!canOperate() )
		return behaviac::BT_FAILURE;

	if (m_pOwner->isMoving())
		return behaviac::BT_SUCCESS;

	if (m_uNextPatrolTime > getTime() )
		return behaviac::BT_SUCCESS;

	m_uNextPatrolTime	= getTime() + randRange(m_pAIModel->uPatrolTimeMax,m_pAIModel->uPatrolTimeMin);

	float32 fDisMax = MAX(m_pAIModel->fPatrolRadius,1.0f);
	float32 fDisMin = 1.0f;

	float fDistance = (float) (randRange(int32(fDisMax*100), int32(fDisMin*100)) / 100.0);
	float fDir		= getRandDir();
	_stPoint2F tar;
	getTargetPoint(m_stBirthPoint, fDir, fDistance, tar);
	setMoveDest(MoveType_Walk,tar.x, tar.y);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------
bool					CSceneUnitAI::isPathComplete			()							//·���ƶ�������
{
	if (!canOperate() )
		return false;

	CGameScene* pScene = m_pOwner->getSceneMap();
	if (!pScene)
		return false;

	const _stPoint2F* pTarPos = pScene->findRoutePoint(m_uPathID,m_uPathTarIndex);
	if (!pTarPos)
		return true;

	return false;
}
//-------------------------------------------------------------
//------------------------------
bool					CSceneUnitAI::isTargetPosComplete			()							//Ŀ����ƶ�������
{
	if (!canOperate() )
		return false;

	CGameScene* pScene = m_pOwner->getSceneMap();
	if (!pScene)
		return false;

	return getDistance(m_pOwner->getPosition(),m_stTargetPos) <= 0.5f;
}
//-------------------------------------------------------------
//------------------------------
uint32	CSceneUnitAI::getFightTime			()
{
	if (!canOperate() )
		return 0;

	return m_pOwner->getFightTime();
}
//-------------------------------------------------------------
//------------------------------
behaviac::EBTStatus		CSceneUnitAI::doPath					()
{
	if (!canOperate() )
		return behaviac::BT_FAILURE;

	setAIPoint();
	if (m_pOwner->isMoving())
		return behaviac::BT_SUCCESS;

	CGameScene* pScene = m_pOwner->getSceneMap();
	if (!pScene)
		return behaviac::BT_FAILURE;

	const _stPoint2F* pTarPos = pScene->findRoutePoint(m_uPathID,m_uPathTarIndex);
	if (!pTarPos)
		return behaviac::BT_SUCCESS;

	if (getDistance(m_pOwner->getPosition(),*pTarPos) <= 1.0f/*m_pOwner->getMoveSpeed(MoveType_Walk) * 100*/ )
	{
		++m_uPathTarIndex;

		pTarPos = pScene->findRoutePoint(m_uPathID,m_uPathTarIndex);
		if (!pTarPos)
			return behaviac::BT_SUCCESS;
	}

	setMoveDest(MoveType_Walk,pTarPos->x, pTarPos->y);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------
behaviac::EBTStatus		CSceneUnitAI::doTargetPos					()
{
	if (!canOperate() )
		return behaviac::BT_FAILURE;

	setAIPoint();
	if (m_pOwner->isMoving())
		return behaviac::BT_SUCCESS;

	CGameScene* pScene = m_pOwner->getSceneMap();
	if (!pScene)
		return behaviac::BT_FAILURE;

	setMoveDest(MoveType_Walk,m_stTargetPos.x, m_stTargetPos.y);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::doChase					()							//ִ��׷��
{
	if (!canOperate() )
		return behaviac::BT_SUCCESS;

	if (m_pAIModel->fPursueDis <= 0.01f)
		return behaviac::BT_SUCCESS;

	_stPoint2F stTargetPoint;
	if (!getCurrentTargetPos(stTargetPoint))
		return behaviac::BT_FAILURE;

	//�״�/Ŀ������䶯������Ѱ·
	if ( !m_pOwner->isMoving() || getDistance(m_stMoveDest, stTargetPoint) >= 1.0f  )
	{
		int32 iOffset = g_clConstantData.Game_AIFollowOffset;
		if (iOffset > 0)
		{
			stTargetPoint.x += (randRange(2 * iOffset) - iOffset) / 100.0f;
			stTargetPoint.y += (randRange(2 * iOffset) - iOffset) / 100.0f;
		}

		setMoveDest(MoveType_Run,stTargetPoint.x, stTargetPoint.y);
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::doReturn				()							//ִ�л���
{
	if (!canOperate() )
		return behaviac::BT_SUCCESS;

	if ( m_pOwner->isMoving() )
		return behaviac::BT_RUNNING;

	if (m_pAIModel->uIdleType == AIIdleType_Nothing || getDistance(m_stAIPoint,m_stBirthPoint) > m_pAIModel->fPatrolRadius )
		m_stAIPoint	= m_stBirthPoint;

	if ( getDistance(m_pOwner->getPosition(), m_stAIPoint) >= 0.5f )
	{
		setMoveDest(MoveType_Run,m_stAIPoint.x, m_stAIPoint.y);
		return behaviac::BT_RUNNING;
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnitAI::hasSkill				()									//�Ƿ���ѡ�м���
{
	return NULL!=m_pSkillModel;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus	CSceneUnitAI::selectSkill()							//�ȴ����ü��ܵ�cd��������ȡ����id,��֤�����߼�������Ѿ��ǿ����õ���
{
	if (!canOperate() || m_vecOrdinalSkill.empty() )
		return behaviac::BT_FAILURE;

	if (isUsingSkill())
		return behaviac::BT_FAILURE;

	static stl_vector<uint32> vec_skills;
	vec_skills.clear();
	_stAIOrdinalSkill stPreSkill;
	CCooling& clCooling = m_pOwner->getCooling();
	auto _pos = m_vecOrdinalSkill.begin();
	auto _end = m_vecOrdinalSkill.end();
	for (; _pos != _end; ++_pos)
	{
		// �Ѿ��к�ѡ���� ���ҵ�ǰ�����ȼ��Ⱥ�ѡ���ܵ����ȼ�����
		if ( stPreSkill.uSkillID && _pos->uPriority < stPreSkill.uSkillID )
			break;

		// ������ȴ��
		if (clCooling.isCooling(CoolingType_Skill, _pos->uSkillID, SKILL_GOOLING_GROUP_ID))
			continue;

		stPreSkill = *_pos;
		vec_skills.push_back(stPreSkill.uSkillID);
	}

	uint32 nSize = (uint32)vec_skills.size();
	if (nSize<=0)
		return behaviac::BT_FAILURE;

	uint32 nIndex = 0;
	if (nSize > 1 )
	{
		nIndex = randRange((uint32)(nSize - 1) ,(uint32)0);
		if (nIndex >= nSize )
			nIndex = 0;
	}

	m_pSkillModel = g_clSkillData.findSkill(vec_skills[nIndex],1);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::setSelectSkill			(uint32 uSkillID)
{
	m_pSkillModel = g_clSkillData.findSkill(uSkillID,1);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnitAI::isSkillCooling			(uint32 uSkillID)						//�ü����Ƿ�����ȥ��
{
	if (!canOperate() )
		return false;

	CCooling& clCooling = m_pOwner->getCooling();
	// ������ȴ��
	return clCooling.isCooling(CoolingType_Skill, uSkillID, SKILL_GOOLING_GROUP_ID);
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::startSpecialCD(uint32 uIndex, uint32 uSecondCD) //����ָ����CD��ʼ��ȴ
{
	uint64	currentTick = getSystemTick64();
	m_mapSpecialCD[uIndex] = currentTick + uSecondCD;
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnitAI::checkSpecialCD(uint32 uIndex) //�鿴ָ����CD��û����ȴ���
{
	uint64	currentTick = getSystemTick64();
	return currentTick >= m_mapSpecialCD[uIndex];
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnitAI::checkAndStartSpecialCD(uint32 uIndex, uint32 uSecondCD) //�鿴ָ���Ĳ���û��cd����,�������㷵��true�������²��ڵ�ʱ���
{
	if ( !checkSpecialCD(uIndex))
		return false;

	startSpecialCD(uIndex,uSecondCD);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::setTime					(uint32 uIndex,uint32 uMS,bool bRuning)	
{
	uint64	currentTick = getSystemTick64();
	uint64&	uEndTick = m_mapSpecialTimer[uIndex];
	if ( !uEndTick )
		uEndTick = currentTick + uMS;

	if ( currentTick >= uEndTick  )
		return behaviac::BT_SUCCESS;

	if (bRuning)
		return behaviac::BT_RUNNING;

	return behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::resetTime				(uint32 uIndex)
{
	m_mapSpecialTimer[uIndex] = 0;
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::doNothing				()						//ʲô��������
{
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneUnitAI::callbackUseSkillFailedByBlock(uint32 uSkillID)
{
	m_uFailedSkillIdByBlock = uSkillID;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnitAI::callbackByAttack				(CSceneUnit* pUnit,int32 iDamage)
{
	m_uLastBeAttackTick = getSystemTick64();
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::checkSkillFailedAndWait		(uint32 uSkillID)			//����Ƿ��м�����Ϊ�赲�������û���ͷųɹ���
{
	if (isUsingSkill())
		return behaviac::BT_RUNNING;

	if (uSkillID == m_uFailedSkillIdByBlock )
	{
		m_uFailedSkillIdByBlock = 0;
		return behaviac::BT_SUCCESS;
	}
	return behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::addSceneArea		(uint32 uAreaID)								//���������е���������
{
	if ( !canOperate() )
		return behaviac::BT_SUCCESS;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;

	//�����赲��.ͬ���赲����Ϣ��ǰ̨
	pCell->addArea(uAreaID);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::delSceneArea		(uint32 uAreaID)								//�رճ����е���������
{
	if ( !canOperate() )
		return behaviac::BT_SUCCESS;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;

	//�����赲��.ͬ���赲����Ϣ��ǰ̨
	pCell->delArea(uAreaID);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::killMonsterById			(uint32 uMonsterId, uint32 uCount, float32 fRadius)		//ɱ�����ָ���Ĺ��
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;
	MAP_GAME_UNIT _map;
	_map.clear();
	_stPoint2F _st = m_pOwner->getPosition();
	pCell->gatherObject(_st, fRadius, _BIT32(GameObject_Monster), _map);
	MAP_GAME_UNIT::iterator iter = _map.begin();
	MAP_GAME_UNIT::iterator iter_end = _map.end();
	for (;uCount>0 && iter != iter_end; ++iter)
	{
		CSceneUnit* pUnit = iter->second;
		if (!pUnit)
			continue;

		CSceneMonster* pMonster = pUnit->asMonster();
		if (!pMonster || pMonster->isDead() )
			continue;

		const _stMonsterModel* pMonsterModel = pMonster->getMonsterModel();
		if ( !pMonsterModel || pMonsterModel->uMonsterID != uMonsterId )
			continue;

		//ɱ���������
		uCount--;
		pMonster->getGameBuffMgr().clear();
		pMonster->updateHP( -pMonster->getHP() ,m_pOwner , DoingType_AI);
	} 
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::removeMonsterById			(uint32 uMonsterId, uint32 uCount, float32 fRadius)		//ɱ�����ָ���Ĺ��
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;
	MAP_GAME_UNIT _map;
	_map.clear();
	_stPoint2F _st = m_pOwner->getPosition();
	pCell->gatherObject(_st, fRadius, _BIT32(GameObject_Monster), _map);
	MAP_GAME_UNIT::iterator iter = _map.begin();
	MAP_GAME_UNIT::iterator iter_end = _map.end();
	for (;uCount>0 && iter != iter_end; ++iter)
	{
		CSceneUnit* pUnit = iter->second;
		if (!pUnit)
			continue;

		CSceneMonster* pMonster = pUnit->asMonster();
		if (!pMonster)
			continue;

		const _stMonsterModel* pMonsterModel = pMonster->getMonsterModel();
		if ( !pMonsterModel || pMonsterModel->uMonsterID != uMonsterId )
			continue;

		//ɾ���������
		uCount--;
		pCell->leaveScene(pUnit);
	} 
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::killSelf			()
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	if (m_pOwner->isDead())
		return behaviac::BT_FAILURE;

	m_pOwner->getGameBuffMgr().clear();
	m_pOwner->updateHP( -m_pOwner->getHP() ,m_pOwner , DoingType_AI);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::removeSelf			()
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;

	pCell->leaveScene(m_pOwner);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::speak(uint32 uSpeakID)		//˵��
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CSceneMonster* pMonster = m_pOwner->asMonster();
	if (pMonster)
	{
		pMonster->speak(uSpeakID);
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus CSceneUnitAI::changeCamp(uint8 uNewCampID)			//�ı���Ӫ
{
	if (m_pOwner != NULL && m_pOwner->getCampID() != uNewCampID)
	{
		m_pOwner->setCampID(uNewCampID);
	}

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
uint8 CSceneUnitAI::getCamp()			//�����Ӫ
{
	return m_pOwner->getCampID();
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::setAIPoint			()							//����ǰ��������AIս�����ĵ� CSceneUnitAI::setAIPoint()
{
	if (!canOperate())
		return behaviac::BT_SUCCESS;

	m_stAIPoint = m_pOwner->getPosition();
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus	CSceneUnitAI::scanHatredObj()
{//���������龯�䷶Χ�ڵĵ���,���Ҽ��뵽����б� (�ú��� ��Զ���سɹ�)
	if (!canOperate())
		return behaviac::BT_FAILURE;

	if (m_pOwner->getHatredMgr().hasHatred())
		return behaviac::BT_SUCCESS;

	if ( m_pAIModel->fFightRadius <= 0.01  )
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	MAP_GAME_UNIT _map;
	m_pOwner->getSceneCell()->gatherObject(m_pOwner->getPosition(), m_pAIModel->fFightRadius, _BIT32(GameObject_Monster)|_BIT32(GameObject_Player) , _map);
	MAP_GAME_UNIT::iterator _iter	= _map.begin();
	MAP_GAME_UNIT::iterator _end	= _map.end();
	for (;_iter != _end; ++_iter)
	{
		CSceneUnit* pUnit = _iter->second;
		if (!pUnit || m_pOwner == pUnit || getDistance(m_pOwner->getPosition(),pUnit->getPosition()) >= m_pAIModel->fHatredRadius)
			continue;

		m_pOwner->getHatredMgr().onEnterFightDis(pUnit);
		if (m_pOwner->getHatredMgr().hasHatred())
			return behaviac::BT_SUCCESS;
	}
	return behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnitAI::canOperate			()
{
	return m_pOwner&&m_pAIModel;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnitAI::hasTarget				()									//����Ƿ��й����������Ŀǰ�У�ҲҪ����Ƿ�Ϸ�
{
	return 0!=m_uTargetID;
}
//-------------------------------------------------------------
//------------------------------ 
float32	CSceneUnitAI::getTargetDistance		()									//����뵱ǰĿ��ľ���
{
	if (!m_pOwner || !m_uTargetID)
		return 0.0f;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return 0.0f;

	CSceneUnit* pTarget = pCell->findObject(m_uTargetID);
	if (!pTarget)
		return 0.0f;

	return getDistance(m_pOwner->getPosition(), pTarget->getPosition());
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnitAI::canMove					()								//�Ƿ���ƶ�
{
	return m_pOwner&&m_pOwner->canMove();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnitAI::canUseSkill				()							//�Ƿ��ʹ�ü���
{
	return m_pOwner&&m_pOwner->canUseSkill(NULL);
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::change2IdleState			()						//ת��������״̬
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CSceneMonster* pMonster = m_pOwner->asMonster();
	if (!pMonster)
		return behaviac::BT_FAILURE;
	
	CCopymapBase* pCopymapBase = pCell->getGameCopymap();
	if (pCopymapBase)
		pCopymapBase->clearMonsterArea(pMonster);
	
	return changeState(AIState_Idle);
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::change2BattleState()								//ת����ս��״̬
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CSceneMonster* pMonster = m_pOwner->asMonster();
	if (!pMonster)
		return behaviac::BT_FAILURE;

	CCopymapBase* pCopymapBase = pCell->getGameCopymap();
	if (pCopymapBase)
		pCopymapBase->callback_callAllPlayer(pMonster);
	
	return changeState(AIState_Battle);
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::change2ReturnState()								//ת����ս��״̬
{
	return changeState(AIState_Return);
}
//
//-------------------------------------------------------------
//------------------------------ 
bool					CSceneUnitAI::isIdleState				()							//�ж��Ƿ�������״̬
{
	return AIState_Idle == m_eAIStateType;
}
//-------------------------------------------------------------
//------------------------------ 
bool					CSceneUnitAI::isBattleState			()							//�ж��Ƿ���ս��״̬
{
	return AIState_Battle == m_eAIStateType;
}
//-------------------------------------------------------------
//------------------------------ 
bool					CSceneUnitAI::isReturnState			()							//�ж��Ƿ���ս��״̬
{
	return AIState_Return == m_eAIStateType;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::doIdle					()						//ִ�в߻����õ�������Ϊ
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	switch (m_pAIModel->uIdleType)
	{
	case AIIdleType_Patroll:
		{
			return doPatroll();
		}
		break;
	case AIIdleType_Path:
		{
			return doPath();
		}
		break;
	case AIIdleType_TargetPos:
		{
			return doTargetPos();
		}
		break;
	case AIIdleType_Nothing:
		{
			if (m_fFirstDirection!=m_pOwner->getDirection())
			{
				m_pOwner->setDirectionAndSync(m_fFirstDirection);
			}
			return doNothing();
		}
		break;
	}
	return behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::changeState				(_emAIStateType eState)
{
	endMove();
	m_eAIStateType = eState;
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::clearTarget				()
{
	m_uTargetID = 0;
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::addMonster_fixPos		(uint32 uMonsterID,uint32 uCount,float32 fPosX,float32 fPosY,float32 fAngleDir)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;

	uCount = MAX((uint32)1, uCount);
	if ( uCount > 1000 )
		uCount = 1000;

	const _stMonsterModel*pModel = g_clMonsterData.find(uMonsterID);
	if (!pModel)
		return behaviac::BT_FAILURE;

	for ( uint32 i = 0 ; i < uCount ;++i )
	{
		pCell->createMonster(pModel,fPosX,fPosY,fAngleDir< 0.0f ? getRandDir() : getAngleToRadian(fAngleDir), 0, true);
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::addMonster_relatedPos	(uint32 uMonsterID,uint32 uCount,float32 fRelatedX,float32 fRelatedY,float32 fAngleDir)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;

	uCount = MAX((uint32)1, uCount);
	if ( uCount > 1000 )
		uCount = 1000;

	const _stMonsterModel*pModel = g_clMonsterData.find(uMonsterID);
	if (!pModel)
		return behaviac::BT_FAILURE;

	_stPoint2F stTar = m_pOwner->getPosition();
	stTar.x += fRelatedX;
	stTar.y += fRelatedY;

	_stPoint2F stRealTar = stTar;
	m_pOwner->getUnitTargetPoint(stTar,stRealTar);
	for ( uint32 i = 0 ; i < uCount ;++i )
	{
		pCell->createMonster(pModel,stRealTar.x,stRealTar.y,fAngleDir< 0.0f ? getRandDir() : getAngleToRadian(fAngleDir), 0, true);
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::addMonster_randomPos	(uint32 uMonsterID,uint32 uCount,float32 fMinRadius,float32 fMaxRadius,float32 fAngleDir)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;

	uCount = MAX((uint32)1, uCount);
	if ( uCount > 1000 )
		uCount = 1000;

	const _stMonsterModel*pModel = g_clMonsterData.find(uMonsterID);
	if (!pModel)
		return behaviac::BT_FAILURE;

	_stPoint2F stCenter = m_pOwner->getPosition();
	for ( uint32 i = 0 ; i < uCount ;++i )
	{
		float fDir = getRandDir();
		float fDistance = (float) (randRange(int32(fMaxRadius*100), int32(fMinRadius*100) ) / 100.0);
		_stPoint2F stTestPos;
		::getTargetPoint(stCenter, fDir, fDistance, stTestPos);
		_stPoint2F stReadPos = stCenter;
		m_pOwner->getUnitTargetPoint(stTestPos,stReadPos);
		pCell->createMonster(pModel,stReadPos.x,stReadPos.y,fAngleDir< 0.0f ? getRandDir() : getAngleToRadian(fAngleDir), 0, true);
	}

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::changeDirection			(uint32 uAngle)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	m_pOwner->setDirectionAndSync( getAngleToRadian((float32)uAngle) );
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::askForHelpAround		(uint32 uMonsterID,float32 fRadius)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	if(getDistance(m_pOwner->getPosition(), m_stAIPoint) > m_pAIModel->fPursueDis )
		return behaviac::BT_SUCCESS;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;

	CSceneUnit* pTarget = m_uTargetID ? pCell->findObject(m_uTargetID) : NULL;
	if (!pTarget)
		return behaviac::BT_SUCCESS;

	MAP_GAME_UNIT _map;
	_map.clear();
	_stPoint2F _st = m_pOwner->getPosition();
	pCell->gatherObject(_st, fRadius, _BIT32(GameObject_Monster), _map);
	MAP_GAME_UNIT::iterator iter = _map.begin();
	MAP_GAME_UNIT::iterator iter_end = _map.end();
	for (;iter != iter_end; ++iter)
	{
		CSceneUnit* pUnit = iter->second;
		if (!pUnit)
			continue;

		CSceneMonster* pMonster = pUnit->asMonster();
		if (!pMonster || pMonster->isDead() )
			continue;

		if (pMonster->getCampID() != m_pOwner->getCampID())
			continue;

		if (uMonsterID && uMonsterID != pMonster->getMonsterID())
			continue;

		if (!pMonster->getHatredMgr().hasHatred())
			pMonster->getHatredMgr().onEnterFightDis(pTarget);
	}

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::askForHelp				(uint32 uMonsterID)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;

	CSceneUnit* pTarget = m_uTargetID ? pCell->findObject(m_uTargetID) : NULL;
	if (!pTarget)
		return behaviac::BT_SUCCESS;

	auto _pos = pCell->object_begin();
	auto _end = pCell->object_end();
	for ( ; _pos != _end ; ++_pos )
	{
		CSceneUnit*	pUnit = _pos->second;
		if (!pUnit)
			continue;

		CSceneMonster* pMonster = pUnit->asMonster();
		if (!pMonster || pMonster->isDead() )
			continue;

		if (pMonster->getCampID() != m_pOwner->getCampID())
			continue;

		if (uMonsterID && uMonsterID != pMonster->getMonsterID())
			continue;

		if (!pMonster->getHatredMgr().hasHatred())
			pMonster->getHatredMgr().onEnterFightDis(pTarget);
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::sendMessageAround		(uint32 uIndex,uint32 uMonsterID,float32 fRadius)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;
	static MAP_GAME_UNIT _map;
	_map.clear();
	_stPoint2F _st = m_pOwner->getPosition();
	pCell->gatherObject(_st, fRadius, _BIT32(GameObject_Monster), _map);
	MAP_GAME_UNIT::iterator iter = _map.begin();
	MAP_GAME_UNIT::iterator iter_end = _map.end();
	for (;iter != iter_end; ++iter)
	{
		CSceneUnit* pUnit = iter->second;
		if (!pUnit)
			continue;

		CSceneMonster* pMonster = pUnit->asMonster();
		if (!pMonster || pMonster->isDead() )
			continue;

		if (uMonsterID && uMonsterID != pMonster->getMonsterID())
			continue;

		CSceneUnitAI* pAI = pMonster->getAI();
		if (!pAI)
			continue;

		pAI->m_setMessage.insert(uIndex);
	} 
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::sendMessage				(uint32 uIndex,uint32 uMonsterID)
{
	if (!canOperate())
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_SUCCESS;

	auto _pos = pCell->object_begin();
	auto _end = pCell->object_end();
	for ( ; _pos != _end ; ++_pos )
	{
		CSceneUnit*	pUnit = _pos->second;
		if (!pUnit)
			continue;

		CSceneMonster* pMonster = pUnit->asMonster();
		if (!pMonster || pMonster->isDead() )
			continue;

		if (uMonsterID && uMonsterID != pMonster->getMonsterID())
			continue;

		CSceneUnitAI* pAI = pMonster->getAI();
		if (!pAI)
			continue;

		pAI->m_setMessage.insert(uIndex);
	}
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus	CSceneUnitAI::readMessage				(uint32 uIndex,bool bDelete)
{
	if (m_setMessage.find(uIndex)==m_setMessage.end())
		return behaviac::BT_FAILURE;

	if (bDelete)
		m_setMessage.erase(uIndex);

	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::targetAddBuff					(uint32 uBuffID, uint8 uLevel)
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CSceneUnit* pUnit = pCell->findObject(m_uTargetID);
	if (!pUnit)
		return behaviac::BT_FAILURE;

	pUnit->getGameBuffMgr().addBuff(uBuffID, uLevel, m_pOwner->getUnitID(),0, DoingType_AI);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::targetDelBuff					(uint32 uBuffID)
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CSceneUnit* pUnit = pCell->findObject(m_uTargetID);
	if (!pUnit)
		return behaviac::BT_FAILURE;

	pUnit->getGameBuffMgr().delBuff(uBuffID, DoingType_AI);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::selectBuffTarget		(uint32 uBuffID)
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	m_uTargetID	 = 0;
	auto _pos = pCell->object_begin();
	auto _end = pCell->object_end();
	for ( ; _pos != _end ; ++_pos )
	{
		CSceneUnit*	pUnit = _pos->second;
		if (!pUnit)
			continue;

		if (pUnit->getGameBuffMgr().findFirst(uBuffID))
		{
			m_uTargetID = pUnit->getUnitID();
			break;
		}
	}
	return m_uTargetID ? behaviac::BT_SUCCESS : behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::playerScanTarget		()												//ǰ�� �Զ�ս���õ�
{
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------ 
behaviac::EBTStatus		CSceneUnitAI::playerSelectTarget		()													//ǰ�� �Զ�ս���õ�
{
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------
behaviac::EBTStatus	CSceneUnitAI::moveBack				(float32 fDis)										//��Ŀ��ķ������ƶ����õľ���
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CSceneUnit* pUnit = pCell->findObject(m_uTargetID);
	if (!pUnit)
	{
		endMove();
		return behaviac::BT_SUCCESS;
	}

	float32 fCurDis = getDistance(m_pOwner->getPosition(), pUnit->getPosition());
	if ( fCurDis >= fDis )
	{
		endMove();
		return behaviac::BT_SUCCESS;
	}

	float32 fMoveDis = fDis - fCurDis;
	if (fMoveDis <= 0.3f )
	{
		endMove();
		return behaviac::BT_SUCCESS;
	}

	float32 fMoveDir = get2PiRadian(pUnit->getPosition(),m_pOwner->getPosition());
	_stPoint2F _stTarPos;
	m_pOwner->getUnitTargetPoint(fMoveDir,fMoveDis,_stTarPos);
	if ( !isMoving() || getDistance(_stTarPos,m_stMoveDest) >= 0.5f )
	{
		setMoveDest(MoveType_Run,_stTarPos.x, _stTarPos.y);
	}
	return behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------
behaviac::EBTStatus	CSceneUnitAI::moveToTarget				(float32 fDis)										//�ƶ������뵱ǰĿ��ָ�������ڵ�λ��
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CSceneUnit* pUnit = pCell->findObject(m_uTargetID);
	if (!pUnit)
	{
		endMove();
		return behaviac::BT_SUCCESS;
	}

	float32 fCurDis = getDistance(m_pOwner->getPosition(), pUnit->getPosition());
	if ( fCurDis <= fDis || fCurDis <= 0.3f )
	{
		endMove();
		return behaviac::BT_SUCCESS;
	}

	if ( !isMoving() || getDistance(pUnit->getPosition(),m_stMoveDest) >= 0.5f )
	{
		setMoveDest(MoveType_Run,pUnit->getPosition().x, pUnit->getPosition().y);
	}
	return behaviac::BT_FAILURE;
}
//-------------------------------------------------------------
//------------------------------
behaviac::EBTStatus	CSceneUnitAI::faceTarget				()
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	endMove();
	CSceneUnit* pUnit = pCell->findObject(m_uTargetID);
	if (!pUnit)
		return behaviac::BT_SUCCESS;

	float32 fFaceDir = get2PiRadian(m_pOwner->getPosition(),pUnit->getPosition());
	m_pOwner->setDirectionAndSync(fFaceDir);
	return behaviac::BT_SUCCESS;
}
//-------------------------------------------------------------
//------------------------------
behaviac::EBTStatus		CSceneUnitAI::changeCopymapTask		(uint32 uQuestID)						//�л����������֧
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CCopymapBase* pCopymapBase = pCell->getGameCopymap();
	if (!pCopymapBase)
		return behaviac::BT_FAILURE;

	CCopymapNormal* pCopymapNormal =  (CCopymapNormal*)pCopymapBase;
	if (!pCopymapNormal)
		return behaviac::BT_FAILURE;

	pCopymapNormal->changeCopymapTask(uQuestID);
	return behaviac::BT_SUCCESS;
}

//-------------------------------------------------------------
//------------------------------�رո���
behaviac::EBTStatus		CSceneUnitAI::closeCopymap		(uint32 uTime, uint32 uChatID)						
{
	if (!m_pOwner)
		return behaviac::BT_FAILURE;

	CGameSceneCell* pCell = m_pOwner->getSceneCell();
	if (!pCell)
		return behaviac::BT_FAILURE;

	CCopymapBase* pCopymapBase = pCell->getGameCopymap();
	if (!pCopymapBase)
		return behaviac::BT_FAILURE;

	pCopymapBase->callback_AICloseCopymap(getTime()+uTime, uChatID);

	return behaviac::BT_SUCCESS;
}