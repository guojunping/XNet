///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __UDP_SOCKET_H
#define __UDP_SOCKET_H

#include "TypesDn.h"

#include "SocketImpl.h"

namespace XNetFrame{

class CUdpSocket{
	private:
#ifdef WIN32
	WSAEVENT     m_hIoEvent;    // 
#endif
	CSocketImpl* m_pSocketImpl; // 

	sint32 m_siSendSystemBufferSize;
	sint32 m_siReceiveSystemBufferSize;

public:
	CUdpSocket(void);
	virtual ~CUdpSocket(void);

#ifdef SOCKET_LEVEL
	void setLog(CLog* pLog){ m_pSocketImpl->setLog(pLog); }
#endif

	void setSendSystemBufferSize(sint32);
	void setReceiveSystemBufferSize(sint32);

	void                           close(void);   // 
#ifdef WIN32	
	CSocketImpl::ESocketImplResult eventSelect(long);     // 
	CSocketImpl::ESocketImplResult create(string, uint32, long lEvent = FD_READ | FD_WRITE); // 
#else
	CSocketImpl::ESocketImplResult create(string, uint32); // 
#endif
	CSocketImpl::ESocketImplResult connect(CInetAddress&);                                           // 
	CSocketImpl::ESocketImplResult connect(string&, uint16);
	CSocketImpl::ESocketImplResult bind(string&, uint16); //
	CSocketImpl::ESocketImplResult sendTo(char*, uint32&, CInetAddress*);   // 
	CSocketImpl::ESocketImplResult recvFrom(char*, uint32&, CInetAddress*); // 

	CSocketImpl::ESocketImplResult setSendBufferSize(sint32);
	CSocketImpl::ESocketImplResult setReceiveBufferSize(sint32);

	CSocketImpl::ESocketImplResult getSendBufferSize(sint32*);
	CSocketImpl::ESocketImplResult getReceiveBufferSize(sint32*);
	
#ifdef WIN32	
	WSAEVENT      getIoEvent(void);  // 
#endif	

	SOCKET        getSocketId(void); // 

	uint32        getErrorCode(void);
	TSTRING       getErrorString(uint32);

	virtual void onRecv(uint32) = 0; // 
	virtual void onSend(uint32) = 0; // 
};

}

#endif
