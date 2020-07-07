#include "ContextDispatchService.h"

namespace XNetFrame{

CContextDispatchService::CContextDispatchService(CLog* pLog, sint32 siDispatchMode, bool bDeliveryMode) 
 : CDispatchService(pLog,siDispatchMode,bDeliveryMode){

}

CContextDispatchService::~CContextDispatchService(void){
	stopService();
}


bool CContextDispatchService::dispatchContext(void){ // 
	uint32    uiMessageId;
	char*     szMessage;
	CContext* pContext;
	CContextMessageHandler* pMessage;
	bool      bExFlag;
	time_t    startTime;

	pContext = (CContext*)getObject(bExFlag, startTime); //
	//pContext = (CContext*)getObject(bExFlag); // 
	if(pContext == NULL) return false;

	if(pContext->getState() == CContext::CS_SHUTTING_DOWN) return true; // 

	if(pContext->getMessage(&uiMessageId, &szMessage, getInetAddress()) != CContext::CR_OK) return true; // 

	pMessage = (CContextMessageHandler*)CMessageHandlerFactory::getMessageHandler(uiMessageId); // 
	if(pMessage == NULL) return true;

	if(pMessage->handleMessage(pContext, szMessage, getInetAddress()) == 0) return true; // 

	return true;
}

bool CContextDispatchService::deliveryContext(void){ // 
	if(m_bDeliveryMode == false) return false; // 

	while(true){
		if(dispatchContext() != true) break;
	};

	return false;
}


}