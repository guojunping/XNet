///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "TcpService.h"
#include "TcpThread.h"
#include "ChannelDispatchService.h"

namespace XNetFrame{

CTcpService::CTcpService(CLog* pLog){
	m_pLog               = pLog;

	m_uiPort             = 0;
	m_siRefCount         = 0;

	m_pChannel           = NULL;
	m_pProperties        = NULL;
	m_pDispatchService   = NULL;

	m_uiKeepAlive	=	0;
	m_uiKeepTime	=	0;
	m_uiKeepInterval	=	0;

	m_bServiceRunning    = false;

	m_sTcpServiceClass   = _T("");
	m_sPortProperty = _T("");
	m_sHostProperty = _T("");
	m_sRotativeBufferProperty = _T("");

	m_emBufferLevel      = CRotativeBuffer::BL_NORMAL;
}

CTcpService::~CTcpService(void){
	stopService();
}

CTcpService::ETcpServiceResult CTcpService::createService(void){ // 
	ETcpServiceResult tsrResult;

	if(m_pProperties == NULL) return TSR_NULL;
	if(m_bServiceRunning == true) return TSR_ALREADY_SERVICE_RUNNING;

	tsrResult = createDispatchService();
	if(tsrResult != TSR_OK) return tsrResult;

	tsrResult = createChannel();
	if(tsrResult != TSR_OK) return tsrResult;

	return TSR_OK;
}

CTcpService::ETcpServiceResult CTcpService::createDispatchService(void){ // 
	m_pDispatchService = NULL;

	m_pDispatchService=new CChannelDispatchService(m_pLog, 0, false);
	if(m_pDispatchService == NULL) return TSR_NULL;

	return TSR_OK;
}

CTcpService::ETcpServiceResult CTcpService::createChannel(void){ // 
	m_pChannel = NULL;

	callbackCreateChannel(&m_pChannel);
	if(m_pChannel == NULL) return TSR_CHANNEL_CREATE_FAILURE;

	if(m_pChannel->initChannel(m_pDispatchService, m_emBufferLevel) != CChannel::CR_OK){
		m_uiErrorCode = m_pChannel->getErrorCode();
		return TSR_CHANNEL_INIT_FAILURE;
	};

	if(m_pChannel->setTcpKeepAlive(m_uiKeepAlive, m_uiKeepTime, m_uiKeepInterval) != CSocketImpl::SIR_OK){
		m_uiErrorCode = m_pChannel->getErrorCode();
		return TSR_CHANNEL_SET_OPTION_FAILURE;
	}

#ifdef SOCKET_LEVEL
	m_pChannel->setLog(m_pLog);
#endif

	return TSR_OK;
}

CTcpService::ETcpServiceResult CTcpService::startService(void){ // 
	CTcpThread*       pTcpThread;
	ETcpServiceResult tsrResult;

	CDispatchService::EDispatchServiceResult dsrResult;

	m_siRefCount++;

	tsrResult = createService();
	if(tsrResult != TSR_OK) return tsrResult;

	setErrorCode(TSR_OK);

	onCreateService();

	if(m_tsrResult != TSR_OK) return m_tsrResult;

	pTcpThread = new CTcpThread(this); // 
	if(pTcpThread == NULL) return TSR_NULL;

    m_liTcpThreadList.push_back((TcpThreadList::value_type)pTcpThread);

	if(m_pChannel->doConnect(m_sHost, m_uiPort) != CChannel::CR_OK){ // 
		m_uiErrorCode = m_pChannel->getErrorCode();
		return TSR_CHANNEL_CONNECT_FAILURE;
	};
	
	m_bServiceRunning = true;

	dsrResult = m_pDispatchService->startService();

	if(dsrResult == CDispatchService::DSR_NULL) return TSR_NULL;
	if(dsrResult == CDispatchService::DSR_THREAD_START_FAILURE) return TSR_THREAD_START_FAILURE;
	
	if(pTcpThread->start() != CThread::TR_OK) return TSR_THREAD_START_FAILURE;

	return TSR_OK;
}

CTcpService::ETcpServiceResult CTcpService::startService(string& sHost, uint16 uiPort){ // 
	CTcpThread*       pTcpThread;
	ETcpServiceResult tsrResult;

	CDispatchService::EDispatchServiceResult dsrResult;

	m_siRefCount++;

	tsrResult = createService();
	if(tsrResult != TSR_OK) return tsrResult;

	setErrorCode(TSR_OK);

	if(m_pChannel->bind(sHost, uiPort) != CSocketImpl::SIR_OK) return TSR_CHANNEL_BIND_FAILURE;
	
	if(m_pChannel->setOption(SO_REUSEADDR, true) != CSocketImpl::SIR_OK) return TSR_CHANNEL_SET_OPTION_FAILURE;
#ifdef WIN32
	if(m_pChannel->setOption(SO_DONTLINGER, false) != CSocketImpl::SIR_OK) return TSR_CHANNEL_SET_OPTION_FAILURE;
#endif
	onCreateService();

	if(m_tsrResult != TSR_OK) return m_tsrResult;

	m_bServiceRunning = true;

	dsrResult = m_pDispatchService->startService();

	if(dsrResult == CDispatchService::DSR_NULL) return TSR_NULL;
	if(dsrResult == CDispatchService::DSR_THREAD_START_FAILURE) return TSR_THREAD_START_FAILURE;

	pTcpThread = new CTcpThread(this); // 
	if(pTcpThread == NULL) return TSR_NULL;

	m_liTcpThreadList.push_back((TcpThreadList::value_type)pTcpThread);

	pTcpThread->setPriority(CThread::TP_NORMAL);

	if(pTcpThread->start() != CThread::TR_OK) return TSR_THREAD_START_FAILURE;

	if(m_pChannel->doConnect(m_sHost, m_uiPort) != CChannel::CR_OK){ // 
		m_uiErrorCode = m_pChannel->getErrorCode();
		return TSR_CHANNEL_CONNECT_FAILURE;
	};

	return TSR_OK;
}

void CTcpService::setErrorCode(CTcpService::ETcpServiceResult tsrResult){
	m_tsrResult = tsrResult;
}

CTcpService::ETcpServiceResult CTcpService::stopService(void){ // 
	CTcpThread*             pTcpThread;
	ETcpServiceResult       tsrResult;
	TcpThreadList::iterator itr;

	if(m_siRefCount == 0) return TSR_OK; // 
	
	m_siRefCount--;

	if(m_pProperties == NULL) return TSR_NULL;
	if(m_bServiceRunning == false) return TSR_ALREADY_SERVICE_STOPPED;

	m_bServiceRunning = false;

	for(itr = m_liTcpThreadList.begin(); itr != m_liTcpThreadList.end(); itr++){ // 
		pTcpThread = (CTcpThread*)(*itr);
#ifdef WIN32
		pTcpThread->stop(m_pChannel->getIoEvent()); // 
#else
	    pTcpThread->stop(NULL); // 
#endif
		delete pTcpThread;
		pTcpThread = NULL;
	}

	itr = m_liTcpThreadList.begin();
	if(itr != m_liTcpThreadList.end()) m_liTcpThreadList.erase(itr, m_liTcpThreadList.end());
	
	m_pDispatchService->stopService(); // 

	onDestroyService();

	tsrResult = destroyService();
	if(tsrResult != TSR_OK) return tsrResult;

	return TSR_OK;
}

CTcpService::ETcpServiceResult CTcpService::destroyService(void){ // 
	if(m_bServiceRunning == true) return TSR_ALREADY_SERVICE_RUNNING;

	if(m_pDispatchService != NULL){
		delete m_pDispatchService; // 
		m_pDispatchService = NULL;
	}

	if(m_pChannel != NULL){
		m_pChannel->doClose(CChannel::CS_SERVICE_STOP); // 
		delete m_pChannel;       
		m_pChannel = NULL;// 
	};

	return TSR_OK;
}

CTcpService::ETcpServiceResult CTcpService::setProperties(CProperties* pProperties, TSTRING sServerProperty){ // 
	TSTRING sHost, sBufferLevel;
	
	if(pProperties == NULL) return TSR_NULL;
	if(m_bServiceRunning == true) return TSR_ALREADY_SERVICE_RUNNING;

	m_pProperties = pProperties;

	m_sTcpServiceClass = TCP_SERVICE_CLASS;

	m_sPortProperty = sServerProperty + _T("_") + PORT_PROPERTY;
	m_sHostProperty = sServerProperty + _T("_") + HOST_PROPERTY;

	m_uiPort     = m_pProperties->getIntProperty((LPTSTR)m_sTcpServiceClass.c_str(), (LPTSTR)m_sPortProperty.c_str());
	sHost			= m_pProperties->getStringProperty((LPTSTR)m_sTcpServiceClass.c_str(), (LPTSTR)m_sHostProperty.c_str());

	m_uiKeepAlive	=	m_pProperties->getIntProperty((LPTSTR)m_sTcpServiceClass.c_str(), KEEP_ALIVE);
	m_uiKeepTime	=	m_pProperties->getIntProperty((LPTSTR)m_sTcpServiceClass.c_str(), KEEP_TIME);
	m_uiKeepInterval	=	m_pProperties->getIntProperty((LPTSTR)m_sTcpServiceClass.c_str(), KEEP_INTERVAL);

#ifdef _UNICODE
	char szHost[64];

	if(::WideCharToMultiByte(CP_ACP, 0, sHost.c_str(), -1, szHost, 64, NULL, NULL) == 0) return TSR_NULL;
	m_sHost = szHost;
#else
	m_sHost = sHost;
#endif

	m_sRotativeBufferProperty = sServerProperty + _T("_") + BUFFER_LEVEL_PROPERTY;

	sBufferLevel = m_pProperties->getStringProperty((LPTSTR)m_sTcpServiceClass.c_str(), (LPTSTR)m_sRotativeBufferProperty.c_str());

	if(_tcscmp(sBufferLevel.c_str(),      _T("Large"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Small"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_SMALL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Normal"))    == 0) m_emBufferLevel = CRotativeBuffer::BL_NORMAL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VeryLarge")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VerySmall")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_SMALL;

