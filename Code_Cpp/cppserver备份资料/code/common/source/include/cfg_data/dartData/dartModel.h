//do NOT change me manually   2017-01-10 20:37:33
#pragma once
#include "def_struct/gameStruct.h"
#include "stream/streamBase.h"
#include "def_struct/stlDefine.h"
#include "basic/basicTypes.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
//-------------------------------------------------------------
//------------------------------ 
#pragma pack(push,1)

struct _stDartActiveTime
{
    uint32 wDay;
    uint32 hour;
    uint32 minute;
    uint32 lastSeconds;
    bool parse(std::string s)
    {
        wDay = (uint32)readInt64FromString(s, '|');
        hour = (uint32)readInt64FromString(s, '|');
        minute = (uint32)readInt64FromString(s, '|');
        lastSeconds = (uint32)readInt64FromString(s, '|');
        return true;
    }
    void initZero(){	memset(this,0,sizeof(*this));	}
    READ_WRITE_STRUCT
};
    
struct _stMoneyPrize
{
    uint32 moneyType;
    uint32 count;
    bool parse(std::string s)
    {
        moneyType = (uint32)readInt64FromString(s, '|');
        count = (uint32)readInt64FromString(s, '|');
        return true;
    }
    void initZero(){	memset(this,0,sizeof(*this));	}
    READ_WRITE_STRUCT
};
    
struct _stDartItem
{
    uint32 itemId;
    uint32 itemCount;
    bool parse(std::string s)
    {
        itemId = (uint32)readInt64FromString(s, '|');
        itemCount = (uint32)readInt64FromString(s, '|');
        return true;
    }
    void initZero(){	memset(this,0,sizeof(*this));	}
    READ_WRITE_STRUCT
};
    
struct _stDartCheckPoint
{
    uint32 uSceneId;
    uint32 uRouteID;
    uint32 uRobCpyMapID;
    uint32 uCellID;
    bool parse(std::string s)
    {
        uSceneId = (uint32)readInt64FromString(s, '|');
        uRouteID = (uint32)readInt64FromString(s, '|');
        uRobCpyMapID = (uint32)readInt64FromString(s, '|');
        uCellID = (uint32)readInt64FromString(s, '|');
        return true;
    }
    void initZero(){	memset(this,0,sizeof(*this));	}
    READ_WRITE_STRUCT
};
    
struct _stRobTimeDelay
{
    uint32 uMinSecond;
    uint32 uMaxSecond;
    bool parse(std::string s)
    {
        uMinSecond = (uint32)readInt64FromString(s, '|');
        uMaxSecond = (uint32)readInt64FromString(s, '|');
        return true;
    }
    void initZero(){	memset(this,0,sizeof(*this));	}
    READ_WRITE_STRUCT
};
    
struct _stRobMonster
{
    uint32 monsterId;
    uint32 monsterCount;
    uint32 CDSeconds;
    bool parse(std::string s)
    {
        monsterId = (uint32)readInt64FromString(s, '|');
        monsterCount = (uint32)readInt64FromString(s, '|');
        CDSeconds = (uint32)readInt64FromString(s, '|');
        return true;
    }
    void initZero(){	memset(this,0,sizeof(*this));	}
    READ_WRITE_STRUCT
};
    
#pragma pack(pop)


