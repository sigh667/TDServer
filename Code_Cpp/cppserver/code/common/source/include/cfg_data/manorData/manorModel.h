//do NOT change me manually   2017-07-17 20:57:00
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

struct _stSubCopymap
{
	uint32 RegionID;
	uint32 CopymapID;
	bool parse(std::string s)
	{
		RegionID = (uint32)readInt64FromString(s, '|');
		CopymapID = (uint32)readInt64FromString(s, '|');
		return true;
	}
	void initZero(){	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
	
struct _stActiveTime
{
	uint32 wDay;
	uint32 hour;
	uint32 minute;
	bool parse(std::string s)
	{
		wDay = (uint32)readInt64FromString(s, '|');
		hour = (uint32)readInt64FromString(s, '|');
		minute = (uint32)readInt64FromString(s, '|');
		return true;
	}
	void initZero(){	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
	
struct _stProducts
{
	uint32 uItemID;
	uint32 uCount;
	uint32 uBind;
	bool parse(std::string s)
	{
		uItemID = (uint32)readInt64FromString(s, '|');
		uCount = (uint32)readInt64FromString(s, '|');
		uBind = (uint32)readInt64FromString(s, '|');
		return true;
	}
	void initZero(){	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
	
#pragma pack(pop)


struct _stmanorModel
{
	uint32 uID;		//ID
	std::string strName;		//领地名
	uint32 uCorpsId;		//所属军团id _emCorps {  Corps_Unknown =0;  Corps_East  =1;//龙翎卫  Corps_West  =2;//天策营  Corps_South  =3;//虎豹骑  Corps_north  =4;//羽林军
	uint32 uLobbyCmpMapId;		//对应大厅的副本id
	stl_vector<_stSubCopymap> vecBattleField;		//对应具体战场的副本,一共6个,副本id允许一样,必须填满6个
	_stActiveTime stStartApplyTime;		//报名开始时间
	_stActiveTime stStopApplyTime;		//报名结束时间
	_stActiveTime stStartMobaTime;		//领地战开始时间
	_stActiveTime stStopMobaTime;		//领地战结束时间
	std::string strSceneId;		//场景id
	std::string strContainerName;		//世界地图元件名字
	uint32 uDailyPartyjewel;		//每日产出帮派玉石
	uint32 uDailyPartyMoney;		//每日产出帮派资金
	stl_vector<_stProducts> vecSpecialty;		//地盘特产 填道具ID，支持多个，通用格式
	uint32 uType;		//领地类型 1=首府 2=普通 _emManorType
	stl_vector<_stProducts> vecFactionWinPrize;		//帮派奖励,领地胜利
	stl_vector<_stProducts> vecFactionLosePrize;		//帮派奖励,领地失败
	stl_vector<_stProducts> vecFactionWage;		//帮派奖励,周俸禄
	stl_vector<_stProducts> vecMemberWinPrize;		//个人奖励,领地胜利
	stl_vector<_stProducts> vecMemberLosePrize;		//个人奖励,领地失败
	stl_vector<_stProducts> vecMemberScorePrize10;		//个人奖励,贡献前10名
	stl_vector<_stProducts> vecMemberScorePrize11;		//个人奖励,贡献11名之后

	_stmanorModel& getmanorModel()				{   return *this;				   }
	void					 initmanorModel()			   {   
		uID	 = 0;

		uCorpsId	 = 0;
		uLobbyCmpMapId	 = 0;
		vecBattleField.clear();
		stStartApplyTime.initZero();
		stStopApplyTime.initZero();
		stStartMobaTime.initZero();
		stStopMobaTime.initZero();


		uDailyPartyjewel	 = 0;
		uDailyPartyMoney	 = 0;
		vecSpecialty.clear();
		uType	 = 0;
		vecFactionWinPrize.clear();
		vecFactionLosePrize.clear();
		vecFactionWage.clear();
		vecMemberWinPrize.clear();
		vecMemberLosePrize.clear();
		vecMemberScorePrize10.clear();
		vecMemberScorePrize11.clear();
	}

	bool readFromXml(TiXmlElement* pData)
	{

		if(!pData)
			return false;


		uID	 = pData->getUInt32("ID");
		strName	 = pData->getString("Name");
		uCorpsId	 = pData->getUInt32("CorpsId");
		uLobbyCmpMapId	 = pData->getUInt32("LobbyCmpMapId");
		{
			std::string s = pData->getString("BattleField");
			//for(int i = 0; i < 6; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stSubCopymap _t;
				_t.initZero();
				_t.parse(_s);
				vecBattleField.push_back(_t);
			}
		}
		{
			std::string s = pData->getString("StartApplyTime");
			stStartApplyTime.parse(s);
		}
		{
			std::string s = pData->getString("StopApplyTime");
			stStopApplyTime.parse(s);
		}
		{
			std::string s = pData->getString("StartMobaTime");
			stStartMobaTime.parse(s);
		}
		{
			std::string s = pData->getString("StopMobaTime");
			stStopMobaTime.parse(s);
		}
		strSceneId	 = pData->getString("SceneId");
		strContainerName	 = pData->getString("ContainerName");
		uDailyPartyjewel	 = pData->getUInt32("DailyPartyjewel");
		uDailyPartyMoney	 = pData->getUInt32("DailyPartyMoney");
		{
			std::string s = pData->getString("Specialty");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stProducts _t;
				_t.initZero();
				_t.parse(_s);
				vecSpecialty.push_back(_t);
			}
		}
		uType	 = pData->getUInt32("Type");
		{
			std::string s = pData->getString("FactionWinPrize");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stProducts _t;
				_t.initZero();
				_t.parse(_s);
				vecFactionWinPrize.push_back(_t);
			}
		}
		{
			std::string s = pData->getString("FactionLosePrize");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stProducts _t;
				_t.initZero();
				_t.parse(_s);
				vecFactionLosePrize.push_back(_t);
			}
		}
		{
			std::string s = pData->getString("FactionWage");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stProducts _t;
				_t.initZero();
				_t.parse(_s);
				vecFactionWage.push_back(_t);
			}
		}
		{
			std::string s = pData->getString("MemberWinPrize");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stProducts _t;
				_t.initZero();
				_t.parse(_s);
				vecMemberWinPrize.push_back(_t);
			}
		}
		{
			std::string s = pData->getString("MemberLosePrize");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stProducts _t;
				_t.initZero();
				_t.parse(_s);
				vecMemberLosePrize.push_back(_t);
			}
		}
		{
			std::string s = pData->getString("MemberScorePrize10");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stProducts _t;
				_t.initZero();
				_t.parse(_s);
				vecMemberScorePrize10.push_back(_t);
			}
		}
		{
			std::string s = pData->getString("MemberScorePrize11");
			//for(int i = 0; i < 3; i++)
			while(1){
				std::string _s;
				getSubString(s, _s, ';');
				if (_s.empty())
				{
					break;
				}
				_stProducts _t;
				_t.initZero();
				_t.parse(_s);
				vecMemberScorePrize11.push_back(_t);
			}
		}

		return true;

	}

	inline bool		read			(CStreamBase& stream)
	{
		return  stream.read(uID)&&
			 stream.read(strName)&&
			 stream.read(uCorpsId)&&
			 stream.read(uLobbyCmpMapId)&&
			 stream.read(vecBattleField)&&
			 stream.read(stStartApplyTime)&&
			 stream.read(stStopApplyTime)&&
			 stream.read(stStartMobaTime)&&
			 stream.read(stStopMobaTime)&&
			 stream.read(strSceneId)&&
			 stream.read(strContainerName)&&
			 stream.read(uDailyPartyjewel)&&
			 stream.read(uDailyPartyMoney)&&
			 stream.read(vecSpecialty)&&
			 stream.read(uType)&&
			 stream.read(vecFactionWinPrize)&&
			 stream.read(vecFactionLosePrize)&&
			 stream.read(vecFactionWage)&&
			 stream.read(vecMemberWinPrize)&&
			 stream.read(vecMemberLosePrize)&&
			 stream.read(vecMemberScorePrize10)&&
			 stream.read(vecMemberScorePrize11);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return  stream.write(uID)&&
			 stream.write(strName)&&
			 stream.write(uCorpsId)&&
			 stream.write(uLobbyCmpMapId)&&
			 stream.write(vecBattleField)&&
			 stream.write(stStartApplyTime)&&
			 stream.write(stStopApplyTime)&&
			 stream.write(stStartMobaTime)&&
			 stream.write(stStopMobaTime)&&
			 stream.write(strSceneId)&&
			 stream.write(strContainerName)&&
			 stream.write(uDailyPartyjewel)&&
			 stream.write(uDailyPartyMoney)&&
			 stream.write(vecSpecialty)&&
			 stream.write(uType)&&
			 stream.write(vecFactionWinPrize)&&
			 stream.write(vecFactionLosePrize)&&
			 stream.write(vecFactionWage)&&
			 stream.write(vecMemberWinPrize)&&
			 stream.write(vecMemberLosePrize)&&
			 stream.write(vecMemberScorePrize10)&&
			 stream.write(vecMemberScorePrize11);
	}
};

