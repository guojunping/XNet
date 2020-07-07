/**
 * profile
 * 
 */
#ifndef __PROFILER_H
#define __PROFILER_H

#include "TypesDn.h"

namespace XNetFrame{

typedef	sint64	TTicks;
typedef	uint32 TTickDuration;
typedef	uint32 TMsDuration;

class CProfiler{
private:
	uint32 m_uiCount; // 

	uint32 m_uiOpenedTime;      // 
	uint32 m_uiClosedTime;      // 
	uint32 m_uiElapsedTime;     // 
	uint32 m_uiActivatedTime;   // 
	uint32 m_uiDeactivatedTime; // 
	uint32 m_uiAccumulatedTime; // 

	TTicks	m_ttBeginTime;
	TTickDuration	m_ttdElapsedTicks;

public:
	CProfiler(void);
	virtual ~CProfiler(void);

	void   incCount(void);   // 
	uint32 getCount(void);   // 
	void   resetCount(void); // 

	bool   isElapsed(void);        // 
	void   setElapsedTime(uint32); // 

	void   checkTime(bool);            // 
	uint32 getTimeInterval(void);      // 
	uint32 getAccumulatedTime(void);   // 
	void   resetAccumulatedTime(void); // 

	TTicks	getPerformanceTime(void);
	double	ticksToSecond (TTicks);
	void			start(void);
	void			stop(void);
	TMsDuration	getDuration(void);
};

}

#endif
