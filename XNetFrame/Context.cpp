///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "Context.h"

namespace XNetFrame{

CContext::CContext(CLog* pLog) : CUdpSocket(){
	m_pLog    = pLog;

	m_emState = CS_DEAD;

	m_pRotativeBuffer  = NULL;
	m_pDispatchService = NULL;
}

CContext::~CContext(void){
	if(m_pRotativeBuffer != NULL){
		delete m_pRotativeBuffer;
		m_pRotativeBuffer = NULL;
	}
}

CContext::EContextResult CContext::initContext(string& sHost, uint32 uiPort, CDispatchService* pDispatchService, CRotativeBuffer::LBufferLevel emBufferLevel){ // 
	if(uiPort == 0 || pDispatchService == NULL) return CR_NULL;

	m_sHost            = sHost;
	m_uiPort           = uiPort;
	m_pDispatchService = pDispatchService;

	if(CUdpSocket::create(m_sHost, m_uiPort) != CSocketImpl::SIR_OK) return CR_FAILURE; // 

	m_pRotativeBuffer = new CRotativeBuffer(emBufferLevel); // 
	if(m_pRotativeBuffer == NULL) return CR_NULL;

	if(m_pRotativeBuffer->initBuffer() != CRotativeBuffer::RBR_OK) return CR_BUFFER_INIT_FAILURE; // 

	// bind to INADDR_ANY, if m_sHost is empty.
	// and should do connect before do send, see doConnect function.
	// bind to a fixed ip do not requre do connect.
	if(!m_sHost.empty())	m_emState = CS_CONNECTED;

	return CR_OK;
}

void CContext::onRecv(uint32 uiErrorCode){ // 
	uint32 uiReceivedBytes, uiMessageLen;
	char   szRecvBuf[1024];
	char*  pTailRecvPtr;

	CSocketImpl::ESocketImplResult sirResult;

	if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET) return; // 
	
	if(uiErrorCode != 0) return; // 

	if(m_emState == CS_DEAD || m_emState == CS_SHUTTING_DOWN) return;

	if(m_pRotativeBuffer->isOverflowRecvBuffer(1024) == true || m_pDispatchService->isOverflowObjectQueue(16) == true){ // 
		CUdpSocket::recvFrom(szRecvBuf, uiReceivedBytes, &m_iaInetAddress); // 

		return;
	};

	uiReceivedBytes = MAX_PACKET_LEN;                                // 
	pTailRecvPtr    = m_pRotativeBuffer->getTailRecvPtr(); // 

	sirResult = CUdpSocket::recvFrom(pTailRecvPtr, uiReceivedBytes, &m_iaInetAddress);

	if(sirResult == CSocketImpl::SIR_CONNECTION_CLOSED) return; // 

	if(sirResult != CSocketImpl::SIR_OK && sirResult != CSocketImpl::SIR_WOULD_BLOCK){
		uiErrorCode = CUdpSocket::getErrorCode();

		if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET) return; // 

		if(uiErrorCode != 0) return; // 
	};

	if(uiReceivedBytes == 0) return;

	uiMessageLen = m_pRotativeBuffer->getMessageLen(pTailRecvPtr); // 
	if(uiMessageLen != uiReceivedBytes) return; // 

	if(addRecvBuffer(uiReceivedBytes, &m_iaInetAddress) != CR_OK){ // 
		return;
	};

	doDispatchService(); // 
}

CContext::EContextResult CContext::addRecvBuffer(uint32 uiReceivedBytes,
												 CInetAddress* pInetAddress){ // 
	CRotativeBuffer::ERotativeBufferResult rbrResult;

	synchronized sync(&m_cMonitor);

	rbrResult = m_pRotativeBuffer->forwardTailRecvPtr(uiReceivedBytes); // 

	if(rbrResult == CRotativeBuffer::RBR_PACKET_BIG) return CR_PACKET_BIG;
	else if(rbrResult == CRotativeBuffer::RBR_BUFFER_OVERFLOW) return CR_BUFFER_OVERFLOW;

	rbrResult = m_pRotativeBuffer->addRecvBuffer(pInetAddress);// 

	if(rbrResult == CRotativeBuffer::RBR_PACKET_BIG) return CR_PACKET_BIG;
	else if(rbrResult == CRotativeBuffer::RBR_BUFFER_OVERFLOW) return CR_BUFFER_OVERFLOW;

	return CR_OK;
}

CContext::EContextResult CContext::doDispatchService(void){ // 
	uint32 uiHeaderLen, uiHeaderPos, uiBodyLen, uiLen, uiInetAddressLen;
	char*  pMarkRecvPtr;
	sint32 siRemainLen;

	synchronized sync(&m_cMonitor);

	uiHeaderPos = 0;
	uiHeaderLen = sizeof(uint32);

	uiInetAddressLen = sizeof(sockaddr_in);

	siRemainLen = m_pRotativeBuffer->getDiffTailMarkRecvPtr(); // 

	if(siRemainLen < (sint32)uiHeaderLen) return CR_OK; // 

	while(true){
		uiLen = m_pRotativeBuffer->getDiffFinalMarkRecvPtr(); // 

		if(uiLen < uiHeaderLen) m_pRotativeBuffer->copyOriginFinalRecvBuffer(uiHeaderLen - uiLen);
		// 

		pMarkRecvPtr = m_pRotativeBuffer->getMarkRecvPtr();
		memcpy(&uiBodyLen, pMarkRecvPtr + uiHeaderPos, sizeof(uint32)); // 

		if(uiBodyLen > MAX_PACKET_LEN) return CR_PACKET_BIG; // 

		if(siRemainLen < (sint32)(uiBodyLen)) return CR_OK; // 

		m_pRotativeBuffer->forwardMarkRecvPtr(uiBodyLen + uiInetAddressLen); // 

		siRemainLen = siRemainLen - uiBodyLen - uiInetAddressLen;

		if(m_pDispatchService->addObject(this, false,0) != CDispatchService::DSR_OK) return CR_FAILURE; // 

		if(siRemainLen < (sint32)uiHeaderLen) return CR_OK; // 
	};
}

