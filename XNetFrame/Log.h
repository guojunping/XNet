///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __LOG_H
#define __LOG_H

#include "TypesDn.h"

#include "Properties.h"
#include "Viewer.h"
#include "Monitor.h"
#include "BlockingQueue.h"
#include "LogThread.h"

#include "Profiler.h"

namespace XNetFrame{

typedef list<CViewer*> ViewerList;
typedef list<TSTRING>  FilterList;
// 

#define VAR_TO_STR(__szDes,__szFormat,__uiSize) \
TCHAR __szString[__uiSize]; \
va_list __args; \
va_start(__args, __szFormat); \
int __iResult = _vsntprintf(__szString, __uiSize - 1, __szFormat, __args); \
if(__iResult == -1 || __iResult == __uiSize - 1) \
{ \
	__szString[__uiSize-1] = 0x00; \
}; \
va_end(__args); \
__szDes = __szString

#define LOG_SERVICE_CLASS			(_T("LogService"))

#define LOG_LEVEL_PROPERTY			(_T("LogLevel"))

class CLog{ // 
public:
	enum ELogResult { LR_NULL, LR_OK, LR_ALREADY_EXIST, LR_NOT_CONNECTED };
	enum ELogLevel { LL_0, LL_1, LL_2, LL_3 };

	const static uint32 MAX_LINE_LEN	= 1024;

private:
	CMonitor   m_cMonitor;

	CBlockingQueue<TSTRING> m_cBlockingQueue;

	CLogThread*     m_pLogThread;

	CProperties*	m_pProperties;

	ViewerList m_liViewers;       //  
	ViewerList m_liBypassViewers; // 

	FilterList m_liPositiveFilters; // 
	FilterList m_liNegativeFilters; // 

	ELogLevel m_emLogLevel;

private:
	void       logString(LPTSTR);        // 
	bool       isPassableFilter(LPTSTR); // 

public:
	CLog(void);
	virtual ~CLog(void);
	
	void        start(void);
	uint32      countViewer(void);                // 
	CViewer*    getViewer(LPTSTR);                // 
	ELogResult  deleteViewer(LPTSTR);             // 
	ELogResult  deleteViewer(CViewer*);           // 
	ELogResult  addViewer(CViewer*, bool = true); // 

	ELogResult  isConnected(LPTSTR);   // 
	ELogResult  isConnected(CViewer*); // 

	void        dumpFilters(void);         // 
	void        deleteFilter(LPTSTR);      // 
	ELogResult  addPositiveFilter(LPTSTR); // 
	ELogResult  addNegativeFilter(LPTSTR); // 

	void		log(ELogLevel, LPTSTR szFormat, ...);   // 
	void		logLn(ELogLevel, LPTSTR szFormat, ...); // 
	void		dumpMem(char*, uint32);
	ELogResult setProperties(CProperties*);

	void        logOutput(void);

	void        deleteAll(void); // 
};

class CFunctionLog{
private:
	CLog*	m_pLog;
	CLog::ELogLevel m_emLogLevel;
	TCHAR	m_szStr[CLog::MAX_LINE_LEN];
	CProfiler*	m_pProfiler;
public:
	CFunctionLog(CLog*, CLog::ELogLevel, CProfiler*, LPTSTR szFormat, ...);
	~CFunctionLog(void);
};

}

#endif
