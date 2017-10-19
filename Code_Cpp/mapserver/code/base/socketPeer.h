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

	virtual bool afterRecv() = 0;//依赖于实现
								 //这种recv多了一个拷贝，应该结合具体的io做优化，不要这种单独的onRecv
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
	virtual bool doSend(const unsigned char* p, size_t s) = 0;//依赖于实现
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
		//尝试解包，只要满足长度就组包，并投递给相应的场景线程
		//-----------------------测试
		//实际上不要这么粗暴的调用
		m_nRecvIndex = 0;
		//-----------------------测试
		return true;
	}
	bool doSend(const unsigned char* p, size_t s) override
	{
		//需要用枷锁的事件队列来做缓冲，主线程才真正调用send
		bufferevent_write(m_bev, p, s);
		return true;
	}
	void onClose() override
	{
		BASECLASS::onClose();
		//设置自己是垃圾标志
		//1.上层要负责遍历，剔除不好的链接
		//2.通过指针调用上层管理器，做掉线处理 
	}

private:
	bufferevent * m_bev;
};

#endif // !__SOCKET_PEER_H__

