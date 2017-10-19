#ifndef __EVENT_H__
#define __EVENT_H__

#include "base/eventBase.h"

class CEventBAHead :public IEventByteArray
{
public:
	unsigned int   m_nLength{ 4 + 2 + 2 + 4 };//���ͷ��Ҳ����ʡ�Ե�
	unsigned short m_nInnerType{ 0 };
	unsigned short m_nType{ 0 };
	unsigned int   m_nSocket{ 0 };//������Ϣ��Դsocket

public:
	CEventBAHead(int innerType, int type, int socket) :
		m_nInnerType(innerType), 
		m_nType(m_nType), m_nSocket(socket)
	{}
	CEventBAHead() {}
	size_t getHeadSize()
	{
		return 4 + 2 + 2 + 4;
	}
	size_t getByteCount() override
	{
		return m_nLength;
	}
	bool fakeReadFrom(CByteArray* pBA)
	{
		if (pBA
			&& pBA->getDataLength() >= getByteCount())
		{
			pBA->startFakeRead();
			readFrom(pBA);
			pBA->commitFakeRead(false);
			return true;
		}
		return false;

	}
	bool readFrom(CByteArray* pBA) override
	{
		if (pBA)
		{
			m_nLength = pBA->tryReadInt();
			m_nInnerType = pBA->tryReadShort();
			m_nType = pBA->tryReadShort();
			m_nSocket = pBA->tryReadInt();
			return true;
		}
		return false;
	}
	bool writeTo(CByteArray* pBA) override
	{
		if (pBA
			&& pBA->getFreeSpace() >= getByteCount())
		{
			return pBA->tryWriteInt(m_nLength)
				&& pBA->tryWriteShort(m_nInnerType)
				&& pBA->tryWriteShort(m_nType)
				&& pBA->tryWriteInt(m_nSocket);
		}
		return false;
	}
};

//�������Ϣ����
class CEventBAClient :public CEventBAHead
{
public:
	CEventBAClient(unsigned char*p, size_t s
		, int type, unsigned int sceneID
	, unsigned int socket) :
		CEventBAHead(EVENT_INNERTYPE_MSG_FROM_CLIENT, 0, socket)
		, m_pData(p), m_nSize(s), m_nTargetSceneId(sceneID)
	{
		m_nLength += m_nSize;
	}
	size_t getByteCount() override
	{
		return CEventBAHead::getByteCount() + m_nSize;
	}

	bool writeTo(CByteArray* pBA) override
	{
		if (pBA
			&& pBA->getFreeSpace() >= getByteCount())
		{
			return CEventBAHead::writeTo(pBA)
				&& pBA->tryWriteData(m_pData, m_nSize);
		}
		return false;
	}
	unsigned char*  m_pData{ NULL };  //ע�⣬���ָ���Ŀ�����ջ�ϵ�ָ�룬���������Ժ�Ҫ����ʹ�ã����٣�
	size_t			m_nSize{ 0 };
	int				m_nTargetSceneId{ 0 };
	int				m_nSourceSocket{ 0 };
};

#endif // !__EVENT__H
