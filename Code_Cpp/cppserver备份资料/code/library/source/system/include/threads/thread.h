/*----------------- thread.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/12/7 11:43:02
*--------------------------------------------------------------
* �̷߳�װ��
*------- linux ----------------
*�̴߳�������(����EINVAL=ʧ��)
*1 ��ʼ���߳�����
*2 �����߳�����
*3 �����߳�
pthread_cond_wait
pthread_join
pthread_exit(NULL); 

WaitForSingleObject() = em_wait()
SetEvent = sem_post()
sem_destroy
sem_t sem

http://mingarlic.blog.163.com/blog/static/31148911201081664512718/
uninstall crossover in ubuntu  �����ػ����� daemon.

*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#ifdef WIN32
#include <windows.h>
#include <process.h>
#else//linux
#include <sys/types.h>//ϵͳ���Ͷ���
#include <pthread.h> 
#include <signal.h>
#endif//WIN32
/*************************************************************/
/*win32
HANDLE	GetCurrentThread();
DWORD	GetCurrentThreadId();
HANDLE	GetCurrentProcess();				//���ص�ǰ���̾��
DWORD	GetCurrentProcessId();				//���ص�ǰ����ID
DWORD	GetThreadId(HANDLE Thread);			//���߳̾���õ��߳�ID
DWORD	GetProcessIdOfThread(HANDLE Thread);//���߳̾���õ�����ID
DWORD	GetProcessId( HANDLE Process );		//�ɽ��̾���õ�����ID
DWORD	GetCurrentProcessorNumber();		//��ǰ����CPU��� 
DWORD_PTR SetThreadAffinityMask(HANDLE hThread,DWORD_PTR dwThreadAffinityMask);//Ϊ�����߳�������Ե������
	--�߳�0ֻ����cpu 0������   
	-SetThreadAffinityMask(hThread0,0x00000001);   
	--//�߳�1��2��3ֻ����cpu 1��2��3������   
	-SetThreadAffinityMask(hThread1,0x0000000E);   
	-SetThreadAffinityMask(hThread2,0x0000000E);   
	-SetThreadAffinityMask(hThread3,0x0000000E);   
DWORD SetThreadIdealProcessor(HANDLE hThread,DWORD dwIdealProcessor);//���ý�����ѡ��CPU
	--�ú����ĵڶ�����������λ�������ݣ�����һ��0��31��32λϵͳ����0��63��64λϵͳ����������������ָ����ѡ��CPU��
	--Ҳ���Դ���MAXIMUM_PROCESSORS������ǰû�������CPU��
VOID	ExitThread(DWORD dwExitCode);		//�˳���ǰ�߳�	
*/
/*linux
pthread_t	pthread_self(void);								//��ȡ��ǰ�߳�ID����ʶ����
int			pthread_equal(pthread_t tid1, pthread_t tid2);	//�Ƚ��߳�ID����� ����ֵ�����򽫷����㡿
pid_t		getpid();										//��õ�ǰ����
pid_t		getppid();										//������
pid_t		getpgrp();										//��ȡ������id
pid_t		getpgid(pid_t pid)								//���ָ��pid�����������id
int			pthread_exit(void*status);						//�˳���ǰ�߳�
			--��������߳���δ���룬���߳� ID �� status ָ�����˳�״̬�����ֲ��䣬
			--ֱ��Ӧ�ó������ pthread_join() �Եȴ����̡߳����򣬽����� status���߳� ID ������������
int			pthread_setconcurrency(int new_level);			//����ϵͳ�߳�����Ĳ�������
int			pthread_getconcurrency();						//��ȡϵͳ�߳�����Ĳ��������磺pthread_setconcurrencyδ���÷���0��
int			pthread_sigmask(int _how,const sigset_t *_new,sigset_t *_old)//���ķ��ʵ����̵߳��ź�����
			-@how ����ȷ����θ����ź���
			-SIG_BLOCK		��ǰ���ź���������� new������ new ��ʾҪ�������ź��顣 
			-SIG_UNBLOCK	�ӵ�ǰ���ź�������ɾ�� new������ new ��ʾҪȡ���������ź��顣 
			-SIG_SETMASK	����ǰ���ź������滻Ϊ new������ new ��ʾ�µ��ź����롣 
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
	//--- ��õ�ǰ�߳�ID
	static int64	getCurrentThreadID();

public:
#ifdef WIN32
	bool	create		(unsigned(__stdcall*start_routine)(void*),void*arg);
#else//linux
	bool	create		(void*(*start_routine)(void*),void*arg);
#endif//WIN32
	void	shutdown	();

//------------------------------ linux�߳�����
public://��ǰ�߳�
	//--- ��ֹ�߳�
	int32	terminate			(uint64 exit_code = 0);
	//--- �ȴ��߳��˳�
	int32	wait_exit			();
	//--- �Ƿ�������
	bool	is_runing			();
	//--- �����̵߳����ȼ�
	int32	set_sched_param		(int32 policy,const struct sched_param *param);
	int32	get_sched_param		(int32*policy,struct sched_param *param);
	//------------------------------ linux�߳�����
	//--- ���̷߳����ź�
	int32	kill				(int32 sig);

public://�߳�����
	//--- �̰߳�״̬
	int32	set_scope			(int32 scope);
	int32	get_scope			(int32*scope);
	//--- �̷߳���״̬
	int32	set_detach_state	(int32 detach_state);
	int32	get_detach_state	(int32*detach_state);
	//--- ջ�����������С
	int32	set_guard_size		(size_t guardsize);
	int32	get_guard_size		(size_t*guardsize);
	//--- ���õ��Ȳ���
	int32	set_sched_policy	(int32 policy);
	int32	get_sched_policy	(int32*policy);
	//--- �̳еĵ��Ȳ���
	int32	set_inherit_sched	(int32 inherit);
	int32	get_inherit_sched	(int32*inherit);
	//--- ���Ȳ���
	int32	set_sched_param		(const struct sched_param *param);
	int32	get_sched_param		(struct sched_param *param);
	//--- ջ��С
	int32	set_stack_size		(size_t stacksize);
	int32	get_stack_size		(size_t*stacksize);
	//--- ջ��ַ�ʹ�С
	int32	set_stack			(void *stackaddr,size_t stacksize);
	int32	get_stack			(void **stackaddr,size_t*stacksize);
};

