///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////
#include "SessionFactory.h"
#include "Session.h"
#include "EpollImpl.h"

namespace XNetFrame{
	
const char* CSession::szPolicyReq   = "<policy-file-request/>";
const char* CSession::szPolicyResp  = "<cross-domain-policy> <allow-access-from domain=\"*\" to-ports=\"*\"/></cross-domain-policy> ";

CSession::CSession(CLog* pLog){
	m_pLog        = pLog;

	m_uiSessionId = 0;
	m_uiRecvBytes = 0;
	m_uiSendBytes = 0;
	m_uiErrorCode = 0;
	m_emState = SS_DEAD;

#ifdef WIN32
	memset(&m_ovRecvOverlapped, 0, sizeof(OVERLAPPEDEX));
	memset(&m_ovSendOverlapped, 0, sizeof(OVERLAPPEDEX));

	m_ovRecvOverlapped.bIoMode = false; // 
	m_ovSendOverlapped.bIoMode = true;  // 

	m_ovRecvOverlapped.pSession = (CSession*)this;
	m_ovSendOverlapped.pSession = (CSession*)this;

	m_pSocket           = NULL;
	m_pServerSocket     = NULL;
	
#endif

	m_pRotativeBuffer   = NULL;
	m_pSessionFactory   = NULL;
	m_pDispatchService  = NULL;
	m_pAdjustableBuffer = NULL;
}

CSession::~CSession(void){
#ifdef WIN32
	if(m_pSocket != NULL){
		delete m_pSocket; //
		m_pSocket = NULL;
	}
#endif

	if(m_pRotativeBuffer != NULL){
		delete m_pRotativeBuffer;
		m_pRotativeBuffer = NULL;
	}
	if(m_pAdjustableBuffer != NULL){
		delete m_pAdjustableBuffer;
		m_pAdjustableBuffer = NULL;
	}
}

#ifdef WIN32
CSession::ESessionResult CSession::initSession(uint32 uiSessionId, CServerSocket* pServerSocket, CRotativeBuffer::LBufferLevel emBufferLevel, CSessionFactory* pSessionFactory){ // 
	DWORD dwReceivedBytes;

	if(pServerSocket == NULL || pSessionFactory == NULL) return SR_NULL;

	m_pSocket = new CSocket();
	if(m_pSocket == NULL) return SR_NULL;

	if(m_pSocket->createForOverlapped() != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pSocket->getErrorCode();
		m_pLog->logLn(CLog::LL_3, "createForOverlapped failed ErrorCode = %d, initSession", m_uiErrorCode);
		return SR_FAILURE;
	}

	m_pRotativeBuffer = new CRotativeBuffer(emBufferLevel); // 
	if(m_pRotativeBuffer == NULL) return SR_NULL;

	if(m_pRotativeBuffer->initBuffer() != CRotativeBuffer::RBR_OK) return SR_BUFFER_INIT_FAILURE; // 

	m_pAdjustableBuffer = new CAdjustableBuffer(); // 
	if(m_pAdjustableBuffer == NULL) return SR_NULL;

	if(m_pAdjustableBuffer->initBuffer() != CAdjustableBuffer::ABR_OK) return SR_BUFFER_INIT_FAILURE; // 

	m_uiSessionId     = uiSessionId;
	m_pServerSocket   = pServerSocket;
	m_pSessionFactory = pSessionFactory;

	m_sSocketId       = m_pSocket->getSocketId(); // 
	

	m_ovRecvOverlapped.uiSessionId = m_uiSessionId;
	m_ovSendOverlapped.uiSessionId = m_uiSessionId;

	if(m_pServerSocket->acceptExForOverlapped(m_pSocket, m_pRotativeBuffer->getTailRecvPtr(),
		&dwReceivedBytes, &m_ovRecvOverlapped) != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		m_pLog->logLn(CLog::LL_0, "acceptExForOverlapped failed ErrorCode = %d, initSession", m_uiErrorCode);
		return SR_FAILURE;
	};

	m_emState = SS_ACCEPTING;

	m_pSessionFactory->incCountEx();          // 
	m_pSessionFactory->incAvailableCountEx(); // 

	return SR_OK;
}
#else
CSession::ESessionResult CSession::initSession(uint32 uiSessionId, CEpollImpl* pEpollImpl,CRotativeBuffer::LBufferLevel emBufferLevel, CSessionFactory* pSessionFactory){ // 
	if(pEpollImpl == NULL) return SR_NULL;

	if(pSessionFactory == NULL) return SR_NULL;

	m_pSocket = new CSocket();
	if(m_pSocket == NULL) return SR_NULL;
	
	if(m_pSocket->create()!=CSocketImpl::SIR_OK)
		return SR_NULL;
	
	m_pRotativeBuffer = new CRotativeBuffer(emBufferLevel); // 
	if(m_pRotativeBuffer == NULL) return SR_NULL;

	if(m_pRotativeBuffer->initBuffer() != CRotativeBuffer::RBR_OK) return SR_BUFFER_INIT_FAILURE; // 

	m_pAdjustableBuffer = new CAdjustableBuffer(); // 
	if(m_pAdjustableBuffer == NULL) return SR_NULL;

	if(m_pAdjustableBuffer->initBuffer() != CAdjustableBuffer::ABR_OK) return SR_BUFFER_INIT_FAILURE; // 

	m_uiSessionId     = uiSessionId;
	m_pEpollImpl      = pEpollImpl;
	m_pSessionFactory = pSessionFactory;
	
	m_emState = SS_ACCEPTING;

	return SR_OK;
}

CSession::ESessionResult CSession::activeSession(CDispatchService* pDispatchService,uint32 uiKeepAlive,uint32 uiKeepTime,uint32 uiKeepInterval)
{
	
	CSocket* pSocket = getSocket();
	if(pSocket == NULL){
		m_pLog->logLn(CLog::LL_0, "Session socket is  null error ");
		return SR_NULL; //
	} 
	
	time_t tCurTime;
	tCurTime=time(NULL);
	
	synchronized sync(&m_cMonitor);
	
	setStartTime(tCurTime);
	
	pSocket->setTcpNodelay(true);     // 
	pSocket->setSendBufferSize(1024*24);    // 
	pSocket->setReceiveBufferSize(1024*24); // 
	pSocket->setNonBlockingMode(true);
	
	pSocket->setTcpKeepAlive(uiKeepAlive,uiKeepTime,uiKeepInterval);
	
	linger m_sLinger;
	m_sLinger.l_onoff = 1; //
	m_sLinger.l_linger = 0; //
	setsockopt(pSocket->getSocketId(),
			 SOL_SOCKET,
			 SO_LINGER,
			 (const char*)&m_sLinger,
			 sizeof(linger));

	setDispatchService(pDispatchService); //
	setState(CSession::SS_CONNECTED); 
	 
	return SR_OK;
}


#endif

void CSession::setDispatchService(CDispatchService* pDispatchService){ // 
	m_pDispatchService = pDispatchService;
}

#ifdef WIN32
CSession::ESessionResult CSession::doRecv(void){ // 

	DWORD  dwReceivedBytes=0;

	synchronized sync(&m_cMonitor);

	if(	m_emState != SS_CONNECTED ){
		m_pLog->logLn(CLog::LL_0, "invalid mode error m_emState = %d", m_emState);
		return SR_INVALID_MODE;
	}

	WSABUF wsaRecvBuf;
	wsaRecvBuf.buf = m_pRotativeBuffer->getTailRecvPtr();
	wsaRecvBuf.len = MAX_PACKET_LEN;

	if(m_pSocket->recvForOverlapped(&wsaRecvBuf, &dwReceivedBytes, &m_ovRecvOverlapped) != CSocketImpl::SIR_OK){
		m_uiErrorCode = m_pSocket->getErrorCode();
		m_pLog->logLn(CLog::LL_0, "recvForOverlapped ErrorCode = %d, doRecv", m_uiErrorCode);
		return SR_FAILURE;
	}

	return SR_OK;
}
#else
CSession::ESessionResult CSession::doRecv(uint32& uiReceivedBytes){ // 

	synchronized sync(&m_cMonitor);
	
	CSocketImpl::ESocketImplResult srResult;
	
	if(	m_emState != SS_CONNECTED ){
		m_pLog->logLn(CLog::LL_0, "invalid mode error m_emState = %d", m_emState);
		return SR_INVALID_MODE;
	}

    if((srResult=m_pSocket->recv((char*)m_pRotativeBuffer->getTailRecvPtr(),uiReceivedBytes))!= CSocketImpl::SIR_OK)
	{
		if(srResult==CSocketImpl::SIR_CONNECTION_CLOSED)
			return  SR_CONNECTION_RESET;
	    if(srResult==CSocketImpl::SIR_WOULD_BLOCK)
			return SR_WOULD_BLOCK;
			
		m_uiErrorCode = m_pSocket->getErrorCode();
		m_pLog->logLn(CLog::LL_0, "recv ErrorCode = %d, %d doRecv", m_uiErrorCode,srResult);
		return SR_FAILURE;
	}

	return SR_OK;
}

#endif

CSession::ESessionResult CSession::addRecvBuffer(uint32 uiReceivedBytes){ // 
	CRotativeBuffer::ERotativeBufferResult rbrResult;

	synchronized sync(&m_cMonitor);

	m_uiRecvBytes += uiReceivedBytes;
#ifdef LOG_LEVEL_2
	m_pLog->logLn(CLog::LL_2, "Session addRecvBuffer RemainRecvLen = %d", m_pRotativeBuffer->getRemainRecvLen());
#endif
	rbrResult = m_pRotativeBuffer->forwardTailRecvPtr(uiReceivedBytes);

	if(rbrResult == CRotativeBuffer::RBR_PACKET_BIG) return SR_PACKET_BIG;
	else if(rbrResult == CRotativeBuffer::RBR_BUFFER_OVERFLOW) return SR_BUFFER_OVERFLOW;

	return SR_OK;
}

CSession::ESessionResult CSession::doSendPolicyMsg(void)
{
	return doSend(szPolicyResp,strlen(szPolicyResp)+1);
}

CSession::ESessionResult CSession::doDispatchService(void){ // 
	uint32 uiHeaderLen, uiHeaderPos, uiBodyLen, uiLen;
	char*  pMarkRecvPtr;
	sint32 siRemainLen;

	synchronized sync(&m_cMonitor);

	uiHeaderPos = 0;
	uiHeaderLen = sizeof(uint32);

	siRemainLen = m_pRotativeBuffer->getDiffTailMarkRecvPtr(); // 

	if(siRemainLen < (sint32)uiHeaderLen) return SR_OK; // 

	while(true){
		uiLen = m_pRotativeBuffer->getDiffFinalMarkRecvPtr(); // 

		if(uiLen < uiHeaderLen) m_pRotativeBuffer->copyOriginFinalRecvBuffer(uiHeaderLen - uiLen);
		// 

		pMarkRecvPtr = m_pRotativeBuffer->getMarkRecvPtr();
		uiBodyLen=*(uint32*)pMarkRecvPtr;
		//memcpy(&uiBodyLen, pMarkRecvPtr + uiHeaderPos, sizeof(uint32)); // 

		if(uiBodyLen > MAX_PACKET_LEN){
			uint32 uiReqLen=strlen(szPolicyReq)+1;
			if(memcmp(pMarkRecvPtr,szPolicyReq,uiReqLen-1)==0)
			{
				m_pRotativeBuffer->forwardMarkRecvPtr(uiReqLen); //
				siRemainLen -= uiReqLen ;
				m_pRotativeBuffer->delMessage(uiReqLen);
				return SR_POLICY_PACK;
			}
			return SR_PACKET_BIG; // 
		}

		if(siRemainLen < (sint32)(uiBodyLen)) return SR_OK; // 

		m_pRotativeBuffer->forwardMarkRecvPtr(uiBodyLen); // 

		siRemainLen -= uiBodyLen;

		CDispatchService::EDispatchServiceResult dsrResult;
		if((dsrResult = m_pDispatchService->addObject(this, false,m_tStartTime)) != CDispatchService::DSR_OK){
			m_pLog->logLn(CLog::LL_0, "Session dispatch addObject, dsrResult = %d, doDispatchService", dsrResult);
			return SR_FAILURE; // 
		}

		if(siRemainLen < (sint32)uiHeaderLen) return SR_OK; // 
	};
}

#ifdef WIN32
CSession::ESessionResult CSession::doRestSend(uint32 uiSentBytes){ // 
	uint32 uiRestSendBytes, uiLen;
	
	synchronized sync(&m_cMonitor);

	if(m_emState == SS_DEAD || m_emState == SS_RESERVED_DOWN || m_emState == SS_SHUTTING_DOWN) return SR_INVALID_MODE;

	m_uiSendBytes += uiSentBytes;

	m_pRotativeBuffer->forwardHeadSendPtr(uiSentBytes); // 

	uiRestSendBytes = m_pRotativeBuffer->getRemainSendLen();
	uiLen            = m_pRotativeBuffer->getDiffFinalHeadSendPtr();

	//m_pLog->logLn(CLog::LL_0, "doRestSend, uiRestSendBytes = %d, uiSentBytes = %d, uiLen = %d", uiRestSendBytes, uiSentBytes, uiLen);

	if(uiRestSendBytes > 0){
		if(uiRestSendBytes > MAX_PACKET_LEN) uiRestSendBytes = MAX_PACKET_LEN;
		if(uiRestSendBytes > uiLen) uiRestSendBytes = uiLen;
		
		DWORD  dwSentBytes;
		WSABUF wsaSendBuf;
		wsaSendBuf.buf = m_pRotativeBuffer->getHeadSendPtr();
		wsaSendBuf.len = uiRestSendBytes;

		if(m_pSocket->sendForOverlapped(&wsaSendBuf, &dwSentBytes, &m_ovSendOverlapped) != CSocketImpl::SIR_OK){
			m_uiErrorCode = m_pSocket->getErrorCode();
			m_pLog->logLn(CLog::LL_0, "sendForOverlapped ErrorCode = %d, doRestSend", m_uiErrorCode);
			return SR_FAILURE;
		}
	}

	return SR_OK;
}
#else
CSession::ESessionResult CSession::doRestSend(uint32 & uiSentBytes){ // 
	uint32 uiRestSendBytes, uiLen;
	
	synchronized sync(&m_cMonitor);

	if(m_emState != SS_CONNECTED ) return SR_INVALID_MODE;

	uiRestSendBytes = m_pRotativeBuffer->getRemainSendLen();
	uiLen            = m_pRotativeBuffer->getDiffFinalHeadSendPtr();

	//m_pLog->logLn(CLog::LL_0, "doRestSend, uiRestSendBytes = %d, uiSentBytes = %d, uiLen = %d", uiRestSendBytes, uiSentBytes, uiLen);

	if(uiRestSendBytes > 0){
		if(uiRestSendBytes > MAX_PACKET_LEN) uiRestSendBytes = MAX_PACKET_LEN;
		if(uiRestSendBytes > uiLen) uiRestSendBytes = uiLen;
		CSocketImpl::ESocketImplResult sirResult;
		if((sirResult=m_pSocket->send(m_pRotativeBuffer->getHeadSendPtr(), uiRestSendBytes)) != CSocketImpl::SIR_OK){
			m_uiErrorCode = m_pSocket->getErrorCode();
			
			if(sirResult ==CSocketImpl::SIR_WOULD_BLOCK)
			{
				//m_pEpollImpl->modify(m_pSocket->getSocketId(),EPOLLIN|EPOLLOUT|EPOLLHUP|EPOLLERR|EPOLLRDHUP|EPOLLET);
				m_pLog->logLn(CLog::LL_2, "Session doRestSend SR_WOULD_BLOCK RemainSendLen=%u",m_pRotativeBuffer->getRemainSendLen());
				return SR_WOULD_BLOCK; //
			}

			if(sirResult==CSocketImpl::SIR_CONNECTION_CLOSED)
				return  SR_CONNECTION_RESET;
		
			m_pLog->logLn(CLog::LL_0, "send ErrorCode = %d, %d doRestSend", m_uiErrorCode,sirResult);
			return SR_FAILURE;
		}
		
		m_uiSendBytes += uiRestSendBytes;

		m_pRotativeBuffer->forwardHeadSendPtr(uiRestSendBytes); // 

		uiSentBytes=uiRestSendBytes;
	}
	
	return SR_OK;
}
#endif

CSession::ESessionResult CSession::doSend(const char* szBuffer, uint32 uiSentBytes){ // 
	char*  pTailSendPtr;
	
#ifdef WIN32
	DWORD  dwSentBytes;
#endif

	CRotativeBuffer::ERotativeBufferResult rbrResult;

	synchronized sync(&m_cMonitor);

	if(	m_emState != SS_CONNECTED ){
		m_pLog->logLn(CLog::LL_0, "invalid mode error m_emState = %d", m_emState);
		return SR_INVALID_MODE;
	}

	if(szBuffer == NULL || uiSentBytes == 0){
		m_pLog->logLn(CLog::LL_0, "szBuffer = %d, uiSendBytes = %d", szBuffer, uiSentBytes);
		return SR_NULL;
	}

	if(uiSentBytes > MAX_PACKET_LEN){
		m_pLog->logLn(CLog::LL_0, "uiSendBytes = %d", uiSentBytes);
		return SR_PACKET_BIG; //
	}

	rbrResult = m_pRotativeBuffer->addSendBuffer(szBuffer, uiSentBytes, &pTailSendPtr); // 

	if(rbrResult == CRotativeBuffer::RBR_PACKET_BIG) 
		return SR_PACKET_BIG;
	else if(rbrResult == CRotativeBuffer::RBR_BUFFER_OVERFLOW) 
		return SR_BUFFER_OVERFLOW;

#ifdef WIN32
	if(m_pRotativeBuffer->getRemainSendLen() == uiSentBytes)
	{
		CSocketImpl::ESocketImplResult sirResult;
		
		WSABUF wsaSendBuf;
		wsaSendBuf.buf = pTailSendPtr;
		wsaSendBuf.len = uiSentBytes;
		
		if((sirResult = m_pSocket->sendForOverlapped(&wsaSendBuf, &dwSentBytes, &m_ovSendOverlapped)) != CSocketImpl::SIR_OK){
			m_uiErrorCode = m_pSocket->getErrorCode();
			m_pLog->logLn(CLog::LL_0, "sendForOverlapped ErrorCode = %d, sirResult = %d, %d, %d, %d, %d, doSend", m_uiErrorCode, sirResult, m_pSocket->getSocketImpl(), &wsaSendBuf, &dwSentBytes, &m_ovSendOverlapped);
			return SR_FAILURE;
		}

		return SR_OK;
	}
	
	m_pLog->logLn(CLog::LL_0, "[%d]would block RemainSendLen(%d) SentBytes(%d)", m_uiSessionId, m_pRotativeBuffer->getRemainSendLen(), uiSentBytes);

	return SR_WOULD_BLOCK; // 
	
#else
	uint32 uiRestSendBytes, uiLen;
	
	uiRestSendBytes  = m_pRotativeBuffer->getRemainSendLen();
	uiLen             = m_pRotativeBuffer->getDiffFinalHeadSendPtr();

	//m_pLog->logLn(CLog::LL_0, "doRestSend, uiRestSendBytes = %d, uiSentBytes = %d, uiLen = %d", uiRestSendBytes, uiSentBytes, uiLen);

	if(uiRestSendBytes > 0){
		if(uiRestSendBytes > MAX_PACKET_LEN) uiRestSendBytes = MAX_PACKET_LEN;
		if(uiRestSendBytes > uiLen) uiRestSendBytes = uiLen;
		CSocketImpl::ESocketImplResult sirResult;
		if((sirResult=m_pSocket->send(m_pRotativeBuffer->getHeadSendPtr(), uiRestSendBytes)) != CSocketImpl::SIR_OK){
			m_uiErrorCode = m_pSocket->getErrorCode();
			if(sirResult ==CSocketImpl::SIR_WOULD_BLOCK)
			{
				//m_pEpollImpl->modify(m_pSocket->getSocketId(), EPOLLIN|EPOLLOUT|EPOLLHUP|EPOLLERR|EPOLLRDHUP|EPOLLET);
				m_pLog->logLn(CLog::LL_2, "Session doSend SR_WOULD_BLOCK RemainSendLen=%u",m_pRotativeBuffer->getRemainSendLen());
				return SR_WOULD_BLOCK; //
			}
			else if(sirResult==CSocketImpl::SIR_CONNECTION_CLOSED)
				return  SR_CONNECTION_RESET;
			else	
				
			m_pLog->logLn(CLog::LL_0, "send ErrorCode = %d, %d doSend", m_uiErrorCode,sirResult);
			return SR_FAILURE;
		}
		
		m_uiSendBytes += uiRestSendBytes;

		m_pRotativeBuffer->forwardHeadSendPtr(uiRestSendBytes); // 
	}
	
	return SR_OK;
	
#endif
}

CSession::ESessionResult CSession::doClose(SCloseStatus csStatus){
	MS_CLOSE_SESSION sMS_CLOSE_SESSION;

	sMS_CLOSE_SESSION.m_uiLen	=	(uint32)sizeof(MS_CLOSE_SESSION);
	sMS_CLOSE_SESSION.m_uiMessageId	=	CDispatchService::SM_CLOSE_SESSION;
	sMS_CLOSE_SESSION.m_uiState	=	(uint32)csStatus;

	m_pLog->logLn(CLog::LL_0, "[%u] add doClose message (state = %d,CloseState = %d)", m_uiSessionId,m_emState,csStatus);

	return addMessageEx((char*)&sMS_CLOSE_SESSION, sMS_CLOSE_SESSION.m_uiLen);
}

CSession::ESessionResult CSession::realClose(SCloseStatus csStatus){ // 
#ifdef WIN32
	DWORD dwReceivedBytes;
#endif
	//synchronized sync(&m_cMonitor); // 

	if(onClose(csStatus) == false){
		synchronized sync(&m_cMonitor);
		
		m_emState = SS_RESERVED_DOWN;

		return SR_OK;
	};
	
	m_pLog->logLn(CLog::LL_0, "[%u] realClose message (Stat = %d,CloseState = %d)", m_uiSessionId,m_emState,csStatus);
	
	synchronized sync(&m_cMonitor);
	
	if(m_emState == SS_DEAD || m_emState == SS_SHUTTING_DOWN) return SR_OK; // 

	m_emState = SS_SHUTTING_DOWN;

	m_uiErrorCode=0;
	
	m_pSocket->close();

	m_uiRecvBytes = 0;
	m_uiSendBytes = 0;

	m_emState = SS_DEAD;

#ifdef WIN32
	if(m_pSocket->createForOverlapped() != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pSocket->getErrorCode();
		m_pLog->logLn(CLog::LL_0, "createForOverlapped ErrorCode = %d, realClose", m_uiErrorCode);
		return SR_FAILURE;
	};
#else

	m_pSessionFactory->putAuailableSession(this); //

	if(m_pSocket->create()!=CSocketImpl::SIR_OK)
		return SR_NULL;
#endif

	if(m_pRotativeBuffer->initBuffer() != CRotativeBuffer::RBR_OK) return SR_BUFFER_INIT_FAILURE; // 

	if(m_pAdjustableBuffer->initBuffer() != CAdjustableBuffer::ABR_OK) return SR_BUFFER_INIT_FAILURE; // 

#ifdef WIN32
	m_sSocketId = m_pSocket->getSocketId(); // 
	if(m_pServerSocket->acceptExForOverlapped(m_pSocket, m_pRotativeBuffer->getTailRecvPtr(),
		&dwReceivedBytes, &m_ovRecvOverlapped) != CSocketImpl::SIR_OK){ // 
		m_uiErrorCode = m_pServerSocket->getErrorCode();
		m_pLog->logLn(CLog::LL_0, "acceptExForOverlapped ErrorCode = %d, realClose", m_uiErrorCode);
		return SR_FAILURE;
	};
#endif

	m_emState = SS_ACCEPTING;

	return SR_OK;
}

// 

CSession::ESessionResult CSession::addMessageEx(char* szBuffer, uint32 uiReceivedBytes){ // 
	ESessionResult srResult;

	srResult = addRecvBufferEx(szBuffer, uiReceivedBytes); // 
	if(srResult != SR_OK) return srResult;

	return doDispatchServiceEx();  // 
}

CSession::ESessionResult CSession::addRecvBufferEx(char* szBuffer, uint32 uiReceivedBytes){ // 
	CAdjustableBuffer::EAdjustableBufferResult abrResult;

	synchronized sync(&m_cMonitor);

	if(m_emState == SS_DEAD || m_emState == SS_SHUTTING_DOWN) return SR_INVALID_MODE;

	if(szBuffer == NULL || uiReceivedBytes == 0) return SR_NULL;

	abrResult = m_pAdjustableBuffer->addRecvBuffer(szBuffer, uiReceivedBytes);

	if(abrResult == CAdjustableBuffer::ABR_PACKET_BIG) return SR_PACKET_BIG;
	else if(abrResult == CAdjustableBuffer::ABR_BUFFER_OVERFLOW) return SR_BUFFER_OVERFLOW;

	return SR_OK;
}

CSession::ESessionResult CSession::doDispatchServiceEx(void){ // 
	uint32 uiHeaderLen, uiHeaderPos, uiBodyLen, uiLen;
	char*  pMarkRecvPtr;
	sint32 siRemainLen;

	synchronized sync(&m_cMonitor);

	uiHeaderPos = 0;
	uiHeaderLen = sizeof(uint32);

	siRemainLen = m_pAdjustableBuffer->getDiffTailMarkRecvPtr(); // 

	if(siRemainLen < (sint32)uiHeaderLen) return SR_OK; // 

	while(true){
		uiLen = m_pAdjustableBuffer->getDiffFinalMarkRecvPtr(); // 

		if(uiLen < uiHeaderLen) m_pAdjustableBuffer->copyOriginFinalRecvBuffer(uiHeaderLen - uiLen);
		// 

		pMarkRecvPtr = m_pAdjustableBuffer->getMarkRecvPtr();
		memcpy(&uiBodyLen, pMarkRecvPtr + uiHeaderPos, sizeof(uint32)); // 

		if(uiBodyLen > MAX_PACKET_LEN) return SR_PACKET_BIG; // 

		if(siRemainLen < (sint32)(uiBodyLen)) return SR_OK; // 

		m_pAdjustableBuffer->forwardMarkRecvPtr(uiBodyLen); // 

		siRemainLen -= uiBodyLen;

		if(m_pDispatchService->addObject(this, true,m_tStartTime) != CDispatchService::DSR_OK){
			m_pLog->logLn(CLog::LL_3, "addObject failed, doDispatchServiceEx");
			return SR_FAILURE; //
		}

		if(siRemainLen < (sint32)uiHeaderLen) return SR_OK; // 
	}
}

uint32 CSession::getSessionId(void){ // 
	return m_uiSessionId;
}

CSocket* CSession::getSocket(void){ // 
	return m_pSocket;
}

SOCKET CSession::getSocketId(void) const{ // 
	return m_pSocket->getSocketId();
}

CSession::SSessionState CSession::getState(void){ // 
	return m_emState;
}

void CSession::setState(CSession::SSessionState emState){ // 
	m_emState = emState;
}

bool CSession::isAvailable(void){ // 
	return (m_emState == SS_ACCEPTING)? true : false;
}

uint32 CSession::getErrorCode(void){ // 
	return m_uiErrorCode;
}

uint32 CSession::getRecvBytes(void){ // 
	return m_uiRecvBytes;
}

uint32 CSession::getSendBytes(void){ // 
	return m_uiSendBytes;
}

CRotativeBuffer* CSession::getRotativeBuffer(void){ // 
	return m_pRotativeBuffer;
}

void CSession::setLocalAddress(sockaddr_in* psaLocalAddress){ // 
	if(m_pSocket != NULL && psaLocalAddress != NULL) m_pSocket->setLocalAddress((sockaddr_in*)psaLocalAddress);
}

void CSession::setRemoteAddress(sockaddr_in* psaRemoteAddress){ // 
	if(m_pSocket != NULL && psaRemoteAddress != NULL) m_pSocket->setRemoteAddress((sockaddr_in*)psaRemoteAddress);
}

CInetAddress* CSession::getRemoteAddress(void){ // 

	if(m_pSocket == NULL) return NULL;
	return m_pSocket->getAddress();
}

CSession::ESessionResult CSession::getMessage(uint32* uiMessageId, char** szMessage){ // 
	CRotativeBuffer::ERotativeBufferResult rbrResult;

	synchronized sync(&m_cMonitor);

	rbrResult = m_pRotativeBuffer->getMessage(uiMessageId, szMessage);
	
	if(rbrResult == CRotativeBuffer::RBR_FAILURE){
		m_pLog->logLn(CLog::LL_3, "getMessage failed");
		return SR_FAILURE;
	}

	return SR_OK;
}

CSession::ESessionResult CSession::getMessageEx(uint32* uiMessageId, char** szMessage){ // 
	CAdjustableBuffer::EAdjustableBufferResult abrResult;

	synchronized sync(&m_cMonitor);

	abrResult = m_pAdjustableBuffer->getMessage(uiMessageId, szMessage);
	
	if(abrResult != CAdjustableBuffer::ABR_OK){
		m_pLog->logLn(CLog::LL_0, "abrResult = %d", abrResult);
		return SR_FAILURE;
	}

	return SR_OK;
}

}