	if(m_uiPort == 0 || m_sHost.empty() == true){
		//return TSR_INVALID_PROPERTY;
		m_uiPort = 1;
		m_sHost = _T("192.168.0.1");
	}
	if(sBufferLevel.empty() == true) m_emBufferLevel = CRotativeBuffer::BL_VERY_LARGE;

	return TSR_OK;
}

CChannel* CTcpService::getChannel(void){ // 
	return m_pChannel;
}

uint32 CTcpService::getErrorCode(void){
	return m_uiErrorCode;
}

uint32 CTcpService::getPort(void){ // 
	return m_uiPort;
}

string CTcpService::getIpAddress(void){ // 
	return m_sHost;
}

CTcpService::ETcpServiceResult CTcpService::setPort(uint32 uiPort){ // 
	if(m_pProperties == NULL) return TSR_NULL;
	if(m_bServiceRunning == true) return TSR_ALREADY_SERVICE_RUNNING;

	if(uiPort == 0) return TSR_INVALID_PROPERTY;

	m_uiPort = uiPort;

	return TSR_OK;
}

CTcpService::ETcpServiceResult CTcpService::setIpAddress(string sHost){ // 
	if(m_pProperties == NULL) return TSR_NULL;
	if(m_bServiceRunning == true) return TSR_ALREADY_SERVICE_RUNNING;

	if(sHost.empty() == true) return TSR_INVALID_PROPERTY;

	m_sHost = sHost;

	return TSR_OK;
}

CTcpService::ETcpServiceResult CTcpService::setBufferLevel(TSTRING sBufferLevel){ // 
	if(m_pProperties == NULL) return TSR_NULL;
	if(m_bServiceRunning == true) return TSR_ALREADY_SERVICE_RUNNING;

	if(_tcscmp(sBufferLevel.c_str(),      _T("Large"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Small"))     == 0) m_emBufferLevel = CRotativeBuffer::BL_SMALL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("Normal"))    == 0) m_emBufferLevel = CRotativeBuffer::BL_NORMAL;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VeryLarge")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_LARGE;
	else if(_tcscmp(sBufferLevel.c_str(), _T("VerySmall")) == 0) m_emBufferLevel = CRotativeBuffer::BL_VERY_SMALL;
	else return TSR_INVALID_PROPERTY;

	return TSR_OK;
}

bool CTcpService::isServiceRunning(void){ // 
	return m_bServiceRunning;
}

}
