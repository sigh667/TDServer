/*----------------- net_encrypt.h
*
* Copyright (C): 2012  Mokylin·Mokyqi
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
 * 网络加密算法类
 */
class Encrypt
{
private:
    uint64 m_key;
    /** 根据输入的密钥缓冲区，生成起始密钥 **/
    void makeKey(const uint8* key, int32 len);
    /** 根据当前的key值，生成下一个key值 **/
    void makeNextKey();
public:
	inline uint64 getKey	()				{	return m_key;		}
	inline void	 setKey		(uint64 uKey)	{	m_key = uKey;		}

public:
    Encrypt();
    /**
     * 设置加密使用的密钥
     * @param key 加密使用的密钥
     * @param len 密钥缓冲区长度
     * @author 赵文源 2012-10-11
     */
    void setKey(const uint8* key, int32 len);
    
    /**
     * 加密source指定的数据，每次加密都会导致密钥更新
     * @param source 需要被加密的数据缓冲区
     * @param len 需要被加密的数据缓冲区长度
     * @param dest 加密后内容输出缓冲区，这个缓冲区的大小必须大于len
     * @return 返回加密后内容输出缓冲区地址
     * @author 赵文源 2012-10-11
     */
    uint8* encrypt(const uint8* source, int32 len, uint8* dest);
    
    /**
     * 解密source指定的内容，每次解密都会导致密钥更新
     * @param source 需要被解密的数据缓冲区
     * @param len 需要被解密的数据缓冲区
     * @param dest 解密后的数据缓冲区，这个缓冲区的大小必须大于len
     * @return 返回解密后的内容输出缓冲区地址
     * @author 赵文源 2012-10-11
     */
    uint8* decrypt(const uint8* source, int32 len, uint8* dest);
};

#endif /* defined(__encrypt__encrypt__) */
