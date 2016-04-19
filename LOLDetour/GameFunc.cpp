#include "GameProc.h"
#pragma comment(lib, "version.lib")


extern DWORD dwBASE_LOL;
extern HWND hGame_LOL;
extern HMODULE hInstance;
extern CONFIG_EXE cfgEXE;

BOOL bStartD3DHook;
BOOL bDraw;
DWORD dwBASE_MyObject=0;
DWORD dwBASE_NameHook=0;
DWORD dwBASE_GameTime=0;

void HookHeroName();



DWORD GetMyObjectBase(){
	DWORD ret;
	try{
		if(!dwBASE_MyObject) return 0;
		ret=*(DWORD*)(dwBASE_MyObject);
	}catch(...){}	
	return ret;
}

//[A-Z0-9][A-Z0-9] ->\\x\0 
//[A-Z0-9] -> x


BOOL InitCode(){
	DWORD dwResult;DWORD TIME;BOOL bFind=TRUE;
	try{
		//TIME=GetTickCount();
		////MyObjectBase
		//dwResult=SearchSignature((PBYTE)"\x56\x57\x8B\x3D\x00\x00\x00\x00\xFF\x50\x0C","xxxx????xxx",dwBASE_LOL,5);
		//if(dwResult){
		//	dwResult=*(DWORD*)(dwResult+0x4);
		//	dwBASE_MyObject=dwResult;
		//}else{
		//	bFind=FALSE;
		//}

		//dwResult=SearchSignature((PBYTE)"\x6A\x01\x83\xEC\x08\xC7\x44\x24\x04\x00\x00\x80\x3F\xC7\x04\x24\x00\x00\x80\x3F","xxxxxxxxxxxxxxxxxxxx",dwBASE_LOL,14);
		//if(dwResult){
		//	dwBASE_NameHook=dwResult;
		//}else{
		//	bFind=FALSE;
		//}	

		//dwResult=SearchSignature((PBYTE)"\x8B\x0D\x00\x00\x00\x00\x80\x79\x38\x00\x75\x1A","xx????xxxxxx",dwBASE_LOL,15);
		//if(dwResult){
		//	dwBASE_GameTime=*(DWORD*)(dwResult+0x2);//-0x16
		//}else{
		//	bFind=FALSE;
		//}

		//
		//if(!bFind){
		//	Sleep(3000);
		//	InitCode();
		//	return FALSE;
		//}


	//if(!bFind){
	//	Sleep(3000);
	//	InitCode();
	//	return FALSE;
	//}
		//dwBASE_MyObject = dwBASE_LOL + 0x011A5414;
		//dwBASE_NameHook = dwBASE_LOL + 0xC5A2C1;
		//dwBASE_GameTime = dwBASE_LOL + 0x2E368C4;
		//League of Legends.exe+C5A2C1
		dwBASE_MyObject = dwBASE_LOL + 0x11A5414;
		dwBASE_NameHook = dwBASE_LOL + 0xC5A2C1; 
		dwBASE_GameTime = dwBASE_LOL + 0x2E368C4;
		CHAR szPath[MAX_PATH] = { 0 };
	//DbgPrintA("[Hook]Code Init OK!");
	//DbgPrintA("[Hook]MyObjectBase:%08X",dwBASE_MyObject);
	//DbgPrintA("[Hook]NameHook:%08X",dwBASE_NameHook);
	//DbgPrintA("[Hook]GameTime:%08X",dwBASE_GameTime);
	DbgPrintA("[Hook]%s", GetCommandLineA());
	DbgPrintA("[Hook]=======================================");
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	DWORD dwSize = GetFileVersionInfoSize(szPath, NULL);
	LPBYTE pBlock = (BYTE*)malloc(dwSize);
	GetFileVersionInfoA(szPath, 0, dwSize, pBlock);
	char* pVerValue = NULL; UINT nlen1 = NULL;
	VerQueryValue(pBlock, "\\",(LPVOID*)&pVerValue,&nlen1);
	VS_FIXEDFILEINFO *pfixfileinfo = (VS_FIXEDFILEINFO *)pVerValue;
	if (HIWORD(pfixfileinfo->dwFileVersionMS) == 6 && LOWORD(pfixfileinfo->dwFileVersionMS) == 7 && HIWORD(pfixfileinfo->dwFileVersionLS) == 138 && LOWORD(pfixfileinfo->dwFileVersionLS) == 9658)
		HookHeroName();
	else
		DbgPrintA("[Hook]版本失效");
	}catch(...){}
	return TRUE;
}






