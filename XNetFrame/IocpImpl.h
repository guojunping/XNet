///////////////////////////////////////
// �� �� �� : ��漮
// ��    �� : IOCP ���� Ŭ������ ���� ����
///////////////////////////////////////

#ifndef __IOCP_IMPL_H
#define __IOCP_IMPL_H

#include "TypesDn.h"

namespace DnNetworkFramework{

class CIocpImpl{
public:
	enum EIocpImplResult { IIR_NULL, IIR_TIMEOUT, IIR_FAILURE, IIR_OK };

private:
	HANDLE m_hIocp;       // IOCP ��ü
	uint32 m_uiErrorCode; // ���� �ڵ� ��ȣ

public:
	CIocpImpl(void);
	virtual ~CIocpImpl(void);

	uint32 getErrorCode(void);

	EIocpImplResult create(uint32);                                       // IOCP ��ü�� ����
	EIocpImplResult bindHandle(HANDLE, ULONG_PTR);                        // �־��� �ڵ��� Ű�� �Բ� IOCP ��ü�� ���ε�
	EIocpImplResult getStatus(ULONG_PTR*, PDWORD, OVERLAPPED**);          // IOCP ��ü�κ��� �뺸�� ȹ��
	EIocpImplResult getStatus(ULONG_PTR*, PDWORD, OVERLAPPED**, DWORD);
	EIocpImplResult postStatus(ULONG_PTR, DWORD = 0, OVERLAPPED* = NULL); // IOCP ��ü�� �뺸�� ����
};

}

#endif
