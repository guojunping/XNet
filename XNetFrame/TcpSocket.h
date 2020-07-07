///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __TCP_SOCKET_H
#define __TCP_SOCKET_H

#include "TypesDn.h"

#include "SocketImpl.h"

namespace XNetFrame{

class CTcpSocket{
	private:
	
#ifdef WIN32
	WSAEVENT     m_hIoEvent;    // 
#endif

	CSocketImpl* m_pSocketImpl; // 

public:
	CTcpSocket(void);
	virtual ~CTcpSocket(void);

#ifdef SOCKET_LEVEL
	void setLog(CLog* pLog){ m_pSocketImpl->setLog(pLog); }
#endif

	void                           close(void);                                                      // 
#ifdef WIN32
	CSocketImpl::ESocketImplResult eventSelect(long);   
#endif
	CSocketImpl::ESocketImplResult connect(CInetAddress&);                                           // 
	CSocketImpl::ESocketImplResult connect(string&, uint16);
	CSocketImpl::ESocketImplResult bind(string&, uint16); // 
#ifdef WIN32	
	CSocketImpl::ESocketImplResult create(long lEvent = FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE); // 
#else
	CSocketImpl::ESocketImplResult create();
#endif	
	CSocketImpl::ESocketImplResult setOption(sint32, sint32);
	CSocketImpl::ESocketImplResult setOption(sint32, bool);

	CSocketImpl::ESocketImplResult send(char*, uint32&); // 
	CSocketImpl::ESocketImplResult recv(char*, uint32&); // 

	CSocketImpl::ESocketImplResult setSendBufferSize(sint32);
	CSocketImpl::ESocketImplResult setReceiveBufferSize(sint32);

	CSocketImpl::ESocketImplResult getSendBufferSize(sint32*);
	CSocketImpl::ESocketImplResult getReceiveBufferSize(sint32*);

	CSocketImpl::ESocketImplResult setTcpNodelay(bool);
	CSocketImpl::ESocketImplResult getTcpNodelay(bool*);

	CSocketImpl::ESocketImplResult setNonBlockingMode(bool);

	CSocketImpl::ESocketImplResult setTcpKeepAlive(uint32, uint32, uint32);

	CSocketImpl::ESocketImplResult available(void); // 

	sint16        getPort(void);    // 
	CInetAddress* getAddress(void); // 

	sint16	getLocalPort(void);
	CInetAddress* getLocalAddress(void);

	bool          isConnected(void);

#ifdef WIN32
	WSAEVENT      getIoEvent(void);  // 
#endif

	SOCKET        getSocketId(void); // 

	uint32        getSentByte(void);
	uint32        getReceivedByte(void);

	void          setSentByte(uint32);
	void          setReceivedByte(uint32);

	uint32        getErrorCode(void);
	TSTRING       getErrorString(uint32);

	TSTRING       toString(void); // 

	virtual void onRecv(uint32)    = 0; // 
	virtual void onSend(uint32)    = 0; // 
	virtual void onClose(uint32)   = 0; // 
	virtual void onConnect(uint32) = 0; // 
};

}

#endif
