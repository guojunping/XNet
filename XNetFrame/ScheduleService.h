///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __SCHEDULE_SERVICE_H
#define __SCHEDULE_SERVICE_H

#include "TypesDn.h"

#include "Log.h"
#include "Event.h"
#include "Monitor.h"
#include "EpollImpl.h"
#include "ScheduleThread.h"
#include "TimerEventHandler.h"
#include "EventHandlerFactory.h"

namespace XNetFrame{

class CTimerThread;
class CEventEpoll;

typedef list<CScheduleThread*> ScheduleThreadList;

#define MAX_EVENT_QUEUE_LEN (1024)

class CScheduleService : public CEventHandlerFactory{
public:
	enum EScheduleServiceResult { SSR_NULL, SSR_THREAD_START_FAILURE, SSR_FAILURE, SSR_EVENT_QUEUE_OVERFLOW, SSR_INVALID_PARAMETER, SSR_ALREADY_SERVICE_RUNNING, SSR_ALREADY_SERVICE_STOPPED, SSR_OK };

#ifdef WIN32
	CPassiveResetEvent m_IoEvent; // 
#else
	CAutomaticResetEvent m_IoEvent; 
	CEventEpoll*         m_pEpoll;
	CTimerThread*        m_pTimerThread;  
#endif

private:
	CLog*    m_pLog;        // 
	CMonitor m_cMonitor;    // 

	uint32   m_uiHeadPos;   // 
	uint32   m_uiTailPos;   // 
	uint32   m_uiUsedCount; // 

	uint32   m_uiMaxDelayTime; // 
	uint32   m_uiMinDelayTime; // 

	sint32   m_siRefCount;      // 
	bool     m_bServiceRunning; // 

	uint32   m_uiEventQueue[MAX_EVENT_QUEUE_LEN + 1]; // 

	ScheduleThreadList m_liScheduleThreadList; // 

private:
	uint32                 getEvent(void);   // 
	EScheduleServiceResult addEvent(uint32); // 

	EScheduleServiceResult createService(void);  // 
	EScheduleServiceResult destroyService(void); // 
#ifdef WIN32
	static void CALLBACK TimerProc(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR); // 
#else
public:
	void  eventHandle(sint32 ,sint32);
#endif

public:
	explicit CScheduleService(CLog*); // 
	~CScheduleService(void);

	bool scheduleEvent(void); // 

	bool isServiceRunning(void); // 

	EScheduleServiceResult stopService(void);  // 
	EScheduleServiceResult startService(void); // 

	void                   unregisterAll(void);        // 
	EScheduleServiceResult unregisterEvent(uint32);    // 
	EScheduleServiceResult registerEvent(CTimerEventHandler*); // 
	CTimerEventHandler* getRegistedEvent(uint32);

	virtual void onCreateService(void)  = 0;
	virtual void onDestroyService(void) = 0; // 
};

class  CEventEpoll :public CEpollImpl
{
	public:

	CEventEpoll(void):m_pScheduleService(NULL)
	{
	}
	
	virtual ~CEventEpoll(void)
	{
	}
	
	EEpollImplResult create(CScheduleService* pScheduleService, uint32 uiEpollSize)
	{
		m_pScheduleService=pScheduleService;
		if(CEpollImpl::create(uiEpollSize)!=EIR_OK)
		{
			return EIR_FAILURE;
		}
		return EIR_OK;
	}
	
	virtual void onEvent(sint32 siFd,sint32 siEvents,void* pPtr)
	{
		m_pScheduleService->eventHandle(siFd,siEvents);
	}
	
    private:
	
	CScheduleService* m_pScheduleService;
};

class CTimerThread : public CThread{
private:
	CScheduleService* m_pScheduleService; // 

public:
	explicit CTimerThread(CScheduleService* pScheduleService):m_pScheduleService(pScheduleService){}
	~CTimerThread(void){};

	void run(void)
	{
		//CEpollImpl::EEpollImplResult iirResult;
		
		if(m_pScheduleService->m_pEpoll == NULL) return; // 
		sint32 siRet=-1;
		while(true){

			siRet=m_pScheduleService->m_pEpoll->poll(-1);
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
				
			m_pScheduleService->m_pEpoll->handleEvents();
		}
	}
};


}

#endif
