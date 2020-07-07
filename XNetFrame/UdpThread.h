///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __UDP_THREAD_H
#define __UDP_THREAD_H

#include "TypesDn.h"

#include "Thread.h"
#include "Context.h"
#include "UdpService.h"

namespace XNetFrame{
#ifdef WIN32
	#define FD_READ_ERROR_CODE(iErrorCode)  (iErrorCode[FD_READ_BIT])
	#define FD_WRITE_ERROR_CODE(iErrorCode) (iErrorCode[FD_WRITE_BIT])

	#define IS_FD_READ(lNetWorkEvents)  ((lNetWorkEvents & FD_READ) == FD_READ)
	#define IS_FD_WRITE(lNetWorkEvents) ((lNetWorkEvents & FD_WRITE) == FD_WRITE)
#endif

class CUdpThread : public CThread{
	private:

	CContext*    m_pContext;    // 
	
#ifdef WIN32
	WSAEVENT     m_hIoEvent;    // 
#endif

	CUdpService * m_pUdpService; // 

public:
	explicit CUdpThread(CUdpService * pUdpService); // 
	~CUdpThread(void);

	void run(void);
};

}

#endif
