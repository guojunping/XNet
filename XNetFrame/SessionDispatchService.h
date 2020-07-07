#ifndef __SESS_DISPATCH_SERVICE_H
#define __SESS_DISPATCH_SERVICE_H

#include "DispatchService.h"

namespace XNetFrame{
	
class CSessionDispatchService : public CDispatchService{
public:
	explicit CSessionDispatchService(CLog*, sint32, bool); // 
	~CSessionDispatchService(void);
	
	bool  dispatchSession(void);  // 
	
	uint32 systemMessageHandler(void*, void*, void*);
	
	virtual void onCreateService(void) {};
	virtual void onDestroyService(void){};
};
}
#endif