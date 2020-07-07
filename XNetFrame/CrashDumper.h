///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __CRASH_DUMPER_H
#define __CRASH_DUMPER_H

#include "TypesDn.h"

#include "System.h"

namespace DnNetworkFramework{

typedef BOOL (*MINI_DUMP_WRITE_DUMP)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION,
									 PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);

class CCrashDumper{
public:
	enum LDumpLevel { DL_LEVEL0, DL_LEVEL1, DL_LEVEL2 };

public:
	static bool       m_bDumpDate;   // 
	static TSTRING    m_sDumpFile;   // 
	static LDumpLevel m_dlDumpLevel; // 

private:
	static LONG ExceptionHandler(struct __EXCEPTION_POINTERS*); // 

public:
	CCrashDumper(void);
	virtual ~CCrashDumper(void);

	static TSTRING getDumpDate(void);                              // 
	static void    setExceptionHandler(bool, TSTRING, LDumpLevel); // 
};

}

#endif
