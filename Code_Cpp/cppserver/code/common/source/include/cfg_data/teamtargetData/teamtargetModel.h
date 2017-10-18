//do NOT change me manually   2017-03-29 15:14:29
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


#pragma pack(pop)


struct _stTeamTargetModel
{
    uint32 uTeamTargetID;        //ID,对应客户端 界面上的一个具体选项
    uint32 uUniqueTeamTargetID;        //UniqueID,每条组队目标的唯一id,请用这个id和后台交流 规则: TeamTargetID * 10 + Difficulty
    uint32 uActivityID;        //对应活动id, 以后有活动表, 填写对应的活动id
    uint32 uType;        //目标类型, _emTeamTargetMainType 1 副本 2 有难度选择副本 3 任务
    uint32 uParam;        //参数,根据目标类型解释 1.2,  这里填写副本id 3     这里填写任务id
    uint32 uMinLevel;        //允许最小等级
    uint32 uMaxLevel;        //允许最大等级
    std::string strDifficultyDesc;        //难度说明
    uint32 uDifficulty;        //难度, _emCopymapDifficulty
    std::string strIcon;        //icon
    std::string strTargetDesc;        //活动说明

    _stTeamTargetModel& getTeamTargetModel()                {   return *this;                   }
    void                     initTeamTargetModel()               {   
        uTeamTargetID     = 0;
        uUniqueTeamTargetID     = 0;
        uActivityID     = 0;
        uType     = 0;
        uParam     = 0;
        uMinLevel     = 0;
        uMaxLevel     = 0;

        uDifficulty     = 0;


    }

    bool readFromXml(TiXmlElement* pData)
    {

        if(!pData)
            return false;


        uTeamTargetID     = pData->getUInt32("TeamTargetID");
        uUniqueTeamTargetID     = pData->getUInt32("UniqueTeamTargetID");
        uActivityID     = pData->getUInt32("ActivityID");
        uType     = pData->getUInt32("Type");
        uParam     = pData->getUInt32("Param");
        uMinLevel     = pData->getUInt32("MinLevel");
        uMaxLevel     = pData->getUInt32("MaxLevel");
        strDifficultyDesc     = pData->getString("DifficultyDesc");
        uDifficulty     = pData->getUInt32("Difficulty");
        strIcon     = pData->getString("Icon");
        strTargetDesc     = pData->getString("TargetDesc");

        return true;

    }

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uTeamTargetID)&&
			 stream.read(uUniqueTeamTargetID)&&
			 stream.read(uActivityID)&&
			 stream.read(uType)&&
			 stream.read(uParam)&&
			 stream.read(uMinLevel)&&
			 stream.read(uMaxLevel)&&
			 stream.read(strDifficultyDesc)&&
			 stream.read(uDifficulty)&&
			 stream.read(strIcon)&&
			 stream.read(strTargetDesc);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uTeamTargetID)&&
			 stream.write(uUniqueTeamTargetID)&&
			 stream.write(uActivityID)&&
			 stream.write(uType)&&
			 stream.write(uParam)&&
			 stream.write(uMinLevel)&&
			 stream.write(uMaxLevel)&&
			 stream.write(strDifficultyDesc)&&
			 stream.write(uDifficulty)&&
			 stream.write(strIcon)&&
			 stream.write(strTargetDesc);
	}
};

