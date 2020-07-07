///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __MESSAGE_HANDLER_FACTORY_H
#define __MESSAGE_HANDLER_FACTORY_H

#include "TypesDn.h"
#include "MessageHandler.h"

namespace XNetFrame{

#pragma pack(1)

struct MS_HEADER{ // 
	uint32 m_uiLen;
	uint32 m_uiMessageId;
};

#pragma pack()

//class CMessageHandler;
/*
class CMessage{
public:
	uint32 m_uiMessageId;   // 
	//string m_sMessageName;  // 
	//uint32 m_uiMessageType; // 
	
	CMessageHandler* m_pMessageHandler;

public:
	CMessage(void);
	CMessage(uint32 uiMessageId,CMessageHandler* pMessageHandler);
	~CMessage(void);
};


typedef hash_map<uint32, CMessage*> MessageMap;
*/
typedef hash_map<uint32, CMessageHandler*> MessageMap;

class CMessageHandlerFactory{ // 
private:
	MessageMap m_mMessageMap; // 

public:
	CMessageHandlerFactory(void);
	virtual ~CMessageHandlerFactory(void);

	CMessageHandler* getMessageHandler(uint32);    // 
	bool      addMessageHandler(CMessageHandler*); // 
	bool      deleteMessageHandler(uint32); // 

	uint32    getCount(void); // 

	void      deleteAll(void); // 
};

}

#endif
