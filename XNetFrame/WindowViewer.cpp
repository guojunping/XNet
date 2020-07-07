///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "WindowViewer.h"

namespace DnNetworkFramework{

CWindowViewer::CWindowViewer(void) : CViewer(_T("WV")){
	m_hWindow   = NULL;
	m_siMaxChar = 4096;
}

CWindowViewer::CWindowViewer(LPTSTR szViewer, HWND hWindow) : CViewer(szViewer){
	m_siMaxChar = 4096;

	setWindow(hWindow);
}

CWindowViewer::~CWindowViewer(void){
}

CViewer::EViewerResult CWindowViewer::setWindow(HWND hWindow){ // 
	if(hWindow == NULL) return VR_NULL;

	m_hWindow = hWindow;

	::SendMessage(m_hWindow, EM_LIMITTEXT, m_siMaxChar, 0);
	
	return VR_OK;
}

CViewer::EViewerResult CWindowViewer::setMaxChar(sint32 siMaxChar){ // 
	if(m_hWindow == NULL) return VR_NULL;

	m_siMaxChar = siMaxChar;

	::SendMessage(m_hWindow, EM_LIMITTEXT, m_siMaxChar, 0); // 
	
	return VR_OK;
}

CViewer::EViewerResult CWindowViewer::view(time_t tDate, LPTSTR szLine){
	sint32  siTextLength, siLineIndex, siCharIndex;
	DWORD   dwResult;
	LRESULT lResult;

	if(m_hWindow == NULL || tDate == 0 || szLine == NULL) return VR_NULL;

	if(_tcslen(szLine) > 960) return VR_BUFFER_OVERFLOW;

	_stprintf(m_szBuffer, _T("%s : %s"), CViewer::convertDateToString(tDate), szLine);

	while(true){
		lResult = (sint32)::SendMessageTimeout(m_hWindow, WM_GETTEXTLENGTH, 0, 0, SMTO_BLOCK, 5, (DWORD*)&siTextLength); // 
		if(lResult == 0 && ::GetLastError() == 0) return VR_OK;

		if((m_siMaxChar - 512) > siTextLength) break;

		lResult = (sint32)::SendMessageTimeout(m_hWindow, EM_LINEFROMCHAR, 512, 0, SMTO_BLOCK, 5, (DWORD*)&siLineIndex); // 
		if(lResult == 0 && ::GetLastError() == 0) return VR_OK;

		lResult = (sint32)::SendMessageTimeout(m_hWindow, EM_LINEINDEX, siLineIndex, 0, SMTO_BLOCK, 5, (DWORD*)&siCharIndex); // 
		if(lResult == 0 && ::GetLastError() == 0) return VR_OK;

		lResult = ::SendMessageTimeout(m_hWindow, EM_SETSEL, 0, siCharIndex, SMTO_BLOCK, 5, &dwResult); // 
		if(lResult == 0 && ::GetLastError() == 0) return VR_OK;

		lResult = ::SendMessageTimeout(m_hWindow, EM_REPLACESEL, 0, (LPARAM)_T(""), SMTO_BLOCK, 5, &dwResult); // 
		if(lResult == 0 && ::GetLastError() == 0) return VR_OK;
	};

	lResult = ::SendMessageTimeout(m_hWindow, EM_SETSEL, siTextLength, siTextLength, SMTO_BLOCK, 5, &dwResult);
	if(lResult == 0 && ::GetLastError() == 0) return VR_OK;

	lResult = ::SendMessageTimeout(m_hWindow, EM_REPLACESEL, 0, (LPARAM)m_szBuffer, SMTO_BLOCK, 5, &dwResult);
	if(lResult == 0 && ::GetLastError() == 0) return VR_OK;

	lResult = ::SendMessageTimeout(m_hWindow, EM_SCROLL, SB_LINEDOWN, 0, SMTO_BLOCK, 5, &dwResult);
	if(lResult == 0 && ::GetLastError() == 0) return VR_OK;

	return VR_OK;
}

}
