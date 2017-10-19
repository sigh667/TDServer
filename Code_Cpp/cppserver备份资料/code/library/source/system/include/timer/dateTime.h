/*----------------- dateTime.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
//------------------------------ ����ʱ��[�ֶ�С��0�����]
struct _stDateTime
{//[*/��][*/��][[*/<��-��>/<w��,w��>/<w-w>][ʱ��-ʱ��]
	int16	iYearBegin;		//��[��׼��]
	int16	iYearEnd;		//��[��׼��]
	int8	iMonthBegin;	//��[1,12]
	int8	iMonthEnd;		//��[1,12]
	uint8	uWeek;			//��[0,6]λ
	int8	iBegin;			//��[1,31]��[0,6]
	int8	iEnd;
	int8	iHourBegin;		//ʱ[0,23]
	int8	iHourEnd;
	int8	iMinuteBegin;	//��[0,59]
	int8	iMinuteEnd;

	_stDateTime()					{	initDataTime();	}
	_stDateTime&	getDataTime		()	{	return *this;	}
	inline void		initDataTime	();
	//--- �Ƿ��ܷ�Χ
	inline bool		rangeWeek		()	const;
	//--- �ܼ������Ƿ�����
	inline bool		isWeek			(const int32&_week)	const;
	//--- �Ƿ�ָ����
	inline bool		weekRange		(const int32&_week)	const;
	//--- ��ÿ�ʼʱ��
	inline	uint64	getDateTimeBegin()	const;
	//--- ��ý���ʱ��
	inline	uint64	getDateTimeEnd	()	const;
	//--- ��ÿ�ʼʱ��
	inline uint64	getBeginTime	(uint64 tTime = 0)	const;
	//--- ��ÿ�ʼʱ��(��ʱ����ڻ�ñ��ο���ʱ�䣬��ʱ��������¿���ʱ��)
	inline uint64	getStartTime	(uint64 uCurrTime)	const;
	//--- ��ý���ʱ��
	inline uint64	getEndTime		(uint64 tTime)	const;
	//--- �Ƿ񵽴�ʱ��
	inline bool		toDataTime		(uint64 tTime)	const;
	//--- ���ַ�����ȡ����
	//[*/��/��-��][*/��/��-��][[*/��-��]/[w��,w��]]/[w��-w��]][ʱ��-ʱ��]
	inline bool		readFromString	(std::string&strString);
	// �����ַ���
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
//------------------------------ ��ʼ������
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
//------------------------------ �����ܷ�Χ����
inline bool	_stDateTime::rangeWeek	()	const
{
	if(this->uWeek == 0)
		return false;

	return _CHECK_BIT(this->uWeek,_BIT32(7));
}
//-------------------------------------------------------------
//------------------------------ �Ƿ�ָ���ܼ�
inline bool	_stDateTime::isWeek		(const int32&_week)	const
{
	if(this->uWeek == 0)
		return true;

	return _CHECK_BIT(this->uWeek,_BIT32(_week));
}
//-------------------------------------------------------------
//------------------------------ �Ƿ�ָ���ܷ�Χ
inline bool	_stDateTime::weekRange	(const int32&_week)	const
{
	//��ָ��
	if(!rangeWeek())
		return isWeek(_week);

	//��ʱ���
	if(this->iBegin < this->iEnd)
	{//˳��
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
//------------------------------ ��ÿ�ʼʱ��
inline	uint64	_stDateTime::getDateTimeBegin()	const
{
	if(iYearBegin < 1900 || iMonthBegin < 0 || uWeek != 0 || iBegin < 0)
		return 0;

	tm	tmTime;
	memset(&tmTime,0,sizeof(tmTime));

	//��
	tmTime.tm_year	= this->iYearBegin - 1900;
	//��
	tmTime.tm_mon	= this->iMonthBegin - 1;
	//��
	tmTime.tm_mday	= this->iBegin;

	//ʱ
	if(this->iHourBegin >= 0)
		tmTime.tm_hour = this->iHourBegin;
	//��
	if(this->iMinuteBegin >= 0)
		tmTime.tm_min = this->iMinuteBegin;

	return mktime(&tmTime);
}

//-------------------------------------------------------------
//------------------------------ ��ý���ʱ��
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

	//��
	if(this->iYearEnd > 1900)
		tmTime.tm_year	= this->iYearEnd - 1900;
	else
		tmTime.tm_year	= this->iYearBegin - 1900;
	//��
	if(this->iMonthEnd > 0)
		tmTime.tm_mon	= this->iMonthEnd - 1;
	else
		tmTime.tm_mon	= this->iMonthBegin - 1;

	//��
	if(this->iEnd > 0)
		tmTime.tm_mday	= this->iEnd;
	else
		tmTime.tm_mday	= this->iBegin;

	//ʱ
	if(this->iHourBegin >= 0)
		tmTime.tm_hour = this->iHourEnd;
	//��
	if(this->iMinuteBegin >= 0)
		tmTime.tm_min = this->iMinuteEnd;

	return mktime(&tmTime);
}

//-------------------------------------------------------------
//------------------------------ ��ÿ�ʼʱ��
inline uint64	_stDateTime::getBeginTime(uint64 tTime)	const
{
	if(!tTime)
		tTime = (uint64)getTime();

	tm		tmTime;
	getLocalTime(&tmTime,&tTime);

	//���ж�-�Ƿ�Χ
	if(!rangeWeek() && !isWeek(tmTime.tm_wday))
		return 0;

	//��ʼʱ��
	if(this->iYearBegin > 1900)
		tmTime.tm_year = this->iYearBegin - 1900;
	//��
	if(this->iMonthBegin > 0)
		tmTime.tm_mon = this->iMonthBegin - 1;

	if(!uWeek)
	{//����
		//��
		if(this->iBegin > 0)
			tmTime.tm_mday = this->iBegin;
	}
	else
	{
		//�ܷ�Χ���
		if(rangeWeek())
		{
			tmTime.tm_mday += this->iBegin - tmTime.tm_wday;
			if(this->iBegin > this->iEnd)
			{//����
				if(tmTime.tm_wday == this->iEnd || !weekRange(tmTime.tm_wday))
					tmTime.tm_mday -= 7;
			}
		}
	}
	//ʱ
	if(this->iHourBegin >= 0)
		tmTime.tm_hour = this->iHourBegin;
	else
		tmTime.tm_hour = 0;
	//��
	if(this->iMinuteBegin >= 0)
		tmTime.tm_min = this->iMinuteBegin;
	else
		tmTime.tm_min = 0;

	tmTime.tm_sec = 0;
	//��ʼʱ���Ƿ񵽴�
	return mktime(&tmTime);
}

//-------------------------------------------------------------
//------------------------------ ��ÿ�ʼʱ��
//- (��ʱ����ڻ�ñ��ο���ʱ�䣬��ʱ��������¿���ʱ��)
inline uint64	_stDateTime::getStartTime	(uint64 uCurrTime)	const
{
	tm		tmTime;
	getLocalTime(&tmTime,&uCurrTime);
	//���ж�
	if(this->uWeek != 0)
	{
		bool bTimeing = toDataTime(uCurrTime);
		for (uint8 i = 0;i < 7;++i)
		{
			if (isWeek(tmTime.tm_wday))
				break;

			if(bTimeing)
			{//��ǰʱ�������ǰ��ѯ(Forward)
				if(tmTime.tm_mday > 0)
					--tmTime.tm_mday;
			}
			else
			{//�ǵ�ǰʱ���������ѯ(Back)
				++tmTime.tm_mday;
			}

			uint64 uTemp = mktime(&tmTime);
			getLocalTime(&tmTime,&uTemp);
		}
	}

	//��ʼʱ��
	if(this->iYearBegin > 1900)
		tmTime.tm_year = this->iYearBegin - 1900;
	//��
	if(this->iMonthBegin > 0)
		tmTime.tm_mon = this->iMonthBegin - 1;

	if(!uWeek)
	{//����
		//��
		if(this->iBegin > 0)
			tmTime.tm_mday = this->iBegin;
	}

	//ʱ
	if(this->iHourBegin >= 0)
		tmTime.tm_hour = this->iHourBegin;
	else
		tmTime.tm_hour = 0;
	//��
	if(this->iMinuteBegin >= 0)
		tmTime.tm_min = this->iMinuteBegin;
	else
		tmTime.tm_min = 0;

	tmTime.tm_sec = 0;
	//��ʼʱ���Ƿ񵽴�
	return mktime(&tmTime);
}

//-------------------------------------------------------------
//------------------------------ ��ÿ�ʼʱ��
inline uint64	_stDateTime::getEndTime(uint64 tTime)	const
{
	if(!tTime)
		tTime = (uint64)getTime();

	tm		tmTime;
	getLocalTime(&tmTime,&tTime);

	//���ж�-�Ƿ�Χ
	if(!rangeWeek() && !isWeek(tmTime.tm_wday))
		return (tTime - 1);//<��������>

	//����ʱ��
	//��
	if(this->iYearEnd > 1900)
		tmTime.tm_year = this->iYearEnd - 1900;
	else if(this->iYearBegin > 1900)
		tmTime.tm_year = this->iYearBegin - 1900;

	//��
	if(this->iMonthEnd > 0)
		tmTime.tm_mon = this->iMonthEnd - 1;
	else if(this->iMonthBegin > 0)
		tmTime.tm_mon = this->iMonthBegin - 1;

	if(!uWeek)
	{//����
		//��
		if(this->iEnd > 0)
			tmTime.tm_mday = this->iEnd;
		else if(this->iBegin > 0)
			tmTime.tm_mday = this->iBegin;
	}
	else
	{
		//�ܷ�Χ���
		if(rangeWeek())
		{
			tmTime.tm_mday += this->iEnd - tmTime.tm_wday;
			if(this->iBegin > this->iEnd)
			{//����
				if(this->iEnd < tmTime.tm_wday && weekRange(tmTime.tm_wday))
					tmTime.tm_mday += 7;
			}
		}
	}

	//ʱ
	if(iHourEnd >= 0)
		tmTime.tm_hour = iHourEnd;
	else
		tmTime.tm_hour = 0;
	//��
	if(iMinuteEnd >= 0)
		tmTime.tm_min = iMinuteEnd;
	else
		tmTime.tm_min = 0;

	tmTime.tm_sec = 0;
	//��ʼʱ���Ƿ񵽴�
	return mktime(&tmTime);
}
//-------------------------------------------------------------
//------------------------------ �Ƿ񵽴�ʱ��
inline bool	_stDateTime::toDataTime	(uint64 tTime)	const
{
	tm		tmTime;
	getLocalTime(&tmTime,&tTime);

	//�ܼ��
	if(!weekRange(tmTime.tm_wday))
		return false;

	//��ʼʱ��
	uint64 _Time = getBeginTime(tTime);
	if(_Time > tTime)
		return false;

	//����ʱ��
	_Time = getEndTime(tTime);
	if(_Time <= tTime)
	{
		if(iMinuteEnd != -1)
			return false;
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ ���ַ�����ȡ����
inline bool	_stDateTime::readFromString(std::string&strString)
{//[*/��/��-��][*/��/��-��][[*/��-��]/[w��,w��]]/[w��-w��]][ʱ��-ʱ��]
	initDataTime();
	if(strString.empty())
		return false;

	if(strString[0] != '[')
		return false;

	std::string strTemp;
	//��
	{
		truncateString(strString,'[');	//�޳���ǰ[
		getSubString(strString,strTemp,']');
		if(strTemp.empty())
			return false;
		if(!haveFromString(strTemp,'*'))
		{
			this->iYearBegin = (int16)readInt64FromString(strTemp,'-');
			if(!strTemp.empty())
				this->iYearEnd = (int16)readInt64FromString(strTemp,']');
		}
	}//end��

	//��
	{
		truncateString(strString,'[');	//�޳���ǰ[
		getSubString(strString,strTemp,']');
		if(strTemp.empty())
			return false;
		if(!haveFromString(strTemp,'*'))
		{
			this->iMonthBegin = (int8)readInt64FromString(strTemp,'-');
			if(!strTemp.empty())
				this->iMonthEnd = (int8)readInt64FromString(strTemp,']');
		}
	}//end��
	//��/��
	{
		truncateString(strString,'[');	//�޳���/��ǰ[
		getSubString(strString,strTemp,']');
		if(strTemp.empty())
			return false;
		if(!haveFromString(strTemp,'*'))
		{
			if(haveFromString(strTemp,'w'))
			{//��
				bool bRange = haveFromString(strTemp,'-');
				if(!bRange)
				{//�Ƿ�Χ
					while(!strTemp.empty())
					{
						std::string strWeek;
						getSubString(strTemp,strWeek,',');
						if(strWeek.empty())
							break;

						truncateString(strWeek,'w');	//�޳���ǰw
						uint8 _week = (uint8)readInt64FromString(strWeek,',');
						if(_week < 7)
							_SET_BIT32(this->uWeek,_week,true);
					}
				}
				else
				{//��Χ
					truncateString(strTemp,'w');	//�޳���ǰw
					this->iBegin= (int8)readInt64FromString(strTemp,'-');
					truncateString(strTemp,'w');	//�޳���ǰw
					this->iEnd	= (int8)readInt64FromString(strTemp);
					//�޸�
					this->iBegin= _RANGE(this->iBegin	,0,6);
					this->iEnd	= _RANGE(this->iEnd		,0,6);

					if(this->iBegin == this->iEnd)
						_SET_BIT32(this->uWeek,this->iBegin,true);//ͬһʱ��
					else
						_SET_BIT32(this->uWeek,7,true);
				}
			}
			else
			{//��
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
	}//end��/��
	//ʱ���
	{
		truncateString(strString,'[');	//ʱ��ǰ[
		getSubString(strString,strTemp,']');
		if(strTemp.empty())
			return false;

		//ȡ����ʼʱ��
		std::string	strBeginTime;
		getSubString(strTemp,strBeginTime,'-');

		this->iHourBegin	=(int8)readInt64FromString(strBeginTime,':');
		this->iMinuteBegin	=(int8)readInt64FromString(strBeginTime);
		if(strTemp.empty())
			return false;

		this->iHourEnd		=(int8)readInt64FromString(strTemp,':');
		this->iMinuteEnd	=(int8)readInt64FromString(strTemp);
	}//endʱ���
	return true;
}

//-------------------------------------------------------------
//------------------------------ �����ַ���
inline bool	_stDateTime::toString(std::string&strDataTime)
{
	strDataTime.clear();
	if(iHourBegin < 0 || iHourEnd < 0 || iMinuteBegin < 0 || iMinuteEnd < 0)
		return true;

	char szBuffer[64] = "";

	//��
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

	//��
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

	//��/��
	if(this->uWeek == 0)
	{//��
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
	{//��
		strDataTime += "[";

		if(rangeWeek())
		{//�ܷ�Χ
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

	//ʱ
	dSprintf(szBuffer,sizeof(szBuffer),"[%.2d:%.2d-%.2d:%.2d]",this->iHourBegin,this->iMinuteBegin,this->iHourEnd,this->iMinuteEnd);
	strDataTime += szBuffer;
	return true;
}
