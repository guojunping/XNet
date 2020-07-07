
#include "TimerEventHandler.h"

namespace XNetFrame{
	
CTimerEventHandler::CTimerEventHandler(uint32 uiEventId,uint32 uiDeleyTime,bool bRepetition,uint32 uiRepetitionCount)
:m_uiTimerId(0),m_uiEventId(uiEventId),m_uiDeleyTime(uiDeleyTime),m_bRepetition(bRepetition),m_uiRepetitionCount(uiRepetitionCount){

}

CTimerEventHandler::~CTimerEventHandler(void){
	
}

void CTimerEventHandler::setTimerId(uint32 uiTimerId)
{
	m_uiTimerId=uiTimerId;
}

uint32 CTimerEventHandler::getTimerId()
{
	return m_uiTimerId;
}

uint32 CTimerEventHandler::getEventId()
{
	return m_uiEventId;
}

bool   CTimerEventHandler::isRepetition()
{
    return 	m_bRepetition;
}
uint32 CTimerEventHandler::getDeleyTime()
{
	return m_uiDeleyTime;
}

uint32 CTimerEventHandler::getRepetitionCount()
{
	return m_uiRepetitionCount;
}

void CTimerEventHandler::setRepetitionCount(uint32 uiRepetitionCount)
{
	m_uiRepetitionCount=uiRepetitionCount;
}

}