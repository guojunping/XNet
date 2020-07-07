///////////////////////////////////////
// °³ ¹ß ÀÚ : ±è°æŒ®
// Œ³    ží : IOCPÀÇ ºñµ¿±â I/O¿ë Ÿ²·¹µå¿¡ ŽëÇÑ ±žÇö
///////////////////////////////////////

#ifndef __EPOLL_ASYNC_IO_THREAD_H
#define __EPOLL_ASYNC_IO_THREAD_H

#include "TypesDn.h"

#include "Thread.h"
#include "Socket.h"
#include "Session.h"
#include "EpollImpl.h"
#include "EpollService.h"

namespace XNetFrame{

class CEpollAsyncIoThread : public CThread{
public:

    CEpoll*    m_pAsyncIoEpoll; // ºñµ¿±â I/O¿ë IOCP °ŽÃŒ
	CEpollService* m_pEpollService; // IOCP Œ­ºñœº °ŽÃŒ

public:
	explicit CEpollAsyncIoThread(CEpollService*); // À¯ÀÏÇÑ »ýŒºÀÚ, ±âº» »ýŒºÀÚŽÂ »ýŒºµÇÁö ŸÊÀœ
	~CEpollAsyncIoThread(void);

	void run(void);
};

}

#endif