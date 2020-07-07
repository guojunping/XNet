///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "Channel.h"

namespace XNetFrame{

CChannel::CChannel(CLog* pLog) : CTcpSocket(){
	m_pLog    = pLog;

	m_emState = CS_DEAD;

	m_pRotativeBuffer  = NULL;
	m_pDispatchService = NULL;
}

CChannel::~CChannel(void){
	if(m_pRotativeBuffer != NULL){
		delete m_pRotativeBuffer;
		m_pRotativeBuffer = NULL;
	}
}

CChannel::EChannelResult CChannel::initChannel(CDispatchService* pDispatchService, CRotativeBuffer::LBufferLevel emBufferLevel){ // 
	if(pDispatchService == NULL) return CR_NULL;

	m_pDispatchService = pDispatchService;

	if(CTcpSocket::create() != CSocketImpl::SIR_OK) return CR_FAILURE; // 

	m_pRotativeBuffer = new CRotativeBuffer(emBufferLevel); // 
	if(m_pRotativeBuffer == NULL) return CR_NULL;

	if(m_pRotativeBuffer->initBuffer() != CRotativeBuffer::RBR_OK) return CR_BUFFER_INIT_FAILURE; // 

	m_emState = CS_DEAD;

	return CR_OK;
}

CChannel::EChannelResult CChannel::doConnect(string& sHost, uint32 uiPort){ // 
	uint32 uiErrorCode;

	CSocketImpl::ESocketImplResult sirResult;

	if(m_emState == CS_CONNECTED || m_emState == CS_SHUTTING_DOWN) return CR_INVALID_MODE;

	CTcpSocket::setSendBufferSize(1024*24);     // 
	CTcpSocket::setReceiveBufferSize(1024*24); // 

	sirResult = CTcpSocket::connect(sHost, uiPort);
	if(sirResult != CSocketImpl::SIR_OK && sirResult != CSocketImpl::SIR_WOULD_BLOCK){
		uiErrorCode = CTcpSocket::getErrorCode();

		if(uiErrorCode == WSAEALREADY || uiErrorCode == WSAEISCONN) return CR_OK; // 

		if(uiErrorCode == WSAENETUNREACH){ // 
			doClose(CS_UNREACHABLE_FAILURE);
			return CR_FAILURE;
		};
	
		if(uiErrorCode == WSAETIMEDOUT){ // 
			doClose(CS_TIMEOUT_FAILURE);
			return CR_FAILURE;
		};
	
		if(uiErrorCode == WSAECONNREFUSED){ // 
			doClose(CS_CONNECTION_REFUSE);
			return CR_FAILURE;
		};
	
		if(uiErrorCode == WSAEADDRNOTAVAIL){ // 
			doClose(CS_INVALID_ADDRESS);
			return CR_FAILURE;
		};
	
		if(uiErrorCode != 0){ // 
			doClose(CS_SYSTEM_FAILURE);
			return CR_FAILURE;
		};
	};

    setNonBlockingMode(true);
#ifndef WIN32
	if(sirResult == CSocketImpl::SIR_WOULD_BLOCK)
	{
		setPollEvent(getSocketId(),POLLIN|POLLOUT|POLLERR);
		return CR_OK;
	}
	else
	{
		setPollEvent(getSocketId(),POLLIN|POLLERR);
		onConnect(0);
	}
#endif

	m_emState = CS_CONNECTED;

	return CR_OK;
}

void CChannel::onConnect(uint32 uiErrorCode){ // 
	if(uiErrorCode == 0){ // 
		setPollEvent(getSocketId(),POLLIN|POLLERR);
		m_emState = CS_CONNECTED;
		onConnect();
		return;
	}

	if(uiErrorCode == WSAEALREADY || uiErrorCode == WSAEISCONN) return; // 

	if(uiErrorCode == WSAENETUNREACH){ // 
		doClose(CS_UNREACHABLE_FAILURE);
		return;
	}
	
	if(uiErrorCode == WSAETIMEDOUT){ // 
		doClose(CS_TIMEOUT_FAILURE);
		return;
	}
	
	if(uiErrorCode == WSAECONNREFUSED){ // 
		doClose(CS_CONNECTION_REFUSE);
		return;
	}
	
	if(uiErrorCode == WSAEADDRNOTAVAIL){ // 
		doClose(CS_INVALID_ADDRESS);
		return;
	}
	
	doClose(CS_SYSTEM_FAILURE);
}

void CChannel::onRecv(uint32 uiErrorCode){ // 
	uint32 uiReceivedBytes;

	CSocketImpl::ESocketImplResult sirResult;

	if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET){ // 
		m_pLog->logLn(CLog::LL_0, "onRecv before uiErrorCode = %d", uiErrorCode);
		doClose(CS_CONNECTION_RESET);
		return;
	}

