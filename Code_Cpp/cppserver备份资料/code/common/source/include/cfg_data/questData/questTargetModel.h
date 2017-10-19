/*------------- palyerModel.h
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ������
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

// �������ͣ��������������ճ����񡢼��������
enum QuestType
{
	QUEST_TYPE_START		= 0,

	QUEST_TYPE_MAIN			= 1,		// ���ߣ����飩����
	QUEST_TYPE_TEACH		= 2,		// ��ѧ����
	QUEST_TYPE_SEE			= 3,		// ��������
	QUEST_TYPE_TRIAL		= 4,		// ��������
	QUEST_TYPE_SOCIETY		= 5,		// �������
	QUEST_TYPE_FESTIVAL		= 6,		// ��������
	QUEST_TYPE_DAILY		= 7,		// �ճ������лᡢ���š��۽���

	QUEST_TYPE_END
};

// ����Ŀ������
enum QuestTargetType
{
	QUEST_TARGET_TYPE_START = 0 ,

	QUEST_TARGET_TYPE_REPLAY_NPC	= 1,			// �ظ�NPC����
	QUEST_TARGET_TYPE_KILL_MONSTER	= 2,			// ɱ��
	QUEST_TARGET_TYPE_COLLECT_GOODS	= 3,			// �ɼ�
	QUEST_TARGET_TYPE_DEPOT_GOODS	= 4,			// �ռ���Ʒ
	QUEST_TARGET_TYPE_MONSTER_DROP_GOODS = 5,		// ɱ���ռ���Ʒ
	QUEST_TARGET_TYPE_LOCATION		= 6,			// ����ص�����
	QUEST_TARGET_TYPE_PROTECT_NPC	= 7,			// ����NPC
	QUEST_TARGET_TYPE_ESCORT_NPC	= 8,			// ����NPC
	QUEST_TARGET_TYPE_EPISODE		= 9,			// �ۿ�����
	QUEST_TARGET_TYPE_USE_ITEM		= 10,			// ʹ�õ���
	QUEST_TARGET_TYPE_WAITING		= 11,			// �ȴ�ʱ��
	QUEST_TARGET_TYPE_KILLRIVALPLAYER	= 12,		// ��ɱ�ж���Ӫ���
	QUEST_TARGET_TYPE_UPGRADE_LEVEL = 13,			// �ﵽ�ȼ����ﵽתְ

	QUEST_TARGET_TYPE_END
};

// �����¼����ͣ�������ϵͳ�������¼���
enum QuestEventType
{
	QUEST_EVENT_TYPE_START = 0 ,

	QUEST_EVENT_TYPE_ON_KILL_MONSTER ,	// ɱ���¼�
	QUEST_EVENT_TYPE_ON_GET_GOODS ,		// �����Ʒ�¼�
	QUEST_EVENT_TYPE_ON_INTERACT_NPC ,	// NPC�����¼�

	QUEST_EVENT_TYPE_ON_KILL_END
};

// ����Ŀ���ϵ
enum QuestTargetRelation
{
	QUEST_TARGET_RELATION_PARALLEL	= 0 ,	// ���й�ϵ
	QUEST_TARGET_RELATION_SERIAL	= 1		// ���й�ϵ
};

// ����ǰ����������
enum QuestPreconditionType
{
	QUEST_PRECONDITION_TYPE_START = 0,

	QUEST_PRECONDITION_TYPE_LEVEL				= 1,	// �ȼ�
	QUEST_PRECONDITION_TYPE_GETSKILL			= 2,	// ��ȡ����
	QUEST_PRECONDITION_TYPE_NEWFEATURE			= 3,	// ��������
	QUEST_PRECONDITION_TYPE_LEGION				= 4,	// �������
	QUEST_PRECONDITION_TYPE_INCONSTRUCTION		= 5,	// ���Ž�����������
	QUEST_PRECONDITION_TYPE_DATE				= 6,	// ����ʱ��
	QUEST_PRECONDITION_TYPE_PVP					= 7,	// �۽�����

	QUEST_PRECONDITION_TYPE_END
};

// �ճ���������
enum QuestDailyType
{
	QUEST_DAILY_TYPE_START = 0,

	QUEST_DAILY_TYPE_COMMON			= 1,		// ��ͨ�ճ�
	QUEST_DAILY_TYPE_SOCIETY		= 2,		// ����ճ�
	QUEST_DAILY_TYPE_LEGION			= 3,		// �����ճ�
	QUEST_DAILY_TYPE_PVP			= 4,		// �۽��ճ�

	QUEST_DAILY_TYPE_END
};

typedef stl_vector<uint32> QuestTargetTypeList;	// <����Ŀ������>

// ����������������Ŀ��
typedef stl_vector<uint32> QuestConditionList;
struct QuestTarget 
{
	uint32 uType;
	uint32 uTargetNum;
	// uint32 uClientUpdate;	// �ɷ�ͻ��˸��£�ƾ�ͻ��˸�������Ŀ����Ϣ��������Ŀ�꣺0-���ɣ�1-�ɣ�//Ŀǰͳһ��Ŀ�������ж�
	QuestConditionList vecCondition;	// <����Ŀ������ֵ���������弰���㡢��鷽ʽ��Լ����>

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

// ����ͼ�ĵ�
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

// ����ͼ
struct DirectedGraph
{
	// ����ͼ����AB��Ϊ���A�ĳ��ߣ�Ϊ����B����ߣ������㣬����ͼ������ͼ����ͨͼ

	typedef stl_map<uint32, PointWithEdge> PointMap;
	PointMap mapPoint;

	PointWithEdge * getPointWithEdge(uint32 uPoint)
	{
		return mapPoint.find_(uPoint);
	}

	bool addEdge(uint32 uIn, uint32 uOut)
	{
		// Ϊ�����ӳ���
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
				// �Ѿ����ڵĳ��ߣ����ߵ�����
				return false;
			}
		}

		// Ϊ����������
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
				// �Ѿ����ڵ���ߣ����ߵ�����
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

	// ѭ��ɾ����������ߵĵ㼰����ߣ�����ͼΪ�գ��޻��������л�
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

		// ����л���ɾ���޳��ߵĵ㣬��С����λ���ķ�Χ
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

		CLog::error("struct DirectedGraph::checkCycle:��������������Ԫ�أ����֣���");
		PointMap::iterator it = mapPoint.begin();
		PointMap::iterator itEnd = mapPoint.end();
		for(uint32 i = 0; it != itEnd && i < 10; ++it, ++i)
		{
			CLog::error("%d", it->first);
		}

		return false;
	}
};

// ͳһ�Ľ������ж�����Ŀ��Ľӿ�
struct QuestTargetModel
{
	typedef stl_vector<void *> VoidPtrList;

	virtual bool parse(QuestTarget& target) const = 0;
	virtual bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const = 0;

	virtual uint32 getTargetId(const QuestTarget& target) const = 0;
};

struct QuestTools
{
	//�ַ����ָ��
	static void splitStr(std::string strSource, std::string strPattern, stl_vector<std::string>& vecResult)
	{
		std::string::size_type patternSize = strPattern.size();
		if(patternSize == 0)
		{
			vecResult.push_back(strSource);
			return;
		}

		std::string::size_type pos;
		strSource += strPattern;		// ��չ�ַ����Է������
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

// todelete ʾ���ö���Ŀ��ģ��ʹ�õ���������Ķ���
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
*	����ض����͵�����Ŀ����н������ж���ģ��
*	����Ŀ�����ͣ�QUEST_TARGET_TYPE_KILL_MONSTER��ɱ�֣�
*/
struct QuestTargetModel_KillMonster : public QuestTargetModel
{
	QuestTargetModel_KillMonster() : uConditionSize(1), uParameterSize(1)
	{

	}

