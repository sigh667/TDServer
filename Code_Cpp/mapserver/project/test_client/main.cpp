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
#include "test/testServer.h"
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


	if (g_pClientServer 
		&& g_pClientServer->initThread()
		&& g_pClientServer->start())
	{
		g_pClientServer->wait();
	}

	return 0;
}
