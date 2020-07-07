///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __DISPATCH_SERVICE_H
#define __DISPATCH_SERVICE_H

#include "TypesDn.h"

#include "Log.h"
#include "Event.h"
#include "Channel.h"
#include "Session.h"
#include "Monitor.h"
#include "Context.h"

#include "MessageHandlerFactory.h"

namespace XNetFrame{

typedef list<CDispatchThread*> DispatchThreadList;

#define MAX_OBJECT_QUEUE_LEN (4096)

#pragma pack(1)

struct MS_SYSTEM_HEADER :MS_HEADER{
	char   m_szMd5[16 + 2];        //
};

struct MS_CLOSE_SESSION : MS_SYSTEM_HEADER{
	uint32 m_uiState;
};

struct MS_BACK_DOOR : MS_SYSTEM_HEADER{
	
};

#pragma pack()

class CDispatchService : public CMessageHandlerFactory{
public:
	enum EDispatchServiceResult { DSR_NULL, DSR_THREAD_START_FAILURE, DSR_FAILURE, DSR_OBJECT_QUEUE_OVERFLOW, DSR_OK };
	enum ESessionMessageId { SM_CLOSE_SESSION = 1, SM_BACK_DOOR = 2 };
	
	#ifdef WIN32
	CPassiveResetEvent m_IoEvent;        // 
	#else
	CAutomaticResetEvent m_IoEvent;
	#endif
	
	bool               m_bDeliveryMode;  // 
	sint32             m_siDispatchMode; // 

private:
	CLog*              m_pLog;          // 
	CMonitor           m_cMonitor;      // 
	CInetAddress       m_iaInetAddress; // 

	uint32             m_uiHeadPos;   // 
	uint32             m_uiTailPos;   // 
	sint32             m_siRefCount;  // 
	uint32             m_uiUsedCount; // 

	void*              m_oqObjectQueue[MAX_OBJECT_QUEUE_LEN + 1]; // 
	bool               m_fqExFlagQueue[MAX_OBJECT_QUEUE_LEN + 1]; // 
	time_t             m_startTimeQueue[MAX_OBJECT_QUEUE_LEN + 1];

	DispatchThreadList m_liDispatchThreadList; // 

	EDispatchServiceResult m_dsrResult;

private:

	EDispatchServiceResult createService(void);  // 
	EDispatchServiceResult destroyService(void); // 

public:
	explicit CDispatchService(CLog*, sint32, bool); // 
	~CDispatchService(void);

	EDispatchServiceResult stopService(void);  // 
	EDispatchServiceResult startService(void); // 

    CInetAddress *getInetAddress(){ return &m_iaInetAddress; }
	void* getObject(bool&, time_t&); // 
	EDispatchServiceResult addObject(void*, bool, time_t); //
	bool                   isOverflowObjectQueue(uint32); // 

	void setErrorCode(EDispatchServiceResult);

	virtual void onCreateService(void)  = 0;
	virtual void onDestroyService(void) = 0; // 

};

}

#endif
