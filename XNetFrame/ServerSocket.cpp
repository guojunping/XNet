///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "ServerSocket.h"

namespace XNetFrame{

CServerSocket::CServerSocket(void){
	m_pSocketImpl = NULL;
}

CSocketImpl::ESocketImplResult CServerSocket::create(void){ // 
	m_pSocketImpl = new CSocketImpl();
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->create(SOCK_STREAM);
}

void CServerSocket::close(void){ // 
	if(m_pSocketImpl != NULL){
		m_pSocketImpl->close();
		delete m_pSocketImpl;
		m_pSocketImpl = NULL;
	};
}

CServerSocket::~CServerSocket(void){
	if(m_pSocketImpl != NULL){
		m_pSocketImpl->close();
		delete m_pSocketImpl;
		m_pSocketImpl = NULL;
	};
}

uint32 CServerSocket::getErrorCode(void){
	if(m_pSocketImpl == NULL) return 0;
	
	return m_pSocketImpl->getErrorCode();
}

TSTRING CServerSocket::getErrorString(uint32 uiErrorCode){
	if(m_pSocketImpl == NULL) return _T("");
	
	return m_pSocketImpl->getErrorString(uiErrorCode);
}

CSocketImpl::ESocketImplResult CServerSocket::setReuseAddress(bool bValue){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setOption(SO_REUSEADDR, bValue);
}

CSocketImpl::ESocketImplResult CServerSocket::getReuseAddress(bool* bValue){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->getOption(SO_REUSEADDR, bValue);
}

CSocketImpl::ESocketImplResult CServerSocket::setKeepAlive(uint32 uiKeepAlive, uint32 uiKeepTime, uint32 uiKeepInterval){
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setTcpKeepAlive(uiKeepAlive, uiKeepTime, uiKeepInterval);
}

CSocketImpl::ESocketImplResult CServerSocket::setNonBlockingMode(bool bBlocking)
{
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;
		
	return m_pSocketImpl->setNonBlockingMode(bBlocking);
}

SOCKET CServerSocket::getSocketId(void){
	if(m_pSocketImpl == NULL) return 0;
	
	return m_pSocketImpl->getSocketId();
}

CInetAddress* CServerSocket::getAddress(void){ // 
	if(m_pSocketImpl == NULL) return NULL;
	
	return m_pSocketImpl->getLocalAddress();
}

sint16 CServerSocket::getPort(void){ // 
	if(m_pSocketImpl == NULL) return 0;

	return m_pSocketImpl->getLocalPort();
}

CSocketImpl::ESocketImplResult CServerSocket::bind(CInetAddress& iaInetAddress){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->bind(iaInetAddress);
}

CSocketImpl::ESocketImplResult CServerSocket::bind(string& sHost, uint16 uiPort){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->bind(sHost, uiPort);
}

CSocketImpl::ESocketImplResult CServerSocket::listen(uint32 uiBacklog){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->listen(uiBacklog);
}

CSocketImpl::ESocketImplResult CServerSocket::accept(CSocket* pSocket){ // 
	if(pSocket == NULL || m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->accept(pSocket->getSocketImpl());
}

#ifdef WIN32
	
CSocketImpl::ESocketImplResult CServerSocket::createForOverlapped(void){ // 
	m_pSocketImpl = new CSocketImpl();
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->createForOverlapped(SOCK_STREAM);
}

CSocketImpl::ESocketImplResult CServerSocket::acceptExForOverlapped(CSocket* pSocket, char* szRecvBuffer,
																	DWORD* pdwReceivedBytes, OVERLAPPED* pOverlapped){ // 
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;
	if(pSocket == NULL || szRecvBuffer == NULL || pdwReceivedBytes == NULL || pOverlapped == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->acceptExForOverlapped(pSocket->getSocketImpl(), szRecvBuffer, pdwReceivedBytes, pOverlapped);
}

#endif

}
