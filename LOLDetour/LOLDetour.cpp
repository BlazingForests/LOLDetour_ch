
#include <regex>
#include "GameProc.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <locale.h>

#define FILE_CFG "lolxzs_cfg"
#define FILE_SKN "lolxzs_skn"
#pragma warning (disable:4996)

CONFIG_EXE cfgEXE;

float TOP_MIS,MID_MIS,BOT_MIS;
DWORD CHWND_LOL,sk_mCamp;
char gTxt[255];
void *pHook_Ret,*pHook_Ret2,*pCreateFile,*pHook_Zoom;
HMODULE hInstance;
HHOOK g_hLOLGetMsg,g_hGetMsg,g_hKey;
HANDLE dwThread_Func;
MEMORYINFO MEM_SKIN,MEM_SKIN2;
MEMORYINFO MEM_CREATFILE;
MEMORYINFO MEM_ZoomHook;


DWORD dwBASE_LOL;
HWND hGame,hGame_LOL;
BOOL bInitLOLClient,bInitLOL;
LPSTR ErrorReason;HANDLE g_hMemFile,g_hMemFile_skn,m_hMemFile;

DWORD WINAPI dwWaitThread( LPVOID lpArgs );
BOOL LoadCFG(LPCSTR PATH,CONFIG_EXE* cfg);


char tmp[255]; HANDLE g_hMemFile_CommandLine;
LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam){
	PMSG pMsg=(PMSG)lParam;
	char* lpAddr;
	
	if(wParam==0){
	switch (pMsg->message){
	case 0x801:
		hGame_LOL=pMsg->hwnd;
		g_hGetMsg=(HHOOK)(pMsg->lParam);
		if(!bInitLOLClient) {
			bInitLOLClient=TRUE;
			CreateThread(NULL,NULL,&dwWaitThread,NULL,NULL,NULL);
		}
		break;
	case 0x802:
		g_hMemFile=OpenFileMappingA(FILE_MAP_ALL_ACCESS,FALSE,FILE_CFG);
		if(g_hMemFile){
			lpAddr=(char*)MapViewOfFile(g_hMemFile,FILE_MAP_ALL_ACCESS,0,0,255);
			DbgPrintA("[Hook]GetCFG PATH %s",lpAddr);
			LoadCFG(lpAddr,&cfgEXE);
			UnmapViewOfFile(lpAddr);
			CloseHandle(g_hMemFile);
			}else{
				DbgPrintA("[Hook] OpenFileFILE_CFG Error");
		}
		break;
	case 0x803:
		g_hGetMsg=(HHOOK)(pMsg->lParam);
		if(!bInitLOL) {
			DbgPrintA("[Hook]Init");
			bInitLOL=TRUE;
			CreateThread(NULL,NULL,&dwWaitThread,NULL,NULL,NULL);
		}
		else {
			DbgPrintA("[Hook]has been Inited");

		}
		break;
	case 0x805:
		if (!bInitLOL) {
			bInitLOL = TRUE;
			CreateThread(NULL, NULL, &dwWaitThread, NULL, NULL, NULL);
		}

		break;
	}
	}
	return CallNextHookEx(g_hGetMsg,code,wParam,lParam);
}

BOOL LoadCFG(LPCSTR PATH,CONFIG_EXE* cfg){
	return TRUE;
}




DWORD WINAPI dwWaitThread( LPVOID lpArgs )
{ 
	DWORD dwBase;
	DbgPrintA("[Hook]dwWaitThread Create",lpArgs);
	if(bInitLOL){
		dwBase=(DWORD)GetModuleHandleA("League of Legends.exe");
		if (dwBase)
		{
			dwBASE_LOL=dwBase;
			InitCode();
		}
	}else if(bInitLOLClient){
		dwBase=(DWORD)GetModuleHandleA("LolClient.exe");
		if (dwBase)
		{
			g_hMemFile_CommandLine = CreateFileMappingA((HANDLE)-1, NULL, PAGE_READWRITE, 0, 0x1000, "zeCommand");
			if (g_hMemFile_CommandLine) {
				
				char* lpAddr;
				lpAddr = (char*)MapViewOfFile(g_hMemFile_CommandLine, FILE_MAP_ALL_ACCESS, 0, 0, 800);	
				strcpy(lpAddr,GetCommandLineA());
				UnmapViewOfFile(lpAddr);
			}			
			//InitCreateFile();
		}
	}
	return 0;
}
BOOL WINAPI IsAdministrator() {//判断是否管理员模式  

	BOOL bIsElevated = FALSE;
	HANDLE hToken = NULL;
	UINT16 uWinVer = LOWORD(GetVersion());
	uWinVer = MAKEWORD(HIBYTE(uWinVer), LOBYTE(uWinVer));

	if (uWinVer < 0x0600)//不是VISTA、Windows7  
		return(FALSE);

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {

		struct {
			DWORD TokenIsElevated;

		} /*TOKEN_ELEVATION*/te;
		DWORD dwReturnLength = 0;

		if (GetTokenInformation(hToken,/*TokenElevation*/(_TOKEN_INFORMATION_CLASS)20, &te, sizeof(te), &dwReturnLength)) {
			if (dwReturnLength == sizeof(te))
				bIsElevated = te.TokenIsElevated;

		}
		CloseHandle(hToken);

	}
	return bIsElevated;
}

