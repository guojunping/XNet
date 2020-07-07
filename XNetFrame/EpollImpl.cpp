#include "EpollImpl.h"


namespace XNetFrame{
	
CEpoll::CEpoll(void):CEpollImpl(),m_pServerSocket(NULL)
{

}

CEpoll::~CEpoll(void){

}

	
CEpoll::EEpollImplResult CEpoll::create(CEpollService* pEpollService,CServerSocket* pServerSocket,uint32 uiEpollSize)
{

	if(uiEpollSize==0)
		return EIR_FAILURE;
		
	m_pEpollService=pEpollService;
	if(m_pEpollService==NULL)
	{
		return EIR_NULL;
	}
		
	m_pServerSocket=pServerSocket;
	
	if(m_pServerSocket==NULL)
		return EIR_NULL;
		
	
	if(CEpollImpl::create(uiEpollSize)!=EIR_OK)
	{
		return EIR_FAILURE;
	}
	else
	{	
		if(!add(m_pServerSocket->getSocketId(),EPOLLIN|EPOLLET))
			return EIR_FAILURE;
	}
		
	return EIR_OK;
}

void CEpoll::onEvent(sint32 siFd,sint32 siEvents,void* pPtr)
{	
   m_pEpollService->eventHandle(siFd,siEvents,pPtr);
}

}