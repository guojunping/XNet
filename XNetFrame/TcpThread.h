///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __TCP_THREAD_H
#define __TCP_THREAD_H

#include "TypesDn.h"

#include "Thread.h"
#include "Channel.h"
#include "TcpService.h"

namespace XNetFrame{
#ifdef WIN32
	#define FD_READ_ERROR_CODE(iErrorCode)    (iErrorCode[FD_READ_BIT])
	#define FD_WRITE_ERROR_CODE(iErrorCode)   (iErrorCode[FD_WRITE_BIT])
	#define FD_CLOSE_ERROR_CODE(iErrorCode)   (iErrorCode[FD_CLOSE_BIT])
	#define FD_CONNECT_ERROR_CODE(iErrorCode) (iErrorCode[FD_CONNECT_BIT])

	#define IS_FD_READ(lNetWorkEvents)    ((lNetWorkEvents & FD_READ) == FD_READ)
	#define IS_FD_WRITE(lNetWorkEvents)   ((lNetWorkEvents & FD_WRITE) == FD_WRITE)
	#define IS_FD_CLOSE(lNetWorkEvents)   ((lNetWorkEvents & FD_CLOSE) == FD_CLOSE)
	#define IS_FD_CONNECT(lNetWorkEvents) ((lNetWorkEvents & FD_CONNECT) == FD_CONNECT)
#endif

class CTcpThread : public CThread{
private:
	CChannel*    m_pChannel;    // 
#ifdef WIN32	
	WSAEVENT     m_hIoEvent;    // 
#endif	
	CTcpService* m_pTcpService; // 

public:
	explicit CTcpThread(CTcpService*); // 
	~CTcpThread(void);

	void run(void);
};

}

#endif
