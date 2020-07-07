#ifndef __EPOLL_IMPL_H
#define __EPOLL_IMPL_H

#include "TypesDn.h"
#include "ServerSocket.h"
#include "EpollService.h"

namespace XNetFrame{
	
class CEpollImpl{
	public:

	enum EEpollImplResult { EIR_NULL, EIR_TIMEOUT, EIR_FAILURE, EIR_OK };

	private:

	sint32 m_hEpoll;       // IOCP °ŽÃŒ
	uint32 m_uiErrorCode; // ¿¡·¯ ÄÚµå ¹øÈ£

	sint32 m_siMaxfd;
	uint32 m_uiEpollSize;
	epoll_event* m_pEvents;
	
	public:

	CEpollImpl(void):m_hEpoll(-1),m_uiEpollSize(0),m_pEvents(NULL) { }
	
	virtual ~CEpollImpl(void)
	{
		if(m_pEvents!=NULL)
		{
			delete [] m_pEvents;
			m_pEvents = NULL;
		}
	}

	uint32 getErrorCode(void){ return m_uiErrorCode ; }

	EEpollImplResult create(uint32 uiEpollSize)
	{
		m_uiEpollSize = uiEpollSize;
		
		m_pEvents=new epoll_event[m_uiEpollSize];
		memset(m_pEvents,0,sizeof(epoll_event)*m_uiEpollSize);
		m_hEpoll=epoll_create(m_uiEpollSize);
		if(m_hEpoll==-1)
		{
			m_uiErrorCode=errno;
			return EIR_FAILURE;
		}
			
		return EIR_OK;
	}
	// 
	
	bool add(sint32 siFd,uint32 uiEvents,void *pData=NULL)
	{
		epoll_event polevt;
		polevt.events=uiEvents;
		polevt.data.fd=siFd;
		if(pData!=NULL)
			polevt.data.ptr=pData;
			
		return ctl(EPOLL_CTL_ADD,siFd,polevt)==EIR_OK;
	}
        
	bool del(sint32 siFd,uint32 uiEvents)
	{
		epoll_event polevt;
		polevt.events=uiEvents;
		polevt.data.fd=siFd;
		return ctl(EPOLL_CTL_DEL,siFd,polevt)==EIR_OK;
	}
        
	bool modify(sint32 siFd,uint32 uiEvents)
	{
		epoll_event polevt;
		polevt.events=uiEvents;
		polevt.data.fd=siFd;
		return ctl(EPOLL_CTL_MOD,siFd,polevt)==EIR_OK;
	}
        
	int poll(sint32 timeout=5000)
	{
		m_siMaxfd=epoll_wait(m_hEpoll,m_pEvents,m_uiEpollSize,timeout);
		return m_siMaxfd;
	}
	
	void handleEvents()
	{
		for(int i=0;i<m_siMaxfd;i++)
		{
			 onEvent(m_pEvents[i].data.fd,m_pEvents[i].events,m_pEvents[i].data.ptr);
		}
	}
        
    protected:
        
	EEpollImplResult ctl(sint32 siOpt, sint32 siFd, struct epoll_event &stEvent)
	{
		sint32 ret=epoll_ctl(m_hEpoll,siOpt,siFd,&stEvent);
		if(ret!=0)
		{
			m_uiErrorCode=errno;
			return EIR_FAILURE;
		}
		return EIR_OK;
	}
	
	virtual void onEvent(sint32 siFd,sint32 siEvents,void *pPtr)=0;
};

class  CEpoll :public CEpollImpl
{
	public:

	CEpoll(void);
	virtual ~CEpoll(void);
	
	EEpollImplResult create(CEpollService* ,CServerSocket* ,uint32);
	
	virtual void onEvent(sint32 siFd,sint32 siEvents,void *pPtr);
	
    private:
	
	CEpollService* m_pEpollService;
	CServerSocket* m_pServerSocket;
};

}

#endif