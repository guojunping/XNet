///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __SESSION_H
#define __SESSION_H

#include "TypesDn.h"

#include "Log.h"
#include "Socket.h"
#include "Monitor.h"
#include "ServerSocket.h"
#include "RotativeBuffer.h"
#include "DispatchService.h"
#include "AdjustableBuffer.h"

namespace XNetFrame{

#ifdef WIN32

#pragma pack(1)

	struct OVERLAPPEDEX : public WSAOVERLAPPED{
		bool      bIoMode;     // 
		CSession* pSession;    // 
		uint32    uiSessionId; // 

		OVERLAPPEDEX(){
			uiSessionId = 0;
			pSession    = NULL;
			bIoMode     = false;
		}
	};

	#pragma pack()

#endif

class CSession{
public:
	enum SSessionState  { SS_DEAD, SS_ACCEPTING, SS_CONNECTED, SS_RESERVED_DOWN, SS_SHUTTING_DOWN };
	enum ESessionResult { SR_NULL, SR_BUFFER_INIT_FAILURE, SR_INVALID_MODE, SR_CONNECTION_RESET, SR_FAILURE, SR_OK, SR_PACKET_BIG, SR_BUFFER_OVERFLOW, SR_WOULD_BLOCK,SR_POLICY_PACK };
	enum SCloseStatus   { CS_SERVICE_STOP, CS_NULL_FAILURE, CS_RESULT_FAILURE, CS_CONNECTION_RESET, CS_INTERNAL_FAILURE, CS_RECV_FAILURE, CS_SEND_FAILURE,
		CS_BUFFER_FAILURE, CS_HANDLER_FAILURE, CS_DISPATCH_FAILURE, CS_BIND_FAILURE, CS_DOS_FAILURE ,SR_OCCUPIED };

	static const char* szPolicyReq; 
	static const char* szPolicyResp;

public:
	CLog* m_pLog; // 

private:
	time_t        m_tStartTime;
	SSessionState m_emState;          // 
	
	uint32        m_uiSessionId;      // 

	uint32        m_uiErrorCode;      // 

	uint32        m_uiRecvBytes;      // 
	uint32        m_uiSendBytes;      // 
	
#ifdef WIN32
	OVERLAPPEDEX   m_ovRecvOverlapped; // 
	OVERLAPPEDEX   m_ovSendOverlapped; // 
	CServerSocket* m_pServerSocket;    //
#else
	CEpollImpl*    m_pEpollImpl;
#endif


	CMonitor        m_cMonitor;         // 
	CSocket*        m_pSocket;           // 
	CRotativeBuffer*   m_pRotativeBuffer;   // 
	CSessionFactory*   m_pSessionFactory;   // 
	CDispatchService*  m_pDispatchService;  // 
	CAdjustableBuffer* m_pAdjustableBuffer; // 

public:
	CSession(CLog*);
	virtual ~CSession(void);

	SSessionState getState(void);          // 

	void          setStartTime(time_t tTime){ m_tStartTime= tTime; }
	time_t	      getStartTime(){ return m_tStartTime; }

	CSocket*      getSocket(void);          // 
	SOCKET        getSocketId(void) const ; // 
	uint32        getSessionId(void);       // 
	void          setState(SSessionState);  // 
	uint32        getErrorCode(void);       // 

	uint32        getRecvBytes(void);       // 
	uint32        getSendBytes(void);       // 

	bool          isAvailable(void);        // 

	CInetAddress* getRemoteAddress(void);         // 
	void          setLocalAddress(sockaddr_in*);  // 
	void          setRemoteAddress(sockaddr_in*); // 

	CRotativeBuffer* getRotativeBuffer(void);               // 
	void             setDispatchService(CDispatchService*); // 

#ifdef WIN32
	ESessionResult doRecv(void);                        // 
#else
	ESessionResult doRecv(uint32&);                        // 
#endif

#ifdef WIN32
	ESessionResult doRestSend(uint32);                  // 
#else
	ESessionResult doRestSend(uint32&);                  // 
#endif

	ESessionResult doClose(SCloseStatus);               // 
	ESessionResult realClose(SCloseStatus);
	ESessionResult addRecvBuffer(uint32);               // 
	ESessionResult doSend(const char*, uint32);               // 
	ESessionResult doSendPolicyMsg(void);
	ESessionResult doDispatchService(void);             // 
	ESessionResult doDispatchServiceEx(void);           // 
	ESessionResult getMessage(uint32*, char**);         // 
	ESessionResult addMessageEx(char*, uint32);         // 
	ESessionResult getMessageEx(uint32*, char**);       // 
	ESessionResult addRecvBufferEx(char*, uint32);      // 
	
#ifdef WIN32
	ESessionResult initSession(uint32, CServerSocket*, CRotativeBuffer::LBufferLevel, CSessionFactory*); // 
#else
	ESessionResult initSession(uint32, CEpollImpl*, CRotativeBuffer::LBufferLevel, CSessionFactory*); // 
	ESessionResult activeSession(CDispatchService*,uint32,uint32,uint32);
#endif

	virtual void onConnect(void)       = 0; // 
	virtual bool onClose(SCloseStatus) = 0; // 
};

#define INVALID_SESSIONID	0xFFFFFFFF

}

#endif
