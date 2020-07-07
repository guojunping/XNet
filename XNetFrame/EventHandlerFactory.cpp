///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "EventHandlerFactory.h"
#include "TimerEventHandler.h"

namespace XNetFrame{

CEventHandlerFactory::CEventHandlerFactory(void){
}

CEventHandlerFactory::~CEventHandlerFactory(void){
	deleteAll();
}

uint32 CEventHandlerFactory::getCount(void){ // 
	return (uint32)m_mEventMap.size();
}

void CEventHandlerFactory::deleteAll(void){ // 
	CTimerEventHandler*       pTimerEvent;
	TimerMap::iterator itr1;
	EventMap::iterator itr2;

	itr1 = m_mTimerMap.begin();
	if(itr1 != m_mTimerMap.end()) m_mTimerMap.erase(itr1, m_mTimerMap.end());

	for(itr2 = m_mEventMap.begin(); itr2 != m_mEventMap.end(); itr2++){
		pTimerEvent = (CTimerEventHandler*)(*itr2).second;
		delete pTimerEvent;
		pTimerEvent = NULL;
	}

	itr2 = m_mEventMap.begin();
	if(itr2 != m_mEventMap.end()) m_mEventMap.erase(itr2, m_mEventMap.end());
}

uint32 CEventHandlerFactory::getTimer(uint32 uiTimerId){ // 
	uint32             uiEventId;
	TimerMap::iterator itr;

	itr = m_mTimerMap.find(uiTimerId);
	
	if(itr == m_mTimerMap.end()) uiEventId = 0;
	else uiEventId = (uint32)(*itr).second;

	return uiEventId;
}

CTimerEventHandler* CEventHandlerFactory::getEvent(uint32 uiEventId){ // 
	CTimerEventHandler*       pTimerEvent;
	EventMap::iterator itr;

	itr = m_mEventMap.find(uiEventId);
	
	if(itr == m_mEventMap.end()) pTimerEvent = NULL;
	else pTimerEvent = (CTimerEventHandler*)(*itr).second;

	return pTimerEvent;
}

bool CEventHandlerFactory::deleteEvent(uint32 uiEventId){ // 
	CTimerEventHandler*       pTimerEvent;
	uint32             uiTimerId;
	EventMap::iterator itr;

	itr = m_mEventMap.find(uiEventId);
	if(itr == m_mEventMap.end()) return false; // 
	
	pTimerEvent = (CTimerEventHandler*)(*itr).second;
	uiTimerId   = pTimerEvent->getTimerId();

	delete pTimerEvent;
	pTimerEvent = NULL;

	m_mTimerMap.erase(uiTimerId);
	m_mEventMap.erase(uiEventId);

	return true;
}

bool CEventHandlerFactory::addEvent(CTimerEventHandler* pTimerEvent){ // 
	uint32             uiTimerId, uiEventId;
	EventMap::iterator itr;

	if(pTimerEvent == NULL) return false;

	uiTimerId = pTimerEvent->getTimerId();
	uiEventId = pTimerEvent->getEventId();

	if(uiTimerId == 0 || uiEventId == 0) return false;     // 

	itr = m_mEventMap.find(uiEventId);
	if(itr != m_mEventMap.end()) return false; // 

	m_mTimerMap[uiTimerId] = uiEventId;
	m_mEventMap[uiEventId] = pTimerEvent;
	
	return true;
}

}