float GetGameTime(){
float fRet;
try{
	__asm {
		mov eax, dwBASE_GameTime
		mov eax, [eax]
		mov eax, [eax + 0x2C]
		mov fRet,eax
	}
	return fRet;
}catch(...){}
}

float GetSkillCoolTime(DWORD dwObject,int SkillID){
	float fRet;
	try{
		__asm{
			pushad
			mov eax,dwObject
			mov ebx,SkillID
			lea eax,[eax+0x2450+0x518+ebx*4]
			mov eax,[eax]
			mov eax,[eax+0x14]
			mov fRet,eax
			popad
		}
		fRet=fRet-GetGameTime();
		if(fRet>=0){
			return fRet;
		}else{
			return 0;
		}
		
	}catch(...){}
}

void __stdcall gDrawText(DWORD Device,OBJECT_POSITION p,WCHAR* Text,DWORD Color){
	try {
		DWORD Param1, Param2;
		float x, y;
		Param1 = 0xFF;
		Param2 = Color;
		x = p.x;
		y = p.y;
		__asm {
				MOV ESI,Device
				MOVSS XMM1, y
				MOVSS XMM2, x
				PUSH 0x2
				MOV ECX, DWORD PTR DS : [ESI + 0x18]
				SUB ESP, 0x8
				MOV EAX, DWORD PTR DS : [ECX]
				MOV DWORD PTR SS : [ESP + 0x4], 0x3F800000
				MOV DWORD PTR SS : [ESP], 0x3F800000
				PUSH 0x3
				PUSH 0x1
				PUSH 0x0
				PUSH 0xFF
				PUSH 0x00
				PUSH Color
				PUSH Text
				SUB ESP, 0x0C
				MOVSS DWORD PTR SS : [ESP + 0x8], XMM1
				MOVSS DWORD PTR SS : [ESP + 0x4], XMM2
				MOV DWORD PTR SS : [ESP], 0x3F800000
				CALL DWORD PTR DS : [EAX + 0x14]
		}
	}
	catch (...) {
		DbgPrintA("[Hook] DrawText Error");
	}

}

LPVOID pHookHeroName;
MEMORYINFO MEM_INFO_HERONAME;
WCHAR* NEWName=L"GANK提示！请注意周围！";
BOOL bJmp = FALSE;
void __stdcall DrawHeroName(DWORD POS,DWORD NAME,DWORD DEVICE,DWORD HEROBASE){
	try {
		OBJECT_POSITION p,p2,p3;
		WCHAR tmp[25];
		DWORD clr;
		float SkillTime;
		//DWORD dwTmp;
		//for (int n = 0; n <= 100; n++) {
		//	dwTmp = *(DWORD*)(HEROBASE + 4 * n);
		//	//DbgPrintA("[Hook]%08X %08X", HEROBASE + 4 * n, dwTmp);
		//	if (dwTmp == GetMyObjectBase()) {
		//		DbgPrintA("[Hook]%08X", 4 * n);
		//	}
		//}
		bJmp = FALSE;
		if (IsBadReadPtr((void*)HEROBASE, 0x4)) return;
		if (*(DWORD*)(HEROBASE + 0x18) != 0x1401) return;
		bJmp = TRUE;
		p3.y = *(float*)POS;
		p3.x = *(float*)(POS + 0x4);
		gDrawText(DEVICE, p3, (WCHAR*)NAME, 0xFFFFFFFF);

		p.y= *(float*)POS + 25;
		p.x = *(float*)(POS + 0x4) - 85;
		wsprintfW(tmp, L"%d", (int)*(float*)(HEROBASE+0x154));
		//wsprintfW(tmp, L"%08X", HEROBASE);
		gDrawText(DEVICE, p, tmp, 0xFF00FF00);
		
		if (HEROBASE != GetMyObjectBase()) {
		//	if (GetObjectCamp(GetMyObjectBase()) == GetObjectCamp(HEROBASE)) {

				for (int n = 0; n < 6; n++) {
					p2.x = *(float*)(POS + 0x4) - 65;
					p2.y = *(float*)POS + 40;
					SkillTime = GetSkillCoolTime(HEROBASE, n);
					if (0 <= n && n <= 3) {
						DWORD clr = 0xFF00FF00;
						if (SkillTime > 0) {
							wsprintfW(tmp, L"%d", (int)(SkillTime + 1));
							clr = 0xFFFF0000;
						}
						else if (n == 0) wsprintfW(tmp, L"Q");
						else if (n == 1) wsprintfW(tmp, L"W");
						else if (n == 2) wsprintfW(tmp, L"E");
						else if (n == 3) wsprintfW(tmp, L"R");
						p2.x += (n + 1) * 25;
						gDrawText(DEVICE, p2, tmp, clr);
					}
					else {
						DWORD clr = 0xFF00FF00;
						if (SkillTime > 0) wsprintfW(tmp, L"%d", (int)(SkillTime + 1));
						else if (n == 4) wsprintfW(tmp, L"D");
						else if (n == 5)wsprintfW(tmp, L"F");
						if (SkillTime > 0) {
							clr = 0xFFFF0000;
							p2.x += 5;
						}
						p2.y += 15 * (n - 3) - 35;
						p2.x += 4 * 25 + 47;
						gDrawText(DEVICE, p2, tmp, clr);
					}
				}

			}
		
		//}
	}
	catch(...){

	}
}


