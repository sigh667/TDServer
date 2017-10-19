/*----------------- netPacket.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/21 13:01:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stream/netStream.h"
#include "network/include/network.h"
#include "encrypt/include/encrypts.h"
#include "msg_queue/message_queue.h"
#include "network/netStruct.h"
#include "network/MsgOperate.pb.h"
#include "netPacketFlow.h"

/*************************************************************/
class CNetPacket	: public CNetStream
{
protected:
	typedef CNetStream Parent;

protected:
	typedef bool (CNetPacket::*_HandleMsg)		(_stPacketHead &pHead,CNetStream &clStream);
	typedef bool (CNetPacket::*_HandleSubMsg)	(uint8 uEventID,CNetStream& clStream);

protected:
	enum
	{
		Check_PacketCount_Time  = 20,  //检测包数量时间间隔
		Print_PacketSize_Time	= 120,
	};
protected:
	enum _enConnectFlag
	{
		_CF_Verify	,	/*密码钥验证通过*/ 
		_CF_Login	,	/*已处理登录*/ 
		_CF_Disconn	,	/*断开标志*/ 

		_CF_Max
	};
public:
	enum _emConnectCheck
	{
		_CC_Normal	,
		_CC_Open_QQ,

		_CC_Max
	};
public:
	enum _emTimeoutEvent
	{
		_TE_Normal	,
		_TE_Ping	,
		_TE_Close	,

		_TE_Max
	};
public:
	bool		m_bPacketFlowSwitch;				// 是否开启数据流量统计
	uint64		m_PacketFlowTime;					// 据流量统计时间

private:
	uint32		m_apply_key;						//引用key,确保独立引用
	CMutex_		m_apply_mutex;						//确保线程安全

protected:
	uint8		m_uConnectFlag;						/*链接标志*/ 
	uint64		m_uPacketTime;						/*收包时间(用来计算每10秒收到多少包做均值)*/ 
	uint32		m_uBadPacketCount;					/*错误包数量*/ 
	uint32		m_uPacketCountMax;					/*每10秒收到包的最大量*/ 
	uint32		m_uPacketCount;						/*每10秒收到包的数量*/ 
	ulong		m_uTeleNetAddress;					/*自己的远程地址*/ 
	char		m_szChallenge[CMD5::MD5_LEN];		/*通信密钥,用于验证用户的合法性*/ 
	char		m_szBuffer[_IO_BUFFER_];

protected:
	uint8		m_uRecvType;						/*接收者类型*/ 
	uint8		m_uEncodeType;						/*加密类型*/ 
	uint8		m_uDecodeType;						/*解密类类型*/ 
	uint8		m_uCompress;						/*最小压缩等级*/ 
	Encrypt		m_clEncode;							/*加密*/
	Encrypt		m_clDecode;							/*解密*/ 

protected:
	uint8		m_uDisconnFlagEvent;	//_emTimeoutEvent(断开标记收到包处理事件)
	uint8		m_uTimeoutReadEvent;	//_emTimeoutEvent(读超时事件)
	uint8		m_uTimeoutWriteEvent;	//_emTimeoutEvent(写超时事件)

protected:
	bool		m_bConnectCheck;
	uint8		m_uConnectCheck;

private:
	_HandleMsg	m_SoHandleMsg[_e_P_OperateFlag_ARRAYSIZE];

public:
	/*----->{ 设置每秒最大包数量 }*/ 
	inline void		setPacketMaxNum	(uint32 uNum = 5);
	inline ulong	getTeleNetAddress();
	inline void		setConnectCheck	(uint8 uTpye);

public:
	inline uint32	get_apply_key	()const;
	inline CMutex_*	get_apply_mutex	();
	inline void		set_apply_lock	(bool _lock);

protected:
	/*--->[ 构建带结构类型包头 ]*/
	inline char*buildPacketHeads	(uint8 uMainProtocol,uint8 uProtocol,uint8 uEventFlag = 0);
	/*--->[ 设置包事件 ]*/
	inline void	setPacketEvent		(uint8 uEvent);
	inline char*build_Login_Ask		(uint8 uEventFlag = 0);
	inline char*build_Login_Ack		(uint8 uEventFlag = 0);

public:
	explicit CNetPacket();
	virtual ~CNetPacket();

public:
	/*----->{ 初始化 }*/ 
	virtual void		initialize			();

//------------------------------ 链接操作
protected:
	/*----->{ 发送数据 }*/ 
	virtual bool		send				(bool bCheck = true);
	/*----->{ 断开 }*/ 
	virtual void		disconnect			()	= 0;

