
#include <windows.h>  
#include "GameProc.h"
#include <iostream>  
#include <tlhelp32.h>
#pragma warning (disable:4996)


BOOL ResumeOtherThread (){
	THREADENTRY32 te32={sizeof(THREADENTRY32)};
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,GetCurrentProcessId());
	HANDLE mThread=GetCurrentThread();
	DWORD mThreadID=GetCurrentThreadId();
	HANDLE hThread;
	if ( hThreadSnap == INVALID_HANDLE_VALUE )
		return FALSE ;
	if ( Thread32First ( hThreadSnap, &te32 ) )
	{
		do{
			if(te32.th32ThreadID != mThreadID){
				hThread=OpenThread(THREAD_SUSPEND_RESUME,FALSE,te32.th32ThreadID);
				if (hThread)
				{
					ResumeThread(hThread);
					CloseHandle(hThread);
				}
			}
		}while (Thread32Next ( hThreadSnap, &te32 ) ) ;
	}
	CloseHandle ( hThreadSnap ) ;
	return TRUE ;
}

BOOL SuspendOtherThread ()
{
	THREADENTRY32 te32={sizeof(THREADENTRY32)};
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,GetCurrentProcessId());
	HANDLE mThread=GetCurrentThread();
	DWORD mThreadID=GetCurrentThreadId();
	HANDLE hThread;
	if ( hThreadSnap == INVALID_HANDLE_VALUE )
		return FALSE ;
	if ( Thread32First ( hThreadSnap, &te32 ) )
	{
		do{
			if(te32.th32ThreadID != mThreadID){
				hThread=OpenThread(THREAD_SUSPEND_RESUME,FALSE,te32.th32ThreadID);
				if (hThread)
				{
					SuspendThread(hThread);
					CloseHandle(hThread);
				}
			}
		}while (Thread32Next ( hThreadSnap, &te32 ) ) ;
	}
	CloseHandle ( hThreadSnap ) ;
	return TRUE ;
}



DWORD MakeLParam(UINT Key,UINT State){
	BYTE* pByte;
	DWORD ret;
	pByte=(BYTE*)&ret;
	*(WORD*)pByte=0x0001;
	pByte+=2;
	*pByte=MapVirtualKey(Key,0);
	pByte++;
	if(State==WM_KEYDOWN){
		*pByte=0x00;
	}else{
		*pByte=0xC0;
	}
	return (ret);
}

void *memcpy_m(void *dest, const void *src, int n)
{
	byte *c_dest = (byte *)dest;
	const byte *c_src = (const byte *)src;
	while (n-- > 0)
		*c_dest++ = *c_src++;
	return((void *) dest);
}

LPVOID DetourFunc(BYTE *src, const BYTE *dst, int len,int TYPE,PMEMORYINFO lpMemInfo)
{
	if(lpMemInfo){
		lpMemInfo->dwAddr=(DWORD)src;
		lpMemInfo->len=len;
		lpMemInfo->lpOriginCode=(PBYTE)malloc(len+0x5);
		memcpy_m(lpMemInfo->lpOriginCode,src,len);
		lpMemInfo->IsHook=TRUE;
	}
	DWORD lpOld;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &lpOld);
	switch(TYPE){
	case DETOUR_TYPE_JMP:
		src[0] = 0xE9;
		break;
	case DETOUR_TYPE_CALL:
		src[0] = 0xE8;
		break;
	}
	*(DWORD*)(src+1) = (DWORD)(dst - src) - 5;
	BYTE* Addr=src+5;
	BYTE* dstination=src+len;

	BYTE* pCode=lpMemInfo->lpOriginCode+len;
	*pCode=0xE9;
	*(DWORD*)(pCode+0x1) = (DWORD)(dstination - pCode) - 5;

	while (Addr!=dstination)
	{
		*Addr=0x90;
		Addr++;
	}
	VirtualProtect(src, len, lpOld, &lpOld);
	VirtualProtect((LPVOID)lpMemInfo->lpOriginCode,len+0x5,PAGE_EXECUTE_READWRITE,&lpOld);
	return (src+len);
}
void RetourFunc (MEMORYINFO MemInfo){
	DWORD dwAddr;
	DWORD lpOld;
	BYTE* pCode;
	int len;

	if(!MemInfo.IsHook) return;
	if(!MemInfo.dwAddr) return;
	if(!MemInfo.len) return;
	if(!MemInfo.lpOriginCode) return;

	try{
		len=MemInfo.len;
		pCode=MemInfo.lpOriginCode;
		dwAddr=MemInfo.dwAddr;
		VirtualProtect((BYTE*)dwAddr, len, PAGE_EXECUTE_READWRITE, &lpOld);
		memcpy_m((void*)dwAddr,pCode,len);
		VirtualProtect((BYTE*)dwAddr, len, lpOld, &lpOld);
		free(pCode);
	}catch(...){}
	MemInfo.IsHook=FALSE;
}

