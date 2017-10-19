/*----------------- netPacket.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
		Check_PacketCount_Time  = 20,  //��������ʱ����
		Print_PacketSize_Time	= 120,
	};
protected:
	enum _enConnectFlag
	{
		_CF_Verify	,	/*����Կ��֤ͨ��*/ 
		_CF_Login	,	/*�Ѵ����¼*/ 
		_CF_Disconn	,	/*�Ͽ���־*/ 

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
	bool		m_bPacketFlowSwitch;				// �Ƿ�����������ͳ��
	uint64		m_PacketFlowTime;					// ������ͳ��ʱ��

private:
	uint32		m_apply_key;						//����key,ȷ����������
	CMutex_		m_apply_mutex;						//ȷ���̰߳�ȫ

protected:
	uint8		m_uConnectFlag;						/*���ӱ�־*/ 
	uint64		m_uPacketTime;						/*�հ�ʱ��(��������ÿ10���յ����ٰ�����ֵ)*/ 
	uint32		m_uBadPacketCount;					/*���������*/ 
	uint32		m_uPacketCountMax;					/*ÿ10���յ����������*/ 
	uint32		m_uPacketCount;						/*ÿ10���յ���������*/ 
	ulong		m_uTeleNetAddress;					/*�Լ���Զ�̵�ַ*/ 
	char		m_szChallenge[CMD5::MD5_LEN];		/*ͨ����Կ,������֤�û��ĺϷ���*/ 
	char		m_szBuffer[_IO_BUFFER_];

protected:
	uint8		m_uRecvType;						/*����������*/ 
	uint8		m_uEncodeType;						/*��������*/ 
	uint8		m_uDecodeType;						/*����������*/ 
	uint8		m_uCompress;						/*��Сѹ���ȼ�*/ 
	Encrypt		m_clEncode;							/*����*/
	Encrypt		m_clDecode;							/*����*/ 

protected:
	uint8		m_uDisconnFlagEvent;	//_emTimeoutEvent(�Ͽ�����յ��������¼�)
	uint8		m_uTimeoutReadEvent;	//_emTimeoutEvent(����ʱ�¼�)
	uint8		m_uTimeoutWriteEvent;	//_emTimeoutEvent(д��ʱ�¼�)

protected:
	bool		m_bConnectCheck;
	uint8		m_uConnectCheck;

private:
	_HandleMsg	m_SoHandleMsg[_e_P_OperateFlag_ARRAYSIZE];

public:
	/*----->{ ����ÿ���������� }*/ 
	inline void		setPacketMaxNum	(uint32 uNum = 5);
	inline ulong	getTeleNetAddress();
	inline void		setConnectCheck	(uint8 uTpye);

public:
	inline uint32	get_apply_key	()const;
	inline CMutex_*	get_apply_mutex	();
	inline void		set_apply_lock	(bool _lock);

protected:
	/*--->[ �������ṹ���Ͱ�ͷ ]*/
	inline char*buildPacketHeads	(uint8 uMainProtocol,uint8 uProtocol,uint8 uEventFlag = 0);
	/*--->[ ���ð��¼� ]*/
	inline void	setPacketEvent		(uint8 uEvent);
	inline char*build_Login_Ask		(uint8 uEventFlag = 0);
	inline char*build_Login_Ack		(uint8 uEventFlag = 0);

public:
	explicit CNetPacket();
	virtual ~CNetPacket();

public:
	/*----->{ ��ʼ�� }*/ 
	virtual void		initialize			();

//------------------------------ ���Ӳ���
protected:
	/*----->{ �������� }*/ 
	virtual bool		send				(bool bCheck = true);
	/*----->{ �Ͽ� }*/ 
	virtual void		disconnect			()	= 0;

	//------------------------------ ���ӻ�ȡ
public:
	/*----->{ �������IP }*/ 
	virtual pc_str		remote_ip			();
	virtual ulong		remote_address		();

	//------------------------------ ���ӶϿ��ص�
public:
	/*----->{ ���ӻص�(������֤��) }*/ 
	virtual void		onConnect			(bool bConnect);
	/*----->{ �Ͽ��ص� }*/ 
	virtual void		onDisconnect		();

	//------------------------------ �հ�����
protected:
	/*----->{ ���ݰ����� }*/ 
	virtual bool		onPacket			(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ �߳����� }*/ 
	virtual void		onKickOut			();
	/*----->{ ��������� }*/ 
	virtual void		onBadPacket			(uint32 uCount,_stPacketHead* pHead);
	virtual void		onBadPacket			(_stPacketHead* pHead);
	/*----->{ �汾ʧ�ܴ��� }*/ 
	virtual void		onVersion			();
	//--- ����ѹ��
	virtual void		onSetCompress		();

protected:	
	//--- ��������С
	void	packetSize						();
	/*----->{ ���� }*/ 
	bool	encodePacket					();
	/*----->{ ���� }*/ 
	bool	decodePacket					(_stPacketHead* pHead,char*& pData,uint16 uSize);

public:
	/*----->{ �Ƿ����ӵ�¼�ɹ� }*/ 
	virtual bool		isLogin				();
	/*----->{ ���ӳ�ʼ�� }*/ 
	virtual void		connectInit			();
	/*----->{ ���ӻص� }*/ 
	virtual void		onConnected			(bool bConnect);
	/*----->{ ��ʱ�ص� }*/ 
	virtual void		onTimeoutRead		();
	virtual void		onTimeoutWrite		();
	/*----->{ ����� }*/ 
	virtual void		handlePacket		(char* pBuffer,uint16 uSize);
	/*----->{ ���ݴ���� }*/ 
	virtual void		onHandlePacket		(_stPacketHead* pHead,char* pBuffer,uint16 uSize);
	//--- ���Ӽ��
	virtual bool		onConnectCheck		(char* pBuffer,int32&nSize);

protected:
	/*----->{ ������֤���� }*/ 
	virtual void		send_Verify_Ask		();
	virtual bool		send_Ping			();
	virtual bool		send_Pong			(uint32 uOrder);
	virtual bool		send_BadPacket		(_stPacketHead* pHead);

public:
	virtual bool		send_Disconnection();
	virtual bool		send_Kick_Notify	(uint8 uCode = 0,void* pData = 0,uint32 uSize = 0);

private:
	/*����������*/ 
	/*----->{ ������֤���� }*/ 
	virtual bool		handle_Verify_Ask	(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ ����Ping���� }*/ 
	virtual bool		handle_Ping_Ask		(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ ��¼���� }*/ 
	virtual bool		handle_Login_Ask	(_stPacketHead &pHead,CNetStream &clStream);

	/*�ͻ��˴���*/ 
	/*----->{ ������֤Ӧ�� }*/ 
	virtual bool		handle_Verify_Ack	(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ ����PingӦ�� }*/ 
	virtual bool		handle_Ping_Ack		(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ ����Ͽ� }*/ 
	virtual bool		handle_Disconnect	(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ ����֪ͨ }*/ 
	virtual bool		handle_Bad_Notify	(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ �߳�֪ͨ }*/ 
	virtual bool		handle_Kick_Notify	(_stPacketHead &pHead,CNetStream &clStream);
	/*----->{ ��¼Ӧ�� }*/ 
	virtual bool		handle_Login_Ack	(_stPacketHead &pHead,CNetStream &clStream);

private:
	virtual bool		handle_TimeoutRead	(_stPacketHead &pHead,CNetStream &clStream);
	virtual bool		handle_TimeoutWrite	(_stPacketHead &pHead,CNetStream &clStream);
};
#include "netPacket_inline.h"
