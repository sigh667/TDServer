/*----------------- luaScript.cpp
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2011/10/24 11:53:49
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "stackChecker/stackChecker.h"

#include "lua_functions/luaStruct.h"

#include "luaScript/luaScript.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
#include "basic/systemFunction.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogic/sceneObject/sceneMonster.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
void	defLuaScript	(lua_State* pL)
{
	FUN_STACK_CHECK();
	if(!pL)
		return;

	defLuaStruct(pL);

	/*
	//------------------------------ CGameSceneMgr
	lua_tinker::class_add<CGameSceneMgr>(pL,"CGameSceneMgr");
	lua_tinker::class_def<CGameSceneMgr>(pL,"buildScene"			,&CGameSceneMgr::buildScene			);
	lua_tinker::class_def<CGameSceneMgr>(pL,"buildVirtualScene"		,&CGameSceneMgr::buildVirtualScene	);

	*/

	//------------------------------ CSceneUnit
	lua_tinker::class_add<CSceneUnit>	(pL,"CSceneUnit");
	lua_tinker::class_def<CSceneUnit>	(pL,"getLevel"		,&CSceneUnit::getLevel);
	lua_tinker::class_def<CSceneUnit>	(pL,"getCampID"		,&CSceneUnit::getCampID);
	lua_tinker::class_def<CSceneUnit>	(pL,"setCampID"		,&CSceneUnit::setCampID);
	lua_tinker::class_def<CSceneUnit>	(pL,"getJob"		,&CSceneUnit::getJob);

	//------------------------------ CScenePlayer
	lua_tinker::class_add<CScenePlayer>	(pL,"CScenePlayer");
	lua_tinker::class_inh<CScenePlayer,CSceneUnit>(pL);

	//------------------------------ CScenePlayer
	lua_tinker::class_add<CSceneMonster>(pL,"CSceneMonster");
}
#ifdef _WIN32
bool _loadLuaScript( lua_State* pL , const char* pszDir )
{
	_finddata_t filestruct;

	std::string strSearch = pszDir;
	strSearch += "/*.*";

	int hnd = _findfirst(strSearch.c_str() , &filestruct);
	if(hnd == -1) 
	{
		return true;
	}
	do
	{
		if ( filestruct.attrib & _A_HIDDEN) 
		{
			continue;
		}

		if (!(filestruct.attrib & _A_SUBDIR)) 
		{
			std::string strFileName = filestruct.name;
			if ( strFileName.find(".lua") != std::string::npos )
			{
				strFileName =	pszDir;
				strFileName	+= "/";
				strFileName += filestruct.name;

				if (!lua_tinker::dofile(pL,strFileName.c_str()))
					return false;
			}
		}
		else // Directory
		{
			if (strcmp(filestruct.name , "..") != 0 && strcmp(filestruct.name, ".") != 0)
			{
				std::string  strPathName =	pszDir;
				strPathName	+= "/";
				strPathName += filestruct.name;

				if ( !_loadLuaScript(pL,strPathName.c_str()) )
					return false;
			}
		}

	}while(!_findnext(hnd , &filestruct));

	return true;
}
#else
bool _loadLuaScript(lua_State* pL , const char* dir )
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(dir)) == NULL) {
		return true;
	}
	chdir(dir);
	while((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) {

			if(strcmp(".",entry->d_name) == 0 ||
				strcmp("..",entry->d_name) == 0)
				continue;

			if ( !_loadLuaScript(pL,entry->d_name) )
				return false;
		}
		else
		{
			if (!lua_tinker::dofile(pL,entry->d_name))
				return false;
		}
	}
	chdir("..");
	closedir(dp);
	return true;
}
#endif
//-------------------------------------------------------------
//------------------------------ 
bool	reLoadLuaScript	(lua_State* pL,const char* pszDir)
{
#ifdef _WIN32
	return _loadLuaScript(pL,pszDir);
#else
	bool bRet = _loadLuaScript(pL,pszDir);
	setWorkDirectory();
	return bRet;
#endif
}