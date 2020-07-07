///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "IocpService.h"

namespace DnNetworkFramework{

CIocpService::CIocpService(CLog* pLog) : CSessionFactory(){
	sint32 siIndex;

	m_pLog            = pLog;

	m_uiPort          = 0;
	m_uiMaxSession    = 0;
	m_uiDispatchIndex = 0;

	m_dwSendBytes = 0;
	m_dwRecvBytes = 0;

	m_uiKeepAlive	=	0;
	m_uiKeepTime	=	0;
	m_uiKeepInterval	=	0;

	m_pAcceptIocp     = NULL;

	m_pProperties     = NULL;
	m_pServerSocket   = NULL;

	m_siRefCount      = 0;
	m_bServiceRunning = false;

	m_sIocpServiceClass = _T("");
	m_sMaxSessionProperty	    = _T("");
	m_sNumDispatchProperty = _T("");
	m_sRotativeBufferLevelProperty  = _T("");

	m_sServerSocketId = INVALID_SOCKET;

	m_emBufferLevel   = CRotativeBuffer::BL_NORMAL;

	for(siIndex = 0; siIndex < MAX_NUM_IOCP; siIndex++){
		m_pAsyncIoIocp[siIndex] = NULL;
	};
}

uint32 CIocpService::getThreadCount(void){ // 
	uint32 uiNumThread;

	uiNumThread = CSystem::getProcessorCount() * 2 + 1;

	return uiNumThread;
}

CIocpService::EIocpServiceResult CIocpService::createIocp(void){ //
	uint32 uiNumThread;
	sint32 uiIndex;

	m_pAcceptIocp = new CIocpImpl();
	if(m_pAcceptIocp == NULL) return ISR_NULL;

	//If this parameter is zero, the system allows as many concurrently running threads as there are processors in the system. 
	if(m_pAcceptIocp->create(0) != CIocpImpl::IIR_OK){
		m_uiErrorCode = m_pAcceptIocp->getErrorCode();
		return ISR_FAILURE;
	};

	uiNumThread = getThreadCount(); // 
	
	if(uiNumThread > MAX_NUM_IOCP) return ISR_FAILURE;

	for(uiIndex = 0; uiIndex < uiNumThread; uiIndex++){ // 
		m_pAsyncIoIocp[uiIndex] = new CIocpImpl();
		if(m_pAsyncIoIocp[uiIndex] == NULL) return ISR_NULL;

		if(m_pAsyncIoIocp[uiIndex]->create(0) != CIocpImpl::IIR_OK){
			m_uiErrorCode = m_pAsyncIoIocp[uiIndex]->getErrorCode();
			return ISR_FAILURE;
		};
	};

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::createSocket(void){ // 
	m_pServerSocket = new CServerSocket();
	if(m_pServerSocket == NULL) return ISR_NULL;

	if(m_pServerSocket->createForOverlapped() != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		return ISR_FAILURE;
	};

	if(m_pServerSocket->bind(m_sHost, (uint16)m_uiPort) != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		return ISR_FAILURE;
	};

	if(m_pServerSocket->listen(m_uiMaxSession) != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		return ISR_FAILURE;
	};

	if(m_pServerSocket->setReuseAddress(true) != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		return ISR_FAILURE;
	};

	if(m_pServerSocket->setKeepAlive(m_uiKeepAlive, m_uiKeepTime, m_uiKeepInterval) != CSocketImpl::SIR_OK){
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		return ISR_FAILURE;
	}

	m_sServerSocketId = m_pServerSocket->getSocketId(); // 

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::createDispatchService(void){ // 
	CDispatchService* pDispatchService;
	uint32            uiIndex;

	for(uiIndex = 0; uiIndex < m_uiNumDispatch; uiIndex++){
		pDispatchService = NULL;

		callbackCreateDispatchService(&pDispatchService);
		if(pDispatchService == NULL) return ISR_NULL;

		m_mDispatchServices[uiIndex] = pDispatchService;
	};

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::createSession(void){ // 
	CSession* pSession;
	uint32    uiIndex;

	for(uiIndex = 0; uiIndex < m_uiMaxSession; uiIndex++){
		pSession = NULL;

		callbackCreateSession(&pSession);
		if(pSession == NULL){
			return ISR_SESSION_CREATE_FAILURE;
		}

		if(pSession->initSession(uiIndex, m_pServerSocket, m_emBufferLevel, this) != CSession::SR_OK){
			m_uiErrorCode = pSession->getErrorCode();
			return ISR_SESSION_INIT_FAILURE;
		};

#ifdef SOCKET_LEVEL
		pSession->getSocket()->getSocketImpl()->setLog(m_pLog);
#endif

		callbackInitSession(&pSession);
		if(pSession == NULL){
			return ISR_SESSION_CREATE_FAILURE;
		}

		if(CSessionFactory::addSession(pSession) == false){
			return ISR_SESSION_ADD_FAILURE;
		}
	};

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::createService(void){ // 
	EIocpServiceResult isrResult;

	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	isrResult = createIocp();
	if(isrResult != ISR_OK) return isrResult;

	isrResult = createSocket();
	if(isrResult != ISR_OK) return isrResult;

	if(m_pAcceptIocp->bindHandle(reinterpret_cast<HANDLE>(m_sServerSocketId), (ULONG_PTR)m_pAcceptIocp) != CIocpImpl::IIR_OK){
		m_uiErrorCode = m_pAcceptIocp->getErrorCode();
		return ISR_FAILURE;
	};

	isrResult = createDispatchService();
	if(isrResult != ISR_OK) return isrResult;

	isrResult = createSession();
	if(isrResult != ISR_OK) return isrResult;

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::startService(void){ //
	uint32                       uiIndex, uiNumThread;
	CIocpAsyncIoThread*          pIocpAsyncIoThread;
	CIocpAcceptThread*           pIocpAcceptThread;
	CDispatchService*            pDispatchService;
	EIocpServiceResult           isrResult;
	DispatchServiceMap::iterator itr;

	CDispatchService::EDispatchServiceResult dsrResult;

	m_siRefCount++;

	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	isrResult = createService();
	if(isrResult != ISR_OK) return isrResult;

	setErrorCode(ISR_OK);

	onCreateService();

	if(m_isrResult != ISR_OK) return m_isrResult;

	m_bServiceRunning = true;

	for(itr = m_mDispatchServices.begin(); itr != m_mDispatchServices.end(); itr++){ // 
		pDispatchService = (CDispatchService*)(*itr).second;
		
		dsrResult = pDispatchService->startService();

		if(dsrResult == CDispatchService::DSR_NULL) return ISR_NULL;
		if(dsrResult == CDispatchService::DSR_THREAD_START_FAILURE) return ISR_THREAD_START_FAILURE;
	};

	uiNumThread = getThreadCount(); // SystemProcessorCount * 2 + 1

	for(uiIndex = 0; uiIndex < uiNumThread; uiIndex++){ // 
		pIocpAsyncIoThread = new CIocpAsyncIoThread(this, uiIndex); // 
		if(pIocpAsyncIoThread == NULL) return ISR_NULL;

        m_liIocpAsyncIoThreadList.push_back(pIocpAsyncIoThread);

		if(pIocpAsyncIoThread->start() != CThread::TR_OK) return ISR_THREAD_START_FAILURE;
	};
	
	pIocpAcceptThread = new CIocpAcceptThread(this, uiNumThread); // 
	if(pIocpAcceptThread == NULL) return ISR_NULL;

    m_liIocpAcceptThreadList.push_back(pIocpAcceptThread);

	if(pIocpAcceptThread->start() != CThread::TR_OK) return ISR_THREAD_START_FAILURE;

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::destroyService(void){ // 
	CDispatchService*            pDispatchService;
	sint32                       siIndex;
	DispatchServiceMap::iterator itr;

	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	for(itr = m_mDispatchServices.begin(); itr != m_mDispatchServices.end(); itr++){ // 
		pDispatchService = (CDispatchService*)(*itr).second;
		delete pDispatchService;
		pDispatchService = NULL;
	};

	itr = m_mDispatchServices.begin();
	if(itr != m_mDispatchServices.end()) m_mDispatchServices.erase(itr, m_mDispatchServices.end());

	CSessionFactory::closeAll();  // 
	CSessionFactory::deleteAll(); // 

	if(m_pAcceptIocp != NULL){
		delete m_pAcceptIocp; //
		m_pAcceptIocp = NULL;
	}

	for(siIndex = 0; siIndex < MAX_NUM_IOCP; siIndex++){
		if(m_pAsyncIoIocp[siIndex] != NULL){
			delete m_pAsyncIoIocp[siIndex]; // 
			m_pAsyncIoIocp[siIndex] = NULL;
		}
	}

	if(m_pServerSocket != NULL){
		delete m_pServerSocket; //
		m_pServerSocket = NULL;
	}

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::stopService(bool bMode){ // 
	CIocpAsyncIoThread*             pIocpAsyncIoThread;
	CIocpAcceptThread*              pIocpAcceptThread;
	CDispatchService*               pDispatchService;
	EIocpServiceResult              isrResult;
	IocpAcceptThreadList::iterator  itr1;
	IocpAsyncIoThreadList::iterator itr2;
	DispatchServiceMap::iterator    itr3;

	if(m_siRefCount == 0) return ISR_OK; // 
	m_siRefCount--;

	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == false) return ISR_ALREADY_SERVICE_STOPPED;

	m_bServiceRunning = false;

	for(itr1 = m_liIocpAcceptThreadList.begin(); itr1 != m_liIocpAcceptThreadList.end(); itr1++){ //
		pIocpAcceptThread = (CIocpAcceptThread*)(*itr1);

		pIocpAcceptThread->m_pAcceptIocp->postStatus(KEY_CLOSE_IOCP, 0, NULL); // 
		pIocpAcceptThread->stop();

		delete pIocpAcceptThread;
		pIocpAcceptThread = NULL;
	}

	itr1 = m_liIocpAcceptThreadList.begin();
	if(itr1 != m_liIocpAcceptThreadList.end()) m_liIocpAcceptThreadList.erase(itr1, m_liIocpAcceptThreadList.end());
	
	for(itr2 = m_liIocpAsyncIoThreadList.begin(); itr2 != m_liIocpAsyncIoThreadList.end(); itr2++){ // 
		pIocpAsyncIoThread = (CIocpAsyncIoThread*)(*itr2);

		pIocpAsyncIoThread->m_pAsyncIoIocp->postStatus(KEY_CLOSE_IOCP, 0, NULL); // 
		pIocpAsyncIoThread->stop();

		delete pIocpAsyncIoThread;
		pIocpAsyncIoThread = NULL;
	}

	itr2 = m_liIocpAsyncIoThreadList.begin();
	if(itr2 != m_liIocpAsyncIoThreadList.end()) m_liIocpAsyncIoThreadList.erase(itr2, m_liIocpAsyncIoThreadList.end());
	
	for(itr3 = m_mDispatchServices.begin(); itr3 != m_mDispatchServices.end(); itr3++){ // 
		pDispatchService = (CDispatchService*)(*itr3).second;

		pDispatchService->stopService();
	};

	if(bMode == true) onDestroyService();

	isrResult = destroyService();
	if(isrResult != ISR_OK) return isrResult;

	return ISR_OK;
}

CIocpService::~CIocpService(void){
	stopService(false); // 
}

void CIocpService::setServerName(LPTSTR szServerName){ // 
	if(szServerName != NULL) m_sServerName = szServerName;
}

TSTRING CIocpService::getServerName(void){ // 
	return m_sServerName;
}

CIocpService::EIocpServiceResult CIocpService::setProperties(CProperties* pProperties, TSTRING sServerClass){ // 
	TSTRING sHost, sBufferLevel;

	if(pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	m_pProperties = pProperties;

	m_sIocpServiceClass = sServerClass;

	m_uiPort        = m_pProperties->getIntProperty((LPTSTR)m_sIocpServiceClass.c_str(), PORT_PROPERTY);
	sHost           = m_pProperties->getStringProperty((LPTSTR)m_sIocpServiceClass.c_str(), HOST_PROPERTY);

#ifdef _UNICODE
	char szHost[64];

	if(::WideCharToMultiByte(CP_ACP, 0, sHost.c_str(), -1, szHost, 64, NULL, NULL) == 0) return ISR_NULL;
	m_sHost = szHost;
#else
    m_sHost = sHost;
#endif
	
	m_sMaxSessionProperty = MAX_SESSION_PROPERTY;
	m_sNumDispatchProperty = NUM_DISPATCH_PROPERTY;
	m_sRotativeBufferLevelProperty  = BUFFER_LEVEL_PROPERTY;

	m_uiMaxSession  = m_pProperties->getIntProperty((LPTSTR)m_sIocpServiceClass.c_str(), MAX_SESSION_PROPERTY);
	m_uiNumDispatch = m_pProperties->getIntProperty((LPTSTR)m_sIocpServiceClass.c_str(), NUM_DISPATCH_PROPERTY);
	
	m_uiKeepAlive	=	m_pProperties->getIntProperty((LPTSTR)m_sIocpServiceClass.c_str(), KEEP_ALIVE);
	m_uiKeepTime	=	m_pProperties->getIntProperty((LPTSTR)m_sIocpServiceClass.c_str(), KEEP_TIME);
	m_uiKeepInterval	=	m_pProperties->getIntProperty((LPTSTR)m_sIocpServiceClass.c_str(), KEEP_INTERVAL);

	sBufferLevel    = m_pProperties->getStringProperty((LPTSTR)m_sIocpServiceClass.c_str(), BUFFER_LEVEL_PROPERTY);

	if(_tcscmp(sBufferLevel.c_str(),      _T("Large"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Small"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_SMALL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Normal"))    == 0) m_emBufferLevel = CRotativeBuffer::BL_NORMAL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VeryLarge")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VerySmall")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_SMALL;

	if(m_uiPort == 0 || m_sHost.empty() == true || m_uiMaxSession == 0) return ISR_INVALID_PROPERTY;

	if(m_uiNumDispatch == 0) m_uiNumDispatch = getThreadCount();

	return ISR_OK;
}

CProperties* CIocpService::getProperties(void){ // 
	return m_pProperties;
}

uint32 CIocpService::getErrorCode(void){
	return m_uiErrorCode;
}

void CIocpService::setErrorCode(CIocpService::EIocpServiceResult isrResult){
	m_isrResult = isrResult;
}

bool CIocpService::isServiceRunning(void){ // 
	return m_bServiceRunning;
}

CDispatchService* CIocpService::getDispatchService(void){ // 
	CDispatchService*            pDispatchService;
	DispatchServiceMap::iterator itr;

	m_uiDispatchIndex++;

	if(m_uiDispatchIndex >= m_uiNumDispatch) m_uiDispatchIndex = 0;

	itr = m_mDispatchServices.find(m_uiDispatchIndex);
	pDispatchService = (*itr).second;

	return pDispatchService;
}

CDispatchService* CIocpService::getDispatchService(uint32 uiDispatchIndex){ // 
	CDispatchService*            pDispatchService;
	DispatchServiceMap::iterator itr;

	itr = m_mDispatchServices.find(uiDispatchIndex);

	if(itr == m_mDispatchServices.end()) pDispatchService = NULL;
	else pDispatchService = (CDispatchService*)(*itr).second;

	return pDispatchService;
}

uint32 CIocpService::getPort(void){ // 
	return m_uiPort;
}

string CIocpService::getIpAddress(void){ //
	return m_sHost;
}

CIocpService::EIocpServiceResult CIocpService::setPort(uint32 uiPort){ // 
	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	if(uiPort == 0) return ISR_INVALID_PROPERTY;

	m_uiPort = uiPort;

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::setIpAddress(string sHost){ // 
	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	if(sHost.empty() == true) return ISR_INVALID_PROPERTY;

	m_sHost = sHost;

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::setBufferLevel(TSTRING sBufferLevel){ // 
	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	if(_tcscmp(sBufferLevel.c_str(),      _T("Large"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Small"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_SMALL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Normal"))    == 0) m_emBufferLevel = CRotativeBuffer::BL_NORMAL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VeryLarge")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VerySmall")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_SMALL;
	else return ISR_INVALID_PROPERTY;

	return ISR_OK;
}

uint32 CIocpService::getMaxSession(void){ // 
	return m_uiMaxSession;
}

CIocpService::EIocpServiceResult CIocpService::setMaxSession(uint32 uiMaxSession){ // 
	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	if(uiMaxSession == 0) return ISR_INVALID_PROPERTY;

	m_uiMaxSession = uiMaxSession;

	return ISR_OK;
}

uint32 CIocpService::getNumDispatch(void){ // 
	return m_uiNumDispatch;
}

CIocpService::EIocpServiceResult CIocpService::setNumDispatch(uint32 uiNumDispatch){ // 
	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	m_uiNumDispatch = uiNumDispatch;

	if(m_uiNumDispatch == 0) m_uiNumDispatch = getThreadCount();
	
	return ISR_OK;
}

bool CIocpService::getKeepAlive(void){
	if(m_uiKeepAlive == 0) return false;
	else return true;
}

uint32 CIocpService::getKeepTime(void){
	return m_uiKeepTime;
}

uint32 CIocpService::getKeepInterval(void){
	return m_uiKeepInterval;
}

CIocpService::EIocpServiceResult CIocpService::setKeepAlive(bool bKeepAlive){
	if(bKeepAlive == false) m_uiKeepAlive = 0;
	else m_uiKeepAlive = 1;

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::setKeepTime(uint32 uiKeepTime){
	m_uiKeepTime	=	uiKeepTime;

	return ISR_OK;
}

CIocpService::EIocpServiceResult CIocpService::setKeepInterval(uint32 uiKeepInterval){
	m_uiKeepInterval = uiKeepInterval;

	return ISR_OK;
}

void CIocpService::incSendBytes(DWORD dwSendBytes){
	synchronized sync(&m_cMonitor);

	m_dwSendBytes += dwSendBytes;
}

void CIocpService::incRecvBytes(DWORD dwRecvBytes){
	synchronized sync(&m_cMonitor);

	m_dwRecvBytes += dwRecvBytes;
}

DWORD CIocpService::getSendBytes(DWORD dwSeconds){
	synchronized sync(&m_cMonitor);

	DWORD dwSendBytes = m_dwSendBytes;
	m_dwSendBytes = 0;

	return dwSendBytes/dwSeconds;
}

DWORD CIocpService::getRecvBytes(DWORD dwSeconds){
	synchronized sync(&m_cMonitor);

	DWORD dwRecvBytes = m_dwRecvBytes;
	m_dwRecvBytes = 0;

	return dwRecvBytes/dwSeconds;
}
}
