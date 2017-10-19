/*------------- palyerModel.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 晁旭升
* Version      : V1.01
* Date         : 2016/02/22
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "log/log.h"
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 

// 任务类型，例如主线任务、日常任务、见闻任务等
enum QuestType
{
	QUEST_TYPE_START		= 0,

	QUEST_TYPE_MAIN			= 1,		// 主线（剧情）任务
	QUEST_TYPE_TEACH		= 2,		// 教学任务
	QUEST_TYPE_SEE			= 3,		// 见闻任务
	QUEST_TYPE_TRIAL		= 4,		// 试炼任务
	QUEST_TYPE_SOCIETY		= 5,		// 帮会任务
	QUEST_TYPE_FESTIVAL		= 6,		// 节日任务
	QUEST_TYPE_DAILY		= 7,		// 日常任务（行会、军团、论剑）

	QUEST_TYPE_END
};

// 任务目标类型
enum QuestTargetType
{
	QUEST_TARGET_TYPE_START = 0 ,

	QUEST_TARGET_TYPE_REPLAY_NPC	= 1,			// 回复NPC配置
	QUEST_TARGET_TYPE_KILL_MONSTER	= 2,			// 杀怪
	QUEST_TARGET_TYPE_COLLECT_GOODS	= 3,			// 采集
	QUEST_TARGET_TYPE_DEPOT_GOODS	= 4,			// 收集物品
	QUEST_TARGET_TYPE_MONSTER_DROP_GOODS = 5,		// 杀怪收集物品
	QUEST_TARGET_TYPE_LOCATION		= 6,			// 到达地点配置
	QUEST_TARGET_TYPE_PROTECT_NPC	= 7,			// 保护NPC
	QUEST_TARGET_TYPE_ESCORT_NPC	= 8,			// 护送NPC
	QUEST_TARGET_TYPE_EPISODE		= 9,			// 观看剧情
	QUEST_TARGET_TYPE_USE_ITEM		= 10,			// 使用道具
	QUEST_TARGET_TYPE_WAITING		= 11,			// 等待时间
	QUEST_TARGET_TYPE_KILLRIVALPLAYER	= 12,		// 击杀敌对阵营玩家
	QUEST_TARGET_TYPE_UPGRADE_LEVEL = 13,			// 达到等级，达到转职

	QUEST_TARGET_TYPE_END
};

// 任务事件类型（从其他系统传来的事件）
enum QuestEventType
{
	QUEST_EVENT_TYPE_START = 0 ,

	QUEST_EVENT_TYPE_ON_KILL_MONSTER ,	// 杀怪事件
	QUEST_EVENT_TYPE_ON_GET_GOODS ,		// 获得物品事件
	QUEST_EVENT_TYPE_ON_INTERACT_NPC ,	// NPC互动事件

	QUEST_EVENT_TYPE_ON_KILL_END
};

// 任务目标关系
enum QuestTargetRelation
{
	QUEST_TARGET_RELATION_PARALLEL	= 0 ,	// 并行关系
	QUEST_TARGET_RELATION_SERIAL	= 1		// 串行关系
};

// 任务前置条件类型
enum QuestPreconditionType
{
	QUEST_PRECONDITION_TYPE_START = 0,

	QUEST_PRECONDITION_TYPE_LEVEL				= 1,	// 等级
	QUEST_PRECONDITION_TYPE_GETSKILL			= 2,	// 获取技能
	QUEST_PRECONDITION_TYPE_NEWFEATURE			= 3,	// 开启功能
	QUEST_PRECONDITION_TYPE_LEGION				= 4,	// 加入军团
	QUEST_PRECONDITION_TYPE_INCONSTRUCTION		= 5,	// 军团建筑升级需求
	QUEST_PRECONDITION_TYPE_DATE				= 6,	// 开启时间
	QUEST_PRECONDITION_TYPE_PVP					= 7,	// 论剑功能

	QUEST_PRECONDITION_TYPE_END
};

// 日常任务类型
enum QuestDailyType
{
	QUEST_DAILY_TYPE_START = 0,

	QUEST_DAILY_TYPE_COMMON			= 1,		// 普通日常
	QUEST_DAILY_TYPE_SOCIETY		= 2,		// 帮会日常
	QUEST_DAILY_TYPE_LEGION			= 3,		// 军团日常
	QUEST_DAILY_TYPE_PVP			= 4,		// 论剑日常

	QUEST_DAILY_TYPE_END
};

typedef stl_vector<uint32> QuestTargetTypeList;	// <任务目标类型>

// 描述组成任务的任务目标
typedef stl_vector<uint32> QuestConditionList;
struct QuestTarget 
{
	uint32 uType;
	uint32 uTargetNum;
	// uint32 uClientUpdate;	// 可否客户端更新（凭客户端更新任务目标消息更新任务目标：0-不可；1-可）//目前统一按目标类型判定
	QuestConditionList vecCondition;	// <任务目标条件值（具体意义及计算、检查方式按约定）>

	QuestTarget() : uType(0), uTargetNum(0)//, uClientUpdate(0)
	{

	}

	QuestTarget(uint32 _uType, uint32 _uTargetNum) : uType(_uType), uTargetNum(_uTargetNum)//, uClientUpdate(0)
	{

	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(uType)&&
			stream.read(uTargetNum)&&
			stream.read(vecCondition);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(uType)&&
			stream.write(uTargetNum)&&
			stream.write(vecCondition);
	}
};

// 有向图的点
struct PointWithEdge
{
	uint32 uPoint;
	stl_set<uint32> in;
	stl_set<uint32> out;

	PointWithEdge() : uPoint(0)
	{

	}

	PointWithEdge(uint32 _uPoint) : uPoint(_uPoint)
	{

	}

	bool addIn(uint32 uIn)
	{
		if(in.is_exist(uIn))
		{
			return false;
		}

		in.insert(uIn);

		return true;
	}

	bool addOut(uint32 uOut)
	{
		if(out.is_exist(uOut))
		{
			return false;
		}

		out.insert(uOut);

		return true;
	}

	bool hasInEdge()
	{
		return ! in.empty();
	}

	bool hasOutEdge()
	{
		return ! out.empty();
	}

	void delInEdge(uint32 uIn)
	{
		in.erase(uIn);
	}

	void delOutEdge(uint32 uOut)
	{
		out.erase(uOut);
	}
};

// 有向图
struct DirectedGraph
{
	// 有向图：边AB，为入点A的出边，为出点B的入边；单个点，不算图，不入图；连通图

	typedef stl_map<uint32, PointWithEdge> PointMap;
	PointMap mapPoint;

	PointWithEdge * getPointWithEdge(uint32 uPoint)
	{
		return mapPoint.find_(uPoint);
	}

	bool addEdge(uint32 uIn, uint32 uOut)
	{
		// 为入点添加出边
		PointWithEdge * pInPoint = getPointWithEdge(uIn);
		if(! pInPoint)
		{
			PointWithEdge inPoint(uIn);
			inPoint.addOut(uOut);

			mapPoint[uIn] = inPoint;
		}
		else
		{
			if(! pInPoint->addOut(uOut))
			{
				// 已经存在的出边，告诉调用者
				return false;
			}
		}

		// 为出点添加入边
		PointWithEdge * pOutPoint = getPointWithEdge(uOut);
		if(! pOutPoint)
		{
			PointWithEdge outPoint(uOut);
			outPoint.addIn(uIn);

			mapPoint[uOut] = outPoint;
		}
		else
		{
			if(! pOutPoint->addIn(uIn))
			{
				// 已经存在的入边，告诉调用者
				return false;
			}
		}

		return true;
	}

	void delInEdge(uint32 uIn, uint32 uOut)
	{
		PointWithEdge * pOutPoint = getPointWithEdge(uOut);
		if(pOutPoint)
		{
			pOutPoint->delInEdge(uIn);
		}
	}

	void delOutEdge(uint32 uIn, uint32 uOut)
	{
		PointWithEdge * pInPoint = getPointWithEdge(uIn);
		if(pInPoint)
		{
			pInPoint->delOutEdge(uOut);
		}
	}

	// 循环删除所有无入边的点及其出边，最终图为空，无环；否则，有环
	bool checkCycle(stl_set<uint32>& vecPoint)
	{
		uint32 uCount = 0;

		do
		{
			uCount = 0;

			PointMap::iterator it = mapPoint.begin();
			PointMap::iterator itEnd = mapPoint.end();
			for(; it != itEnd;)
			{
				if(! it->second.hasInEdge())
				{
					uint32 uIn = it->first;
					stl_set<uint32>::iterator itOut = it->second.out.begin();
					stl_set<uint32>::iterator itOutEnd = it->second.out.end();
					for(; itOut != itOutEnd; ++itOut)
					{
						delInEdge(uIn, *itOut);
					}

					vecPoint.insert(uIn);

					mapPoint.erase(it++);

					++uCount;
				}
				else
				{
					++it;
				}
			}
		} while (uCount != 0);

		if(mapPoint.empty())
		{
			return true;
		}

		// 如果有环，删除无出边的点，缩小、定位环的范围
		do
		{
			uCount = 0;

			PointMap::iterator it = mapPoint.begin();
			PointMap::iterator itEnd = mapPoint.end();
			for(; it != itEnd;)
			{
				if(! it->second.hasOutEdge())
				{
					uint32 uOut = it->first;
					stl_set<uint32>::iterator itIn = it->second.in.begin();
					stl_set<uint32>::iterator itInEnd = it->second.in.end();
					for(; itIn != itInEnd; ++itIn)
					{
						delOutEdge(*itIn, uOut);
					}

					vecPoint.insert(uOut);

					mapPoint.erase(it++);

					++uCount;
				}
				else
				{
					++it;
				}
			}
		} while (uCount != 0);

		CLog::error("struct DirectedGraph::checkCycle:发现死锁！死锁元素（部分）：");
		PointMap::iterator it = mapPoint.begin();
		PointMap::iterator itEnd = mapPoint.end();
		for(uint32 i = 0; it != itEnd && i < 10; ++it, ++i)
		{
			CLog::error("%d", it->first);
		}

		return false;
	}
};

// 统一的解析、判定任务目标的接口
struct QuestTargetModel
{
	typedef stl_vector<void *> VoidPtrList;

	virtual bool parse(QuestTarget& target) const = 0;
	virtual bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const = 0;

	virtual uint32 getTargetId(const QuestTarget& target) const = 0;
};

struct QuestTools
{
	//字符串分割函数
	static void splitStr(std::string strSource, std::string strPattern, stl_vector<std::string>& vecResult)
	{
		std::string::size_type patternSize = strPattern.size();
		if(patternSize == 0)
		{
			vecResult.push_back(strSource);
			return;
		}

		std::string::size_type pos;
		strSource += strPattern;		// 扩展字符串以方便操作
		std::string::size_type size = strSource.size();
		std::string::size_type i = 0;
		for( ; i < size; )
		{
			pos = strSource.find(strPattern, i);
			if(pos < size)
			{
				std::string strTmp(strSource.substr(i, pos-i));
				vecResult.push_back(strTmp);
				i = pos + patternSize;
			}
		}
	}

	static void atoi(const stl_vector<std::string>& vecSource, stl_vector<uint32>& vecResult)
	{
		stl_vector<std::string>::const_iterator it = vecSource.begin();
		stl_vector<std::string>::const_iterator itEnd = vecSource.end();
		for (; it != itEnd; ++it)
		{
			vecResult.push_back(static_cast<uint32>(std::atoi((*it).c_str())));
		}
	}
};

// todelete 示例用对象：目标模板使用到了其他类的对象
struct ObjModleMgr
{
	struct CGoods
	{
		uint32 uId;

		CGoods() : uId(0)
		{

		}

		CGoods(uint32 _uId) : uId(_uId)
		{

		}

		uint32 getId() const
		{
			return uId;
		}
	};

	struct CMonster
	{
		uint32 uId;

		CMonster() : uId(0)
		{

		}

		CMonster(uint32 _uId) : uId(_uId)
		{

		}

		uint32 getId() const
		{
			return uId;
		}
	};

};

typedef ObjModleMgr::CGoods CGoods;
typedef ObjModleMgr::CMonster CMonster;


/*
*	针对特定类型的任务目标进行解析和判定的模板
*	任务目标类型：QUEST_TARGET_TYPE_KILL_MONSTER（杀怪）
*/
struct QuestTargetModel_KillMonster : public QuestTargetModel
{
	QuestTargetModel_KillMonster() : uConditionSize(1), uParameterSize(1)
	{

	}

