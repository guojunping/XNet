#ifndef __X_SCHEDULE_SERVICE_H
#define __X_SCHEDULE_SERVICE_H

#include "Log.h"
#include "ScheduleService.h"

namespace XNetFrame{

class CXScheduleService : public CScheduleService{

public:
	CXScheduleService(CLog*);
	~CXScheduleService(void);

	virtual void onCreateService(void);
	virtual void onDestroyService(void);
};

}
#endif
