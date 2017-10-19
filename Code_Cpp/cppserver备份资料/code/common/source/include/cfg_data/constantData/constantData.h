/*----------------- constantData.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/1/7 13:59:47
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "protocol/ConstantDefine.pb.h"
#include "stl/std_map.h"
/*************************************************************/
class CConstantData  	: public CFileData
{
public:
	enum
	{
		Version	= 2016010701
	};

private:
	stl_map<uint32,int32>			m_mapConstant;

public:
#define _declare_param(_sys, _param)		int32	_sys##_##_param
	// Constant_Game
	_declare_param(Game, MaxPlayerNum       );
	_declare_param(Game, MaxLevel           );
	_declare_param(Game, LeaveFighting      );
	_declare_param(Game, RushSpeed          );
	_declare_param(Game, FlySpeed           );
	_declare_param(Game, BlackPK            );
	_declare_param(Game, RedPK              );
	_declare_param(Game, HatredBuffHurt     );
	_declare_param(Game, HatredBuffCure     );
	_declare_param(Game, HatredSkillCure    );
	_declare_param(Game, HatredSkillGain    );
	_declare_param(Game, MaxBagItemNum      );
	_declare_param(Game, EnermyMax          );
	_declare_param(Game, GloBalCacheLevel   );
	_declare_param(Game, AIFollowOffset     );
	_declare_param(Game, RebirthLevel       );
	_declare_param(Game, KillTime           );
	_declare_param(Game, KillNotAddPK       );
	_declare_param(Game, AddPKValue         );
	_declare_param(Game, ReconnetTime       );
	_declare_param(Game, BingfuRepeatNum    );
	_declare_param(Game, FlyByWind          );
	_declare_param(Game, AutoReliveTime     );
	_declare_param(Game, WaitAutoReliveTime );
	_declare_param(Game, StoreScoreRate      );
	_declare_param(Game, BingfuBagMaxNum    );
	_declare_param(Game, ChatJianghuRate    );
	_declare_param(Game, WuxueMaxSchemeNum  );
	_declare_param(Game, WuxueMaxSkillNum   );
	_declare_param(Game, FightTimeUseSkill	);
	_declare_param(Game, FightTimeAttackPlayer);
	_declare_param(Game, FightTimeBeAttackPlayer);
	_declare_param(Game, FightTimeAttackMonster);
	_declare_param(Game, FightTimeBeAttackMonster);
	_declare_param(Game, FightTimeHatred	);
	_declare_param(Game, FightTimeBeHatred	);
	_declare_param(Game, SaleMoneyScoreRate	);
	_declare_param(Game, CheckNameNum	);
	_declare_param(Game, FinalDamageRand_Low	);
	_declare_param(Game, FinalDamageRand_High	);
	_declare_param(Game, MailMax_Svr	); 
	_declare_param(Game, MailExpirationTime	);
	_declare_param(Game, DazuoBuffId	);
	_declare_param(Game, DazuoGuajiBuffId	);
	_declare_param(Game, MaxBingfuBagNum    );
	_declare_param(Game, LogciDay_Delay     );
	_declare_param(Game, Activity_Clear_Time );
	_declare_param(Game, Corps_DeclarLength	);
	_declare_param(Game, MailItemMax		);