	bool parse(QuestTarget& target) const
	{
		// 校验
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_KillMonster::parse:解析所得条件参数数量与约定不一致");
			return false;
		}

		// 前uConditionSize个为任务条件

		// 第uConditionSize+1个为任务目标数量
		target.uTargetNum = target.vecCondition[uConditionSize];

		// 其余为客户端用扩展信息，不管

		return true;
	}

	// 判定顺序，参数意义，被比较的对象，都按约定
	bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const
	{
		if(vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_KillMonster:条件个数不匹配!");
			return false;
		}
		if(vecParameter.size() != uParameterSize)
		{
			CLog::error("QuestTargetModel_KillMonster:参数个数不匹配!");
			return false;
		}

		if(! vecParameter[0])
		{
			CLog::error("QuestTargetModel_KillMonster:空指针!");
			return false;
		}

		const CMonster* pMonster = (const CMonster*)vecParameter[0];

		// 怪物id必须匹配
		if(pMonster->getId() != vecCondition[0])
		{
			return false;
		}

		return true;
	}

	uint32 getTargetId(const QuestTarget& target) const
	{
		// 校验
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_KillMonster::getTargetId:条件参数数量与约定不一致");
			return 0;
		}

		return target.vecCondition[0];
	}

private:
	uint32 uConditionSize;
	uint32 uParameterSize;
};