struct _stDartModel
{
    uint32 uID;        //ID
    std::string strName;        //镖车名称
    std::string strDesc;        //镖车描述
    uint32 uDartSafeType;        //镖车安全类型（1-个人安全镖；2-个人威武镖,可以pvp劫镖） _emDartPVPType
    uint32 uDartPVPType;        //镖车pvp类型 1:日常镖 怪物劫镖 2:天下镖 怪物劫镖  补货 _emDartActivityType
    uint32 uMoveSpeed;        //基础移动速度,不能从怪物表读了,请在这里设定
    stl_vector<_stDartActiveTime> vecDartActiveTime;        //活动时间,天下镖需要使用
    uint32 uDartHardType;        //镖车难度类型 1:初级 2:中级 3:高级 _emDartHardType
    uint32 uCostItemID;        //消耗道具id
    uint32 uCostItemCount;        //消耗道具数量
    uint32 uAddSupply;        //天下镖每次补货数量
    uint32 uMaxSupplyLimit;        //天下镖补货总数量限制
    uint32 uMinMember;        //最小开启人数
    uint32 uMaxMember;        //最大开启人数
    uint32 uLevelLimit;        //开启等级
    uint32 uTimeLimit;        //押镖最大时间（分钟）
    stl_vector<_stMoneyPrize> vecPrizeMoney;        //奖励银两数量（单位铜）
    uint32 uPrizeSuffer;        //历练值数量
    stl_vector<_stDartItem> vecItemPrize;        //道具奖励
    stl_vector<_stMoneyPrize> vecFailedMoney;        //失败绑银
    uint32 uRegionID;        //npc所在区域id
    stl_vector<_stDartCheckPoint> vecPath;        //第一检查开始坐标（地图id,寻路点id）
    uint32 uWeight;        //一倍权重
    uint32 uDoubleWeight;        //二倍权重
    uint32 uTripleWeight;        //三倍权重
    uint32 uCanReliveInPlane;        //位面是否不允许复活 0=允许//1=不允许
    uint32 uReliveCD;        //位面复活时间（秒）
    uint32 uRobPrepareCD;        //劫镖位面准备时间 （秒）
    uint32 uRobTimelimitMinute;        //劫镖最大时间（分钟）
    uint32 uProtectionBuffID;        //护镖成功保护BUFF ID 
    uint32 uSystemRobProbability;        //系统劫镖概率（万分比）
    _stRobTimeDelay stSystemRobCD;        //系统劫镖延迟时间（秒）
    uint32 uEasyMonsterTeam;        //简单劫镖怪物刷新组
    uint32 uNormalMonsterTeam;        //普通劫镖怪物刷新组
    uint32 uHardMonsterTeam;        //厄运劫镖怪物刷新组
    uint32 uRefForce;        //参考战力
    float32 fNormalMonsterFactor;        //小怪系数
    float32 fEliteMonsterFactor;        //精英怪系数
    float32 fBossMonsterFactor;        //BOSS系数
    float32 fDartCarFactor;        //镖车系数
    uint32 uFightDartCarMonsterID;        //战斗镖车monsterID
    std::string strPrefabPath;        //镖车模型

    _stDartModel& getDartModel()                {   return *this;                   }
    void                     initDartModel()               {   
        uID     = 0;


        uDartSafeType     = 0;
        uDartPVPType     = 0;
        uMoveSpeed     = 0;
		vecDartActiveTime.clear();
        uDartHardType     = 0;
        uCostItemID     = 0;
        uCostItemCount     = 0;
        uAddSupply     = 0;
        uMaxSupplyLimit     = 0;
        uMinMember     = 0;
        uMaxMember     = 0;
        uLevelLimit     = 0;
        uTimeLimit     = 0;
		vecPrizeMoney.clear();
        uPrizeSuffer     = 0;
		vecItemPrize.clear();
		vecFailedMoney.clear();
        uRegionID     = 0;
		vecPath.clear();
        uWeight     = 0;
        uDoubleWeight     = 0;
        uTripleWeight     = 0;
        uCanReliveInPlane     = 0;
        uReliveCD     = 0;
        uRobPrepareCD     = 0;
        uRobTimelimitMinute     = 0;
        uProtectionBuffID     = 0;
        uSystemRobProbability     = 0;
		stSystemRobCD.initZero();
        uEasyMonsterTeam     = 0;
        uNormalMonsterTeam     = 0;
        uHardMonsterTeam     = 0;
        uRefForce     = 0;
        fNormalMonsterFactor     = 0;
        fEliteMonsterFactor     = 0;
        fBossMonsterFactor     = 0;
        fDartCarFactor     = 0;
        uFightDartCarMonsterID     = 0;

    }

