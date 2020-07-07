///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "DispatchService.h"
#include "DispatchThread.h"
#include "MessageHandler.h"
namespace XNetFrame{

CDispatchService::CDispatchService(CLog* pLog, sint32 siDispatchMode, bool bDeliveryMode) : CMessageHandlerFactory(){
	m_pLog        = pLog;

	m_siRefCount  = 0;
	m_uiHeadPos   = 0;
	m_uiTailPos   = 0;
	m_uiUsedCount = 0;

	m_bDeliveryMode  = bDeliveryMode;  // 
	m_siDispatchMode = siDispatchMode; // 
}

CDispatchService::~CDispatchService(void){
	stopService();
}

CDispatchService::EDispatchServiceResult CDispatchService::createService(void){ // 
#ifdef WIN32
	CEvent::EEventResult erResult;
	erResult = m_IoEvent.reset();
	if(erResult != CEvent::ER_OK) return DSR_FAILURE;
#endif
	m_uiHeadPos   = 0;
	m_uiTailPos   = 0;
	m_uiUsedCount = 0;

	return DSR_OK;
}

CDispatchService::EDispatchServiceResult CDispatchService::destroyService(void){ // 
	CMessageHandlerFactory::deleteAll(); // 

	return DSR_OK;
}

void CDispatchService::setErrorCode(CDispatchService::EDispatchServiceResult dsrResult){
	m_dsrResult = dsrResult;
}

CDispatchService::EDispatchServiceResult CDispatchService::startService(void){ // 
	CDispatchThread*       pDispatchThread;
	EDispatchServiceResult dsrResult;

	m_siRefCount++;

	dsrResult = createService();
	if(dsrResult != DSR_OK) return dsrResult;

	setErrorCode(DSR_OK);

	onCreateService();

	if(m_dsrResult != DSR_OK) return m_dsrResult;

	if(m_bDeliveryMode == false){ // 
		pDispatchThread = new CDispatchThread(this); // 
		if(pDispatchThread == NULL) return DSR_NULL;

		m_liDispatchThreadList.push_back((DispatchThreadList::value_type)pDispatchThread);

		pDispatchThread->setPriority(CThread::TP_NORMAL);

		if(pDispatchThread->start() != CThread::TR_OK) return DSR_THREAD_START_FAILURE;
	};

	return DSR_OK;
}

CDispatchService::EDispatchServiceResult CDispatchService::stopService(void){ // 
	CDispatchThread*              pDispatchThread;
	EDispatchServiceResult        dsrResult;
	DispatchThreadList::iterator  itr;

	if(m_siRefCount == 0) return DSR_OK; // 

	m_siRefCount--;

	if(m_bDeliveryMode == false){ // 
		for(itr = m_liDispatchThreadList.begin(); itr != m_liDispatchThreadList.end(); itr++){ // 
			pDispatchThread = (CDispatchThread*)(*itr);
			pDispatchThread->stop(m_IoEvent.getEvent()); // 

			delete pDispatchThread;
			pDispatchThread = NULL;
		}

		itr = m_liDispatchThreadList.begin();
		if(itr != m_liDispatchThreadList.end()) m_liDispatchThreadList.erase(itr, m_liDispatchThreadList.end());
	}

	onDestroyService();

	dsrResult = destroyService();
	if(dsrResult != DSR_OK) return dsrResult;

	return DSR_OK;
}
/*
CDispatchService::EDispatchServiceResult CDispatchService::addObject(void* pObject, bool bExFlag){ // 
	CEvent::EEventResult erResult;

	synchronized sync(&m_cMonitor);

	if(pObject == NULL) return DSR_NULL;
	if(m_uiUsedCount == MAX_OBJECT_QUEUE_LEN) return DSR_OBJECT_QUEUE_OVERFLOW;

	m_oqObjectQueue[m_uiTailPos]   = pObject;
	m_fqExFlagQueue[m_uiTailPos++] = bExFlag;

	if(m_uiTailPos == MAX_OBJECT_QUEUE_LEN) m_uiTailPos = 0; // 

	m_uiUsedCount++;
#ifdef LOG_LEVEL_2
	m_pLog->logLn(CLog::LL_3, "DispatchService addObject UsedCount = %d", m_uiUsedCount);
#endif
	erResult = m_IoEvent.set();
	if(erResult != CEvent::ER_OK) return DSR_FAILURE;

	return DSR_OK;
}
*/

CDispatchService::EDispatchServiceResult CDispatchService::addObject(void* pObject, bool bExFlag, time_t t){ // 
	CEvent::EEventResult erResult;

	synchronized sync(&m_cMonitor);

	if(pObject == NULL) return DSR_NULL;
	if(m_uiUsedCount == MAX_OBJECT_QUEUE_LEN) return DSR_OBJECT_QUEUE_OVERFLOW;

	m_oqObjectQueue[m_uiTailPos]   = pObject;
	m_startTimeQueue[m_uiTailPos]   = t;
	m_fqExFlagQueue[m_uiTailPos++] = bExFlag;

	if(m_uiTailPos == MAX_OBJECT_QUEUE_LEN) m_uiTailPos = 0; // 

	m_uiUsedCount++;
#ifdef LOG_LEVEL_2
	m_pLog->logLn(CLog::LL_3, "DispatchService addObject UsedCount = %d", m_uiUsedCount);
#endif
	erResult = m_IoEvent.set();
	
#ifndef WIN32
	//sint32 val =m_IoEvent.getEventVal();
	//printf("set semval=%d\n",val);
#endif
		
	if(erResult != CEvent::ER_OK) return DSR_FAILURE;

	return DSR_OK;
}

/*
void* CDispatchService::getObject(bool& bExFlag){ // 
	void* pObject;

	synchronized sync(&m_cMonitor);

	if(m_uiUsedCount == 0){ // 
#ifdef WIN32
		m_IoEvent.reset();
#endif
		return NULL;
	};

	pObject = m_oqObjectQueue[m_uiHeadPos];
	bExFlag = m_fqExFlagQueue[m_uiHeadPos++];

	if(m_uiHeadPos == MAX_OBJECT_QUEUE_LEN) m_uiHeadPos = 0; // 

	m_uiUsedCount--;

	return pObject;
}
*/

void* CDispatchService::getObject(bool& bExFlag, time_t& t){ // 
	void* pObject;

	synchronized sync(&m_cMonitor);

	if(m_uiUsedCount == 0){ // 
#ifdef WIN32
		m_IoEvent.reset();
#else
		//sint32 val =m_IoEvent.getEventVal();
		//printf("get semval=%d\n",val);
#endif
		return NULL;
	};

	pObject = m_oqObjectQueue[m_uiHeadPos];
	t = m_startTimeQueue[m_uiHeadPos];
	bExFlag = m_fqExFlagQueue[m_uiHeadPos++];

	if(m_uiHeadPos == MAX_OBJECT_QUEUE_LEN) m_uiHeadPos = 0; // 

	m_uiUsedCount--;

	return pObject;
}

bool CDispatchService::isOverflowObjectQueue(uint32 uiActivatedSlots){ // 
	if(MAX_OBJECT_QUEUE_LEN <= (m_uiUsedCount + uiActivatedSlots)) return true;
	else return false;
}

}
