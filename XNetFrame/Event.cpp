///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "Event.h"

namespace XNetFrame{
	

#ifdef WIN32

IOEVENT CEvent::create(LPSECURITY_ATTRIBUTES lpEventAttribute, bool bPassiveReset, bool bInitialState, LPTSTR szName){
	return ::CreateEvent(lpEventAttribute, bPassiveReset, bInitialState, szName);
}

CEvent::CEvent(LPSECURITY_ATTRIBUTES lpEventAttribute, bool bPassiveReset, bool bInitialState){ // 
	m_hEvent = create(lpEventAttribute, bPassiveReset, bInitialState, NULL);
	if(m_hEvent == NULL) throw new CEventException(_T("Event::createEvent"), ::GetLastError());
}

CEvent::CEvent(LPSECURITY_ATTRIBUTES lpEventAttribute, bool bPassiveReset, bool bInitialState, TSTRING& sName){ // 
	m_hEvent = create(lpEventAttribute, bPassiveReset, bInitialState, (LPTSTR)sName.c_str());
	if(m_hEvent == NULL) throw new CEventException(_T("Event::createEvent"), ::WSAGetLastError());
}

#else

CEvent::CEvent(bool bIsShared, sint32 sibInitialState)
{
	sint32 ret=sem_init(&m_hEvent,bIsShared?1:0,sibInitialState);
	if(ret<0) throw new CEventException(_T("Event::createEvent"), ::WSAGetLastError());
}

#endif


CEvent::~CEvent(void){
#ifdef WiN32
   ::CloseHandle(m_hEvent);
#endif
}

#ifdef WIN32
CPassiveResetEvent::CPassiveResetEvent(bool bInitialState) : CEvent(NULL, true, bInitialState){ // 
}

CPassiveResetEvent::CPassiveResetEvent(TSTRING& sName, bool bInitialState) : CEvent(NULL, true, bInitialState, sName){ // 
}

CPassiveResetEvent::~CPassiveResetEvent(void){
}
#endif


#ifdef WIN32
CAutomaticResetEvent::CAutomaticResetEvent(bool bInit) :
CEvent(NULL, false, bInit)
{ // 

}

CAutomaticResetEvent::CAutomaticResetEvent(TSTRING& sName, sint32 siInit) :
CEvent(sName, false, siInit)
{ // 

}

#else

CAutomaticResetEvent::CAutomaticResetEvent(sint32 siInit) :
CEvent(false, siInit)
{ // 

}

CAutomaticResetEvent::CAutomaticResetEvent(bool bIsShared, sint32 siInit) :
CEvent(bIsShared, siInit)
{ // 

}
#endif


CAutomaticResetEvent::~CAutomaticResetEvent(void){
}

IOEVENT CEvent::getEvent(void){
#ifdef WIN32
   return m_hEvent;
#else
   return &m_hEvent;
#endif
}


#ifdef WIN32

CEvent::EEventResult CEvent::reset(void){
   if(::ResetEvent(m_hEvent) == false){
	   m_uiErrorCode = ::GetLastError();

	   return ER_FAILURE;
   };

   return ER_OK;
}

CEvent::EEventResult CEvent::pulse(void){
   if(::PulseEvent(m_hEvent) == false){
	   m_uiErrorCode = ::GetLastError();

	   return ER_FAILURE;
   };

   return ER_OK;
}

#endif

CEvent::EEventResult CEvent::set(void){
	
   #ifdef WIN32
   if(::SetEvent(m_hEvent) == false)
   #else
   if(sem_post(&m_hEvent)<0)
   #endif  
   {
	   m_uiErrorCode = ::WSAGetLastError();

	   return ER_FAILURE;
   };

   return ER_OK;
}

CEvent::EEventResult CEvent::wait(DWORD dwTimeout){ // 

#ifdef WIN32

	DWORD dwResult;
	dwResult = ::WaitForSingleObject(m_hEvent, dwTimeout);
	if(dwResult == WAIT_TIMEOUT) return ER_TIMEOUT; // 
	else if(dwResult == WAIT_OBJECT_0) return ER_OK; // 
	m_uiErrorCode = ::GetLastError();
	
#else

	struct timespec abs_timeout, addor;

	addor.tv_sec = dwTimeout / 1000;
	addor.tv_nsec = (dwTimeout % 1000) * 1000000L;
	clock_gettime(CLOCK_REALTIME, &abs_timeout);
	abs_timeout.tv_nsec += addor.tv_nsec;
	if(abs_timeout.tv_nsec >= 1000000000L) {
		abs_timeout.tv_sec++;
		abs_timeout.tv_nsec -= 1000000000L;
	}
	abs_timeout.tv_sec += addor.tv_sec;
	
	sint32 ret=0;
	while ((ret = sem_timedwait(&m_hEvent, &abs_timeout)) == -1 && errno == EINTR)
               continue;       /* Restart if interrupted by handler */

	if(ret<0)
	{
		if(errno==ETIMEDOUT)
			return ER_TIMEOUT;
		else
			m_uiErrorCode = ::WSAGetLastError();
	}
	else
		return ER_OK;
#endif

	return ER_FAILURE;
}

CEvent::EEventResult CEvent::wait(void){ // 
   
#ifdef WIN32
   return wait(INFINITE);
#else
	sint32 ret=0;
	while ((ret = sem_wait(&m_hEvent) ) == -1 && errno == EINTR)
               continue;       /* Restart if interrupted */
	if(ret<0)
	{
		m_uiErrorCode = ::WSAGetLastError();
		return ER_FAILURE;
	}
	return ER_OK;
#endif

}

#ifndef WIN32
    sint32 CEvent::getEventVal()
	{
		sint32 semval=0;

		if(sem_getvalue(&m_hEvent,&semval)<0)
			return -1;
		return  semval;
	}
#endif

uint32 CEvent::getErrorCode(void){
	return m_uiErrorCode;
}

}
