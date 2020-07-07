
#ifndef __TIMER_EVENT_HANDLER_H
#define __TIMER_EVENT_HANDLER_H

#include "TypesDn.h"

namespace XNetFrame{

class CTimerEventHandler{
	
private:
	uint32 m_uiTimerId; // 
	uint32 m_uiEventId; // 

	bool   m_bRepetition; // 

	uint32 m_uiDeleyTime;       // 
	uint32 m_uiRepetitionCount; // 

public:

	explicit CTimerEventHandler(uint32 uiEventId,uint32 uiDeleyTime,bool bRepetition=false,uint32 uiRepetitionCount=0);
	~CTimerEventHandler(void);
	
	void setTimerId(uint32 uiTimerId);
	uint32 getTimerId();
	uint32 getEventId();
	bool   isRepetition();
	uint32 getDeleyTime();
	void   setRepetitionCount(uint32 uiRepetitionCount);
	uint32 getRepetitionCount();
	
	virtual uint32 handleEvent(void)=0; //
};

}

#endif
