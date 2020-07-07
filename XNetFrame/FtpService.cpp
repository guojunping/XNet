///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "FtpService.h"

namespace DnNetworkFramework{

CFtpService::CFtpService(CLog* pLog) : CFtpInternet(), CEnvironmentFactory(){
	m_pLog = pLog; // 

	m_bCancel         = false; // 
	m_bServiceRunning = false; // 
}

CFtpService::~CFtpService(void){
}

CFtpService::EFtpServiceResult CFtpService::createService(TSTRING& sAgent){ // 
	CEnvironmentContainer*             pEnvironmentContainer;
	CInternetImpl::EInternetImplResult iirResult;

	if(m_bServiceRunning == true) return FSR_ALREADY_SERVICE_RUNNING; // 

	if(CFtpInternet::m_pInternetImpl == NULL) return FSR_NULL;

	iirResult = CFtpInternet::m_pInternetImpl->open(sAgent); // 
	if(iirResult != CInternetImpl::IIR_OK) return FSR_FAILURE;

	if(CFtpInternet::m_pInternetImpl->setStatusCallback(CFtpService::__statusCallback) == false){ // 
		return FSR_REGISTER_FAILURE;
	};

	pEnvironmentContainer = new CEnvironmentContainer(); // 
	if(pEnvironmentContainer == NULL) return FSR_NULL;

	if(CEnvironmentFactory::setEnvironmentContainer(pEnvironmentContainer) == false){ // 
		return FSR_FACTORY_INIT_FAILURE;
	};

	CEnvironmentFactory::deleteAll();                                          // 
	CEnvironmentFactory::m_pEnvironmentContainer->clearEnvironmentContainer(); // 

	onCreateService();

	return FSR_OK;
}

CFtpService::EFtpServiceResult CFtpService::destroyService(void){ // 
	if(m_bServiceRunning == true) return FSR_ALREADY_SERVICE_RUNNING; // 

	onDestroyService();

	if(CFtpInternet::m_pInternetImpl != NULL){
		CFtpInternet::m_pInternetImpl->resetStatusCallback(); // 
		CFtpInternet::m_pInternetImpl->close();               // 
	};

	CEnvironmentFactory::deleteAll(); // 

	if(CEnvironmentFactory::m_pEnvironmentContainer != NULL){
		CEnvironmentFactory::m_pEnvironmentContainer->clearEnvironmentContainer(); // 
		delete CEnvironmentFactory::m_pEnvironmentContainer;    
		CEnvironmentFactory::m_pEnvironmentContainer = NULL; // 
	}

	return FSR_OK;
}

bool CFtpService::isServiceRunning(void){ // 
	return m_bServiceRunning;
}

CFtpService::EFtpServiceResult CFtpService::setPort(uint32 uiPort){ // 
	if(m_bServiceRunning == true) return FSR_ALREADY_SERVICE_RUNNING; // 

	m_uiPort = uiPort;

	return FSR_OK;
}

CFtpService::EFtpServiceResult CFtpService::setIpAddress(string& sIpAddress){ // 
	if(m_bServiceRunning == true) return FSR_ALREADY_SERVICE_RUNNING; // 

	m_sIpAddress = sIpAddress;

	return FSR_OK;
}

uint32 CFtpService::getPort(void){ // 
	return m_uiPort;
}

string CFtpService::getIpAddress(void){ //
	return m_sIpAddress;
}

CFtpService::EFtpServiceResult CFtpService::setId(TCHAR* szId){ // 
	if(m_bServiceRunning == true) return FSR_ALREADY_SERVICE_RUNNING; // 

	m_sId = szId;

	return FSR_OK;
}

CFtpService::EFtpServiceResult CFtpService::setPassword(TCHAR* szPassword){ // 
	if(m_bServiceRunning == true) return FSR_ALREADY_SERVICE_RUNNING; // 

	m_sPassword = szPassword;

	return FSR_OK;
}

TSTRING CFtpService::getId(void){ // 
	return m_sId;
}

TSTRING CFtpService::getPassword(void){ // 
	return m_sPassword;
}

CFtpService::EFtpServiceResult CFtpService::startService(void){ // 
	CInternetImpl::EInternetImplResult iirResult;

	if(m_bServiceRunning == true) return FSR_ALREADY_SERVICE_RUNNING; // 

	if(CFtpInternet::m_pInternetImpl == NULL || CEnvironmentFactory::m_pEnvironmentContainer == NULL) return FSR_NULL;

	iirResult = CFtpInternet::m_pInternetImpl->connect(m_sIpAddress, m_uiPort, (TCHAR*)m_sId.c_str(), (TCHAR*)m_sPassword.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, (DWORD)this); // 
	if(iirResult != CInternetImpl::IIR_OK) return FSR_CONNECT_FAILURE;

	CEnvironmentFactory::deleteAll(); // 

	CEnvironmentFactory::m_pEnvironmentContainer->clearEnvironmentContainer(); // 

	m_bServiceRunning = true;

	return FSR_OK;
}

CFtpService::EFtpServiceResult CFtpService::stopService(void){ // 
	if(m_bServiceRunning == false) return FSR_ALREADY_SERVICE_STOPPED; // 

	if(CFtpInternet::m_pInternetImpl == NULL || CEnvironmentFactory::m_pEnvironmentContainer == NULL) return FSR_NULL;

	m_bServiceRunning = false;

	CFtpInternet::m_pInternetImpl->disconnect(); // 

	CEnvironmentFactory::deleteAll(); // 

	CEnvironmentFactory::m_pEnvironmentContainer->clearEnvironmentContainer(); // 

	return FSR_OK;
}

CFtpService::EFtpServiceResult CFtpService::uploadFiles(void){ // 
	TSTRING                   sLocalFile, sRemoteFile;
	uint32                    uiLocalLen, uiRemoteLen;
	char                      szBuffer[4096 + 2];
	CEnvironment*             pEnvironment;
	FILE*                     pLocalFile;
	EnvironmentMap::iterator  itr;

	synchronized sync(&m_mMonitor);

	for(itr = m_mEnvironmentMap.begin(); itr != m_mEnvironmentMap.end(); itr++){
		pEnvironment = (CEnvironment*)(*itr).second;

		sLocalFile  = pEnvironment->m_sLocalFolder + _T("\\") + pEnvironment->m_sFile;  // 
		sRemoteFile = pEnvironment->m_sRemoteFolder + _T("\\") + pEnvironment->m_sFile; // 

		pLocalFile = _tfopen(sLocalFile.c_str(), _T("rb")); // 
		if(pLocalFile == NULL) return FSR_OPEN_FAILURE;

		CFtpInternet::createDirectory(pEnvironment->m_sRemoteFolder); // 

		if(CFtpInternet::openFile(sRemoteFile, true, true) != CInternetImpl::IIR_OK){ // 
			return FSR_OPEN_FAILURE;
		};

		CEnvironmentFactory::m_siCurFileCount++; // 

		pEnvironment->m_siCurLength = 0; // 

		onOpenFile(pEnvironment);

		while(1){
			if(isCancel() == true){ // 
				return FSR_CANCEL;
			};

			uiLocalLen = (uint32)fread(szBuffer, 1, 4096, pLocalFile); // 

			if(uiLocalLen != 0){ // 
				uiRemoteLen = uiLocalLen;

				if(CFtpInternet::m_pInternetImpl->writeFile(szBuffer, uiRemoteLen) != CInternetImpl::IIR_OK){ // 
					return FSR_WRITE_FAILURE;
				};
			
				if(uiLocalLen != uiRemoteLen) return FSR_WRITE_FAILURE; // 

				pEnvironment->m_siCurLength            += uiLocalLen; // 
				CEnvironmentFactory::m_siCurFileLength += uiLocalLen; // 
			};

			onUploadFile(pEnvironment);

			if(uiLocalLen != 4096) break; // 
		};

		fclose(pLocalFile);        // 
		CFtpInternet::closeFile(); // 

		onCloseFile(pEnvironment);
	};

	return FSR_OK;
}

CFtpService::EFtpServiceResult CFtpService::downloadFiles(void){ // 
	TSTRING                   sLocalFile, sRemoteFile;
	uint32                    uiLocalLen, uiRemoteLen;
	char                      szBuffer[4096 + 2];
	CEnvironment*             pEnvironment;
	FILE*                     pLocalFile;
	EnvironmentMap::iterator  itr;

	synchronized sync(&m_mMonitor);

	for(itr = m_mEnvironmentMap.begin(); itr != m_mEnvironmentMap.end(); itr++){
		pEnvironment = (CEnvironment*)(*itr).second;

		sLocalFile  = pEnvironment->m_sLocalFolder + _T("\\") + pEnvironment->m_sFile;  // 
		sRemoteFile = pEnvironment->m_sRemoteFolder + _T("\\") + pEnvironment->m_sFile; // 

		CSystem::createDirectory(pEnvironment->m_sLocalFolder); // 

		pLocalFile = _tfopen(sLocalFile.c_str(), _T("wb")); // 
		if(pLocalFile == NULL) return FSR_OPEN_FAILURE;

		if(CFtpInternet::openFile(sRemoteFile, false, true) != CInternetImpl::IIR_OK){ // 
			return FSR_OPEN_FAILURE;
		};

		CEnvironmentFactory::m_siCurFileCount++; // 

		pEnvironment->m_siCurLength = 0; // 

		onOpenFile(pEnvironment);

		while(1){
			if(isCancel() == true){ // 
				return FSR_CANCEL;
			};

			uiRemoteLen = 4096;

			if(CFtpInternet::m_pInternetImpl->readFile(szBuffer, uiRemoteLen) != CInternetImpl::IIR_OK){ // 
				return FSR_READ_FAILURE;
			};
			
			if(uiRemoteLen != 0){ // 
				uiLocalLen = uiRemoteLen;

				uiLocalLen = (uint32)fwrite(szBuffer, 1, uiLocalLen, pLocalFile); // 

				if(uiLocalLen != uiRemoteLen) return FSR_WRITE_FAILURE; // 

				pEnvironment->m_siCurLength            += uiLocalLen; // 
				CEnvironmentFactory::m_siCurFileLength += uiLocalLen; // 
			};

			onDownloadFile(pEnvironment);

			if(uiRemoteLen != 4096) break; // 
		};

		fclose(pLocalFile);        // 
		CFtpInternet::closeFile(); // 

		onCloseFile(pEnvironment);
	};

	return FSR_OK;
}

CFtpService::EFtpServiceResult CFtpService::enumerateFiles(void){ // 
	TSTRING                            sCurrentFolder, sRemoteFile;
	CEnvironment*                      pEnvironment;
	WIN32_FIND_DATA                    wfdFileData;
	CInternetImpl::EInternetImplResult iirResult;

	synchronized sync(&m_mMonitor);

	if(CFtpInternet::getCurrentFolder(sCurrentFolder) != CInternetImpl::IIR_OK){ // 
		return FSR_FAILURE;
	};

	sRemoteFile = sCurrentFolder + _T("\\*.*");

	if(CFtpInternet::findFirstFile(sRemoteFile, &wfdFileData) != CInternetImpl::IIR_OK){ // 
		return FSR_FAILURE;
	};

	do{
		pEnvironment = CEnvironmentFactory::m_pEnvironmentContainer->createEnvironment(); // 
		if(pEnvironment == NULL) return FSR_NULL;

		pEnvironment->m_sRemoteFolder = sCurrentFolder;        // 
		pEnvironment->m_sFile         = wfdFileData.cFileName; // 

		if((wfdFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0){ // 
			pEnvironment->m_ftGeneratedTime = wfdFileData.ftLastWriteTime;      // 
			pEnvironment->m_siLength        = (sint32)wfdFileData.nFileSizeLow; // 
		};

		if(CEnvironmentFactory::addEnvironment(pEnvironment) == false){ // 
			return FSR_FACTORY_ADD_FAILURE;
		};

		iirResult = CFtpInternet::findNextFile(&wfdFileData); // 
	}while(iirResult == CInternetImpl::IIR_OK);

	CFtpInternet::closeFind(); // 

	return FSR_OK;
}

void CALLBACK CFtpService::__statusCallback(HINTERNET hInternetId, DWORD dwContext, DWORD dwInternetStatus, void* pStatusInformation, DWORD dwStatusInformationLength){ // 
	CFtpService* pFtpService;

	pFtpService = (CFtpService*)dwContext; // 
	
	pFtpService->statusCallback(dwInternetStatus); // 
}

void CFtpService::statusCallback(DWORD dwInternetStatus){ // 
	switch(dwInternetStatus){
		case INTERNET_STATUS_CONNECTED_TO_SERVER : onConnect(); break;
		case INTERNET_STATUS_CONNECTION_CLOSED   : onClose();   break;
	};
}

bool CFtpService::isCancel(void){ // 
	return m_bCancel;
}

void CFtpService::setCancel(bool bCancel){ // 
	m_bCancel = bCancel;
}

}
