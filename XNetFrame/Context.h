////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __CONTEXT_H
#define __CONTEXT_H

#include "TypesDn.h"

#include "Log.h"
#include "Monitor.h"
#include "UdpSocket.h"
#include "InetAddress.h"
#include "RotativeBuffer.h"
#include "DispatchService.h"

namespace XNetFrame{

class CContext : public CUdpSocket{
public:
	enum SContextState  { CS_DEAD, CS_CONNECTED, CS_SHUTTING_DOWN };
	enum EContextResult { CR_NULL, CR_BUFFER_INIT_FAILURE, CR_FAILURE, CR_PACKET_BIG, CR_BUFFER_OVERFLOW, CR_INVALID_MODE, CR_WOULD_BLOCK, CR_OK };
	enum SCloseStatus   { CS_HANDLER_FAILURE, CS_INTERNAL_FAILURE, CS_UNREACHABLE_FAILURE, CS_TIMEOUT_FAILURE, CS_CONNECTION_REFUSE, CS_INVALID_ADDRESS,
		CS_RECV_FAILURE, CS_SERVICE_STOP, CS_CONNECTION_RESET, CS_BUFFER_FAILURE, CS_DISPATCH_FAILURE, CS_SEND_FAILURE, CS_SYSTEM_FAILURE};

private:
	string m_sHost;  // 
	uint32 m_uiPort; // 

	SContextState m_emState; // 

	CMonitor     m_cMonitor;      // 
	CInetAddress m_iaInetAddress; // 

	CLog*             m_pLog;             // 
	CRotativeBuffer*  m_pRotativeBuffer;  // 
	CDispatchService* m_pDispatchService; // 

private:
	EContextResult doDispatchService(void);              // 
	EContextResult addRecvBuffer(uint32, CInetAddress*); // 

public:
	CContext(CLog*);
	~CContext(void);

	SContextState getState(void);          // 
	void          setState(SContextState); // 

	EContextResult doClose(SCloseStatus);                              
	EContextResult doConnect(string&, uint32);// 
	EContextResult doSend(char*, uint32, CInetAddress*);                                           // 
	EContextResult getMessage(uint32*, char**, CInetAddress*);                                     // 
	EContextResult initContext(string&, uint32, CDispatchService*, CRotativeBuffer::LBufferLevel); // 

	void onRecv(uint32); // 
	void onSend(uint32); // 
};

}

#endif
