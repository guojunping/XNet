///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "IocpAcceptThread.h"

namespace DnNetworkFramework{

CIocpAcceptThread::CIocpAcceptThread(CIocpService* pIocpService, uint32 uiMaxIocp) : CThread(){
	m_pAcceptIocp  = NULL;
	m_pAsyncIoIocp = NULL;

	m_uiMaxIocp    = uiMaxIocp;
	m_pIocpService = pIocpService;
}

CIocpAcceptThread::~CIocpAcceptThread(void){
}

void CIocpAcceptThread::acquireAddressInfo(CSession* pSession){ // 
	sint32           siLocalLen, siRemoteLen;
	sockaddr_in*     psaRemoteAddress;
	sockaddr_in*     psaLocalAddress;
	CRotativeBuffer* pRotativeBuffer;

	pRotativeBuffer = pSession->getRotativeBuffer(); // 
	if(pRotativeBuffer == NULL) return;

	::GetAcceptExSockaddrs(pRotativeBuffer->getTailRecvPtr(), 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
		(sockaddr**)&psaLocalAddress, &siLocalLen, (sockaddr**)&psaRemoteAddress, &siRemoteLen); // 

	pSession->setLocalAddress((sockaddr_in*)psaLocalAddress);   // 
	pSession->setRemoteAddress((sockaddr_in*)psaRemoteAddress); // 
}

void CIocpAcceptThread::run(void){
	DWORD         dwReceivedBytes;
	OVERLAPPEDEX* pOverlappedEx;
	uint32        uiIocpIndex;
	CSession*     pSession;
	CSocket*      pSocket;

	CIocpImpl::EIocpImplResult iirResult;

	m_pAcceptIocp  = m_pIocpService->m_pAcceptIocp;
	m_pAsyncIoIocp = m_pIocpService->m_pAsyncIoIocp;

	if(m_pAcceptIocp == NULL || m_pAsyncIoIocp == NULL) return; // 

	while(true){
		dwReceivedBytes = 0;
		pSession        = NULL;
		pOverlappedEx   = NULL;

		iirResult = m_pAcceptIocp->getStatus((ULONG_PTR*)&pSession, &dwReceivedBytes, (OVERLAPPED**)&pOverlappedEx);
		// 

		if(pSession == (CSession*)KEY_CLOSE_IOCP){
			return; //
		}

		if(iirResult != CIocpImpl::IIR_OK){
			continue; //
		}

		if(pOverlappedEx == NULL){
			continue; //
		}

		pSession = m_pIocpService->getSession(pOverlappedEx->uiSessionId); // 
		if(pSession == NULL){
			continue; //
		}

		if(pSession->getState() == CSession::SS_SHUTTING_DOWN){
			continue; //
		}

		pSocket = pSession->getSocket();
		if(pSocket == NULL){
			continue; //
		}

		acquireAddressInfo(pSession); // 

		pSocket->setTcpNodelay(true);     // 
		pSocket->setSendBufferSize(0);    // 
		pSocket->setReceiveBufferSize(0); // 
		pSocket->setTcpKeepAlive(m_pIocpService->getKeepAlive(), m_pIocpService->getKeepTime(), m_pIocpService->getKeepInterval());

		uiIocpIndex = pSession->getSessionId() % m_uiMaxIocp; // 

		iirResult = m_pAsyncIoIocp[uiIocpIndex]->bindHandle(reinterpret_cast<HANDLE>(pSession->getSocketId()), (ULONG_PTR)pSession);
		if(iirResult != CIocpImpl::IIR_OK){
			pSession->doClose(CSession::CS_BIND_FAILURE);
			continue; // 
		}

		pSession->setDispatchService(m_pIocpService->getDispatchService()); // 
		pSession->setState(CSession::SS_CONNECTED);                         // 

		m_pIocpService->decAvailableCountEx(); // 

		pSession->onConnect();

		if(pSession->doRecv() != CSession::SR_OK){
			pSession->doClose(CSession::CS_RECV_FAILURE); // 
		}
	}
}

}
