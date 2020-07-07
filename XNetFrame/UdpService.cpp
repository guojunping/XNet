///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "UdpService.h"
#include "UdpThread.h"
#include "ContextDispatchService.h"

namespace XNetFrame{

CUdpService::CUdpService(CLog* pLog){
	m_pLog             = pLog;

	m_sHost	=	_T("");
	m_uiPort           = 0;
	m_siRefCount       = 0;
	m_uiPortRange      = 1;

	m_pContext         = NULL;
	m_pProperties      = NULL;
	m_pDispatchService = NULL;

	m_bServiceRunning  = false;

	m_sUdpServiceClass = _T("");
	m_sPortProperty = _T("");
	m_sHostProperty = _T("");
	m_sRotativeBufferProperty = _T("");

	m_emBufferLevel    = CRotativeBuffer::BL_NORMAL;
}

CUdpService::~CUdpService(void){
	stopService();
}

CUdpService::EUdpServiceResult CUdpService::createService(void){ // 
	EUdpServiceResult usrResult;

	if(m_pProperties == NULL) return USR_NULL;
	if(m_bServiceRunning == true) return USR_ALREADY_SERVICE_RUNNING;

	usrResult = createDispatchService();
	if(usrResult != USR_OK) return usrResult;

	usrResult = createContext();
	if(usrResult != USR_OK) return usrResult;

	return USR_OK;
}

CUdpService::EUdpServiceResult CUdpService::createDispatchService(void){ //
	m_pDispatchService = NULL;

	m_pDispatchService=new CContextDispatchService(m_pLog, 2, false);
	if(m_pDispatchService == NULL) return USR_NULL;

	return USR_OK;
}

CUdpService::EUdpServiceResult CUdpService::createContext(void){ // 
	uint32 uiIndex;

	m_pContext = NULL;

	callbackCreateContext(&m_pContext);
	if(m_pContext == NULL) return USR_CONTEXT_CREATE_FAILURE;

	for(uiIndex = 0; uiIndex < m_uiPortRange; uiIndex++){
		if(m_pContext->initContext(m_sHost, m_uiPort, m_pDispatchService, m_emBufferLevel) == CContext::CR_OK){
#ifdef SOCKET_LEVEL
			m_pContext->setLog(m_pLog);
#endif
			return USR_OK;
		}

		m_uiPort = m_uiPort + 1;
	};

	m_uiErrorCode = m_pContext->getErrorCode();

	return USR_CONTEXT_INIT_FAILURE;
}

CUdpService::EUdpServiceResult CUdpService::startService(uint32 uiPortRange){ // 
	CUdpThread*       pUdpThread;
	EUdpServiceResult usrResult;

	CDispatchService::EDispatchServiceResult dsrResult;

	m_siRefCount++;

	m_uiPortRange = uiPortRange;

	usrResult = createService();
	if(usrResult != USR_OK) return usrResult;

	setErrorCode(USR_OK);

	onCreateService();

	if(m_usrResult != USR_OK) return m_usrResult;

	m_bServiceRunning = true;

	dsrResult = m_pDispatchService->startService();

	if(dsrResult == CDispatchService::DSR_NULL) return USR_NULL;
	if(dsrResult == CDispatchService::DSR_THREAD_START_FAILURE) return USR_THREAD_START_FAILURE;

	pUdpThread = new CUdpThread(this); // 
	if(pUdpThread == NULL) return USR_NULL;

    m_liUdpThreadList.push_back((UdpThreadList::value_type)pUdpThread);

	if(pUdpThread->start() != CThread::TR_OK) return USR_THREAD_START_FAILURE;

	return USR_OK;
}

void CUdpService::setErrorCode(CUdpService::EUdpServiceResult usrResult){
	m_usrResult = usrResult;
}

CUdpService::EUdpServiceResult CUdpService::stopService(void){ // 
	CUdpThread*             pUdpThread;
	EUdpServiceResult       usrResult;
	UdpThreadList::iterator itr;

	if(m_siRefCount == 0) return USR_OK; // 
	
	m_siRefCount--;

	if(m_pProperties == NULL) return USR_NULL;
	if(m_bServiceRunning == false) return USR_ALREADY_SERVICE_STOPPED;

	m_bServiceRunning = false;

	for(itr = m_liUdpThreadList.begin(); itr != m_liUdpThreadList.end(); itr++){ // 
		pUdpThread = (CUdpThread*)(*itr);
#ifdef WIN32		
		pUdpThread->stop(m_pContext->getIoEvent()); // 
#else
		pUdpThread->stop(NULL);
#endif
		delete pUdpThread;
		pUdpThread = NULL;
	};

	itr = m_liUdpThreadList.begin();
	if(itr != m_liUdpThreadList.end()) m_liUdpThreadList.erase(itr, m_liUdpThreadList.end());
	
	m_pDispatchService->stopService(); // 

	onDestroyService();

	usrResult = destroyService();
	if(usrResult != USR_OK) return usrResult;

	return USR_OK;
}

CUdpService::EUdpServiceResult CUdpService::destroyService(void){ //
	if(m_bServiceRunning == true) return USR_ALREADY_SERVICE_RUNNING;

	if(m_pDispatchService != NULL){
		delete m_pDispatchService; //
		m_pDispatchService = NULL;
	}

	if(m_pContext != NULL){
		delete m_pContext; //
		m_pContext = NULL;
	}

	return USR_OK;
}

CUdpService::EUdpServiceResult CUdpService::setProperties(CProperties* pProperties, TSTRING sServerClass, TSTRING sServerProperty){ // 
	TSTRING sHost, sBufferLevel;
	
	if(pProperties == NULL) return USR_NULL;
	if(m_bServiceRunning == true) return USR_ALREADY_SERVICE_RUNNING;

	m_pProperties = pProperties;

	m_sUdpServiceClass = sServerClass;

	m_sPortProperty = sServerProperty + _T("_") + PORT_PROPERTY;
	m_sHostProperty = sServerProperty + _T("_") + HOST_PROPERTY;
	
	m_uiPort     = m_pProperties->getIntProperty((LPTSTR)m_sUdpServiceClass.c_str(), (LPTSTR)m_sPortProperty.c_str());
	sHost        = m_pProperties->getStringProperty((LPTSTR)m_sUdpServiceClass.c_str(), (LPTSTR)m_sHostProperty.c_str());

#ifdef _UNICODE
	char szHost[64];

	if(::WideCharToMultiByte(CP_ACP, 0, sHost.c_str(), -1, szHost, 64, NULL, NULL) == 0) return USR_NULL;
	m_sHost = szHost;
#else
	m_sHost = sHost;
#endif

	m_sRotativeBufferProperty = sServerProperty + _T("_") + BUFFER_LEVEL_PROPERTY;

	sBufferLevel = m_pProperties->getStringProperty((LPTSTR)m_sUdpServiceClass.c_str(), (LPTSTR)m_sRotativeBufferProperty.c_str());

	if(_tcscmp(sBufferLevel.c_str(),      _T("Large"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Small"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_SMALL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Normal"))    == 0) m_emBufferLevel = CRotativeBuffer::BL_NORMAL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VeryLarge")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VerySmall")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_SMALL;

