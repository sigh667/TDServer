/*------------- main.cpp
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/14 14:27:27
*
*/ 
/***************************************************************
* 定义控制台应用程序的入口点
***************************************************************/
//#include "basic/basicFunctions.h"
//#include "basic/systemFunction.h"
//
//#include "main/gameServer.h"
//#include "gameLogic/sceneObject/aiRegister.h"
#include "logic/server.h"
#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif
//****************************************************************
int main(int argc, char* argv[])
{



#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}
#endif // WIN32

//	INIT_STACK_CHECKER();
//
//	FUN_STACK_CHECK();
//	if (argc > 1 && (0 == strcmp(argv[1], "/e") || 0 == strcmp(argv[1], "/E")))
//	{
//		printf("building...\n");
//		bool bResult = AIRegister::exportMeta();
//		if (!bResult)
//			printf("导出文件失败！\n");
//
//#ifdef WIN32
//		if (!bResult)
//			system("pause");
//#endif
//
//		return 0;
//	}
//
//	//if(argc < 2)
//		setWorkDirectory();
//
//	g_pServer = _single_instance_new(CGameServer);
//	if(!g_pServer)
//	{
//		CServerRoot::messageBoxOK("CGameServer","CGameServer::CreateInstance()失败!");
//		return 0;
//	}
//	uint16 uServerID			= (argc >= 2) ? dAtoui(argv[1]) : 0;
//	const char* szConfigPath	= (argc >= 3) ? argv[2] : "../server_config";
//	if (!g_pServer->initialize(uServerID, szConfigPath))
//	{
//		CServerRoot::messageBoxOK("CGameServer","CGameServer::Initialize()失败!");
//		_single_instance_del(g_pServer); 
//		return 0;
//	}
//
//	if (g_pServer->startServices())
//	{
//		g_pServer->maintenances();
//	}
//	else
//	{
//		CGameServer::messageBoxOK("启动服务失败",":StartServices()启动服务失败!");
//	}
//
//	g_pServer->stopServices();
//	_single_instance_del(g_pServer);
//

	if (g_pServer 
		&& g_pServer->initThread()
		&& g_pServer->start())
	{
		g_pServer->wait();
	}

	return 0;
}
