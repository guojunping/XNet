///////////////////////////////////////
// Author : Mr Kim
// Name : CSystem is class to check the hardware and OS spec
///////////////////////////////////////

#include "System.h"

namespace XNetFrame{

CSystem::CSystem(void){
}

CSystem::~CSystem(void){
}

#ifdef WIN32

bool CSystem::hasMMX(void){	 // MultiMedia eXtensions
	uint32 uiResult = 0;

	if(CSystem::hasCPUID() == false) return false;

	__asm{
		mov  eax,1
		cpuid
		test edx,0x800000
		je   noMMX
		mov  uiResult, 1	
	noMMX:
	};

	return (uiResult == 1)? true : false;
}

bool CSystem::hasCPUID(void){// CPUID
	uint32 uiResult = 0;

	 __asm{
		pushad
		pushfd
		pushfd
		pop  eax
		mov  ecx,eax
		xor  eax,0x200000
		push eax
		popfd
		pushfd
		pop  eax
		xor  eax,ecx
		je   noCpuid
		
		popfd
		popad
		mov  uiResult, 1
		jmp  CPUIDPresent
		
	noCpuid:
		popfd
		popad
		mov uiResult, 0
	CPUIDPresent:
	};

	return (uiResult == 1)? true : false;
}

TSTRING CSystem::getOperatingSystem(void){ // 
	TCHAR           szProductType[80], szVersion[512];
	OSVERSIONINFOEX oviOsVersionInfo;
	DWORD           dwBufferLen = 80;
	TSTRING         sOS, sVersion;
	HKEY            hKey;

	sOS = _T("Unknown");

	::ZeroMemory(&oviOsVersionInfo, sizeof(OSVERSIONINFOEX));
	oviOsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if(::GetVersionEx((OSVERSIONINFO*)&oviOsVersionInfo) == false){
		oviOsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		if(::GetVersionEx((OSVERSIONINFO*)&oviOsVersionInfo) == false) return sOS;
	};

	switch (oviOsVersionInfo.dwPlatformId){
		case VER_PLATFORM_WIN32_NT : // 
			if(oviOsVersionInfo.dwMajorVersion <= 4) sOS = _T("Microsoft Windows NT ");
			else{
				if(oviOsVersionInfo.dwMinorVersion == 0) sOS = _T("Microsoft Windows 2000 ");
				else sOS = _T("Microsoft Windows XP or .NET");
			};

			::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), 0, KEY_QUERY_VALUE, &hKey);
			::RegQueryValueEx(hKey, _T("ProductType"), NULL, NULL, (LPBYTE)szProductType, &dwBufferLen);
			::RegCloseKey(hKey);

			if(lstrcmpi(_T("WINNT"), szProductType) == 0) sOS += _T("Workstation ");
			if(lstrcmpi(_T("SERVERNT"), szProductType) == 0) sOS += _T("Server ");

			_stprintf(szVersion, _T("version %d.%d '%s' (Build %d)"), oviOsVersionInfo.dwMajorVersion, oviOsVersionInfo.dwMinorVersion,
				oviOsVersionInfo.szCSDVersion, oviOsVersionInfo.dwBuildNumber);
			sVersion =  szVersion;
			sOS      += sVersion;
			break;

		case VER_PLATFORM_WIN32_WINDOWS : // 
			if(oviOsVersionInfo.dwMinorVersion == 0) sOS = _T("Microsoft Windows 95 ");
			else if(oviOsVersionInfo.dwMinorVersion == 10) sOS = _T("Microsoft Windows 98 ");
			else if(oviOsVersionInfo.dwMinorVersion == 90) sOS = _T("Microsoft Windows Me ");

			_stprintf(szVersion, _T("version %d.%d '%s' (Build %d)"), oviOsVersionInfo.dwMajorVersion, oviOsVersionInfo.dwMinorVersion,
				oviOsVersionInfo.szCSDVersion, oviOsVersionInfo.dwBuildNumber & 0xFFFF);
			sVersion =  szVersion;
			sOS      += sVersion;
			break;

		case VER_PLATFORM_WIN32s : // 
			sOS = _T("Microsoft Windows 3.1");
			break;
	};

	return sOS;
}