	if(uiErrorCode != 0){ // 
		m_pLog->logLn(CLog::LL_0, "onRecv system failure,uiErrorCode = %d",uiErrorCode);
		doClose(CS_SYSTEM_FAILURE);
		return;
	}

	if(m_emState == CS_DEAD || m_emState == CS_SHUTTING_DOWN) return;

	uiReceivedBytes = MAX_PACKET_LEN; // 

	sirResult = CTcpSocket::recv(m_pRotativeBuffer->getTailRecvPtr(), uiReceivedBytes);

	if(sirResult == CSocketImpl::SIR_CONNECTION_CLOSED){ // 
		m_pLog->logLn(CLog::LL_0, "onRecv SIR_CONNECTION_CLOSED");
		doClose(CS_CONNECTION_RESET);
		return;
	}

	if(sirResult != CSocketImpl::SIR_OK && sirResult != CSocketImpl::SIR_WOULD_BLOCK){
		uiErrorCode = CTcpSocket::getErrorCode();

		if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET){ // 
			m_pLog->logLn(CLog::LL_0, "onRecv after uiErrorCode = %d", uiErrorCode);
			doClose(CS_CONNECTION_RESET);
			return;
		}

		if(uiErrorCode != 0){ // 
			m_pLog->logLn(CLog::LL_0, "onRecv recv failure,uiErrorCode = %d",uiErrorCode);
			doClose(CS_RECV_FAILURE);
			return;
		}
	}

	if(uiReceivedBytes == 0){
		m_pLog->logLn(CLog::LL_2, "uiReceivedBytes = 0");
		return;
	}

	CChannel::EChannelResult crResult;
	if((crResult = addRecvBuffer(uiReceivedBytes)) != CR_OK){ // 
		m_pLog->logLn(CLog::LL_0, "Channel addRecvBuffer crResult %d", crResult);
		doClose(CS_BUFFER_FAILURE);
		return;
	}

	if((crResult = doDispatchService()) != CR_OK){ //
		m_pLog->logLn(CLog::LL_0, "Channel doDispatchService crResult %d", crResult);
		doClose(CS_DISPATCH_FAILURE);
		return;
	}

	return;
}

CChannel::EChannelResult CChannel::addRecvBuffer(uint32 uiReceivedBytes){ // 
	CRotativeBuffer::ERotativeBufferResult rbrResult;

	synchronized sync(&m_cMonitor);

	CTcpSocket::setReceivedByte(CTcpSocket::getReceivedByte() + uiReceivedBytes);

	rbrResult = m_pRotativeBuffer->forwardTailRecvPtr(uiReceivedBytes);

	if(rbrResult == CRotativeBuffer::RBR_PACKET_BIG) return CR_PACKET_BIG;
	else if(rbrResult == CRotativeBuffer::RBR_BUFFER_OVERFLOW) return CR_BUFFER_OVERFLOW;

	return CR_OK;
}