	//------------------------------ 链接获取
public:
	/*----->{ 获得链接IP }*/ 
	virtual pc_str		remote_ip			();
	virtual ulong		remote_address		();

	//------------------------------ 链接断开回调
public:
	/*----->{ 链接回调(链接验证后) }*/ 
	virtual void		onConnect			(bool bConnect);
	/*----->{ 断开回调 }*/ 
	virtual void		onDisconnect		();

	//------------------------------ 收包处理
protected:
	/*----->{ 数据包处理 }*/ 
	virtual bool		onPacket			(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ 踢出处理 }*/ 
	virtual void		onKickOut			();
	/*----->{ 错误包处理 }*/ 
	virtual void		onBadPacket			(uint32 uCount,_stPacketHead* pHead);
	virtual void		onBadPacket			(_stPacketHead* pHead);
	/*----->{ 版本失败处理 }*/ 
	virtual void		onVersion			();
	//--- 设置压缩
	virtual void		onSetCompress		();

protected:	
	//--- 构建包大小
	void	packetSize						();
	/*----->{ 加密 }*/ 
	bool	encodePacket					();
	/*----->{ 解密 }*/ 
	bool	decodePacket					(_stPacketHead* pHead,char*& pData,uint16 uSize);

public:
	/*----->{ 是否连接登录成功 }*/ 
	virtual bool		isLogin				();
	/*----->{ 链接初始化 }*/ 
	virtual void		connectInit			();
	/*----->{ 连接回调 }*/ 
	virtual void		onConnected			(bool bConnect);
	/*----->{ 超时回调 }*/ 
	virtual void		onTimeoutRead		();
	virtual void		onTimeoutWrite		();
	/*----->{ 处理包 }*/ 
	virtual void		handlePacket		(char* pBuffer,uint16 uSize);
	/*----->{ 数据处理包 }*/ 
	virtual void		onHandlePacket		(_stPacketHead* pHead,char* pBuffer,uint16 uSize);
	//--- 链接检测
	virtual bool		onConnectCheck		(char* pBuffer,int32&nSize);

protected:
	/*----->{ 发送验证请求 }*/ 
	virtual void		send_Verify_Ask		();
	virtual bool		send_Ping			();
	virtual bool		send_Pong			(uint32 uOrder);
	virtual bool		send_BadPacket		(_stPacketHead* pHead);

public:
	virtual bool		send_Disconnection();
	virtual bool		send_Kick_Notify	(uint8 uCode = 0,void* pData = 0,uint32 uSize = 0);

private:
	/*服务器处理*/ 
	/*----->{ 处理验证请求 }*/ 
	virtual bool		handle_Verify_Ask	(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ 处理Ping请求 }*/ 
	virtual bool		handle_Ping_Ask		(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ 登录请求 }*/ 
	virtual bool		handle_Login_Ask	(_stPacketHead &pHead,CNetStream &clStream);

	/*客户端处理*/ 
	/*----->{ 处理验证应答 }*/ 
	virtual bool		handle_Verify_Ack	(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ 处理Ping应答 }*/ 
	virtual bool		handle_Ping_Ack		(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ 处理断开 }*/ 
	virtual bool		handle_Disconnect	(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ 错误通知 }*/ 
	virtual bool		handle_Bad_Notify	(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ 踢出通知 }*/ 
	virtual bool		handle_Kick_Notify	(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ 登录应答 }*/ 
	virtual bool		handle_Login_Ack	(_stPacketHead &pHead,CNetStream &clStream);

private:
	virtual bool		handle_TimeoutRead	(_stPacketHead &pHead,CNetStream &clStream);
	virtual bool		handle_TimeoutWrite	(_stPacketHead &pHead,CNetStream &clStream);
};
#include "netPacket_inline.h"
