#ifndef	__FILE_FACTORY_H
#define	__FILE_FACTORY_H

#include "TypesDn.h"
#include "Directory.h"

namespace XNetFrame{
	
class CFileInfo{
public:
	TSTRING m_sFilename;
public:
	CFileInfo(void);
	~CFileInfo(void);
};

typedef hash_map<TSTRING, CFileInfo*> FileInfoMap;

class CFileFactory{
public:
	FileInfoMap::iterator m_itrFileInfo;
	CFileInfo* nextElement();
protected:
	bool m_bBegin;
	FileInfoMap m_mFileInfoMap;
public:
	CFileFactory(void);
	virtual ~CFileFactory(void);

public:
	uint32 getCount(void);  // 
	void   deleteAll(void); // 

	CFileInfo*	getFileInfo(TSTRING);       // 
	bool				deleteFileInfo(TSTRING);    // 
	bool				addFileInfo(CFileInfo*); // 
	bool				loadFilesByExtension(TSTRING, TSTRING, bool);
	bool				loadFilesByExtensionInDir(TSTRING, TSTRING);
};

}


#endif