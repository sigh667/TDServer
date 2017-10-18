//do NOT change me manually   2017-06-01 10:50:48
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


struct _stequip_make_vigourModel
{
    uint32 uLevelStart;        //等级起始
    uint32 uVigourMax;        //活力上限
    uint32 uRecoverPerHour;        //每小时恢复数量

    _stequip_make_vigourModel& getequip_make_vigourModel()                {   return *this;                   }
    void                     initequip_make_vigourModel()               {   
        uLevelStart     = 0;
        uVigourMax     = 0;
        uRecoverPerHour     = 0;
    }

    bool readFromXml(TiXmlElement* pData)
    {

        if(!pData)
            return false;


        uLevelStart     = pData->getUInt32("LevelStart");
        uVigourMax     = pData->getUInt32("VigourMax");
        uRecoverPerHour     = pData->getUInt32("RecoverPerHour");

        return true;

    }

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uLevelStart)&&
			 stream.read(uVigourMax)&&
			 stream.read(uRecoverPerHour);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uLevelStart)&&
			 stream.write(uVigourMax)&&
			 stream.write(uRecoverPerHour);
	}
};


struct _stequip_skilledModel
{
    uint32 uSkilledLevel;        //熟练度等级
    uint32 uSkilledPoint;        //熟练度升级需要点数

    _stequip_skilledModel& getequip_skilledModel()                {   return *this;                   }
    void                     initequip_skilledModel()               {   
        uSkilledLevel     = 0;
        uSkilledPoint     = 0;
    }

    bool readFromXml(TiXmlElement* pData)
    {

        if(!pData)
            return false;


        uSkilledLevel     = pData->getUInt32("SkilledLevel");
        uSkilledPoint     = pData->getUInt32("SkilledPoint");

        return true;

    }

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uSkilledLevel)&&
			 stream.read(uSkilledPoint);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uSkilledLevel)&&
			 stream.write(uSkilledPoint);
	}
};

