/*----------------- thread.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/12/7 11:43:02
*--------------------------------------------------------------
* 线程封装类
*------- linux ----------------
*线程创建步逐(返回EINVAL=失败)
*1 初始化线程属性
*2 设置线程属性
*3 创建线程
pthread_cond_wait
pthread_join
pthread_exit(NULL); 

WaitForSingleObject() = em_wait()
SetEvent = sem_post()
sem_destroy
sem_t sem

http://mingarlic.blog.163.com/blog/static/31148911201081664512718/
uninstall crossover in ubuntu  创建守护进程 daemon.

*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#ifdef WIN32
#include <windows.h>
#include <process.h>
#else//linux
#include <sys/types.h>//系统类型定义
#include <pthread.h> 
#include <signal.h>
#endif//WIN32
/*************************************************************/
/*win32
HANDLE	GetCurrentThread();
DWORD	GetCurrentThreadId();
HANDLE	GetCurrentProcess();				//返回当前进程句柄
DWORD	GetCurrentProcessId();				//返回当前进程ID
DWORD	GetThreadId(HANDLE Thread);			//由线程句柄得到线程ID
DWORD	GetProcessIdOfThread(HANDLE Thread);//由线程句柄得到进程ID
DWORD	GetProcessId( HANDLE Process );		//由进程句柄得到进程ID
DWORD	GetCurrentProcessorNumber();		//当前运行CPU编号 
DWORD_PTR SetThreadAffinityMask(HANDLE hThread,DWORD_PTR dwThreadAffinityMask);//为各个线程设置亲缘性屏蔽
	--线程0只能在cpu 0上运行   
	-SetThreadAffinityMask(hThread0,0x00000001);   
	--//线程1，2，3只能在cpu 1，2，3上运行   
	-SetThreadAffinityMask(hThread1,0x0000000E);   
	-SetThreadAffinityMask(hThread2,0x0000000E);   
	-SetThreadAffinityMask(hThread3,0x0000000E);   
DWORD SetThreadIdealProcessor(HANDLE hThread,DWORD dwIdealProcessor);//设置进程首选的CPU
	--该函数的第二个参数不是位屏蔽数据，而是一个0～31（32位系统）或0～63（64位系统）的整数。该数据指明首选的CPU。
	--也可以传递MAXIMUM_PROCESSORS表明当前没有理想的CPU。
VOID	ExitThread(DWORD dwExitCode);		//退出当前线程	
*/
/*linux
pthread_t	pthread_self(void);								//获取当前线程ID【标识符】
int			pthread_equal(pthread_t tid1, pthread_t tid2);	//比较线程ID【相等 非零值，否则将返回零】
pid_t		getpid();										//获得当前进程
pid_t		getppid();										//父进程
pid_t		getpgrp();										//获取进程组id
pid_t		getpgid(pid_t pid)								//获得指定pid进程所属组的id
int			pthread_exit(void*status);						//退出当前线程
			--如果调用线程尚未分离，则线程 ID 和 status 指定的退出状态将保持不变，
			--直到应用程序调用 pthread_join() 以等待该线程。否则，将忽略 status。线程 ID 可以立即回收
int			pthread_setconcurrency(int new_level);			//设置系统线程所需的并发级别
int			pthread_getconcurrency();						//获取系统线程所需的并发级别【如：pthread_setconcurrency未设置返回0】
int			pthread_sigmask(int _how,const sigset_t *_new,sigset_t *_old)//更改访问调用线程的信号掩码
			-@how 用来确定如何更改信号组
			-SIG_BLOCK		向当前的信号掩码中添加 new，其中 new 表示要阻塞的信号组。 
			-SIG_UNBLOCK	从当前的信号掩码中删除 new，其中 new 表示要取消阻塞的信号组。 
			-SIG_SETMASK	将当前的信号掩码替换为 new，其中 new 表示新的信号掩码。 
*/
#ifdef WIN32
#define THREAD_RETURN	unsigned __stdcall
#else//linux
#define THREAD_RETURN	void*
#endif//WIN32

class CThread
{
#ifdef WIN32
protected:
	HANDLE			m_hThread;
	unsigned int	m_uThreadID;

public:
	inline int64	getThreadID		()
	{
#ifdef WIN32
		return m_uThreadID;
#else//linux
		return int64(m_pthreadID);
#endif//WIN32
	}

#else//linux
protected:
	int32			m_iInit;
	pthread_t		m_pthreadID; 
	pthread_attr_t	m_threadAtrr;

public:
	inline pthread_t		getThreadID		()					{	return m_pthreadID;					}
	inline pthread_attr_t	getThreadAtrr	()					{	return m_threadAtrr;				}

#endif//WIN32

public:
	CThread();
	virtual~CThread();

public:
	//--- 获得当前线程ID
	static int64	getCurrentThreadID();

public:
#ifdef WIN32
	bool	create		(unsigned(__stdcall*start_routine)(void*),void*arg);
#else//linux
	bool	create		(void*(*start_routine)(void*),void*arg);
#endif//WIN32
	void	shutdown	();

//------------------------------ linux线程属性
public://当前线程
	//--- 终止线程
	int32	terminate			(uint64 exit_code = 0);
	//--- 等待线程退出
	int32	wait_exit			();
	//--- 是否运行中
	bool	is_runing			();
	//--- 设置线程的优先级
	int32	set_sched_param		(int32 policy,const struct sched_param *param);
	int32	get_sched_param		(int32*policy,struct sched_param *param);
	//------------------------------ linux线程属性
	//--- 向线程发送信号
	int32	kill				(int32 sig);

public://线程属性
	//--- 线程绑定状态
	int32	set_scope			(int32 scope);
	int32	get_scope			(int32*scope);
	//--- 线程分离状态
	int32	set_detach_state	(int32 detach_state);
	int32	get_detach_state	(int32*detach_state);
	//--- 栈溢出保护区大小
	int32	set_guard_size		(size_t guardsize);
	int32	get_guard_size		(size_t*guardsize);
	//--- 设置调度策略
	int32	set_sched_policy	(int32 policy);
	int32	get_sched_policy	(int32*policy);
	//--- 继承的调度策略
	int32	set_inherit_sched	(int32 inherit);
	int32	get_inherit_sched	(int32*inherit);
	//--- 调度参数
	int32	set_sched_param		(const struct sched_param *param);
	int32	get_sched_param		(struct sched_param *param);
	//--- 栈大小
	int32	set_stack_size		(size_t stacksize);
	int32	get_stack_size		(size_t*stacksize);
	//--- 栈地址和大小
	int32	set_stack			(void *stackaddr,size_t stacksize);
	int32	get_stack			(void **stackaddr,size_t*stacksize);
};

