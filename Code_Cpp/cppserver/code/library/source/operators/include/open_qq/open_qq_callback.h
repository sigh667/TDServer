/*----------------- open_qq_callback.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/8/31 18:19:11
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "encrypt/include/urlEncode.h"
#include "basic/stringFunctions.h"
#include "stl/std_map.h"
#include "stream/streamBase.h"
#include "open_qq_define.h"
/*************************************************************/
namespace open_qq
{
	//-------------------------------------------------------------
	//------------------------------ 
	struct _stCallbackBase
	{
		uint16		_zoneid;				//����ID
		uint8		_providetype;			//��������(0):0��ʾ���߹���1��ʾÿ�����Ӫ����еĵ������ͣ�2��ʾ����Ӫ���������еĽ�������
		uint64		_ts;					//ʱ��(15���Ӳ�)

		std::string _openid;				//�ʺ�
		std::string _payitem;				//����ID*����*����[;....]
		std::string _token;					//ԭ����
		std::string _billno;				//֧����ˮ��

		_stCallbackBase&getCallbackBase()	{	return *this;	}
		inline void	initCallbackBase()
		{
			_zoneid			= 0;
			_providetype	= 0;
			_ts				= 0;

			_openid.clear();
			_payitem.clear();
			_token.clear();
			_billno.clear();
		}
		inline bool	is_base(uint8 _type)
		{
			if(_openid.empty() || _payitem.empty() || (_providetype != 2 && _token.empty()) || _billno.empty() || !_zoneid || _providetype != _type)
				return false;

			return true;
		}
		inline bool	read_base(CStreamBase&clStream)
		{
			if(!clStream.read(_zoneid) || !clStream.read(_providetype) || !clStream.read(_ts))
				return false;

			if(!clStream.readString(_openid) || !clStream.readString(_payitem) || !clStream.readString(_token) || !clStream.readString(_billno))
				return false;

			return true;
		}
		inline bool	write_base(CStreamBase&clStream)
		{
			if(!clStream.write(_zoneid) || !clStream.write(_providetype) || !clStream.write(_ts))
				return false;

			if(!clStream.writeString(_openid) || !clStream.writeString(_payitem) || !clStream.writeString(_token) || !clStream.writeString(_billno))
				return false;

			return true;
		}
	};
	//-------------------------------------------------------------
	//------------------------------ ֧���ص�
	struct _stCallbackPay	: _stCallbackBase
	{
		uint32		_amt;					//�ۿ���
		uint32		_payamt_coins;			//��ȡ����Ϸ������,��λΪQ��
		uint32		_pubacct_payamt_coins;	//��ȡ�ĵ���ȯ�ܽ���λΪQ��

		_stCallbackPay&getCallbackPay()	{	return *this;	}
		inline void	initCallbackPay()
		{
			_amt					= 0;
			_payamt_coins			= 0;
			_pubacct_payamt_coins	= 0;

			initCallbackBase();
		}
		inline bool	is_pay()
		{
			return is_base(0);
		}
		inline bool	read(CStreamBase&clStream)
		{
			if(!read_base(clStream) || !clStream.read(_amt) || !clStream.read(_payamt_coins) || !clStream.read(_pubacct_payamt_coins))
				return false;

			return true;
		}
		inline bool	write(CStreamBase&clStream)
		{
			if(!write_base(clStream) || !clStream.write(_amt) || !clStream.write(_payamt_coins) || !clStream.write(_pubacct_payamt_coins))
				return false;

			return true;
		}
	};
	//-------------------------------------------------------------
	//------------------------------ ���ͻص�
	struct _stCallbackToken	: _stCallbackBase
	{
		std::string	_discountid;				////��������/��Ʒ��Ӫ���ID

