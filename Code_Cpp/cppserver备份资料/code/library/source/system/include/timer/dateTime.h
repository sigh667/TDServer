/*----------------- dateTime.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/12/23 12:22:20
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "basic/stringFunctions.h"
#include "basic/timeFunction.h"
#include "timer/dateTime.h"
#include "stream/streamBase.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 日期时间[字段小于0不检测]
struct _stDateTime
{//[*/年][*/月][[*/<日-日>/<w周,w周>/<w-w>][时间-时间]
	int16	iYearBegin;		//年[标准年]
	int16	iYearEnd;		//年[标准年]
	int8	iMonthBegin;	//月[1,12]
	int8	iMonthEnd;		//月[1,12]
	uint8	uWeek;			//周[0,6]位
	int8	iBegin;			//日[1,31]周[0,6]
	int8	iEnd;
	int8	iHourBegin;		//时[0,23]
	int8	iHourEnd;
	int8	iMinuteBegin;	//分[0,59]
	int8	iMinuteEnd;

	_stDateTime()					{	initDataTime();	}
	_stDateTime&	getDataTime		()	{	return *this;	}
	inline void		initDataTime	();
	//--- 是否周范围
	inline bool		rangeWeek		()	const;
	//--- 周几条件是否满足
	inline bool		isWeek			(const int32&_week)	const;
	//--- 是否指定周
	inline bool		weekRange		(const int32&_week)	const;
	//--- 获得开始时间
	inline	uint64	getDateTimeBegin()	const;
	//--- 获得结束时间
	inline	uint64	getDateTimeEnd	()	const;
	//--- 获得开始时间
	inline uint64	getBeginTime	(uint64 tTime = 0)	const;
	//--- 获得开始时间(在时间段内获得本次开启时间，在时间段外获得新开启时间)
	inline uint64	getStartTime	(uint64 uCurrTime)	const;
	//--- 获得结束时间
	inline uint64	getEndTime		(uint64 tTime)	const;
	//--- 是否到达时间
	inline bool		toDataTime		(uint64 tTime)	const;
	//--- 从字符串读取数据
	//[*/年/年-年][*/月/月-月][[*/日-日]/[w周,w周]]/[w周-w周]][时间-时间]
	inline bool		readFromString	(std::string&strString);
	// 生成字符串
	inline bool		toString		(std::string&strDataTime);


	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(iYearBegin)&&
			stream.read(iYearEnd)&&
			stream.read(iMonthBegin)&&
			stream.read(iMonthEnd)&&
			stream.read(uWeek)&&
			stream.read(iBegin)&&
			stream.read(iEnd)&&
			stream.read(iHourBegin)&&
			stream.read(iHourEnd)&&
			stream.read(iMinuteBegin)&&
			stream.read(iMinuteEnd);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(iYearBegin)&&
			stream.write(iYearEnd)&&
			stream.write(iMonthBegin)&&
			stream.write(iMonthEnd)&&
			stream.write(uWeek)&&
			stream.write(iBegin)&&
			stream.write(iEnd)&&
			stream.write(iHourBegin)&&
			stream.write(iHourEnd)&&
			stream.write(iMinuteBegin)&&
			stream.write(iMinuteEnd);
	}
};
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 初始化函数
inline void	_stDateTime::initDataTime()
{
	iYearBegin	=-1;
	iYearEnd	=-1;
	iMonthBegin	=-1;
	iMonthEnd	=-1;
	uWeek		= 0;
	iBegin		=-1;
	iEnd		=-1;
	iHourBegin	=-1;
	iHourEnd	=-1;
	iMinuteBegin=-1;
	iMinuteEnd	=-1;
}
//-------------------------------------------------------------
//------------------------------ 是周周范围条件
inline bool	_stDateTime::rangeWeek	()	const
{
	if(this->uWeek == 0)
		return false;

	return _CHECK_BIT(this->uWeek,_BIT32(7));
}
//-------------------------------------------------------------
//------------------------------ 是否指定周几
inline bool	_stDateTime::isWeek		(const int32&_week)	const
{
	if(this->uWeek == 0)
		return true;

	return _CHECK_BIT(this->uWeek,_BIT32(_week));
}
//-------------------------------------------------------------
//------------------------------ 是否指定周范围
inline bool	_stDateTime::weekRange	(const int32&_week)	const
{
	//周指定
	if(!rangeWeek())
		return isWeek(_week);

	//周时间段
	if(this->iBegin < this->iEnd)
	{//顺序
		if(int32(this->iBegin) > _week || int32(this->iEnd) < _week)
			return false;
	}
	else
	{
		if(int32(this->iEnd) < _week && int32(this->iBegin) > _week)
			return false;
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 获得开始时间
inline	uint64	_stDateTime::getDateTimeBegin()	const
{
	if(iYearBegin < 1900 || iMonthBegin < 0 || uWeek != 0 || iBegin < 0)
		return 0;

	tm	tmTime;
	memset(&tmTime,0,sizeof(tmTime));

	//年
	tmTime.tm_year	= this->iYearBegin - 1900;
	//月
	tmTime.tm_mon	= this->iMonthBegin - 1;
	//日
	tmTime.tm_mday	= this->iBegin;

	//时
	if(this->iHourBegin >= 0)
		tmTime.tm_hour = this->iHourBegin;
	//分
	if(this->iMinuteBegin >= 0)
		tmTime.tm_min = this->iMinuteBegin;

	return mktime(&tmTime);
}

//-------------------------------------------------------------
//------------------------------ 获得结束时间
inline	uint64	_stDateTime::getDateTimeEnd()	const
{
	if(uWeek != 0)
		return 0;

	if(iYearEnd < 1900 && iYearBegin < 1900)
		return 0;

	if(iMonthEnd < 0 && iMonthBegin < 0)
		return 0;

	if(iEnd < 0 && iBegin < 0)
		return 0;

	tm	tmTime;
	memset(&tmTime,0,sizeof(tmTime));

	//年
	if(this->iYearEnd > 1900)
		tmTime.tm_year	= this->iYearEnd - 1900;
	else
		tmTime.tm_year	= this->iYearBegin - 1900;
	//月
	if(this->iMonthEnd > 0)
		tmTime.tm_mon	= this->iMonthEnd - 1;
	else
		tmTime.tm_mon	= this->iMonthBegin - 1;

	//日
	if(this->iEnd > 0)
		tmTime.tm_mday	= this->iEnd;
	else
		tmTime.tm_mday	= this->iBegin;

	//时
	if(this->iHourBegin >= 0)
		tmTime.tm_hour = this->iHourEnd;
	//分
	if(this->iMinuteBegin >= 0)
		tmTime.tm_min = this->iMinuteEnd;

	return mktime(&tmTime);
}

//-------------------------------------------------------------
//------------------------------ 获得开始时间
inline uint64	_stDateTime::getBeginTime(uint64 tTime)	const
{
	if(!tTime)
		tTime = (uint64)getTime();

	tm		tmTime;
	getLocalTime(&tmTime,&tTime);

	//周判断-非范围
	if(!rangeWeek() && !isWeek(tmTime.tm_wday))
		return 0;

	//开始时间
	if(this->iYearBegin > 1900)
		tmTime.tm_year = this->iYearBegin - 1900;
	//月
	if(this->iMonthBegin > 0)
		tmTime.tm_mon = this->iMonthBegin - 1;

	if(!uWeek)
	{//非周
		//日
		if(this->iBegin > 0)
			tmTime.tm_mday = this->iBegin;
	}
	else
	{
		//周范围检测
		if(rangeWeek())
		{
			tmTime.tm_mday += this->iBegin - tmTime.tm_wday;
			if(this->iBegin > this->iEnd)
			{//逆序
				if(tmTime.tm_wday == this->iEnd || !weekRange(tmTime.tm_wday))
					tmTime.tm_mday -= 7;
			}
		}
	}
	//时
	if(this->iHourBegin >= 0)
		tmTime.tm_hour = this->iHourBegin;
	else
		tmTime.tm_hour = 0;
	//分
	if(this->iMinuteBegin >= 0)
		tmTime.tm_min = this->iMinuteBegin;
	else
		tmTime.tm_min = 0;

	tmTime.tm_sec = 0;
	//开始时间是否到达
	return mktime(&tmTime);
}

//-------------------------------------------------------------
//------------------------------ 获得开始时间
//- (在时间段内获得本次开启时间，在时间段外获得新开启时间)
inline uint64	_stDateTime::getStartTime	(uint64 uCurrTime)	const
{
	tm		tmTime;
	getLocalTime(&tmTime,&uCurrTime);
	//周判断
	if(this->uWeek != 0)
	{
		bool bTimeing = toDataTime(uCurrTime);
		for (uint8 i = 0;i < 7;++i)
		{
			if (isWeek(tmTime.tm_wday))
				break;

			if(bTimeing)
			{//当前时间段内向前查询(Forward)
				if(tmTime.tm_mday > 0)
					--tmTime.tm_mday;
			}
			else
			{//非当前时间段内向后查询(Back)
				++tmTime.tm_mday;
			}

			uint64 uTemp = mktime(&tmTime);
			getLocalTime(&tmTime,&uTemp);
		}
	}

	//开始时间
	if(this->iYearBegin > 1900)
		tmTime.tm_year = this->iYearBegin - 1900;
	//月
	if(this->iMonthBegin > 0)
		tmTime.tm_mon = this->iMonthBegin - 1;

	if(!uWeek)
	{//非周
		//日
		if(this->iBegin > 0)
			tmTime.tm_mday = this->iBegin;
	}

	//时
	if(this->iHourBegin >= 0)
		tmTime.tm_hour = this->iHourBegin;
	else
		tmTime.tm_hour = 0;
	//分
	if(this->iMinuteBegin >= 0)
		tmTime.tm_min = this->iMinuteBegin;
	else
		tmTime.tm_min = 0;

	tmTime.tm_sec = 0;
	//开始时间是否到达
	return mktime(&tmTime);
}

//-------------------------------------------------------------
//------------------------------ 获得开始时间
inline uint64	_stDateTime::getEndTime(uint64 tTime)	const
{
	if(!tTime)
		tTime = (uint64)getTime();

	tm		tmTime;
	getLocalTime(&tmTime,&tTime);

	//周判断-非范围
	if(!rangeWeek() && !isWeek(tmTime.tm_wday))
		return (tTime - 1);//<立即结束>

	//结束时间
	//年
	if(this->iYearEnd > 1900)
		tmTime.tm_year = this->iYearEnd - 1900;
	else if(this->iYearBegin > 1900)
		tmTime.tm_year = this->iYearBegin - 1900;

	//月
	if(this->iMonthEnd > 0)
		tmTime.tm_mon = this->iMonthEnd - 1;
	else if(this->iMonthBegin > 0)
		tmTime.tm_mon = this->iMonthBegin - 1;

	if(!uWeek)
	{//非周
		//日
		if(this->iEnd > 0)
			tmTime.tm_mday = this->iEnd;
		else if(this->iBegin > 0)
			tmTime.tm_mday = this->iBegin;
	}
	else
	{
		//周范围检测
		if(rangeWeek())
		{
			tmTime.tm_mday += this->iEnd - tmTime.tm_wday;
			if(this->iBegin > this->iEnd)
			{//逆序
				if(this->iEnd < tmTime.tm_wday && weekRange(tmTime.tm_wday))
					tmTime.tm_mday += 7;
			}
		}
	}

	//时
	if(iHourEnd >= 0)
		tmTime.tm_hour = iHourEnd;
	else
		tmTime.tm_hour = 0;
	//分
	if(iMinuteEnd >= 0)
		tmTime.tm_min = iMinuteEnd;
	else
		tmTime.tm_min = 0;

	tmTime.tm_sec = 0;
	//开始时间是否到达
	return mktime(&tmTime);
}
//-------------------------------------------------------------
//------------------------------ 是否到达时间
inline bool	_stDateTime::toDataTime	(uint64 tTime)	const
{
	tm		tmTime;
	getLocalTime(&tmTime,&tTime);

	//周检测
	if(!weekRange(tmTime.tm_wday))
		return false;

	//开始时间
	uint64 _Time = getBeginTime(tTime);
	if(_Time > tTime)
		return false;

	//结束时间
	_Time = getEndTime(tTime);
	if(_Time <= tTime)
	{
		if(iMinuteEnd != -1)
			return false;
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 从字符串读取数据
inline bool	_stDateTime::readFromString(std::string&strString)
{//[*/年/年-年][*/月/月-月][[*/日-日]/[w周,w周]]/[w周-w周]][时间-时间]
	initDataTime();
	if(strString.empty())
		return false;

	if(strString[0] != '[')
		return false;

	std::string strTemp;
	//年
	{
		truncateString(strString,'[');	//剔除年前[
		getSubString(strString,strTemp,']');
		if(strTemp.empty())
			return false;
		if(!haveFromString(strTemp,'*'))
		{
			this->iYearBegin = (int16)readInt64FromString(strTemp,'-');
			if(!strTemp.empty())
				this->iYearEnd = (int16)readInt64FromString(strTemp,']');
		}
	}//end年

	//月
	{
		truncateString(strString,'[');	//剔除月前[
		getSubString(strString,strTemp,']');
		if(strTemp.empty())
			return false;
		if(!haveFromString(strTemp,'*'))
		{
			this->iMonthBegin = (int8)readInt64FromString(strTemp,'-');
			if(!strTemp.empty())
				this->iMonthEnd = (int8)readInt64FromString(strTemp,']');
		}
	}//end月
	//日/周
	{
		truncateString(strString,'[');	//剔除日/周前[
		getSubString(strString,strTemp,']');
		if(strTemp.empty())
			return false;
		if(!haveFromString(strTemp,'*'))
		{
			if(haveFromString(strTemp,'w'))
			{//周
				bool bRange = haveFromString(strTemp,'-');
				if(!bRange)
				{//非范围
					while(!strTemp.empty())
					{
						std::string strWeek;
						getSubString(strTemp,strWeek,',');
						if(strWeek.empty())
							break;

						truncateString(strWeek,'w');	//剔除周前w
						uint8 _week = (uint8)readInt64FromString(strWeek,',');
						if(_week < 7)
							_SET_BIT32(this->uWeek,_week,true);
					}
				}
				else
				{//范围
					truncateString(strTemp,'w');	//剔除周前w
					this->iBegin= (int8)readInt64FromString(strTemp,'-');
					truncateString(strTemp,'w');	//剔除周前w
					this->iEnd	= (int8)readInt64FromString(strTemp);
					//修复
					this->iBegin= _RANGE(this->iBegin	,0,6);
					this->iEnd	= _RANGE(this->iEnd		,0,6);

					if(this->iBegin == this->iEnd)
						_SET_BIT32(this->uWeek,this->iBegin,true);//同一时间
					else
						_SET_BIT32(this->uWeek,7,true);
				}
			}
			else
			{//日
				std::string strDay;
				getSubString(strTemp,strDay,'-');
				if(strDay.empty())
					return false;
				this->iBegin	= (int8)readInt64FromString(strDay,'-');
				getSubString(strTemp,strDay);
				if(!strDay.empty())
					this->iEnd	=(int8)readInt64FromString(strDay);
			}
		}
	}//end日/周
	//时间段
	{
		truncateString(strString,'[');	//时间前[
		getSubString(strString,strTemp,']');
		if(strTemp.empty())
			return false;

		//取出开始时间
		std::string	strBeginTime;
		getSubString(strTemp,strBeginTime,'-');

		this->iHourBegin	=(int8)readInt64FromString(strBeginTime,':');
		this->iMinuteBegin	=(int8)readInt64FromString(strBeginTime);
		if(strTemp.empty())
			return false;

		this->iHourEnd		=(int8)readInt64FromString(strTemp,':');
		this->iMinuteEnd	=(int8)readInt64FromString(strTemp);
	}//end时间段
	return true;
}

//-------------------------------------------------------------
//------------------------------ 生成字符串
inline bool	_stDateTime::toString(std::string&strDataTime)
{
	strDataTime.clear();
	if(iHourBegin < 0 || iHourEnd < 0 || iMinuteBegin < 0 || iMinuteEnd < 0)
		return true;

	char szBuffer[64] = "";

	//年
	if(iYearBegin < 1900)
	{
		strDataTime += "[*]";
	}
	else
	{
		dSprintf(szBuffer,sizeof(szBuffer),"[%.4d",this->iYearBegin);
		strDataTime += szBuffer;
		if(this->iYearEnd < 1900)
			dSprintf(szBuffer,sizeof(szBuffer),"]");
		else
			dSprintf(szBuffer,sizeof(szBuffer),"-%.4d]",this->iYearEnd);
		strDataTime += szBuffer;
	}

	//月
	if(this->iMonthBegin <= 0)
	{
		strDataTime += "[*]";
	}
	else
	{
		dSprintf(szBuffer,sizeof(szBuffer),"[%.2d",this->iMonthBegin);
		strDataTime += szBuffer;
		if(this->iMonthEnd <= 0)
			dSprintf(szBuffer,sizeof(szBuffer),"]");
		else
			dSprintf(szBuffer,sizeof(szBuffer),"-%.2d]",this->iMonthEnd);
		strDataTime += szBuffer;
	}

	//日/周
	if(this->uWeek == 0)
	{//日
		if(this->iBegin == -1 && this->iEnd == -1)
		{
			strDataTime += "[*]";
		}
		else
		{
			dSprintf(szBuffer,sizeof(szBuffer),"[%.2d-%.2d]",this->iBegin,this->iEnd);
			strDataTime += szBuffer;
		}
	}
	else
	{//周
		strDataTime += "[";

		if(rangeWeek())
		{//周范围
			dSprintf(szBuffer,sizeof(szBuffer),"[w%.2d-w%.2d]",this->iBegin,this->iEnd);
			strDataTime += szBuffer;
		}
		else
		{
			for(uint8 i = 0; i < 7; ++i)
			{
				if(!_CHECK_BIT(this->uWeek,_BIT32(i)))
					continue;

				dSprintf(szBuffer,sizeof(szBuffer),"w%1d,",i);
				strDataTime += szBuffer;
			}

			std::string::size_type tFind = strDataTime.rfind(',');
			if(tFind == strDataTime.length() - 1)
				strDataTime.erase(tFind,1);
		}

		strDataTime += ']';
	}

	//时
	dSprintf(szBuffer,sizeof(szBuffer),"[%.2d:%.2d-%.2d:%.2d]",this->iHourBegin,this->iMinuteBegin,this->iHourEnd,this->iMinuteEnd);
	strDataTime += szBuffer;
	return true;
}
