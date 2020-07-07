///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __ENVIRONMENT_CONTAINER_H
#define __ENVIRONMENT_CONTAINER_H

#include "Monitor.h"
#include "EnvironmentFactory.h"

namespace DnNetworkFramework{

class CEnvironmentContainer{
private:
	CMonitor m_mMonitor; // 

	CEnvironment* m_pEnvironment; // 

	sint32 m_siMaxEnvironmentCount;       // 
	sint32 m_siCurEnvironmentCount;       // 
	sint32 m_siAvailableEnvironmentIndex; // 

public:
	CEnvironmentContainer(void);
	virtual ~CEnvironmentContainer(void);

	void clearEnvironmentContainer(void);  // 
	bool initEnvironmentContainer(sint32); // 

	CEnvironment* createEnvironment(void);    // 
	void          destroyEnvironment(sint32); // 
};

}

#endif