void __declspec(naked) HeroNameProc(){
	__asm{
		push ebp
			lea ebp, [esp + 0x4]
			pushad
			pushfd
			mov eax, [ebp + 0x98]
			push eax  //HEROBASE
			push esi  //DEVICE
			push[ebp + 0x10]  //UNINAME
			lea eax, [ebp + 0x4]
			push eax //POS
			call DrawHeroName
			popfd
			popad
			pop ebp
			cmp bJmp, 1
			je Label
			JMP MEM_INFO_HERONAME.lpOriginCode
			Label:
			mov eax,pHookHeroName
			add eax,0x54
		    JMP eax
	}
}

void HookHeroName(){
	pHookHeroName=DetourFunc((BYTE*)dwBASE_NameHook,(BYTE*)&HeroNameProc,0x6,DETOUR_TYPE_JMP,&MEM_INFO_HERONAME);
}

void UnHookHeroName(){
	RetourFunc(MEM_INFO_HERONAME);
}


float GetObjectHP_Min(DWORD ObjectBase){
	float ret_f;
try{
	__asm{
		mov eax,ObjectBase
		mov eax,[eax+0x154]
		mov ret_f,eax
	}
	return ret_f;
}catch(...){
}
return 0.0f;
}

float GetObjectHP_Max(DWORD ObjectBase){
	float ret_f;
	try{
		__asm{
			mov eax,ObjectBase
				mov eax,[eax+0x164]
			mov ret_f,eax
		}
		return ret_f;
	}catch(...){
	}
	return 0.0f;
}



int GetObjectCamp(DWORD ObjectBase){
	int ret;
	try{
	ret= *(int*)(ObjectBase+0x14);
	}catch(...){}
	return ret;
}

char* GetAnsiName(DWORD ObjectBase){
	char* ret_char;
	DWORD dwAddr;
	try{
	dwAddr=*(DWORD*)(ObjectBase+0x24);
	if(dwAddr==0x656A624F){
		dwAddr=*(DWORD*)(ObjectBase+0x20);
		ret_char=(char*)dwAddr;
	}else{
		dwAddr=ObjectBase+0x20;
		ret_char=(char*)dwAddr;
	}
	}catch(...){}


	return ret_char;
}

char* GetHeroName(DWORD ObjectBase){
	char* ret_char;
	try{
	__asm{
		mov eax,ObjectBase
		mov eax,[eax+0x38]
		lea eax,[eax+0x1C]
		mov ret_char,eax
	}}catch(...){}

	return ret_char;
}
