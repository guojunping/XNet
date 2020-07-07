///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "TcpSocket.h"

namespace XNetFrame{

CTcpSocket::CTcpSocket(void){
	m_pSocketImpl = NULL;
	#ifdef WIN32
	m_hIoEvent    = WSA_INVALID_EVENT;
	#endif
}

CTcpSocket::~CTcpSocket(void){
	close();
}

void CTcpSocket::close(void){ // 
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
CSocketImpl::ESocketImplResult CTcpSocket::eventSelect(long lEvent){ // 
	sint32 siResult;

	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	if(lEvent == 0) siResult = ::WSAEventSelect(m_pSocketImpl->getSocketId(), m_hIoEvent, 0);
	else siResult = ::WSAEventSelect(m_pSocketImpl->getSocketId(), m_hIoEvent, lEvent);
	
	if(siResult == SOCKET_ERROR){
		m_pSocketImpl->setErrorCode(::WSAGetLastError());
		return CSocketImpl::SIR_FAILURE;
	};

	return CSocketImpl::SIR_OK;
}
#endif

CSocketImpl::ESocketImplResult CTcpSocket::connect(CInetAddress& iaInetAddress){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->connect(iaInetAddress);
}

CSocketImpl::ESocketImplResult CTcpSocket::connect(string& sHost, uint16 uiPort){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;
	return m_pSocketImpl->connect(sHost, uiPort);
}

CSocketImpl::ESocketImplResult CTcpSocket::bind(string& sHost, uint16 uiPort){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->bind(sHost, uiPort);
}

CSocketImpl::ESocketImplResult CTcpSocket::setOption(sint32 siName, sint32 siValue){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setOption(siName, siValue);
}

CSocketImpl::ESocketImplResult CTcpSocket::setOption(sint32 siName, bool bValue){
	if (m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setOption(siName, bValue);
}

#ifdef WIN32
CSocketImpl::ESocketImplResult CTcpSocket::create(long lEvent){ // 
	CSocketImpl::ESocketImplResult sirResult;

	m_pSocketImpl = new CSocketImpl();
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	//m_pSocketImpl->m_acAuthCrypt.setKey(DEFAULT_CRYPT_STR);

	sirResult = m_pSocketImpl->create(SOCK_STREAM);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	sirResult = m_pSocketImpl->setNonBlockingMode(true); // 
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	m_hIoEvent = ::WSACreateEvent();
	if(m_hIoEvent == WSA_INVALID_EVENT) return CSocketImpl::SIR_NULL;

	return eventSelect(lEvent); // 
}
#else
CSocketImpl::ESocketImplResult CTcpSocket::create(){ // 
	CSocketImpl::ESocketImplResult sirResult;

	m_pSocketImpl = new CSocketImpl();
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	//m_pSocketImpl->m_acAuthCrypt.setKey(DEFAULT_CRYPT_STR);

	sirResult = m_pSocketImpl->create(SOCK_STREAM);
	if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	//sirResult = m_pSocketImpl->setNonBlockingMode(true); // 
	//if(sirResult != CSocketImpl::SIR_OK) return sirResult;

	return sirResult; // 
}
#endif

CSocketImpl::ESocketImplResult CTcpSocket::send(char* szBuffer, uint32& uiLen){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->send(szBuffer, uiLen);
}

CSocketImpl::ESocketImplResult CTcpSocket::recv(char* szBuffer, uint32& uiLen){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->recv(szBuffer, uiLen);
}

CSocketImpl::ESocketImplResult CTcpSocket::setSendBufferSize(sint32 siSize){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setOption(SO_SNDBUF, siSize);
}

CSocketImpl::ESocketImplResult CTcpSocket::setReceiveBufferSize(sint32 siSize){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setOption(SO_RCVBUF, siSize);
}

CSocketImpl::ESocketImplResult CTcpSocket::getSendBufferSize(sint32* siSize){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->getOption(SO_SNDBUF, siSize);
}

CSocketImpl::ESocketImplResult CTcpSocket::getReceiveBufferSize(sint32* siSize){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->getOption(SO_RCVBUF, siSize);
}

CSocketImpl::ESocketImplResult CTcpSocket::setTcpNodelay(bool bValue){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setTcpNodelay(bValue);
}

CSocketImpl::ESocketImplResult CTcpSocket::getTcpNodelay(bool* bValue){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->getTcpNodelay(bValue);
}

CSocketImpl::ESocketImplResult CTcpSocket::setNonBlockingMode(bool bBlocking)
{
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;
		
	return m_pSocketImpl->setNonBlockingMode(bBlocking);
}

CSocketImpl::ESocketImplResult CTcpSocket::setTcpKeepAlive(uint32 uiKeepAlive, uint32 uiKeepTime, uint32 uiKeepInterval){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setTcpKeepAlive(uiKeepAlive, uiKeepTime, uiKeepInterval);
}

CSocketImpl::ESocketImplResult CTcpSocket::available(void){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->available();
}

sint16 CTcpSocket::getPort(void){ // 
	if(m_pSocketImpl == NULL) return 0;

	return m_pSocketImpl->getRemotePort();
}

CInetAddress* CTcpSocket::getAddress(void){ // 
	if(m_pSocketImpl == NULL) return NULL;
	
	return m_pSocketImpl->getRemoteAddress();
}

sint16 CTcpSocket::getLocalPort(void){
	if(m_pSocketImpl == NULL) return 0;

	return m_pSocketImpl->getLocalPort();
}

CInetAddress* CTcpSocket::getLocalAddress(void){
	if(m_pSocketImpl == NULL) return NULL;

	return m_pSocketImpl->getLocalAddress();
}

bool CTcpSocket::isConnected(void){
	if(m_pSocketImpl == NULL) return false;

	return m_pSocketImpl->isConnected();
}

#ifdef WIN32
WSAEVENT CTcpSocket::getIoEvent(void){ // 
	return m_hIoEvent;
}
#endif

SOCKET CTcpSocket::getSocketId(void){ // 
	if(m_pSocketImpl == NULL) return -1;

	return m_pSocketImpl->getSocketId();
}

uint32 CTcpSocket::getSentByte(void){
	if(m_pSocketImpl == NULL) return 0;

	return m_pSocketImpl->getSentByte();
}

uint32 CTcpSocket::getReceivedByte(void){
	if(m_pSocketImpl == NULL) return 0;

	return m_pSocketImpl->getReceivedByte();
}

void CTcpSocket::setSentByte(uint32 uiSentByte){
	if(m_pSocketImpl != NULL) m_pSocketImpl->setSentByte(uiSentByte);
}

void CTcpSocket::setReceivedByte(uint32 uiReceivedByte){
	if(m_pSocketImpl != NULL) m_pSocketImpl->setReceivedByte(uiReceivedByte);
}

uint32 CTcpSocket::getErrorCode(void){
	if(m_pSocketImpl == NULL) return 0;
	
	return m_pSocketImpl->getErrorCode();
}

TSTRING CTcpSocket::getErrorString(uint32 uiErrorCode){
	if(m_pSocketImpl == NULL) return _T("");
	
	return m_pSocketImpl->getErrorString(uiErrorCode);
}

TSTRING CTcpSocket::toString(void){ // 
	if(m_pSocketImpl == NULL) return _T("");
	
	return m_pSocketImpl->toString();
}

}
