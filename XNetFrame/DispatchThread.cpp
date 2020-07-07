///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "DispatchThread.h"
#include "SessionDispatchService.h"
#include "ChannelDispatchService.h"
#include "ContextDispatchService.h"

namespace XNetFrame{

CDispatchThread::CDispatchThread(CDispatchService* pDispatchService) : CThread(){
	m_pDispatchService = pDispatchService;
}

CDispatchThread::~CDispatchThread(void){
}

void CDispatchThread::run(void){
	if(m_pDispatchService->m_siDispatchMode == 0) dispatchChannel();      // 
	else if(m_pDispatchService->m_siDispatchMode == 1) dispatchSession(); // 
	else dispatchContext();                                               // 
}

void CDispatchThread::dispatchChannel(void){ // 
	CEvent::EEventResult erResult;

	while(true){
		erResult = m_pDispatchService->m_IoEvent.wait(); //
		
		if(CThread::getThreadFlag() == false) break; // 

		if(erResult != CEvent::ER_FAILURE){
			while(true){
				if(((CChannelDispatchService*)m_pDispatchService)->dispatchChannel() == 0) break;
			};
		};
	};
}

void CDispatchThread::dispatchSession(void){ // 
	CEvent::EEventResult erResult;

	while(true){
		erResult = m_pDispatchService->m_IoEvent.wait(); // 
		
#ifndef WIN32
		//sint32 val =m_pDispatchService->m_IoEvent.getEventVal();
		//printf("wait semval=%d\n",val);
#endif
		if(CThread::getThreadFlag() == false) break; // 

		if(erResult != CEvent::ER_FAILURE){
		#ifdef WIN32
			while(true){
				if(((CSessionDispathService*)m_pDispatchService)->dispatchSession() != true) break;
			};
		#else
			((CSessionDispatchService*)m_pDispatchService)->dispatchSession();
		#endif
		};
	};
}

void CDispatchThread::dispatchContext(void){ // 
	CEvent::EEventResult erResult;

	while(true){
		erResult = m_pDispatchService->m_IoEvent.wait(); // 
		
		if(CThread::getThreadFlag() == false) break; // 

		if(erResult != CEvent::ER_FAILURE){
			while(true){
				if(((CContextDispatchService*)m_pDispatchService)->dispatchContext() != true) break;
			};
		};
	};
}

}
