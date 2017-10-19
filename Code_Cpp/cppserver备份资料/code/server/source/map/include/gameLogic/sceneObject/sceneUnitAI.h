/*----------------- sceneUnitAI.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
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

	//自身指针
	CSceneUnit*				m_pOwner;

	// ai 配置参数
	const _stAIModel*		m_pAIModel;				//ai配置模板
	VEC_SKILL				m_vecOrdinalSkill;		//通用技能列表 按优先级排序后的

	// 运行时的变量
	_emAIStateType			m_eAIStateType;			//ai状态
	uint32					m_uFreeVar;				//暴露给脚本使用的自由变量
	MAP_I32_I32				m_mapProperty;			//自定义属性
	SET_U32					m_setMessage;			//接受到的消息
	uint32					m_uRunAIFrameCount;		//控制ai执行频率的
	uint32					m_uAICallFrequency;		//ai执行频率的
	uint32					m_uNextPatrolTime;		//下一次可随机移动时间 控制巡逻的
	uint32					m_uTargetID;			//当前目标ID
	const _stSkillModel*	m_pSkillModel;			//当前选中技能模版
	_stPoint2F				m_stMoveDest;			//移动终极目标点
	_stPoint2F				m_stMoveBeginPos;		//当前直线移动的起始坐标
	float32					m_fCurrentMoveDir;		//当前需要直线移动的方向
	float32					m_fCurrentMoveDis;		//当前需要直线移动的距离
	uint8					m_uMoveType;
	_stPoint2F				m_stBirthPoint;			//出生点
	_stPoint2F				m_stAIPoint;			//AI战斗中心点
	MAP_I32_U64				m_mapSpecialCD;			//特殊定制cd
	MAP_I32_U64				m_mapSpecialTimer;		//特殊定时器
	uint32					m_uFailedSkillIdByBlock;//因为距离有阻挡而失败的技能id
	float32					m_fFirstDirection;		//初始朝向

	uint32					m_uPathID;				//当前路径ID
	uint32					m_uPathTarIndex;		//当前路径中的目标索引

	_stPoint2F				m_stTargetPos;			//当前目标点

	uint32					m_uSelectTargetType;	//当前仇恨模式
	uint32					m_uHatredSelectOrder;	//选择第几仇恨目标

	uint64					m_uLastBeAttackTick;

	bool					m_bStop;
public:
	behaviac::EBTStatus		change2IdleState		();							//转换到休闲状态
	behaviac::EBTStatus		change2BattleState		();							//转换到战斗状态
	behaviac::EBTStatus		change2ReturnState		();							//转换到回退状态

	bool					isIdleState				();							//判断是否在休闲状态
	bool					isBattleState			();							//判断是否在战斗状态
	bool					isReturnState			();							//判断是否在回退状态

	behaviac::EBTStatus		doIdle					();							//执行策划配置的休闲行为
private:
	behaviac::EBTStatus		changeState				(_emAIStateType eState);
public://状态相关
	//叶子节点，执行具体的指令。其实应该要细化的。
	behaviac::EBTStatus		doPatroll				();							//执行巡逻
	behaviac::EBTStatus		doPath					();							//执行路径点移动
	behaviac::EBTStatus		doTargetPos				();							//执行目标点移动
	behaviac::EBTStatus		doNothing				();							//什么都不做
	behaviac::EBTStatus		doAttack				();							//执行攻击
	behaviac::EBTStatus		doChase					();							//执行追击
	behaviac::EBTStatus		doReturn				();							//执行回退
public://自定义属性相关
	uint32					getFreeVar				()			{return m_uFreeVar;};		//获得自由变量
	void					setFreeVar				(uint32 t)	{m_uFreeVar = t;};			//设置自由变量

	int32					getProperty				(int32 iKey);							//获取自定义属性(默认值为0)
	void					setProperty				(int32 iKey, int32 iValue);				//设置自定义属性
	void					clearProperty			();										//清空自定义属性列表
public://目标相关
	behaviac::EBTStatus		clearTarget				();										//清除目标
	bool					hasTarget				();										//检查是否有攻击对象
	behaviac::EBTStatus		selectTarget			();										//按规则选取目标
	behaviac::EBTStatus		selectHatredTarget		(uint32 uHatredOrder);					//从仇恨列表里选取指定仇恨值排序的目标
	behaviac::EBTStatus		selectFirstHatredTarget	();										//从仇恨列表里选取仇恨最高的目标
	behaviac::EBTStatus		selectRandTarget		();										//从仇恨列表里随机选择目标
	behaviac::EBTStatus		selectMinHPTarget		();										//从仇恨列表里选取血量最少的目标
	behaviac::EBTStatus		selectNearstTarget		();										//从仇恨列表里选取距离最小的目标
	behaviac::EBTStatus		selectBuffTarget		(uint32 uBuffID);						//选择携带特定buff的目标
	behaviac::EBTStatus		scanHatredObj			();										//检查警戒范围内的敌人,并且加入仇恨列表,
	behaviac::EBTStatus		selectNearstTargetAround(float32 fRadius);						//无视仇恨机制查找查找最近的目标
	behaviac::EBTStatus		selectMonsterTarget		(uint32 uMonsterID);					//无视仇恨查找指定怪物设为目标
	float32					getTargetDistance		();										//获得与当前目标的距离
	bool					isTargetInReleaseRange	();										//目标是否在选中技能的射程范围内
	bool					isTargetInSkillRange	(uint32 uSkillID);						//目标是否在指定技能的射程范围内
	behaviac::EBTStatus		setSelectTargetType		(uint32 uType);							//设置目标选取类型
	behaviac::EBTStatus		setHatredSelectOrder	(uint32 uOrder);						//设置仇恨导向的怪的选择仇恨值排序数
	behaviac::EBTStatus		changeCopymapTask		(uint32 uQuestID);						//切换副本任务分支 closeCopymap
	behaviac::EBTStatus		closeCopymap			(uint32 uTime, uint32 uChatID);			//关闭副本 
private:
	bool					isTargetInRange			(const _stSkillModel*	pSkillModel);
public://技能相关
	behaviac::EBTStatus		selectSkill				();										//按技能使用优先级选取当前要使用的技能
	behaviac::EBTStatus		setSelectSkill			(uint32 uSkillID);						//指定当前要使用的技能
	behaviac::EBTStatus		useSkill				(uint32 uSkillID, uint8 uLevel);		//使用指定技能,不会重新选取目标
	behaviac::EBTStatus		useSelectedSkill		();										//使用当前选中技能,当前没有选中技能的话会自动选取一个可用技能,不会重新选取目标
	bool					hasSkill				();										//是否有选中技能
	bool					isSkillCooling			(uint32 uSkillID);						//该技能是否在冷去中

	behaviac::EBTStatus		addSkill				(uint32 uSkillID,uint32 uPriority);		//增加常规技能
	behaviac::EBTStatus		delSkill				(uint32 uSkillID);						//删除常规技能
	behaviac::EBTStatus		resetSkill				();										//重置常规技能

	behaviac::EBTStatus		startSpecialCD			(uint32 uIndex, uint32 uMS);			//启动指定的CD开始冷却
	bool					checkSpecialCD			(uint32 uIndex);						//查看指定的CD有没有冷却完成
	bool					checkAndStartSpecialCD	(uint32 uIndex, uint32 uMS);			//查看指定的CD有没有冷却完成，还在冷却中返回假，已经完成冷却返回真并重启冷却

	behaviac::EBTStatus		clearBuff				();								//清空Buff
	behaviac::EBTStatus		clearHatred				();								//清空仇恨列表
	behaviac::EBTStatus		clearTargetHatred		();								//清除当前目标的仇恨
	behaviac::EBTStatus		clearHurt				();								//清空伤害列表
	behaviac::EBTStatus		addBuff					(uint32 uBuffID, uint8 uLevel);	//增加buff
	behaviac::EBTStatus		delBuff					(uint32 uBuffID);				//删除buff
	behaviac::EBTStatus		targetAddBuff			(uint32 uBuffID, uint8 uLevel);	//给目标增加buff
	behaviac::EBTStatus		targetDelBuff			(uint32 uBuffID);				//给目标删除buff
public:// 属主相关
	bool					hasHatredObj			();								//是否有仇恨对象
	bool					canMove					();								//是否可移动
	bool					canUseSkill				();								//是否可使用技能
	bool					isFighting				();								//是否在战斗中
	bool					isMoving				();								//是否移动中
	bool					isUsingSkill			();								//是否使用技能中
	bool					isBeAttack				(uint32 uIntervalMS);			//判断最近时间内有没有被攻击

	uint32					getHPPercent			();								//获取当前血量百分比
	uint8					getCamp					();								//获得阵营
	behaviac::EBTStatus		changeCamp				(uint8 uNewCampID);				//改变阵营

	bool					isPathComplete			();								//路径点移动结束了
	bool					isTargetPosComplete		();								//目标点移动结束了

	uint32					getFightTime			();								//进入战斗时间
public:// 特殊功能相关
	behaviac::EBTStatus		killMonsterById			(uint32 uMonsterId, uint32 uCount, float32 fRadius);	//杀死身边指定id的怪物
	behaviac::EBTStatus		removeMonsterById		(uint32 uMonsterId, uint32 uCount, float32 fRadius);	//删除身边指定id的怪物
	behaviac::EBTStatus		killSelf				();											//自杀
	behaviac::EBTStatus		removeSelf				();											//自己消失
	behaviac::EBTStatus		addSceneArea			(uint32 uAreaID);							//开启场景中的特殊区域
	behaviac::EBTStatus		delSceneArea			(uint32 uAreaID);							//关闭场景中的特殊区域
	behaviac::EBTStatus		checkSkillFailedAndWait	(uint32 uSkillID);							//检查是否有技能因为阻挡点的问题没有释放成功。如果有则阻塞
	behaviac::EBTStatus		speak					(uint32 uSpeakID);							//说话
	behaviac::EBTStatus		setAIPoint				();											//将当前坐标设置AI战斗中心点
	behaviac::EBTStatus		setBirthPoint			();											//将当前坐标设置为出生点
	behaviac::EBTStatus		setAI					(uint32 uID);								//重设AI
	behaviac::EBTStatus		stopAI					(bool bStop);								//停止AI 前端提的AI需求
	behaviac::EBTStatus		setPathID				(uint32 uPathID) ;							//设置路径ID
	behaviac::EBTStatus		setTargetPos			(float32 fPosX,float32 fPosY) ;				//设置目标点
	uint32					getPlayerCountRange		(float32 fRadius);							//获取范围内玩家数量
	float32					getDisFromAIPoint		();											//获得与战斗中心点的距离
	behaviac::EBTStatus		questComplete			() ;										//完成杀怪任务
	behaviac::EBTStatus		setTime					(uint32 uIndex,uint32 uMS,bool bRuning);	//设置定时期
	behaviac::EBTStatus		resetTime				(uint32 uIndex);							//执行追击
	behaviac::EBTStatus		addMonster_fixPos		(uint32 uMonsterID,uint32 uCount,float32 fPosX,float32 fPosY,float32 fAngleDir);				//固定位置刷怪
	behaviac::EBTStatus		addMonster_relatedPos	(uint32 uMonsterID,uint32 uCount,float32 fRelatedX,float32 fRelatedY,float32 fAngleDir);		//相对位置刷怪
	behaviac::EBTStatus		addMonster_randomPos	(uint32 uMonsterID,uint32 uCount,float32 fMinRadius,float32 fMaxRadius,float32 fAngleDir);	//随机位置刷怪
	behaviac::EBTStatus		changeDirection			(uint32 uAngle);							//改变朝向
	behaviac::EBTStatus		sendMessageAround		(uint32 uIndex,uint32 uMonsterID,float32 fRadius);	//向周围发送消息
	behaviac::EBTStatus		sendMessage				(uint32 uIndex,uint32 uMonsterID);					//发送消息
	behaviac::EBTStatus		readMessage				(uint32 uIndex,bool bDelete);						//读取消息
	behaviac::EBTStatus		askForHelpAround		(uint32 uMonsterID,float32 fRadius);				//在指定范围求援
	behaviac::EBTStatus		askForHelp				(uint32 uMonsterID);								//求援
	behaviac::EBTStatus		moveBack				(float32 fDis);										//向目标的反方向移动设置的距离
	behaviac::EBTStatus		moveToTarget			(float32 fDis);										//移动到距离当前目标指定距离内的位置
	behaviac::EBTStatus		faceTarget				();													//怪物面向朝向他的目标
public:
	CSceneUnitAI();
	virtual ~CSceneUnitAI();

	static CSceneUnitAI*	createSceneUnitAI	(CSceneUnit* pOwner,uint32 uAIID);
	static void				destroySceneUnitAI	(CSceneUnitAI*& pUnitAI);

	void					initialize			();

	void					process				(uint64 uTick,uint64 uTime,uint32 uSecond);
	void					build				(CSceneUnit* pOwner,uint32 uAIID);
public://外部注入事件
	void					callbackUseSkillFailedByBlock	(uint32 uSkillID);
	void					callbackByAttack				(CSceneUnit* pUnit,int32 iDamage);
public://移动控制
	bool					canOperate			();
	bool					setMoveDest			(uint8 MoveType,float fPX, float fPY);		//设置移动目标点
	float32					getMoveSpeed		()const;
	bool					beginMove			(uint8 MoveType,const _stPoint2F& stDirectDest);
	bool					endMove				();
	void					processMove			();//移动推进
private:
	void					processOutHatred	();//仇恨目标脱离追击范围处理
private:
	bool					getCurrentTargetPos	(_stPoint2F& stPoint);
	bool					getTargetPosByUnitID(uint32 uUnitID,_stPoint2F& stPoint);

	DECLARE_BEHAVIAC_AGENT(CSceneUnitAI, behaviac::Agent);

// 前端用的接口
private:
	behaviac::EBTStatus		playerScanTarget		();													//前端 自动战斗用的
	behaviac::EBTStatus		playerSelectTarget		();													//前端 自动战斗用的
};
