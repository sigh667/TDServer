/*------------- Encrypt.h
* Copyright (C): 2011 Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/8/22 9:47:37
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "blowfish.h"
#include "rijndael.h"
#include "xor256.h"
#include "mars.h"
#include "serpent.h"
/*************************************************************/
const int BlockSize = 16;			//���������ﶨ��Կ����Ϊ128bit

class CEncrypt
{
public:
	enum EncryptEnum
	{
		Encrypt_XOR256			,		//����ģʽ
		Encrypt_BlowFish_ECB	,		//BlowFish��ECBģʽ
		Encrypt_BlowFish_CBC	,		//BlowFish��CBCģʽ
		Encrypt_BlowFish_CFB	,		//BlowFish��CFBģʽ
		Encrypt_AES_ECB		,		//AES��ECBģʽ
		Encrypt_AES_CBC		,		//AES��CBCģʽ
		Encrypt_AES_CFB		,		//AES��CFBģʽ
		Encrypt_MARS			,		//IBM's MARS�����㷨
		Encrypt_SERPENT		,		//Serpent�㷨
		Encrypt_MaxNum
	};
private:
	EncryptEnum m_type;
	char			PublicKey[BlockSize+1];	//��Կ
	char			XORKey[257];				//���256�ֽ���Կ		

public:
	bool	makeKey	(int type, const char* KeyValue);
	bool	encrypt	(char* InString, char* OutString, int BufferLen);
	bool	decrypt	(char* InString, char* OutString, int BufferLen);		
};
