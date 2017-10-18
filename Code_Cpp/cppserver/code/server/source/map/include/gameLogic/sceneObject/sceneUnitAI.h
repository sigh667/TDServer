/*----------------- sceneUnitAI.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/1/26 11:30:25
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "behaviac/behaviac.h"
#include "math/point2.h"
#include "stl/stl_list.h"
#include "protocol/GameDefine.pb.h"
#include "cfg_data/monsterData/monsterModel.h"
class CSceneUnit;
struct _stSkillModel;
struct _stAIModel;
/*************************************************************/
class CSceneUnitAI: public behaviac::Agent
{
private:
	typedef stl_map<int32,int32>			MAP_I32_I32;
	typedef stl_map<int32,uint64>			MAP_I32_U64;
	typedef	stl_vector<_stAIOrdinalSkill>	VEC_SKILL;
	typedef stl_set<uint32>					SET_U32;

	//����ָ��
	CSceneUnit*				m_pOwner;

	// ai ���ò���
	const _stAIModel*		m_pAIModel;				//ai����ģ��
	VEC_SKILL				m_vecOrdinalSkill;		//ͨ�ü����б� �����ȼ�������

	// ����ʱ�ı���
	_emAIStateType			m_eAIStateType;			//ai״̬
	uint32					m_uFreeVar;				//��¶���ű�ʹ�õ����ɱ���
	MAP_I32_I32				m_mapProperty;			//�Զ�������
	SET_U32					m_setMessage;			//���ܵ�����Ϣ
	uint32					m_uRunAIFrameCount;		//����aiִ��Ƶ�ʵ�
	uint32					m_uAICallFrequency;		//aiִ��Ƶ�ʵ�
	uint32					m_uNextPatrolTime;		//��һ�ο�����ƶ�ʱ�� ����Ѳ�ߵ�
	uint32					m_uTargetID;			//��ǰĿ��ID
	const _stSkillModel*	m_pSkillModel;			//��ǰѡ�м���ģ��
	_stPoint2F				m_stMoveDest;			//�ƶ��ռ�Ŀ���
	_stPoint2F				m_stMoveBeginPos;		//��ǰֱ���ƶ�����ʼ����
	float32					m_fCurrentMoveDir;		//��ǰ��Ҫֱ���ƶ��ķ���
	float32					m_fCurrentMoveDis;		//��ǰ��Ҫֱ���ƶ��ľ���
	uint8					m_uMoveType;
	_stPoint2F				m_stBirthPoint;			//������
	_stPoint2F				m_stAIPoint;			//AIս�����ĵ�
	MAP_I32_U64				m_mapSpecialCD;			//���ⶨ��cd
	MAP_I32_U64				m_mapSpecialTimer;		//���ⶨʱ��
	uint32					m_uFailedSkillIdByBlock;//��Ϊ�������赲��ʧ�ܵļ���id
	float32					m_fFirstDirection;		//��ʼ����

	uint32					m_uPathID;				//��ǰ·��ID
	uint32					m_uPathTarIndex;		//��ǰ·���е�Ŀ������

	_stPoint2F				m_stTargetPos;			//��ǰĿ���

	uint32					m_uSelectTargetType;	//��ǰ���ģʽ
	uint32					m_uHatredSelectOrder;	//ѡ��ڼ����Ŀ��

	uint64					m_uLastBeAttackTick;

	bool					m_bStop;
public:
	behaviac::EBTStatus		change2IdleState		();							//ת��������״̬
	behaviac::EBTStatus		change2BattleState		();							//ת����ս��״̬
	behaviac::EBTStatus		change2ReturnState		();							//ת��������״̬

	bool					isIdleState				();							//�ж��Ƿ�������״̬
	bool					isBattleState			();							//�ж��Ƿ���ս��״̬
	bool					isReturnState			();							//�ж��Ƿ��ڻ���״̬