	bool parse(QuestTarget& target) const
	{
		// У��
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_KillMonster::parse:����������������������Լ����һ��");
			return false;
		}

		// ǰuConditionSize��Ϊ��������

		// ��uConditionSize+1��Ϊ����Ŀ������
		target.uTargetNum = target.vecCondition[uConditionSize];

		// ����Ϊ�ͻ�������չ��Ϣ������

		return true;
	}

	// �ж�˳�򣬲������壬���ȽϵĶ��󣬶���Լ��
	bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const
	{
		if(vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_KillMonster:����������ƥ��!");
			return false;
		}
		if(vecParameter.size() != uParameterSize)
		{
			CLog::error("QuestTargetModel_KillMonster:����������ƥ��!");
			return false;
		}

		if(! vecParameter[0])
		{
			CLog::error("QuestTargetModel_KillMonster:��ָ��!");
			return false;
		}

		const CMonster* pMonster = (const CMonster*)vecParameter[0];

		// ����id����ƥ��
		if(pMonster->getId() != vecCondition[0])
		{
			return false;
		}

		return true;
	}

	uint32 getTargetId(const QuestTarget& target) const
	{
		// У��
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_KillMonster::getTargetId:��������������Լ����һ��");
			return 0;
		}

		return target.vecCondition[0];
	}

