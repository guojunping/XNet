/**
 * loader
 * 
 */

#ifndef __LOADER_H
#define __LOADER_H

#include "TypesDn.h"

namespace XNetFrame{

class CProperties;
class CLoader{
public:
	enum ELoaderResult { LR_NULL, LR_FAILURE, LR_BUFFER_OVERFLOW, LR_OK };

	string      m_sName;       // 
	CProperties* m_pProperties; // 

public:
	CLoader(const char* szName = "");
	virtual ~CLoader(void);

	virtual ELoaderResult loadProperties(void)                      = 0; // 
	virtual ELoaderResult resetProperty(string&, string&)         = 0;
	virtual ELoaderResult setProperty(string&, string&, string&) = 0;
};

}

#endif
