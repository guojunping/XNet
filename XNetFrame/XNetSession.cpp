///////////////////////////////////////
// Author : Hank Lee
// Name : GameSession.cpp
///////////////////////////////////////

#include "XNetSession.h"

#include "InetAddress.h"
#include "XNetService.h"

namespace XNetFrame{

CXNetSession::CXNetSession(CXNetService *pXNetService,CLog* pLog) 
: CSession(pLog),m_pXNetService(pXNetService){

}

CXNetSession::~CXNetSession(void){
}

void CXNetSession::onConnect(void){
	CInetAddress* pInetAddress;
	
	pInetAddress = CSession::getRemoteAddress();

	if(pInetAddress != NULL && pInetAddress->isValid() == true) 
		m_pLog->logLn(CLog::LL_0, "ssesion object (%s) connected", pInetAddress->toString().c_str()); //
	else 
		m_pLog->logLn(CLog::LL_0, "session object connected"); //
	
    m_pXNetService->onSessionConnect(this);
	
}

bool CXNetSession::onClose(CSession::SCloseStatus csCloseStatus){

	m_pLog->logLn(CLog::LL_2,"[%u][%lu] onClose CloseStatus=%lu ",getSessionId(),::GetCurrentThreadId(),csCloseStatus);
    
	return m_pXNetService->onSessionClose(this,csCloseStatus);
}

bool CXNetSession::SendPacket(void *pData, uint32 uiLen)
{
	CSession::ESessionResult srResult;
	srResult = doSend((char*)pData, uiLen);
	if(getState() != SS_CONNECTED)
		return false;
	if(srResult != CSession::SR_OK && srResult != CSession::SR_WOULD_BLOCK)
	{
		doClose(CSession::CS_SEND_FAILURE);
		m_pLog->logLn(CLog::LL_0, "error(%d) line(%d) func(%s)", srResult, __LINE__, __FUNCTION__);	
		return false;
	}

	return true;
}

}