CChannel::EChannelResult CChannel::doDispatchService(void){ // 
	uint32 uiHeaderLen, uiHeaderPos, uiBodyLen, uiLen;
	char*  pMarkRecvPtr;
	sint32 siRemainLen;
	
#ifdef LOG_LEVEL_2
	uint32 uiMsgId;
#endif

	synchronized sync(&m_cMonitor);

	uiHeaderPos = 0;
	uiHeaderLen = sizeof(uint32);

	siRemainLen = m_pRotativeBuffer->getDiffTailMarkRecvPtr(); // 

	if(siRemainLen < (sint32)uiHeaderLen) return CR_OK; // 

	while(true){
		uiLen = m_pRotativeBuffer->getDiffFinalMarkRecvPtr(); // 

		if(uiLen < uiHeaderLen) m_pRotativeBuffer->copyOriginFinalRecvBuffer(uiHeaderLen - uiLen);
		// 
		pMarkRecvPtr = m_pRotativeBuffer->getMarkRecvPtr();
		memcpy(&uiBodyLen, pMarkRecvPtr + uiHeaderPos, sizeof(uint32)); // 
		
#ifdef LOG_LEVEL_2
		memcpy(&uiMsgId, pMarkRecvPtr + uiHeaderPos + sizeof(uint32), sizeof(uint32));
		m_pLog->logLn(CLog::LL_3, "uiMsgId = %d", uiMsgId);
#endif

		if(uiBodyLen > MAX_PACKET_LEN) 
			return CR_PACKET_BIG; // 

		if(siRemainLen < (sint32)(uiBodyLen)) return CR_OK; // 

		m_pRotativeBuffer->forwardMarkRecvPtr(uiBodyLen); // 

		siRemainLen -= uiBodyLen;

		if(m_pDispatchService->addObject(this, false,0) != CDispatchService::DSR_OK) 
			return CR_FAILURE; // 

		if(siRemainLen < (sint32)uiHeaderLen) return CR_OK; // 
	};
}

#ifdef WIN32
CChannel::EChannelResult CChannel::doSend(char* szBuffer, uint32 uiSendBytes){ // 
	uint32 uiSentBytes, uiErrorCode;
	char*  pTailSendPtr;

	CRotativeBuffer::ERotativeBufferResult rbrResult;
	CSocketImpl::ESocketImplResult         sirResult;

	synchronized sync(&m_cMonitor);

	if(szBuffer == NULL || uiSendBytes == 0){
		m_pLog->logLn(CLog::LL_0, "szBuffer = %d, uiSendBytes = %d", szBuffer, uiSendBytes);
		return CR_NULL;
	}

	if(m_emState == CS_DEAD || m_emState == CS_SHUTTING_DOWN){
		m_pLog->logLn(CLog::LL_0, "m_emState = %d", m_emState);
		return CR_INVALID_MODE;
	}

	rbrResult = m_pRotativeBuffer->addSendBuffer(szBuffer, uiSendBytes, &pTailSendPtr); // 

#ifdef SOCKET_LEVEL
	m_pLog->logLn(CLog::LL_0, "rbrResult = %d", rbrResult);
#endif

	if(rbrResult == CRotativeBuffer::RBR_PACKET_BIG) return CR_PACKET_BIG;
	else if(rbrResult == CRotativeBuffer::RBR_BUFFER_OVERFLOW) return CR_BUFFER_OVERFLOW;

	if(m_pRotativeBuffer->getRemainSendLen() == uiSendBytes){
		uiSentBytes = uiSendBytes;

		sirResult = CTcpSocket::send(pTailSendPtr, uiSentBytes);
		if(sirResult != CSocketImpl::SIR_OK && sirResult != CSocketImpl::SIR_WOULD_BLOCK){
			uiErrorCode = CTcpSocket::getErrorCode();
			m_pLog->logLn(CLog::LL_0, "doSend uiErrorCode = %d", uiErrorCode);
			
			if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET){ // 
				return CR_FAILURE;
			}

			if(uiErrorCode != 0){ // 
				return CR_FAILURE;
			}
		}

		CTcpSocket::setSentByte(CTcpSocket::getSentByte() + uiSentBytes);

		m_pRotativeBuffer->forwardHeadSendPtr(uiSentBytes); // 
		
		return CR_OK;
	}

	return CR_WOULD_BLOCK; // 
}
#else

