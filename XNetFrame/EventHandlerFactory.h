///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __EVENT_HANDLER_FACTORY_H
#define __EVENT_HANDLER_FACTORY_H

#include "TypesDn.h"

namespace XNetFrame{

class CTimerEventHandler;

typedef hash_map<uint32, uint32>              TimerMap;
typedef hash_map<uint32, CTimerEventHandler*> EventMap;

class CEventHandlerFactory{
protected:
	TimerMap m_mTimerMap; // 
	EventMap m_mEventMap; // 

public:
	CEventHandlerFactory(void);
	virtual ~CEventHandlerFactory(void);

protected:
	uint32 getCount(void);  // 
	void   deleteAll(void); // 

	uint32       getTimer(uint32);       // 
	CTimerEventHandler* getEvent(uint32);       // 
	bool         deleteEvent(uint32);    // 
	bool         addEvent(CTimerEventHandler*); // 
};

}

#endif
