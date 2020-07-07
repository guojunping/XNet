#ifndef __CONT_DISPATCH_SERVICE_H
#define __CONT_DISPATCH_SERVICE_H

#include "DispatchService.h"

namespace XNetFrame{
	
class CContextDispatchService : public CDispatchService{
public:
	explicit CContextDispatchService(CLog*, sint32, bool); // 
	~CContextDispatchService(void);
	
	bool dispatchContext(void);  // 
	bool deliveryContext(void); //
	
	virtual void onCreateService(void) {};
	virtual void onDestroyService(void){};
};
}



#endif