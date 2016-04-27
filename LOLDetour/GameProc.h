#pragma once
///////GameProc//////////
#define DETOUR_TYPE_JMP 1
#define DETOUR_TYPE_CALL 2

#define CAMP_BLUE 100
#define CAMP_PURPLE 200

#define LINE_MISS 0
#define LINE_TOP 1
#define LINE_MID 2
#define LINE_BOT 3

#define APPNAME "Config"
#define JUMPSET "JUMPSETTING"

#define OFFSET_DES_1 0x4EF4
#define OFFSET_DES_2 0x60

#include <windows.h>
#include <vector>
#include <string> 
using namespace std; 

#define SightWard 0x68676953 
#define VisionWard 0x69736956


typedef struct MEMORYINFO{
	BOOL IsHook;
	DWORD dwAddr;
	int len;
	BYTE* lpOriginCode;
}MEMORYINFO;
typedef MEMORYINFO* PMEMORYINFO;

typedef struct CONFIG_EXE 
{
	char path[255];
	char Msg1[255];
	char Msg2[255];
	char Msg3[255];
	char Msg4[255];
	char Msg5[255];
	char Msg6[255];	
	char Msg7[255];
	char Msg8[255];
	char Msg9[255];
	int bGankNotice;
	int bShowPath;
	int bAutoSend;
	int bJumpGuard;
	int bZoom;
	int bCDTimeFriend;
	int bCDTimeArmy;
	int bMissProc;
	int bIconNotice;
	int bSkin;
	int bShowHP;
	int bLastHit;
	int bADistance;
	int bTowerDistance;
	int bIconTimer;
	int bWardTimer;
}CONFIG_EXE;

char* Utf82Unicode(const char* utf, size_t *unicode_number);
char* Unicode2Utf8(const char* unicode);

DWORD MakeLParam(UINT Key,UINT State);
void *memcpy_m(void *dest, const void *src, int n);
LPVOID DetourFunc(BYTE *src, const BYTE *dst, int len,int TYPE,PMEMORYINFO lpMemInfo);
void RetourFunc (MEMORYINFO MemInfo);
DWORD FindPattern(DWORD dwdwAdd,DWORD dwLen,BYTE *bMask,char * szMask);
DWORD SearchSignature(PBYTE code,PCHAR szMask,DWORD dwStartAddress,int ID);
DWORD GetAddress(DWORD* Addr);
BOOL HideModuleByHandle(HMODULE hModule);
void _stdcall DbgPrintW(const WCHAR *format, ...);
void _stdcall DbgPrintA(const char *format, ...);
wstring ANSIToUnicode(const string& str);


//////GameFunc//////////
typedef struct OBJECT_POSITION{
	float x;
	float z;
	float y;
}OBJECT_POSITION;
typedef struct INFOTEXT{
	float x;
	float y;
	char* Text;
	DWORD BeginTime;
	DWORD lastTime;
}INFOTEXT;

typedef OBJECT_POSITION* POBJECT_POSITION;


BOOL InitCode();
int GetObjectCamp(DWORD ObjectBase);
float GetObjectHP_Min(DWORD ObjectBase);
void UnHookHeroName();