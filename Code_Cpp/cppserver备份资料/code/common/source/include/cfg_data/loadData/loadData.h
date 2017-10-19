/*----------------- loadData.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2011/9/23 10:08:56
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "def_struct/configStruct.h"
/*************************************************************/
enum _emLoadFrom
{
	_emLoadFrom_BinFile,
	_emLoadFrom_Xml,

	_emLoadFrom_Max
};

class PBCfg_Res;
class CLoadData
{
public:
	enum _emDataType
	{
		_D_Trap				,
		_D_WordFilter		,
		_D_Battle			,
		_D_Birth			,
		_D_Scene			,
		_D_Copymap			,
		_D_Constant			,
		_D_Skill			,
		_D_Buff				,
		_D_Player			,
		_D_Quest			,
		_D_Monster			,
		_D_Halo				,
		_D_Job				,
		_D_Bingfu			,
		_D_Item				,
		_D_Expend			,
		_D_Prize			,
		_D_Camp				,
		_D_Store			,
		_D_Drop				,
		_D_Wulue			,
		_D_WenTao			,
		_D_Equip			,
		_D_Identity			,
		_D_GM				,
		_D_FLY				,
		_D_AUCTION			,
		_D_PROMOTION		,
		_D_SystemSwitch		,
		_D_Faction			,
		_D_Wuxue			,
		_D_IdentityQuest	,
		_D_Dart				,
		_D_AI				,
		_D_FactionBoss		,
		_D_TeamTarget		,
		_D_Title			,
		_D_Dazuoguaji		,
		_D_Fashion			,
		_D_TeamTask			,
		_D_ClimbTower		,
		_D_Pvp				,
		_D_GwTrain			,
		_D_Question			,
		_D_TreasureMap		,
		_D_WorldBoss		,
		_D_Activity			,
		_D_Relive			,
		_D_TopList			,
		_D_Corps			,
		_D_Weal				,
		_D_Manor			,
		_D_Stronger			,
		_D_Max
	};

	struct _stLoadMark
	{
		uint8 arrMark[_BIT8ArraySize(_D_Max)];

		_stLoadMark()										{	initLoadMark();											}
		_stLoadMark&	getLoadMark	()						{	return *this;											}
		inline void		initLoadMark()						{	memset(this,0,sizeof(*this));							}
		inline bool		checkMark	(uint8 _mark)			{	return _checkBIT8Array(arrMark,_mark,sizeof(arrMark));	}
		inline void		setMark		(uint8 _mark,bool _set)	{	_setBIT8Array(arrMark,_mark,_set,sizeof(arrMark));		}
	};

public:
	CFileData*		m_arrLoadFile[_D_Max];
private:
	bool			m_bReload;
	_stLoadMark		m_stReloadMark;
	std::string		m_strDataPath;
	std::string		m_strClientDataPath;
	std::string		m_strScenePath;
	std::string		m_strAIPath;
	std::string		m_strScriptPath;
	uint8			m_uLoadFrom;

public:
	CLoadData();

public:
	void			load_from_pb	(const PBCfg_Res &pb);
	void			set_data_path	(pc_str _path);
	void			set_client_data_path(pc_str _path);
	void			set_scene_path	(pc_str _path);
	void			set_ai_path		(pc_str _path);
	void			set_script_path	(pc_str _path);

	std::string&	buld_data_path	(pc_str _path,...);
	std::string&	buld_scene_path	(pc_str _path,...);
	std::string&	buld_ai_path	(pc_str _path,...);
	std::string&	buld_script_path(pc_str _path,...);

	bool			is_reload_mark	();
	bool			check_reload_mark(uint8 _mark);
	void			del_reload_mark	();

public:
	bool	reLoadFile		(_stLoadMark&stLoad);
	void	onCompletioned();
public:
	bool	reloadFromFile	(_stLoadMark&stLoad);
	bool	reloadFromXml	(_stLoadMark&stLoad);
};
//-------------------------------------------------------------
extern CLoadData	g_clLoadData;
