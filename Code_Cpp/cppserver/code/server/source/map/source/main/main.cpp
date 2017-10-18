/*------------- main.cpp
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/2/14 14:27:27
*
*/ 
/***************************************************************
* �������̨Ӧ�ó������ڵ�
***************************************************************/
#include "basic/basicFunctions.h"
#include "basic/systemFunction.h"

#include "main/gameServer.h"
#include "gameLogic/sceneObject/aiRegister.h"
//****************************************************************
int main(int argc, char* argv[])
{
	INIT_STACK_CHECKER();

	FUN_STACK_CHECK();
	if (argc > 1 && (0 == strcmp(argv[1], "/e") || 0 == strcmp(argv[1], "/E")))
	{
		printf("building...\n");
		bool bResult = AIRegister::exportMeta();
		if (!bResult)
			printf("�����ļ�ʧ�ܣ�\n");

#ifdef WIN32
		if (!bResult)
			system("pause");
#endif

		return 0;
	}

	//if(argc < 2)
		setWorkDirectory();

	g_pServer = _single_instance_new(CGameServer);
	if(!g_pServer)
	{
		CServerRoot::messageBoxOK("CGameServer","CGameServer::CreateInstance()ʧ��!");
		return 0;
	}
	uint16 uServerID			= (argc >= 2) ? dAtoui(argv[1]) : 0;
	const char* szConfigPath	= (argc >= 3) ? argv[2] : "../server_config";
	if (!g_pServer->initialize(uServerID, szConfigPath))
	{
		CServerRoot::messageBoxOK("CGameServer","CGameServer::Initialize()ʧ��!");
		_single_instance_del(g_pServer); 
		return 0;
	}

	if (g_pServer->startServices())
	{
		g_pServer->maintenances();
	}
	else
	{
		CGameServer::messageBoxOK("��������ʧ��",":StartServices()��������ʧ��!");
	}

	g_pServer->stopServices();
	_single_instance_del(g_pServer);

	return 0;
}