		_stCallbackToken&getCallbackToken()	{	return *this;	}
		inline void		initCallbackToken()
		{
			_discountid.clear();
			initCallbackBase();
		}
		inline bool	is_token()
		{
			return is_base(1);
		}
		inline bool	read(CStreamBase&clStream)
		{
			if(!read_base(clStream) || !clStream.readString(_discountid))
				return false;

			return true;
		}
		inline bool	write(CStreamBase&clStream)
		{
			if(!write_base(clStream) || !clStream.writeString(_discountid))
				return false;

			return true;
		}
	};
	//-------------------------------------------------------------
	//------------------------------ �ص������л�
	struct _stCallbackTask	: _stCallbackBase
	{
		enum _emCmd
		{/*v3����*/
			_Cmd_Check		,//���
			_Cmd_CheckAward	,//��ⷢ��
			_Cmd_Award		,//����

			_Cmd_Max	,
		};
		uint32		_awardid;					//����Ӫ�����������������õ���ƷID
		uint32		_taskid;					//����Ӫ���������������õ�����ID
		std::string	_contractid;				//����Ӫ�����������򷽾��ĳɹ���ϵͳ����ľ���ID

		_stCallbackTask&getCallbackTask()	{	return *this;	}
		inline void		initCallbackTask()
		{
			_awardid	= 0;
			_taskid		= 0;
			_contractid.clear();
			initCallbackBase();
		}
		inline bool	is_task()
		{
			return is_base(2);
		}
		inline bool	read(CStreamBase&clStream)
		{
			if(!read_base(clStream) || !clStream.read(_awardid) || !clStream.read(_taskid) || !clStream.readString(_contractid))
				return false;

			return true;
		}
		inline bool	write(CStreamBase&clStream)
		{
			if(!write_base(clStream) || !clStream.write(_awardid) || !clStream.write(_taskid) || !clStream.writeString(_contractid))
				return false;

			return true;
		}
	};
	//#############################################################
	//##############################
	//#############################################################
	class CCallback
	{
	protected:
		typedef bool (CCallback::*_handle_callback)(url_parser&_param);
		typedef stl_map<std::string,_handle_callback>	MAP_CALLBACK;

	protected:
		uint8			m_result_code;
		std::string		m_result_content;
		std::string		m_protocol;
		MAP_CALLBACK	m_mapCallback;

	public:
		enum _emCallbackResult
		{
			_CallbackResult_Success	,//�ɹ�
			_CallbackResult_Busy	,//ϵͳ��æ
			_CallbackResult_Timeout	,//token�ѹ���
			_CallbackResult_NotExist,//token������
			_CallbackResult_Error	,//�����������
			_CallbackResult_NotDone	,//δ���
			_CallbackResult_Ship	,//����ʧ��

			_CallbackResult_Max
		};
		static pc_str	m_result_string[_CallbackResult_Max];
		static pc_str	m_result_str[_CallbackResult_Max];

	public:
		CCallback();
		virtual ~CCallback(){}

	public:
		inline void			set_result			(uint8 _code)				{	m_result_code = ((_code < _CallbackResult_Max) ? _code : _CallbackResult_Error);		}
		inline uint8		get_result			()const						{	return ((m_result_code < _CallbackResult_Max) ? m_result_code : _CallbackResult_Error);	}
		inline pc_str		get_result_string	()const						{	return m_result_string[get_result()];													}
		inline std::string&	get_protocol		()							{	return m_protocol;																		}
		inline void			set_result_content	(const std::string& _con)	{	m_result_content = _con;																}

	public:
		static pc_str		get_result_str		(uint8 _code)		{	return m_result_str[(_code < _CallbackResult_Max) ? _code : _CallbackResult_Error];		}

	public:
		virtual void	init_callback			();
		virtual void	get_response_content	(std::string&_result);

	public:
		virtual bool	callback				(pc_str pBuffer,uint16 uSize);

	public:
		//--- �����ص�
		virtual bool	callback_pay			(url_parser&_param);
		//--- �ص����͵��߷���
		virtual bool	callback_token			(url_parser&_param);
		//--- �ص������з���
		virtual bool	callback_task			(url_parser&_param);
		//--- �ص������м��
		virtual bool	callback_task_check		(url_parser&_param);
		//--- ������v3
		virtual	bool	callback_task_v3		(url_parser&_param);

	public:
		//--- ���ۻص�
		virtual bool	callback_exchange		(url_parser&_param);
	};
};