	// Constant_Quest
	_declare_param(Quest, DailyQueseCount	);
	_declare_param(Quest, MasterQueseMax);
	// Constant_Skill
	_declare_param(Skill, MaxBuffCount		);
	_declare_param(Skill, EnemyClearTime	);
	_declare_param(Skill, TrapAngleSpeed	);
	_declare_param(Skill, BeAttackAddBuff	);
	_declare_param(Skill, SkillSchemeNum	);
	// Constant_Trade
	_declare_param(Trade, ItemCount		);
	_declare_param(Trade, RecoverTime	);
	_declare_param(Trade, LockCoolTime	);
	// Constant_SaleMoney
	_declare_param(SaleMoney, UnitCash		);
	_declare_param(SaleMoney, CancelTime	);
	_declare_param(SaleMoney, AveDay		);
	_declare_param(SaleMoney, ListCount		);
	_declare_param(SaleMoney, DefaultAve	);
	_declare_param(SaleMoney, TaxCash		);
	_declare_param(SaleMoney, TaxSliver		);
	_declare_param(SaleMoney, AveMin		);
	_declare_param(SaleMoney, AveMax		);
	_declare_param(SaleMoney, LogCount		);
	_declare_param(SaleMoney, P1_LimitVaue	);
	_declare_param(SaleMoney, P2_LimitVaue	);
	_declare_param(SaleMoney, P1_Weight		);
	_declare_param(SaleMoney, P2_Weight		);
	_declare_param(SaleMoney, PriceHours	);
	_declare_param(SaleMoney, AmountHours	);
	_declare_param(SaleMoney, IntervalMin	);
	_declare_param(SaleMoney, IntervalMax	);
	_declare_param(SaleMoney, IntervalExtra	);
	_declare_param(SaleMoney, TradeLogCount	);
	// Constant_Auction
	_declare_param(Auction, CancelTime		);
	_declare_param(Auction, LogCount		);
	_declare_param(Auction, P1_LimitVaue	);
	_declare_param(Auction, P2_LimitVaue	);
	_declare_param(Auction, P1_Weight		);
	_declare_param(Auction, P2_Weight		);
	_declare_param(Auction, TaxBase			);
	_declare_param(Auction, TaxExtra		);
	// Constant_Friend
	_declare_param(Friend, FriendApply_Time		);
	_declare_param(Friend, FriendCountLimit		);
	_declare_param(Friend, EnemyCountLimit		);
	_declare_param(Friend, TmpFriendCountLimit	);
	_declare_param(Friend, BlacklistCountLimit	);
	_declare_param(Friend, TmpFriendTimeOut		);
	// Constant_WenTao
	_declare_param(WenTao, Scheme_Count		);
	// Constant_Team
	_declare_param(Team, Member_Count		);
	_declare_param(Team, KickVote_Time		);
	_declare_param(Team, AllotVote_Time		);
	_declare_param(Team, Callup_CoolTime	);
	_declare_param(Team, Around_Distance	);
	_declare_param(Team, ShoulieLoopNum		);
	_declare_param(Team, ShouliePrizeCount	);
	_declare_param(Team, RemainConfirmTime	);

	// Constant_DropItem
	_declare_param(DropItem, Dice_Time		);
	_declare_param(DropItem, Assign_Time	);
	_declare_param(DropItem, Pick_Time		);
	_declare_param(DropItem, Exist_Time		);
	// Constant_Item
	_declare_param(Item, Inherit_Enhance	);
	_declare_param(Item, Inherit_Forge		);
	_declare_param(Item, Inherit_Special	);
	_declare_param(Item, EnhanceMoneyCount	);
	_declare_param(Item, ForgeMoneyCount	);
	_declare_param(Item, BingfuSplitAddPercent	);

	// Constant_Faction
	_declare_param(Faction, Create_Money		);
	_declare_param(Faction, Name_Size			);
	_declare_param(Faction, Banner_Size			);
	_declare_param(Faction, Declaration_Size	);
	_declare_param(Faction, Apply_Max			);
	_declare_param(Faction, Player_Apply_Max	);
	_declare_param(Faction, Apply_OutOfTime		);
	_declare_param(Faction, Apply_CD			);
	_declare_param(Faction, Leave_CD			);
	_declare_param(Faction, Invite_Timeout		);
	_declare_param(Faction, Develop_Count		);
	_declare_param(Faction, Study_Count			);
	_declare_param(Faction, Study_Time			);
	_declare_param(Faction, Skill_DefaultLevel	);
	_declare_param(Faction, Contribution_Max	);
	_declare_param(Faction, Task_Count			);
	_declare_param(Faction, Task_Multi_Award1	);
	_declare_param(Faction, Task_Multi_Award2	);
	_declare_param(Faction, ItemMax				);
	_declare_param(Faction, HomeSceneID			);
	_declare_param(Faction, HomeSceneRegionID	);
	_declare_param(Faction, EventMemeber		);
	_declare_param(Faction, EventExperience		);
	_declare_param(Faction, EventDepot			);
	_declare_param(Faction, ImpeachOfflineTime	);
	_declare_param(Faction, ImpeachContinueTime	);
	_declare_param(Faction, ImpeachAgreeRate	);
	_declare_param(Faction, FormalMemberTime	);
	_declare_param(Faction, CorpsBoomLow		);
	_declare_param(Faction, CorpsBoomMiddle		);
	_declare_param(Faction, CorpsBoomHigh		);
	_declare_param(Faction, ForbitSpeakHourNum	);
	_declare_param(Faction, ForbitSpeakTime		);

