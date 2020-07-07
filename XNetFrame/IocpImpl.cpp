///////////////////////////////////////
// �� �� �� : ��漮
// ��    �� : IOCP ���� Ŭ������ ���� ����
///////////////////////////////////////

#include "IocpImpl.h"

namespace DnNetworkFramework{

CIocpImpl::CIocpImpl(void){
	m_hIocp = NULL;
}

CIocpImpl::~CIocpImpl(void){
   ::CloseHandle(m_hIocp);
}

CIocpImpl::EIocpImplResult CIocpImpl::create(uint32 uiNumThread){ // IOCP ��ü�� ����
	m_hIocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, uiNumThread);
	if(m_hIocp == NULL){
		m_uiErrorCode = ::GetLastError();

		return IIR_FAILURE;
	};

	return IIR_OK;
}

CIocpImpl::EIocpImplResult CIocpImpl::bindHandle(HANDLE hHandle, ULONG_PTR puiCompletionKey){ // �־��� �ڵ��� Ű�� �Բ� IOCP ��ü�� ���ε�
	if(m_hIocp == NULL || hHandle == NULL) return IIR_NULL;

	m_hIocp = ::CreateIoCompletionPort(hHandle, m_hIocp, puiCompletionKey, 0);
	if(m_hIocp == NULL){
		m_uiErrorCode = ::GetLastError();

		return IIR_FAILURE;
	};

	return IIR_OK;
}

CIocpImpl::EIocpImplResult CIocpImpl::getStatus(ULONG_PTR* ppuiCompletionKey, DWORD* pdwReceivedBytes, OVERLAPPED** ppOverlapped){ // IOCP ��ü�κ��� �뺸�� ȹ��
	if(m_hIocp == NULL || ppuiCompletionKey == NULL || ppOverlapped == NULL) return IIR_NULL;

	if(::GetQueuedCompletionStatus(m_hIocp, pdwReceivedBytes, ppuiCompletionKey, ppOverlapped, INFINITE) == false){
		m_uiErrorCode = ::GetLastError();

		return IIR_FAILURE;
	};

	return IIR_OK;
}

CIocpImpl::EIocpImplResult CIocpImpl::getStatus(ULONG_PTR* ppuiCompletionKey, DWORD* pdwReceivedBytes, OVERLAPPED** ppOverlapped, DWORD dwTimeout){
	if(m_hIocp == NULL || ppuiCompletionKey == NULL || ppOverlapped == NULL) return IIR_NULL;

	if(::GetQueuedCompletionStatus(m_hIocp, pdwReceivedBytes, ppuiCompletionKey, ppOverlapped, dwTimeout) == false){
		m_uiErrorCode = ::GetLastError();
		if(m_uiErrorCode == WAIT_TIMEOUT) return IIR_TIMEOUT; // Ÿ�Ӿƿ��� �߻��� ���

		return IIR_FAILURE;
	};

	return IIR_OK;
}

CIocpImpl::EIocpImplResult CIocpImpl::postStatus(ULONG_PTR puiCompletionKey, DWORD dwReceivedBytes, OVERLAPPED* pOverlapped){ // IOCP ��ü�� �뺸�� ����
	if(m_hIocp == NULL || puiCompletionKey == NULL) return IIR_NULL;

	if(::PostQueuedCompletionStatus(m_hIocp, dwReceivedBytes, puiCompletionKey, pOverlapped) == false){
		m_uiErrorCode = ::GetLastError();

		return IIR_FAILURE;
	};

	return IIR_OK;
}

uint32 CIocpImpl::getErrorCode(void){
	return m_uiErrorCode;
}

}
