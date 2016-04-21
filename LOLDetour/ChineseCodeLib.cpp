#include <windows.h> 
#pragma warning (disable:4996)

char* Utf82Unicode(const char* utf, size_t *unicode_number)  
{  
	if(!utf || !strlen(utf))  
	{  
		*unicode_number = 0;  
		return NULL;  
	}  
	int dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,utf,-1,NULL,0);  
	size_t num = dwUnicodeLen*sizeof(wchar_t);  
	wchar_t *pwText = (wchar_t*)malloc(num);  
	memset(pwText,0,num);  
	MultiByteToWideChar(CP_UTF8,0,utf,-1,pwText,dwUnicodeLen);  
	*unicode_number = dwUnicodeLen - 1;  
	return (char*)pwText;  
}  

char* Unicode2Utf8(const char* unicode)  
{  
	int len;  
	len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, NULL, 0, NULL, NULL);  
	char *szUtf8 = (char*)malloc(len + 1);  
	memset(szUtf8, 0, len + 1);  
	WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, szUtf8, len, NULL,NULL);  
	return szUtf8;  
}  