TSTRING CSystem::getProcessor(void){ // 
	TCHAR   szValue[1024];
	DWORD   dwBufferLen;
	TSTRING sProcessor;
	LONG    lResult;
	HKEY    hKey;

	sProcessor = _T("Unknown");

	lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 0, KEY_QUERY_VALUE, &hKey);
	if(lResult == ERROR_SUCCESS){
		dwBufferLen = 1024;

		lResult = ::RegQueryValueEx(hKey, _T("ProcessorNameString"), NULL, NULL, (LPBYTE)&szValue, &dwBufferLen);
		if(lResult == ERROR_SUCCESS) sProcessor = szValue;
		else sProcessor = _T("UnknownProcessor");

		sProcessor += _T(" / ");

		dwBufferLen = 1024;

		lResult = ::RegQueryValueEx(hKey, _T("Identifier"), NULL, NULL, (LPBYTE)&szValue, &dwBufferLen);
		if(lResult == ERROR_SUCCESS) sProcessor += szValue;
		else sProcessor += _T("UnknownIdentifier");

		sProcessor += _T(" / ");

		dwBufferLen = 1024;

		lResult = ::RegQueryValueEx(hKey, _T("VendorIdentifier"), NULL, NULL, (LPBYTE)&szValue, &dwBufferLen);
		if(lResult == ERROR_SUCCESS) sProcessor += szValue;
		else sProcessor += _T("UnknownVendor");
	};

	::RegCloseKey(hKey);

	return sProcessor;
}

uint32 CSystem::getProcessorCount(void){ //  ¸®ÅÏ
	SYSTEM_INFO m_siSystemInfo;

	::GetSystemInfo(&m_siSystemInfo);

	return (uint32)m_siSystemInfo.dwNumberOfProcessors;
}

bool CSystem::isNT_Series(void){ // 
	OSVERSIONINFOEX oviOsVersionInfo;

	::ZeroMemory(&oviOsVersionInfo, sizeof(OSVERSIONINFOEX));
	oviOsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if(::GetVersionEx((OSVERSIONINFO*)&oviOsVersionInfo) == false){
		oviOsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		if(::GetVersionEx((OSVERSIONINFO*)&oviOsVersionInfo) == false) return false;
	};

	switch (oviOsVersionInfo.dwPlatformId){
		case VER_PLATFORM_WIN32_NT : // 
			return true;
		case VER_PLATFORM_WIN32_WINDOWS : // 
			return false;
		case VER_PLATFORM_WIN32s : // 
			return false;
	};

	return false;
}

TSTRING CSystem::getCurrentDirectory(void){ // 
	TSTRING sFileName, sDirectory;
	TCHAR   szFileName[1024 + 1];
	sint32  siStop;

	::GetModuleFileName(NULL, szFileName, 1024);
	
	sFileName  = szFileName;
	siStop     = (sint32)sFileName.find_last_of(_T("/"));
	sDirectory = sFileName.substr(0, siStop);

	return sDirectory;
}

CSystem::ESystemResult CSystem::execCommand(TSTRING& sCommand, TSTRING& sParameter){ // 
	sint32 siResult;
	
	siResult = (sint32)::ShellExecute(NULL, _T("open"), sCommand.c_str(), sParameter.c_str(), NULL, SW_SHOWNORMAL);
	if(siResult > 32) return SR_OK;
	
	switch(siResult){
		case SE_ERR_OOM           : return SR_OUT_OF_MEMORY;
		case ERROR_PATH_NOT_FOUND :
		case ERROR_FILE_NOT_FOUND : return SR_FILE_NOT_FOUND;
		case ERROR_BAD_FORMAT     : return SR_INVALID_EXE_FORMAT;
		default                   : return SR_UNKNOWN_ERROR;
	};
}

uint64 CSystem::getTotalDiskSpace(LPTSTR szDirectoryName){ // 
	ULARGE_INTEGER uiFreeBytesAvailable, uiTotalNumberOfBytes;

	::GetDiskFreeSpaceEx(szDirectoryName, &uiFreeBytesAvailable, &uiTotalNumberOfBytes, NULL);

	return (uint64)uiTotalNumberOfBytes.QuadPart;
}

uint64 CSystem::getFreeDiskSpace(LPTSTR szDirectoryName){ // 
	ULARGE_INTEGER uiFreeBytesAvailable, uiTotalNumberOfBytes;

	::GetDiskFreeSpaceEx(szDirectoryName, &uiFreeBytesAvailable, &uiTotalNumberOfBytes, NULL);

	return (uint64)uiFreeBytesAvailable.QuadPart;
}

