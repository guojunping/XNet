/**
 * critical section
 * 
 */
#ifndef __MONITOR_H
#define __MONITOR_H

#include "TypesDn.h"

#include "Exception.h"

namespace XNetFrame{

class CMonitor{
private:
	MutexType m_csMonitor;

public:
	CMonitor(void); 
	virtual ~CMonitor(void);

	void enter(void); 
	void leave(void);
};

class synchronized{ 
private:
	CMonitor* m_pMonitor;

public:
	explicit synchronized(CMonitor*); 
	virtual ~synchronized(void);
};

}

#endif
