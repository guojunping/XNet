///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __PROPERTIES_H
#define __PROPERTIES_H

#include "TypesDn.h"
#include "Monitor.h"

namespace XNetFrame{

class CLoader;
typedef list<CLoader*>        LoaderList;

typedef map<string, string> LoaderMap;
typedef map<string, string> PropertyMap;

class CProperties{ // 
public:
	enum EPropertiesResult { PR_NULL, PR_FAILURE, PR_ALREADY_EXIST, PR_NOT_FOUND, PR_NOT_CONNECTED, PR_OK };

private:
	CMonitor    m_cMonitor;

	LoaderList  m_liLoaders;   // 

	LoaderMap   m_mLoaders;    // 
	PropertyMap m_mProperties; // 

public:
	CProperties(void);
	virtual ~CProperties(void);

	uint32            countLoader(void);      // 
	CLoader*          getLoader(LPTSTR);      // 
	EPropertiesResult addLoader(CLoader*);    // 
	EPropertiesResult deleteLoader(LPTSTR);   // 
	EPropertiesResult deleteLoader(CLoader*); // 

	EPropertiesResult isConnected(LPTSTR);    // 
	EPropertiesResult isConnected(CLoader*);  // 

	uint32            countProperty(void);    // 

	sint32            getIntProperty(LPTSTR);
	float             getFloatProperty(LPTSTR);
	string            getStringProperty(LPTSTR);

	sint32            getIntProperty(LPTSTR, LPTSTR);
	float             getFloatProperty(LPTSTR, LPTSTR);
	string            getStringProperty(LPTSTR, LPTSTR); // 

	EPropertiesResult addProperty(LPTSTR, LPTSTR, float);
	EPropertiesResult addProperty(LPTSTR, LPTSTR, sint32);
	EPropertiesResult addProperty(LPTSTR, LPTSTR, LPTSTR);

	EPropertiesResult addProperty(LPTSTR, LPTSTR, LPTSTR, float);
	EPropertiesResult addProperty(LPTSTR, LPTSTR, LPTSTR, sint32);
	EPropertiesResult addProperty(LPTSTR, LPTSTR, LPTSTR, LPTSTR); // 

	EPropertiesResult addPropertyEx(LPTSTR, LPTSTR, LPTSTR, float);
	EPropertiesResult addPropertyEx(LPTSTR, LPTSTR, LPTSTR, sint32);
	EPropertiesResult addPropertyEx(LPTSTR, LPTSTR, LPTSTR, LPTSTR); // 

	void              deleteProperty(LPTSTR);
	void              deleteProperty(LPTSTR, LPTSTR); // 

	EPropertiesResult deletePropertyEx(LPTSTR, LPTSTR, LPTSTR); // 

	bool              isProperty(LPTSTR);
	bool              isProperty(LPTSTR, LPTSTR); // 

	void              deleteAll(void); // 

	EPropertiesResult loadPropertis(void); // 
};

}

#endif
