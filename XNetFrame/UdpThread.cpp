///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "UdpThread.h"

namespace XNetFrame{

void udp_func_handler(int sig)
{
	return;
}

CUdpThread::CUdpThread(CUdpService * pUdpService) : CThread(){
	m_pContext    = NULL;
#ifdef WIN32
	m_hIoEvent    = WSA_INVALID_EVENT;
#endif
	m_pUdpService = pUdpService;
}

CUdpThread::~CUdpThread(void){
}

void CUdpThread::run(void){
#ifdef WIN32
	WSANETWORKEVENTS neNetworkEvent;
	DWORD            dwResult;
	sint32           siResult;
#endif

	if(m_pUdpService == NULL) return; // 

	m_pContext = m_pUdpService->getContext(); // 
	
#ifdef WIN32	
	m_hIoEvent = m_pContext->getIoEvent();    // 

	if(m_pContext == NULL || m_hIoEvent == WSA_INVALID_EVENT) return; // 

	while(true){
		dwResult = ::WSAWaitForMultipleEvents(1, &m_hIoEvent, FALSE, WSA_INFINITE, FALSE); // 

		if(CThread::getThreadFlag() == false) break; // 

		if(dwResult != WSA_WAIT_EVENT_0){ // 
			::WSAResetEvent(m_hIoEvent); // 
			continue;
		};

		siResult = ::WSAEnumNetworkEvents(m_pContext->getSocketId(), m_hIoEvent, &neNetworkEvent); // 
		if(siResult != 0){ // 
			::WSAResetEvent(m_hIoEvent); // 
			continue;
		};

		if(m_pContext->getState() == CContext::CS_SHUTTING_DOWN){ // 
			::WSAResetEvent(m_hIoEvent); // 
			continue;
		};

		if(IS_FD_READ(neNetworkEvent.lNetworkEvents) == true) m_pContext->onRecv(FD_READ_ERROR_CODE(neNetworkEvent.iErrorCode));   // 
		if(IS_FD_WRITE(neNetworkEvent.lNetworkEvents) == true) m_pContext->onSend(FD_WRITE_ERROR_CODE(neNetworkEvent.iErrorCode)); // 
	};
#else
	if(m_pContext == NULL ) return; // 
		
	struct pollfd fds;
	struct sigaction sighandler;
	sighandler.sa_handler=udp_func_handler;
	
	fds.fd=m_pContext->getSocketId();
	fds.events=POLLIN|POLLOUT;
	sigaddset(&sighandler.sa_mask,SIGQUIT);
	sigaddset(&sighandler.sa_mask,SIGINT);
	sigaction(SIGUSR1,&sighandler,NULL);
	while(true){
		
		if(CThread::getThreadFlag() == false) break; // 
		
		sint32 siRet=poll(&fds,1,-1);
		if(siRet<0)
		{
			continue;
		}
	
	    if(fds.events==POLLIN)
		{
			m_pContext->onRecv(errno);
		}
		if(fds.events==POLLOUT)
		{
			m_pContext->onSend(errno);
		}
	
	}
#endif
}

}
