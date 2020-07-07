#include "ChannelDispatchService.h"

namespace XNetFrame{

CChannelDispatchService::CChannelDispatchService(CLog* pLog, sint32 siDispatchMode, bool bDeliveryMode) 
 : CDispatchService(pLog,siDispatchMode,bDeliveryMode){

}

CChannelDispatchService::~CChannelDispatchService(void){
	stopService();
}

sint32 CChannelDispatchService::dispatchChannel(void){ // 
	uint32    uiMessageId, uiResult;
	char*     szMessage;
	CChannel* pChannel;
	CChannelMessageHandler* pMessage;
	bool      bExFlag;
	time_t    startTime;

	pChannel = (CChannel*)getObject(bExFlag, startTime); //
	//pChannel = (CChannel*)getObject(bExFlag); // 
	if(pChannel == NULL) return 0;

	if(pChannel->getState() == CChannel::CS_SHUTTING_DOWN) return 1; // 

	if(pChannel->getMessage(&uiMessageId, &szMessage) != CChannel::CR_OK){ // 
		pChannel->doClose(CChannel::CS_HANDLER_FAILURE);
		return 1;
	};

	pMessage = (CChannelMessageHandler*)CMessageHandlerFactory::getMessageHandler(uiMessageId); // 
	if(pMessage == NULL) return 1;
#ifdef LOG_LEVEL_1
	m_pLog->logLn("Channel uiMessageId = %d", uiMessageId);
#endif
	uiResult = pMessage->handleMessage(pChannel, szMessage, NULL);
		
	if(uiResult == 0){ // 
		pChannel->doClose(CChannel::CS_HANDLER_FAILURE);
		return 1;
	};

	if(uiResult == 2){ // 
		return 2;
	};

	return 1;
}

bool CChannelDispatchService::deliveryChannel(void){ // 
	if(m_bDeliveryMode == false) return false; // 

	while(true){
		if(dispatchChannel() == 1) continue;
		else break;
	};

	return false;
}

}