void CContext::onSend(uint32 uiErrorCode){ // 
	if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET) return; // 

	if(uiErrorCode != 0) return; // 
}

CContext::EContextResult CContext::doConnect(string& sHost, uint32 uiPort){ // 
	uint32 uiErrorCode;

	CSocketImpl::ESocketImplResult sirResult;

	if(m_emState == CS_CONNECTED || m_emState == CS_SHUTTING_DOWN) return CR_INVALID_MODE;

	sirResult = CUdpSocket::connect(sHost, uiPort);
	if(sirResult != CSocketImpl::SIR_OK && sirResult != CSocketImpl::SIR_WOULD_BLOCK){
		uiErrorCode = CUdpSocket::getErrorCode();

		if(uiErrorCode == WSAEALREADY || uiErrorCode == WSAEISCONN) return CR_OK; // 

		if(uiErrorCode == WSAENETUNREACH){ // 
			doClose(CS_UNREACHABLE_FAILURE);
			return CR_FAILURE;
		}

		if(uiErrorCode == WSAETIMEDOUT){ // 
			doClose(CS_TIMEOUT_FAILURE);
			return CR_FAILURE;
		}

		if(uiErrorCode == WSAECONNREFUSED){ // 
			doClose(CS_CONNECTION_REFUSE);
			return CR_FAILURE;
		}

		if(uiErrorCode == WSAEADDRNOTAVAIL){ // 
			doClose(CS_INVALID_ADDRESS);
			return CR_FAILURE;
		}

		if(uiErrorCode != 0){ // 
			doClose(CS_SYSTEM_FAILURE);
			return CR_FAILURE;
		}
	}

	m_emState = CS_CONNECTED;

	return CR_OK;
}

CContext::EContextResult CContext::doSend(char* szBuffer, uint32 uiSendBytes, CInetAddress* pInetAddress){ // 
	uint32 uiSentBytes, uiErrorCode;
	char*  pTailSendPtr;

	CRotativeBuffer::ERotativeBufferResult rbrResult;
	CSocketImpl::ESocketImplResult         sirResult;

	synchronized sync(&m_cMonitor);

	if(szBuffer == NULL || uiSendBytes == 0 || pInetAddress == NULL) return CR_NULL;

	if(m_emState == CS_DEAD || m_emState == CS_SHUTTING_DOWN) return CR_INVALID_MODE;

	if(m_pRotativeBuffer->isOverflowSendBuffer(1024) == true) return CR_BUFFER_OVERFLOW; // 

	rbrResult = m_pRotativeBuffer->addSendBuffer(szBuffer, uiSendBytes, &pTailSendPtr); // 
	
	if(rbrResult == CRotativeBuffer::RBR_PACKET_BIG) return CR_PACKET_BIG;
	else if(rbrResult == CRotativeBuffer::RBR_BUFFER_OVERFLOW) return CR_BUFFER_OVERFLOW;

	uiSentBytes = uiSendBytes;

	sirResult = CUdpSocket::sendTo(pTailSendPtr, uiSentBytes, pInetAddress);
	
	if(sirResult != CSocketImpl::SIR_OK && sirResult != CSocketImpl::SIR_WOULD_BLOCK){
		uiErrorCode = CUdpSocket::getErrorCode();

		if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET) return CR_FAILURE; // 

		if(uiErrorCode != 0) return CR_FAILURE; // 
	};

	if(uiSentBytes != 0 && uiSendBytes != uiSentBytes) return CR_FAILURE; // 

	m_pRotativeBuffer->forwardHeadSendPtr(uiSendBytes); // 
	
	return CR_OK;
}

CContext::EContextResult CContext::doClose(SCloseStatus csStatus){ // 
	
	//synchronized sync(&m_cMonitor); // lishu

	if(m_emState == CS_DEAD || m_emState == CS_SHUTTING_DOWN) return CR_OK; // 

	m_emState = CS_SHUTTING_DOWN;

	CUdpSocket::close(); // 

	synchronized sync(&m_cMonitor);

	m_emState = CS_DEAD;

	if(CUdpSocket::create(m_sHost, m_uiPort) != CSocketImpl::SIR_OK) return CR_FAILURE; // 

	if(m_pRotativeBuffer->initBuffer() != CRotativeBuffer::RBR_OK) return CR_BUFFER_INIT_FAILURE; // 

	m_emState = CS_CONNECTED;

	return CR_OK;
}

CContext::SContextState CContext::getState(void){ // 
	return m_emState;
}

void CContext::setState(CContext::SContextState emState){ // 
	m_emState = emState;
}

CContext::EContextResult CContext::getMessage(uint32* uiMessageId, char** szMessage, CInetAddress* pInetAddress){ // 
	CRotativeBuffer::ERotativeBufferResult rbrResult;

	synchronized sync(&m_cMonitor);

	rbrResult = m_pRotativeBuffer->getMessage(uiMessageId, szMessage, pInetAddress);
	
	if(rbrResult != CRotativeBuffer::RBR_OK){
		m_pLog->logLn(CLog::LL_0, "rbrResult = %d", rbrResult);
		return CR_FAILURE;
	}

	return CR_OK;
}

}
