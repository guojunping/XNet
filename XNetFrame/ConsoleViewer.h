///////////////////////////////////////
// Author : Hank Lee
// Name : 
///////////////////////////////////////

#ifndef __CONSOLE_VIEWER_H
#define __CONSOLE_VIEWER_H

#include "TypesDn.h"

#include "Viewer.h"

namespace XNetFrame{

	class CConsoleViewer : public CViewer{
	private:

	public:
		CConsoleViewer(void);
		CConsoleViewer(LPTSTR); // 
		~CConsoleViewer(void);

		EViewerResult view(time_t, LPTSTR);
	};

}

#endif