/*
*	针对特定类型的任务目标进行解析和判定的模板
*	任务目标类型：QUEST_TARGET_TYPE_REPLY_NPC（回复npc）
*	npc对话类任务目标，客户端请求直接完成，这里不做什么
*/
struct QuestTargetModel_ReplyNpc : public QuestTargetModel
{
	QuestTargetModel_ReplyNpc()
	{

	}

	bool parse(QuestTarget& target) const
	{
		target.uTargetNum = 1;

		// 不做操作
		return true;
	}

	// 判定顺序，参数意义，被比较的对象，都按约定
	bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const
	{
		// 不做判断
		return false;
	}

	uint32 getTargetId(const QuestTarget& target) const
	{
		return 0;
	}
};

/*
*	针对特定类型的任务目标进行解析和判定的模板
*	任务目标类型：QUEST_TARGET_TYPE_COLLECT_GOODS（采集）
*	// todo 代码仅为示例，具体按约定重写
*/
struct QuestTargetModel_CollectGoods : public QuestTargetModel
{
	QuestTargetModel_CollectGoods() : uConditionSize(1), uParameterSize(1)
	{

	}

	bool parse(QuestTarget& target) const
	{
		// 校验
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_CollectGoods::parse:解析所得条件参数数量与约定不一致");
			return false;
		}

