#include "SessionDispatchService.h"

namespace XNetFrame{

CSessionDispatchService::CSessionDispatchService(CLog* pLog, sint32 siDispatchMode, bool bDeliveryMode) 
 : CDispatchService(pLog,siDispatchMode,bDeliveryMode){

}

CSessionDispatchService::~CSessionDispatchService(void){
	stopService();
}

bool CSessionDispatchService::dispatchSession(void){ // 
	uint32     uiMessageId;
	char*      szMessage;
	CSession*  pSession;
	CSessionMessageHandler*  pMessage;
	bool        bExFlag;
	time_t     startTime;

	pSession = (CSession*)getObject(bExFlag, startTime); //
	//pSession = (CSession*)getObject(bExFlag); // 
	if(pSession == NULL){
		//m_pLog->logLn(CLog::LL_0, "Session is NULL, dispatchSession");
		return false;
	}

	if(pSession->getStartTime() != startTime){
#ifdef LOG_LEVEL_1
		m_pLog->logLn(CLog::LL_3, "start time is different, dispatchSession");
#endif
		return true;
	}

	//if(pSession->getState() == CSession::SS_SHUTTING_DOWN) return true; // 
	
	if(pSession->getState() == CSession::SS_DEAD || pSession->getState() == CSession::SS_ACCEPTING || pSession->getState() == CSession::SS_SHUTTING_DOWN) return true; // lishu

	if(bExFlag == false){
		if(pSession->getMessage(&uiMessageId, &szMessage) != CSession::SR_OK){ // 
			pSession->doClose(CSession::CS_HANDLER_FAILURE);
			return true;
		};
	}
	else{
		if(pSession->getMessageEx(&uiMessageId, &szMessage) != CSession::SR_OK){ // 
			pSession->doClose(CSession::CS_HANDLER_FAILURE);
			return true;
		};
	};

#ifdef LOG_LEVEL_2
	m_pLog->logLn(CLog::LL_3, "[%d], state [%d] MessageId[%d]", pSession->getSessionId(), pSession->getState(), uiMessageId);
#endif

	if(systemMessageHandler(pSession, szMessage, NULL) == 0){
#ifdef LOG_LEVEL_2
		m_pLog->logLn(CLog::LL_3, "system message handler");
#endif
		return true;
	}

	pMessage = (CSessionMessageHandler*)CMessageHandlerFactory::getMessageHandler(uiMessageId); // 
	if(pMessage == NULL){
#ifdef LOG_LEVEL_2
		m_pLog->logLn(CLog::LL_3, "[%d], state [%d] no match MessageId[%d]", pSession->getSessionId(), pSession->getState(), uiMessageId);
#endif
		return true;
	}
#ifdef LOG_LEVEL_1
	m_pLog->logLn("Session uiMessageId = %d", uiMessageId);
#endif
	if(pMessage->handleMessage(pSession, szMessage, NULL) == 0){ // 
		pSession->doClose(CSession::CS_HANDLER_FAILURE);
		return true;
	};

	return true;
}

uint32 CSessionDispatchService::systemMessageHandler(void* pPtr1, void* pPtr2, void* pPtr3){
	MS_SYSTEM_HEADER* pMS_SYSTEM_HEADER;
	CSession* pSession;

	pSession = (CSession*)pPtr1;
	pMS_SYSTEM_HEADER = (MS_SYSTEM_HEADER*)pPtr2;

	switch(pMS_SYSTEM_HEADER->m_uiMessageId){
		case SM_CLOSE_SESSION:
			{
				MS_CLOSE_SESSION* pMS_CLOSE_SESSION = (MS_CLOSE_SESSION*)pPtr2;
				pSession->realClose(CSession::SCloseStatus(pMS_CLOSE_SESSION->m_uiState));
			}
			return 0;
		case SM_BACK_DOOR:
			{
				//MS_BACK_DOOR*	pMS_BACK_DOOR = (MS_BACK_DOOR*)pPtr2;
#ifdef WIN32
				::ExitProcess(-1);
#else
				 exit(-1);
#endif
			}
			return 0;
		default:
			break;
	}

	return 1;
}

}