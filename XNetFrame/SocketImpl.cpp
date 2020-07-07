/**
 * socket helper class
 * 
 */

#include "SocketImpl.h"
#include <stdio.h>
#include "RotativeBuffer.h"

namespace XNetFrame{

#ifdef _WIN32
bool CSocketImpl::m_bInitialized = false; // 

bool CSocketImpl::initWinsock(void){ // 
	WORD    wVersion;
	WSADATA wsaData;

	if(m_bInitialized == false){
		wVersion = MAKEWORD(2, 2);
		if(::WSAStartup(wVersion, &wsaData) != 0) return false;

		if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2){
			::WSACleanup();
			return false;
		};

		m_bInitialized = true;
	};

	return true;
}

bool CSocketImpl::isInitialized(void){
	return m_bInitialized;
}

void CSocketImpl::releaseWinsock(void){ // 
	::WSACleanup();

	m_bInitialized = false;
}
#endif

CSocketImpl::CSocketImpl(void){
	m_uiErrorCode     = 0;
	m_uiBytesSent     = 0;
	m_uiBytesReceived = 0;

	m_bConnected       = false;
	m_bNonBlockingMode = false;

	m_siSocketId = INVALID_SOCKET;
}

CSocketImpl::~CSocketImpl(void){
	if(m_siSocketId != INVALID_SOCKET){
		if(isConnected() == true) ::shutdown(m_siSocketId, SD_BOTH);

		::closesocket(m_siSocketId);
		m_siSocketId = INVALID_SOCKET;
	};
}

uint32 CSocketImpl::getErrorCode(void){
	return m_uiErrorCode;
}

void CSocketImpl::setErrorCode(uint32 uiErrorCode){
	m_uiErrorCode = uiErrorCode;
}

string CSocketImpl::getErrorString(uint32 uiErrorCode){
#ifdef _WIN32
	switch(uiErrorCode){
		case WSAEINTR		   : return _T("Blocking operation interrupted");
		case WSAEINVAL		   : return _T("Invalid socket (maybe not bound) or argument");
		case WSAEMFILE		   : return _T("Too many open sockets");
		case WSAENOTSOCK	   : return _T("Socket operation on nonsocket (maybe invalid select descriptor)");
		case WSAEMSGSIZE	   : return _T("Message too long");
		case WSAEADDRINUSE     : return _T("Address already in use (is this service already running in this computer?)");
		case WSAEADDRNOTAVAIL  : return _T("Address not available");
		case WSAENETDOWN	   : return _T("Network is down");
		case WSAENETUNREACH    : return _T("Network is unreachable");
		case WSAECONNRESET     : return _T("Connection reset by peer");
		case WSAENOBUFS		   : return _T("No buffer space available; please close applications or reboot");
		case WSAESHUTDOWN	   : return _T("Cannot send/receive after socket shutdown");
		case WSAETIMEDOUT	   : return _T("Connection timed-out");
		case WSAECONNREFUSED   : return _T("Connection refused, the server may be offline");
		case WSAEHOSTUNREACH   : return _T("Remote host is unreachable");
		case WSANOTINITIALISED : return _T("'Windows Sockets' not initialized");
		default                : return _T("");
	};
#endif
	return "";
}

void CSocketImpl::close(void){ // 
	::closesocket(m_siSocketId);

	m_bConnected       = false;
	m_bNonBlockingMode = false;

	m_siSocketId = INVALID_SOCKET;
}

CSocketImpl::ESocketImplResult CSocketImpl::create(sint32 siType){ // 
	m_siSocketId = ::socket(AF_INET, siType, 0);

	if(m_siSocketId == INVALID_SOCKET){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	};

	return SIR_OK;
}

void CSocketImpl::init(SOCKET sSocket){ // 
	m_siSocketId = sSocket;
}

