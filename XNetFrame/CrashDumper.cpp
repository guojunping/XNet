///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "CrashDumper.h"

namespace DnNetworkFramework{

CCrashDumper::CCrashDumper(void){
	m_bDumpDate   = false;
	m_sDumpFile   = _T("");
	m_dlDumpLevel = DL_LEVEL0;
}

CCrashDumper::~CCrashDumper(void){
}

void CCrashDumper::setExceptionHandler(bool bDumpDate, TSTRING sDumpFile, LDumpLevel dlDumpLevel){ // 
	m_bDumpDate   = bDumpDate;
	m_sDumpFile   = sDumpFile;
	m_dlDumpLevel = dlDumpLevel;

	if(m_sDumpFile.empty() == false){ // 
		::SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionHandler);
	};
}

TSTRING CCrashDumper::getDumpDate(void){ // 
	TCHAR      szDumpDate[24];
	TSTRING    sDumpDate;
	time_t     tDumpDate;
	struct tm* tmLocal;

	time(&tDumpDate);
	
	tmLocal = localtime(&tDumpDate);
	_tcsftime(szDumpDate, 24, _T("_%Y%m%d_%H%M%S"), tmLocal);

	sDumpDate = szDumpDate;

	return sDumpDate;
}

LONG CCrashDumper::ExceptionHandler(struct __EXCEPTION_POINTERS* pExceptionPointers){ // 
	_MINIDUMP_EXCEPTION_INFORMATION eiExceptionInformation;
	TSTRING                         sDirectory, sDllFile;
	MINI_DUMP_WRITE_DUMP            pMiniDumpWriteDump;
	HANDLE                          hDumpFile;
	HMODULE                         hDllFile;
	LONG                            lResult;

	lResult = EXCEPTION_CONTINUE_SEARCH;

	if(m_sDumpFile.empty() == true) return lResult; // 

	sDirectory  = CSystem::getCurrentDirectory();
	sDllFile    = sDirectory + _T("\\DBGHELP.DLL");
	m_sDumpFile = sDirectory + _T("\\") + m_sDumpFile;

	if(m_bDumpDate == true){ // 
		m_sDumpFile = m_sDumpFile + getDumpDate();
	};

	hDllFile = ::LoadLibrary(sDllFile.c_str()); // 
	if(hDllFile == NULL) return lResult;
	
	pMiniDumpWriteDump = (MINI_DUMP_WRITE_DUMP)::GetProcAddress(hDllFile, "MiniDumpWriteDump"); // 
	if(pMiniDumpWriteDump == NULL) return lResult;

	hDumpFile = ::CreateFile(m_sDumpFile.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // 
	if(hDumpFile == INVALID_HANDLE_VALUE) return lResult;

	lResult = EXCEPTION_EXECUTE_HANDLER;

	eiExceptionInformation.ClientPointers    = FALSE;
	eiExceptionInformation.ThreadId          = ::GetCurrentThreadId();
	eiExceptionInformation.ExceptionPointers = (PEXCEPTION_POINTERS)pExceptionPointers;

	switch(m_dlDumpLevel){ // 
		case DL_LEVEL0 : pMiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &eiExceptionInformation, NULL, NULL);         break;
		case DL_LEVEL1 : pMiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hDumpFile, MiniDumpWithDataSegs, &eiExceptionInformation, NULL, NULL);   break;
		default        : pMiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hDumpFile, MiniDumpWithFullMemory, &eiExceptionInformation, NULL, NULL); break;
	};

	::CloseHandle(hDumpFile); // 

	return lResult;
}

}
