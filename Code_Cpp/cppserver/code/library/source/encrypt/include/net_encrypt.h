/*----------------- net_encrypt.h
*
* Copyright (C): 2012  Mokylin��Mokyqi
* Author       : kuhx
* Version      : V1.0
* Date         : 2012/10/20 11:51:32
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#ifndef __encrypt__encrypt__
#define __encrypt__encrypt__

#include <iostream>
#include "basic/basicTypes.h"
/*************************************************************/

/**
 * ��������㷨��
 */
class Encrypt
{
private:
    uint64 m_key;
    /** �����������Կ��������������ʼ��Կ **/
    void makeKey(const uint8* key, int32 len);
    /** ���ݵ�ǰ��keyֵ��������һ��keyֵ **/
    void makeNextKey();
public:
	inline uint64 getKey	()				{	return m_key;		}
	inline void	 setKey		(uint64 uKey)	{	m_key = uKey;		}

public:
    Encrypt();
    /**
     * ���ü���ʹ�õ���Կ
     * @param key ����ʹ�õ���Կ
     * @param len ��Կ����������
     * @author ����Դ 2012-10-11
     */
    void setKey(const uint8* key, int32 len);
    
    /**
     * ����sourceָ�������ݣ�ÿ�μ��ܶ��ᵼ����Կ����
     * @param source ��Ҫ�����ܵ����ݻ�����
     * @param len ��Ҫ�����ܵ����ݻ���������
     * @param dest ���ܺ��������������������������Ĵ�С�������len
     * @return ���ؼ��ܺ����������������ַ
     * @author ����Դ 2012-10-11
     */
    uint8* encrypt(const uint8* source, int32 len, uint8* dest);
    
    /**
     * ����sourceָ�������ݣ�ÿ�ν��ܶ��ᵼ����Կ����
     * @param source ��Ҫ�����ܵ����ݻ�����
     * @param len ��Ҫ�����ܵ����ݻ�����
     * @param dest ���ܺ�����ݻ�����������������Ĵ�С�������len
     * @return ���ؽ��ܺ�����������������ַ
     * @author ����Դ 2012-10-11
     */
    uint8* decrypt(const uint8* source, int32 len, uint8* dest);
};

#endif /* defined(__encrypt__encrypt__) */
