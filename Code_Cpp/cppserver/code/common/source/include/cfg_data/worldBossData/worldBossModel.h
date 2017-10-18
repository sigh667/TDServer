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
    uint32 uBossActivityId;        //bossid,��boss������id,
    uint32 uMonsterId;        //monsterid
    std::string strName;        //boss����
    std::string strIcon;        //bossͷ��
    std::string strPlace;        //�Ƽ��ص�
    uint32 uLevel;        //�Ƽ��ȼ�
    std::string strMiniDescription;        //boss����
    std::string strDescription;        //boss����
    std::string strRewardShow;        //������ʾ itemid1_num;itemid2_num; ǰ�˴����²���ʾ����
    _stDailyWorldBossTime stDailyWorldBossTimeBegin;        //ÿ��ˢ��ʱ��[��ʼ]
    _stDailyWorldBossTime stDailyWorldBossTimeEnd;        //ÿ��ˢ��ʱ��[����]
    uint32 uRebornTime;        //�����Ժ��������,����
    uint32 uSceneId;        //ˢ�³�����ͼid
    uint32 uCellId;        //��������id
    _stPoint stBossPoint;        //ˢ�³�������
    uint32 uEnterRegionId;        //��ת���������
    std::string strRullDescription;        //����˵��

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
    uint32 uRankStart;        //������ʼ
    uint32 uPrizeId;        //����id

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

