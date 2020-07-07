///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "FileViewer.h"

namespace XNetFrame{

CFileViewer::CFileViewer(void) : CViewer(_T("FV")){
	m_pFile  = NULL;
	m_bErase = false;
	m_sFile  = _T("");
	m_sFilePath = _T("");
}

CFileViewer::CFileViewer(LPTSTR szViewer, LPTSTR szFile, bool bErase) : CViewer(szViewer){
	m_pFile = NULL;
	setFile(szFile, bErase);
}

CFileViewer::~CFileViewer(void){
}

CViewer::EViewerResult CFileViewer::setFile(LPTSTR szFile, bool bErase){
	//m_sFile  = szFile;
	m_sFilePath = szFile ; 
	m_bErase = bErase;

	//if(m_sFile.empty()) return VR_NULL;
	if(m_sFilePath.empty()) return VR_NULL;

	/*
	if(m_bErase == true){
		m_pFile = _tfopen(m_sFile.c_str(), _T("w")); // 
		if(m_pFile == NULL) return VR_OPEN_FAILURE;

		fclose(m_pFile);
	};
	*/

	return VR_OK;
}

CViewer::EViewerResult CFileViewer::view(time_t tDate, LPTSTR szLine){
	if(m_sFilePath.empty() == true || tDate == 0 || szLine == NULL) return VR_NULL;

	//sam add	
	time_t t=time(NULL);
	struct tm *today = localtime(&t);		

	char	szFileName[1024];
	sprintf(szFileName,"%s_%4d%02d%02d_%02d.LF",m_sFilePath.c_str(),today->tm_year+1900,today->tm_mon+1,today->tm_mday,today->tm_hour);
	m_sFile = szFileName ; 


	m_pFile = _tfopen(m_sFile.c_str(), _T("ab")); // 
	if(m_pFile == NULL) return VR_OPEN_FAILURE;

	_ftprintf(m_pFile, _T("%s : %s"), CViewer::convertDateToString(tDate), szLine);
	fclose(m_pFile);

	return VR_OK;
}

}