		// 前uConditionSize个为任务条件

		// 第uConditionSize+1个为任务目标数量
		target.uTargetNum = target.vecCondition[uConditionSize];

		// 其余为客户端扩展，不管

		return true;
	}

	// 判定顺序，参数意义，被比较的对象，都按约定
	bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const
	{
		if(vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_CollectGoods:条件个数不匹配!");
			return false;
		}
		if(vecParameter.size() != uParameterSize)
		{
			CLog::error("QuestTargetModel_CollectGoods:参数个数不匹配!");
			return false;
		}

		if(! vecParameter[0])
		{
			CLog::error("QuestTargetModel_CollectGoods:空指针!");
			return false;
		}

		const CGoods* pGoods = (const CGoods*)vecParameter[0];

		// 物品id必须匹配
		if(pGoods->getId() != vecCondition[0])
		{
			return false;
		}

		return true;
	}

	uint32 getTargetId(const QuestTarget& target) const
	{
		// 校验
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_CollectGoods::getTargetId:条件参数数量与约定不一致");
			return 0;
		}

		return target.vecCondition[0];
	}

private:
	uint32 uConditionSize;
	uint32 uParameterSize;
};

/*
*	针对特定类型的任务目标进行解析和判定的模板
*	任务目标类型：QUEST_TARGET_TYPE_DEPOT_GOODS（收集物品）
*	// todo 代码仅为示例，具体按约定重写
*/
struct QuestTargetModel_DepotGoods : public QuestTargetModel
{
	QuestTargetModel_DepotGoods() : uConditionSize(1), uParameterSize(1)
	{

	}

