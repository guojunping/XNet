///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "EpollService.h"
#include "SessionDispatchService.h"

#include <sys/resource.h>

namespace XNetFrame{

CEpollService::CEpollService(CLog* pLog) : CSessionFactory(){

	m_pLog              = pLog;

	m_uiPort            = 0;
	m_uiMaxSession     = 0;
	m_uiDispatchIndex  = 0;

	m_dwSendBytes = 0;
	m_dwRecvBytes = 0;

	m_uiKeepAlive	=	0;
	m_uiKeepTime	=	0;
	m_uiKeepInterval	=	0;

	m_pAsyncIoEpoll     = NULL;

	m_pProperties       = NULL;
	m_pServerSocket     = NULL;

	m_siRefCount       = 0;
	m_bServiceRunning = false;

	m_sEpollServiceClass = _T("");
	m_sMaxSessionProperty	    = _T("");
	m_sNumDispatchProperty = _T("");
	m_sRotativeBufferLevelProperty  = _T("");

	m_sServerSocketId = INVALID_SOCKET;

	m_emBufferLevel   = CRotativeBuffer::BL_NORMAL;
	m_pEpollAsyncIoThread = NULL;

}

uint32 CEpollService::getThreadCount(void){ // 

	uint32 uiNumThread;
	
	uiNumThread = CSystem::getProcessorCount() * 2 + 1;

	return uiNumThread;
}

CEpollService::EEpollServiceResult CEpollService::createEpoll(void){ //
	//uint32 uiNumThread;
	//sint32 uiIndex;

	m_pAsyncIoEpoll = new CEpoll();
	if(m_pAsyncIoEpoll == NULL) return ISR_NULL;

	//If this parameter is zero, the system allows as many concurrently running threads as there are processors in the system. 
	if(m_pAsyncIoEpoll->create(this,m_pServerSocket,m_uiMaxSession) != CEpollImpl::EIR_OK){
		m_uiErrorCode = m_pAsyncIoEpoll->getErrorCode();
		return ISR_FAILURE;
	};

	//uiNumThread = getThreadCount(); // 
	
	//if(uiNumThread > MAX_NUM_IOCP) return ISR_FAILURE;

	return ISR_OK;
}

void CEpollService::eventHandle(sint32 siFd,sint32 siEvents,void* pPtr)
{
	//printf("eventHandle fd=%d\n",siFd);

	CSession* pSession=NULL;
	if(siFd==m_sServerSocketId)
	{		 
		while(true)
		{
			pSession=getAvailableSession();
			if(pSession==NULL)
			{
				m_pLog->logLn(CLog::LL_0, "EpollService Not have available session error");
				return;
				////////////////////////////
			}
			
			CSocketImpl::ESocketImplResult srResult=m_pServerSocket->accept(pSession->getSocket());
			if(srResult!= CSocketImpl::SIR_OK)
			{
				putAuailableSession(pSession);
				if(srResult == CSocketImpl::SIR_WOULD_BLOCK)
					break;
					
				m_pLog->logLn(CLog::LL_0, "EpollService Accept connect is error ,errno=%d ",m_pServerSocket->getErrorCode());
				/////////////////////////////
				return;
			}

			if(pSession->activeSession(getDispatchService(),getKeepAlive(), getKeepTime(), getKeepInterval())!=CSession::SR_OK)                // 
			{
				m_pLog->logLn(CLog::LL_0, "EpollService Active session fail error");
				putAuailableSession(pSession);
				continue;
			}

			if(!m_pAsyncIoEpoll->add(pSession->getSocketId(),EPOLLIN|EPOLLOUT|EPOLLHUP|EPOLLERR|EPOLLRDHUP|EPOLLET,pSession))
			{
				m_pLog->logLn(CLog::LL_0, "EpollService Regsite epoll I/O event error ,socketid=%d",pSession->getSocketId());
				pSession->doClose(CSession::CS_RESULT_FAILURE);
				continue;
			}
		}
	}
	else
	{
		pSession=(CSession*)pPtr;
		if(pSession==NULL)
		{
			m_pLog->logLn(CLog::LL_0, "EpollService Session is NULL ,socketid=%d",siFd);
			close(siFd);
			return;
		}
		
		if(siEvents&EPOLLRDHUP)
		{
			pSession->doClose(CSession::CS_CONNECTION_RESET);	
			return ;
		}
		else if(siEvents&EPOLLHUP)
		{
			pSession->doClose(CSession::CS_CONNECTION_RESET);	
			return ;
		}
		else if(siEvents&EPOLLERR)
		{
			m_pAsyncIoEpoll->del(siFd,siEvents);
			if(pSession->getState() == CSession::SS_CONNECTED)
			{
				pSession->doClose(CSession::CS_CONNECTION_RESET);
				m_pLog->logLn(CLog::LL_0, "EpollService Session is close error event=EPOLLHUP");
				return;
			}
		}
		else
		{
			if(pSession->getState() != CSession::SS_CONNECTED)
			{
				m_pLog->logLn(CLog::LL_0, "EpollService Session is status error status=%d,socketid=%d",pSession->getState(),siFd);
				close(siFd);
				return;
			}
			
			if(siEvents&EPOLLIN)
			{
				uint32 dwRecvBytes=MAX_PACKET_LEN;
				CSession::ESessionResult srResult;
				do{
					srResult=pSession->doRecv(dwRecvBytes);
					if(srResult==CSession::SR_OK)
					{					
						incRecvBytes(dwRecvBytes);
						srResult=pSession->addRecvBuffer(dwRecvBytes) ;
						if(srResult!= CSession::SR_OK){ // 
							pSession->m_pLog->logLn(CLog::LL_0, "EpollService Session addRecvBuffer Error srResult=%d", srResult);
							pSession->doClose(CSession::CS_BUFFER_FAILURE);
							return;
						}
						
						if((srResult = pSession->doDispatchService()) != CSession::SR_OK){ //
							if(srResult==CSession::SR_POLICY_PACK)
							{
								CSession::ESessionResult srResult=pSession->doSendPolicyMsg();
								if(srResult != CSession::SR_OK){ //
									if(srResult==CSession::SR_CONNECTION_RESET) 
									{
										pSession->doClose(CSession::CS_CONNECTION_RESET);
										return;
									}	
									pSession->doClose(CSession::CS_SEND_FAILURE);
									return;
								}
								return;
							}
							pSession->m_pLog->logLn(CLog::LL_0, "EpollService Session doDispatchService srResult= %d", srResult);
							pSession->doClose(CSession::CS_DISPATCH_FAILURE);
							
							return;
						}
					}
					else if(srResult==CSession::SR_CONNECTION_RESET)
					{
						pSession->doClose(CSession::CS_CONNECTION_RESET);	
						return ;
					}
					else if(srResult==CSession::SR_WOULD_BLOCK)
					{
						return;
					}
					else
					{
						pSession->m_pLog->logLn(CLog::LL_0, "EpollService Session doRecv error ,errno=%d", pSession->getErrorCode());
						pSession->doClose(CSession::CS_RECV_FAILURE);	
						return;
					}
				}while(true);
			}
			else if(siEvents&EPOLLOUT)
			{
				uint32 dwSendBytes;
				do{
					dwSendBytes=0;
					CSession::ESessionResult srResult=pSession->doRestSend(dwSendBytes);
					if(srResult != CSession::SR_OK){ //
						if(srResult==CSession::SR_WOULD_BLOCK) 
						{
							pSession->m_pLog->logLn(CLog::LL_2, "EpollService Session doRestSend SR_WOULD_BLOCK ");
							return;
						}
						
						if(srResult==CSession::SR_CONNECTION_RESET) 
						{
							pSession->doClose(CSession::CS_CONNECTION_RESET);
							return;
						}	
						pSession->m_pLog->logLn(CLog::LL_0, "EpollService Session doRestSend srResult= %u", srResult);
						pSession->doClose(CSession::CS_SEND_FAILURE);
						return;
					}
					incSendBytes(dwSendBytes);
				}while(dwSendBytes==MAX_PACKET_LEN);
			}
		}
	}
}

CEpollService::EEpollServiceResult CEpollService::createSocket(void){ // 
	m_pServerSocket = new CServerSocket();
	if(m_pServerSocket == NULL) return ISR_NULL;
	
	if(m_pServerSocket->create() != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		return ISR_FAILURE;
	};

	if(m_pServerSocket->bind(m_sHost, (uint16)m_uiPort) != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		return ISR_FAILURE;
	};

	if(m_pServerSocket->listen(10) != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		return ISR_FAILURE;
	};

	if(m_pServerSocket->setReuseAddress(true) != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		return ISR_FAILURE;
	};
	
	if(m_pServerSocket->setNonBlockingMode(true) != CSocketImpl::SIR_OK){ // 
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

CServerSocket* CEpollService::getServerSocket()
{
	return m_pServerSocket;
}

CEpollService::EEpollServiceResult CEpollService::createDispatchService(void){ // 
	CDispatchService* pDispatchService;
	uint32              uiIndex;

	for(uiIndex = 0; uiIndex < m_uiNumDispatch; uiIndex++){
		pDispatchService = new CSessionDispatchService(m_pLog,1,false);

		//callbackCreateDispatchService(&pDispatchService);
		if(pDispatchService == NULL) return ISR_NULL;

		m_mDispatchServices[uiIndex] = pDispatchService;
	};

	return ISR_OK;
}

CEpollService::EEpollServiceResult CEpollService::createSession(void){ // 
	CSession* pSession;
	uint32    uiIndex;

	for(uiIndex = 0; uiIndex < m_uiMaxSession; uiIndex++){
		pSession = NULL;

		callbackCreateSession(&pSession);
		if(pSession == NULL){
			return ISR_SESSION_CREATE_FAILURE;
		}

		if(pSession->initSession(uiIndex, m_pAsyncIoEpoll, m_emBufferLevel, this) != CSession::SR_OK){
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

CEpollService::EEpollServiceResult CEpollService::createService(void){ // 
	EEpollServiceResult isrResult;

	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	isrResult = createSocket();
	if(isrResult != ISR_OK) return isrResult;

	isrResult = createEpoll();
	if(isrResult != ISR_OK) return isrResult;

	isrResult = createDispatchService();
	if(isrResult != ISR_OK) return isrResult;

	isrResult = createSession();
	if(isrResult != ISR_OK) return isrResult;

	return ISR_OK;
}

CEpollService::EEpollServiceResult CEpollService::startService(void){ //
	//uint32                       uiIndex, uiNumThread;

	CDispatchService*            pDispatchService;
	EEpollServiceResult           isrResult;
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

	//uiNumThread = getThreadCount(); // SystemProcessorCount * 2 + 1

	m_pEpollAsyncIoThread = new CEpollAsyncIoThread(this); // 
	if(m_pEpollAsyncIoThread == NULL) return ISR_NULL;
	if(m_pEpollAsyncIoThread->start() != CThread::TR_OK) return ISR_THREAD_START_FAILURE;

	return ISR_OK;
}

CEpollService::EEpollServiceResult CEpollService::destroyService(void){ // 
	CDispatchService*            pDispatchService;
	//sint32                       siIndex;
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

	if(m_pAsyncIoEpoll != NULL){
		delete m_pAsyncIoEpoll; //
		m_pAsyncIoEpoll = NULL;
	}

	if(m_pServerSocket != NULL){
		delete m_pServerSocket; //
		m_pServerSocket = NULL;
	}

	return ISR_OK;
}

CEpollService::EEpollServiceResult CEpollService::stopService(bool bMode){ // 

	CDispatchService*               pDispatchService;
	EEpollServiceResult             isrResult;

	DispatchServiceMap::iterator    itr;

	if(m_siRefCount == 0) return ISR_OK; // 
	m_siRefCount--;

	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == false) return ISR_ALREADY_SERVICE_STOPPED;

	m_bServiceRunning = false;
	
	m_pEpollAsyncIoThread->stop();

	delete m_pEpollAsyncIoThread;
	m_pEpollAsyncIoThread = NULL;
	
	for(itr = m_mDispatchServices.begin(); itr != m_mDispatchServices.end(); itr++){ // 
		pDispatchService = (CDispatchService*)(*itr).second;
		pDispatchService->stopService();
	};

	if(bMode == true) onDestroyService();

	isrResult = destroyService();
	if(isrResult != ISR_OK) return isrResult;

	return ISR_OK;
}

CEpollService::~CEpollService(void){
	stopService(false); // 
}

void CEpollService::setServerName(LPTSTR szServerName){ // 
	if(szServerName != NULL) m_sServerName = szServerName;
}

TSTRING CEpollService::getServerName(void){ // 
	return m_sServerName;
}

CEpollService::EEpollServiceResult CEpollService::setProperties(CProperties* pProperties, TSTRING sServerClass){ // 
	TSTRING sHost, sBufferLevel;

	if(pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	m_pProperties = pProperties;

	m_sEpollServiceClass = sServerClass;

	m_uiPort        = m_pProperties->getIntProperty((LPTSTR)m_sEpollServiceClass.c_str(), PORT_PROPERTY);
	sHost           = m_pProperties->getStringProperty((LPTSTR)m_sEpollServiceClass.c_str(), HOST_PROPERTY);

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

	m_uiMaxSession  = m_pProperties->getIntProperty((LPTSTR)m_sEpollServiceClass.c_str(), MAX_SESSION_PROPERTY);
	m_uiNumDispatch = m_pProperties->getIntProperty((LPTSTR)m_sEpollServiceClass.c_str(), NUM_DISPATCH_PROPERTY);
	
	m_uiKeepAlive	=	m_pProperties->getIntProperty((LPTSTR)m_sEpollServiceClass.c_str(), KEEP_ALIVE);
	m_uiKeepTime	=	m_pProperties->getIntProperty((LPTSTR)m_sEpollServiceClass.c_str(), KEEP_TIME);
	m_uiKeepInterval	=	m_pProperties->getIntProperty((LPTSTR)m_sEpollServiceClass.c_str(), KEEP_INTERVAL);

	sBufferLevel    = m_pProperties->getStringProperty((LPTSTR)m_sEpollServiceClass.c_str(), BUFFER_LEVEL_PROPERTY);

	if(_tcscmp(sBufferLevel.c_str(),      _T("Large"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Small"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_SMALL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Normal"))    == 0) m_emBufferLevel = CRotativeBuffer::BL_NORMAL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VeryLarge")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VerySmall")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_SMALL;

	if(m_uiPort == 0 || m_sHost.empty() == true || m_uiMaxSession == 0) return ISR_INVALID_PROPERTY;

	if(m_uiNumDispatch == 0) m_uiNumDispatch = getThreadCount();

	return ISR_OK;
}

CProperties* CEpollService::getProperties(void){ // 
	return m_pProperties;
}

uint32 CEpollService::getErrorCode(void){
	return m_uiErrorCode;
}

void CEpollService::setErrorCode(CEpollService::EEpollServiceResult isrResult){
	m_isrResult = isrResult;
}

bool CEpollService::isServiceRunning(void){ // 
	return m_bServiceRunning;
}

CDispatchService* CEpollService::getDispatchService(void){ // 
	CDispatchService*            pDispatchService;
	DispatchServiceMap::iterator itr;

	m_uiDispatchIndex++;

	if(m_uiDispatchIndex >= m_uiNumDispatch) m_uiDispatchIndex = 0;

	itr = m_mDispatchServices.find(m_uiDispatchIndex);
	pDispatchService = (*itr).second;

	return pDispatchService;
}

CDispatchService* CEpollService::getDispatchService(uint32 uiDispatchIndex){ // 
	CDispatchService*            pDispatchService;
	DispatchServiceMap::iterator itr;

	itr = m_mDispatchServices.find(uiDispatchIndex);

	if(itr == m_mDispatchServices.end()) pDispatchService = NULL;
	else pDispatchService = (CDispatchService*)(*itr).second;

	return pDispatchService;
}

uint32 CEpollService::getPort(void){ // 
	return m_uiPort;
}

string CEpollService::getIpAddress(void){ //
	return m_sHost;
}

CEpollService::EEpollServiceResult CEpollService::setPort(uint32 uiPort){ // 
	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	if(uiPort == 0) return ISR_INVALID_PROPERTY;

	m_uiPort = uiPort;

	return ISR_OK;
}

CEpollService::EEpollServiceResult CEpollService::setIpAddress(string sHost){ // 
	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	if(sHost.empty() == true) return ISR_INVALID_PROPERTY;

	m_sHost = sHost;

	return ISR_OK;
}

CEpollService::EEpollServiceResult CEpollService::setBufferLevel(TSTRING sBufferLevel){ // 
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

uint32 CEpollService::getMaxSession(void){ // 
	return m_uiMaxSession;
}

CEpollService::EEpollServiceResult CEpollService::setMaxSession(uint32 uiMaxSession){ // 
	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	if(uiMaxSession == 0) return ISR_INVALID_PROPERTY;

	m_uiMaxSession = uiMaxSession;

	return ISR_OK;
}

uint32 CEpollService::getNumDispatch(void){ // 
	return m_uiNumDispatch;
}

CEpollService::EEpollServiceResult CEpollService::setNumDispatch(uint32 uiNumDispatch){ // 
	if(m_pProperties == NULL) return ISR_NULL;
	if(m_bServiceRunning == true) return ISR_ALREADY_SERVICE_RUNNING;

	m_uiNumDispatch = uiNumDispatch;

	if(m_uiNumDispatch == 0) m_uiNumDispatch = getThreadCount();
	
	return ISR_OK;
}

bool CEpollService::getKeepAlive(void){
	if(m_uiKeepAlive == 0) return false;
	else return true;
}

uint32 CEpollService::getKeepTime(void){
	return m_uiKeepTime;
}

uint32 CEpollService::getKeepInterval(void){
	return m_uiKeepInterval;
}

CEpollService::EEpollServiceResult CEpollService::setKeepAlive(bool bKeepAlive){
	if(bKeepAlive == false) m_uiKeepAlive = 0;
	else m_uiKeepAlive = 1;

	return ISR_OK;
}

CEpollService::EEpollServiceResult CEpollService::setKeepTime(uint32 uiKeepTime){
	m_uiKeepTime	=	uiKeepTime;

	return ISR_OK;
}

CEpollService::EEpollServiceResult CEpollService::setKeepInterval(uint32 uiKeepInterval){
	m_uiKeepInterval = uiKeepInterval;

	return ISR_OK;
}

void CEpollService::incSendBytes(DWORD dwSendBytes){
	synchronized sync(&m_cMonitor);

	m_dwSendBytes += dwSendBytes;
}

void CEpollService::incRecvBytes(DWORD dwRecvBytes){
	synchronized sync(&m_cMonitor);

	m_dwRecvBytes += dwRecvBytes;
}

DWORD CEpollService::getSendBytes(DWORD dwSeconds){
	synchronized sync(&m_cMonitor);

	DWORD dwSendBytes = m_dwSendBytes;
	m_dwSendBytes = 0;

	return dwSendBytes/dwSeconds;
}

DWORD CEpollService::getRecvBytes(DWORD dwSeconds){
	synchronized sync(&m_cMonitor);

	DWORD dwRecvBytes = m_dwRecvBytes;
	m_dwRecvBytes = 0;

	return dwRecvBytes/dwSeconds;
}
}