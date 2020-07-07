///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "IocpAsyncIoThread.h"

namespace DnNetworkFramework{

CIocpAsyncIoThread::CIocpAsyncIoThread(CIocpService* pIocpService, uint32 uiIocpIndex) : CThread(){
	m_pAsyncIoIocp = NULL;

	m_uiIocpIndex  = uiIocpIndex;
	m_pIocpService = pIocpService;
}

CIocpAsyncIoThread::~CIocpAsyncIoThread(void){
}

void CIocpAsyncIoThread::run(void){
	DWORD         dwReceivedBytes;
	OVERLAPPEDEX* pOverlappedEx;
	CSession*     pSession;
	CSocket*      pSocket;

	CIocpImpl::EIocpImplResult iirResult;

	m_pAsyncIoIocp = m_pIocpService->m_pAsyncIoIocp[m_uiIocpIndex];

	if(m_pAsyncIoIocp == NULL) return; // 

	while(true){
		dwReceivedBytes = 0;
		pSession        = NULL;
		pOverlappedEx   = NULL;

		iirResult = m_pAsyncIoIocp->getStatus((ULONG_PTR*)&pSession, &dwReceivedBytes, (OVERLAPPED**)&pOverlappedEx); // 


		if(pSession == (CSession*)KEY_CLOSE_IOCP) return; // 

		if(iirResult != CIocpImpl::IIR_OK){ // 
			if(m_pAsyncIoIocp->getErrorCode() == ERROR_NETNAME_DELETED){ // 
				if(pSession != NULL){
#ifdef SOCKET_LEVEL
					pSession->m_pLog->logLn(CLog::LL_0, "[%d] Session reset cuz ERROR_NETNAME_DELETED", pSession->getSessionId());
#endif
					pSession->doClose(CSession::CS_CONNECTION_RESET);
				}
			}

			continue;
		}

		if(pSession == NULL) continue; // 

		//if(pSession->getState() == CSession::SS_SHUTTING_DOWN) continue; // 

		if(pSession->getState() == CSession::SS_SHUTTING_DOWN || pSession->getState() == CSession::SS_DEAD) continue; //lishu

		pSocket = pSession->getSocket();
		if(pSocket == NULL) continue; // 

		if(pSocket->getResultForOverlapped((OVERLAPPED*)pOverlappedEx, &dwReceivedBytes) != CSocketImpl::SIR_OK){ // 
			pSession->doClose(CSession::CS_RESULT_FAILURE);
			continue;
		}

		if(pOverlappedEx == NULL){// 
			pSession->doClose(CSession::CS_NULL_FAILURE);
			continue;
		}

		if(dwReceivedBytes == 0){ // 
#ifdef SOCKET_LEVEL
			pSession->m_pLog->logLn(CLog::LL_0, "[%d] connection reset cuz dwReceivedBytes = 0", pSession->getSessionId());
#endif
			pSession->doClose(CSession::CS_CONNECTION_RESET);
			continue;
		}

		doOverlappedIo(pSession, dwReceivedBytes, pOverlappedEx); //
	}
}

void CIocpAsyncIoThread::doOverlappedIo(CSession* pSession, DWORD dwReceivedBytes, OVERLAPPEDEX* pOverlappedEx){ // I/O를 실시하고 I/O 예약을 설정
	if(pOverlappedEx->bIoMode == false){ // Receive finished
		m_pIocpService->incRecvBytes(dwReceivedBytes);

		if(pSession->addRecvBuffer(dwReceivedBytes) != CSession::SR_OK){ // 
#ifdef SOCKET_LEVEL
			pSession->m_pLog->logLn(CLog::LL_0, "Session addRecvBuffer %d", dwReceivedBytes);
#endif

			pSession->doClose(CSession::CS_BUFFER_FAILURE);
			return;
		}

		CSession::ESessionResult srResult;
		if((srResult = pSession->doDispatchService()) != CSession::SR_OK){ //
#ifdef SOCKET_LEVEL
			pSession->m_pLog->logLn(CLog::LL_0, "Session doDispatchService srResult %d", srResult);
#endif

			pSession->doClose(CSession::CS_DISPATCH_FAILURE);
			return;
		}

		if(pSession->doRecv() != CSession::SR_OK){ // 
#ifdef SOCKET_LEVEL
			pSession->m_pLog->logLn(CLog::LL_0, "Session doRecv failure");
#endif
			pSession->doClose(CSession::CS_RECV_FAILURE);
			return;
		}
	}
	else{ // send finished
		m_pIocpService->incSendBytes(dwReceivedBytes);

		if(pSession->doRestSend(dwReceivedBytes) != CSession::SR_OK){ // 
#ifdef SOCKET_LEVEL
			pSession->m_pLog->logLn(CLog::LL_0, "Session doSend %d", dwReceivedBytes);
#endif
			pSession->doClose(CSession::CS_SEND_FAILURE);
			return;
		}
	}
}

}
