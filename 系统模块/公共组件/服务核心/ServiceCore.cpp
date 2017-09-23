#include "Stdafx.h"
#include "AfxDllx.h"
#include "ServiceCoreHead.h"
#pragma once
#include <string>
#include <DbgHelp.h>
#include <string>
#pragma comment(lib,  "dbghelp.lib")


using namespace std;
class CCreateDump
{
public:
	CCreateDump();
	~CCreateDump(void);
	static CCreateDump* Instance();
	static long __stdcall UnhandleExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);
	//声明Dump文件，异常时会自动生成。会自动加入.dmp文件名后缀
	void DeclarDumpFile(std::string dmpFileName = "");
private:
	static std::string    strDumpFile; 
	static CCreateDump*    __instance;
};


CCreateDump* CCreateDump::__instance = NULL;
std::string CCreateDump::strDumpFile = "";

CCreateDump::CCreateDump()
{
}

CCreateDump::~CCreateDump(void)
{

}

long  CCreateDump::UnhandleExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
	HANDLE hFile   =   CreateFileA(strDumpFile.c_str(),   GENERIC_WRITE,   FILE_SHARE_WRITE,   NULL,   CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,   NULL   );
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION   ExInfo; 
		ExInfo.ThreadId   =   ::GetCurrentThreadId();
		ExInfo.ExceptionPointers   =   ExceptionInfo;
		ExInfo.ClientPointers   =   FALSE;
		//   write   the   dump
		BOOL   bOK   =   MiniDumpWriteDump(GetCurrentProcess(),   GetCurrentProcessId(),   hFile,   MiniDumpNormal,  &ExInfo,   NULL,   NULL   );
		CloseHandle(hFile);
		if (!bOK)
		{
			DWORD dw = GetLastError();
			//写dump文件出错处理,异常交给windows处理
			return EXCEPTION_CONTINUE_SEARCH;
		}
		else
		{    //在异常处结束
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}
	else
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

void CCreateDump::DeclarDumpFile(std::string dmpFileName)
{
	SYSTEMTIME syt;
	GetLocalTime(&syt);
	char c[MAX_PATH];
	sprintf_s(c,MAX_PATH,"[%04d-%02d-%02d %02d：%02d：%02d]",syt.wYear,syt.wMonth,syt.wDay,syt.wHour,syt.wMinute,syt.wSecond);
	strDumpFile = std::string(c);
	if (!dmpFileName.empty())
	{
		strDumpFile += dmpFileName;
	}
	strDumpFile += std::string(".dmp");
	SetUnhandledExceptionFilter(UnhandleExceptionFilter);
}

CCreateDump* CCreateDump::Instance()
{
	if (__instance == NULL)
	{
		__instance = new CCreateDump;
	}
	return __instance;
}
void DisableSetUnhandledExceptionFilter()
{
	void* addr = (void*)GetProcAddress(LoadLibraryA("kernel32.dll"),
		"SetUnhandledExceptionFilter");

	if (addr) 
	{
		unsigned char code[16];
		int size = 0;

		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;

		DWORD dwOldFlag, dwTempFlag;
		VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
	}
}
//////////////////////////////////////////////////////////////////////////////////

//静态变量
static AFX_EXTENSION_MODULE ServiceCoreDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//导出函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	CCreateDump::Instance()->DeclarDumpFile("dumpfile");
	DisableSetUnhandledExceptionFilter();
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(ServiceCoreDLL, hInstance)) return 0;
		new CDynLinkLibrary(ServiceCoreDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(ServiceCoreDLL);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
