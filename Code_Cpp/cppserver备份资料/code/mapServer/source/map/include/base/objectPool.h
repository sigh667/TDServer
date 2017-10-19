#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include <list>

//凡是new出来的对象，都交付给对象池管理 
class IMemObject
{
public:
	void setGarbage() { m_bIsGarbage = true; }
	void setReuse() { m_bIsGarbage = false; }
private:
	bool m_bIsGarbage{ false };
};


//谁创建，谁释放，但是对象可以交给另外的线程使用
template<class T> class CObjectMemPool
{
public:
	CObjectMemPool()
	{
	}
	virtual ~CObjectMemPool()
	{

	}
	bool initMemPool(int initCount)
	{
		T* tA = new T[initCount];
		if (!tA)
		{
			return false;
		}
		for (size_t i = 0; i < initCount; i++)
		{
			m_lstIdle.push_back(tA + i);
		}
		return true;
	}
private:
	std::list<T*> m_lstIdle;
	std::list<T*> m_lstInUse;

};


#endif // !__OBJECT_POOL_H__
