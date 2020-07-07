///////////////////////////////////////
// 개 발 자 : 김경석
// 설    명 : IOCP의 비동기 I/O용 쓰레드에 대한 구현
///////////////////////////////////////

#ifndef __IOCP_ASYNC_IO_THREAD_H
#define __IOCP_ASYNC_IO_THREAD_H

#include "TypesDn.h"

#include "Thread.h"
#include "Socket.h"
#include "Session.h"
#include "IocpImpl.h"
#include "IocpService.h"

namespace DnNetworkFramework{

class CIocpAsyncIoThread : public CThread{
public:
	uint32        m_uiIocpIndex;  // IOCP 객체 인덱스
    CIocpImpl*    m_pAsyncIoIocp; // 비동기 I/O용 IOCP 객체
	CIocpService* m_pIocpService; // IOCP 서비스 객체

private:
	void doOverlappedIo(CSession*, DWORD, OVERLAPPEDEX*); // I/O를 실시하고 I/O 예약을 설정

public:
	explicit CIocpAsyncIoThread(CIocpService*, uint32); // 유일한 생성자, 기본 생성자는 생성되지 않음
	~CIocpAsyncIoThread(void);

	void run(void);
};

}

#endif
