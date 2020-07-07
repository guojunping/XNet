/**
 * thread operation
 * 
 */ 

#include "Thread.h"

namespace XNetFrame{

CThread::CThread(void){
	m_uiThreadId  = 0;
	m_hThread     = 0;
	m_pThread     = NULL;
	m_bThreadFlag = false;
	m_sName       = "";

	m_uiErrorCode = 0;
#ifndef _WIN32
	sem_init(&m_hEvent, 0, 0);
#endif
}

CThread::CThread(const char* szName){
	m_sName = szName;

	m_uiThreadId  = 0;
	m_hThread     = 0;
	m_pThread     = NULL;
	m_bThreadFlag = false;

	m_uiErrorCode = 0;
#ifndef _WIN32
	sem_init(&m_hEvent, 0, 0);
#endif
}

CThread::~CThread(void){ // 
#ifndef _WIN32
	sem_destroy(&m_hEvent);
#endif
}

void CThread::setName(const char* szName){
	m_sName = szName;
}

string CThread::getName(void){
	return m_sName;
}

uint32 CThread::getErrorCode(void){ // 
	return m_uiErrorCode;
}

CThread::EThreadResult CThread::resume(void){ // 
	if(m_hThread == 0) return TR_NULL;
	
#ifdef _WIN32
	if(::ResumeThread(m_hThread) == -1){
		m_uiErrorCode = (uint32)::GetLastError();
		return TR_FAILURE;
	}
#else
	sem_post(&m_hEvent);
#endif
	
	return TR_OK;
}

CThread::EThreadResult CThread::suspend(void){ // 
	if(m_hThread == 0) return TR_NULL;
	
#ifdef _WIN32
	if(::SuspendThread(m_hThread) == -1){
		m_uiErrorCode = (uint32)::GetLastError();
		return TR_FAILURE;
	}
#else
	while(sem_trywait(&m_hEvent) == 0 || errno == EINTR);
#endif

	return TR_OK;
}

CThread::EThreadResult CThread::stop(void){ // 
	if(m_hThread == 0) return TR_NULL;

	setThreadFlag(false); // 

#ifdef _WIN32
	if(::WaitForSingleObject(m_hThread, 5000) == WAIT_TIMEOUT){
		::TerminateThread(m_hThread, 0);
	};

	::CloseHandle(m_hThread);
#else
	void* lpv;
	sem_post(&m_hEvent);
	pthread_join(m_hThread, &lpv);
#endif

	m_uiThreadId = 0;
	m_hThread    = 0;
	m_pThread    = NULL;
	
	return TR_OK;
}

CThread::EThreadResult CThread::stop(IOEVENT hIoEvent){ // 
	if(m_hThread == 0) return TR_NULL;

	setThreadFlag(false); // 

#ifdef _WIN32
	if(hIoEvent != WSA_INVALID_EVENT) ::SetEvent(hIoEvent); // 

	if(::WaitForSingleObject(m_hThread, 5000) == WAIT_TIMEOUT){
		::TerminateThread(m_hThread, 0);
	};

	::CloseHandle(m_hThread);
#else
	void* lpv;
	if(hIoEvent==NULL)
	{
		int ret=pthread_kill(m_hThread,0);
		if(ret  != ESRCH && ret != EINVAL )
			pthread_kill(m_hThread,SIGUSR1);
	}
	else
	{
		sem_post(hIoEvent);
	}
	pthread_join(m_hThread, &lpv);
#endif

	m_uiThreadId = 0;
	m_hThread    = 0;
	m_pThread    = NULL;
	
	return TR_OK;
}

CThread::EThreadResult CThread::wait(void){ //
	if(m_hThread == 0) return TR_NULL;
	
#ifdef _WIN32
	if(::WaitForSingleObject(m_hThread, INFINITE) == WAIT_FAILED){
		m_uiErrorCode = (uint32)::GetLastError();
		return TR_FAILURE;
	}
#else
	while(sem_wait(&m_hEvent) != 0 && errno == EINTR);
#endif

	return TR_OK;
}

CThread::EThreadResult CThread::start(void){ // 
	if(m_hThread != 0) return TR_ALREADY_RUNNING;

#ifdef _WIN32
	m_hThread = (HANDLE)::_beginthreadex(NULL, 0, CThread::__run, this, 0, &m_uiThreadId);
	if(m_hThread == NULL){
		m_uiErrorCode = (uint32)::GetLastError();
		return TR_FAILURE;
	}
#else
	if(pthread_create(&m_hThread, NULL, CThread::__run, this) != 0)
	{
		// error
		m_uiErrorCode = errno;
		return TR_FAILURE;
	}
#endif

	return TR_OK;
}

bool CThread::getThreadFlag(void){ // 
	return m_bThreadFlag;
}

void CThread::setThreadFlag(bool bThreadFlag){ // 
	m_bThreadFlag = bThreadFlag;
}

void* CThread::__run(void* pThis){ // 
	CThread* pThread;

	pThread            = (CThread*)pThis;
	pThread->m_pThread = (CThread*)pThis;

	pThread->setThreadFlag(true); // 

   #ifndef WIN32
    sigset_t signal_mask ;
	sigemptyset (&signal_mask);
	sigaddset (&signal_mask, SIGPIPE);
	sigaddset (&signal_mask, SIGQUIT);
	sigaddset (&signal_mask, SIGINT);
	sigaddset (&signal_mask, SIGTERM);
	sigaddset (&signal_mask, SIGALRM);
	pthread_sigmask(SIG_BLOCK, &signal_mask, NULL);
   #endif
   
	pThread->run();
	
	printf("thread[%lu] exited \n",pthread_self());

	return 0;
}

CThread* CThread::currentThread(void){ // 
	return m_pThread;
}

CThread::EThreadPriority CThread::getPriority(void){ // 
#ifdef WIN32
	sint32 siPriority;
#endif

	if(m_hThread == 0) return TP_NORMAL;

#ifdef _WIN32
	siPriority = ::GetThreadPriority(m_hThread);
	if(siPriority == THREAD_PRIORITY_ERROR_RETURN){
		m_uiErrorCode = (uint32)::GetLastError();
		return TP_NORMAL;
	};
	
	switch(siPriority){
		case THREAD_PRIORITY_HIGHEST      : return TP_HIGHEST;
		case THREAD_PRIORITY_ABOVE_NORMAL : return TP_ABOVE_NORMAL;
		case THREAD_PRIORITY_BELOW_NORMAL : return TP_BELOW_NORMAL;
		case THREAD_PRIORITY_LOWEST       : return TP_LOWEST;
		default                           : return TP_NORMAL;
	};
#endif
	return TP_NORMAL;
}

CThread::EThreadResult CThread::setPriority(EThreadPriority emThreadPriority){ // 
#ifdef WIN32
	sint32 siPriority;
#endif
	if(m_hThread == 0) return TR_NULL;

#ifdef WIN32
	switch(emThreadPriority){
		case TP_HIGHEST      : siPriority = THREAD_PRIORITY_HIGHEST;      break;
		case TP_ABOVE_NORMAL : siPriority = THREAD_PRIORITY_ABOVE_NORMAL; break;
		case TP_NORMAL       : siPriority = THREAD_PRIORITY_NORMAL;       break;
		case TP_BELOW_NORMAL : siPriority = THREAD_PRIORITY_BELOW_NORMAL; break;
		case TP_LOWEST       : siPriority = THREAD_PRIORITY_LOWEST;       break;
	};

	if(::SetThreadPriority(m_hThread, siPriority) == 0){
		m_uiErrorCode = (uint32)::GetLastError();
		return TR_FAILURE;
	}
#endif

	return TR_OK;
}

}
