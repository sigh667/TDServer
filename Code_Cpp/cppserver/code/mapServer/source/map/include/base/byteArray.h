#ifndef __BYTE_ARRAY_H__
#define __BYTE_ARRAY_H__

#define BYTE_ARRAY_MAX_LENGTH (1024*32)
//��loopbuff�����Ϣ���棬����д��͵�������
//�ṩspin lock�����ǵ��ý����ϲ��߼��Լ������ڲ��������裬
#include <atomic>
#include "base/cppByteArray.h"
class CByteArray:public CPPByteArray
{
public:
	void writeLock()
	{//ԭ�ӱ�������������
		while (m_writeLock.test_and_set());
	};
	void writeUnlock()
	{
		m_writeLock.clear();
	};
	void readLock()
	{//ԭ�ӱ�������������
		while (m_readLock.test_and_set());
	};
	void readUnlock()
	{
		m_readLock.clear();
	};


private:
	std::atomic_flag m_writeLock{ ATOMIC_FLAG_INIT };
	std::atomic_flag m_readLock{ ATOMIC_FLAG_INIT };
};

#endif // !__BYTE_ARRAY_H__