void _stdcall DbgPrintW(const WCHAR *format, ...){
	WCHAR buf[4096], *p = buf;	
	va_list args;	
	va_start(args, format);	
	p += _vsnwprintf(p, sizeof buf - 1, format, args);	
	va_end(args);	
	OutputDebugStringW((LPCWSTR)buf);
}

void _stdcall DbgPrintA(const char *format, ...){
	char buf[4096], *p = buf;	va_list args;
	va_start(args, format);
	p += _vsnprintf(p, sizeof buf - 1, format, args);
	va_end(args);
	OutputDebugStringA(buf);
}

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;

typedef struct _LDR_MODULE {
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID BaseAddress;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	SHORT LoadCount;
	SHORT TlsIndex;
	LIST_ENTRY HashTableEntry;
	ULONG TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;

typedef struct _PEB_LDR_DATA
{
	ULONG               Length;
	BOOLEAN             Initialized;
	BYTE        reserved[3];
	PVOID               SsHandle;
	LIST_ENTRY          InLoadOrderModuleList;
	LIST_ENTRY          InMemoryOrderModuleList;
	LIST_ENTRY          InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _NT_PEB 
{
	BOOLEAN InheritedAddressSpace;
	BOOLEAN ReadImageFileExecOptions;
	BOOLEAN BeingDebugged;
	BOOLEAN Spare;
	HANDLE Mutant;
	PVOID ImageBaseAddress;
	PPEB_LDR_DATA LoaderData;
	PVOID ProcessParameters;//PRTL_USER_PROCESS_PARAMETERS
	PVOID SubSystemData;
	PVOID ProcessHeap;
	PVOID FastPebLock;
	PVOID FastPebLockRoutine;//PPEBLOCKROUTINE
	PVOID FastPebUnlockRoutine;//PPEBLOCKROUTINE
	ULONG EnvironmentUpdateCount;
	PVOID *KernelCallbackTable;
	PVOID EventLogSection;
	PVOID EventLog;
	PVOID FreeList;//PPEB_FREE_BLOCK
	ULONG TlsExpansionCounter;
	PVOID TlsBitmap;
	ULONG TlsBitmapBits[0x2];
	PVOID ReadOnlySharedMemoryBase;
	PVOID ReadOnlySharedMemoryHeap;
	PVOID *ReadOnlyStaticServerData;
	PVOID AnsiCodePageData;
	PVOID OemCodePageData;
	PVOID UnicodeCaseTableData;
	ULONG NumberOfProcessors;
	ULONG NtGlobalFlag;
	BYTE Spare2[0x4];
	LARGE_INTEGER CriticalSectionTimeout;
	ULONG HeapSegmentReserve;
	ULONG HeapSegmentCommit;
	ULONG HeapDeCommitTotalFreeThreshold;
	ULONG HeapDeCommitFreeBlockThreshold;
	ULONG NumberOfHeaps;
	ULONG MaximumNumberOfHeaps;
	PVOID **ProcessHeaps;
	PVOID GdiSharedHandleTable;
	PVOID ProcessStarterHelper;
	PVOID GdiDCAttributeList;
	PVOID LoaderLock;
	ULONG OSMajorVersion;
	ULONG OSMinorVersion;
	ULONG OSBuildNumber;
	ULONG OSPlatformId;
	ULONG ImageSubSystem;
	ULONG ImageSubSystemMajorVersion;
	ULONG ImageSubSystemMinorVersion;
	ULONG GdiHandleBuffer[0x22];
	ULONG PostProcessInitRoutine;
	ULONG TlsExpansionBitmap;
	BYTE TlsExpansionBitmapBits[0x80];
	ULONG SessionId;
}NT_PEB, *pNT_PEB;

typedef struct _NT_TEB
{
	NT_TIB Tib;                         /* 00h */
	PVOID EnvironmentPointer;           /* 1Ch */
	//  CLIENT_ID Cid;                      /* 20h */
	BYTE Reserved1[8];				 //占用8个字节
	PVOID ActiveRpcInfo;                /* 28h */
	PVOID ThreadLocalStoragePointer;    /* 2Ch */
	pNT_PEB Peb;                       /* 30h */
	ULONG LastErrorValue;               /* 34h */
	ULONG CountOfOwnedCriticalSections; /* 38h */
	PVOID CsrClientThread;              /* 3Ch */
	void* Win32ThreadInfo;        /* 40h */
	ULONG Win32ClientInfo[0x1F];        /* 44h */
	PVOID WOW32Reserved;                /* C0h */
	LCID CurrentLocale;                 /* C4h */
	ULONG FpSoftwareStatusRegister;     /* C8h */
	PVOID SystemReserved1[0x36];        /* CCh */
	PVOID Spare1;                       /* 1A4h */
	LONG ExceptionCode;                 /* 1A8h */
	UCHAR SpareBytes1[0x28];            /* 1ACh */
	PVOID SystemReserved2[0xA];         /* 1D4h */
	//  GDI_TEB_BATCH GdiTebBatch;          /* 1FCh */
	BYTE Reserved3[0x4e0];				 //占用8个字节
	ULONG gdiRgn;                       /* 6DCh */
	ULONG gdiPen;                       /* 6E0h */
	ULONG gdiBrush;                     /* 6E4h */
	// CLIENT_ID RealClientId;             /* 6E8h */
	BYTE Reserved2[8];				 //占用8个字节
	PVOID GdiCachedProcessHandle;       /* 6F0h */
	ULONG GdiClientPID;                 /* 6F4h */
	ULONG GdiClientTID;                 /* 6F8h */
	PVOID GdiThreadLocaleInfo;          /* 6FCh */
	PVOID UserReserved[5];              /* 700h */
	PVOID glDispatchTable[0x118];       /* 714h */
	ULONG glReserved1[0x1A];            /* B74h */
	PVOID glReserved2;                  /* BDCh */
	PVOID glSectionInfo;                /* BE0h */
	PVOID glSection;                    /* BE4h */
	PVOID glTable;                      /* BE8h */
	PVOID glCurrentRC;                  /* BECh */
	PVOID glContext;                    /* BF0h */
	LONG LastStatusValue;        /* BF4h */
	UNICODE_STRING StaticUnicodeString; /* BF8h */
	WCHAR StaticUnicodeBuffer[0x105];   /* C00h */
	PVOID DeallocationStack;            /* E0Ch */
	PVOID TlsSlots[0x40];               /* E10h */
	LIST_ENTRY TlsLinks;                /* F10h */
	PVOID Vdm;                          /* F18h */
	PVOID ReservedForNtRpc;             /* F1Ch */
	PVOID DbgSsReserved[0x2];           /* F20h */
	ULONG HardErrorDisabled;            /* F28h */
	PVOID Instrumentation[0x10];        /* F2Ch */
	PVOID WinSockData;                  /* F6Ch */
	ULONG GdiBatchCount;                /* F70h */
	USHORT Spare2;                      /* F74h */
	BOOLEAN IsFiber;                    /* F76h */
	UCHAR Spare3;                       /* F77h */
	ULONG Spare4;                       /* F78h */
	ULONG Spare5;                       /* F7Ch */
	PVOID ReservedForOle;               /* F80h */
	ULONG WaitingOnLoaderLock;          /* F84h */
	ULONG Unknown[11];                  /* F88h */
	PVOID FlsSlots;                     /* FB4h */
	PVOID WineDebugInfo;                /* Needed for WINE DLL's  */
}NT_TEB, *pNT_TEB;

NT_TEB*  GetCurrentThreadTEB()
{
	NT_TEB* pTeb=NULL;

	_asm
	{
		mov    eax,fs:[0x18]	//获取本进程的主线程的TEB
		mov    pTeb,eax
	}
	return pTeb;
}

BOOL HideModuleByHandle(HMODULE hModule)
{
	NT_TEB* pTeb = GetCurrentThreadTEB();
	NT_PEB* pPeb = pTeb->Peb;
	PPEB_LDR_DATA pLdrData = pPeb->LoaderData;
	PLDR_MODULE  ListHead = (PLDR_MODULE)(&(pLdrData->InLoadOrderModuleList));
	PLDR_MODULE pFirstLdrModule = (PLDR_MODULE)pLdrData->InLoadOrderModuleList.Flink; 
	PLDR_MODULE pLdrModule = pFirstLdrModule;
	//   PLDR_MODULE pLastModule,pNextModule;

	//LoadOrderModuleList
	while(pLdrModule != ListHead)
	{
		//判断是否是我们要屏蔽的模块
		if(pLdrModule->BaseAddress == hModule)
		{
			pLdrModule->InLoadOrderModuleList.Flink->Blink = pLdrModule->InLoadOrderModuleList.Blink;
			pLdrModule->InLoadOrderModuleList.Blink->Flink = pLdrModule->InLoadOrderModuleList.Flink;

			pLdrModule->InMemoryOrderModuleList.Flink->Blink = pLdrModule->InMemoryOrderModuleList.Blink;
			pLdrModule->InMemoryOrderModuleList.Blink->Flink = pLdrModule->InMemoryOrderModuleList.Flink;

			pLdrModule->InInitializationOrderModuleList.Flink->Blink = pLdrModule->InInitializationOrderModuleList.Blink;
			pLdrModule->InInitializationOrderModuleList.Blink->Flink = pLdrModule->InInitializationOrderModuleList.Flink;
		}

		//移动到链表的下一个
		pLdrModule = (PLDR_MODULE)pLdrModule->InLoadOrderModuleList.Flink;

	} 

	return TRUE;
}


BOOL bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	try
	{
		for(;*szMask;++szMask,++pData,++bMask)
			if(*szMask=='x' && *pData!=*bMask)   return 0;
		return (*szMask) == NULL;
	}
	catch (...)
	{
		DbgPrintA("[Hook]Compare Error!");
	}
	return FALSE;
}

DWORD FindPattern(DWORD dwdwAdd,DWORD dwLen,BYTE *bMask,char * szMask)
{
	try{
		for(DWORD i=0; i<dwLen; i++)
			if (bCompare((BYTE*)(dwdwAdd+i),bMask,szMask))  return (DWORD)(dwdwAdd+i);
		return 0;

	}catch(...){
		DbgPrintA("[Hook]Find Pattern Error");
	}
	return 0;
}

DWORD GetAddress(DWORD* Addr){
	DWORD dwRet;
	dwRet=(DWORD)Addr+*Addr+0x4;
	return dwRet;
}

DWORD SearchSignature(PBYTE code,PCHAR szMask,DWORD dwStartAddress,int ID){
	DWORD dwResult=0;
	DWORD lpAddress=dwStartAddress;
	DWORD dwBestResult=0;
	MEMORY_BASIC_INFORMATION lpBuffer;

	while(VirtualQueryEx((HANDLE)-1,(LPCVOID)lpAddress,&lpBuffer,sizeof(lpBuffer)) != 0)
	{
		if(lpBuffer.Protect == PAGE_EXECUTE_READ || lpBuffer.Protect ==PAGE_EXECUTE_WRITECOPY)
		{
			dwResult = FindPattern((DWORD)lpBuffer.BaseAddress,lpBuffer.RegionSize,code,szMask);
			if(dwResult > dwBestResult)	{
				dwBestResult = dwResult;
				break;
			}
		}
		lpAddress = lpAddress + lpBuffer.RegionSize;
	}

	if(dwBestResult==0){
		DbgPrintA("[Hook]Search Error id:%d",ID);		
		return 0;
	}

	return dwBestResult;
}

wstring ANSIToUnicode(const string& str)
{
	int  len = 0;
	len = str.length();
	int  unicodeLen = ::MultiByteToWideChar( CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0 );  
	wchar_t *  pUnicode;  
	pUnicode = new  wchar_t[unicodeLen+1];  
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  
	::MultiByteToWideChar( CP_ACP,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen );  
	wstring  rt;  
	rt = ( wchar_t* )pUnicode;
	delete  pUnicode; 

	return  rt;  
}