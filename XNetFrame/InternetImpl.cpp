///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "InternetImpl.h"

namespace DnNetworkFramework{

CInternetImpl::CInternetImpl(void){
	m_hSessionId  = NULL;
	m_hInternetId = NULL;

	m_uiErrorCode = 0;
	m_bConnected  = false;
}

CInternetImpl::~CInternetImpl(void){
	disconnect(); // 
	close();      // 
}

CInternetImpl::EInternetImplResult CInternetImpl::open(TSTRING& sAgent){ // 
	m_hInternetId = ::InternetOpen(sAgent.c_str(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if(m_hInternetId == NULL){
		m_uiErrorCode = ::GetLastError();
		return IIR_FAILURE;
	};

	return IIR_OK;
}

uint32 CInternetImpl::getErrorCode(void){ // 
	return m_uiErrorCode;
}

CInternetImpl::EInternetImplResult CInternetImpl::connect(string& sIpAddress, sint32 siPort, TCHAR* szId, TCHAR* szPassword, sint32 siService, sint32 siFlags, DWORD dwContext){ // 
	if(m_hInternetId == NULL) return IIR_NULL;

	if(siService != INTERNET_SERVICE_FTP && siService != INTERNET_SERVICE_HTTP) return IIR_UNKNOWN_SERVICE;

	m_hSessionId = ::InternetConnect(m_hInternetId, sIpAddress.c_str(), siPort, szId, szPassword, (DWORD)siService, (DWORD)siFlags, dwContext);

	if(m_hSessionId == NULL){
		m_uiErrorCode = ::GetLastError();
		return IIR_FAILURE;
	};

	m_bConnected = true;

	return IIR_OK;
}

bool CInternetImpl::isConnected(void){ // 
	return m_bConnected;
}

CInternetImpl::EInternetImplResult CInternetImpl::readFile(void* szBuffer, uint32& uiLen){ // 
	uint32 uiReceivedLen;
	BOOL   bResult;

	if(m_hFileId == NULL) return IIR_NULL;

	if(szBuffer == NULL) return IIR_NULL;

	bResult = ::InternetReadFile(m_hFileId, szBuffer, uiLen, (LPDWORD)&uiReceivedLen);

	if(bResult == FALSE){
		m_uiErrorCode = ::GetLastError();
		return IIR_FAILURE;
	};

	uiLen = uiReceivedLen;

	return IIR_OK;
}

CInternetImpl::EInternetImplResult CInternetImpl::writeFile(void* szBuffer, uint32& uiLen){ // 
	uint32 uiSentLen;
	BOOL   bResult;

	if(m_hFileId == NULL) return IIR_NULL;

	if(szBuffer == NULL) return IIR_NULL;

	bResult = ::InternetWriteFile(m_hFileId, szBuffer, uiLen, (LPDWORD)&uiSentLen);

	if(bResult == FALSE){
		m_uiErrorCode = ::GetLastError();
		return IIR_FAILURE;
	};

	uiLen = uiSentLen;

	return IIR_OK;
}

void CInternetImpl::close(void){ // 
	if(m_hInternetId != NULL) ::InternetCloseHandle(m_hInternetId);

	m_hInternetId = NULL;
}

void CInternetImpl::disconnect(void){ // 
	if(m_hSessionId != NULL) ::InternetCloseHandle(m_hSessionId);

	m_hSessionId = NULL;

	m_bConnected = false;
}

void CInternetImpl::resetStatusCallback(void){ // 
	if(m_hInternetId != NULL) ::InternetSetStatusCallback(m_hInternetId, NULL);
}

bool CInternetImpl::setStatusCallback(INTERNET_STATUS_CALLBACK pStatusCallback){ // 
	if(m_hInternetId == NULL) return false;

	if(::InternetSetStatusCallback(m_hInternetId, pStatusCallback) == INTERNET_INVALID_STATUS_CALLBACK){
		return false;
	};
	
	return true;
}

}
