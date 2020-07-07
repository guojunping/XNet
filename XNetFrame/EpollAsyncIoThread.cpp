///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "EpollAsyncIoThread.h"

namespace XNetFrame{

CEpollAsyncIoThread::CEpollAsyncIoThread(CEpollService* pEpollService) : CThread(){
	m_pAsyncIoEpoll = NULL;

	m_pEpollService = pEpollService;
}

CEpollAsyncIoThread::~CEpollAsyncIoThread(void){
}

void CEpollAsyncIoThread::run(void){

	//CEpollImpl::EEpollImplResult iirResult;

	m_pAsyncIoEpoll = m_pEpollService->m_pAsyncIoEpoll;

	if(m_pAsyncIoEpoll == NULL) return; // 
    sint32 siRet=-1;
	while(true){

		siRet=m_pAsyncIoEpoll->poll(-1);
		if(siRet==-1)
		{
			if(errno==EINTR)
				continue;
				
			 perror("epoll_pwait");
			 exit(-1);
		}
		else if(siRet==0)
		{
			continue;
		}
			
		m_pAsyncIoEpoll->handleEvents();
	}
}

}