    bool readFromXml(TiXmlElement* pData)
    {

        if(!pData)
            return false;


        uID     = pData->getUInt32("ID");
        strName     = pData->getString("Name");
        strDesc     = pData->getString("Desc");
        uDartSafeType     = pData->getUInt32("DartSafeType");
        uDartPVPType     = pData->getUInt32("DartPVPType");
        uMoveSpeed     = pData->getUInt32("MoveSpeed");
        {
			std::string s = pData->getString("DartActiveTime");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stDartActiveTime _t;
				_t.initZero();
				_t.parse(_s);
				vecDartActiveTime.push_back(_t);
			}
		}
        uDartHardType     = pData->getUInt32("DartHardType");
        uCostItemID     = pData->getUInt32("CostItemID");
        uCostItemCount     = pData->getUInt32("CostItemCount");
        uAddSupply     = pData->getUInt32("AddSupply");
        uMaxSupplyLimit     = pData->getUInt32("MaxSupplyLimit");
        uMinMember     = pData->getUInt32("MinMember");
        uMaxMember     = pData->getUInt32("MaxMember");
        uLevelLimit     = pData->getUInt32("LevelLimit");
        uTimeLimit     = pData->getUInt32("TimeLimit");
        {
			std::string s = pData->getString("PrizeMoney");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stMoneyPrize _t;
				_t.initZero();
				_t.parse(_s);
				vecPrizeMoney.push_back(_t);
			}
		}
        uPrizeSuffer     = pData->getUInt32("PrizeSuffer");
        {
			std::string s = pData->getString("ItemPrize");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stDartItem _t;
				_t.initZero();
				_t.parse(_s);
				vecItemPrize.push_back(_t);
			}
		}
        {
			std::string s = pData->getString("FailedMoney");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stMoneyPrize _t;
				_t.initZero();
				_t.parse(_s);
				vecFailedMoney.push_back(_t);
			}
		}
        uRegionID     = pData->getUInt32("RegionID");
        {
			std::string s = pData->getString("Path");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stDartCheckPoint _t;
				_t.initZero();
				_t.parse(_s);
				vecPath.push_back(_t);
			}
		}
        uWeight     = pData->getUInt32("Weight");
        uDoubleWeight     = pData->getUInt32("DoubleWeight");
        uTripleWeight     = pData->getUInt32("TripleWeight");
        uCanReliveInPlane     = pData->getUInt32("CanReliveInPlane");
        uReliveCD     = pData->getUInt32("ReliveCD");
        uRobPrepareCD     = pData->getUInt32("RobPrepareCD");
        uRobTimelimitMinute     = pData->getUInt32("RobTimelimitMinute");
        uProtectionBuffID     = pData->getUInt32("ProtectionBuffID");
        uSystemRobProbability     = pData->getUInt32("SystemRobProbability");
        {
			std::string s = pData->getString("SystemRobCD");
			stSystemRobCD.parse(s);
		}
        uEasyMonsterTeam     = pData->getUInt32("EasyMonsterTeam");
        uNormalMonsterTeam     = pData->getUInt32("NormalMonsterTeam");
        uHardMonsterTeam     = pData->getUInt32("HardMonsterTeam");
        uRefForce     = pData->getUInt32("RefForce");
        fNormalMonsterFactor     = pData->getFloat("NormalMonsterFactor");
        fEliteMonsterFactor     = pData->getFloat("EliteMonsterFactor");
        fBossMonsterFactor     = pData->getFloat("BossMonsterFactor");
        fDartCarFactor     = pData->getFloat("DartCarFactor");
        uFightDartCarMonsterID     = pData->getUInt32("FightDartCarMonsterID");
        strPrefabPath     = pData->getString("PrefabPath");

        return true;

    }

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uID)&&
			 stream.read(strName)&&
			 stream.read(strDesc)&&
			 stream.read(uDartSafeType)&&
			 stream.read(uDartPVPType)&&
			 stream.read(uMoveSpeed)&&
			 stream.read(vecDartActiveTime)&&
			 stream.read(uDartHardType)&&
			 stream.read(uCostItemID)&&
			 stream.read(uCostItemCount)&&
			 stream.read(uAddSupply)&&
			 stream.read(uMaxSupplyLimit)&&
			 stream.read(uMinMember)&&
			 stream.read(uMaxMember)&&
			 stream.read(uLevelLimit)&&
			 stream.read(uTimeLimit)&&
			 stream.read(vecPrizeMoney)&&
			 stream.read(uPrizeSuffer)&&
			 stream.read(vecItemPrize)&&
			 stream.read(vecFailedMoney)&&
			 stream.read(uRegionID)&&
			 stream.read(vecPath)&&
			 stream.read(uWeight)&&
			 stream.read(uDoubleWeight)&&
			 stream.read(uTripleWeight)&&
			 stream.read(uCanReliveInPlane)&&
			 stream.read(uReliveCD)&&
			 stream.read(uRobPrepareCD)&&
			 stream.read(uRobTimelimitMinute)&&
			 stream.read(uProtectionBuffID)&&
			 stream.read(uSystemRobProbability)&&
			 stream.read(stSystemRobCD)&&
			 stream.read(uEasyMonsterTeam)&&
			 stream.read(uNormalMonsterTeam)&&
			 stream.read(uHardMonsterTeam)&&
			 stream.read(uRefForce)&&
			 stream.read(fNormalMonsterFactor)&&
			 stream.read(fEliteMonsterFactor)&&
			 stream.read(fBossMonsterFactor)&&
			 stream.read(fDartCarFactor)&&
			 stream.read(uFightDartCarMonsterID)&&
			 stream.read(strPrefabPath);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uID)&&
			 stream.write(strName)&&
			 stream.write(strDesc)&&
			 stream.write(uDartSafeType)&&
			 stream.write(uDartPVPType)&&
			 stream.write(uMoveSpeed)&&
			 stream.write(vecDartActiveTime)&&
			 stream.write(uDartHardType)&&
			 stream.write(uCostItemID)&&
			 stream.write(uCostItemCount)&&
			 stream.write(uAddSupply)&&
			 stream.write(uMaxSupplyLimit)&&
			 stream.write(uMinMember)&&
			 stream.write(uMaxMember)&&
			 stream.write(uLevelLimit)&&
			 stream.write(uTimeLimit)&&
			 stream.write(vecPrizeMoney)&&
			 stream.write(uPrizeSuffer)&&
			 stream.write(vecItemPrize)&&
			 stream.write(vecFailedMoney)&&
			 stream.write(uRegionID)&&
			 stream.write(vecPath)&&
			 stream.write(uWeight)&&
			 stream.write(uDoubleWeight)&&
			 stream.write(uTripleWeight)&&
			 stream.write(uCanReliveInPlane)&&
			 stream.write(uReliveCD)&&
			 stream.write(uRobPrepareCD)&&
			 stream.write(uRobTimelimitMinute)&&
			 stream.write(uProtectionBuffID)&&
			 stream.write(uSystemRobProbability)&&
			 stream.write(stSystemRobCD)&&
			 stream.write(uEasyMonsterTeam)&&
			 stream.write(uNormalMonsterTeam)&&
			 stream.write(uHardMonsterTeam)&&
			 stream.write(uRefForce)&&
			 stream.write(fNormalMonsterFactor)&&
			 stream.write(fEliteMonsterFactor)&&
			 stream.write(fBossMonsterFactor)&&
			 stream.write(fDartCarFactor)&&
			 stream.write(uFightDartCarMonsterID)&&
			 stream.write(strPrefabPath);
	}
};