CChannel::EChannelResult CChannel::doSend(char* szBuffer, uint32 uiSendBytes){ // 
	uint32 uiSentBytes, uiErrorCode;
	char*  pTailSendPtr;

	CRotativeBuffer::ERotativeBufferResult rbrResult;
	CSocketImpl::ESocketImplResult         sirResult;

	synchronized sync(&m_cMonitor);

	if(szBuffer == NULL || uiSendBytes == 0){
		m_pLog->logLn(CLog::LL_0, "szBuffer = %d, uiSendBytes = %d", szBuffer, uiSendBytes);
		return CR_NULL;
	}

	if(m_emState == CS_DEAD || m_emState == CS_SHUTTING_DOWN){
		m_pLog->logLn(CLog::LL_0, "m_emState = %d", m_emState);
		return CR_INVALID_MODE;
	}

	rbrResult = m_pRotativeBuffer->addSendBuffer(szBuffer, uiSendBytes, &pTailSendPtr); // 

#ifdef SOCKET_LEVEL
	m_pLog->logLn(CLog::LL_0, "rbrResult = %d", rbrResult);
#endif

	if(rbrResult == CRotativeBuffer::RBR_PACKET_BIG) return CR_PACKET_BIG;
	else if(rbrResult == CRotativeBuffer::RBR_BUFFER_OVERFLOW) return CR_BUFFER_OVERFLOW;

	if(m_pRotativeBuffer->getRemainSendLen() == uiSendBytes){
		uiSentBytes = uiSendBytes;

		sirResult = CTcpSocket::send(pTailSendPtr, uiSentBytes);
		if(sirResult != CSocketImpl::SIR_OK && sirResult != CSocketImpl::SIR_WOULD_BLOCK){
			uiErrorCode = CTcpSocket::getErrorCode();
			m_pLog->logLn(CLog::LL_0, "doSend uiErrorCode = %d", uiErrorCode);
			
			if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET){ // 
				return CR_FAILURE;
			}

			if(uiErrorCode != 0){ // 
				return CR_FAILURE;
			}
		}
		
		if(sirResult == CSocketImpl::SIR_WOULD_BLOCK)
		{
			setPollEvent(getSocketId(),POLLIN|POLLOUT|POLLERR);
			return CR_OK;
		}
		else
			setPollEvent(getSocketId(),POLLIN|POLLERR);

		CTcpSocket::setSentByte(CTcpSocket::getSentByte() + uiSentBytes);

		m_pRotativeBuffer->forwardHeadSendPtr(uiSentBytes); // 
		
		return CR_OK;
	}
	else
	{
		return doRestSend();
	}

	return CR_OK; // 
}

#endif

void CChannel::onSend(uint32 uiErrorCode){ // 
#ifdef LOG_LEVEL_2
	m_pLog->logLn(CLog::LL_3, "onSend uiErrorCode = %d", uiErrorCode);
#endif
	if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET){ // 
		doClose(CS_CONNECTION_RESET);
		return;
	};

	if(uiErrorCode != 0){ // 
		doClose(CS_SYSTEM_FAILURE);
		return;
	};

	if(doRestSend() != CR_OK) doClose(CS_SEND_FAILURE);
}

