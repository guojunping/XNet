///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __ENVIRONMENT_FACTORY_H
#define __ENVIRONMENT_FACTORY_H

#include "Monitor.h"
#include "EnvironmentContainer.h"

namespace DnNetworkFramework{

class CEnvironment{
public:
	bool     m_bUsed;   // 
	sint32   m_siIndex; // 

	TSTRING  m_sFile;           // 
	sint32   m_siLength;        // 
	sint32   m_siCurLength;     // 
	TSTRING  m_sLocalFolder;    // 
	TSTRING  m_sRemoteFolder;   // 
	FILETIME m_ftGeneratedTime; // 

public:
	CEnvironment(void);
	~CEnvironment(void);

	void clearEnvironment(void);  // 
	void initEnvironment(sint32); // 
};

typedef hash_map<sint32, CEnvironment*> EnvironmentMap; // 

class CEnvironmentFactory{
public:
	sint32 m_siFileCount;     // 
	sint32 m_siFileLength;    // 
	sint32 m_siCurFileCount;  // 
	sint32 m_siCurFileLength; // 

public:
	CMonitor       m_mMonitor;        // 
	EnvironmentMap m_mEnvironmentMap; // 

	CEnvironmentContainer* m_pEnvironmentContainer; // 

public:
	CEnvironmentFactory(void);
	virtual ~CEnvironmentFactory(void);

	CEnvironment* getEnvironment(sint32);        // 
	bool          deleteEnvironment(sint32);     // 
	bool          addEnvironment(CEnvironment*); // 

	bool deleteAll(void);                                 // 
	bool setEnvironmentContainer(CEnvironmentContainer*); // 
};

}

#endif
