#ifndef __CPP_BYTE_ARRAY_H__
#define __CPP_BYTE_ARRAY_H__

#define BYTE_ARRAY_MAX_LENGTH (1024*256)
//用loopbuff存放消息缓存，包括写入和弹出功能
//提供spin lock，但是调用交给上层逻辑自己处理，内部不做假设，
#include <string.h>

struct U29Int
{
	int t;
};

class CPPByteArray
{
public:
	inline unsigned int getFreeSpace() const
	{
		if (m_IndexWrite >= m_IndexRead)
		{
			return BYTE_ARRAY_MAX_LENGTH - m_IndexWrite
			+ m_IndexRead 
			- 1;
		}
		else
		{
			return m_IndexRead - m_IndexWrite - 1;
		}
	}
	inline unsigned int getDataLength() const
	{
		if (m_IndexWrite >= m_IndexRead)
		{
			return m_IndexWrite - m_IndexRead;
		}
		else
		{
			return m_IndexWrite 
				+ BYTE_ARRAY_MAX_LENGTH - m_IndexRead;
		}
	}
	//尝试读写的时候用，成对调用
	inline void startFakeRead() 
	{ 
		m_bIsFakeRead = true; 
		m_IndexReadFake = m_IndexRead; 
	}
	inline void commitFakeRead(bool isSuccess) 
	{ 
		m_bIsFakeRead = false;
		if (isSuccess)
		{
			m_IndexRead = m_IndexReadFake;
		}
	}
	inline void startFakeWrite() 
	{
		m_bIsFakeWrite = true;

		m_IndexWriteFake = m_IndexWrite; 
	}
	inline void commitFakeWrite(bool isSuccess)
	{ 
		m_bIsFakeWrite = false;
		if (isSuccess)
		{
			m_IndexWrite = m_IndexWriteFake; 
		}
	}

#define INDEX_MOVE_N(idx, n) {idx = (idx+n)%BYTE_ARRAY_MAX_LENGTH; }
	inline int getPart1Read()
	{
		if (m_IndexWrite >= m_IndexRead)
		{
			return m_IndexWrite - m_IndexRead;
		}
		else
		{
			return BYTE_ARRAY_MAX_LENGTH - m_IndexRead;
		}
	}
	inline int getPart1Write()
	{
		if (m_IndexWrite >= m_IndexRead)
		{
			return BYTE_ARRAY_MAX_LENGTH - m_IndexWrite;
		}
		else
		{
			return m_IndexRead - m_IndexWrite;
		}

	}
	//这里是大数据拷贝。比如直接从网络消息里拷贝，不是给coding代码使用
	bool tryWriteData(unsigned char* p, size_t s)
	{
		bool r = false;
		if (p && s>0 && s <= getFreeSpace())
		{
			size_t part1 = getPart1Write();
			part1 = part1 >= s ? s : part1;
			memcpy(m_buff + m_IndexWrite, p, part1);
			INDEX_MOVE_N(m_IndexWrite, part1);
			size_t part2 = s - part1;
			if (part2 > 0)
			{
				memcpy(m_buff + m_IndexWrite, p, part2);
				INDEX_MOVE_N(m_IndexWrite, part2);
			}
			r = true;
		}
		else
		{
			r = false;
		}
		return r;
	}
	void tryMoveReadIndex(size_t s)
	{
		if (getDataLength() >= s)
		{
			INDEX_MOVE_N(m_IndexRead, s);
		}
	}
	size_t tryReadData(unsigned char* pT, size_t s)
	{
		size_t r = 0;
		if (pT && s>getDataLength() && getDataLength()>0)
		{
			size_t part1 = getPart1Read();
			part1 = part1 >= s ? s : part1;
			memcpy(pT, m_buff + m_IndexRead, part1);
			INDEX_MOVE_N(m_IndexRead, part1);
			size_t part2 = getDataLength();
			if (part2 > 0)
			{
				memcpy(pT, m_buff + m_IndexRead, part2);
				INDEX_MOVE_N(m_IndexRead, part2);
			}
			r = part1 + part2;
		}
		return r;
	}

