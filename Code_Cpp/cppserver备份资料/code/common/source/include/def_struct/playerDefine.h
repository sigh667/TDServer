/*------------- globalDefine.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/24 18:48:42
*
*/ 
/***************************************************************
* 用户信息定义
***************************************************************/
#pragma once
#include "basic/basicFunctions.h"
//------------------------------------------------------
//------------------------------ 
#define Account_Name_Max				60		/*用户名长度*/ 
#define Player_Name_Max					40		/*用户名长度*/ 
#define Faction_Name_Max				40		/*帮派名长度*/ 
#define Player_Config_Max				512 	//客户端配置
#define Player_OnlyEvent_Max			2048	//唯一事件数据
#define Playert_Ex_Buffer				4042	/*扩展数据预留*/ 
#define Player_Stream_Max				7680	/*流数据长度*/ 
#define Player_SaveTick					5 * 60	//角色保存间隔时间
#define Player_GhostTick				100		//毫秒
#define Illegal_Characters				(L"`~!@#$%^&*()-_+={}[]|\\:;\"'<,>.?/！￥……（）——【】、：；“”’‘《，》。？ \a\b\f\n\r\t\v ")	// 非法字符简单过滤
const uint32 MAX_ENERGE_NUM = 100;
//-------------------------------------------------------------
//------------------------------账号状态
enum _emAccountState
{
	_AS_BeginLogin,			//开始登录
	_AS_LoginVetify,		//账号服验证验证
	_AS_LoginVetifyAck,		//账号服验证验证返回
	_AS_DBLoadAsk,			//DB加载请求
	_AS_DBLoadAck,			//DB加载返回
	_AS_CreateAsk,			//客户端创建请求
	_AS_RegisterCreateAsk,	//注册服创建请求
	_AS_RegisterCreateAck,	//注册服创建返回
	_AS_CreateAck,			//客户端创建返回
	_AS_DBCreateAsk,		//DB创建请求
	_AS_DBCreateAck,		//DB创建返回
	_AS_SelectAsk,			//客户端选角请求
	_AS_SelectAck,			//客户端选角返回
	_AS_DBSelectAsk,		//DB选角请求
	_AS_DBSelectAck,		//DB选角返回
	_AS_DBLoadPlayerAck,	//DB加载角色信息返回
	_AS_GamePlayerAsk,		//世界服发送游戏服角色信息
	_AS_W2GKickOut,			//世界服通知游戏服玩家踢人
	_AS_G2WPlayerKick,		//游戏通知世界服玩家退出成功
	_AS_Logout,				//玩家真正退出

	_AS_Max,
};
extern const char* g_szAccountState[_AS_Max];
//-------------------------------------------------------------
//------------------------------游戏登录状态
enum _emGameLoginState
{
	_GLS_WorldLoginAsk,		//世界服登录
	_GLS_Load,				//加载世界服角色信息
	_GLS_LoginAsk,			//客户端登录
	_GLS_LoginCancle,		//超时登录取消
	_GLS_PlayerCreate,		//加载完数据创建玩家
	_GLS_PlayerSend,		//开始发送玩家数据
	_GLS_PlayerSendOver,	//发送玩家数据结束
	_GLS_PlayerKickOut,		//游戏服踢人

	_GLS_Max,
};
extern const char* g_szGameLoginState[_GLS_Max];

//-------------------------------------------------------------
//------------------------------ 角色性别
enum _emGenderType
{
	GenderType_Female	,//女
	GenderType_Male		,//男

	GenderType_Max
};

//------------------------------------------------------
//------------------------------ 角色标示
enum _em_PlayerMark
{
	PlayerMark_Disable		,//是否封号
	PlayerMark_FixPoint		,//进入场景中的固定位置
	PlayerMark_Max
};
//------------------------------------------------------
//------------------------------ 角色开关
enum _em_PlayerSwitch
{
	PS_Relive					,//允许复活
	PS_Faction_ByInviteJoin		,//是否允许他人邀请我入帮派协议
	PS_Transmit					,//允许被传送