	behaviac::EBTStatus		doIdle					();							//ִ�в߻����õ�������Ϊ
private:
	behaviac::EBTStatus		changeState				(_emAIStateType eState);
public://״̬���
	//Ҷ�ӽڵ㣬ִ�о����ָ���ʵӦ��Ҫϸ���ġ�
	behaviac::EBTStatus		doPatroll				();							//ִ��Ѳ��
	behaviac::EBTStatus		doPath					();							//ִ��·�����ƶ�
	behaviac::EBTStatus		doTargetPos				();							//ִ��Ŀ����ƶ�
	behaviac::EBTStatus		doNothing				();							//ʲô������
	behaviac::EBTStatus		doAttack				();							//ִ�й���
	behaviac::EBTStatus		doChase					();							//ִ��׷��
	behaviac::EBTStatus		doReturn				();							//ִ�л���
public://�Զ����������
	uint32					getFreeVar				()			{return m_uFreeVar;};		//������ɱ���
	void					setFreeVar				(uint32 t)	{m_uFreeVar = t;};			//�������ɱ���

	int32					getProperty				(int32 iKey);							//��ȡ�Զ�������(Ĭ��ֵΪ0)
	void					setProperty				(int32 iKey, int32 iValue);				//�����Զ�������
	void					clearProperty			();										//����Զ��������б�
public://Ŀ�����
	behaviac::EBTStatus		clearTarget				();										//���Ŀ��
	bool					hasTarget				();										//����Ƿ��й�������
	behaviac::EBTStatus		selectTarget			();										//������ѡȡĿ��
	behaviac::EBTStatus		selectHatredTarget		(uint32 uHatredOrder);					//�ӳ���б���ѡȡָ�����ֵ�����Ŀ��
	behaviac::EBTStatus		selectFirstHatredTarget	();										//�ӳ���б���ѡȡ�����ߵ�Ŀ��
	behaviac::EBTStatus		selectRandTarget		();										//�ӳ���б������ѡ��Ŀ��
	behaviac::EBTStatus		selectMinHPTarget		();										//�ӳ���б���ѡȡѪ�����ٵ�Ŀ��
	behaviac::EBTStatus		selectNearstTarget		();										//�ӳ���б���ѡȡ������С��Ŀ��
	behaviac::EBTStatus		selectBuffTarget		(uint32 uBuffID);						//ѡ��Я���ض�buff��Ŀ��
	behaviac::EBTStatus		scanHatredObj			();										//��龯�䷶Χ�ڵĵ���,���Ҽ������б�,
	behaviac::EBTStatus		selectNearstTargetAround(float32 fRadius);						//���ӳ�޻��Ʋ��Ҳ��������Ŀ��
	behaviac::EBTStatus		selectMonsterTarget		(uint32 uMonsterID);					//���ӳ�޲���ָ��������ΪĿ��
	float32					getTargetDistance		();										//����뵱ǰĿ��ľ���
	bool					isTargetInReleaseRange	();										//Ŀ���Ƿ���ѡ�м��ܵ���̷�Χ��
	bool					isTargetInSkillRange	(uint32 uSkillID);						//Ŀ���Ƿ���ָ�����ܵ���̷�Χ��
	behaviac::EBTStatus		setSelectTargetType		(uint32 uType);							//����Ŀ��ѡȡ����
	behaviac::EBTStatus		setHatredSelectOrder	(uint32 uOrder);						//���ó�޵���Ĺֵ�ѡ����ֵ������
	behaviac::EBTStatus		changeCopymapTask		(uint32 uQuestID);						//�л����������֧ closeCopymap
	behaviac::EBTStatus		closeCopymap			(uint32 uTime, uint32 uChatID);			//�رո��� 
private:
	bool					isTargetInRange			(const _stSkillModel*	pSkillModel);
public://�������
	behaviac::EBTStatus		selectSkill				();										//������ʹ�����ȼ�ѡȡ��ǰҪʹ�õļ���
	behaviac::EBTStatus		setSelectSkill			(uint32 uSkillID);						//ָ����ǰҪʹ�õļ���
	behaviac::EBTStatus		useSkill				(uint32 uSkillID, uint8 uLevel);		//ʹ��ָ������,��������ѡȡĿ��
	behaviac::EBTStatus		useSelectedSkill		();										//ʹ�õ�ǰѡ�м���,��ǰû��ѡ�м��ܵĻ����Զ�ѡȡһ�����ü���,��������ѡȡĿ��
	bool					hasSkill				();										//�Ƿ���ѡ�м���
	bool					isSkillCooling			(uint32 uSkillID);						//�ü����Ƿ�����ȥ��

