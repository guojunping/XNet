///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __FILE_LOADER_H
#define __FILE_LOADER_H

#include "TypesDn.h"

#include "Loader.h"
#include "Properties.h"
#include "StringTokenizer.h"

namespace XNetFrame{

typedef map<TSTRING, TSTRING> SectionMap;

class CFileLoader : public CLoader{
private:
	TSTRING    m_sFile;       // 
	SectionMap m_mSectionMap; // 

private:
	void deleteAll(void); // 
#ifdef WIN32
	ELoaderResult loadSectionInfo(void);     //  
	ELoaderResult loadSectionInfo(TSTRING&); //
#else
	ELoaderResult loadSectionInfo(void);     // 
	ELoaderResult loadFileData(string& sFilePath);
#endif

public:
	CFileLoader(void);
	CFileLoader(LPTSTR);
	~CFileLoader(void);

	ELoaderResult setFile(LPTSTR); // 

	ELoaderResult loadProperties(void);
	ELoaderResult resetProperty(TSTRING&, TSTRING&);
	ELoaderResult setProperty(TSTRING&, TSTRING&, TSTRING&);
};

}

#endif
