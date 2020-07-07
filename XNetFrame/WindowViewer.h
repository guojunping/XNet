///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __WINDOW_VIEWER_H
#define __WINDOW_VIEWER_H

#include "TypesDn.h"

#include "Viewer.h"

namespace DnNetworkFramework{

class CWindowViewer : public CViewer{
private:
	HWND   m_hWindow;        // 

	sint32 m_siMaxChar;      // 
	TCHAR  m_szBuffer[1024]; // 

public:
	CWindowViewer(void);
	CWindowViewer(LPTSTR, HWND); // 
	~CWindowViewer(void);

	EViewerResult setWindow(HWND);        // 
	EViewerResult setMaxChar(sint32);     // 
	EViewerResult view(time_t, LPTSTR);
};

}

#endif
