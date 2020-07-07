///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __SOCKET_IMPL_H
#define __SOCKET_IMPL_H

#include "TypesDn.h"

#include "InetAddress.h"
#include "AuthCrypt.h"

namespace XNetFrame{

#define  SIO_KEEPALIVE_VALS  IOC_IN | IOC_VENDOR | 4

#ifdef _WIN32
struct SOCKET_ADDRESS_LIST{ // 
	sint32         siCount;
	SOCKET_ADDRESS saSocketAddress[7];
};
#endif

struct TCP_KEEPALIVE{
	u_long		ulOnOff;
	u_long		ulKeepAliveTime;
	u_long		ulKeepAliveInterval;
};

class CSocketImpl{
#ifdef SOCKET_LEVEL
private:
	CLog*	m_pLog;
#endif
public:
	enum ESocketImplResult { SIR_OK = 0, SIR_WOULD_BLOCK, SIR_CONNECTION_CLOSED, SIR_IO_ERROR, SIR_FAILURE, SIR_NULL, SIR_INVALID_ADDRESS, SIR_TIMEOUT };

	CInetAddress m_iaLocalAddress;  // 
	CInetAddress m_iaRemoteAddress; // 

private:
	SOCKET m_siSocketId;       // 
	bool   m_bConnected;       // 
	uint32 m_uiErrorCode;      // 
	bool   m_bNonBlockingMode; // 
	
	uint32 m_uiBytesSent;      // 
	uint32 m_uiBytesReceived;  // 

#ifdef _WIN32
	static bool m_bInitialized; // 
#endif

private:
	ESocketImplResult setLocalAddress(void); // 
	//ESocketImplResult setLocalAddress(SOCKET);

public:
	CSocketImpl(void);
	virtual ~CSocketImpl(void);

#ifdef _WIN32
	static bool    initWinsock(void);    // 
	static bool    isInitialized(void);
	static void    releaseWinsock(void); // 
#endif

#ifdef SOCKET_LEVEL
	void setLog(CLog* pLog) { m_pLog = pLog; }
#endif

	uint32         getErrorCode(void);
	void           setErrorCode(uint32);
	static string  getErrorString(uint32);

	void              close(void);                // 
	ESocketImplResult create(sint32);             // 
	void              init(SOCKET sSocket);       //
	ESocketImplResult bind(CInetAddress&);        // 
	ESocketImplResult accept(CSocketImpl*);       // 
	ESocketImplResult bind(string&, uint16);      // 
	ESocketImplResult connect(CInetAddress&);     // 
	ESocketImplResult connect(string&, uint16);   // 
	ESocketImplResult listen(uint32 = SOMAXCONN);


	ESocketImplResult send(char*, uint32&);  // 
	ESocketImplResult recv(char*, uint32&);  // 

	ESocketImplResult setOption(sint32, sint32);  // 
	ESocketImplResult setOption(sint32, bool);    // 

	ESocketImplResult getOption(sint32, sint32*); // 
	ESocketImplResult getOption(sint32, bool*);   // 

	ESocketImplResult setTcpNodelay(bool);  // 
	ESocketImplResult getTcpNodelay(bool*); // 

	ESocketImplResult setTcpKeepAlive(u_long, u_long, u_long);

	bool              getNonBlockingMode(void);
	ESocketImplResult setNonBlockingMode(bool); // 

	SOCKET getSocketId(void);
	bool   isConnected(void);
	sint16 getLocalPort(void);  // 
	sint16 getRemotePort(void); // 

	uint32 getSentByte(void);
	uint32 getReceivedByte(void);

	void   setSentByte(uint32);
	void   setReceivedByte(uint32);

	static string getLocalIpAddress(void); // 

	CInetAddress* getLocalAddress(void);
	CInetAddress* getRemoteAddress(void);

	ESocketImplResult available(void); // 

	string toString(void); // 

#ifdef _WIN32
	ESocketImplResult createForOverlapped(sint32);                                     // 
	ESocketImplResult getResultForOverlapped(OVERLAPPED*, DWORD*);                     // 
	ESocketImplResult recvForOverlapped(WSABUF*, DWORD*, OVERLAPPED*);                 // 
	ESocketImplResult sendForOverlapped(WSABUF*, DWORD*, OVERLAPPED*);                 // 
	ESocketImplResult acceptExForOverlapped(CSocketImpl*, char*, DWORD*, OVERLAPPED*); // 
#endif

	ESocketImplResult sendTo(char*, uint32&, CInetAddress*);   // 
	ESocketImplResult recvFrom(char*, uint32&, CInetAddress*); // 
};

}

#endif