private:
	uint32 uConditionSize;
	uint32 uParameterSize;
};

/*
*	����ض����͵�����Ŀ����н������ж���ģ��
*	����Ŀ�����ͣ�QUEST_TARGET_TYPE_REPLY_NPC���ظ�npc��
*	npc�Ի�������Ŀ�꣬�ͻ�������ֱ����ɣ����ﲻ��ʲô
*/
struct QuestTargetModel_ReplyNpc : public QuestTargetModel
{
	QuestTargetModel_ReplyNpc()
	{

	}

	bool parse(QuestTarget& target) const
	{
		target.uTargetNum = 1;

		// ��������
		return true;
	}

	// �ж�˳�򣬲������壬���ȽϵĶ��󣬶���Լ��
	bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const
	{
		// �����ж�
		return false;
	}

	uint32 getTargetId(const QuestTarget& target) const
	{
		return 0;
	}
};

/*
*	����ض����͵�����Ŀ����н������ж���ģ��
*	����Ŀ�����ͣ�QUEST_TARGET_TYPE_COLLECT_GOODS���ɼ���
*	// todo �����Ϊʾ�������尴Լ����д
*/
struct QuestTargetModel_CollectGoods : public QuestTargetModel
{
	QuestTargetModel_CollectGoods() : uConditionSize(1), uParameterSize(1)
	{

	}

	bool parse(QuestTarget& target) const
	{
		// У��
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_CollectGoods::parse:����������������������Լ����һ��");
			return false;
		}

		// ǰuConditionSize��Ϊ��������

		// ��uConditionSize+1��Ϊ����Ŀ������
		target.uTargetNum = target.vecCondition[uConditionSize];

		// ����Ϊ�ͻ�����չ������

		return true;
	}

	// �ж�˳�򣬲������壬���ȽϵĶ��󣬶���Լ��
	bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const
	{
		if(vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_CollectGoods:����������ƥ��!");
			return false;
		}
		if(vecParameter.size() != uParameterSize)
		{
			CLog::error("QuestTargetModel_CollectGoods:����������ƥ��!");
			return false;
		}

		if(! vecParameter[0])
		{
			CLog::error("QuestTargetModel_CollectGoods:��ָ��!");
			return false;
		}

		const CGoods* pGoods = (const CGoods*)vecParameter[0];

		// ��Ʒid����ƥ��
		if(pGoods->getId() != vecCondition[0])
		{
			return false;
		}

		return true;
	}

	uint32 getTargetId(const QuestTarget& target) const
	{
		// У��
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_CollectGoods::getTargetId:��������������Լ����һ��");
			return 0;
		}

		return target.vecCondition[0];
	}

