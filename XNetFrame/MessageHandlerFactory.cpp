///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "MessageHandlerFactory.h"

namespace XNetFrame{
/*
CMessage::CMessage(void){
	m_uiMessageId     = 0;
	//m_sMessageName.reserve(128); // 
	//m_uiMessageType   = 0;
	m_pMessageHandler = NULL;
}

CMessage::CMessage(uint32 uiMessageId,CMessageHandler* pMessageHandler)
{
	m_uiMessageId      = uiMessageId;
	//m_sMessageName.reserve(128); // 
	//m_uiMessageType   = 0;
	m_pMessageHandler = pMessageHandler;
}

CMessage::~CMessage(void){
}
*/
CMessageHandlerFactory::CMessageHandlerFactory(void){
}

CMessageHandlerFactory::~CMessageHandlerFactory(void){
	deleteAll();
}

uint32 CMessageHandlerFactory::getCount(void){ // 
	return (uint32)m_mMessageMap.size();
}

void CMessageHandlerFactory::deleteAll(void){ // 
	CMessageHandler*  pMessage;
	MessageMap::iterator itr;

	for(itr = m_mMessageMap.begin(); itr != m_mMessageMap.end(); itr++){
		pMessage = (CMessageHandler*)(*itr).second;
		delete pMessage;
		pMessage = NULL;
	};

	itr = m_mMessageMap.begin();
	if(itr != m_mMessageMap.end()) m_mMessageMap.erase(itr, m_mMessageMap.end());
}

CMessageHandler* CMessageHandlerFactory::getMessageHandler(uint32 uiMessageId){ // 
	CMessageHandler*            pMessage;
	MessageMap::iterator itr;

	itr = m_mMessageMap.find(uiMessageId);
	
	if(itr == m_mMessageMap.end()) pMessage = NULL;
	else pMessage = (CMessageHandler*)(*itr).second;

	return pMessage;
}

bool CMessageHandlerFactory::addMessageHandler(CMessageHandler* pMessage){ // 
	MessageMap::iterator itr;

	if(pMessage == NULL) return false;

	itr = m_mMessageMap.find(pMessage->getMessageId());
	if(itr != m_mMessageMap.end()) return false; // 

	m_mMessageMap[pMessage->getMessageId()] = pMessage;
	
	return true;
}

bool CMessageHandlerFactory::deleteMessageHandler(uint32 uiMessageId){ // 
	CMessageHandler*            pMessage;
	MessageMap::iterator itr;

	itr = m_mMessageMap.find(uiMessageId);
	if(itr == m_mMessageMap.end()) return false; // 
	
	pMessage = (CMessageHandler*)(*itr).second;

	m_mMessageMap.erase(uiMessageId);
	delete pMessage;
	pMessage = NULL;

	return true;
}

}
