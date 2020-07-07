///////////////////////////////////////
// Author : Hank Lee
// Name : 
///////////////////////////////////////

#include "ConsoleViewer.h"

namespace XNetFrame{

	CConsoleViewer::CConsoleViewer(void) : CViewer(_T("CV")){
	}

	CConsoleViewer::CConsoleViewer(LPTSTR szViewer) : CViewer(szViewer){
	}

	CConsoleViewer::~CConsoleViewer(void){
	}

	CViewer::EViewerResult CConsoleViewer::view(time_t tDate, LPTSTR szLine){

		if(tDate == 0 || szLine == NULL) return VR_NULL;

		if(_tcslen(szLine) > 960) return VR_BUFFER_OVERFLOW;

		printf(_T("%s : %s"), CViewer::convertDateToString(tDate), szLine);

		return VR_OK;
	}

}
