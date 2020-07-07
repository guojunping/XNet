///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "Log.h"

namespace XNetFrame{

CLog::CLog(void){

	m_pProperties = NULL;
	m_emLogLevel = CLog::LL_0;
	m_cBlockingQueue.SetMaxCount(5000);
	m_pLogThread=new CLogThread(this);

}

CLog::~CLog(void){
	m_cBlockingQueue.Destory();
	m_pLogThread->stop();
	delete m_pLogThread;
	deleteAll();
}

void CLog::start(void)
{
	m_pLogThread->start();
}

CLog::ELogResult CLog::deleteViewer(CViewer* pViewer){ // 
	ViewerList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	if(pViewer == NULL) return LR_NULL;

	itr = find(m_liViewers.begin(), m_liViewers.end(), pViewer);
	if(itr != m_liViewers.end()) m_liViewers.erase(itr);

	itr = find(m_liBypassViewers.begin(), m_liBypassViewers.end(), pViewer);
	if(itr != m_liBypassViewers.end()) m_liBypassViewers.erase(itr);

	return LR_OK;
}

void CLog::deleteAll(void){ // 
	CViewer*              pViewer;
	ViewerList::iterator itr;

	synchronized sync(&m_cMonitor);

	for(itr = m_liViewers.begin(); itr != m_liViewers.end(); itr++){
		pViewer = (CViewer*)(*itr);
		delete pViewer;
		pViewer = NULL;
	};

	for(itr = m_liBypassViewers.begin(); itr != m_liBypassViewers.end(); itr++){
		pViewer = (CViewer*)(*itr);
		delete pViewer;
		pViewer = NULL;
	};

	itr = m_liViewers.begin();
	if(itr != m_liViewers.begin()) m_liViewers.erase(itr, m_liViewers.end());
	
	itr = m_liBypassViewers.begin();
	if(itr != m_liBypassViewers.end()) m_liBypassViewers.erase(itr, m_liBypassViewers.end());

	m_liPositiveFilters.clear();
	m_liNegativeFilters.clear();
}

CLog::ELogResult CLog::deleteViewer(LPTSTR szViewerName){ // 
	ViewerList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	if(szViewerName == NULL || szViewerName[0] == 0x00) return LR_NULL;

	for(itr = m_liViewers.begin(); itr != m_liViewers.end();){
		if(_tcscmp((*itr)->m_sName.c_str(), szViewerName) == 0) itr = m_liViewers.erase(itr); // 
		else itr++;
	};

	for(itr = m_liBypassViewers.begin(); itr != m_liBypassViewers.end();){
		if(_tcscmp((*itr)->m_sName.c_str(), szViewerName) == 0) itr = m_liBypassViewers.erase(itr);
		else itr++;
	};

	return LR_OK;
}

CViewer* CLog::getViewer(LPTSTR szViewerName){ // 
	ViewerList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	if(szViewerName == NULL || szViewerName[0] == 0x00) return NULL;

	for(itr = m_liViewers.begin(); itr != m_liViewers.end(); itr++){
		if(_tcscmp((*itr)->m_sName.c_str(), szViewerName) == 0) return *itr;
	};

	for(itr = m_liBypassViewers.begin(); itr != m_liBypassViewers.end(); itr++){
		if(_tcscmp((*itr)->m_sName.c_str(), szViewerName) == 0) return *itr;
	};

	return NULL;
}

uint32 CLog::countViewer(void){ // 
	return (uint32)m_liViewers.size() + (uint32)m_liBypassViewers.size();
}

CLog::ELogResult CLog::addViewer(CViewer* pViewer, bool bBypass){ // 
	ViewerList::iterator itr;
	
	synchronized sync(&m_cMonitor);
	
	if(pViewer == NULL) return LR_NULL;

	if(bBypass == true){
		itr = find(m_liBypassViewers.begin(), m_liBypassViewers.end(), pViewer);

		if(itr == m_liBypassViewers.end()){
			m_liBypassViewers.push_back(pViewer);
			return LR_OK;
		};
	}
	else{
		itr = find(m_liViewers.begin(), m_liViewers.end(), pViewer);

		if(itr == m_liViewers.end()){
			m_liViewers.push_back(pViewer);
			return LR_OK;
		};
	};

	return LR_ALREADY_EXIST;
}

CLog::ELogResult CLog::isConnected(CViewer* pViewer){ // 
	ViewerList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	if(pViewer == NULL) return LR_NULL;

	itr = find(m_liViewers.begin(), m_liViewers.end(), pViewer);
	if(itr != m_liViewers.end()) return LR_OK;

	itr = find(m_liBypassViewers.begin(), m_liBypassViewers.end(), pViewer);
	if(itr != m_liBypassViewers.end()) return LR_OK;
	
	return LR_NOT_CONNECTED;
}

CLog::ELogResult CLog::isConnected(LPTSTR szViewerName){ // 
	ViewerList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	if(szViewerName == NULL || szViewerName[0] == 0x00) return LR_NULL;

	for(itr = m_liViewers.begin(); itr != m_liViewers.end(); itr++){
		if(_tcscmp((*itr)->m_sName.c_str(), szViewerName) == 0) return LR_OK;
	};

	for(itr = m_liBypassViewers.begin(); itr != m_liBypassViewers.end(); itr++){
		if(_tcscmp((*itr)->m_sName.c_str(), szViewerName) == 0) return LR_OK;
	};

	return LR_NOT_CONNECTED;
}

CLog::ELogResult CLog::addPositiveFilter(LPTSTR szFilter){ // 
	TSTRING              sFilter;
	FilterList::iterator itr;
	
	synchronized sync(&m_cMonitor);
	
	if(szFilter == NULL) return LR_NULL;

	sFilter = szFilter;
	itr     = find(m_liPositiveFilters.begin(), m_liPositiveFilters.end(), sFilter);

	if(itr == m_liPositiveFilters.end()){
		m_liPositiveFilters.push_back(sFilter);
		return LR_OK;
	};

	return LR_ALREADY_EXIST;
}

CLog::ELogResult CLog::addNegativeFilter(LPTSTR szFilter){ // 
	TSTRING              sFilter;
	FilterList::iterator itr;
	
	synchronized sync(&m_cMonitor);
	
	if(szFilter == NULL) return LR_NULL;

	sFilter = szFilter;
	itr     = find(m_liNegativeFilters.begin(), m_liNegativeFilters.end(), sFilter);

	if(itr == m_liNegativeFilters.end()){
		m_liNegativeFilters.push_back(sFilter);
		return LR_OK;
	};

	return LR_ALREADY_EXIST;
}

void CLog::deleteFilter(LPTSTR szFilter){ // 
	synchronized sync(&m_cMonitor);
	
	if(szFilter == NULL){
		m_liPositiveFilters.clear();
		m_liNegativeFilters.clear();
	}
	else{
		m_liPositiveFilters.remove(szFilter);
		m_liNegativeFilters.remove(szFilter);
	};
}

void CLog::dumpFilters(void){ // 
	FilterList::iterator itr;

	log(CLog::LL_0, _T("Positive Filter(s) : "));

	for(itr = m_liPositiveFilters.begin(); itr != m_liPositiveFilters.end(); itr++){
		log(CLog::LL_0, _T("'%s' "), (*itr).c_str());
	};

	logLn(CLog::LL_0, _T(""));

	log(CLog::LL_0, _T("Negative Filter(s) : "));

	for(itr = m_liNegativeFilters.begin(); itr != m_liNegativeFilters.end(); itr++){
		log(CLog::LL_0, _T("'%s' "), (*itr).c_str());
	};

	logLn(CLog::LL_0, _T(""));
}

bool CLog::isPassableFilter(LPTSTR szLine){ // 
	bool  bFound, bResult;
	FilterList::iterator itr;

	bResult = m_liPositiveFilters.empty(); // 

	for(itr = m_liPositiveFilters.begin(); itr != m_liPositiveFilters.end(); itr++){
		bFound = (_tcsstr(szLine, (*itr).c_str()) != NULL);
		if(bFound == true){
			bResult = true;
			break;
		};
	};

	if(bResult == false) return false; // 

	for(itr = m_liNegativeFilters.begin(); itr != m_liNegativeFilters.end(); itr++){
		bFound = (_tcsstr(szLine, (*itr).c_str()) != NULL);
		if(bFound == true) return false; // 
	};

	return true; //
}

void CLog::dumpMem(char* szMem, uint32 uiLen){
	char szHex[MAX_LINE_LEN];

	if(uiLen > MAX_LINE_LEN) return;

	for (uint32 i = 0; i < uiLen; ++i)
		sprintf(szHex + i, "%02x", szMem[i]);
	szHex[uiLen] = 0x00;

	logLn(CLog::LL_0, szHex);
}

void CLog::logLn(ELogLevel eLogLevel, LPTSTR szFormat, ...){ // 
	
	if(eLogLevel > m_emLogLevel) return;
	if(countViewer() == 0) return;

	TCHAR* szStr;
	VAR_TO_STR(szStr, szFormat, MAX_LINE_LEN);

	if(_tcslen(szStr) < MAX_LINE_LEN - 2) 
		_tcscat(szStr, _T("\r\n"));
	else{
		szStr[MAX_LINE_LEN - 3] = _T('\r');
		szStr[MAX_LINE_LEN - 2] = _T('\n');
	};

	logString(szStr);
}

void CLog::log(ELogLevel eLogLevel, LPTSTR szFormat, ...){ // 
	
	if(eLogLevel > m_emLogLevel) return;
	if(countViewer() == 0) return;

	LPTSTR szStr;
	VAR_TO_STR(szStr, szFormat, MAX_LINE_LEN);

	logString(szStr);
}

void CLog::logString(LPTSTR szStr){ // 

	TSTRING sLine=szStr;
	m_cBlockingQueue.Push(sLine);

/*
	time(&m_tDate);
	for(ViewerList::iterator itr = m_liBypassViewers.begin(); itr != m_liBypassViewers.end(); itr++){
		(*itr)->view(m_tDate, szStr);	
	};

	if(m_liViewers.size() != 0 && isPassableFilter(szStr) == true){ // 
		for(ViewerList::iterator itr = m_liViewers.begin(); itr != m_liViewers.end(); itr++){
			(*itr)->view(m_tDate, szStr);
		};
	};
*/

}

void  CLog::logOutput(void)
{
	LPTSTR  szLine = NULL;
	TSTRING sLine=m_cBlockingQueue.Pop();

	if((sint32)sLine.size()==0)
		return ;

	szLine=(LPTSTR)sLine.c_str();

	synchronized sync(&m_cMonitor);
	
	time_t tDate;
	time(&tDate);
	for(ViewerList::iterator itr = m_liBypassViewers.begin(); itr != m_liBypassViewers.end(); itr++){
		(*itr)->view(tDate, szLine);
	};

	if(m_liViewers.size() != 0 && isPassableFilter(szLine) == true){ // 
		for(ViewerList::iterator itr = m_liViewers.begin(); itr != m_liViewers.end(); itr++){
			(*itr)->view(tDate, szLine);
		};
	};
}

CLog::ELogResult CLog::setProperties(CProperties* pProperties){
	TSTRING sLogLevel;

	if(pProperties == NULL) return LR_NULL;

	m_pProperties = pProperties;

	sLogLevel = pProperties->getStringProperty(LOG_SERVICE_CLASS, LOG_LEVEL_PROPERTY);

	if(_tcscmp(sLogLevel.c_str(), _T("LOG_LEVEL_0")) == 0) m_emLogLevel = CLog::LL_0;
	else if(_tcscmp(sLogLevel.c_str(), _T("LOG_LEVEL_1")) == 0) m_emLogLevel = CLog::LL_1;
	else if(_tcscmp(sLogLevel.c_str(), _T("LOG_LEVEL_2")) == 0) m_emLogLevel = CLog::LL_2;
	else if(_tcscmp(sLogLevel.c_str(), _T("LOG_LEVEL_3")) == 0) m_emLogLevel = CLog::LL_3;

	return LR_OK;
}

CFunctionLog::CFunctionLog(CLog* pLog, CLog::ELogLevel eLogLevel, CProfiler* pProfiler, LPTSTR szFormat, ...){
	m_pLog = pLog;
	m_emLogLevel = eLogLevel;
	m_pProfiler = pProfiler;

	va_list args; 
	va_start(args, szFormat); 
	int iResult = _vsntprintf(m_szStr, CLog::MAX_LINE_LEN - 1, szFormat, args); 
	if(iResult == -1 || iResult == CLog::MAX_LINE_LEN - 1) 
	{ 
		m_szStr[CLog::MAX_LINE_LEN-1] = 0x00; 
	}
	va_end(args); 

	m_pLog->logLn(m_emLogLevel, "Enter %s", m_szStr);
	if(m_pProfiler) m_pProfiler->start();
}

CFunctionLog::~CFunctionLog(void){
	if(m_pProfiler){
		m_pProfiler->stop();
		m_pLog->logLn(m_emLogLevel, "Elapsed Time: %d ms", m_pProfiler->getDuration());
	}
	m_pLog->logLn(m_emLogLevel, "Leave %s", m_szStr);
}

}
