/*----------------- itemFactory.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/4/12 11:47:46
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameItem.h"
#include "memoryPool/mapPool.h"
#include "singleton/singleton.h"
#include "stream/numberStream.h"
/*************************************************************/
class CScenePlayer;
// ��Ʒ���ɹ���
class CItemFactory : public CMapPool<uint64,CGameItem>
{
private:
	typedef	CMapPool<uint64,CGameItem>	Parent;

private:
	/*************************************************************/
#pragma pack(push,1)
	union _U_NUMBER
	{
		struct
		{
			uint32	uSequence;	//��� ������
			uint32	uPlayerId;	//���ID
		}_STREAM;

		uint64		uItemSN;	/*������ˮ��*/ 
	};
#pragma pack(pop)
	/*************************************************************/

public:
	CItemFactory() {	}

public:
	static uint64	buildItemSN(uint32 uPlayerID,uint32&uSequence);
	static uint32	getPlayerID(uint64 itemSN)	{		return ((_U_NUMBER*)&itemSN)->_STREAM.uPlayerId;	}

public:
	CGameItem*	buildItem	(CScenePlayer* pPlayer, const _stItemModel* pModel);
	CGameItem*	buildItem	(CScenePlayer* pPlayer, _stItem& stItem, const _stItemModel*pModel,_emDoingType emDoing );
	void		releaseItem	(CGameItem* pItem);																// �ͷ���Ʒ
};