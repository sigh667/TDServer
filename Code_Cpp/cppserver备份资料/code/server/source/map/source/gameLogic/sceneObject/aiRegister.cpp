/*------------- aiRegister.cpp
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.01
* Date         : 2016/01/27
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "behaviac/behaviac.h"
#include "gameLogic/sceneObject/aiRegister.h"
#include "gameLogic/sceneObject/sceneUnitAI.h"
#include "cfg_data/loadData/loadData.h"

void	AIRegister::registerAI()
{
	behaviac::Agent::Register<CSceneUnitAI>();
	behaviac::Workspace::GetInstance()->SetFilePath(g_clLoadData.buld_ai_path("%s", "game_ai").c_str());
}

bool AIRegister::exportMeta()
{
	registerAI();
	bool bResult = behaviac::Workspace::GetInstance()->ExportMetas("./unit_meta.xml");
	behaviac::Workspace::GetInstance()->Cleanup();

	return bResult;
}
