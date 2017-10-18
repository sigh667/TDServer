/*----------------- netStruct.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/8/21 13:03:11
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "network/netDefine.h"
#include "encrypt/include/md5.h"
#include "basic/stringFunctions.h"
#include "stream/streamBase.h"


/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//------------------------------------------------------
//------------------------------ ��ͷ
struct _stPacketHead
{
	uint16	uPacketSize;	//����С
	uint8	uMainProtocol;	//��Э��
	uint8	uProtocol;		//��Э��
	uint8	uEventFlag;		//�¼����

	_stPacketHead()							{	memset(this,0,sizeof(*this));	}

	inline _stPacketHead& getPacketHead()	{	return *this;					}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};
//------------------------------------------------------
//------------------------------ ǰ����Ϣ��ͷ
struct _stPacketHead_Client 	: _stPacketHead
{
	_stPacketHead_Client()			{	memset(this,0,sizeof(*this));	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};
//------------------------------------------------------
//------------------------------ �������ڲ���Ϣ��ͷ
struct _stSvrInfo
{
	uint16	uLogicWorldID;		// �߼�������ID
	uint8	uServerType;		// ����������
	uint16	uServerID;			// ������id

	_stSvrInfo()							{	memset(this,0,sizeof(*this));	}
	_stSvrInfo&		getSvrInfo	()			{	return *this;					}
	void			initSvrInfo()			{	memset(this,0,sizeof(*this));	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}

	bool operator == (const _stSvrInfo& rhs)
	{
		return (this->uLogicWorldID == rhs.uLogicWorldID)&&(this->uServerType == rhs.uServerType)&&(this->uServerID == rhs.uServerID);
	}
};
struct _stPacketHead_Svr : _stPacketHead
{
	_stSvrInfo stFromSrvInfo;
	_stSvrInfo stToSrvInfo;

	_stPacketHead_Svr()			{	memset(this,0,sizeof(*this));	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};
//------------------------------------------------------
struct _stStrIP
{
	char		szIP[IP_Length_Max];	/*IP*/ 

	_stStrIP				()					{	initStrIP();					}
	_stStrIP&	getStrIP	()					{	return *this;					}
	void		initStrIP()						{	memset(this,0,sizeof(*this));	}
	char*		getIP		()					{	return szIP;					}
	void		setStrIP	(const char*pIP)
	{
		memset(szIP,0,sizeof(szIP));
		if(pIP)
			dStrcpyMax(szIP,sizeof(szIP),pIP,dStrlen(pIP));
	}
	void	setAddr(const char*pAddr)
	{
		if (!pAddr || !*pAddr)
			return;

		hostent* _host = ::gethostbyname(pAddr); 
		if(!_host)
			return;

		const char*pIP  = inet_ntoa (*(in_addr*)(_host->h_addr));
		if(pIP)
			dStrcpy(szIP,sizeof(szIP),pIP);
	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};
//------------------------------ IP��ַ��˿�
struct _stIPAddress	: _stStrIP
{
	uint16	uPort;	/*�˿�*/ 

	_stIPAddress				()				{	initAddress();					};
	_stIPAddress&	getIPAddress()				{	return *this;					}
	void			initAddress	()				{	memset(this,0,sizeof(*this));	}
	void			setPort		(uint16 uPort)	{	this->uPort	= uPort;			};
	uint16			getPort		()				{	return this->uPort;				};

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};

//-------------------------------------------------------------
//------------------------------ ����IP
struct _stNetIP
{
	uint64	uIP;		/*�����ֽ�IP*/ 

	_stNetIP				()	{	initNetIP();					}
	_stNetIP&	getNetIP	()	{	return *this;					}
	inline void	initNetIP	()	{	memset(this,0,sizeof(*this));	}
	inline bool	setIP		(pc_str _ip)	
	{
		if (!_ip || !*_ip)
			return false;

		uIP = ::inet_addr(_ip);
		return (uIP == INADDR_NONE);
	}
	inline bool	setAddr(pc_str _addr)
	{
		if (!_addr || !*_addr)
			return false;

		hostent* _host = ::gethostbyname(_addr);
		if(!_host)
			return false;

		uIP = ((in_addr*)_host->h_addr)->s_addr;
		return (uIP == INADDR_NONE);
	}
	inline pc_str	getIP()
	{
		if (uIP == 0 || uIP == INADDR_NONE)
			return "";

		in_addr	addr;
		addr.s_addr = (ulong)uIP;
		return ::inet_ntoa(addr);
	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};
//------------------------------------------------------
//------------------------------ �����ַ��˿�
struct _stNetAddress	: _stNetIP
{
	uint16	uPort;		/*�����ֽڶ˿�*/ 

	_stNetAddress				()				{	initAddress();					}
	_stNetAddress&	getAddress	()				{	return *this;					}
	inline void		initAddress	()				{	memset(this,0,sizeof(*this));	}
	inline void		setPort		(uint16 uPort)	{	this->uPort	= htons(uPort);		}
	inline uint16	getPort		()const			{	return ntohs(uPort);			}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};
//-------------------------------------------------------------
//------------------------------ ��������
struct _stNetHost
{
	char	szHost[64];	/*����/IP*/ 
	uint16	uPort;		/*�˿�*/ 

	void	setHost(const char*_host)
	{
		if(_host)
			dStrcpyMax(szHost,sizeof(szHost),_host,dStrlen(_host));
		else
			memset(szHost,0,sizeof(szHost));
	}
	const char*	getHost()				{	return szHost;					};
	void		setPort(uint16 uPort)	{	this->uPort	= uPort;			};
	uint16		getPort()				{	return uPort;					};

	_stNetHost&	getNetHost()			{	return *this;					}
	void		initNetHost	()			{	memset(this,0,sizeof(*this));	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};
//-------------------------------------------------------------
//------------------------------ ���糬ʱ
struct _stNetTimeout
{
	uint32	uConnect;	//���ӳ�ʱ(��)<��������ʱʹ��>
	uint32	uRead;		//��ȡ��ʱ(��)<�೤ʱ��δ��ȡ������>
	uint32	uWrite;		//д�볬ʱ(��)<�೤ʱ��û�з�������>

	_stNetTimeout				()	{	initNetTimeout();				}
	inline void	initNetTimeout	()
	{
		uConnect= 10;
		uRead	= 30;
		uWrite	= 20;
	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};
/*************************************************************/
#pragma pack(pop)