	// Constant_Dartcar
	_declare_param(Dartcar, Max_Count_Normal	);
	_declare_param(Dartcar, Max_Count_Tianxia	);
	// Constant_FactionBoss
	_declare_param(FactionBoss, CopymapMinTime			);
	_declare_param(FactionBoss, DelayPrizeTime			);
	_declare_param(FactionBoss, FactionPrizeMinPercent	);
	_declare_param(FactionBoss, ScoreParam				);
	_declare_param(FactionBoss, CoolTime				);
	_declare_param(FactionBoss, FactionPrizeMinScore	);
	_declare_param(ClimbTower,  Dalily_Count);

	// Constant_Copymap
	_declare_param(Copymap, LiangyiNum					);
	_declare_param(Copymap, PvpBannerMaxScore			);
	_declare_param(Copymap, BannerKillAddScore			);
	_declare_param(Copymap, BannerKillAddContri			);
	_declare_param(Copymap, BannerAssitKillAddContri	);
	_declare_param(Copymap, BannerCollectAddContri		);
	_declare_param(Copymap, BannerAddScoreStepSecond	);
	_declare_param(Copymap, BannerAddScore				);	
	_declare_param(Copymap, BannerGodlikeKillNum		);	
	_declare_param(Copymap, BannerMegakillKillNum		);	
	_declare_param(Copymap, BannerMegakillStepTime		);	
	_declare_param(Copymap, BannerReliveAddBuff			);	
	_declare_param(Copymap, BannerHookMaxTime			);	
	_declare_param(Copymap, BannerDoorID1				);
	_declare_param(Copymap, BannerDoorID2				);
	_declare_param(Copymap, CashCount					);
	_declare_param(Copymap, BannerKillAssitTime			);

	//复活
	_declare_param(Relive, Max_Free_Count				);
	_declare_param(Relive, Item_ID						);
	_declare_param(Relive, SafePoint_Time				);
	_declare_param(Relive, Buff_ID						);

	//世界boss
	_declare_param(WorldBoss, Broadcast_CD				);
	_declare_param(WorldBoss, KeepDead_CD				);
	_declare_param(WorldBoss, Wudi_Buff_ID				);

	//pvp
	_declare_param(Pvp, SinglePvp_MinScore				);
	_declare_param(Pvp, SinglePvp_DefaultScore			);
	_declare_param(Pvp, SinglePvp_RecordNum				);
	_declare_param(Pvp, SinglePvp_KValue				);
	_declare_param(Pvp, SinglePvp_WeekCount				);

	//领地争夺战常量
	_declare_param(Manor, VoteTimeSecond				);
	_declare_param(Manor, ApplyCDSecond					);
	_declare_param(Manor, ApplyCostFactionSilver		);
	_declare_param(Manor, Lobby_Line_CD					);
	_declare_param(Manor, Lobby_Line_Max_Inside_Count	);
	_declare_param(Manor, BattleField_P_CD  			);

	_declare_param(Manor, BattleField_F_CD_1			);
	_declare_param(Manor, BattleField_F_CD_2			);


	_declare_param(Manor, BattleField_FlagScore_1		);
	_declare_param(Manor, BattleField_FlagScore_2		);

	_declare_param(Manor, BattleField_LostElite_30s		);
	_declare_param(Manor, BattleField_LostElite_2m		);

	_declare_param(Manor, BattleField_SafeZone_CD		);

	_declare_param(Manor, BattleField_Assist_Harmful_CD		);
	_declare_param(Manor, BattleField_Assist_Useful_CD		);

#undef _declare_param

public:
	virtual pc_str	getFileName		()const				{	return "constant.dat";								}
	virtual pc_str	getXmlName		()const				{	return "cs_constant.xml";							}

public:
	int32	getFriendLimit			(uint32 uType);
public:
	CConstantData();

protected:
	bool	getConstant			(int16 uType, int16 uEnum, int32 &value) const;
	void	setConstant			(int16 uType, int16 uEnum, int32 nValue);
	virtual bool	onLoad		(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CConstantData	g_clConstantData;