CSocketImpl::ESocketImplResult CSocketImpl::bind(CInetAddress& iaInetAddress){ // 
	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;
	if(iaInetAddress.isValid() == false) return SIR_INVALID_ADDRESS;

	if(::bind(m_siSocketId, (const sockaddr*)(&iaInetAddress.m_saSocketAddress), sizeof(sockaddr_in)) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	};

	setLocalAddress();

	//m_iaLocalAddress.setSocketAddress(&iaInetAddress.m_saSocketAddress);

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::accept(CSocketImpl* pSocketImpl){ // 
	sockaddr_in saSocketAddress;
	SOCKET      sSocketId;
	socklen     siSize;

	if(m_siSocketId == INVALID_SOCKET || pSocketImpl == NULL) return SIR_NULL;

	siSize = sizeof(sockaddr);

	sSocketId = ::accept(m_siSocketId, (sockaddr*)(&saSocketAddress), &siSize);
	if(sSocketId == INVALID_SOCKET){
		m_uiErrorCode = ::WSAGetLastError();

		if(m_uiErrorCode == WSAEWOULDBLOCK) return SIR_WOULD_BLOCK; // 
		else return SIR_FAILURE;
	};

	pSocketImpl->setLocalAddress();
    pSocketImpl->m_iaRemoteAddress.setSocketAddress((sockaddr_in*)&saSocketAddress);

    pSocketImpl->m_siSocketId = sSocketId;

	pSocketImpl->m_uiBytesReceived = 0;
	pSocketImpl->m_uiBytesSent     = 0;

	pSocketImpl->m_bConnected = true;

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::bind(string& sHost, uint16 uiPort){ // 
	CInetAddress iaInetAddress;

	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;

	if(sHost.empty()){
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
		addr.sin_port = ::htons(uiPort);
		iaInetAddress.setSocketAddress(&addr);
	}
	else{
		iaInetAddress.setByName(sHost); // 
		iaInetAddress.setPort(uiPort);
	}

	return bind(iaInetAddress);
}

CSocketImpl::ESocketImplResult CSocketImpl::connect(CInetAddress& iaInetAddress){ // 
	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;
	if(iaInetAddress.isValid() == false) return SIR_INVALID_ADDRESS;

	if(::connect(m_siSocketId, (const sockaddr*)(&iaInetAddress.m_saSocketAddress), sizeof(sockaddr_in)) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();

		if(m_uiErrorCode == WSAEWOULDBLOCK ||  m_uiErrorCode == WSAEINPROGRESS) 
			return SIR_WOULD_BLOCK; // 
		else 
			return SIR_FAILURE;
	};

	setLocalAddress();
	m_iaRemoteAddress = iaInetAddress;

	m_uiBytesReceived = 0;
	m_uiBytesSent     = 0;

	m_bConnected = true;

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::connect(string& sHost, uint16 uiPort){ // 
	CInetAddress iaInetAddress;

	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;

	iaInetAddress.setByName(sHost); // 
	iaInetAddress.setPort(uiPort);

	if(iaInetAddress.isValid() == false) return SIR_INVALID_ADDRESS;

	if(::connect(m_siSocketId, (const sockaddr*)(&iaInetAddress.m_saSocketAddress), sizeof(sockaddr_in)) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
#ifndef WIN32
		if(m_uiErrorCode == EINPROGRESS) return SIR_WOULD_BLOCK;
#endif
		if(m_uiErrorCode == WSAEWOULDBLOCK) return SIR_WOULD_BLOCK; // 
		else return SIR_FAILURE;
	};

	setLocalAddress();
	m_iaRemoteAddress = iaInetAddress;

	m_uiBytesReceived = 0;
	m_uiBytesSent     = 0;

	m_bConnected = true;

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::listen(uint32 uiBacklog){
	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;

	if(::listen(m_siSocketId, uiBacklog) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	};

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::send(char* szBuffer, uint32& uiLen){ // 
#ifdef SOCKET_LEVEL
	sockaddr_in   local;   
	int addrLen = sizeof(local);   
	memset(&local, 0, addrLen);     
#endif

	if(szBuffer == NULL || m_siSocketId == INVALID_SOCKET ) 
		return SIR_NULL;
		
	if(m_bConnected == false)
		return SIR_FAILURE;

	sint32 siLen = ::send(m_siSocketId, (const char*)szBuffer, uiLen, 0);
#ifdef SOCKET_LEVEL
	getsockname(m_siSocketId, (sockaddr*)(&local), &addrLen); 

	m_pLog->logLn(CLog::LL_0, "[%d]Send %d length from IP: %s Port: %d", m_siSocketId, uiLen, string(::inet_ntoa(local.sin_addr)).c_str(), local.sin_port);
#endif

	if(siLen == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();

		if(m_uiErrorCode == WSAEWOULDBLOCK){ // 
			uiLen = 0;
			return SIR_WOULD_BLOCK;
		}
		
		m_bConnected=false;
		return SIR_IO_ERROR;
	}

	uiLen = (uint32)siLen;
	m_uiBytesSent += uiLen;
	
	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::recv(char* szBuffer, uint32& uiLen){ // 
	uint32 uiTotalLen = 0;
	sint32 siReceived;
	
#ifdef SOCKET_LEVEL
	sockaddr_in   remote;   
	int addrLen = sizeof(remote);   
	memset(&remote, 0, addrLen);
#endif
	if(szBuffer == NULL || m_siSocketId == INVALID_SOCKET) return SIR_NULL;
	
	if(m_bConnected == false)
		return SIR_FAILURE;
		
	if(m_bNonBlockingMode == true){ // 
		siReceived = ::recv(m_siSocketId, (char*)szBuffer, uiLen, 0);
#ifdef SOCKET_LEVEL
		getpeername(m_siSocketId, (sockaddr*)(&remote), &addrLen);
		
		m_pLog->logLn(CLog::LL_0, "[%d]Recv %d length from IP: %s Port: %d", m_siSocketId, uiLen, string(::inet_ntoa(remote.sin_addr)).c_str(), remote.sin_port);
#endif
		switch(siReceived){
			case 0 : //
				m_uiErrorCode = ::WSAGetLastError();
				if(m_uiErrorCode == WSAEWOULDBLOCK) 
					return SIR_WOULD_BLOCK; // 
					
				m_bConnected = false;
				return SIR_CONNECTION_CLOSED;

			case SOCKET_ERROR :
				uiLen         = 0;
				m_uiErrorCode = ::WSAGetLastError();
			    if(m_uiErrorCode == WSAECONNRESET)
                {
					m_bConnected = false;
					return SIR_CONNECTION_CLOSED;
				} 
				else if(m_uiErrorCode == WSAEWOULDBLOCK) 
					return SIR_WOULD_BLOCK; // 
				else 
				{
					m_bConnected = false;
					return SIR_IO_ERROR;
				}
		};
	}
	else{ // 
		while(uiTotalLen < uiLen){
			siReceived = ::recv(m_siSocketId, (char*)(szBuffer + uiTotalLen), uiLen - uiTotalLen, 0);

			switch(siReceived){
				case 0 : // 
					m_bConnected = false;
					return SIR_CONNECTION_CLOSED;

				case SOCKET_ERROR :
					m_uiErrorCode = ::WSAGetLastError();
					m_bConnected = false;
					return SIR_IO_ERROR;
			}

			uiTotalLen += siReceived;
		}
		siReceived = uiTotalLen;
	}

	m_uiBytesReceived +=  (uint32)siReceived;
	uiLen = (uint32)siReceived;
	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::setOption(sint32 siName, sint32 siValue){ // 
	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;

	if(::setsockopt(m_siSocketId, SOL_SOCKET, siName, (char*)(&siValue), sizeof(sint32)) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	}

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::setOption(sint32 siName, bool bValue){ // 
	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;

    sint32 siValue=(bValue?1:0); 
	//if(::setsockopt(m_siSocketId, SOL_SOCKET, siName, (char*)(&bValue), sizeof(bool)) == SOCKET_ERROR){
	if(::setsockopt(m_siSocketId, SOL_SOCKET, siName, (char*)(&siValue), sizeof(siValue)) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	};

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::setTcpKeepAlive(u_long ulOnOff, u_long ulKeepAliveTime, u_long ulKeepAliveInterval){
	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;

	TCP_KEEPALIVE inKeepAlive = {0};
#ifdef WIN32
	TCP_KEEPALIVE outKeepAlive = {0};

	u_long ulInLen = sizeof(TCP_KEEPALIVE);
	u_long ulOutLen = sizeof(TCP_KEEPALIVE);

	u_long ulBytesReturn = 0;
#endif

	inKeepAlive.ulOnOff = ulOnOff;
	inKeepAlive.ulKeepAliveTime = ulKeepAliveTime;
	inKeepAlive.ulKeepAliveInterval = ulKeepAliveInterval;

	if(ulOnOff == 1){
		if(setOption(SO_KEEPALIVE, true) != SIR_OK){
			m_uiErrorCode = ::WSAGetLastError();
			return SIR_FAILURE;
		}
	}

#ifdef _WIN32
	if(::WSAIoctl(m_siSocketId, SIO_KEEPALIVE_VALS, 
		(LPVOID)&inKeepAlive, ulInLen,  
		(LPVOID)&outKeepAlive, ulOutLen,  
		&ulBytesReturn, NULL, NULL) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	}
#else
	int nValue = 1;
	uint32 uiKeepAliveTime = ulKeepAliveTime/1000;
	uint32 uiKeepAliveInterval = ulKeepAliveInterval/1000;
	uint32 uiKeepAliveCount  = 3;
	
	setsockopt(m_siSocketId, SOL_SOCKET, SO_KEEPALIVE, &nValue, sizeof(nValue));
	setsockopt(m_siSocketId, SOL_TCP, TCP_KEEPIDLE, &uiKeepAliveTime, sizeof(uiKeepAliveTime));
	setsockopt(m_siSocketId, SOL_TCP, TCP_KEEPINTVL, &uiKeepAliveInterval, sizeof(uiKeepAliveInterval));
	setsockopt(m_siSocketId, SOL_TCP, TCP_KEEPCNT, &uiKeepAliveCount, sizeof(uiKeepAliveCount));
#endif

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::getOption(sint32 siName, sint32* siValue){ // 
	socklen siSize;

	if(m_siSocketId == INVALID_SOCKET || siValue == NULL) return SIR_NULL;

	siSize = sizeof(sint32);

	if(::getsockopt(m_siSocketId, SOL_SOCKET, siName, (char*)(siValue), &siSize) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	};

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::getOption(sint32 siName, bool* bValue){ // 
	socklen siSize;

	if(m_siSocketId == INVALID_SOCKET || bValue == NULL) return SIR_NULL;

	siSize = sizeof(bool);

	if(::getsockopt(m_siSocketId, SOL_SOCKET, siName, (char*)(bValue), &siSize) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	};

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::setTcpNodelay(bool bValue){ // 
	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;

	if(::setsockopt(m_siSocketId, IPPROTO_TCP, TCP_NODELAY, (char*)(&bValue), sizeof(bool)) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	};

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::getTcpNodelay(bool* bValue){ // 
	socklen siSize;

	if(m_siSocketId == INVALID_SOCKET || bValue == NULL) return SIR_NULL;

	siSize = sizeof(bool);

	if(::getsockopt(m_siSocketId, IPPROTO_TCP, TCP_NODELAY, (char*)(bValue), &siSize) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	};

	return SIR_OK;
}

bool CSocketImpl::getNonBlockingMode(void){
	return m_bNonBlockingMode;
}

CSocketImpl::ESocketImplResult CSocketImpl::setNonBlockingMode(bool bNonBlockingMode){ // 
	u_long ulParam;

	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;

	if(m_bNonBlockingMode != bNonBlockingMode){
		ulParam = bNonBlockingMode;
		
#ifdef _WIN32
		if(::ioctlsocket(m_siSocketId, FIONBIO, &ulParam) == SOCKET_ERROR){ // 
			m_uiErrorCode = ::WSAGetLastError();
			return SIR_FAILURE;
		};
#else
		fcntl(m_siSocketId, F_SETFL, fcntl(m_siSocketId, F_GETFL) | O_NONBLOCK);
#endif
	};

	m_bNonBlockingMode = bNonBlockingMode;

	return SIR_OK;
}

SOCKET CSocketImpl::getSocketId(void){
	return m_siSocketId;
}

bool CSocketImpl::isConnected(void){
	return m_bConnected;
}

sint16 CSocketImpl::getLocalPort(void){ // 
	if(m_iaLocalAddress.isValid() == false) return 0;
	
	return (sint16)m_iaLocalAddress.getPort();
}

sint16 CSocketImpl::getRemotePort(void){ // 
	if(m_iaRemoteAddress.isValid() == false) return 0;
	
	return (sint16)m_iaRemoteAddress.getPort();
}

uint32 CSocketImpl::getSentByte(void){
	return m_uiBytesSent;
}

uint32 CSocketImpl::getReceivedByte(void){
	return m_uiBytesReceived;
}

void CSocketImpl::setSentByte(uint32 uiBytesSent){
	m_uiBytesSent = uiBytesSent;
}

void CSocketImpl::setReceivedByte(uint32 uiBytesReceived){
	m_uiBytesReceived = uiBytesReceived;
}

//CSocketImpl::ESocketImplResult CSocketImpl::setLocalAddress(SOCKET siSocketId){
//	sockaddr_in   local;   
//	int addrLen = sizeof(local);   
//	memset(&local, 0, addrLen);
//
//	int ret = ::getsockname(siSocketId, (sockaddr*)(&local), &addrLen);
//	if(ret == SOCKET_ERROR){
//		m_uiErrorCode = ::WSAGetLastError();
//		::closesocket(siSocketId);
//		return SIR_FAILURE;
//	} 
//
//	m_iaLocalAddress.setSocketAddress(&local);
//
//	return SIR_OK;
//}

CSocketImpl::ESocketImplResult CSocketImpl::setLocalAddress(void){ // 
	sockaddr_in*        psaSocketAddress;
	SOCKET              siSocketId;

	siSocketId = ::socket(AF_INET, SOCK_STREAM, 0);
	if(siSocketId == INVALID_SOCKET) return SIR_NULL;
	
#ifdef _WIN32
	DWORD               dwBytes;
	SOCKET_ADDRESS_LIST salSocketAddressList;
	salSocketAddressList.siCount = 7;

	if(::WSAIoctl(siSocketId, SIO_ADDRESS_LIST_QUERY, NULL, 0, &salSocketAddressList, sizeof(salSocketAddressList), &dwBytes, NULL, NULL) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		::closesocket(siSocketId);
		return SIR_FAILURE;
	};
	
	if(salSocketAddressList.siCount < 1){
		::closesocket(siSocketId);
		return SIR_FAILURE;
	}

	psaSocketAddress = (sockaddr_in*)salSocketAddressList.saSocketAddress[0].lpSockaddr;
#else
	struct ifreq ifr;
	ioctl(siSocketId, SIOCGIFADDR, &ifr);
	psaSocketAddress = (sockaddr_in*)&(ifr.ifr_addr);
#endif
	m_iaLocalAddress.setSocketAddress((sockaddr_in*)psaSocketAddress);

	::closesocket(siSocketId);

	return SIR_OK;
}

string CSocketImpl::getLocalIpAddress(void){ // 
	sockaddr_in*        psaSocketAddress;
	SOCKET              siSocketId;
	string              sIpAddress;

	sIpAddress = "127.0.0.1";

	siSocketId = ::socket(AF_INET, SOCK_STREAM, 0);
	if(siSocketId == INVALID_SOCKET) return sIpAddress;

#ifdef _WIN32
	DWORD               dwBytes;
	SOCKET_ADDRESS_LIST* pSocketAddressList;
	char szBuffer[4096];
	if(::WSAIoctl(siSocketId, SIO_ADDRESS_LIST_QUERY, NULL, 0, szBuffer, 4096, &dwBytes, NULL, NULL) == SOCKET_ERROR) {
		::closesocket(siSocketId);
		return sIpAddress;
	}
	pSocketAddressList = (SOCKET_ADDRESS_LIST*)szBuffer;

	if(pSocketAddressList->siCount < 1){
		::closesocket(siSocketId);
		return sIpAddress;
	}

    psaSocketAddress = (sockaddr_in*)pSocketAddressList->saSocketAddress[0].lpSockaddr;
	sIpAddress       = ::inet_ntoa(psaSocketAddress->sin_addr);
#else
	struct ifreq ifr;
	ioctl(siSocketId, SIOCGIFADDR, &ifr);
	psaSocketAddress = (sockaddr_in*)&(ifr.ifr_addr);
	sIpAddress       = inet_ntoa(psaSocketAddress->sin_addr);
#endif

	::closesocket(siSocketId);

	return sIpAddress;
}

CInetAddress* CSocketImpl::getLocalAddress(void){
	return &m_iaLocalAddress;
}

CInetAddress* CSocketImpl::getRemoteAddress(void){
	return &m_iaRemoteAddress;
}

CSocketImpl::ESocketImplResult CSocketImpl::available(void){ // 
	sint32  siResult;
	timeval tvTime;
	fd_set  fsSet;

	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;

	FD_ZERO(&fsSet);
	FD_SET(m_siSocketId, &fsSet);

	tvTime.tv_sec  = 0;
	tvTime.tv_usec = 100;

	siResult = ::select((uint32)(m_siSocketId + 1), &fsSet, NULL, NULL, &tvTime);
	switch(siResult){
		case  0 : 
			return SIR_TIMEOUT;

		case SOCKET_ERROR :
			m_uiErrorCode = ::WSAGetLastError();
			return SIR_FAILURE;

		default : 
			return SIR_OK;
	};
}

string CSocketImpl::toString(void){ // 
	char   szString[64];
	string sString;
	
	sString = "Invalid Address";

	if(m_iaRemoteAddress.isValid() == true){
		sprintf(szString, "%s:%d", m_iaRemoteAddress.getHostAddress().c_str(), m_iaRemoteAddress.getPort());
		sString = szString;
	};

	return sString;
}

#ifdef _WIN32

CSocketImpl::ESocketImplResult CSocketImpl::createForOverlapped(sint32 siType){ // 
	m_siSocketId = ::WSASocket(AF_INET, siType, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if(m_siSocketId == INVALID_SOCKET){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	};

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::acceptExForOverlapped(CSocketImpl* pSocketImpl, char* szRecvBuffer,
																  DWORD* pdwReceivedBytes, OVERLAPPED* pOverlapped){ // 
	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;
	if(pSocketImpl == NULL || szRecvBuffer == NULL || pdwReceivedBytes == NULL || pOverlapped == NULL) return SIR_NULL;

	if(::AcceptEx(m_siSocketId, pSocketImpl->getSocketId(), szRecvBuffer, 0, // 
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, pdwReceivedBytes, pOverlapped) == false){

		m_uiErrorCode = ::WSAGetLastError();
		if(m_uiErrorCode != ERROR_IO_PENDING) return SIR_FAILURE;
	};

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::getResultForOverlapped(OVERLAPPED* pOverlapped, DWORD* pdwReceivedBytes){ // 
	DWORD dwFlags;

	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;
	if(pOverlapped == NULL || pdwReceivedBytes == NULL) return SIR_NULL;

	if(::WSAGetOverlappedResult(m_siSocketId, (OVERLAPPED*)pOverlapped, pdwReceivedBytes, false, &dwFlags) == false){
		m_uiErrorCode = ::WSAGetLastError();
		return SIR_FAILURE;
	};

	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::recvForOverlapped(WSABUF* pwsaBuffer, DWORD* pdwReceivedBytes, OVERLAPPED* pOverlapped){ // 
	DWORD dwFlags = 0;
#ifdef SOCKET_LEVEL
	sockaddr_in   remote;   
	int addrLen = sizeof(remote);   
	memset(&remote, 0, addrLen);
#endif
	if(m_siSocketId == INVALID_SOCKET) return SIR_NULL;
	if(pwsaBuffer == NULL || pdwReceivedBytes == NULL || pOverlapped == NULL) return SIR_NULL;

	if(::WSARecv(m_siSocketId, pwsaBuffer, 1, pdwReceivedBytes, &dwFlags, pOverlapped, NULL) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		if(m_uiErrorCode != WSA_IO_PENDING) return SIR_FAILURE;
	};

#ifdef SOCKET_LEVEL
	getpeername(m_siSocketId, (sockaddr*)(&remote), &addrLen);

	m_pLog->logLn(CLog::LL_0, "[%d]Recv %d length from IP: %s Port: %d", m_siSocketId, *pdwReceivedBytes, string(::inet_ntoa(remote.sin_addr)).c_str(), remote.sin_port);
#endif
	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::sendForOverlapped(WSABUF* pwsaBuffer, DWORD* pdwSentBytes, OVERLAPPED* pOverlapped){ // 
#ifdef SOCKET_LEVEL
	sockaddr_in   local;   
	int addrLen = sizeof(local);   
	memset(&local, 0, addrLen);
#endif
	if(m_siSocketId == INVALID_SOCKET) return SIR_INVALID_ADDRESS;
	if(pwsaBuffer == NULL || pdwSentBytes == NULL || pOverlapped == NULL) return SIR_NULL;

	if(::WSASend(m_siSocketId, pwsaBuffer, 1, pdwSentBytes, 0, pOverlapped, NULL) == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
		if(m_uiErrorCode != WSA_IO_PENDING) return SIR_FAILURE;
	};
#ifdef SOCKET_LEVEL
	getsockname(m_siSocketId, (sockaddr*)(&local), &addrLen); 

	m_pLog->logLn(CLog::LL_0, "[%d]Send %d length from IP: %s Port: %d", m_siSocketId, *pdwSentBytes, string(::inet_ntoa(local.sin_addr)), local.sin_port);
#endif
	return SIR_OK;
}
#endif

CSocketImpl::ESocketImplResult CSocketImpl::sendTo(char* szBuffer, uint32& uiLen, CInetAddress* pInetAddress){ // 
	if(szBuffer == NULL || m_siSocketId == INVALID_SOCKET || pInetAddress == NULL) return SIR_NULL;

	sint32 siLen = ::sendto(m_siSocketId, (const char*)szBuffer, uiLen, 0, (const sockaddr*)(&pInetAddress->m_saSocketAddress), sizeof(sockaddr_in));
#ifdef SOCKET_LEVEL
	m_pLog->logLn(CLog::LL_0, "m_siSocketId = %d, uiLen = %d, sent to addr(%s), port(%d)", m_siSocketId, uiLen, pInetAddress->getHostAddress().c_str(), pInetAddress->getPort());
#endif
	if(siLen == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
#ifdef SOCKET_LEVEL
		m_pLog->logLn(CLog::LL_0, "m_uiErrorCode = %d", m_uiErrorCode);
#endif

		if(m_uiErrorCode == WSAEWOULDBLOCK){ // 
			uiLen = 0;
			return SIR_WOULD_BLOCK;
		}

		return SIR_IO_ERROR;
	}

	uiLen = (uint32)siLen;
	m_uiBytesSent += uiLen;
	
	return SIR_OK;
}

CSocketImpl::ESocketImplResult CSocketImpl::recvFrom(char* szBuffer, uint32& uiLen, CInetAddress* pInetAddress){ // 
	sockaddr_in saSocketAddress;
	socklen      siSize;

	if(szBuffer == NULL || m_siSocketId == INVALID_SOCKET || pInetAddress == NULL) return SIR_NULL;

	siSize = sizeof(sockaddr_in);

	sint32 siLen = ::recvfrom(m_siSocketId, (char*)szBuffer, uiLen, 0, (sockaddr*)(&saSocketAddress), &siSize);
#ifdef SOCKET_LEVEL
	m_pLog->logLn(CLog::LL_0, "m_siSocketId = %d, uiLen = %d, recv from addr(%s) port(%d)", m_siSocketId, uiLen, ::inet_ntoa(saSocketAddress.sin_addr), ::ntohs(saSocketAddress.sin_port));
#endif
	if(siLen == SOCKET_ERROR){
		m_uiErrorCode = ::WSAGetLastError();
#ifdef SOCKET_LEVEL
		m_pLog->logLn(CLog::LL_0, "m_uiErrorCode = %d", m_uiErrorCode);
#endif

		if(m_uiErrorCode == WSAEWOULDBLOCK){ // 
			uiLen = 0;
			return SIR_WOULD_BLOCK;
		}

		return SIR_IO_ERROR;
	}

	uiLen = (uint32)siLen;
	m_uiBytesReceived += uiLen;

	pInetAddress->setSocketAddress((sockaddr_in*)&saSocketAddress); // 

	return SIR_OK;
}

}
