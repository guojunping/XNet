#ifndef	__DIRECTORY_H
#define	__DIRECTORY_H

#include "TypesDn.h"

namespace XNetFrame{

typedef WIN32_FIND_DATA dirent;

class CDirectory{
public:
	CDirectory(void);
	~CDirectory(void);

	static TSTRING	getCurDir(void);
	static bool setCurDir(TSTRING);
	static bool findFirstFile(TSTRING, HANDLE&, dirent&);
	static bool findNextFile(HANDLE&, dirent&);
	static bool findFirstDir(TSTRING, HANDLE&, dirent&);
	static bool findNextDir(HANDLE&, dirent&);
	static bool isDirectory(dirent&);
};

}


#endif