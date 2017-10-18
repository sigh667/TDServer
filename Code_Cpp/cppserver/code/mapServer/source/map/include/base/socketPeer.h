#ifndef __SOCKET_PEER_H__
#define __SOCKET_PEER_H__

#include <event2/event.h>    
#include <event2/listener.h>    
#include <event2/bufferevent.h>    
#include <event2/bufferevent_struct.h>

#define SINGLE_MSG_LENGTH_MAX 1024
#define RECV_BUF_LENGTH_MAX (SINGLE_MSG_LENGTH_MAX * 3)
#define TD_INVALID_SOCKET (-1)


class CSocketPeer
{
public:
	CSocketPeer(int socket) : m_nSocket(socket) {}
	int socket() { return m_nSocket; }

	virtual bool afterRecv() = 0;//������ʵ��
								 //����recv����һ��������Ӧ�ý�Ͼ����io���Ż�����Ҫ���ֵ�����onRecv
								 //bool onRecv(const char* p, size_t s)
								 //{
								 //	if (p && s > 0)
								 //	{
								 //		if (s + m_nRecvIndex < RECV_BUF_LENGTH_MAX)
								 //		{
								 //			memcpy(m_buff + m_nRecvIndex, p, s);
								 //			m_nRecvIndex += s;
								 //			afterRecv();
								 //		}
								 //	}
								 //	return false;
								 //}
	virtual bool doSend(const unsigned char* p, size_t s) = 0;//������ʵ��
	virtual void onClose()
	{
		release();
	}
	virtual void onConnected() {};
	virtual void release()
	{
		m_nSocket = TD_INVALID_SOCKET;
		m_nRecvIndex = 0;
	}
protected:
	unsigned char m_buff[RECV_BUF_LENGTH_MAX];
	unsigned int m_nRecvIndex{ 0 };
	int m_nSocket{ TD_INVALID_SOCKET };
};

class CLEPeer :public CSocketPeer
{
	typedef CSocketPeer BASECLASS;
public:
	CLEPeer(int socket) :CSocketPeer(socket) {}
	virtual void  release()
	{
		BASECLASS::release();
		if (m_bev)
		{
			bufferevent_free(m_bev);
			m_bev = NULL;
		}
	}
	void setBE(bufferevent * bev)
	{
		m_bev = bev;
	}
	bool onRecv()
	{
		size_t len = bufferevent_read(m_bev, m_buff + m_nRecvIndex, RECV_BUF_LENGTH_MAX - m_nRecvIndex);
		m_nRecvIndex += len;
		afterRecv();
		return true;
	}
	bool afterRecv() override
	{
		//���Խ����ֻҪ���㳤�Ⱦ��������Ͷ�ݸ���Ӧ�ĳ����߳�
		//-----------------------����
		//ʵ���ϲ�Ҫ��ô�ֱ��ĵ���
		m_nRecvIndex = 0;
		//-----------------------����
		return true;
	}
	bool doSend(const unsigned char* p, size_t s) override
	{
		//��Ҫ�ü������¼������������壬���̲߳���������send
		bufferevent_write(m_bev, p, s);
		return true;
	}
	void onClose() override
	{
		BASECLASS::onClose();
		//�����Լ���������־
		//1.�ϲ�Ҫ����������޳����õ�����
		//2.ͨ��ָ������ϲ�������������ߴ��� 
	}

private:
	bufferevent * m_bev;
};

#endif // !__SOCKET_PEER_H__