	bool parse(QuestTarget& target) const
	{
		// 校验
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_DepotGoods::parse:解析所得条件参数数量与约定不一致");
			return false;
		}

		// 前uConditionSize个为任务条件

		// 第uConditionSize+1个为任务目标数量
		target.uTargetNum = target.vecCondition[uConditionSize];

		// 其余为客户端扩展，不管

		return true;
	}

	// 判定顺序，参数意义，被比较的对象，都按约定
	bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const
	{
		if(vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_DepotGoods:条件个数不匹配!");
			return false;
		}
		if(vecParameter.size() != uParameterSize)
		{
			CLog::error("QuestTargetModel_DepotGoods:参数个数不匹配!");
			return false;
		}

		if(! vecParameter[0])
		{
			CLog::error("QuestTargetModel_DepotGoods:空指针!");
			return false;
		}

		const CGoods* pGoods = (const CGoods*)vecParameter[0];

		// 物品id必须匹配
		if(pGoods->getId() != vecCondition[0])
		{
			return false;
		}

		return true;
	}

	uint32 getTargetId(const QuestTarget& target) const
	{
		// 校验
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_DepotGoods::getTargetId:条件参数数量与约定不一致");
			return 0;
		}

		return target.vecCondition[0];
	}

private:
	uint32 uConditionSize;
	uint32 uParameterSize;
};

/*
*	针对特定类型的任务目标进行解析和判定的模板
*	任务目标类型：QUEST_TARGET_TYPE_MONSTER_DROP_GOODS（杀怪收集物品）
*	// todo 代码仅为示例，具体按约定重写
*/
struct QuestTargetModel_MonsterDepotGoods : public QuestTargetModel
{
	QuestTargetModel_MonsterDepotGoods() : uConditionSize(3), uParameterSize(1)
	{

	}

	bool parse(QuestTarget& target) const
	{
		// 校验
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_MonsterDepotGoods::parse:解析所得条件参数数量与约定不一致");
			return false;
		}

		// 第uConditionSize+1个为任务目标数量
		target.uTargetNum = target.vecCondition[uConditionSize];

		return true;
	}

	// 判定顺序，参数意义，被比较的对象，都按约定
	bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const
	{
		if(vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_MonsterDepotGoods:条件个数不匹配!");
			return false;
		}
		if(vecParameter.size() != uParameterSize)
		{
			CLog::error("QuestTargetModel_MonsterDepotGoods:参数个数不匹配!");
			return false;
		}

		if(! vecParameter[0])
		{
			CLog::error("QuestTargetModel_MonsterDepotGoods:空指针!");
			return false;
		}

		const CGoods* pGoods = (const CGoods*)vecParameter[0];

		// 物品id必须匹配
		if(pGoods->getId() != vecCondition[1])
		{
			return false;
		}

		return true;
	}

	uint32 getTargetId(const QuestTarget& target) const
	{
		// 校验
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_MonsterDepotGoods::getTargetId:条件参数数量与约定不一致");
			return 0;
		}

		return target.vecCondition[1];
	}

private:
	uint32 uConditionSize;
	uint32 uParameterSize;
};

struct QuestTargetModelMgr
{
	QuestTargetModel_ReplyNpc		replyNpc;
	QuestTargetModel_KillMonster	killMonster;
	QuestTargetModel_CollectGoods	collectGoods;
	QuestTargetModel_DepotGoods		depotGoods;
	QuestTargetModel_MonsterDepotGoods		monsterDepotGoods;
};