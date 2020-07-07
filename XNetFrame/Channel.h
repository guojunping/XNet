///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __CHANNEL_H
#define __CHANNEL_H

#include "TypesDn.h"

#include "Log.h"
#include "Monitor.h"
#include "TcpSocket.h"
#include "RotativeBuffer.h"
#include "DispatchService.h"

namespace XNetFrame{

class CChannel : public CTcpSocket{
public:
	enum SChannelState  { CS_DEAD, CS_CONNECTED, CS_SHUTTING_DOWN };
	enum EChannelResult { CR_NULL, CR_BUFFER_INIT_FAILURE, CR_FAILURE, CR_PACKET_BIG, CR_BUFFER_OVERFLOW, CR_INVALID_MODE, CR_WOULD_BLOCK, CR_OK };
	enum SCloseStatus   { CS_HANDLER_FAILURE, CS_INTERNAL_FAILURE, CS_UNREACHABLE_FAILURE, CS_TIMEOUT_FAILURE, CS_CONNECTION_REFUSE, CS_INVALID_ADDRESS,
		CS_RECV_FAILURE, CS_SERVICE_STOP, CS_CONNECTION_RESET, CS_BUFFER_FAILURE, CS_DISPATCH_FAILURE, CS_SEND_FAILURE, CS_SYSTEM_FAILURE};
protected:
	CLog*             m_pLog;             // 
private:
	CMonitor m_cMonitor; // 

	SChannelState m_emState; // 

	CRotativeBuffer*  m_pRotativeBuffer;  // 
	
	CDispatchService* m_pDispatchService; // 
	
#ifndef WIN32
	struct pollfd m_pdFdSet;
#endif 

private:
	EChannelResult addRecvBuffer(uint32);   // 
	EChannelResult doDispatchService(void); // 

public:
	CChannel(CLog*);
	~CChannel(void);

	SChannelState getState(void);          // 
	bool          isAvailable(void);       // 
	void          setState(SChannelState); // 
	
#ifndef WIN32
     struct pollfd* getPollEvent(){ return &m_pdFdSet ; }
     void setPollEvent(sint32 siFd,sint16 siFdEvent){  m_pdFdSet.fd=siFd; m_pdFdSet.events=siFdEvent; }
#endif

	EChannelResult doRestSend(void);                                              // 
	EChannelResult doClose(SCloseStatus);                                         // 
	EChannelResult doSend(char*, uint32);                                         // 
	EChannelResult doConnect(string&, uint32);                                    // 
	EChannelResult getMessage(uint32*, char**);                                   // 
	EChannelResult initChannel(CDispatchService*, CRotativeBuffer::LBufferLevel); // 

	void onRecv(uint32);    // 
	void onSend(uint32);    // 
	void onClose(uint32);   // 
	void onConnect(uint32); // 

	virtual void onConnect(void)       = 0; // 
	virtual void onClose(SCloseStatus) = 0; // 
};

}

#endif
