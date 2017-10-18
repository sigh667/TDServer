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
    uint32 uTeamTargetID;        //ID,��Ӧ�ͻ��� �����ϵ�һ������ѡ��
    uint32 uUniqueTeamTargetID;        //UniqueID,ÿ�����Ŀ���Ψһid,�������id�ͺ�̨���� ����: TeamTargetID * 10 + Difficulty
    uint32 uActivityID;        //��Ӧ�id, �Ժ��л��, ��д��Ӧ�Ļid
    uint32 uType;        //Ŀ������, _emTeamTargetMainType 1 ���� 2 ���Ѷ�ѡ�񸱱� 3 ����
    uint32 uParam;        //����,����Ŀ�����ͽ��� 1.2,  ������д����id 3     ������д����id
    uint32 uMinLevel;        //������С�ȼ�
    uint32 uMaxLevel;        //�������ȼ�
    std::string strDifficultyDesc;        //�Ѷ�˵��
    uint32 uDifficulty;        //�Ѷ�, _emCopymapDifficulty
    std::string strIcon;        //icon
    std::string strTargetDesc;        //�˵��

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

