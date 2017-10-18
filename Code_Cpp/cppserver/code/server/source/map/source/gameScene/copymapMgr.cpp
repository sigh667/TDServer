/*----------------- copymapMgr.cpp
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/5/16 18:08:21
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameScene/copymapMgr.h"
#include "basic/basicTypes.h"
#include "log/log.h"
#include "protocol/GameDefine.pb.h"
/*************************************************************/
CCopymapMgr* g_pCopymapMgr = NULL;
//-------------------------------------------------------------
//------------------------------ 
CCopymapMgr::CCopymapMgr()
{
}

//-------------------------------------------------------------
//------------------------------ 
CCopymapBase* CCopymapMgr::alloc(_stCopymapModel*pModel/*, const _stCopyMapCreateParam* pCMCP*/)
{
	if (!pModel)
		return NULL;

	CCopymapBase*pCopymap = NULL;
	switch(pModel->uType)
	{
	case CopymapType_Shoulie:
	case CopymapType_Normal:
	case CopymapType_NormalNoDisplay:
		{
			pCopymap = m_clCopymapNormal.allocate();
		}
		break;
	//case CopymapType_FactionBoss:
	//	{
	//		pCopymap = m_clCopymapFactionBoss.allocate();
	//	}
	//	break;
	//case CopymapType_FactionHome:
	//	{
	//		pCopymap = m_clCopymapFactionHome.allocate();
	//	}
	//	break;
	//case CopymapType_CaptureArrest:
	//	{
	//		pCopymap = m_clCopymapArrestCapture.allocate();
	//	}
	//	break;
	//case CopymapType_TrapArrest:
	//	{
	//		pCopymap = m_clCopymapArrestTrap.allocate();
	//	}
	//	break;
	//case CopymapType_LimitTimeArrest:
	//	{
	//		pCopymap = m_clCopymapArrestNormal.allocate();
	//	}
	//	break;
	//case CopymapType_DialogArrest:
	//	{
	//		pCopymap = m_clCopymapArrestDialog.allocate();
	//	}
	//	break;
	//case CopymapType_GuardArrest:
	//	{
	//		pCopymap = m_clCopymapArrestGuard.allocate();
	//	}
	//	break;
	//case CopymapType_DartRob:
	//	{
	//		pCopymap = m_clCopymapDartRob.allocate();
	//	}
	//	break;
	//case CopymapType_WenquTreasure:
	//	{
	//		pCopymap = m_clCopymapTreasureWenqu.allocate();
	//	}
	//	break;
	//case CopymapType_LiangyiTreasure:
	//	{
	//		pCopymap = m_clCopymapTreasureLiangyi.allocate();
	//	}
	//	break;
	//case CopymapType_SixiangTreasure:
	//	{
	//		pCopymap = m_clCopymapTreasureSixiang.allocate();
	//	}
	//	break;
	//case CopymapType_ZhenshenReward:
	//	{
	//		pCopymap = m_clCopymapRewardZhenshen.allocate();
	//	}
	//	break;
	//case CopymapType_FeibiaoReward:
	//case CopymapType_PaihuoReward:
	//	{
	//		pCopymap = m_clCopymapRewardNormal.allocate();
	//	}
	//	break;
	//case CopymapType_AnshaReward:
	//	{
	//		pCopymap = m_clCopymapRewardThug.allocate();
	//	}
	//	break;
	//case CopymapType_ClimbTower:
	//	{
	//		pCopymap = m_clCopymapClimbTower.allocate();
	//	}
	//	break;
	//case CopymapType_GwTrain:
	//	{
	//		pCopymap = m_clCopymapGwTrain.allocate();
	//	}
	//	break;
	//case CopymapType_PvpBanner:
	//	{
	//		pCopymap = m_clCopymapPvpBanner.allocate();
	//	}
	//	break;
	//case CopymapType_ManorLobby:
	//	{
	//		pCopymap = m_clCopymapManorLobby.allocate();
	//	}
	//	break;
	//case CopymapType_ManorBattleField:
	//	{
	//		pCopymap = m_clCopymapManorBattleField.allocate();
	//	}
	//	break;
	//case CopymapType_SinglePvp:
	//	{
	//		pCopymap = m_clCopymapSinglePvp.allocate();
	//	}
	//	break;
	default:
		break;
	}

	return pCopymap;
}

