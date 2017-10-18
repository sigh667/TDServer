//do NOT change me manually   2017-05-22 15:25:03
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

struct _stDailyWorldBossTime
{
    uint32 hour;
    uint32 minute;
    uint32 second;
    bool parse(std::string s)
    {
        hour = (uint32)readInt64FromString(s, '|');
        minute = (uint32)readInt64FromString(s, '|');
        second = (uint32)readInt64FromString(s, '|');
        return true;
    }
    void initZero(){	memset(this,0,sizeof(*this));	}
    READ_WRITE_STRUCT
};
    
struct _stPoint
{
    uint32 x;
    uint32 y;
    bool parse(std::string s)
    {
        x = (uint32)readInt64FromString(s, '|');
        y = (uint32)readInt64FromString(s, '|');
        return true;
    }
    void initZero(){	memset(this,0,sizeof(*this));	}
    READ_WRITE_STRUCT
};
    
#pragma pack(pop)


struct _stworldbossModel
{
    uint32 uBossActivityId;        //bossid,是boss这个活动的id,
    uint32 uMonsterId;        //monsterid
    std::string strName;        //boss名字
    std::string strIcon;        //boss头像
    std::string strPlace;        //推荐地点
    uint32 uLevel;        //推荐等级
    std::string strMiniDescription;        //boss简述
    std::string strDescription;        //boss详述
    std::string strRewardShow;        //奖励显示 itemid1_num;itemid2_num; 前端处理下不显示数量
    _stDailyWorldBossTime stDailyWorldBossTimeBegin;        //每日刷新时间[开始]
    _stDailyWorldBossTime stDailyWorldBossTimeEnd;        //每日刷新时间[结束]
    uint32 uRebornTime;        //死亡以后重生间隔,分钟
    uint32 uSceneId;        //刷新场景地图id
    uint32 uCellId;        //场景的线id
    _stPoint stBossPoint;        //刷新场景坐标
    uint32 uEnterRegionId;        //跳转进入点区域
    std::string strRullDescription;        //规则说明

    _stworldbossModel& getworldbossModel()                {   return *this;                   }
    void                     initworldbossModel()               {   
        uBossActivityId     = 0;
        uMonsterId     = 0;



        uLevel     = 0;



		stDailyWorldBossTimeBegin.initZero();
		stDailyWorldBossTimeEnd.initZero();
        uRebornTime     = 0;
        uSceneId     = 0;
        uCellId     = 0;
		stBossPoint.initZero();
        uEnterRegionId     = 0;

    }

    bool readFromXml(TiXmlElement* pData)
    {

        if(!pData)
            return false;


        uBossActivityId     = pData->getUInt32("BossActivityId");
        uMonsterId     = pData->getUInt32("MonsterId");
        strName     = pData->getString("Name");
        strIcon     = pData->getString("Icon");
        strPlace     = pData->getString("Place");
        uLevel     = pData->getUInt32("Level");
        strMiniDescription     = pData->getString("MiniDescription");
        strDescription     = pData->getString("Description");
        strRewardShow     = pData->getString("RewardShow");
        {
			std::string s = pData->getString("DailyWorldBossTimeBegin");
			stDailyWorldBossTimeBegin.parse(s);
		}
        {
			std::string s = pData->getString("DailyWorldBossTimeEnd");
			stDailyWorldBossTimeEnd.parse(s);
		}
        uRebornTime     = pData->getUInt32("RebornTime");
        uSceneId     = pData->getUInt32("SceneId");
        uCellId     = pData->getUInt32("CellId");
        {
			std::string s = pData->getString("BossPoint");
			stBossPoint.parse(s);
		}
        uEnterRegionId     = pData->getUInt32("EnterRegionId");
        strRullDescription     = pData->getString("RullDescription");

        return true;

    }

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uBossActivityId)&&
			 stream.read(uMonsterId)&&
			 stream.read(strName)&&
			 stream.read(strIcon)&&
			 stream.read(strPlace)&&
			 stream.read(uLevel)&&
			 stream.read(strMiniDescription)&&
			 stream.read(strDescription)&&
			 stream.read(strRewardShow)&&
			 stream.read(stDailyWorldBossTimeBegin)&&
			 stream.read(stDailyWorldBossTimeEnd)&&
			 stream.read(uRebornTime)&&
			 stream.read(uSceneId)&&
			 stream.read(uCellId)&&
			 stream.read(stBossPoint)&&
			 stream.read(uEnterRegionId)&&
			 stream.read(strRullDescription);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uBossActivityId)&&
			 stream.write(uMonsterId)&&
			 stream.write(strName)&&
			 stream.write(strIcon)&&
			 stream.write(strPlace)&&
			 stream.write(uLevel)&&
			 stream.write(strMiniDescription)&&
			 stream.write(strDescription)&&
			 stream.write(strRewardShow)&&
			 stream.write(stDailyWorldBossTimeBegin)&&
			 stream.write(stDailyWorldBossTimeEnd)&&
			 stream.write(uRebornTime)&&
			 stream.write(uSceneId)&&
			 stream.write(uCellId)&&
			 stream.write(stBossPoint)&&
			 stream.write(uEnterRegionId)&&
			 stream.write(strRullDescription);
	}
};


struct _stworldbossrankprizeModel
{
    uint32 uBossActivityId;        //bossid
    uint32 uRankStart;        //排名起始
    uint32 uPrizeId;        //奖励id

    _stworldbossrankprizeModel& getworldbossrankprizeModel()                {   return *this;                   }
    void                     initworldbossrankprizeModel()               {   
        uBossActivityId     = 0;
        uRankStart     = 0;
        uPrizeId     = 0;
    }

    bool readFromXml(TiXmlElement* pData)
    {

        if(!pData)
            return false;


        uBossActivityId     = pData->getUInt32("BossActivityId");
        uRankStart     = pData->getUInt32("RankStart");
        uPrizeId     = pData->getUInt32("PrizeId");

        return true;

    }

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uBossActivityId)&&
			 stream.read(uRankStart)&&
			 stream.read(uPrizeId);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uBossActivityId)&&
			 stream.write(uRankStart)&&
			 stream.write(uPrizeId);
	}

};

