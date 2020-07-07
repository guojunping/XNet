///////////////////////////////////////
// 与ChatServer的socket连接通道
// 基于 TCP socket
///////////////////////////////////////

#include "XNetChannel.h"
#include "XNetConnector.h"

namespace XNetFrame{

CXNetChannel::CXNetChannel(CXNetConnector* pXNetConnector,CLog* pLog) 
: CChannel(pLog),m_pXNetConnector(pXNetConnector){

}

CXNetChannel::~CXNetChannel(void){
}

void CXNetChannel::onConnect(void)
{
    m_pLog->logLn(CLog::LL_0, "XNetChannel object connected");
	m_pXNetConnector->onChannelConnect(this);
}

void CXNetChannel::onClose(CChannel::SCloseStatus csCloseStatus){

	m_pLog->logLn(CLog::LL_0, "XNetChannel OnClose : csCloseStatus = %d", csCloseStatus);
	m_pXNetConnector->onChannelClose(this,csCloseStatus);
}

bool CXNetChannel::SendPacket(void *pData, uint32 uiLen)
{
	if(getState() != CS_CONNECTED)
		return false;

	CChannel::EChannelResult crResult;
	crResult = doSend((char*)pData, uiLen);	
	if(crResult != CChannel::CR_OK && crResult != CChannel::CR_WOULD_BLOCK)
	{	
		doClose(CChannel::CS_SEND_FAILURE) ; 
		m_pLog->logLn(CLog::LL_0, "error(%d) line(%d) func(%s)", crResult, __LINE__, __FUNCTION__);	 	
		return false ; 
	}

	return true ; 
}

}
