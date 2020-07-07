///////////////////////////////////////
// 개 발 자 : 김경석
// 설    명 : IOCP의 접속 관리용 쓰레드에 대한 구현
///////////////////////////////////////

#ifndef __IOCP_ACCEPT_THREAD_H
#define __IOCP_ACCEPT_THREAD_H

#include "TypesDn.h"

#include "Thread.h"
#include "Session.h"
#include "IocpImpl.h"
#include "IocpService.h"
#include "RotativeBuffer.h"

namespace DnNetworkFramework{

class CIocpAcceptThread : public CThread{
public:
	uint32        m_uiMaxIocp;    // 최대 IOCP
    CIocpImpl*    m_pAcceptIocp;  // 접속 관리용 IOCP 객체
    CIocpImpl**   m_pAsyncIoIocp; // 비동기 I/O용 IOCP 객체
	CIocpService* m_pIocpService; // IOCP 서비스 객체

private:
	void acquireAddressInfo(CSession*); // 세션 객체에 대한 IP 주소 정보를 획득

public:
	explicit CIocpAcceptThread(CIocpService*, uint32); // 유일한 생성자, 기본 생성자는 생성되지 않음
	~CIocpAcceptThread(void);

	void run(void);
};

}

#endif
