///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "TcpThread.h"

namespace XNetFrame{
	
void tcp_func_handler(int sig)
{
	return;
}

CTcpThread::CTcpThread(CTcpService* pTcpService) : CThread(){
	m_pChannel    = NULL;
#ifdef WIN32
	m_hIoEvent    = WSA_INVALID_EVENT;
#endif
	m_pTcpService = pTcpService;
}

CTcpThread::~CTcpThread(void){
}

void CTcpThread::run(void){
#ifdef WIN32
	WSANETWORKEVENTS neNetworkEvent;
	DWORD            dwResult;
	sint32           siResult;
#endif	
	
	if(m_pTcpService == NULL) return; // 

	m_pChannel = m_pTcpService->getChannel(); // 
	
#ifdef WIN32
	m_hIoEvent = m_pChannel->getIoEvent();    // 

	if(m_pChannel == NULL || m_hIoEvent == WSA_INVALID_EVENT) return; // 

	while(true){
		dwResult = ::WSAWaitForMultipleEvents(1, &m_hIoEvent, FALSE, WSA_INFINITE, FALSE); // 

		if(CThread::getThreadFlag() == false) break; // 

		if(dwResult != WSA_WAIT_EVENT_0){ // 
			::WSAResetEvent(m_hIoEvent); // 
			continue;
		};

		siResult = ::WSAEnumNetworkEvents(m_pChannel->getSocketId(), m_hIoEvent, &neNetworkEvent); // 
		if(siResult != 0){ // 
			::WSAResetEvent(m_hIoEvent); // 
			continue;
		};

		if(m_pChannel->getState() == CChannel::CS_SHUTTING_DOWN){ // 
			::WSAResetEvent(m_hIoEvent); // 
			continue;
		};

		if(IS_FD_READ(neNetworkEvent.lNetworkEvents) == true) m_pChannel->onRecv(FD_READ_ERROR_CODE(neNetworkEvent.iErrorCode));          // 
		if(IS_FD_WRITE(neNetworkEvent.lNetworkEvents) == true) m_pChannel->onSend(FD_WRITE_ERROR_CODE(neNetworkEvent.iErrorCode));        // 
		if(IS_FD_CLOSE(neNetworkEvent.lNetworkEvents) == true) m_pChannel->onClose(FD_CLOSE_ERROR_CODE(neNetworkEvent.iErrorCode));       // 
		if(IS_FD_CONNECT(neNetworkEvent.lNetworkEvents) == true) m_pChannel->onConnect(FD_CONNECT_ERROR_CODE(neNetworkEvent.iErrorCode)); // 
	};
	
#else

	if(m_pChannel == NULL ) return; // 
		
	struct sigaction sighandler;
	sighandler.sa_handler=tcp_func_handler;
	sigaction(SIGUSR1,&sighandler,NULL);
	
	struct pollfd* pFdSet;
	while(true){
		
		if(CThread::getThreadFlag() == false) break; // 
		
		pFdSet=m_pChannel->getPollEvent();
		
		sint32 siRet=poll(pFdSet,1,-1);
		if(siRet<0)
		{
			if(errno==EINTR)
				continue;
				
			m_pChannel->onClose(errno); 
			break;
		}
		
	    if(pFdSet->revents&POLLERR)
		{
			m_pChannel->onClose(errno);
			break;
		}
	    else if(pFdSet->revents&POLLIN)
		{
			m_pChannel->onRecv(0);
		}
		else if(pFdSet->revents&POLLOUT)
		{
			if(m_pChannel->getState() != CChannel::CS_CONNECTED)
				m_pChannel->onConnect(0);
			else
				m_pChannel->onSend(0);
		}
	}
	
#endif
}

}
