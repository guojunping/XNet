#ifndef __X_SERVICE_H
#define __X_SERVICE_H

#include "TypesDn.h"

#include "EpollService.h"
#include "MessageHandler.h"
#include "Session.h"
#include "Properties.h"

namespace XNetFrame{
	
class CXNetService : public CEpollService{
	
public:

  CXNetService(CLog*);
  ~CXNetService();
  
  bool initService(TCHAR szServerName[20],CProperties*);
  bool startService(void);
  bool stopService(void);
  bool registerHandler(CMessageHandler *pHandler);
  
  virtual void onCreateService(void);
  virtual void onDestroyService(void);
  
  virtual void onSessionConnect(CSession*)                = 0; // 
  virtual bool onSessionClose(CSession*,CSession::SCloseStatus) = 0; // 
  
private:

	void callbackInitSession(CSession**);
	void callbackCreateSession(CSession**);

};
}

#endif