	if(m_uiPort == 0 || m_sHost.empty() == true){
		//return USR_INVALID_PROPERTY;
		//m_uiPort = 1;
		//m_sHost = _T("");
	}
	if(sBufferLevel.empty() == true) m_emBufferLevel = CRotativeBuffer::BL_VERY_LARGE;

	return USR_OK;
}

CContext* CUdpService::getContext(void){ // 
	return m_pContext;
}

uint32 CUdpService::getErrorCode(void){
	return m_uiErrorCode;
}

uint32 CUdpService::getPort(void){ // 
	return m_uiPort;
}

string CUdpService::getIpAddress(void){ // 
	return m_sHost;
}

CUdpService::EUdpServiceResult CUdpService::setPort(uint32 uiPort){ // 
	if(m_pProperties == NULL) return USR_NULL;
	if(m_bServiceRunning == true) return USR_ALREADY_SERVICE_RUNNING;

	if(uiPort == 0) return USR_INVALID_PROPERTY;

	m_uiPort = uiPort;

	return USR_OK;
}

CUdpService::EUdpServiceResult CUdpService::setIpAddress(string sHost){ // 
	if(m_pProperties == NULL) return USR_NULL;
	if(m_bServiceRunning == true) return USR_ALREADY_SERVICE_RUNNING;

	if(sHost.empty() == true) return USR_INVALID_PROPERTY;

	m_sHost = sHost;

	return USR_OK;
}

CUdpService::EUdpServiceResult CUdpService::setBufferLevel(TSTRING sBufferLevel){ // 
	if(m_pProperties == NULL) return USR_NULL;
	if(m_bServiceRunning == true) return USR_ALREADY_SERVICE_RUNNING;

	if(_tcscmp(sBufferLevel.c_str(),      _T("Large"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Small"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_SMALL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Normal"))    == 0) m_emBufferLevel = CRotativeBuffer::BL_NORMAL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VeryLarge")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VerySmall")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_SMALL;
	else return USR_INVALID_PROPERTY;

	return USR_OK;
}

}
