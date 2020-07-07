///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "ScheduleThread.h"

namespace XNetFrame{

CScheduleThread::CScheduleThread(CScheduleService* pScheduleService) : CThread(){
	m_pScheduleService = pScheduleService;
}

CScheduleThread::~CScheduleThread(void){
}

void CScheduleThread::run(void){
	CEvent::EEventResult erResult;

	if(m_pScheduleService == NULL) return; // 

	while(true){
		erResult = m_pScheduleService->m_IoEvent.wait(); // 
		
		if(CThread::getThreadFlag() == false) break; // 

		if(erResult != CEvent::ER_FAILURE){
			while(true){
				if(m_pScheduleService->scheduleEvent() != true) break; // 
			};
		};
	};
}

}
