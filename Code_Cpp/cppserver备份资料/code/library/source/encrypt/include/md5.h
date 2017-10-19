/*------------- md5.h
* Copyright (C): 2011 Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/8/22 9:54:14
*
*/ 
/***************************************************************
* MD5����
***************************************************************/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
/*************************************************************/
//------------------------------------------------------------------------------
//------------------------------------- 
class CMD5
{
private:
	typedef unsigned int (*FunT)(unsigned int x,unsigned int y,unsigned int z);

public:
	enum
	{
		MD5_LEN		 = 16		,	/*MD5������Ƴ���*/ 
		MD5_STR_LEN = 32 + 1,	/*MD5���ַ�������*/ 
	};

private:
	/*����ѭ������*/ 
	static const unsigned int guArray[4][4];
	/*��ʼ��MD5����*/ 
	static const unsigned int A;
	static const unsigned int B;
	static const unsigned int C;
	static const unsigned int D;

private:
	unsigned int	m_guMD5[4];
	unsigned char	m_gucMD5[MD5_LEN];
	char				m_szMD5Str[MD5_STR_LEN];

	unsigned char	m_ucAppendLength;
	unsigned char	m_gucAppendBit[64];
	unsigned char	m_gucAppendLength[8];

	unsigned int	m_guTransform[16];

public:
	CMD5();
	~CMD5();

public:
	void	initMD5(const char* pBuf = NULL,unsigned int uLength = 0);

public:
	/*MD5���ַ�������*/ 
	inline char*				GetMD5Str	()	{	return m_szMD5Str;	}
	/*MD5������Ƴ���*/ 
	inline unsigned char*	GetMD5		()	{	return m_gucMD5;		}

private:	
	/*�ĸ������İ�λ��������*/ 
	/*----->{ F(X,Y,Z) =(X&Y)|((~X)&Z) }*/ 
	static unsigned int	F(unsigned int x,unsigned int y,unsigned int z);
	/*----->{ G(X,Y,Z) =(X&Z)|(Y&(~Z)) }*/ 
	static unsigned int	G(unsigned int x,unsigned int y,unsigned int z);
	/*----->{ H(X,Y,Z) =X^Y^Z }*/ 
	static unsigned int	H(unsigned int x,unsigned int y,unsigned int z);
	/*----->{ I(X,Y,Z)=Y^(X|(~Z) }*/ 
	static unsigned int	I(unsigned int x,unsigned int y,unsigned int z);

private:	
	/*----->{ �������ת��FF, GG, HH,II�ĸ����� }*/ 
	void	XX(FunT fun, unsigned int& a,unsigned int b,unsigned int c,unsigned int d,unsigned int mj,int s,unsigned int ti);

private:
	/*void	AppendBit(const unsigned __int64& uLength);
	void	Transform();
	void	ArrayData(const char* pBuf,const unsigned __int64& uLength);*/

private:	
	/*----->{ ��λ,�������ݳ��� }*/ 
	std::vector<unsigned int>	appendBit(std::vector<unsigned char> Input);

private:
	/*----->{ �������������任 }*/ 
	void	transform(std::vector<unsigned int> m);
	/*----->{ ���ܺ������ }*/ 
	void	arrayData(std::vector<unsigned char> input);
	/*----->{ ��ʽ�������� }*/ 
	char*	formatHex(bool uppercase);

public:
	/*----->{ �õ�MD5��� }*/ 
	char*			getMD5Str(const char* pBuf,unsigned int uLength,bool bUppercase = false);
	/*----->{ �õ�MD5��� }*/ 
	unsigned char*	getMD5	(const char* pBuf,unsigned int uLength);
};
//------------------------------------------------------------------------------
//------------------------------------- ȫ��MD5�ַ�����ȡ
const char*			getMD5Str(const char* password,int size,bool bUppercase = false);
//------------------------------------- ȫ��MD5���봮��ȡ
const unsigned char* getMD5	(const char* password,int size);
