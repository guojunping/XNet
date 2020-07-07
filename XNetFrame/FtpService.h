///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __FTP_SERVICE_H
#define __FTP_SERVICE_H

#include "Log.h"
#include "System.h"
#include "FtpInternet.h"
#include "EnvironmentFactory.h"

namespace DnNetworkFramework{

class CFtpService : public CFtpInternet, public CEnvironmentFactory{
public:
	enum EFtpServiceResult { FSR_OK = 0, FSR_FAILURE, FSR_NULL, FSR_CONNECT_FAILURE, FSR_ALREADY_SERVICE_RUNNING, FSR_ALREADY_SERVICE_STOPPED, FSR_OPEN_FAILURE, FSR_READ_FAILURE, FSR_WRITE_FAILURE,
		FSR_FACTORY_INIT_FAILURE, FSR_FACTORY_ADD_FAILURE, FSR_REGISTER_FAILURE, FSR_CANCEL };

private:
	CLog* m_pLog; // 

	TSTRING m_sId;             // 
	uint32  m_uiPort;          // 
	bool    m_bCancel;         // 
	TSTRING m_sPassword;       // 
	string  m_sIpAddress;      // 
	bool    m_bServiceRunning; // 

private:
	void                 statusCallback(DWORD);                                   // 
	static void CALLBACK __statusCallback(HINTERNET, DWORD, DWORD, void*, DWORD); // 

public:
	CFtpService(CLog*);
	virtual ~CFtpService(void);

	TSTRING getId(void);            // 
	uint32  getPort(void);          // 
	bool    isCancel(void);         // 
	void    setCancel(bool);        // 
	TSTRING getPassword(void);      // 
	string  getIpAddress(void);     // 
	bool    isServiceRunning(void); // 

	EFtpServiceResult uploadFiles(void);    // 
	EFtpServiceResult downloadFiles(void);  // 
	EFtpServiceResult enumerateFiles(void); // 

	EFtpServiceResult setId(TCHAR*);         // 
	EFtpServiceResult setPort(uint32);       // 
	EFtpServiceResult setPassword(TCHAR*);   // 
	EFtpServiceResult setIpAddress(string&); // 

	EFtpServiceResult stopService(void);       // 
	EFtpServiceResult startService(void);      // 
	EFtpServiceResult destroyService(void);    // 
	EFtpServiceResult createService(TSTRING&); // 
	
	virtual void onClose(void)   = 0;
	virtual void onConnect(void) = 0; // 

	virtual void onCreateService(void)  = 0;
	virtual void onDestroyService(void) = 0; // 

	virtual void onOpenFile(CEnvironment*) = 0;
	virtual void onCloseFile(CEnvironment*) = 0;
	virtual void onUploadFile(CEnvironment*) = 0;
	virtual void onDownloadFile(CEnvironment*) = 0; // 
};

}

#endif
