/*----------------- stackChecker.h
*
* Copyright (C): 2011
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/10/13 9:50:41
*--------------------------------------------------------------
*��⺯����ջʹ��
*------------------------------------------------------------*/
#pragma once
#include <assert.h>
/*************************************************************/
#ifdef WIN32
//-------------------------------------------------------------
//------------------------------ 
class CStackChecker
{
private:
	long long ESPChecker;

public:
	unsigned int nEbp;
	unsigned int nEsp;

	static __declspec( thread ) unsigned int g_uEbp;

public:
	CStackChecker();
	~CStackChecker();

public:
	void check();
};

//--- ��ʼ����ջ���
#define INIT_STACK_CHECKER()		\
{											\
	unsigned int uEbp = 0;			\
	__asm { mov [uEbp], EBP };		\
	CStackChecker::g_uEbp = uEbp;	\
}

//--- ������ջ���
#define FUN_STACK_CHECK()\
	CStackChecker s_clStackChecker;					\
	__asm { mov [s_clStackChecker.nEsp], ESP };	\
	__asm { mov [s_clStackChecker.nEbp], EBP };	\
	s_clStackChecker.check();

#else //WIN32

//--- ��ʼ����ջ���
#define INIT_STACK_CHECKER()
//--- ������ջ���
#define FUN_STACK_CHECK()

#endif//WIN32
//-------------------------------------------------------------
//------------------------------ ʹ�÷���
/*
//--- ��ջ����д
void foo()
{
	FUN_STACK_CHECK();

	int n[10];
	for (int i=10; i<100; i++)
		n[i] = 0;
}

//--- ��ջ���
void foo2()
{
	FUN_STACK_CHECK();
	int n[200*1024];
	for (int i=10; i<100; i++)
		n[i] = 0;
}

int main(int argc, char* argv[])
{
	INIT_STACK_CHECKER();

	FUN_STACK_CHECK();	

	foo();
	//	foo2();

	return 0;
}
*/