	behaviac::EBTStatus		addSkill				(uint32 uSkillID,uint32 uPriority);		//���ӳ��漼��
	behaviac::EBTStatus		delSkill				(uint32 uSkillID);						//ɾ�����漼��
	behaviac::EBTStatus		resetSkill				();										//���ó��漼��

	behaviac::EBTStatus		startSpecialCD			(uint32 uIndex, uint32 uMS);			//����ָ����CD��ʼ��ȴ
	bool					checkSpecialCD			(uint32 uIndex);						//�鿴ָ����CD��û����ȴ���
	bool					checkAndStartSpecialCD	(uint32 uIndex, uint32 uMS);			//�鿴ָ����CD��û����ȴ��ɣ�������ȴ�з��ؼ٣��Ѿ������ȴ�����沢������ȴ

	behaviac::EBTStatus		clearBuff				();								//���Buff
	behaviac::EBTStatus		clearHatred				();								//��ճ���б�
	behaviac::EBTStatus		clearTargetHatred		();								//�����ǰĿ��ĳ��
	behaviac::EBTStatus		clearHurt				();								//����˺��б�
	behaviac::EBTStatus		addBuff					(uint32 uBuffID, uint8 uLevel);	//����buff
	behaviac::EBTStatus		delBuff					(uint32 uBuffID);				//ɾ��buff
	behaviac::EBTStatus		targetAddBuff			(uint32 uBuffID, uint8 uLevel);	//��Ŀ������buff
	behaviac::EBTStatus		targetDelBuff			(uint32 uBuffID);				//��Ŀ��ɾ��buff
public:// �������
	bool					hasHatredObj			();								//�Ƿ��г�޶���
	bool					canMove					();								//�Ƿ���ƶ�
	bool					canUseSkill				();								//�Ƿ��ʹ�ü���
	bool					isFighting				();								//�Ƿ���ս����
	bool					isMoving				();								//�Ƿ��ƶ���
	bool					isUsingSkill			();								//�Ƿ�ʹ�ü�����
	bool					isBeAttack				(uint32 uIntervalMS);			//�ж����ʱ������û�б�����

	uint32					getHPPercent			();								//��ȡ��ǰѪ���ٷֱ�
	uint8					getCamp					();								//�����Ӫ
	behaviac::EBTStatus		changeCamp				(uint8 uNewCampID);				//�ı���Ӫ

	bool					isPathComplete			();								//·�����ƶ�������
	bool					isTargetPosComplete		();								//Ŀ����ƶ�������

