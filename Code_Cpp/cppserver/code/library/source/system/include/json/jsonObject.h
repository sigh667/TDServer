/*----------------- jsonObject.h
*
* Copyright (C): 2015 Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2015/01/22 09:053:12
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "json_c/include/json.h"
#include "basic/basicTypes.h"
/*************************************************************/
struct json_object;
class CJsonObject
{
private:
	bool			m_bAutoPut;
	json_object*	m_pJsonObject;

public:
	CJsonObject(bool bArray = false,bool bAutoPut=false);
	CJsonObject(json_object*pJson,bool bAutoPut=false);
	~CJsonObject();

public:
	inline bool			isArray			()				{	return (m_pJsonObject && json_object_get_type(m_pJsonObject) == json_type_array);	}
	inline json_object*	getObject		()				{	return m_pJsonObject;																}
	//--- get
	inline int32		get_int32		(pc_str key)	{	return (isArray() ? json_object_get_int_key(m_pJsonObject, key) : 0);		}
	inline uint32		get_uint32		(pc_str key)	{	return (isArray() ? json_object_get_uint_key(m_pJsonObject, key) : 0);		}	
	inline int64		get_int64		(pc_str key)	{	return (isArray() ? json_object_get_int64_key(m_pJsonObject, key) : 0);		}
	inline uint64		get_uint64		(pc_str key)	{	return (isArray() ? json_object_get_uint64_key(m_pJsonObject, key) : 0);	}
	inline pc_str		get_string		(pc_str key)	{	return (isArray() ? json_object_get_string_key(m_pJsonObject, key) : 0);	}
	inline byte			get_byte		(pc_str key)	{	return (isArray() ? json_object_get_boolean_key(m_pJsonObject, key) : 0);	}
	inline double		get_double		(pc_str key)	{	return (isArray() ? json_object_get_double_key(m_pJsonObject, key) : 0);	}

public:
	//---�ͷŶ���
	void	put				();
	//--- json to string
	pc_str	json_string		();

public:
	//--- json Object���json Object
	void	add_object		(pc_str szKey, json_object* pValue);
	void	add_object		(pc_str szKey, CJsonObject* pValue);
	void	add_object		(pc_str szKey, CJsonObject& clValue);
	//--- array���json Object
	void	add_object		(CJsonObject* pValue);
	void	del_object		(pc_str szKey);

public:
	//--- ���int32����
	void	add_int32		(pc_str szKey, int32 nValue);
	//--- ���uint32����
	void	add_uint32		(pc_str szKey, uint32 uValue);
	//--- ���int64����
	void	add_int64		(pc_str szKey, int64 nValue);
	//--- ���uint64����
	void	add_uint64		(pc_str szKey, uint64 uValue);
	//--- ���string����
	void	add_string		(pc_str szKey, pc_str szValue);
	//--- ���byte����
	void	add_byte		(pc_str szKey, byte bValue);
	//--- ���double����
	void	add_double		(pc_str szKey, double dValue);

public:
	//--- array���string����
	void	array_add_string(pc_str szValue);
	//--- array���int32����
	void	array_add_int32	(int32 nValue);
	//--- array���uint32����
	void	array_add_uint32(uint32 uValue);
	//--- array���int64����
	void	array_add_int64	(int64 nValue);
	//--- array���uint64����
	void	array_add_uint64(uint64 uValue);
	//--- array���byte����
	void	array_add_byte	(byte bValue);
	//--- array���double����
	void	array_add_double(double dValue);
};
