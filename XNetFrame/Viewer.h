///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __VIEWER_H
#define __VIEWER_H

#include "TypesDn.h"

namespace XNetFrame{

class CViewer{
public:
	enum EViewerResult { VR_NULL, VR_OPEN_FAILURE, VR_BUFFER_OVERFLOW, VR_OK };

	TSTRING m_sName; // 

protected:
	LPTSTR convertDateToString(time_t); // 

public:
	CViewer(LPTSTR szName = _T(""));
	virtual ~CViewer(void);

	virtual EViewerResult view(time_t, LPTSTR) = 0; // 
};

}

#endif