private:
	uint32 uConditionSize;
	uint32 uParameterSize;
};

/*
*	����ض����͵�����Ŀ����н������ж���ģ��
*	����Ŀ�����ͣ�QUEST_TARGET_TYPE_DEPOT_GOODS���ռ���Ʒ��
*	// todo �����Ϊʾ�������尴Լ����д
*/
struct QuestTargetModel_DepotGoods : public QuestTargetModel
{
	QuestTargetModel_DepotGoods() : uConditionSize(1), uParameterSize(1)
	{

	}

	bool parse(QuestTarget& target) const
	{
		// У��
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_DepotGoods::parse:����������������������Լ����һ��");
			return false;
		}

		// ǰuConditionSize��Ϊ��������

		// ��uConditionSize+1��Ϊ����Ŀ������
		target.uTargetNum = target.vecCondition[uConditionSize];

		// ����Ϊ�ͻ�����չ������

		return true;
	}

	// �ж�˳�򣬲������壬���ȽϵĶ��󣬶���Լ��
	bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const
	{
		if(vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_DepotGoods:����������ƥ��!");
			return false;
		}
		if(vecParameter.size() != uParameterSize)
		{
			CLog::error("QuestTargetModel_DepotGoods:����������ƥ��!");
			return false;
		}

		if(! vecParameter[0])
		{
			CLog::error("QuestTargetModel_DepotGoods:��ָ��!");
			return false;
		}

		const CGoods* pGoods = (const CGoods*)vecParameter[0];

		// ��Ʒid����ƥ��
		if(pGoods->getId() != vecCondition[0])
		{
			return false;
		}

		return true;
	}

	uint32 getTargetId(const QuestTarget& target) const
	{
		// У��
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_DepotGoods::getTargetId:��������������Լ����һ��");
			return 0;
		}

		return target.vecCondition[0];
	}

private:
	uint32 uConditionSize;
	uint32 uParameterSize;
};

/*
*	����ض����͵�����Ŀ����н������ж���ģ��
*	����Ŀ�����ͣ�QUEST_TARGET_TYPE_MONSTER_DROP_GOODS��ɱ���ռ���Ʒ��
*	// todo �����Ϊʾ�������尴Լ����д
*/
struct QuestTargetModel_MonsterDepotGoods : public QuestTargetModel
{
	QuestTargetModel_MonsterDepotGoods() : uConditionSize(3), uParameterSize(1)
	{

	}

	bool parse(QuestTarget& target) const
	{
		// У��
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_MonsterDepotGoods::parse:����������������������Լ����һ��");
			return false;
		}

		// ��uConditionSize+1��Ϊ����Ŀ������
		target.uTargetNum = target.vecCondition[uConditionSize];

		return true;
	}

	// �ж�˳�򣬲������壬���ȽϵĶ��󣬶���Լ��
	bool judge(const QuestConditionList& vecCondition, const VoidPtrList& vecParameter) const
	{
		if(vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_MonsterDepotGoods:����������ƥ��!");
			return false;
		}
		if(vecParameter.size() != uParameterSize)
		{
			CLog::error("QuestTargetModel_MonsterDepotGoods:����������ƥ��!");
			return false;
		}

		if(! vecParameter[0])
		{
			CLog::error("QuestTargetModel_MonsterDepotGoods:��ָ��!");
			return false;
		}

		const CGoods* pGoods = (const CGoods*)vecParameter[0];

		// ��Ʒid����ƥ��
		if(pGoods->getId() != vecCondition[1])
		{
			return false;
		}

		return true;
	}

	uint32 getTargetId(const QuestTarget& target) const
	{
		// У��
		if(target.vecCondition.size() < uConditionSize + 1)
		{
			CLog::error("QuestTargetModel_MonsterDepotGoods::getTargetId:��������������Լ����һ��");
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