uint32 CSystem::getFreePhysicalMemory(void){ // 
	MEMORYSTATUS msMemoryStatus;

	msMemoryStatus.dwLength = sizeof(MEMORYSTATUS);

	::GlobalMemoryStatus(&msMemoryStatus);
	return (uint32)msMemoryStatus.dwAvailPhys;
}

uint32 CSystem::getTotalPhysicalMemory(void){ // 
	MEMORYSTATUS msMemoryStatus;

	msMemoryStatus.dwLength = sizeof(MEMORYSTATUS);

	::GlobalMemoryStatus(&msMemoryStatus);
	return (uint32)msMemoryStatus.dwTotalPhys;
}

uint32 CSystem::getFreeVirtualMemory(void){ // 
	MEMORYSTATUS msMemoryStatus;

	msMemoryStatus.dwLength = sizeof(MEMORYSTATUS);

	::GlobalMemoryStatus(&msMemoryStatus);
	return (uint32)msMemoryStatus.dwAvailVirtual;
}

uint32 CSystem::getTotalVirtualMemory(void){ // 
	MEMORYSTATUS msMemoryStatus;

	msMemoryStatus.dwLength = sizeof(MEMORYSTATUS);

	::GlobalMemoryStatus(&msMemoryStatus);
	return (uint32)msMemoryStatus.dwTotalVirtual;
}

uint32 CSystem::getMemoryLoad(void){ // 
	MEMORYSTATUS msMemoryStatus;

	msMemoryStatus.dwLength = sizeof(MEMORYSTATUS);

	::GlobalMemoryStatus(&msMemoryStatus);
	return (uint32)msMemoryStatus.dwMemoryLoad;
}

void CSystem::createDirectory(TSTRING sDirectory){ // 
	TCHAR  szDirectory[1024];
	TCHAR* pDirectory1;
	TCHAR* pDirectory2;
	
	pDirectory2 = szDirectory;
	pDirectory1 = (TCHAR*)sDirectory.c_str();

	while(*pDirectory1){
		if((*pDirectory1 == '\\') || (*pDirectory1 == '/')){
			if(*(pDirectory1 - 1) != ':'){
				::CreateDirectory(szDirectory, NULL);
			};
		};

		*pDirectory2++ = *pDirectory1++;
		*pDirectory2   = '\0';
	};

	::CreateDirectory(szDirectory, NULL);
}

string CSystem::getFirstInvalidIpAddress(void){
	char host_name[MAX_PATH];

	if(::gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR){
		return "";
	}

	struct hostent* phe = ::gethostbyname(host_name);
	if(phe == NULL || phe->h_addr_list[0] == NULL) return "";

	struct in_addr addr;
	memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));

	return ::inet_ntoa(addr);
}

CInetAddress CSystem::getBindIpAddress(SOCKET siSocketId){
	CInetAddress iaInetAddress;
	sockaddr_in   local;   
	int addrLen = sizeof(sockaddr_in);   
	memset(&local, 0, addrLen);

	int ret = ::getsockname(siSocketId, (sockaddr*)(&local), &addrLen);
	if(ret == SOCKET_ERROR){
		::closesocket(siSocketId);
		return iaInetAddress;
	} 

	iaInetAddress.setSocketAddress(&local);

	return iaInetAddress;
}

#else
TSTRING CSystem::getCurrentDirectory(void){ // 
	
	TSTRING sWorkHome;

	if(getWorkHomeDirectory(sWorkHome)==SR_OK)
	{
		return sWorkHome;
	}
	
	char szCurDirectory[PATH_MAX];
	getcwd(szCurDirectory,PATH_MAX);

	return szCurDirectory;
}

 CSystem::ESystemResult  CSystem::getWorkHomeDirectory(TSTRING & sWorkHome)
{
	char *pEnvPath= getenv("GAME_HOME");
	if(pEnvPath==NULL)
		return SR_UNKNOWN_ERROR;
		
	sWorkHome=pEnvPath;
	return SR_OK;
}

uint32   CSystem::getProcessorCount(void)
{
	uint32 uiCpuCount=sysconf(_SC_NPROCESSORS_ONLN);
	return uiCpuCount;
}

#endif

}
