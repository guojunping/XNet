#ifndef __X_CONNECTOR_H
#define __X_CONNECTOR_H

#include "TcpService.h"
#include "MessageHandler.h"
#include "Channel.h"
#include "Properties.h"


namespace XNetFrame{
	
class CXNetConnector : public CTcpService{
	
public:

  CXNetConnector(CLog*);
  ~CXNetConnector();
  
  bool initService(TCHAR szServerName[20],CProperties*);
  bool startService(void);
  bool stopService(void);
  bool registerHandler(CMessageHandler *pHandler);

  virtual void onCreateService(void);
  virtual void onDestroyService(void);
	
  virtual void onChannelConnect(CChannel*) = 0;
  virtual void onChannelClose(CChannel*,CChannel::SCloseStatus)   = 0;
  
private:

	void callbackCreateChannel(CChannel**);

};
}

#endif