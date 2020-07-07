///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __DISPATCH_THREAD_H
#define __DISPATCH_THREAD_H

#include "TypesDn.h"

#include "Event.h"
#include "Thread.h"
#include "DispatchService.h"

namespace XNetFrame{

class CDispatchThread : public CThread{
private:
	CDispatchService* m_pDispatchService; // 

private:
	void dispatchChannel(void); // 
	void dispatchSession(void); // 
	void dispatchContext(void); // 

public:
	explicit CDispatchThread(CDispatchService*); // 
	~CDispatchThread(void);

	void run(void);
};

}

#endif