CChannel::EChannelResult CChannel::doRestSend(void){ // 
	uint32 uiRestSendBytes, uiSentBytes, uiErrorCode, uiLen;

	synchronized sync(&m_cMonitor);

	if(m_emState == CS_DEAD || m_emState == CS_SHUTTING_DOWN) return CR_INVALID_MODE;

	uiRestSendBytes = m_pRotativeBuffer->getRemainSendLen();
	uiLen           = m_pRotativeBuffer->getDiffFinalHeadSendPtr();

	if(uiRestSendBytes > 0){
		if(uiRestSendBytes > MAX_PACKET_LEN) uiRestSendBytes = MAX_PACKET_LEN;
		if(uiRestSendBytes > uiLen) uiRestSendBytes = uiLen;

		uiSentBytes = uiRestSendBytes;
		
		
		CSocketImpl::ESocketImplResult sirResult;
		if((sirResult=CTcpSocket::send(m_pRotativeBuffer->getHeadSendPtr(), uiSentBytes)) != CSocketImpl::SIR_OK){
			uiErrorCode = CTcpSocket::getErrorCode();
#ifdef LOG_LEVEL_2
			m_pLog->logLn(CLog::LL_3, "doRestSend uiErrorCode = %d", uiErrorCode);
#endif
			if(sirResult==CSocketImpl::SIR_WOULD_BLOCK)
			{
				setPollEvent(getSocketId(),POLLIN|POLLOUT|POLLERR);
				return CR_OK;
			}
			
			if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET){ // 
				return CR_FAILURE;
			};

			if(uiErrorCode != 0){ // 
				return CR_FAILURE;
			};
		};
		
		setPollEvent(getSocketId(),POLLIN|POLLERR);

		CTcpSocket::setSentByte(CTcpSocket::getSentByte() + uiSentBytes);

		m_pRotativeBuffer->forwardHeadSendPtr(uiSentBytes); // 
	};

	return CR_OK;
}

void CChannel::onClose(uint32 uiErrorCode){ // 
#ifdef LOG_LEVEL_2
	m_pLog->logLn(CLog::LL_3, "onClose uiErrorCode = %d", uiErrorCode);
#endif
	if(uiErrorCode == 0){ // 
		doClose(CS_SERVICE_STOP);
		return;
	};

	if(uiErrorCode == WSAECONNRESET || uiErrorCode == WSAECONNABORTED || uiErrorCode == WSAENETRESET){ // 
		doClose(CS_CONNECTION_RESET);
		return;
	};

	doClose(CS_SYSTEM_FAILURE);
}

CChannel::EChannelResult CChannel::doClose(SCloseStatus csStatus){ // 

	//synchronized sync(&m_cMonitor); // lishu

	if(m_emState == CS_DEAD || m_emState == CS_SHUTTING_DOWN) return CR_OK; // 

	CTcpSocket::setSentByte(0);     // 
	CTcpSocket::setReceivedByte(0); // 

	m_emState = CS_SHUTTING_DOWN;

	CTcpSocket::close(); // 

	onClose(csStatus);

	synchronized sync(&m_cMonitor);

	m_emState = CS_DEAD;

	if(CTcpSocket::create() != CSocketImpl::SIR_OK) return CR_FAILURE; // 

	if(m_pRotativeBuffer->initBuffer() != CRotativeBuffer::RBR_OK) return CR_BUFFER_INIT_FAILURE; // 

	return CR_OK;
}

CChannel::SChannelState CChannel::getState(void){ // 
	return m_emState;
}

void CChannel::setState(CChannel::SChannelState emState){ // 
	m_emState = emState;
}

bool CChannel::isAvailable(void){ // 
	return (m_emState == CS_DEAD)? true : false;
}

CChannel::EChannelResult CChannel::getMessage(uint32* uiMessageId, char** szMessage){ // 
	CRotativeBuffer::ERotativeBufferResult rbrResult;

	synchronized sync(&m_cMonitor);

	rbrResult = m_pRotativeBuffer->getMessage(uiMessageId, szMessage);
	
	if(rbrResult != CRotativeBuffer::RBR_OK){
		m_pLog->logLn(CLog::LL_0, "rbrResult = %d", rbrResult);
		return CR_FAILURE;
	}

	return CR_OK;
}

}
