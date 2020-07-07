///////////////////////////////////////
// 개 발 자 : 김경석
// 설    명 : IOCP 관련 클래스에 대한 구현
///////////////////////////////////////

#ifndef __IOCP_IMPL_H
#define __IOCP_IMPL_H

#include "TypesDn.h"

namespace DnNetworkFramework{

class CIocpImpl{
public:
	enum EIocpImplResult { IIR_NULL, IIR_TIMEOUT, IIR_FAILURE, IIR_OK };

private:
	HANDLE m_hIocp;       // IOCP 객체
	uint32 m_uiErrorCode; // 에러 코드 번호

public:
	CIocpImpl(void);
	virtual ~CIocpImpl(void);

	uint32 getErrorCode(void);

	EIocpImplResult create(uint32);                                       // IOCP 객체를 생성
	EIocpImplResult bindHandle(HANDLE, ULONG_PTR);                        // 주어진 핸들을 키와 함께 IOCP 객체에 바인딩
	EIocpImplResult getStatus(ULONG_PTR*, PDWORD, OVERLAPPED**);          // IOCP 객체로부터 통보를 획득
	EIocpImplResult getStatus(ULONG_PTR*, PDWORD, OVERLAPPED**, DWORD);
	EIocpImplResult postStatus(ULONG_PTR, DWORD = 0, OVERLAPPED* = NULL); // IOCP 객체로 통보를 보냄
};

}

#endif
