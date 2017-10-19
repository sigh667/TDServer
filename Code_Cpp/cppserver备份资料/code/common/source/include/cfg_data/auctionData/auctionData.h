/*----------------- auctionData.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/9/23 14:31:21
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "stl/std_map.h"
/*************************************************************/
class CAuctionData	: public CFileData
{
public:
	enum
	{
		Version	= 2016092301
	};

private:
	stl_map<uint64,int32>			m_mapLockTime;		// 总价==>锁定时间
public:
	uint32	getLockTime				(uint64 uTotalPrice)const;
public:
	virtual pc_str	getFileName		()const				{	return "auction.dat";							}
	virtual pc_str	getXmlName		()const				{	return "cs_auction.xml";						}
public:
	CAuctionData();
protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CAuctionData	g_clAuctionData;
