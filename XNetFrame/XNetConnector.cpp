#include "XNetConnector.h"
#include "XNetChannel.h"

namespace XNetFrame{

CXNetConnector::CXNetConnector(CLog* pLog):CTcpService(pLog)
{
}

CXNetConnector::~CXNetConnector()
{
	stopService();
}

bool CXNetConnector::initService(TCHAR szServerName[20],CProperties* pProperties)
{ //
	
	//setServerName(szServerName); //
	
	if(setProperties(pProperties, szServerName) != CTcpService::TSR_OK){ // 
		return false;
	}

	return true;
}

bool CXNetConnector::startService(void){ //

	CTcpService::ETcpServiceResult tsrResult;

	tsrResult = CTcpService::startService(); //

	if(tsrResult == CTcpService::TSR_CHANNEL_CREATE_FAILURE || tsrResult == CTcpService::TSR_CHANNEL_INIT_FAILURE){
		return false;
	}

	if(tsrResult == CTcpService::TSR_NULL){
		return false;
	}

	if(tsrResult == CTcpService::TSR_THREAD_START_FAILURE){
		return false;
	}

	if(tsrResult == CTcpService::TSR_CHANNEL_CONNECT_FAILURE){
		return false;
	}

    m_pLog->logLn(CLog::LL_0, "connector started normally"); //


	return true;
}

bool CXNetConnector::stopService(void){ //
    CTcpService::ETcpServiceResult tsrResult;

	tsrResult = CTcpService::stopService(); //

	if(tsrResult != CTcpService::TSR_OK){
		return false;
	}
	
	m_pLog->logLn(CLog::LL_0, "connector closed normally"); //

	return true;
}


bool CXNetConnector::registerHandler(CMessageHandler *pHandler)
{
	CDispatchService* pDispatchServie = getDispatchService();
	if(NULL==pDispatchServie)
		return false;
	if(!pDispatchServie->addMessageHandler(pHandler))
		return false;
		
	return true;
}

void CXNetConnector::onCreateService(void){
	setErrorCode(CTcpService::TSR_OK);
}

void CXNetConnector::onDestroyService(void){
	
}

void CXNetConnector::callbackCreateChannel(CChannel** pChannel){
	CXNetChannel* pXNetChannel;

	pXNetChannel = new CXNetChannel(this,m_pLog); // 
	*pChannel    = (CChannel*)pXNetChannel;  // 
}

}
