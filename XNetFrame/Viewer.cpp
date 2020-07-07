///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "Viewer.h"

namespace XNetFrame{

CViewer::CViewer(LPTSTR szName){
	m_sName = szName;
}

CViewer::~CViewer(void){
}

LPTSTR CViewer::convertDateToString(time_t tDate){ // 
	static TCHAR szTime[50];
	struct tm*   tmLocal;
	
	tmLocal = localtime(&tDate);
	
	if(tmLocal != NULL) _tcsftime(szTime, 50, _T("%Y/%m/%d %H:%M:%S"), tmLocal);
	else _stprintf(szTime, _T("Abnormal Time %d"), (uint32)tDate);
	
	return szTime;
}

}
