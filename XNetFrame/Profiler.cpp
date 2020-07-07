/**
 * profile class
 * 
 */

#include "Profiler.h"

namespace XNetFrame{

CProfiler::CProfiler(void){
	m_uiCount = 0; // 

	m_uiOpenedTime      = 0; // 
	m_uiClosedTime      = 0; // 
	m_uiElapsedTime     = 0; // 
	m_uiActivatedTime   = 0; // 
	m_uiDeactivatedTime = 0; // 
	m_uiAccumulatedTime = 0; // 

	m_ttBeginTime	=	0;
	m_ttdElapsedTicks	=	0;
}

CProfiler::~CProfiler(void){
}

void CProfiler::incCount(void){ // 
	m_uiCount++;
}

uint32 CProfiler::getCount(void){ // 
	return m_uiCount;
}

void CProfiler::resetCount(void){ // 
	m_uiCount = 0;
}

bool CProfiler::isElapsed(void){ // 
	m_uiClosedTime = (uint32)timeGetTime();

	return (m_uiClosedTime > m_uiElapsedTime)? true : false;
}

void CProfiler::setElapsedTime(uint32 uiElapsedTime){ // 
	m_uiOpenedTime  = (uint32)timeGetTime();
	m_uiElapsedTime = m_uiOpenedTime + uiElapsedTime;
}

void CProfiler::checkTime(bool bCheck){ // 
	if(bCheck == true){
		m_uiActivatedTime = (uint32)timeGetTime();
	}
	else{
		m_uiDeactivatedTime = (uint32)timeGetTime();
		m_uiAccumulatedTime = m_uiAccumulatedTime + (m_uiDeactivatedTime - m_uiActivatedTime);
	};
}

uint32 CProfiler::getTimeInterval(void){ // 
	return (m_uiDeactivatedTime - m_uiActivatedTime);
}

uint32 CProfiler::getAccumulatedTime(void){ // 
	return m_uiAccumulatedTime;
}

void CProfiler::resetAccumulatedTime(void){ // 
	m_uiAccumulatedTime = 0;
}

TTicks CProfiler::getPerformanceTime(void){
	LARGE_INTEGER ret;
	if (QueryPerformanceCounter(&ret))
		return ret.QuadPart;
	else
		return 0;
}

double CProfiler::ticksToSecond(TTicks ticks){
	LARGE_INTEGER ret;
	if (QueryPerformanceFrequency(&ret))
		return (double)(sint64)ticks/(double)ret.QuadPart;

	return 0;
}

void CProfiler::start(void){
	m_ttBeginTime = getPerformanceTime();
	m_ttdElapsedTicks = 0;
}

void CProfiler::stop(void){
	m_ttdElapsedTicks += (TTickDuration)(getPerformanceTime() - m_ttBeginTime);
}

TMsDuration CProfiler::getDuration(void){
	return (TMsDuration)(ticksToSecond(m_ttdElapsedTicks) * 1000.0);
}
}
