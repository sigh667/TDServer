/*----------------- mokylin.cpp
*
* Copyright (C): 2013  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2013/10/17 11:55:01
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "basic/basicFunctions.h"
#include "basic/timeFunction.h"
#include "mokylin/mokylin.h"
/*************************************************************/
namespace mokylin
{
	static uint32		m_uTimeout		= 300;
	static uint16		m_uCallPort		= 0;
	static std::string	m_strAppId		= "";
	static std::string	m_strLoginKey	= "";
	static std::string	m_strCallKey	= "";

	//-------------------------------------------------------------
	//------------------------------ 
	std::string&get_app_id		()
	{
		return m_strAppId;
	}
	//-------------------------------------------------------------
	//------------------------------ 
	std::string&get_login_key	()
	{
		return m_strLoginKey;
	}
	//-------------------------------------------------------------
	//------------------------------ 
	std::string&get_call_key	()
	{
		return m_strCallKey;
	}
	//-------------------------------------------------------------
	//------------------------------ 获得回调端口
	uint16		get_call_port	()
	{
		return m_uCallPort;
	}
	//-------------------------------------------------------------
	//------------------------------ 初始化
	bool		init			(pc_str _app_id,pc_str _login_key,uint32 _timeout,uint16 _call_port)
	{
		m_strAppId		= _app_id ? _app_id : "";
		m_strLoginKey	= _login_key ? _login_key : "";
		if(m_strLoginKey.empty())
			return false;

		if(m_strCallKey.empty())
			m_strCallKey= m_strLoginKey;

		m_uTimeout	= _timeout;
		m_uCallPort	= _call_port;

		return true;
	}
	//-------------------------------------------------------------
	//------------------------------ 设置回调key
	void		set_call_key	(pc_str _call_key)
	{
		m_strCallKey	= _call_key ? _call_key : "";
	}

	//-------------------------------------------------------------
	//------------------------------ 登录验证
	bool		login_verify	(url_parser&_param,std::string&_uname,int8&_adult,uint8&_client,std::string*_via,std::string*_adtag)
	{
		if(m_strLoginKey.empty())
			return false;

		if(m_uTimeout < 30)
			m_uTimeout = 30;

		static std::string str_appid;
		static std::string str_uname;
		static std::string str_time;
		static std::string str_adult;
		static std::string str_serverid;
		static std::string str_client;
		static std::string str_sign;
	
		str_appid	= _param.get_param_content("appid"		,"");//帐号名
		str_serverid= _param.get_param_content("serverId"	,"");//服务器ID
		str_uname	= _param.get_param_content("uname"		,"");//帐号名
		str_adult	= _param.get_param_content("adult"		,"");//是否成年
		str_client	= _param.get_param_content("client"		,"");//客户端登录状态
		str_time	= _param.get_param_content("time"		,"");//时间
		str_sign	= _param.get_param_content("sign"		,"");//Sign

		uint64	_time	= getTime();
		//时间验证
		uint64	u_time	= dAtoull(str_time.c_str());
		//时间范围
		if(!u_time || (_time - m_uTimeout) > u_time || (_time + m_uTimeout) < u_time)
			return false;

		if(str_appid != m_strAppId)
			return false;

		//用户名
		if(str_uname.empty() || str_uname.length() > 60)
			return false;

		//MD5字符串
		static std::string strMD5;
		strMD5 = str_appid + str_serverid + str_uname + str_adult + str_client + str_time + m_strLoginKey;
		//md5验证
		if(str_sign.size() < CMD5::MD5_LEN * 2 || memcmp(str_sign.c_str(),getMD5Str(strMD5.c_str(),strMD5.size()),CMD5::MD5_LEN * 2) != 0)
			return false;

		_uname	= str_uname;
		_adult	= (uint8)dAtoull(str_adult.c_str());
		_client	= (uint8)dAtoull(str_client.c_str());

		if(_via)
		{
			_via->clear();
			_via->append(_param.get_param_content("channel"	,""));
		}

		if(_adtag)
		{
			_adtag->clear();
			_adtag->append(_param.get_param_content("adtag"		,""));
		}

		return true;
	}

	//-------------------------------------------------------------
	//------------------------------ 生成gm登陆url信息
	bool		build_login_url	(pc_str _uname,uint16 _serverid,uint8 _client,uint64 _time,std::string& url)
	{
		if (!_uname || !_serverid || !_time)
			return false;

		static url_parser _param;
		_param.clear();

		static char szSign[512] = {0,};
		memset(szSign,0,sizeof(szSign));

		dSprintf(szSign,sizeof(szSign),"%s%u%s%u%u%llu%s",m_strAppId.c_str(),_serverid,_uname,1,_client,_time,m_strLoginKey.c_str());
		const char* pMD5Str = getMD5Str( szSign ,dStrlen( szSign ) );
		if ( !pMD5Str )
			return false;

		_param.param_add( "appid"		, m_strAppId.c_str(),true	);
		_param.param_add( "serverId"	, (uint64)_serverid			);
		_param.param_add( "uname"		, _uname					);
		_param.param_add( "client"		 , (uint64)_client			);
		_param.param_add( "adult"		 , (uint64)1				);
		_param.param_add( "time"		 , _time					);
		_param.param_add( "sign"		 , pMD5Str					);

		const char* pStr = _param.param_to_url_string(NULL,true);
		if ( !pStr )
			return false;

		url = pStr;
		return true;
	}
};
