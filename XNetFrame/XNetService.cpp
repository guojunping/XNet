#include "XNetService.h"
#include "XNetSession.h"

namespace XNetFrame{

CXNetService::CXNetService(CLog* pLog):CEpollService(pLog)
{
}

CXNetService::~CXNetService()
{
	stopService();
}

bool CXNetService::initService(TCHAR szServerName[20],CProperties* pProperties)
{ //
	
	setServerName(szServerName); //
	
	if(setProperties(pProperties, szServerName) != CEpollService::ISR_OK){ // 
		return false;
	}

	return true;
}

bool CXNetService::startService(void){ //

	CEpollService::EEpollServiceResult isrResult;

	isrResult = CEpollService::startService(); //

	if(isrResult == CEpollService::ISR_SESSION_CREATE_FAILURE 
		|| isrResult == CEpollService::ISR_SESSION_INIT_FAILURE 
		|| isrResult == CEpollService::ISR_SESSION_ADD_FAILURE){
		return false;
	}
	
	if(isrResult == CEpollService::ISR_NULL){
		return false;
	}

	if(isrResult == CEpollService::ISR_THREAD_START_FAILURE){
		return false;
	}

	if(isrResult != CEpollService::ISR_OK){
		return false;
	}

	m_pLog->logLn(CLog::LL_0, "session objects (number = %d) created", (sint32)getMaxSession()); //                                           //
	m_pLog->logLn(CLog::LL_0, "service started normally"); //

	return true;
}

bool CXNetService::stopService(void){ //
	CEpollService::EEpollServiceResult isrResult;

	isrResult = CEpollService::stopService(); //

	if(isrResult != CEpollService::ISR_OK){
		return false;
	}

	m_pLog->logLn(CLog::LL_0, "service closed normally"); //

	return true;
}


bool CXNetService::registerHandler(CMessageHandler *pHandler)
{
	uint32 uiDispatchCount=getNumDispatch();
	for(int i=0;i<uiDispatchCount;i++)
    {
		CDispatchService* pDispatchService=getDispatchService(i);
		if(!pDispatchService->addMessageHandler(pHandler))
			return false;
	}
	return true;
	
}

void CXNetService::onCreateService(void){
	setErrorCode(CEpollService::ISR_OK);
}

void CXNetService::onDestroyService(void){
}

void CXNetService::callbackInitSession(CSession** pSession){
}

void CXNetService::callbackCreateSession(CSession** pSession){
	CXNetSession* pXNetSession;

	pXNetSession = new CXNetSession(this,m_pLog); // 
	*pSession    = (CSession*)pXNetSession;  // 
}

}