DWORD WINAPI GetHookPort(){
	return (DWORD)g_hGetMsg;
}
DWORD WINAPI ccvv(){
	DWORD dwThreadID;
	
	hGame=FindWindowA("ApolloRuntimeContentWindow","PVP.net 客户端");//PVP.net 客户端 PVP.net Client
	if (!hGame) { 
		g_hGetMsg = NULL;
		return NULL; 
	}
	else {
		if (g_hGetMsg) return (DWORD)g_hGetMsg;
	}
	dwThreadID=GetWindowThreadProcessId(hGame,NULL);
	if(!dwThreadID) return NULL;
	g_hGetMsg=SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc,hInstance,dwThreadID);
	if(!g_hGetMsg) return NULL;
	PostMessageA(hGame,0x801,0,(LPARAM)g_hGetMsg);
	return (DWORD)g_hGetMsg;
}
BOOL WINAPI UnHook_LOLClient(){
	if(g_hGetMsg) UnhookWindowsHookEx(g_hGetMsg);
	g_hGetMsg=NULL;
	return TRUE;
}

void WINAPI SendCode(const int Code){
	PostMessageA(hGame_LOL,Code,NULL,(LPARAM)g_hLOLGetMsg);
}

DWORD WINAPI ccvva(){
	DWORD dwThreadID;
	hGame_LOL=FindWindowA("RiotWindowClass","League of Legends (TM) Client");
	if(!hGame_LOL) return NULL;
	dwThreadID=GetWindowThreadProcessId(hGame_LOL,NULL);
	g_hLOLGetMsg=SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc,(HINSTANCE)hInstance,dwThreadID);
	if(!g_hLOLGetMsg) return NULL;
	PostMessageA(hGame_LOL,0x803,0,(LPARAM)g_hLOLGetMsg);
	return (DWORD)g_hLOLGetMsg;
}

BOOL WINAPI UnHook_LOL(){
	if(g_hLOLGetMsg) UnhookWindowsHookEx(g_hLOLGetMsg);
	g_hLOLGetMsg=NULL;
	return TRUE;
}


BOOL WINAPI LoadConfig(LPCSTR lpPath){
	LPVOID lpAddr;
	if(lpPath==NULL) return FALSE;
	if(!m_hMemFile){
		m_hMemFile=CreateFileMappingA((HANDLE)-1,NULL,PAGE_READWRITE,0,255,FILE_CFG);
	}
	if(m_hMemFile){
		lpAddr=MapViewOfFile(m_hMemFile,FILE_MAP_ALL_ACCESS,0,0,255);
		strcpy_s((char*)lpAddr,255,lpPath);
		UnmapViewOfFile(lpAddr);
		PostMessageA(hGame_LOL,0x802,0,(LPARAM)g_hLOLGetMsg);
		return TRUE;
	}else{
		DbgPrintA("[Hook]CreateFileMap Error");
		return FALSE;
	}

	return TRUE;
}



BOOL WINAPI Unload(){
	hInstance=NULL;
	if(bInitLOL){
		UnHookHeroName();
		bInitLOL=FALSE;
	}
	if(bInitLOLClient){
		if(g_hMemFile_CommandLine) CloseHandle(g_hMemFile_CommandLine);
		RetourFunc(MEM_CREATFILE);
		bInitLOLClient=FALSE;
	}
	if(m_hMemFile){
		CloseHandle(m_hMemFile);
		m_hMemFile=NULL;
	}
	if(g_hMemFile){
		CloseHandle(g_hMemFile);
		g_hMemFile=NULL;
	}
	if(g_hMemFile_skn){
		CloseHandle(g_hMemFile_skn);
		g_hMemFile_skn=NULL;
	}
	return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hGetMsg = NULL;
		hInstance=hModule;
		setlocale(LC_ALL, "chs");
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Unload();
		break;
	}
	return TRUE;
}