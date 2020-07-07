/**
 * critical section
 * 
 */

#include "Monitor.h"

namespace XNetFrame{

CMonitor::CMonitor(void){
	InitMutex(m_csMonitor);
}

void CMonitor::enter(void){
	LockMutex(m_csMonitor);
}

CMonitor::~CMonitor(void){
	DestroyMutex(m_csMonitor);
}

void CMonitor::leave(void){
   UnlockMutex(m_csMonitor);
}

synchronized::synchronized(CMonitor* pMonitor) 
: m_pMonitor(pMonitor){ 
	if(m_pMonitor != NULL) m_pMonitor->enter();
}

synchronized::~synchronized(void){ 
	if(m_pMonitor != NULL) m_pMonitor->leave();
}

}
