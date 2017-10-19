//do NOT change me manually   2017-03-28 14:38:57
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


struct _stDazuoguajiModel
{
    uint32 uStartLevel;        //起始等级,比如表格最后一条是3等级,那么3456等都按照3来处理
    uint32 uDailyTotalExp;        //每日经验获取总量
    uint32 uSingleExp;        //单次经验获取数量
    uint32 uTeamAdditionExp;        //组队获取经验额外增加
    uint32 uDailyTotalTaolue;        //每日韬略获取总量
    uint32 uSingleTaolue;        //单次韬略获取数量
    uint32 uTeamAdditionTaolue;        //组队获取韬略额外增加

    _stDazuoguajiModel& getDazuoguajiModel()                {   return *this;                   }
    void                     initDazuoguajiModel()               {   
        uStartLevel     = 0;
        uDailyTotalExp     = 0;
        uSingleExp     = 0;
        uTeamAdditionExp     = 0;
        uDailyTotalTaolue     = 0;
        uSingleTaolue     = 0;
        uTeamAdditionTaolue     = 0;
    }

    bool readFromXml(TiXmlElement* pData)
    {

        if(!pData)
            return false;


        uStartLevel     = pData->getUInt32("StartLevel");
        uDailyTotalExp     = pData->getUInt32("DailyTotalExp");
        uSingleExp     = pData->getUInt32("SingleExp");
        uTeamAdditionExp     = pData->getUInt32("TeamAdditionExp");
        uDailyTotalTaolue     = pData->getUInt32("DailyTotalTaolue");
        uSingleTaolue     = pData->getUInt32("SingleTaolue");
        uTeamAdditionTaolue     = pData->getUInt32("TeamAdditionTaolue");

        return true;

    }

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uStartLevel)&&
			 stream.read(uDailyTotalExp)&&
			 stream.read(uSingleExp)&&
			 stream.read(uTeamAdditionExp)&&
			 stream.read(uDailyTotalTaolue)&&
			 stream.read(uSingleTaolue)&&
			 stream.read(uTeamAdditionTaolue);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uStartLevel)&&
			 stream.write(uDailyTotalExp)&&
			 stream.write(uSingleExp)&&
			 stream.write(uTeamAdditionExp)&&
			 stream.write(uDailyTotalTaolue)&&
			 stream.write(uSingleTaolue)&&
			 stream.write(uTeamAdditionTaolue);
	}
};