	//队伍相关
	PS_Team_Invite				,//接受邀请
	PS_Team_InviteAuto			,//自动接受邀请加入队伍
	PS_Team_ApplyAuto			,//自动接受入队申请

	//交易
	PS_Trade_RefuseAuto			,//自动拒绝交易
	//帮会
	PS_Faction_AllowAuto		,//自动同意帮会邀请
	//需要改名
	PS_NeedRename				,//需要改名

	PS_Marriage_RefusePropose	,//禁止被求婚

	PS_Max
};

//-------------------------------------------------------------
//------------------------------ 道具容器类型
enum _emItemContainerType
{
	ItemContainer_Equip		,/*装备*/ 
	ItemContainer_Bag		,/*包裹*/ 
	ItemContainer_Depot		,/*仓库*/ 

	ItemContainer_Max
};

//-------------------------------------------------------------
//------------------------------ 道具容器类型
enum _emSceneUnitMark
{
	SceneUnitMark_OutSideDamage		= 1,//外环伤害 技能踏雪追风
	SceneUnitMark_SkillMiss			= 2,//技能丢失标记
	SceneUnitMark_IgnoreSkillCD		= 4,//临时无视CD 除普攻，翻滚
	SceneUnitMark_AddExtraDamage	= 5,//增加额外伤害			value:技能ID
	SceneUnitMark_IgnoreSkillExpend	= 6,//临时无视消耗
	SceneUnitMark_TrapAddBuffOneTime= 7,//陷阱只能加一次buff	value:陷阱ID

	SceneUnitMark_Max
};

//-------------------------------------------------------------
//------------------------------ 唯一索引清理类型
enum _em_CleanupOnlyIndex
{
	CleanupOnlyIndex_Daily	,//日清理
	CleanupOnlyIndex_Weekly	,//周清理
	CleanupOnlyIndex_Monthly,//月清理

	CleanupOnlyIndex_Max
};

//-------------------------------------------------------------
//------------------------------ 道具列表类型
enum _em_ItemListType
{
	ItemListType_Bag	,//背包道具类型
	ItemListType_Equip	,//装备道具类型
	ItemListType_Sell	,//出售道具类型

	ItemListType_Max
};
//-------------------------------------------------------------
//------------------------------ 副本状态
enum _em_PlayerCopymapState
{
	PlayerCopymapState_None			,//无
	PlayerCopymapState_WaitEnter	,//等待进入

	PlayerCopymapState_Max
};
//-------------------------------------------------------------
//------------------------------ 副本进入状态
enum _em_CopymapPlayerState
{
	CopymapPlayerState_Online		,//在线
	CopymapPlayerState_Offline		,//离线
	CopymapPlayerState_Exit			,//退出
	CopymapPlayerState_Max
};
//-------------------------------------------------------------
//------------------------------ 吟唱状态
enum _em_SingState
{
	SingState_Null			= 0,	//空
	SingState_Ing			= 1,	//吟唱中
	SingState_End			= 2,	//吟唱完成
	SingState_Max,
};
//-------------------------------------------------------------
//------------------------------ 绑定状态
enum _em_ItemBindType
{
	ItemBindType_All		= 0,	//所有
	ItemBindType_NoBind		= 1,	//非绑定
	ItemBindType_Bind		= 2,	//绑定
};
//-------------------------------------------------------------
//------------------------------缉拿任务状态
enum _emWorldTeamArrestQuestState
{
	WorldTeamArrestQuestState_None	= 0,		//	未开始
	WorldTeamArrestQuestState_Ing	= 1,		//	进行中
	WorldTeamArrestQuestState_End	= 2,		//	结束
	WorldTeamArrestQuestState_Max,
};

//-------------------------------------------------------------
//------------------------------ 副本完成原因
enum _emCopymapCompleteReson
{
	CopymapCompleteReson_Success		= 0,//成功
	CopymapCompleteReson_TimeOut		= 1,//超时
	CopymapCompleteReson_PlayerDead		= 2,//玩家死亡
	CopymapCompleteReson_Fail			= 3,//通用失败
	CopymapCompleteReson_AIClose		= 4,//AI关闭
	CopymapCompleteReson_Max
};