struct _stDartBuffModel
{
    uint32 uID;        //ID
    std::string strName;        //名字
    std::string strDesc;        //描述
    uint32 uType;        //功能类型 _emDartBuffType
    int32 uPara1;        //参数1
    std::string strIcon;        //美术资源
    uint32 uLayerType;        //叠加规则 _emDartBuffLayerType 1:叠加 2:替换
    uint32 uEffectiveCD;        //持续时间,秒数

    _stDartBuffModel& getDartBuffModel()                {   return *this;                   }
    void                     initDartBuffModel()               {   
        uID     = 0;


        uType     = 0;
        uPara1     = 0;

        uLayerType     = 0;
        uEffectiveCD     = 0;
    }

    bool readFromXml(TiXmlElement* pData)
    {

        if(!pData)
            return false;


        uID     = pData->getUInt32("ID");
        strName     = pData->getString("Name");
        strDesc     = pData->getString("Desc");
        uType     = pData->getUInt32("Type");
        uPara1     = pData->getInt32("Para1");
        strIcon     = pData->getString("Icon");
        uLayerType     = pData->getUInt32("LayerType");
        uEffectiveCD     = pData->getUInt32("EffectiveCD");

        return true;

    }

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uID)&&
			 stream.read(strName)&&
			 stream.read(strDesc)&&
			 stream.read(uType)&&
			 stream.read(uPara1)&&
			 stream.read(strIcon)&&
			 stream.read(uLayerType)&&
			 stream.read(uEffectiveCD);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uID)&&
			 stream.write(strName)&&
			 stream.write(strDesc)&&
			 stream.write(uType)&&
			 stream.write(uPara1)&&
			 stream.write(strIcon)&&
			 stream.write(uLayerType)&&
			 stream.write(uEffectiveCD);
	}
};


struct _stRobMonsterModel
{
    uint32 uID;        //镖车id
    stl_vector<_stRobMonster> vecRobMonster;        //怪物信息, struct _stRobMonster{  uint32 monsterId;    怪物id  uint32 monsterCount; 怪物数量  uint32 CDSeconds;    上一波刷怪以后等待时间,刷这一波怪物 }RobMonster[5];       这个5不用在意, 超过5个也可以

    _stRobMonsterModel& getRobMonsterModel()                {   return *this;                   }
    void                     initRobMonsterModel()               {   
        uID     = 0;
		vecRobMonster.clear();
    }

    bool readFromXml(TiXmlElement* pData)
    {

        if(!pData)
            return false;


        uID     = pData->getUInt32("ID");
        {
			std::string s = pData->getString("RobMonster");
			//for(int i = 0; i < 5; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stRobMonster _t;
				_t.initZero();
				_t.parse(_s);
				vecRobMonster.push_back(_t);
			}
		}

        return true;

    }

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uID)&&
			 stream.read(vecRobMonster);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uID)&&
			 stream.write(vecRobMonster);
	}
};

