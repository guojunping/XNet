///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "UdpSocket.h"

namespace XNetFrame{

CUdpSocket::CUdpSocket(void){
	m_pSocketImpl = NULL;
#ifdef WIN32
	m_hIoEvent    = WSA_INVALID_EVENT;
#endif
	m_siSendSystemBufferSize    = 8192;
	m_siReceiveSystemBufferSize = 8192;
}

CUdpSocket::~CUdpSocket(void){
	close();
}

void CUdpSocket::close(void){ // 
	if(m_pSocketImpl != NULL){
#ifdef WIN32
		eventSelect(0); // 

		::WSACloseEvent(m_hIoEvent);
		m_hIoEvent = WSA_INVALID_EVENT;
#endif
		m_pSocketImpl->close();

		delete m_pSocketImpl;
		m_pSocketImpl = NULL;
	};
}

#ifdef WIN32
CSocketImpl::ESocketImplResult CUdpSocket::eventSelect(long lEvent){ // 
	sint32 siResult;

	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	// To cancel the association and selection of network events on a socket, lNetworkEvents should be set to zero, 
	// in which case the hEventObject parameter will be ignored.
	if(lEvent == 0) siResult = ::WSAEventSelect(m_pSocketImpl->getSocketId(), m_hIoEvent, 0);
	else siResult = ::WSAEventSelect(m_pSocketImpl->getSocketId(), m_hIoEvent, lEvent);
	
	if(siResult == SOCKET_ERROR){
		m_pSocketImpl->setErrorCode(::WSAGetLastError());
		return CSocketImpl::SIR_FAILURE;
	};

	return CSocketImpl::SIR_OK;
}

CSocketImpl::ESocketImplResult CUdpSocket::create(string sHost, uint32 uiPort, long lEvent){ // 
	CSocketImpl::ESocketImplResult sirResult;

	m_pSocketImpl = new CSocketImpl();
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	//m_pSocketImpl->m_acAuthCrypt.setKey(DEFAULT_CRYPT_STR);

	sirResult = m_pSocketImpl->create(SOCK_DGRAM);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = m_pSocketImpl->setNonBlockingMode(true); // 
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = setSendBufferSize(m_siSendSystemBufferSize);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = setReceiveBufferSize(m_siReceiveSystemBufferSize);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = m_pSocketImpl->bind(sHost, (uint16)uiPort);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = m_pSocketImpl->setOption(SO_REUSEADDR, true);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	m_hIoEvent = ::WSACreateEvent();
	if(m_hIoEvent == WSA_INVALID_EVENT) return CSocketImpl::SIR_NULL;

	return eventSelect(lEvent); // 
}
#else
CSocketImpl::ESocketImplResult CUdpSocket::create(string sHost, uint32 uiPort){ // 
	CSocketImpl::ESocketImplResult sirResult;

	m_pSocketImpl = new CSocketImpl();
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	//m_pSocketImpl->m_acAuthCrypt.setKey(DEFAULT_CRYPT_STR);

	sirResult = m_pSocketImpl->create(SOCK_DGRAM);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = m_pSocketImpl->setNonBlockingMode(true); // 
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = setSendBufferSize(m_siSendSystemBufferSize);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = setReceiveBufferSize(m_siReceiveSystemBufferSize);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = m_pSocketImpl->bind(sHost, (uint16)uiPort);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = m_pSocketImpl->setOption(SO_REUSEADDR, true);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	return sirResult; // 
}
#endif

CSocketImpl::ESocketImplResult CUdpSocket::connect(CInetAddress& iaInetAddress){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->connect(iaInetAddress);
}

CSocketImpl::ESocketImplResult CUdpSocket::connect(string& sHost, uint16 uiPort){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->connect(sHost, uiPort);
}

CSocketImpl::ESocketImplResult CUdpSocket::bind(string& sHost, uint16 uiPort){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->bind(sHost, uiPort);
}

CSocketImpl::ESocketImplResult CUdpSocket::sendTo(char* szBuffer, uint32& uiLen, CInetAddress* pInetAddress){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->sendTo(szBuffer, uiLen, pInetAddress);
}

CSocketImpl::ESocketImplResult CUdpSocket::recvFrom(char* szBuffer, uint32& uiLen, CInetAddress* pInetAddress){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->recvFrom(szBuffer, uiLen, pInetAddress);
}

CSocketImpl::ESocketImplResult CUdpSocket::setSendBufferSize(sint32 siSize){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setOption(SO_SNDBUF, siSize);
}

CSocketImpl::ESocketImplResult CUdpSocket::setReceiveBufferSize(sint32 siSize){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setOption(SO_RCVBUF, siSize);
}

CSocketImpl::ESocketImplResult CUdpSocket::getSendBufferSize(sint32* siSize){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->getOption(SO_SNDBUF, siSize);
}

CSocketImpl::ESocketImplResult CUdpSocket::getReceiveBufferSize(sint32* siSize){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->getOption(SO_RCVBUF, siSize);
}

#ifdef WIN32
WSAEVENT CUdpSocket::getIoEvent(void){ // 
	return m_hIoEvent;
}
#endif

SOCKET CUdpSocket::getSocketId(void){ // 
	if(m_pSocketImpl == NULL) return -1;

	return m_pSocketImpl->getSocketId();
}

uint32 CUdpSocket::getErrorCode(void){
	if(m_pSocketImpl == NULL) return 0;
	
	return m_pSocketImpl->getErrorCode();
}

TSTRING CUdpSocket::getErrorString(uint32 uiErrorCode){
	if(m_pSocketImpl == NULL) return _T("");
	
	return m_pSocketImpl->getErrorString(uiErrorCode);
}

void CUdpSocket::setSendSystemBufferSize(sint32 siSendSystemBufferSize){
	m_siSendSystemBufferSize = siSendSystemBufferSize;
}

void CUdpSocket::setReceiveSystemBufferSize(sint32 siReceiveSystemBufferSize){
	m_siReceiveSystemBufferSize = siReceiveSystemBufferSize;
}

}
