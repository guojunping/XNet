///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "FtpInternet.h"

namespace DnNetworkFramework{

CFtpInternet::CFtpInternet(void){
	m_hFileId = NULL;
	m_hFindId = NULL;

	m_uiErrorCode = 0;

	m_pInternetImpl = new CInternetImpl();
}

CFtpInternet::~CFtpInternet(void){
	closeFile(); // 
	closeFind(); // 

	if(m_pInternetImpl != NULL){
		delete m_pInternetImpl;
		m_pInternetImpl = NULL;
	}
}

CInternetImpl::EInternetImplResult CFtpInternet::openFile(TSTRING& sFile, bool bAccess, bool bFlags){ // 
	sint32 siAccess, siFlags;

	if(m_pInternetImpl == NULL) return CInternetImpl::IIR_NULL;
	if(m_pInternetImpl->m_hSessionId == NULL) return CInternetImpl::IIR_NULL;

	siAccess = (bAccess == false)? GENERIC_READ : GENERIC_WRITE;
	siFlags  = (bFlags == false)? FTP_TRANSFER_TYPE_ASCII : FTP_TRANSFER_TYPE_BINARY;

	m_hFileId = ::FtpOpenFile(m_pInternetImpl->m_hSessionId, (TCHAR*)sFile.c_str(), (DWORD)siAccess, (DWORD)siFlags, NULL);

	if(m_hFileId == NULL){
		m_uiErrorCode = ::GetLastError();
		return CInternetImpl::IIR_FAILURE;
	};

	m_pInternetImpl->m_hFileId = m_hFileId;

	return CInternetImpl::IIR_OK;
}

uint32 CFtpInternet::getErrorCode(void){ // 
	return m_uiErrorCode;
}

void CFtpInternet::closeFile(void){ // 
	if(m_hFileId != NULL) ::InternetCloseHandle(m_hFileId);

	m_hFileId = NULL;
}

CInternetImpl::EInternetImplResult CFtpInternet::deleteFile(TSTRING& sFile){ // 
	BOOL bResult;

	if(m_pInternetImpl == NULL) return CInternetImpl::IIR_NULL;
	if(m_pInternetImpl->m_hSessionId == NULL) return CInternetImpl::IIR_NULL;

	bResult = ::FtpDeleteFile(m_pInternetImpl->m_hSessionId, sFile.c_str());

	if(bResult == FALSE){
		m_uiErrorCode = ::GetLastError();
		return CInternetImpl::IIR_FAILURE;
	};

	return CInternetImpl::IIR_OK;
}

CInternetImpl::EInternetImplResult CFtpInternet::renameFile(TSTRING& sOldFile, TSTRING& sNewFile){ // 
	BOOL bResult;

	if(m_pInternetImpl == NULL) return CInternetImpl::IIR_NULL;
	if(m_pInternetImpl->m_hSessionId == NULL) return CInternetImpl::IIR_NULL;

	bResult = ::FtpRenameFile(m_pInternetImpl->m_hSessionId, sOldFile.c_str(), sNewFile.c_str());

	if(bResult == FALSE){
		m_uiErrorCode = ::GetLastError();
		return CInternetImpl::IIR_FAILURE;
	};

	return CInternetImpl::IIR_OK;
}

CInternetImpl::EInternetImplResult CFtpInternet::getFileSize(TSTRING& sFile, uint32& uiLen){ //
	uint32 uiFileLen;
	BOOL   bResult;

	if(m_hFileId == NULL) return CInternetImpl::IIR_NULL;

	bResult = ::FtpGetFileSize(m_hFileId, (LPDWORD)&uiFileLen);

	if(bResult == FALSE){
		m_uiErrorCode = ::GetLastError();
		return CInternetImpl::IIR_FAILURE;
	};

	uiLen = uiFileLen;

	return CInternetImpl::IIR_OK;
}

CInternetImpl::EInternetImplResult CFtpInternet::getCurrentFolder(TSTRING& sFolder){ //
	TCHAR  szFolder[MAX_PATH];
	BOOL   bResult;
	uint32 uiLen;

	if(m_pInternetImpl == NULL) return CInternetImpl::IIR_NULL;
	if(m_pInternetImpl->m_hSessionId == NULL) return CInternetImpl::IIR_NULL;

	uiLen = MAX_PATH;

	bResult = ::FtpGetCurrentDirectory(m_pInternetImpl->m_hSessionId, szFolder, (LPDWORD)&uiLen);

	if(bResult == FALSE){
		m_uiErrorCode = ::GetLastError();
		return CInternetImpl::IIR_FAILURE;
	};

	szFolder[uiLen] = 0x00;
	sFolder         = szFolder;

	return CInternetImpl::IIR_OK;
}

CInternetImpl::EInternetImplResult CFtpInternet::setCurrentFolder(TSTRING& sFolder){ //
	BOOL bResult;

	if(m_pInternetImpl == NULL) return CInternetImpl::IIR_NULL;
	if(m_pInternetImpl->m_hSessionId == NULL) return CInternetImpl::IIR_NULL;

	bResult = ::FtpSetCurrentDirectory(m_pInternetImpl->m_hSessionId, sFolder.c_str());

	if(bResult == FALSE){
		m_uiErrorCode = ::GetLastError();
		return CInternetImpl::IIR_FAILURE;
	};

	return CInternetImpl::IIR_OK;
}

CInternetImpl::EInternetImplResult CFtpInternet::createFolder(TSTRING& sFolder){ //
	BOOL bResult;

	if(m_pInternetImpl == NULL) return CInternetImpl::IIR_NULL;
	if(m_pInternetImpl->m_hSessionId == NULL) return CInternetImpl::IIR_NULL;

	bResult = ::FtpCreateDirectory(m_pInternetImpl->m_hSessionId, sFolder.c_str());

	if(bResult == FALSE){
		m_uiErrorCode = ::GetLastError();
		return CInternetImpl::IIR_FAILURE;
	};

	return CInternetImpl::IIR_OK;
}

CInternetImpl::EInternetImplResult CFtpInternet::createDirectory(TSTRING& sDirectory1){ //
	TCHAR   szDirectory[1024];
	TSTRING sDirectory2;
	TCHAR*  pDirectory1;
	TCHAR*  pDirectory2;
	
	pDirectory2 = szDirectory;
	pDirectory1 = (TCHAR*)sDirectory1.c_str();

	*pDirectory2 = '\0';

	while(*pDirectory1){
		if((*pDirectory1 == '\\') || (*pDirectory1 == '/')){
			sDirectory2 = szDirectory;

			if(sDirectory2.length() != 0){
				createFolder(sDirectory2);
			};
		};

		*pDirectory2++ = *pDirectory1++;
		*pDirectory2   = '\0';
	};

	sDirectory2 = szDirectory;

	return createFolder(sDirectory2);
}

CInternetImpl::EInternetImplResult CFtpInternet::removeFolder(TSTRING& sFolder){ //
	BOOL bResult;

	if(m_pInternetImpl == NULL) return CInternetImpl::IIR_NULL;
	if(m_pInternetImpl->m_hSessionId == NULL) return CInternetImpl::IIR_NULL;

	bResult = ::FtpRemoveDirectory(m_pInternetImpl->m_hSessionId, sFolder.c_str());

	if(bResult == FALSE){
		m_uiErrorCode = ::GetLastError();
		return CInternetImpl::IIR_FAILURE;
	};

	return CInternetImpl::IIR_OK;
}

CInternetImpl::EInternetImplResult CFtpInternet::findFirstFile(TSTRING& sFolder, WIN32_FIND_DATA* pFileData){ //
	if(m_pInternetImpl == NULL) return CInternetImpl::IIR_NULL;
	if(m_pInternetImpl->m_hSessionId == NULL) return CInternetImpl::IIR_NULL;

	if(pFileData == NULL) return CInternetImpl::IIR_NULL;

	m_hFindId = ::FtpFindFirstFile(m_pInternetImpl->m_hSessionId, sFolder.c_str(), pFileData, 0, NULL);

	if(m_hFindId == NULL){
		m_uiErrorCode = ::GetLastError();
		return CInternetImpl::IIR_FAILURE;
	};

	return CInternetImpl::IIR_OK;
}

CInternetImpl::EInternetImplResult CFtpInternet::findNextFile(WIN32_FIND_DATA* pFileData){ //
	BOOL bResult;

	if(m_hFindId == NULL) return CInternetImpl::IIR_NULL;

	if(pFileData == NULL) return CInternetImpl::IIR_NULL;

	bResult = ::InternetFindNextFile(m_hFindId, pFileData);

	if(bResult == NULL){
		m_uiErrorCode = ::GetLastError();
		return CInternetImpl::IIR_FAILURE;
	};

	return CInternetImpl::IIR_OK;
}

void CFtpInternet::closeFind(void){ //
	if(m_hFindId != NULL) ::InternetCloseHandle(m_hFindId);

	m_hFindId = NULL;
}

}
