///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __EVENT_H
#define __EVENT_H

#include "TypesDn.h"

#include "Exception.h"
#include <semaphore.h>
namespace XNetFrame{

class CEvent{
public:
	enum EEventResult { ER_TIMEOUT, ER_FAILURE, ER_OK };

	private:
	
#ifdef WIN32
	IOEVENT m_hEvent;      // 
#else
	sem_t m_hEvent;
#endif
   
	uint32 m_uiErrorCode; // 

	public:
	
#ifdef WIN32
	CEvent(LPSECURITY_ATTRIBUTES, bool, bool);           // 
	CEvent(LPSECURITY_ATTRIBUTES, bool, bool, TSTRING&); // 
#else
	CEvent(bool ,sint32);
#endif

	virtual ~CEvent(void);

	IOEVENT getEvent(void);
	
#ifdef WIN32
	EEventResult reset(void);
	EEventResult pulse(void);
#endif

	uint32 getErrorCode(void);

	EEventResult set(void);
	EEventResult wait(void);  // 
	EEventResult wait(DWORD); // 
	
#ifndef WIN32
    sint32 getEventVal(void);
#endif

	private:
	
#ifdef WIN32
	IOEVENT create(LPSECURITY_ATTRIBUTES, bool, bool, LPTSTR);
#endif

};

#ifdef WIN32

class CPassiveResetEvent : public CEvent{ // 
public:
	explicit CPassiveResetEvent(bool bInit = false); // 
	explicit CPassiveResetEvent(TSTRING&, bool bInit = false); // 
	~CPassiveResetEvent(void);
};

#endif

class CAutomaticResetEvent : public CEvent{ // 
public:
	
#ifdef WIN32
	explicit CAutomaticResetEvent(bool bInit = false); // 
	explicit CAutomaticResetEvent(TSTRING &, boolbInit = false); // 
#else
	explicit CAutomaticResetEvent(sint32 siInit = 0); // 
	explicit CAutomaticResetEvent(bool, sint32 siInit = 0); // 
#endif
	~CAutomaticResetEvent(void);
};

}

#endif
