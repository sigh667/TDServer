/*----------------- find_path.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/8/10 21:41:07
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include <string>
#ifdef WIN32
#include <windows.h>
#else//WIN32
#include <sys/types.h>	//系统类型定义
#include <sys/stat.h>
#include <dirent.h>
#endif//WIN32
/*************************************************************/
class find_path
{
public:
	struct find_data
	{
		std::string		dir_path;
#ifdef WIN32
		WIN32_FIND_DATA	dir_data;
#else//WIN32
		struct stat		dir_stat;
		struct dirent*	dir_data;
#endif//WIN32

		void	init_data	();
		bool	is_empty	();
		bool	is_dir		();
		bool	is_file		();
		pc_str	get_name	();
		pc_str	get_path	();
	};

protected:
#ifdef WIN32
	HANDLE	m_find;
#else//WIN32
	DIR*	m_find;
#endif//WIN32

public:
	find_path();
	~find_path();

public:
	void	close_dir	();
	bool	open_dir	(pc_str _path,find_data&_data);
	bool	next_dir	(pc_str _path,find_data&_data);
};