	//下面的方法给coding调用
	inline bool tryWriteByte(unsigned char t)
	{
		bool r = false;
		if (getFreeSpace() >= 1)
		{
			if(m_bIsFakeWrite)
			{ 
				m_buff[m_IndexWriteFake] = t;
				INDEX_MOVE_N(m_IndexWriteFake, 1);
			}
			else
			{
				m_buff[m_IndexWrite] = t;
				INDEX_MOVE_N(m_IndexWrite, 1);
			}
			r = true;
		}
		return r;
	}

	inline unsigned char tryReadByte()
	{
		unsigned char r = 0;
		if (getDataLength() >= 1 )
		{
			if (m_bIsFakeRead)
			{
				r = m_buff[m_IndexReadFake];
				INDEX_MOVE_N(m_IndexReadFake, 1);
			}
			else
			{
				r = m_buff[m_IndexRead];
				INDEX_MOVE_N(m_IndexRead, 1);
			}
		}
		return r;
	}
	inline bool tryWriteShort(unsigned short t)
	{
		bool r = false;
		if (getFreeSpace() >= 2)
		{
			tryWriteByte((unsigned char )(t >> 8));
			tryWriteByte((unsigned char)(t));
			r = true;
		}
		return r;
	}

	inline unsigned short tryReadShort()
	{
		unsigned short r = 0;
		if (getDataLength() >= 2)
		{
			r += tryReadByte() << 8;
			r += tryReadByte();
		}
		return r;
	}
	inline bool tryWriteU29Int(U29Int t)
	{
		bool r = false;
		if (getFreeSpace() >= 3)
		{
			tryWriteByte((unsigned char)(t.t >> 16));
			tryWriteByte((unsigned char)(t.t >> 8));
			tryWriteByte((unsigned char)(t.t));
			r = true;
		}
		return r;
	}
	inline U29Int tryReadU29Int()
	{
		U29Int r = { 0 };
		if (getDataLength() >= 3)
		{
			r.t += tryReadByte() << 16;
			r.t += tryReadByte() << 8;
			r.t += tryReadByte();
		}
		return r;
	}

	inline bool tryWriteInt(unsigned int t)
	{
		bool r = false;
		if (getFreeSpace() >= 4)
		{
			tryWriteShort((unsigned short)(t >> 16));
			tryWriteShort((unsigned short)(t));
			r = true;
		}
		return r;
	}

	inline unsigned int tryReadInt()
	{
		unsigned int r = 0;
		if (getDataLength() >= 4)
		{
			r += tryReadShort() << 16;
			r += tryReadShort();
		}
		return r;
	}

	inline bool tryWriteU64Int(unsigned long long  t)
	{
		bool r = false;
		if (getFreeSpace() >= 8)
		{
			tryWriteInt((unsigned int)(t >> 32));
			tryWriteInt((unsigned int)(t));
			r = true;
		}
		return r;
	}

	inline unsigned long long tryReadU64Int()
	{
		unsigned long long r = 0;
		if (getDataLength() >= 8)
		{
			r += (unsigned long long )tryReadInt() << 32;
			r += tryReadInt();
		}
		return r;
	}

public:
	unsigned char* getBuff() { return m_buff + m_IndexRead; }


private:
	unsigned char m_buff[BYTE_ARRAY_MAX_LENGTH]{0};
	int m_IndexRead{ 0 };
	int m_IndexWrite{ 0 };
	int m_IndexReadFake{ 0 };
	int m_IndexWriteFake{ 0 };
	int m_bIsFakeRead{ false };
	int m_bIsFakeWrite{ false };


public:
	template<class T>void write(T t)
	{
		throw 0;
	};
	template<class T>void write(T t, size_t s)
	{
		throw 0;
	};
	template<class T>void read(T t)
	{
		throw 0;
	};
	template<class T>void read(T t, size_t s)
	{
		throw 0;
	};

};


#endif // !__CPP_BYTE_ARRAY_H__

