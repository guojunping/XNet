///////////////////////////////////////
// �� �� �� : ��漮
// ��    �� : IOCP�� ���� ������ �����忡 ���� ����
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
	uint32        m_uiMaxIocp;    // �ִ� IOCP
    CIocpImpl*    m_pAcceptIocp;  // ���� ������ IOCP ��ü
    CIocpImpl**   m_pAsyncIoIocp; // �񵿱� I/O�� IOCP ��ü
	CIocpService* m_pIocpService; // IOCP ���� ��ü

private:
	void acquireAddressInfo(CSession*); // ���� ��ü�� ���� IP �ּ� ������ ȹ��

public:
	explicit CIocpAcceptThread(CIocpService*, uint32); // ������ ������, �⺻ �����ڴ� �������� ����
	~CIocpAcceptThread(void);

	void run(void);
};

}

#endif