	uint32					getFightTime			();								//����ս��ʱ��
public:// ���⹦�����
	behaviac::EBTStatus		killMonsterById			(uint32 uMonsterId, uint32 uCount, float32 fRadius);	//ɱ�����ָ��id�Ĺ���
	behaviac::EBTStatus		removeMonsterById		(uint32 uMonsterId, uint32 uCount, float32 fRadius);	//ɾ�����ָ��id�Ĺ���
	behaviac::EBTStatus		killSelf				();											//��ɱ
	behaviac::EBTStatus		removeSelf				();											//�Լ���ʧ
	behaviac::EBTStatus		addSceneArea			(uint32 uAreaID);							//���������е���������
	behaviac::EBTStatus		delSceneArea			(uint32 uAreaID);							//�رճ����е���������
	behaviac::EBTStatus		checkSkillFailedAndWait	(uint32 uSkillID);							//����Ƿ��м�����Ϊ�赲�������û���ͷųɹ��������������
	behaviac::EBTStatus		speak					(uint32 uSpeakID);							//˵��
	behaviac::EBTStatus		setAIPoint				();											//����ǰ��������AIս�����ĵ�
	behaviac::EBTStatus		setBirthPoint			();											//����ǰ��������Ϊ������
	behaviac::EBTStatus		setAI					(uint32 uID);								//����AI
	behaviac::EBTStatus		stopAI					(bool bStop);								//ֹͣAI ǰ�����AI����
	behaviac::EBTStatus		setPathID				(uint32 uPathID) ;							//����·��ID
	behaviac::EBTStatus		setTargetPos			(float32 fPosX,float32 fPosY) ;				//����Ŀ���
	uint32					getPlayerCountRange		(float32 fRadius);							//��ȡ��Χ���������
	float32					getDisFromAIPoint		();											//�����ս�����ĵ�ľ���
	behaviac::EBTStatus		questComplete			() ;										//���ɱ������
	behaviac::EBTStatus		setTime					(uint32 uIndex,uint32 uMS,bool bRuning);	//���ö�ʱ��
	behaviac::EBTStatus		resetTime				(uint32 uIndex);							//ִ��׷��
	behaviac::EBTStatus		addMonster_fixPos		(uint32 uMonsterID,uint32 uCount,float32 fPosX,float32 fPosY,float32 fAngleDir);				//�̶�λ��ˢ��
	behaviac::EBTStatus		addMonster_relatedPos	(uint32 uMonsterID,uint32 uCount,float32 fRelatedX,float32 fRelatedY,float32 fAngleDir);		//���λ��ˢ��
	behaviac::EBTStatus		addMonster_randomPos	(uint32 uMonsterID,uint32 uCount,float32 fMinRadius,float32 fMaxRadius,float32 fAngleDir);	//���λ��ˢ��
	behaviac::EBTStatus		changeDirection			(uint32 uAngle);							//�ı䳯��
	behaviac::EBTStatus		sendMessageAround		(uint32 uIndex,uint32 uMonsterID,float32 fRadius);	//����Χ������Ϣ
	behaviac::EBTStatus		sendMessage				(uint32 uIndex,uint32 uMonsterID);					//������Ϣ
	behaviac::EBTStatus		readMessage				(uint32 uIndex,bool bDelete);						//��ȡ��Ϣ
	behaviac::EBTStatus		askForHelpAround		(uint32 uMonsterID,float32 fRadius);				//��ָ����Χ��Ԯ
	behaviac::EBTStatus		askForHelp				(uint32 uMonsterID);								//��Ԯ
	behaviac::EBTStatus		moveBack				(float32 fDis);										//��Ŀ��ķ������ƶ����õľ���
	behaviac::EBTStatus		moveToTarget			(float32 fDis);										//�ƶ������뵱ǰĿ��ָ�������ڵ�λ��
	behaviac::EBTStatus		faceTarget				();													//��������������Ŀ��
public:
	CSceneUnitAI();
	virtual ~CSceneUnitAI();

	static CSceneUnitAI*	createSceneUnitAI	(CSceneUnit* pOwner,uint32 uAIID);
	static void				destroySceneUnitAI	(CSceneUnitAI*& pUnitAI);

	void					initialize			();

	void					process				(uint64 uTick,uint64 uTime,uint32 uSecond);
	void					build				(CSceneUnit* pOwner,uint32 uAIID);
public://�ⲿע���¼�
	void					callbackUseSkillFailedByBlock	(uint32 uSkillID);
	void					callbackByAttack				(CSceneUnit* pUnit,int32 iDamage);
public://�ƶ�����
	bool					canOperate			();
	bool					setMoveDest			(uint8 MoveType,float fPX, float fPY);		//�����ƶ�Ŀ���
	float32					getMoveSpeed		()const;
	bool					beginMove			(uint8 MoveType,const _stPoint2F& stDirectDest);
	bool					endMove				();
	void					processMove			();//�ƶ��ƽ�
private:
	void					processOutHatred	();//���Ŀ������׷����Χ����
private:
	bool					getCurrentTargetPos	(_stPoint2F& stPoint);
	bool					getTargetPosByUnitID(uint32 uUnitID,_stPoint2F& stPoint);

	DECLARE_BEHAVIAC_AGENT(CSceneUnitAI, behaviac::Agent);

// ǰ���õĽӿ�
private:
	behaviac::EBTStatus		playerScanTarget		();													//ǰ�� �Զ�ս���õ�
	behaviac::EBTStatus		playerSelectTarget		();													//ǰ�� �Զ�ս���õ�
};
