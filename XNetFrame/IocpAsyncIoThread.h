///////////////////////////////////////
// �� �� �� : ��漮
// ��    �� : IOCP�� �񵿱� I/O�� �����忡 ���� ����
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
	uint32        m_uiIocpIndex;  // IOCP ��ü �ε���
    CIocpImpl*    m_pAsyncIoIocp; // �񵿱� I/O�� IOCP ��ü
	CIocpService* m_pIocpService; // IOCP ���� ��ü

private:
	void doOverlappedIo(CSession*, DWORD, OVERLAPPEDEX*); // I/O�� �ǽ��ϰ� I/O ������ ����

public:
	explicit CIocpAsyncIoThread(CIocpService*, uint32); // ������ ������, �⺻ �����ڴ� �������� ����
	~CIocpAsyncIoThread(void);

	void run(void);
};

}

#endif
