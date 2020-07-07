///////////////////////////////////////
// °³ ¹ß ÀÚ : ±è°æ¼®
// ¼³    ¸í : ¼ÒÄÏ °ü·Ã Å¬·¡½º¿¡ ´ëÇÑ ±¸Çö
///////////////////////////////////////

#include "Socket.h"

namespace XNetFrame{

CSocketImpl* CSocket::getSocketImpl(void){
	return m_pSocketImpl;
}

CSocket::CSocket(void){
	m_pSocketImpl = NULL;
}
#ifdef WIN32
CSocketImpl::ESocketImplResult CSocket::create(void){ // SocketImpl °´Ã¼¸¦ ÇÒ´çÇÏ°í ¼ÒÄÏÀ» »ý¼º
	m_pSocketImpl = new CSocketImpl();
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	//m_pSocketImpl->m_acAuthCrypt.setKey(DEFAULT_CRYPT_STR);

	return m_pSocketImpl->create(SOCK_STREAM);
}
#else
CSocketImpl::ESocketImplResult CSocket::create(void){ // SocketImpl °´Ã¼¸¦ ÇÒ´çÇÏ°í ¼ÒÄÏÀ» »ý¼º
	m_pSocketImpl = new CSocketImpl();
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	//m_pSocketImpl->m_acAuthCrypt.setKey(DEFAULT_CRYPT_STR);

	return CSocketImpl::SIR_OK;
}
#endif

void CSocket::close(void){ // ¼ÒÄÏÀ» ÆÄ±«
	if(m_pSocketImpl != NULL){
		m_pSocketImpl->close();
		delete m_pSocketImpl;
		m_pSocketImpl = NULL;
	};
}

CSocket::~CSocket(void){
	if(m_pSocketImpl != NULL){
		m_pSocketImpl->close();
		delete m_pSocketImpl;
		m_pSocketImpl = NULL;
	}
}

uint32 CSocket::getErrorCode(void){
	if(m_pSocketImpl == NULL) return 0;
	
	return m_pSocketImpl->getErrorCode();
}

TSTRING CSocket::getErrorString(uint32 uiErrorCode){
	if(m_pSocketImpl == NULL) return _T("");
	
	return m_pSocketImpl->getErrorString(uiErrorCode);
}

CSocketImpl::ESocketImplResult CSocket::connect(CInetAddress& iaInetAddress){ // ¿ÜºÎ È£½ºÆ®¿Í ¿¬°á
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->connect(iaInetAddress);
}

CSocketImpl::ESocketImplResult CSocket::connect(string& sHost, uint16 uiPort){ // ¿ÜºÎ È£½ºÆ®¿Í ¿¬°á
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->connect(sHost, uiPort);
}

// ºí·ÏÅ· ¸ðµåÀÏ °æ¿ì ÁÖ¾îÁø Å©±â¸¸Å­ µ¥ÀÌÅÍ°¡ Àü¼ÛµÉ ¶§±îÁö ºí·ÏµÊ
// ³Íºí·ÏÅ· ¸ðµåÀÏ °æ¿ì ÁÖ¾îÁø Å©±âº¸´Ù ÀûÀº µ¥ÀÌÅÍ Àü¼ÛÀÌ µÇ°í ¸®ÅÏÇÒ ¼ö ÀÖÀ½

CSocketImpl::ESocketImplResult CSocket::send(char* szBuffer, uint32& uiLen){ // ¼ÒÄÏÀ» ÅëÇÑ µ¥ÀÌÅÍ ¼Û½Å
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->send(szBuffer, uiLen);
}

CSocketImpl::ESocketImplResult CSocket::recv(char* szBuffer, uint32& uiLen){ // ¼ÒÄÏÀ» ÅëÇÑ µ¥ÀÌÅÍ ¼ö½Å
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->recv(szBuffer, uiLen);
}

CSocketImpl::ESocketImplResult CSocket::setSendBufferSize(sint32 siSize){
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setOption(SO_SNDBUF, siSize);
}

CSocketImpl::ESocketImplResult CSocket::setReceiveBufferSize(sint32 siSize){
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setOption(SO_RCVBUF, siSize);
}

CSocketImpl::ESocketImplResult CSocket::getSendBufferSize(sint32* siSize){
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->getOption(SO_SNDBUF, siSize);
}

CSocketImpl::ESocketImplResult CSocket::getReceiveBufferSize(sint32* siSize){
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->getOption(SO_RCVBUF, siSize);
}

CSocketImpl::ESocketImplResult CSocket::setTcpNodelay(bool bValue){
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setTcpNodelay(bValue);
}

CSocketImpl::ESocketImplResult CSocket::getTcpNodelay(bool* bValue){
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->getTcpNodelay(bValue);
}

CSocketImpl::ESocketImplResult CSocket::setTcpKeepAlive(uint32 uiKeepAlive, uint32 uiKeepTime, uint32 uiKeepInterval){
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->setTcpKeepAlive(uiKeepAlive, uiKeepTime, uiKeepInterval);
}

CSocketImpl::ESocketImplResult CSocket::available(void){ // µ¥ÀÌÅÍ°¡ µµÂøÇß´ÂÁö È®ÀÎ
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->available();
}

uint32 CSocket::getSentByte(void){
	if(m_pSocketImpl == NULL) return 0;

	return m_pSocketImpl->getSentByte();
}

uint32 CSocket::getReceivedByte(void){
	if(m_pSocketImpl == NULL) return 0;

	return m_pSocketImpl->getReceivedByte();
}

void CSocket::setLocalAddress(sockaddr_in* psaLocalAddress){
	if(m_pSocketImpl != NULL && psaLocalAddress != NULL) m_pSocketImpl->m_iaLocalAddress.setSocketAddress((sockaddr_in*)psaLocalAddress);
}

void CSocket::setRemoteAddress(sockaddr_in* psaRemoteAddress){
	if(m_pSocketImpl != NULL && psaRemoteAddress != NULL) m_pSocketImpl->m_iaRemoteAddress.setSocketAddress((sockaddr_in*)psaRemoteAddress);
}

CSocketImpl::ESocketImplResult CSocket::setNonBlockingMode(bool bBlocking)
{
	if(m_pSocketImpl == NULL) 
		return CSocketImpl::SIR_NULL;
		
	return m_pSocketImpl->setNonBlockingMode(bBlocking);
}

SOCKET CSocket::getSocketId(void){
	if(m_pSocketImpl == NULL) return 0;
	
	return m_pSocketImpl->getSocketId();
}

CInetAddress* CSocket::getAddress(void){ // ¿ø°ÝÁö IP Á¤º¸¸¦ ¸®ÅÏ
	if(m_pSocketImpl == NULL) return NULL;
	
	return m_pSocketImpl->getRemoteAddress();
}

sint16 CSocket::getPort(void){ // ¿ø°ÝÁö Æ÷Æ® ¹øÈ£¸¦ ¸®ÅÏ
	if(m_pSocketImpl == NULL) return 0;

	return m_pSocketImpl->getRemotePort();
}

bool CSocket::isConnected(void){
	if(m_pSocketImpl == NULL) return false;

	return m_pSocketImpl->isConnected();
}

TSTRING CSocket::toString(void){ // ¿ø°ÝÁö IP Á¤º¸¸¦ Ãâ·Â
	if(m_pSocketImpl == NULL) return _T("");
	
	return m_pSocketImpl->toString();
}

#ifdef WIN32

CSocketImpl::ESocketImplResult CSocket::createForOverlapped(void){ // ¿À¹ö·¦µå ¼ÒÄÏÀ» »ý¼º
	m_pSocketImpl = new CSocketImpl();
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;

	//m_pSocketImpl->m_acAuthCrypt.setKey(DEFAULT_CRYPT_STR);

	return m_pSocketImpl->createForOverlapped(SOCK_STREAM);
}

CSocketImpl::ESocketImplResult CSocket::getResultForOverlapped(OVERLAPPED* pOverlapped, DWORD* pdwReceivedBytes){ // Overlapped ¼ÒÄÏ¿¡ ´ëÇÑ ÃÖÁ¾ I/O »óÅÂ¸¦ È¹µæ
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;
	if(pOverlapped == NULL || pdwReceivedBytes == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->getResultForOverlapped(pOverlapped, pdwReceivedBytes);
}

CSocketImpl::ESocketImplResult CSocket::recvForOverlapped(WSABUF* pwsaBuffer, DWORD* pdwReceivedBytes, OVERLAPPED* pOverlapped){ // Overlapped ¼ÒÄÏ¿¡ ´ëÇÑ ÀÔ·Â ¿¬»ê ¼³Á¤
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;
	if(pwsaBuffer == NULL || pdwReceivedBytes == NULL || pOverlapped == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->recvForOverlapped(pwsaBuffer, pdwReceivedBytes, pOverlapped);
}

CSocketImpl::ESocketImplResult CSocket::sendForOverlapped(WSABUF* pwsaBuffer, DWORD* pdwSentBytes, OVERLAPPED* pOverlapped){ // Overlapped ¼ÒÄÏ¿¡ ´ëÇÑ Ãâ·Â ¿¬»ê ¼³Á¤
	if(m_pSocketImpl == NULL) return CSocketImpl::SIR_NULL;
	if(pwsaBuffer == NULL || pdwSentBytes == NULL || pOverlapped == NULL) return CSocketImpl::SIR_NULL;

	return m_pSocketImpl->sendForOverlapped(pwsaBuffer, pdwSentBytes, pOverlapped);
}

#endif

}
