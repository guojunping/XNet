///////////////////////////////////////
// Author : Hank Lee
// Name : GameSession.h
///////////////////////////////////////

#ifndef __XNET_SESSION_H
#define __XNET_SESSION_H

#include "Log.h"
#include "Session.h"



namespace XNetFrame{

class CXNetService;
	
class CXNetSession : public CSession{
	
public:

	CXNetSession(CXNetService *,CLog*);
	~CXNetSession(void);

	void onConnect(void);
	bool onClose(CSession::SCloseStatus);
	bool SendPacket(void *pData, uint32 uiLen);
private:
    CXNetService *m_pXNetService;
};

}

#endif
