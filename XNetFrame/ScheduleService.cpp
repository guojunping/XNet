///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "ScheduleService.h"

namespace XNetFrame{

CScheduleService::CScheduleService(CLog* pLog) : CEventHandlerFactory(),m_pEpoll(NULL),m_pTimerThread(NULL){
	m_pLog        = pLog;

	m_uiHeadPos   = 0; // 
	m_uiTailPos   = 0; // 
	m_uiUsedCount = 0; // 

	m_uiMaxDelayTime = 0; // 
	m_uiMinDelayTime = 0; // 

	m_siRefCount      = 0;     // 
	m_bServiceRunning = false; // 
}

CScheduleService::~CScheduleService(void){
	stopService();
	if(m_pEpoll!=NULL)
	{
		delete m_pEpoll;
		m_pEpoll=NULL;
	}
}

CScheduleService::EScheduleServiceResult CScheduleService::createService(void){ // 
#ifdef WIN32
	TIMECAPS tcTimeCaps;
	CEvent::EEventResult erResult;
#endif
	

	m_uiHeadPos   = 0; // 
	m_uiTailPos   = 0;		// 
	m_uiUsedCount = 0; // 
	
#ifdef WIN32
	erResult = m_IoEvent.reset(); // 
	if(erResult != CEvent::ER_OK) return SSR_FAILURE;
	
	if(::timeBeginPeriod(500) != TIMERR_NOERROR) return SSR_FAILURE; // 

	if(::timeGetDevCaps(&tcTimeCaps, sizeof(TIMECAPS)) != TIMERR_NOERROR) return SSR_FAILURE; // 

	m_uiMaxDelayTime = (uint32)tcTimeCaps.wPeriodMax; // 
	m_uiMinDelayTime = (uint32)tcTimeCaps.wPeriodMin; // 
#else
	m_uiMaxDelayTime=1000000;
	m_uiMinDelayTime=1;
	m_pEpoll=new CEventEpoll();
	m_pEpoll->create(this,500);
	m_pTimerThread=new CTimerThread(this);  
	
#endif
	return SSR_OK;
}

CScheduleService::EScheduleServiceResult CScheduleService::destroyService(void){ // 
#ifdef WIN32
	if(::timeEndPeriod(500) != TIMERR_NOERROR) return SSR_FAILURE; // 
#endif

	unregisterAll(); // 

	return SSR_OK;
}

CScheduleService::EScheduleServiceResult CScheduleService::startService(void){ //
	CScheduleThread*       pScheduleThread;
	EScheduleServiceResult ssrResult;

	m_siRefCount++;

	if(m_bServiceRunning == true) return SSR_ALREADY_SERVICE_RUNNING;

	ssrResult = createService();
	if(ssrResult != SSR_OK) return ssrResult;

	pScheduleThread = new CScheduleThread(this); // 
	if(pScheduleThread == NULL) return SSR_NULL;

    m_liScheduleThreadList.push_back(pScheduleThread);

	pScheduleThread->setPriority(CThread::TP_NORMAL);

	if(pScheduleThread->start() != CThread::TR_OK) return SSR_THREAD_START_FAILURE;
	
	if(m_pTimerThread->start() != CThread::TR_OK) return SSR_THREAD_START_FAILURE;

	onCreateService();

	m_bServiceRunning = true;

	return SSR_OK;
}

CScheduleService::EScheduleServiceResult CScheduleService::stopService(void){ // 
	CScheduleThread*              pScheduleThread;
	EScheduleServiceResult        ssrResult;
	ScheduleThreadList::iterator  itr;

	if(m_siRefCount == 0) return SSR_OK; // 

	m_siRefCount--;

	if(m_bServiceRunning == false) return SSR_ALREADY_SERVICE_STOPPED;

	m_bServiceRunning = false;

	onDestroyService();

	ssrResult = destroyService();
	if(ssrResult != SSR_OK) return ssrResult;

	for(itr = m_liScheduleThreadList.begin(); itr != m_liScheduleThreadList.end(); itr++){ // 
		pScheduleThread = (CScheduleThread*)(*itr);
		pScheduleThread->stop(m_IoEvent.getEvent()); // 

		delete pScheduleThread;
		pScheduleThread = NULL;
	};
	
	if(m_pTimerThread!=NULL)
	{
		m_pTimerThread->stop(); // 
		delete m_pTimerThread;
		m_pTimerThread = NULL;
	}

	itr = m_liScheduleThreadList.begin();
	if(itr != m_liScheduleThreadList.end()) m_liScheduleThreadList.erase(itr, m_liScheduleThreadList.end());
	
	return SSR_OK;
}

CScheduleService::EScheduleServiceResult CScheduleService::addEvent(uint32 uiTimerId){ // 
	uint32 uiEventId;

	CEvent::EEventResult erResult;

	synchronized sync(&m_cMonitor);

	if(m_uiUsedCount == MAX_EVENT_QUEUE_LEN){
		m_pLog->logLn(CLog::LL_2,"addEvent SSR_EVENT_QUEUE_OVERFLOW");
		return SSR_EVENT_QUEUE_OVERFLOW; //
	}

	uiEventId = CEventHandlerFactory::getTimer(uiTimerId); // 
#ifdef WIN32
	CTimerEvent* pTimerEvent=CEventHandlerFactory::getEvent();
	if(pTimerEvent->m_bRepetition==false)
	{
		::timeKillEvent((UINT)pTimerEvent->m_uiTimerId) ; // 
	}
#endif

	m_uiEventQueue[m_uiTailPos++] = uiEventId;

	if(m_uiTailPos == MAX_EVENT_QUEUE_LEN) m_uiTailPos = 0; // 

	m_uiUsedCount++;

	erResult = m_IoEvent.set(); // 
	if(erResult != CEvent::ER_OK){

		m_pLog->logLn(CLog::LL_2,"addEvent m_IoEvent set failure");
		return SSR_FAILURE;
	}

	//m_pLog->logLn(CLog::LL_2,"TimerProc AddEvent Successfully");

	return SSR_OK;
}

CTimerEventHandler* CScheduleService::getRegistedEvent(uint32 uiEventId)
{
	return CEventHandlerFactory::getEvent(uiEventId);
}

uint32 CScheduleService::getEvent(void){ // 
	uint32 uiEventId;

	synchronized sync(&m_cMonitor);

	if(m_uiUsedCount == 0){ // 

#ifdef WIN32
		m_pLog->logLn(CLog::LL_2,"m_IoEvent reset");
		m_IoEvent.reset(); // 
#endif
		return 0;
	};

	uiEventId = m_uiEventQueue[m_uiHeadPos++];

	if(m_uiHeadPos == MAX_EVENT_QUEUE_LEN) m_uiHeadPos = 0; // 

	m_uiUsedCount--;

	//m_pLog->logLn(CLog::LL_2,"uiEventId = %d", uiEventId);

	return uiEventId;
}

#ifdef WIN32
void CALLBACK CScheduleService::TimerProc(UINT uiID, UINT uiMessage, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2){ // 
	CScheduleService* pScheduleService;

	pScheduleService = (CScheduleService*)dwUser; // 

	if(pScheduleService != NULL){
		pScheduleService->addEvent((uint32)uiID); // 
	}
}
#else
void  CScheduleService::eventHandle(sint32 siFd,sint32 siEvents)
{
	 uint64_t exp=0;
	 sint32 siRet= read(siFd, &exp, sizeof(uint64_t));
	 if (siRet != sizeof(uint64_t))
	 {
		 m_pEpoll->del(siFd,siEvents);
		 close(siFd);
	 }

	addEvent((uint32)siFd); //
}
#endif

bool CScheduleService::scheduleEvent(void){ // 
	void*        pEventParameter[2];
	CTimerEventHandler* pTimerEvent;
	uint32       uiEventId;

	uiEventId = getEvent(); // 
	if(uiEventId == 0){
		//m_pLog->logLn(CLog::LL_2, "Leave scheduleEvent no EventId");
		return false;
	}

	//m_pLog->logLn(CLog::LL_0, "Enter scheduleEvent, %d", uiEventId);
    //printf("Enter scheduleEvent, %d\n", uiEventId);
	pTimerEvent = CEventHandlerFactory::getEvent(uiEventId); // 
	if(pTimerEvent == NULL){
		m_pLog->logLn(CLog::LL_0, "Leave scheduleEvent no TimerEvent, %d", uiEventId);
		return false;
	}


	if(pTimerEvent->handleEvent() == 0){ // 
		m_pLog->logLn(CLog::LL_0, "Leave scheduleEvent eventhandler false, %d", uiEventId);
		unregisterEvent(uiEventId); // 
		return true;
	}

	if(pTimerEvent->isRepetition() == false){ //
		m_pLog->logLn(CLog::LL_2, "Leave scheduleEvent bRepetion = false, %d", uiEventId);
		unregisterEvent(uiEventId); // 
		return true;
	}
	else if(pTimerEvent->getRepetitionCount() > 0){ // 
	    uint32 uiRepetitionCount= pTimerEvent->getRepetitionCount();
		pTimerEvent->setRepetitionCount(--uiRepetitionCount) ;

		if(pTimerEvent->getRepetitionCount() == 0){ // 
			m_pLog->logLn(CLog::LL_2, "Leave scheduleEvent repetitionCount = 0, %d", uiEventId);
			unregisterEvent(uiEventId); // 
			return true;
		}
	}
	//printf("Leave scheduleEvent, %d\n", uiEventId);
	//m_pLog->logLn(CLog::LL_2, "Leave scheduleEvent true, %d", uiEventId);

	return true;
}

CScheduleService::EScheduleServiceResult CScheduleService::registerEvent(CTimerEventHandler* pTimerEvent){ // 
	CTimerEventHandler* pValTimerEvent;

	synchronized sync(&m_cMonitor);

	if(pTimerEvent == NULL) return SSR_NULL; // 

	if(pTimerEvent->getEventId() == 0) return SSR_INVALID_PARAMETER;        // 
	if(pTimerEvent->getDeleyTime() < 500) return SSR_INVALID_PARAMETER;     // 

	pValTimerEvent = CEventHandlerFactory::getEvent(pTimerEvent->getEventId()); // 
	if(pValTimerEvent != NULL) return SSR_FAILURE;
	
#ifdef WIN32
	if(pTimerEvent->isRepetition() == false){ // 
		pTimerEvent->setTimerId( (uint32)::timeSetEvent((UINT)pTimerEvent->m_uiDeleyTime, 500, CScheduleService::TimerProc, (DWORD_PTR)this, TIME_ONESHOT));
	}
	else{ // 
		pTimerEvent->setTimerId( (uint32)::timeSetEvent((UINT)pTimerEvent->m_uiDeleyTime, 500, CScheduleService::TimerProc, (DWORD_PTR)this, TIME_PERIODIC));
	};
	
#else
	sint32 siTimerId=(uint32)::CreateTimer(pTimerEvent->getDeleyTime());
	if(siTimerId<0)
	{
		return SSR_FAILURE;
	}
	
	pTimerEvent->setTimerId(  (uint32)siTimerId);
	
	if(!m_pEpoll->add(siTimerId,EPOLLIN))
		return SSR_FAILURE;
		
#endif

	if(pTimerEvent->getTimerId() == 0) return SSR_FAILURE; // 

	if(CEventHandlerFactory::addEvent(pTimerEvent) == false) return SSR_FAILURE; // 

	return SSR_OK;
}

CScheduleService::EScheduleServiceResult CScheduleService::unregisterEvent(uint32 uiEventId){ // 
	CTimerEventHandler* pTimerEvent;

	synchronized sync(&m_cMonitor);

	pTimerEvent = CEventHandlerFactory::getEvent(uiEventId); // 
	if(pTimerEvent == NULL) return SSR_NULL;

	if(pTimerEvent->isRepetition() == true){ // 
#ifdef WIN32
		if(::timeKillEvent((UINT)pTimerEvent->getTimerId()) != TIMERR_NOERROR) return SSR_FAILURE; // 
#else
		 sint32 siFd=(sint32)pTimerEvent->getTimerId();
		 m_pEpoll->del(siFd,EPOLLIN);
		 close(siFd);
#endif
	};

	if(CEventHandlerFactory::deleteEvent(uiEventId) == false) return SSR_FAILURE; // 

	return SSR_OK;
}

void CScheduleService::unregisterAll(void){ // 
	uint32             uiEventId;
	TimerMap::iterator itr;

	for(itr = m_mTimerMap.begin(); itr != m_mTimerMap.end(); itr++){ // 
		uiEventId = (uint32)(*itr).second;
		unregisterEvent(uiEventId); // 
	};

	CEventHandlerFactory::deleteAll(); // 
}

bool CScheduleService::isServiceRunning(void){ // 
	return m_bServiceRunning;
}

}
