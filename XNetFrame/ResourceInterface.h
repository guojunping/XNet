///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __RESOURCE_INTERFACE_H
#define __RESOURCE_INTERFACE_H

#include "TypesDn.h"

namespace XNetFrame{

#define MAX_INDEX_LEN  (10)
#define MAX_BUFFER_LEN (512)

typedef hash_map<sint32, LPTSTR> ResourceMap;

class CResourceInterface{
public:
	enum EResourceInterfaceResult { RIR_NULL, RIR_NOT_FOUND, RIR_INDEX_OVERFLOW, RIR_OPEN_FAILURE, RIR_ALREADY_EXIST, RIR_LOAD_FAILURE, RIR_OK };

private:
	TSTRING     m_sFile;        // 
	ResourceMap m_mResourceMap; // 

public:
	CResourceInterface(void);
	virtual ~CResourceInterface(void);

	sint32 getCount(void);      // 
	void   deleteAll(void);     // 
	LPTSTR getResource(sint32); // 

	EResourceInterfaceResult setFile(LPTSTR);             // 
	EResourceInterfaceResult loadResource(sint32);        // 
	EResourceInterfaceResult deleteResource(sint32);      // 
	EResourceInterfaceResult addResource(sint32, LPTSTR); // 
};

}

#endif
