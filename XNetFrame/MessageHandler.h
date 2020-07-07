#ifndef __MESSAGE_HANDLER_H
#define __MESSAGE_HANDLER_H

#include "TypesDn.h"

namespace XNetFrame{
	
	//class CSession;
	//class CChannel;
	//class CContext;
	
	class CMessageHandler{
		
	private:
			uint32 m_uiMessageId;   // 
	public:
	       explicit  CMessageHandler(uint32);
			~CMessageHandler();

			uint32  getMessageId(){return m_uiMessageId;}
			
	};
	
	class CSessionMessageHandler :public CMessageHandler{

	public:
	       explicit  CSessionMessageHandler(uint32 uiMessageId):CMessageHandler(uiMessageId){};
			~CSessionMessageHandler(){};

			virtual  uint32 handleMessage(CSession* pSession, char* szMessage, void*) = 0;
				
	};
	
	class CChannelMessageHandler :public CMessageHandler{
		
	public:
	       explicit  CChannelMessageHandler(uint32 uiMessageId):CMessageHandler(uiMessageId){};
			~CChannelMessageHandler(){};

			virtual  uint32 handleMessage(CChannel* pChannel, char* szMessage, void*) = 0;
				
	};
	
	class CContextMessageHandler :public CMessageHandler{
		
	public:
	       explicit  CContextMessageHandler(uint32 uiMessageId):CMessageHandler(uiMessageId){};
			~CContextMessageHandler(){};

			virtual  uint32 handleMessage(CContext* pContext, char* szMessage, void*) = 0;
	};
	
}

#endif