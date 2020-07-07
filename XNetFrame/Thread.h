/**
 * Thread operation
 * 
 */
#ifndef __THREAD_H
#define __THREAD_H

#include "TypesDn.h"

namespace XNetFrame{

class CThread{
public:
	enum EThreadResult   { TR_NULL, TR_FAILURE, TR_ALREADY_RUNNING, TR_OK };
	enum EThreadPriority { TP_HIGHEST = 0, TP_ABOVE_NORMAL, TP_NORMAL, TP_BELOW_NORMAL, TP_LOWEST };

private:
	string  m_sName;       // 
	CThread* m_pThread;     // 
	uint32   m_uiThreadId;  // 
	bool     m_bThreadFlag; // 

	uint32   m_uiErrorCode; // 
	
	Thread_t   m_hThread;
	
#ifndef _WIN32
	sem_t	 m_hEvent;
#endif

private:
#ifdef _WIN32	
	static DWORD WINAPI __run(LPVOID); // thread routine
#else
	static void* __run(void*); // thread routine
#endif
protected:
	virtual void run(void) = 0;

public:
	CThread(void);
	CThread(const char*);
	virtual ~CThread(void); // 

	EThreadResult wait(void);    // 
	EThreadResult start(void);   // 
	EThreadResult resume(void);  // 
	EThreadResult suspend(void); // 
	
	EThreadResult stop(void);     // 

	EThreadResult stop(IOEVENT); // 
	
	string        getName(void);
	void          setName(const char*);

	uint32        getErrorCode(void); // 

	CThread*      currentThread(void); // 

	bool          getThreadFlag(void); // 
	void          setThreadFlag(bool); // 

	EThreadPriority getPriority(void);            // 
	EThreadResult   setPriority(EThreadPriority); // 
};

}

#endif
