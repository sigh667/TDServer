//do NOT change me manually   2017-06-23 17:08:23
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

struct _stLuckDrawID
{
	uint32 drawID;
	bool parse(std::string s)
	{
		drawID = (uint32)readInt64FromString(s, '|');
		return true;
	}
	void initZero(){	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
	
#pragma pack(pop)


struct _stClimbPrizeModel
{
	uint32 uCopyMapID;		//����ID
	std::string strBossName;		//BOSS����
	uint32 uType;		//����  �����Ѷ� _emClimbTowerClass
	uint32 uRanking;		//����,�����������ʼ
	uint32 uTotalRanking;		//��������,����Ϳ�ʼ,��̨��
	stl_vector<_stLuckDrawID> vecLuckDrawID;		//��Ӧ����ID ID1;ID2;ID3��
	uint32 uWeekDrop;		//�ܽ��������,�ӵ�������ѯ

	_stClimbPrizeModel& getClimbPrizeModel()				{   return *this;				   }
	void					 initClimbPrizeModel()			   {   
		uCopyMapID	 = 0;

		uType	 = 0;
		uRanking	 = 0;
		uTotalRanking	 = 0;
		vecLuckDrawID.clear();
		uWeekDrop	 = 0;
	}

	bool readFromXml(TiXmlElement* pData)
	{

		if(!pData)
			return false;


		uCopyMapID	 = pData->getUInt32("CopyMapID");
		strBossName	 = pData->getString("BossName");
		uType	 = pData->getUInt32("Type");
		uRanking	 = pData->getUInt32("Ranking");
		uTotalRanking	 = pData->getUInt32("TotalRanking");
		{
			std::string s = pData->getString("LuckDrawID");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stLuckDrawID _t;
				_t.initZero();
				_t.parse(_s);
				vecLuckDrawID.push_back(_t);
			}
		}
		uWeekDrop	 = pData->getUInt32("WeekDrop");

		return true;

	}

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uCopyMapID)&&
			 stream.read(strBossName)&&
			 stream.read(uType)&&
			 stream.read(uRanking)&&
			 stream.read(uTotalRanking)&&
			 stream.read(vecLuckDrawID)&&
			 stream.read(uWeekDrop);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uCopyMapID)&&
			 stream.write(strBossName)&&
			 stream.write(uType)&&
			 stream.write(uRanking)&&
			 stream.write(uTotalRanking)&&
			 stream.write(vecLuckDrawID)&&
			 stream.write(uWeekDrop);
	}
};


struct _stClimbLuckDrawModel
{
	uint32 uID;		//��ˮID
	uint32 uPrizeID;		//����ID
	uint32 uType;		//���� _emClimbTowerPrizeType
	uint32 uKeyType;		//��ֵ
	uint32 uCount;		//����

	_stClimbLuckDrawModel& getClimbLuckDrawModel()				{   return *this;				   }
	void					 initClimbLuckDrawModel()			   {   
		uID	 = 0;
		uPrizeID	 = 0;
		uType	 = 0;
		uKeyType	 = 0;
		uCount	 = 0;
	}

	bool readFromXml(TiXmlElement* pData)
	{

		if(!pData)
			return false;


		uID	 = pData->getUInt32("ID");
		uPrizeID	 = pData->getUInt32("PrizeID");
		uType	 = pData->getUInt32("Type");
		uKeyType	 = pData->getUInt32("KeyType");
		uCount	 = pData->getUInt32("Count");

		return true;

	}

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uID)&&
			 stream.read(uPrizeID)&&
			 stream.read(uType)&&
			 stream.read(uKeyType)&&
			 stream.read(uCount);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uID)&&
			 stream.write(uPrizeID)&&
			 stream.write(uType)&&
			 stream.write(uKeyType)&&
			 stream.write(uCount);
	}
};

