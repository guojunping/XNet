#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "TypesDn.h"

#include "InetAddress.h"

namespace XNetFrame{

class CSystem{
public:
	enum ESystemResult { SR_INVALID_EXE_FORMAT, SR_FILE_NOT_FOUND, SR_OUT_OF_MEMORY, SR_UNKNOWN_ERROR, SR_OK };

public:
	CSystem(void);
	~CSystem(void);
	
#ifdef WIN32
	static bool    hasMMX(void);   // 
	static bool    hasCPUID(void); // 

	static uint32  getMemoryLoad(void);          // 
	static uint32  getFreeVirtualMemory(void);   // 
	static uint32  getTotalVirtualMemory(void);  // 
	static uint32  getFreePhysicalMemory(void);  // 
	static uint32  getTotalPhysicalMemory(void); // 

	static TSTRING getProcessor(void);       // 
	static uint32  getProcessorCount(void);  // 
	static TSTRING getOperatingSystem(void); // 

	static bool    isNT_Series(void); // 

	static void    createDirectory(TSTRING);  // 
	static TSTRING getCurrentDirectory(void); // 

	static uint64  getFreeDiskSpace(LPTSTR szDirectoryName = NULL);  // 
	static uint64  getTotalDiskSpace(LPTSTR szDirectoryName = NULL); // 

	static ESystemResult execCommand(TSTRING&, TSTRING&); //
	static string getFirstInvalidIpAddress(void);
	static CInetAddress getBindIpAddress(SOCKET);
#else

static TSTRING getCurrentDirectory(void); // 

static ESystemResult getWorkHomeDirectory(TSTRING & ); // 

static uint32  getProcessorCount(void);  // 

#endif

};

}

#endif