//-------------------------------------------------------------
//------------------------------ 
void	CCopymapMgr::free(CCopymapBase*pCopymap)
{
	if(!pCopymap)
		return;

	switch(pCopymap->getCopymapType())
	{
	case CopymapType_Normal:
	case CopymapType_NormalNoDisplay:
	case CopymapType_Shoulie:
		{
			m_clCopymapNormal.release(dynamic_cast<CCopymapNormal*>(pCopymap));
		}
		break;
	//case CopymapType_FactionBoss:
	//	{
	//		m_clCopymapFactionBoss.release(dynamic_cast<CCopymapFactionBoss*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_FactionHome:
	//	{
	//		m_clCopymapFactionHome.release(dynamic_cast<CCopymapFactionHome*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_CaptureArrest:
	//	{
	//		m_clCopymapArrestCapture.release(dynamic_cast<CCopymapArrestCapture*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_TrapArrest:
	//	{
	//		m_clCopymapArrestTrap.release(dynamic_cast<CCopymapArrestTrap*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_LimitTimeArrest:
	//	{
	//		m_clCopymapArrestNormal.release(dynamic_cast<CCopymapArrestNormal*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_DialogArrest:
	//	{
	//		m_clCopymapArrestDialog.release(dynamic_cast<CCopymapArrestDialog*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_GuardArrest:
	//	{
	//		m_clCopymapArrestGuard.release(dynamic_cast<CCopymapArrestGuard*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_DartRob:
	//	{
	//		m_clCopymapDartRob.release(dynamic_cast<CCopymapDartRob*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_WenquTreasure:
	//	{
	//		m_clCopymapTreasureWenqu.release(dynamic_cast<CCopymapTreasureWenqu*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_LiangyiTreasure:
	//	{
	//		m_clCopymapTreasureLiangyi.release(dynamic_cast<CCopymapTreasureLiangyi*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_SixiangTreasure:
	//	{
	//		m_clCopymapTreasureSixiang.release(dynamic_cast<CCopymapTreasureSixiang*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_ZhenshenReward:
	//	{
	//		m_clCopymapRewardZhenshen.release(dynamic_cast<CCopymapRewardZhenshen*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_FeibiaoReward:
	//case CopymapType_PaihuoReward:
	//	{
	//		m_clCopymapRewardNormal.release(dynamic_cast<CCopymapRewardNormal*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_AnshaReward:
	//	{
	//		m_clCopymapRewardThug.release(dynamic_cast<CCopymapRewardThug*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_ClimbTower:
	//	{
	//		m_clCopymapClimbTower.release(dynamic_cast<CCopymapClimbTower*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_GwTrain:
	//	{
	//		m_clCopymapGwTrain.release(dynamic_cast<CCopymapGreatwallTrain*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_PvpBanner:
	//	{
	//		m_clCopymapPvpBanner.release(dynamic_cast<CCopymapPvpBanner*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_ManorLobby:
	//	{
	//		m_clCopymapManorLobby.release(dynamic_cast<CCopymapManorLobby*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_ManorBattleField:
	//	{
	//		m_clCopymapManorBattleField.release(dynamic_cast<CCopymapManorBattleField*>(pCopymap));
	//	}
	//	break;
	//case CopymapType_SinglePvp:
	//	{
	//		m_clCopymapSinglePvp.release(dynamic_cast<CCopymapSinglePvp*>(pCopymap));
	//	}
	//	break;
	default:
		break;
	}
}