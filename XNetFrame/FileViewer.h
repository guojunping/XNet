///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __FILE_VIEWER_H
#define __FILE_VIEWER_H

#include "TypesDn.h"

#include "Viewer.h"

namespace XNetFrame{

class CFileViewer : public CViewer{
private:
	FILE*   m_pFile;  // 
	TSTRING m_sFile;  // 
	bool    m_bErase; // 

	TSTRING m_sFilePath;  // 

public:
	CFileViewer(void);
	CFileViewer(LPTSTR, LPTSTR, bool = false); // 
	~CFileViewer(void);

	EViewerResult setFile(LPTSTR, bool = false);
	EViewerResult view(time_t, LPTSTR);
};

}

#endif
