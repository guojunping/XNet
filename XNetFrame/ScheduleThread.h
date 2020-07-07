///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __SCHEDULE_THREAD_H
#define __SCHEDULE_THREAD_H

#include "TypesDn.h"

#include "Event.h"
#include "Thread.h"
#include "ScheduleService.h"

namespace XNetFrame{

class CScheduleThread : public CThread{
private:
	CScheduleService* m_pScheduleService; // 

public:
	explicit CScheduleThread(CScheduleService*); // 
	~CScheduleThread(void);

	void run(void);
};

}

#endif
